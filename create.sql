CREATE TABLE Movie(
    id INT PRIMARY KEY, -- every movie has a unique identification number
    title VARCHAR(100), 
    year INT, 
    rating VARCHAR(10), 
    company VARCHAR(50),
    CHECK (year > 0)) -- no movies made before jesus was born
    ENGINE = INNODB;

CREATE TABLE Actor(
    id INT PRIMARY KEY, -- every actor has a unique identification number
    last VARCHAR(20), 
    first VARCHAR(20), 
    sex VARCHAR(6), 
    dob DATE, 
    dod DATE, 
    CHECK (sex="male" OR sex="female")) -- enforce the gender binary
    ENGINE = INNODB;

CREATE TABLE Director(
    id INT PRIMARY KEY, -- every director has a unique primary key
    last VARCHAR(20), 
    first VARCHAR(20), 
    dob DATE, 
    dod DATE,
    CHECK (dod IS NULL OR (dod > dob))) -- you can't die before you're born
    ENGINE = INNODB;

CREATE TABLE MovieGenre(
    mid INT, 
    genre VARCHAR(20))
    ENGINE = INNODB;

CREATE TABLE MovieDirector(
    mid INT, 
    did INT)
    ENGINE = INNODB;

CREATE TABLE MovieActor(
    mid INT, 
    aid INT, 
    role VARCHAR(50))
    ENGINE = INNODB;

CREATE TABLE Review(
    name VARCHAR(20), 
    time TIMESTAMP, 
    mid INT, 
    rating INT, 
    comment VARCHAR(500))
    ENGINE = INNODB;

CREATE TABLE MaxPersonID(
    id INT)
    ENGINE = INNODB;

CREATE TABLE MaxMovieID(
    id INT)
    ENGINE = INNODB;


