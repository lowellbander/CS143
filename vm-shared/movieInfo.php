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

            $query = "SELECT DISTINCT * FROM MovieActor WHERE mid=" . $id . ";";
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

            print "<br><b><a href='./addReview.php?id=$id'>Review this movie!</a></b>";

        }

    ?>
<html>
