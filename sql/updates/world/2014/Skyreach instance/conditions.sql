
DELETE FROM conditions WHERE SourceEntry IN (154149, 154179);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 154149, 31, 0, 3, 76141),
(13, 1, 154179, 31, 0, 3, 76141);