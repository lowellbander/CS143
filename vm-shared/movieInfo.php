<!DOCTYPE html>
<html>
    <head>
        <title>Movie Information</title>
    </head>
    <body>
        <h1>Movie Information</h1>
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

        }

    ?>
<html>
