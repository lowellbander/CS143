<!DOCTYPE html>
<html>
    <head>
        <title>Add An Actor to a Movie</title>
    </head>
    <body>
        <h1>Add An Actor to a Movie</h1>
    </body>
    <?php
        include 'exe_q.php';
        
        print "<form action='./addMovieActor.php' method='GET'>";
        // populate the dropdowns with data from the database

        $movies = query("SELECT id, title, year FROM Movie;");
        print "Movie: <select name='mid'>";
        while ($row = mysql_fetch_array($movies)) {
            print "<option value='" . $row['id'] . "'>" . $row['title'] . " (" . $row['year'] . ") </option>";
        }
        print "</select><br>";
        
        $actors = query("SELECT id, first, last, dob FROM Actor;");
        print "Actor: <select name='aid'>";
        while ($row = mysql_fetch_array($actors)) {
            print "<option value='" . $row['id'] . "'>" . $row['first'] . " ". $row['last'] . " (" . $row['dob'] . ") </option>";
        }
        print "</select><br>";
        print "Role: <input type='text' name='role'>";
        print "<br><input type='submit' value='Add'></form>";

        if (!empty($_GET['mid'])) {

        // Build SQL query from URL params
        
        $role = $_GET['role'];

        $query = "INSERT INTO MovieActor VALUES (" . $_GET['mid'] . "," . $_GET['aid'] . ",'" . $role . "');";
            
        //print "query: $query";

        // execute query
            
       $rs = query($query);
       
        // echo success or failure
    
        if ($rs) {
            print "<b>Successfully added Actor/Movie Relation!</b>";
        } else {
            print "<b>Sorry bro, bad SQL. Maybe invalid syntax, or the command violated a CHECK constraint. Bummer.</b>";
        }

        }

    ?>
<html>
