--  Give me the names of all the actors in the movie 'Die Another Day'. Please also make sure actor names are in this format:  <firstname> <lastname>   (seperated by single space).

SELECT DISTINCT first, last
FROM Movie AS M, MovieActor AS MA, Actor AS A
WHERE M.title = "Die Another Day" AND
	M.id = MA.mid AND
	MA.aid = A.id;

-- In theory, this query should do the same, but takes too long.
/*SELECT DISTINCT first, last
FROM Actor
WHERE id IN (
	SELECT aid
	FROM MovieActor
	WHERE mid = (
		SELECT DISTINCT id
		FROM Movie
		WHERE title="Die Another Day")
);*/

-- Give me the count of all the actors who acted in multiple movies.

SELECT COUNT(*)
FROM (
    SELECT COUNT(*) AS n
    FROM MovieActor
    GROUP BY aid) AS counts
WHERE n > 1;

-- another query that you came up with
/* First and last names of all actors and actresses who have played themselved in movies */

SELECT first, last 
FROM Actor 
WHERE id 
IN (
	SELECT aid 
	FROM MovieActor 
	WHERE role="Himself" OR role="Herself"
   );


 
