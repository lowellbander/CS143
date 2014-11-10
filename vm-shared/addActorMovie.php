<!DOCTYPE html>
<html>
    <head>
        <title>Add An Actor to a Movie</title>
    </head>
    <body>
        <h1>Add An Actor to a Movie</h1>
        <form>
            <input type="submit" value="Add">
        </form>
    </body>
    <?php
        include 'exe_q.php';
        
        $movies = query("SELECT id, title, year FROM Movie;");
        print "<select name='mid'>";
        while ($row = mysql_fetch_array($movies)) {
            print "<option value='" . $row['id'] . "'>" . $row['title'] . " (" . $row['year'] . ") </option>";
        }
        print "</select>";
        
    ?>
<html>
