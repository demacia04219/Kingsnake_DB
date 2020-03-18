
create table BASICINFO (
	id				varchar(20) primary key not null
    , nick			varchar(20) not null
    , lv			int not null
    , exp			int not null
	, gold			int not null
    , gem			int not null
    , gemOfVictory	int not null
    , costume		int not null
);

CREATE TABLE MATCHINFO (
	id varchar(20) primary key not null
    , matchingCount int not null
    , victoryCount int not null
    , victoryStreak int not null
    , loseStreak int not null
	, score int not null
);

CREATE TABLE CHARA_DACK (
	id varchar(20) primary key not null
	, leaderSlot int not null
    , subSlot1 int not null
    , subSlot2 int not null
    , subSlot3 int not null
);

CREATE TABLE ARTIFACT_DACK (
	id varchar(20) primary key not null
    , slot1 int not null
    , slot2 int not null
    , slot3 int not null
);

CREATE TABLE COSTUME_DACK (
	id varchar(20) primary key not NULL
	, slot1 int not null
	, slot2 int not null
	, slot3 int not null
	, slot4 int not null
)

CREATE TABLE SUBCHARA_INVENTORY (
	id varchar(20) primary key not null
	, c1 bool not null
	, c2 bool not null
	, c3 bool not null
)

CREATE TABLE ARTIFACT_INVENTORY (
	ID VARCHAR(20) PRIMARY KEY NOT NULL
	, A1 BOOL NOT NULL
	, A2 BOOL NOT NULL
	, A3 BOOL NOT NULL
	, A4 BOOL NOT NULL
	, A5 BOOL NOT NULL
	, A6 BOOL NOT NULL
	, A7 BOOL NOT NULL
)

CREATE TABLE COSTUME_INVENTORY (
	ID varcahr(20) primary key not null
	, c1 bool not null
	, c2 bool not null
	, c3 bool not null
	, c4 bool not null
)
