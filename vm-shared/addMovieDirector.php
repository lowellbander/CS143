<!DOCTYPE html>
<html>
    <head>
        <title>Add a Director to a Movie</title>
    </head>
    <body>
        <a href="./index.php"><- Go Home<a><br>
        <h1>Add a Director to a Movie</h1>
    </body>
    <?php
        include 'exe_q.php';
        
        print "<form action='./addMovieDirector.php' method='GET'>";
        // populate the dropdowns with data from the database

        $movies = query("SELECT id, title, year FROM Movie;");
        print "Movie: <select name='mid'>";
        while ($row = mysql_fetch_array($movies)) {
            print "<option value='" . $row['id'] . "'>" . $row['title'] . " (" . $row['year'] . ") </option>";
        }
        print "</select><br>";
        
        $directors = query("SELECT id, first, last, dob FROM Director;");
        print "Dctor: <select name='did'>";
        while ($row = mysql_fetch_array($directors)) {
            print "<option value='" . $row['id'] . "'>" . $row['first'] . " ". $row['last'] . " (" . $row['dob'] . ") </option>";
        }
        print "</select><br>";
        print "<br><input type='submit' value='Add'></form>";

        if (!empty($_GET['mid'])) {

        // Build SQL query from URL params
        

        $query = "INSERT INTO MovieDirector VALUES (" . $_GET['mid'] . "," . $_GET['did'] . ");";
            
        //print "query: $query";

        // execute query
            
       $rs = query($query);
       
        // echo success or failure
    
        if ($rs) {
            print "<b>Successfully added Director/Movie Relation!</b>";
        } else {
            print "<b>Sorry bro, bad SQL. Maybe invalid syntax, or the command violated a CHECK constraint. Bummer.</b>";
        }

        }

    ?>
<html>
