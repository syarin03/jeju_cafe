-- 반경 1km 이내 정보를 호출하는 프로시저

DELIMITER $$
CREATE PROCEDURE in_one_kilometer(
	longitude DOUBLE,
	latitude DOUBLE
)
BEGIN
	SELECT * FROM 1team_db.jeju_total_table WHERE pow((x-longitude)/0.0112689, 2)+pow((y-latitude)/0.0090896, 2)<1;
END $$
DELIMITER ;