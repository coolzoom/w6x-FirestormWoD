#include "instance_skyreach.h"

namespace MS
{
    namespace InstanceSkyreach
    {
        class mob_SkyreachArcanologist : public CreatureScript
        {
        public:
            // Entry: 76376
            mob_SkyreachArcanologist()
                : CreatureScript("mob_SkyreachArcanologist")
            {
            }

            enum class Spells : uint32
            {
                SOLAR_DETONATION = 160288, // 7 seconds.
                SOLAR_STORM = 159215, // 12 to 15 seconds.
                SOLAR_STORM_1 = 159216,
                SOLAR_STORM_2 = 159218,
                SOLAR_STORM_3 = 159221,
                SOLAR_STORM_DMG = 159226,
            };

            enum class Events : uint32
            {
                SOLAR_DETONATION = 1,
                SOLAR_STORM = 2,
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_SkyreachArcanologistAI(creature);
            }

            struct mob_SkyreachArcanologistAI : public ScriptedAI
            {
                mob_SkyreachArcanologistAI(Creature* creature) : ScriptedAI(creature),
                m_instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();

                    if (m_instance)
                        m_instance->SetData(Data::SkyreachArcanologistReset, 0);
                }

                void JustDied(Unit*)
                {
                    if (m_instance)
                        m_instance->SetData(Data::SkyreachArcanologistIsDead, 0);
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::SOLAR_DETONATION), urand(2500, 7500));
                    m_events.ScheduleEvent(uint32(Events::SOLAR_STORM), urand(5000, 10000));
                }

                void UpdateAI(const uint32 diff)
                {
                    if (!UpdateVictim())
                        return;

                    m_events.Update(diff);

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;

                    switch (m_events.ExecuteEvent())
                    {
                    case uint32(Events::SOLAR_DETONATION):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_DETONATION), urand(6500, 7500));
                        if (Player* l_Plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 15.0f))
                            me->CastSpell(l_Plr, uint32(Spells::SOLAR_DETONATION));
                        break;
                    case uint32(Events::SOLAR_STORM):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_STORM), urand(12000, 15000));
                        me->CastSpell(me->getVictim(), uint32(Spells::SOLAR_STORM));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_instance;
                EventMap m_events;
            };
        };

        class boss_Araknath : public CreatureScript
        {
        public:
            // Entry: 76141
            boss_Araknath()
                : CreatureScript("boss_Araknath")
            {
            }

            enum class Spells : uint32
            {
                MELEE = 154121, // Every 2 seconds.
                SMASH = 154113, // Every 6 to 7 seconds.
                BURST = 154135, // Every 23 seconds.
                BREATH_OF_SINDRAGOSA = 155168,
                SUBMERGE = 154164,
                SUBMERGED = 154163,
            };

            enum class Events : uint32
            {
                SHUT_DOORS = 1,
                MELEE = 2,
                SMASH = 3,
                BURST = 4,
                ENERGIZE_START = 5,
                ENERGIZE_STOP = 6
            };

            ScriptedAI* GetAI(Creature* creature) const
            {
                return new boss_AraknathAI(creature);
            }

            struct boss_AraknathAI : public BossAI
            {
                boss_AraknathAI(Creature* creature) : BossAI(creature, Data::Araknath)
                {
                }

                void Reset()
                {
                    _Reset();

                    me->AddAura(uint32(Spells::SUBMERGED), me);
                    me->SetReactState(REACT_PASSIVE);
                }

                void JustDied(Unit* /*p_Killer*/)
                {
                    _JustDied();
                }

                void KilledUnit(Unit* /*p_Victim*/)
                {
                }

                void JustReachedHome()
                {
                    _JustReachedHome();

                    if (instance)
                    {
                        instance->SetBossState(Data::Araknath, FAIL);
                        instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    }
                }

                void EnterCombat(Unit* who)
                {
                    _EnterCombat();
                    /*events.ScheduleEvent(uint32(Events::MELEE), 2000);
                    events.ScheduleEvent(uint32(Events::SMASH), urand(5500, 7000));
                    events.ScheduleEvent(uint32(Events::BURST), urand(21500, 23000));*/
                    events.ScheduleEvent(uint32(Events::ENERGIZE_START), 12000);
                }

                void UpdateAI(const uint32 diff)
                {
                    if (!UpdateVictim())
                        return;

                    events.Update(diff);

                    if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;

                    switch (events.ExecuteEvent())
                    {
                    case uint32(Events::MELEE):
                        events.ScheduleEvent(uint32(Events::MELEE), 2000);
                        me->CastSpell(me->getVictim(), uint32(Spells::MELEE));
                        break;
                    case uint32(Events::SMASH):
                        events.ScheduleEvent(uint32(Events::SMASH), urand(5500, 7000));
                        me->CastSpell(me->getVictim(), uint32(Spells::SMASH));
                        break;
                    case uint32(Events::BURST):
                        events.ScheduleEvent(uint32(Events::BURST), urand(21500, 23000));
                        me->CastSpell(me->getVictim(), uint32(Spells::BURST));
                        break;
                    case uint32(Events::ENERGIZE_START):
                        events.ScheduleEvent(uint32(Events::ENERGIZE_STOP), 12000);

                        if (instance)
                            instance->SetData(Data::AraknathSolarConstructorActivation, true);
                        break;
                    case uint32(Events::ENERGIZE_STOP):
                        events.ScheduleEvent(uint32(Events::ENERGIZE_START), 3000);

                        if (instance)
                            instance->SetData(Data::AraknathSolarConstructorActivation, false);
                        break;
                    default:
                        break;
                    }
                }
            };
        };
    }
}

void AddSC_boss_Araknath()
{
    new MS::InstanceSkyreach::mob_SkyreachArcanologist();
    new MS::InstanceSkyreach::boss_Araknath();
}