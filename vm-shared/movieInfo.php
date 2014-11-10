<!DOCTYPE html>
<html>
    <head>
        <title>Movie Information</title>
    </head>
    <body>
        <a href="./index.php"><- Go Home<a><br>
        <h1>Movie Information</h1>
    </body>
    <?php
        include 'exe_q.php';

        if (!empty($_GET['id'])) {
            
            // get information about the actor

            $id = $_GET['id'];
            $query = "SELECT * FROM Movie WHERE id = " . $id . ";";
            $movie = query($query);

            $title = "";
            $year = "";
            $rating = "";
            $company = "";

            while ($row = mysql_fetch_array($movie)) {
                $title = $row['title'];
                $year = $row['year'];
                $rating = $row['rating'];
                $company = $row['company'];
            }

            // display information about the actor

            print "Title: $title ($year)<br>";
            print "Rating: $rating<br>";
            print "Producer: $company<br>";

            // which movies have they acted in?
            
            print "<h3>Actors in this movie:</h3><br>";

            $query = "SELECT * FROM MovieActor WHERE mid=" . $id . ";";
            $movieActors = query($query);
            while ($row = mysql_fetch_array($movieActors)) {
                $aid = $row['aid'];
                $role = $row['role'];
            
                $name = "";
                $actor = query("SELECT id, first, last FROM Actor WHERE id=" . $aid . ";");
                while ($row = mysql_fetch_array($actor))
                    $name = $row['first'] . " " . $row['last'];

                print "<a href='./actorInfo.php?id=$aid'>$name</a> played the role of $role.<br>";
            }

            //show movie reviews
            $scoreQuery = query("SELECT AVG(rating) FROM Review WHERE mid=" . $id . ";") ;
            $avgScoreRow = mysql_fetch_array($scoreQuery);
            $avgScore = $avgScoreRow['AVG(rating)'];

            $scoreCountQuery = query("SELECT COUNT(*) AS count FROM Review WHERE mid=" . $id . ";");
            $scoreCountRow = mysql_fetch_array($scoreCountQuery);
            $scoreCount = $scoreCountRow['count'];

            print "<h3>User Reviews:</h3><br>";
            print "<p>Average score: " . $avgScore . "/5 (5.0 is best) by " . $scoreCount . " reviews.";
            print "<a href='./addReview.php?id=" . $id . ">Add your review!</a>";


            $reviewQuery = "SELECT * FROM Review WHERE mid=" . $id . ";";
            $movieReviews = query($reviewQuery);
            while($row = mysql_fetch_array($movieReviews)){
                print "On " . $movieReviews['time'] . ", " . $movieReviews['name'] . " said " . $movieReviews['comment'] .".<br>";
            }

        }

    ?>
<html>
