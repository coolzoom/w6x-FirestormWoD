DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_rushing_jade_wind_heal';
INSERT INTO spell_script_names VALUE (162530, 'spell_monk_rushing_jade_wind_heal');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_rushing_jade_wind_damage';
INSERT INTO spell_script_names VALUE (148187, 'spell_monk_rushing_jade_wind_damage');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_serpents_zeal';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_muscle_memory';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_fists_of_fury_stun';
DELETE FROM spell_script_names WHERE spell_id = 120086 AND ScriptName = 'spell_monk_fists_of_fury';
INSERT INTO spell_script_names VALUES (120086, 'spell_monk_fists_of_fury');
