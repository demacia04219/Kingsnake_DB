
CREATE TABLE INITTABLE_BASICINFO (
	  ver int not null AUTO_INCREMENT PRIMARY KEY
    , lv int not null
    , exp int not null
    , gold int not null
    , gem int not null
    , gemOfVictory int not null
    , costume int not null
);
INSERT INTO INITTABLE_BASICINFO(lv, exp, gold, gem, gemOfVictory, costume) VALUE(
      1
    , 0
    , 0
    , 0
    , 0
    , -1
);

CREATE TABLE INITTABLE_MATCHINFO (
      ver int not null AUTO_INCREMENT PRIMARY KEY
    , matchingCount int not null
    , victoryCount int not null
    , victoryStreak int not null
    , loseStreak int not null
    , score int not null
);
INSERT INTO INITTABLE_MATCHINFO(matchingCount, victoryCount, victoryStreak, loseStreak, score) VALUE(
      0
    , 0
    , 0
    , 0
    , 0
);

CREATE TABLE INITTABLE_CHARA_DACK (
      ver int not null AUTO_INCREMENT PRIMARY KEY
    , leaderSlot int not null
    , subSlot1 int not null
    , subSlot2 int not null
    , subSlot3 int not null
);
INSERT INTO INITTABLECHARA_DACK(leaderSlot, subSlot1, subSlot2, subSlot3) VALUE(
      -1
    , -1
    , -1
    , -1
);

CREATE TABLE INITTABLE_ARTIFACT_DACK (
      ver int not null AUTO_INCREMENT PRIMARY KEY
    , slot1 int not null
    , slot2 int not null
    , slot3 int not null
);
INSERT INTO INITTABLE_ARTIFACT_DACK(slot1, slot2, slot3) VALUE(
      -1
    , -1
    , -1
);

CREATE TABLE INITTABLE_COSTUME_DACK (
      ver int not null AUTO_INCREMENT PRIMARY KEY
    , slot1 int not null
    , slot2 int not null
    , slot3 int not null
    , slot4 int not null
);
INSERT INTO INITTABLE_COSTUME_DACK(slot1, slot2, slot3, slot4) VALUE(
      -1
    , -1
    , -1
    , -1
);

CREATE TABLE INITTABLE_SUBCHARA_INVENTORY(
      ver int not null AUTO_INCREMENT PRIMARY KEY
    , isExist1 bool not null
    , isExist2 bool not null
    , isExist3 bool not null
);
INSERT INTO INITTABLE_SUBCHARA_INVENTORY(isExist1, isExist2, isExist3) VALUE(
      true
    , true
    , true
);

CREATE TABLE INITTABLE_ARTIFACT_INVENTORY(
      ver int not null AUTO_INCREMENT PRIMARY KEY
    , isExist1 bool not null
    , isExist2 bool not null
    , isExist3 bool not null
    , isExist4 bool not null
    , isExist5 bool not null
    , isExist6 bool not null
    , isExist7 bool not null
);
INSERT INTO INITTABLE_ARTIFACT_INVENTORY(isExist1, isExist2, isExist3, isExist4, isExist5, isExist6, isExist7) VALUE(
      true
    , true
    , true
    , true
    , true
    , true
    , true
);

CREATE TABLE INITTABLE_COSTUME_INVENTORY(
      ver int not null AUTO_INCREMENT PRIMARY KEY
    , isExist1 bool not null
    , isExist2 bool not null
    , isExist3 bool not null
    , isExist4 bool not null
);
INSERT INTO INITTABLE_COSTUME_INVENTORY(isExist1, isExist2, isExist3, isExist4) VALUE(
      true
    , true
    , true
    , true
);
