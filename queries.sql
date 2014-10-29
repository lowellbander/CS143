--  Give me the names of all the actors in the movie 'Die Another Day'. Please also make sure actor names are in this format:  <firstname> <lastname>   (seperated by single space).

/*SELECT first, last
FROM Actor
WHERE id IN (
);*/

	SELECT aid
	FROM MovieActor
	WHERE mid = (
		SELECT DISTINCT id
		FROM Movie
		WHERE title="Die Another Day");

-- Give me the count of all the actors who acted in multiple movies.

-- another query that you came up with
