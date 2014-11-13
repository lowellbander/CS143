/* for each constraint, demonstrate at least one database modification that
 * violates it, and explain why the operation violates the constraint. Create
 * another MySQL script named violate.sql that contains all such modification
* statements. For each statement, explain what constraint it violates in detail
* as a comment in the script. Again, for the three CHECK constraints, pretend
* that MySQL supports them and write the modification statements that will
* violate them if MySQL supports them.
*/

-- (PK1) every movie has a unique identification number 

INSERT INTO Movie (id)
VALUES (2000);

/* Because Movie.id is a primary key, there can be no two entries in the Movie
 * table with identical id's. Because there is already an existing Movie with id
 * of 2000, this INSERT statement violates the primary key constraint.
 */

-- (PK2) every actor has a unique identification number 

INSERT INTO Actor (id)
VALUES (1000);

/* Because Actor.id is a primary key, there can be no two entries in the Actor
 * table with identical id's. Because there is already an existing Actor with id
 * of 1000, this INSERT statement violates the primary key constraint.
 */

-- (PK3) every director has a unique primary key

INSERT INTO Director (id)
VALUES (68626);

/* Because Director.id is a primary key, there can be no two entries in the Actor
 * table with identical id's. Because there is already an existing Director with id
 * of 68626, this INSERT statement violates the primary key constraint.
 */

-- (CHECK1) no movies made before jesus was born 

INSERT INTO Movie (year)
VALUES (-20);

/* Because of the check constraint, Movie.year must be a nonnegative number.
 * Because -20 is negative, the INSERT will be rejected.
 */

-- (CHECK2) enforce the gender binary 

INSERT INTO Actor (sex)
VALUES ("trans");

/* Because of the check constraint, the value of Actor.sex must either be "male"
 * or "female". Because "trans" is neither of these, the bigendered check
 * constraint will reject this INSERT.
 */

-- (CHECK3) you can't die before you're born

INSERT INTO Actor (dob, dod)
VALUES ('2010-01-01', '1994-01-01');

/* Because of the check constraint, Actor.dob must be beore Actor.dod. Because
 * the values given in this INSERT statement violate the check constraint, the
 * INSERT will be rejected.
 */

-- (FK1) mid must reference an existing movie

DELETE FROM Movie
WHERE id = 100;

/* This SQL statement would violate this referential constraint because there
 * are two entries in the MovieGenre which have an mid of 100. Because of this,
 * these two entries will need to be deleted before the corresponding entry from
 * the Movie table can be deleted.
 */

-- (FK2) mid must reference an existing movie 

DELETE FROM Movie
WHERE id = 3743;

/* This SQL statement would violate this referential constraint because there is
 * an entry in the MovieDirector table with mid of 3743. Because of this, this
 * entry will need to be deleted before the corresponding entry from the Movie
 * table can be deleted.
 */

-- (FK3) did must reference an existing director

DELETE FROM Director
WHERE id = 68622;

/* This SQL statement would violate this referential constraint because there is
 * an entry in the MovieDirector table with did of 68622. Because of this, this
 * entry will need to be deleted before the corresponding entry from the
 * Director table can be deleted.
 */

-- (FK4) mid must reference an existing movie 

DELETE FROM Movie
WHERE id = 2000;

/* This SQL statement would violate this referential constraint because there
 * are multiple entries in the MovieActor which have an mid of 2000. Because of this,
 * these multiple entries will need to be deleted before the corresponding entry from
 * the Movie table can be deleted.
 */

-- (FK5) aid must reference an existing actor 

DELETE FROM Actor
WHERE id = 4346;

/* This SQL statement would violate this referential constraint because there
 * are multiple entries in the MovieActor which have an aid of 4346. Because of this,
 * these multiple entries will need to be deleted before the corresponding entry from
 * the Actor table can be deleted.
 */

-- (FK6) mid must reference an existing movie

INSERT INTO Review (mid, comment)
VALUES (3000, "This movie was lame.");

DELETE FROM Movie
WHERE id = 3000;

/* This SQL statement would violate this referential constraint because there is
 * now a entry in the Review table which references the entry in the Movie table
 * with id = 3000. Because of this, this entry in the Review table will need to
 * be deleted before the corresponding entry from the Movie table can be
 * deleted.
 */
