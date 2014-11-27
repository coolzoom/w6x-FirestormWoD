#include "instance_skyreach.h"

/*
 TODO: There seems to have other spells to cast here. For example:
 - Magma explosion supreme
 - Sun trinckets <- Need modelid and spell to cast. It's healing normaly.
 - Air trinckets
 - Cyclone trinckets ?
 - Do visual stuff when no one is attacking.
 - Need to see if the timers are good enough.
 */
namespace MS
{
    namespace InstanceSkyreach
    {
        class mob_SkyreachSunTalon : public CreatureScript
        {
        public:
            // Entry: 79093
            mob_SkyreachSunTalon()
                : CreatureScript("mob_SkyreachSunTalon")
            {
            }

            enum class Spells : uint32
            {
                EXPOSE_WEAKNESS = 152982,
                INVISIBILITY_DETECTION = 141048,
                FIXATED = 152838,
            };

            enum class Events : uint32
            {
                CHANGE_TARGET = 1, // Every 6 seconds.
                EXPOSE_WEAKNESS = 2, // Every 10 seconds.
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_SkyreachSunTalonAI(creature);
            }

            struct mob_SkyreachSunTalonAI : public ScriptedAI
            {
                mob_SkyreachSunTalonAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::CHANGE_TARGET), 6000);
                    m_events.ScheduleEvent(uint32(Events::EXPOSE_WEAKNESS), 10000);
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
                    case uint32(Events::CHANGE_TARGET):
                        m_events.ScheduleEvent(uint32(Events::CHANGE_TARGET), 6000);
                        me->CastSpell(me, uint32(Spells::INVISIBILITY_DETECTION), true);
                        me->CastSpell(me->getVictim(), uint32(Spells::FIXATED), true);
                        me->getThreatManager().resetAllAggro();

