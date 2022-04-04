-- Created by Vertabelo (http://vertabelo.com)
-- Last modification date: 2022-04-04 06:18:05.07

-- tables
-- Table: CompactDisk
CREATE TABLE CompactDisk (
    id int  NOT NULL,
    date date  NOT NULL,
    company char(256)  NOT NULL,
    price int  NOT NULL,
    PRIMARY KEY (id)
) ;

-- Table: MusicalComposition
CREATE TABLE MusicalComposition (
    name int  NOT NULL,
    author char(256)  NOT NULL,
    performer char(256)  NOT NULL,
    compactID int  NOT NULL,
    PRIMARY KEY (compactID)
) ;

-- Table: Trade
CREATE TABLE Trade (
    date date  NOT NULL,
    code int  NOT NULL,
    compactID int  NOT NULL,
    amount int  NOT NULL,
    TradeCodeInfo_id int  NOT NULL
) ;

-- Table: TradeCodeInfo
CREATE TABLE TradeCodeInfo (
    id int  NOT NULL,
    type char(10)  NOT NULL,
    PRIMARY KEY (id)
) ;

-- foreign keys
-- Reference: MusicalComposition_CompactDisk (table: MusicalComposition)
ALTER TABLE MusicalComposition ADD FOREIGN KEY (compactID)
    REFERENCES CompactDisk (id)
;

-- Reference: Trade_CompactDisk (table: Trade)
ALTER TABLE Trade ADD FOREIGN KEY (compactID)
    REFERENCES CompactDisk (id)
;

-- Reference: Trade_TradeCodeInfo (table: Trade)
ALTER TABLE Trade ADD FOREIGN KEY (TradeCodeInfo_id)
    REFERENCES TradeCodeInfo (id)
;

-- End of file.

