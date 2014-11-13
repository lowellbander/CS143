<!DOCTYPE html>
<html>
    <head>
        <title>Actor Information</title>
    </head>
    <body>
        <a href="./index.php"><- Go Home<a><br>
        <h1>Actor Information</h1>
    </body>
    <?php
        include 'exe_q.php';

        if (!empty($_GET['id'])) {
            
            // get information about the actor

            $id = $_GET['id'];
            $query = "SELECT * FROM Actor WHERE id = " . $id . ";";
            $actor = query($query);

            $name = "";
            $sex = "";
            $dob = "";
            $dod = "";

            while ($row = mysql_fetch_array($actor)) {
                $name = $row['first'] . " " . $row['last'];
                $sex = $row['sex'];
                $dob = $row['dob'];
                $dod = $row['dod'];
            }

            if ($dod == "") $dod = "~Still Living~";

            // display information about the actor

            print "Name: $name<br>";
            print "Sex: $sex<br>";
            print "Date of Birth: $dob<br>";
            print "Date of Death: $dod<br>";

            // which movies have they acted in?
            
            $filmCount = query("SELECT COUNT(*) AS num FROM MovieActor WHERE aid=" . $id . ";");
            $countRow = mysql_fetch_array($filmCount);
            if($countRow['num'] == 0)
                print "<h3>$name has not acted in any films</h3><br>"; 
            else{
                print "<h3>$name has acted in the following films</h3><br>";

                $query = "SELECT DISTINCT * FROM MovieActor WHERE aid=" . $id . ";";
                $movieActors = query($query);
                while ($row = mysql_fetch_array($movieActors)) {
                    $mid = $row['mid'];
                    $role = $row['role'];
                
                    $title = "";
                    $movie = query("SELECT title FROM Movie WHERE id=" . $mid . ";");
                    while ($row = mysql_fetch_array($movie))
                        $title = $row['title'];

                    print "Played the role of $role in <a href='./movieInfo.php?id=$mid'>$title<a>.<br>";
            }
            
            }

        }

    ?>
<html>
