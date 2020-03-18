DELIMITER $$

CREATE PROCEDURE INSERT_BASICINFO(IN p_id varchar(20), IN p_nick varchar(20))
BEGIN
  DECLARE v_lv int;
  DECLARE v_exp int;
  DECLARE v_gold int;
  DECLARE v_gem int;
  DECLARE v_gemOfVictory int;
  DECLARE v_costume int;

  SELECT lv, exp, gold, gem, gemOfVictory, costume
  INTO v_lv, v_exp, v_gold, v_gem, v_gemOfVictory, v_costume
  FROM INITTABLE_BASICINFO;

  INSERT INTO BASICINFO VALUE(p_id, p_nick, v_lv, v_exp, v_gold, v_gem, v_gemOfVictory, v_costume);

END;
$$

CREATE PROCEDURE INSERT_MATCHINFO(IN p_id varchar(20))
BEGIN
  DECLARE v_matchingCount int;
  DECLARE v_victoryCount int;
  DECLARE v_victoryStreak int;
  DECLARE v_loseStreak int;
  DECLARE v_score int;

  SELECT matchingCount, victoryCount, victoryStreak, loseStreak, score
  INTO v_matchingCount, v_victoryCount, v_victoryStreak, v_loseStreak, v_score
  FROM INITTABLE_MATCHINFO;

  INSERT INTO MATCHINFO VALUE(p_id, v_matchingCount, v_victoryCount, v_victoryStreak, v_loseStreak, v_score);

END;
$$

CREATE PROCEDURE INSERT_CHARA_DACK(IN p_id varchar(20))
BEGIN
  DECLARE v_leader int;
  DECLARE v_subChara1 int;
  DECLARE v_subChara2 int;
  DECLARE v_subChara3 int;

  SELECT leaderChara, subChara1, subChara2, subChara3
  INTO v_leaderChara, v_subChara1, v_subChara2, v_subChara3
  FROM INITTABLE_CHARA_DACK;

  INSERT INTO CHARA_DACK VALUE(p_id, v_leader, v_subChara1, v_subChara2, v_subChara3);

END;
$$

CREATE PROCEDURE INSERT_ARTIFACT_DACK(IN p_id varchar(20))
BEGIN
  DECLARE v_a1 int;
  DECLARE v_a2 int;
  DECLARE v_a3 int;

  SELECT a1, a2, a3
  INTO v_a1, v_a2, v_a3
  FROM INITTABLE_ARTIFACT_DACK;

  INSERT INTO ARTIFACT_DACK VALUE(p_id, v_a1, v_a2, v_a3);

 END;
 $$

 CREATE PROCEDURE INSERT_COSTUME_DACK(IN p_id varchar(20))
 BEGIN
   DECLARE v_c1 int;
   DECLARE v_c2 int;
   DECLARE v_c3 int;

   SELECT c1, c2, c3
   INTO v_c1, v_c2, v_c3
   FROM INITTABLE_COSTUME_DACK;

   INSERT INTO COSTUME_DACK VALUE(p_id, v_c1, v_c2, v_c3);

END;
$$

CREATE PROCEDURE INSERT_SUBCHARA_INVENTORY(IN p_id varchar(20))
BEGIN
  DECLARE v_c1 bool;
  DECLARE v_c2 bool;
  DECLARE v_c3 bool;

  SELECT c1, c2, c3
  INTO v_c1, v_c2, v_c3
  FROM INITTABLE_SUBCHARA_INVENTORY;

  INSERT INTO SUBCHARA_INVENTORY VALUE(p_id, v_c1, v_c2, v_c3);

END;
$$

CREATE PROCEDURE INSERT_ARTIFACT_INVENTORY(IN p_id varchar(20))
BEGIN
  DECLARE v_a1 bool;
  DECLARE v_a2 bool;
  DECLARE v_a3 bool;
  DECLARE v_a4 bool;
  DECLARE v_a5 bool;
  DECLARE v_a6 bool;
  DECLARE v_a7 bool;

  SELECT a1, a2, a3, a4, a5, a6, a7
  INTO v_a1, v_a2, v_a3, v_a4, v_a5, v_a6, v_a7
  FROM INITTABLE_ARTIFACT_INVENTORY;

  INSERT INTO ARTIFACT_INVENTORY VALUE(p_id, v_a1, v_a2, v_a3, v_a4, v_a5, v_a6, v_a7);

END;
$$

CREATE PROCEDURE INSERT_COSTUME_INVENTORY(IN p_id varchar(20))
BEGIN
  DECLARE v_c1 bool;
  DECLARE v_c2 bool;
  DECLARE v_c3 bool;
  DECLARE v_c4 bool;

  SELECT c1, c2, c3, v_c4
  INTO v_c1, v_c2, v_c3, v_c4
  FROM INITTABLE_COSTUME_INVENTORY;

  INSERT INTO COSTUME_INVENTORY VALUE(p_id, v_c1, v_c2, v_c3, v_c4);

END;
$$

CREATE PROCEDURE REGISTER(IN p_id varchar(20), IN p_nick varchar(20))
BEGIN
  CALL INSERT_BASICINFO(p_id, p_nick);
  CALL INSERT_MATCHINFO(p_id);
  CALL INSERT_CHARA_DACK(p_id);
  CALL INSERT_ARTIFACT_DACK(p_id);
  CALL INSERT_COSTUME_DACK(p_id);
  CALL INSERT_SUBCHARA_INVENTORY(p_id);
  CALL INSERT_ARTIFACT_INVENTORY(p_id);
  CALL INSERT_COSTUME_INVENTORY(p_id);

END;
$$