                        if (Player* l_Plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 30.0f))
                        {
                            me->getThreatManager().addThreat(l_Plr, 1000.0f);
                            me->Attack(l_Plr, true);
                        }
                        break;
                    case uint32(Events::EXPOSE_WEAKNESS):
                        m_events.ScheduleEvent(uint32(Events::EXPOSE_WEAKNESS), 10000);
                        me->CastSpell(me->getVictim(), uint32(Spells::EXPOSE_WEAKNESS));
                        break;
                    default:
                        break;
                    }


                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_AdornedBladetalon : public CreatureScript
        {
        public:
            // Entry: 79303
            mob_AdornedBladetalon()
                : CreatureScript("mob_AdornedBladetalon")
            {
            }

            enum class Spells : uint32
            {
                SLASH = 158083,
                PIERCING_RUSH = 138175,
            };

            enum class Events : uint32
            {
                SLASH = 1, // Every 2 to 5 seconds.
                PIERCING_RUSH = 2, // Every 2 to 5 seconds.
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_AdornedBladetalonAI(creature);
            }

            struct mob_AdornedBladetalonAI : public ScriptedAI
            {
                mob_AdornedBladetalonAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::SLASH), urand(0, 4000));
                    m_events.ScheduleEvent(uint32(Events::PIERCING_RUSH), urand(5000, 7000));
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
                    case uint32(Events::SLASH):
                        m_events.ScheduleEvent(uint32(Events::SLASH), urand(2500, 5000));
                        me->CastSpell(me->getVictim(), uint32(Spells::SLASH));
                        break;
                    case uint32(Events::PIERCING_RUSH):
                        m_events.ScheduleEvent(uint32(Events::PIERCING_RUSH), urand(2500, 5000));

                        if (Player* l_Plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 20.0f))
                            me->CastSpell(l_Plr, uint32(Spells::PIERCING_RUSH));
                        break;
                    default:
                        break;
                    }


                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_DrivingGaleCaller : public CreatureScript
        {
        public:
            // Entry: 78932
            mob_DrivingGaleCaller()
                : CreatureScript("mob_DrivingGaleCaller")
            {
            }

            enum class Spells : uint32
            {
                DERVISH = 153905,
            };

            enum class Events : uint32
            {
                DERVISH = 1, // 20 seconds duration so heavy respawn.
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_DrivingGaleCallerAI(creature);
            }

            struct mob_DrivingGaleCallerAI : public ScriptedAI
            {
                mob_DrivingGaleCallerAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::DERVISH), urand(5000, 7000));
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
                    case uint32(Events::DERVISH):
                        m_events.ScheduleEvent(uint32(Events::DERVISH), urand(5000, 7000));
                        me->CastSpell(me->getVictim(), uint32(Spells::DERVISH));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_AdeptOfTheDawn : public CreatureScript
        {
        public:
            // Entry: 79467
            mob_AdeptOfTheDawn()
                : CreatureScript("mob_AdeptOfTheDawn")
            {
            }

            enum class Spells : uint32
            {
                CRAFT_SUN_TRINCKET = 153521,
                FLASH_HEAL = 152894,
            };

            enum class Events : uint32
            {
                CRAFT_SUN_TRINCKET = 1, // 20 seconds duration so heavy respawn.
                FLASH_HEAL = 2          // Every 2 to 5 seconds.
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_AdeptOfTheDawnAI(creature);
            }

            struct mob_AdeptOfTheDawnAI : public ScriptedAI
            {
                mob_AdeptOfTheDawnAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    // FIXME: Script Sun trinket.
                    //m_events.ScheduleEvent(uint32(Events::CRAFT_SUN_TRINCKET), urand(3000, 5000));
                    m_events.ScheduleEvent(uint32(Events::FLASH_HEAL), urand(5000, 8000));
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
                    case uint32(Events::CRAFT_SUN_TRINCKET):
                        m_events.ScheduleEvent(uint32(Events::CRAFT_SUN_TRINCKET), urand(20000, 25000));
                        me->CastSpell(me, uint32(Spells::CRAFT_SUN_TRINCKET));
                        break;
                    case uint32(Events::FLASH_HEAL):
                        m_events.ScheduleEvent(uint32(Events::FLASH_HEAL), urand(2500, 4500));
                        me->CastSpell(me, uint32(Spells::FLASH_HEAL));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_SkyReachOverlord : public CreatureScript
        {
        public:
            // Entry: 75975
            mob_SkyReachOverlord()
                : CreatureScript("mob_SkyreachOverlord")
            {
            }

            enum class Spells : uint32
            {
                TORN_FEATHERS = 153178,
                BEAT_LOW_BORN_DNT = 178635, // Visual when not attacking.
            };

            enum class Events : uint32
            {
                TORN_FEATHERS = 1,
                BEAT_LOW_BORN_DNT = 2
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_SkyReachOverlordAI(creature);
            }

            struct mob_SkyReachOverlordAI : public ScriptedAI
            {
                mob_SkyReachOverlordAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::TORN_FEATHERS), urand(1000, 3000));
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
                    case uint32(Events::TORN_FEATHERS):
                        m_events.ScheduleEvent(uint32(Events::TORN_FEATHERS), 125000);
                        me->CastSpell(me, uint32(Spells::TORN_FEATHERS));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_WindFamiliar : public CreatureScript
        {
        public:
            // Entry: 76102
            mob_WindFamiliar()
                : CreatureScript("mob_WindFamiliar")
            {
            }

            enum class Spells : uint32
            {
                SERENE = 153716,
                WIND_BURST = 157352,
            };

            enum class Events : uint32
            {
                WIND_BURST = 1,
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_WindFamiliarAI(creature);
            }

            struct mob_WindFamiliarAI : public ScriptedAI
            {
                mob_WindFamiliarAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void JustDied(Unit* /*p_Killer*/)
                {
                    // When dying, Serene aura release all the nearby friends from crowd-control.
                    std::list<Unit*> l_TargetList;
                    float l_Radius = 10.0f;

                    JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(me, me, l_Radius);
                    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(me, l_TargetList, l_Check);
                    me->VisitNearbyObject(l_Radius, l_Searcher);

                    for (Unit* l_Unit : l_TargetList)
                    {
                        if (l_Unit)
                            l_Unit->RemoveCharmAuras();
                    }
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::WIND_BURST), urand(4000, 6000));
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
                    case uint32(Events::WIND_BURST):
                        m_events.ScheduleEvent(uint32(Events::WIND_BURST), urand(4000, 6000));
                        me->CastSpell(me->getVictim(), uint32(Spells::WIND_BURST));
                        break;
                    default:
                        break;
                    }
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_SolarFamiliar : public CreatureScript
        {
        public:
            // Entry: 76097
            mob_SolarFamiliar()
                : CreatureScript("mob_SolarFamiliar")
            {
            }

            enum class Spells : uint32
            {
                EMPOWER = 159290,   // 2:40:45 - 2:40:52
                FLASH_BANG = 152953, // Something like every 10 seconds and when it dies.
            };

            enum class Events : uint32
            {
                EMPOWER = 1,
                FLASH_BANG = 2
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_SolarFamiliarAI(creature);
            }

            struct mob_SolarFamiliarAI : public ScriptedAI
            {
                mob_SolarFamiliarAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void JustDied(Unit* /*p_Killer*/)
                {
                    // When Familiar dies, Flash Bang occures once again.
                    me->CastSpell(me, uint32(Spells::FLASH_BANG));
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::EMPOWER), urand(5000, 7000));
                    m_events.ScheduleEvent(uint32(Events::FLASH_BANG), urand(5000, 8000));
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
                    case uint32(Events::FLASH_BANG):
                        m_events.ScheduleEvent(uint32(Events::FLASH_BANG), urand(5000, 8000));
                        me->CastSpell(me, uint32(Spells::FLASH_BANG));
                        break;
                    case uint32(Events::EMPOWER):
                        m_events.ScheduleEvent(uint32(Events::EMPOWER), urand(5000, 7000));
                        if (Unit* l_Unit = InstanceSkyreach::SelectNearestFriendExcluededMe(me, 30.0f))
                            me->CastSpell(l_Unit, uint32(Spells::EMPOWER));
                        break;
                    default:
                        break;
                    }
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_SoaringChrakramMaster : public CreatureScript
        {
        public:
            // Entry: 76132
            mob_SoaringChrakramMaster()
                : CreatureScript("mob_SoaringChrakramMaster")
            {
            }

            enum class Spells : uint32
            {
                THROW_CHAKRAM_YELLOW = 169689,  // 2:40:57 - 2:40:58 2:41:00 - 2:41:06
                THROW_CHAKRAM_WHITE = 169690,   // 2:40:48 - 2:40:52 - 2:40:56 - 2:40:57
                THROW_CHAKRAM_2 = 169687,       // Launched with the two previous ones.
                THROW_CHAKRAM_DNT = 178612,
                SPINNING_BLADE = 153544,    // 2:40:16 - 2:40:35
                BLADE_DANCE = 153581,       // 2:40:56 - 2:41:17
            };

            enum class Events : uint32
            {
                THROW_CHAKRAM = 1,
                SPINNING_BLADE = 2,
                BLADE_DANCE = 3,
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_SoaringChrakramMasterAI(creature);
            }

            struct mob_SoaringChrakramMasterAI : public ScriptedAI
            {
                mob_SoaringChrakramMasterAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::THROW_CHAKRAM), urand(2000, 3000));
                    m_events.ScheduleEvent(uint32(Events::SPINNING_BLADE), urand(8000, 10000));
                    m_events.ScheduleEvent(uint32(Events::BLADE_DANCE), urand(17000, 25000));
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
                    case uint32(Events::THROW_CHAKRAM):
                        m_events.ScheduleEvent(uint32(Events::THROW_CHAKRAM), urand(2000, 3000));

                        if (urand(0, 1) == 0)
                        {
                            if (Player* l_plr = InstanceSkyreach::SelectFarEnoughPlayerIncludedTank(me, 5.0f))
                                me->CastSpell(l_plr, uint32(Spells::THROW_CHAKRAM_YELLOW));
                        }
                        else
                        {
                            if (Player* l_plr = InstanceSkyreach::SelectFarEnoughPlayerIncludedTank(me, 5.0f))
                                me->CastSpell(l_plr, uint32(Spells::THROW_CHAKRAM_WHITE));
                        }
                        me->CastSpell(me->getVictim(), uint32(Spells::THROW_CHAKRAM_2)); // Always launched with the two others.
                        break;
                    case uint32(Events::SPINNING_BLADE):
                        m_events.ScheduleEvent(uint32(Events::SPINNING_BLADE), urand(8000, 10000));
                        if (Player* l_plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 45.0f))
                            me->CastSpell(l_plr, uint32(Spells::SPINNING_BLADE));
                        break;
                    case uint32(Events::BLADE_DANCE):
                        m_events.ScheduleEvent(uint32(Events::BLADE_DANCE), urand(17000, 25000));
                        me->CastSpell(me->getVictim(), uint32(Spells::BLADE_DANCE));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_HeraldOfSunrise : public CreatureScript
        {
        public:
            // Entry: 78933
            mob_HeraldOfSunrise()
                : CreatureScript("mob_HeraldOfSunrise")
            {
            }

            enum class Spells : uint32
            {
                SOLAR_ZONE = 163330,    // 2:40:57 - 2:41:11
                MAGMA_EXPLOSION_SUPREME = 152864, // -FIXME: NOT WORKING
                SOLAR_HEAL = 152893,    // 2:41:08
                FLASH_HEAL = 152894,
            };

            enum class Events : uint32
            {
                SOLAR_ZONE = 1,
                SOLAR_HEAL = 2,
                FLASH_HEAL = 3
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_HeraldOfSunriseAI(creature);
            }

            struct mob_HeraldOfSunriseAI : public ScriptedAI
            {
                mob_HeraldOfSunriseAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::SOLAR_ZONE), urand(14500, 17000));
                    m_events.ScheduleEvent(uint32(Events::SOLAR_HEAL), urand(5500, 7000));
                    m_events.ScheduleEvent(uint32(Events::FLASH_HEAL), urand(5500, 7000));
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
                    case uint32(Events::SOLAR_ZONE):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_ZONE), urand(14500, 17000));
                        if (Unit* l_Unit = InstanceSkyreach::SelectNearestFriendExcluededMe(me, 30.0f))
                            me->CastSpell(l_Unit, uint32(Spells::SOLAR_ZONE));
                        break;
                    case uint32(Events::SOLAR_HEAL):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_HEAL), urand(5500, 7000));
                        if (Unit* l_Unit = InstanceSkyreach::SelectNearestFriendExcluededMe(me, 30.0f))
                            me->CastSpell(l_Unit, uint32(Spells::SOLAR_HEAL));
                        break;
                    case uint32(Events::FLASH_HEAL):
                        m_events.ScheduleEvent(uint32(Events::FLASH_HEAL), urand(5500, 7000));
                        me->CastSpell(me, uint32(Spells::FLASH_HEAL));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_WhirlingDervish : public CreatureScript
        {
        public:
            // Entry: 77605
            mob_WhirlingDervish()
                : CreatureScript("mob_WhirlingDervish")
            {
            }

            enum class Spells : uint32
            {
                STORM = 156515,
                DERVISH = 153905,
            };

            enum class Events : uint32
            {
                STORM = 1,
                DERVISH = 2
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_WhirlingDervishAI(creature);
            }

            struct mob_WhirlingDervishAI : public ScriptedAI
            {
                mob_WhirlingDervishAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::STORM), urand(8000, 13000));
                    m_events.ScheduleEvent(uint32(Events::DERVISH), urand(5500, 7000));
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
                    case uint32(Events::STORM):
                        m_events.ScheduleEvent(uint32(Events::STORM), urand(8000, 13000));
                        me->CastSpell(me, uint32(Spells::STORM));
                        break;
                    case uint32(Events::DERVISH):
                        m_events.ScheduleEvent(uint32(Events::DERVISH), urand(5500, 7000));
                        me->CastSpell(me, uint32(Spells::DERVISH));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_BloodedBladeFeather : public CreatureScript
        {
        public:
            // Entry: 76205
            mob_BloodedBladeFeather()
                : CreatureScript("mob_BloodedBladeFeather")
            {
            }

            enum class Spells : uint32
            {
                PIERCING_RUSH = 165731,
                DESPAWN_AREA_TRIGGERS = 138175,
            };

            enum class Events : uint32
            {
                PIERCING_RUSH = 1
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_BloodedBladeFeatherAI(creature);
            }

            struct mob_BloodedBladeFeatherAI : public ScriptedAI
            {
                mob_BloodedBladeFeatherAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::PIERCING_RUSH), urand(5000, 13000));
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
                    case uint32(Events::PIERCING_RUSH):
                        m_events.ScheduleEvent(uint32(Events::PIERCING_RUSH), urand(5000, 13000));
                        me->CastSpell(me->getVictim(), uint32(Spells::PIERCING_RUSH));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_InitiateOfTheRisingSun : public CreatureScript
        {
        public:
            // Entry: 79466
            mob_InitiateOfTheRisingSun()
                : CreatureScript("mob_InitiateOfTheRisingSun")
            {
            }

            enum class Spells : uint32
            {
                MAGMA_EXPLOSION_SUPREME = 152864, // FIXME: NOT WORKING, Didn't see it in the sniffs.
                SOLAR_SHOWER = 160274,          // Around 10 seconds.
                FLASH_HEAL = 152894,            // Around 6 seconds.
                CONJURE_SUN_ORB_DNT = 178618, // Visual to do when closed to sun orbs.
            };

            enum class Events : uint32
            {
                SOLAR_SHOWER = 1,
                FLASH_HEAL = 2
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_InitiateOfTheRisingSunAI(creature);
            }

            struct mob_InitiateOfTheRisingSunAI : public ScriptedAI
            {
                mob_InitiateOfTheRisingSunAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::SOLAR_SHOWER), urand(8000, 13000));
                    m_events.ScheduleEvent(uint32(Events::FLASH_HEAL), urand(5500, 7000));
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
                    case uint32(Events::SOLAR_SHOWER):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_SHOWER), urand(8000, 13000));
                        me->CastSpell(me, uint32(Spells::SOLAR_SHOWER));
                        break;
                    case uint32(Events::FLASH_HEAL):
                        m_events.ScheduleEvent(uint32(Events::FLASH_HEAL), urand(5500, 7000));
                        me->CastSpell(me, uint32(Spells::FLASH_HEAL));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_DivingChakramSpinner : public CreatureScript
        {
        public:
            // Entry: 76116
            mob_DivingChakramSpinner()
                : CreatureScript("mob_DivingChakramSpinner")
            {
            }

            enum class Spells : uint32
            {
                THROW_CHAKRAM_YELLOW = 169689,  // 2:40:57 - 2:40:58 2:41:00 - 2:41:06
                THROW_CHAKRAM_WHITE = 169690,   // 2:40:48 - 2:40:52 - 2:40:56 - 2:40:57
                THROW_CHAKRAM_2 = 169687,       // Launched with the two previous ones.
                THROW_CHAKRAM_DNT = 178612,
                SPINNING_BLADE = 153544,    // 2:40:16 - 2:40:35
            };

            enum class Events : uint32
            {
                THROW_CHAKRAM = 1,
                SPINNING_BLADE = 2
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_DivingChakramSpinnerAI(creature);
            }

            struct mob_DivingChakramSpinnerAI : public ScriptedAI
            {
                mob_DivingChakramSpinnerAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::THROW_CHAKRAM), urand(2000, 3000));
                    m_events.ScheduleEvent(uint32(Events::SPINNING_BLADE), urand(8000, 10000));
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
                    case uint32(Events::THROW_CHAKRAM):
                        m_events.ScheduleEvent(uint32(Events::THROW_CHAKRAM), urand(2000, 3000));

                        if (urand(0, 1) == 0)
                        {
                            if (Player* l_plr = InstanceSkyreach::SelectFarEnoughPlayerIncludedTank(me, 5.0f))
                                me->CastSpell(l_plr, uint32(Spells::THROW_CHAKRAM_YELLOW));
                        }
                        else
                        {
                            if (Player* l_plr = InstanceSkyreach::SelectFarEnoughPlayerIncludedTank(me, 5.0f))
                                me->CastSpell(l_plr, uint32(Spells::THROW_CHAKRAM_WHITE));
                        }
                        me->CastSpell(me->getVictim(), uint32(Spells::THROW_CHAKRAM_2)); // Always launched with the two others.
                        break;
                    case uint32(Events::SPINNING_BLADE):
                        m_events.ScheduleEvent(uint32(Events::SPINNING_BLADE), urand(8000, 10000));
                        if (Player* l_plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 45.0f))
                            me->CastSpell(l_plr, uint32(Spells::SPINNING_BLADE));
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

        class mob_BlindingSolarFlare : public CreatureScript
        {
        public:
            // Entry: 79462
            mob_BlindingSolarFlare()
                : CreatureScript("mob_BlindingSolarFlare")
            {
            }

            enum class Spells : uint32
            {
                SOLAR_WRATH = 157020,           // 2:40:56 - 2:41:00 - 2:41:05 - 2:41:08
                SOLAR_DETONATION_2 = 160282,      // 2:41:05
                SOLAR_DETONATION = 160288,    // 2:40:59 - 2:41:12
            };

            enum class Texts : int32
            {
                SOLAR_DETONATION = 14061,
            };

            enum class Events : uint32
            {
                SOLAR_WRATH = 1,
                SOLAR_DETONATION = 2
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new mob_BlindingSolarFlareAI(creature);
            }

            struct mob_BlindingSolarFlareAI : public ScriptedAI
            {
                mob_BlindingSolarFlareAI(Creature* creature) : ScriptedAI(creature),
                m_Instance(creature->GetInstanceScript()),
                m_events()
                {
                }

                void Reset()
                {
                    m_events.Reset();
                }

                void EnterCombat(Unit* who)
                {
                    m_events.ScheduleEvent(uint32(Events::SOLAR_WRATH), urand(3000, 5000));
                    m_events.ScheduleEvent(uint32(Events::SOLAR_DETONATION), urand(10000, 14000));
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
                    case uint32(Events::SOLAR_WRATH):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_WRATH), urand(3000, 5000));
                        me->CastSpell(me->getVictim(), uint32(Spells::SOLAR_WRATH));
                        break;
                    case uint32(Events::SOLAR_DETONATION):
                        m_events.ScheduleEvent(uint32(Events::SOLAR_DETONATION), urand(10000, 14000));
                        if (Player* l_plr = InstanceSkyreach::SelectRandomPlayerIncludedTank(me, 45.0f))
                        {
                            me->CastSpell(l_plr, uint32(Spells::SOLAR_DETONATION));
                            l_plr->MonsterWhisper(int32(Texts::SOLAR_DETONATION), l_plr->GetGUID(), true);
                        }
                        break;
                    default:
                        break;
                    }

                    DoMeleeAttackIfReady();
                }

                InstanceScript* m_Instance;
                EventMap m_events;
            };
        };

    }
}

void AddSC_mob_instance_skyreach()
{
    // Mobs.
    new MS::InstanceSkyreach::mob_BlindingSolarFlare();
    new MS::InstanceSkyreach::mob_DivingChakramSpinner();
    new MS::InstanceSkyreach::mob_InitiateOfTheRisingSun();
    new MS::InstanceSkyreach::mob_WhirlingDervish();
    new MS::InstanceSkyreach::mob_BloodedBladeFeather();
    new MS::InstanceSkyreach::mob_HeraldOfSunrise();
    new MS::InstanceSkyreach::mob_SoaringChrakramMaster();
    new MS::InstanceSkyreach::mob_SolarFamiliar();
    new MS::InstanceSkyreach::mob_SkyReachOverlord();
    new MS::InstanceSkyreach::mob_AdeptOfTheDawn();
    new MS::InstanceSkyreach::mob_WindFamiliar();
    new MS::InstanceSkyreach::mob_DrivingGaleCaller();
    new MS::InstanceSkyreach::mob_AdornedBladetalon();
    new MS::InstanceSkyreach::mob_SkyreachSunTalon();
}