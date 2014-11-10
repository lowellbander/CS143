<!DOCTYPE html>
<html>
    <head>
        <title>Add a Review</title>
    </head>
    <body>
        <a href="./index.php"><- Go Home<a><br>
    </body>
    <?php
        include 'exe_q.php';
        $id = $_GET['id'];
        $movie = query("SELECT id, title, year FROM Movie WHERE id=" . $id . ";");
        $title = "";
        $year = "";
        while ($row = mysql_fetch_array($movie)) {
            $title = $row['title'];
            $year = $row['year'];
        }
        print "<h1>Create a review for \"$title ($year)\"</h1>";
        print "<form method='GET' action='./addReview.php'>";
        print "<input type='hidden' name='id' value='$id'>";
        print "Your Name: <input type='text' name='name'><br>";
        print "Rating: <select name='rating'>";
        print "<option value='5'>5 - Great</option>";
        print "<option value='4'>4 - Good</option>";
        print "<option value='3'>3 - Neutral</option>";
        print "<option value='2'>2 - Bad</option>";
        print "<option value='1'>1 - Terrible</option>";
        print "</select><br>";
        print "Comment:<br><textarea name='comment' cols='60' rows='8'></textarea>";
        print "<br><input type='submit' value='Submit'>";
        print "</form>";

        if (!empty($_GET['name'])) {

            $name = $_GET['name'];
            $rating = $_GET['rating'];
            $comment = $_GET['comment'];

            $query = "INSERT INTO Review (name, mid, rating, comment) VALUES ('" . $name . "'," .$id . "," . $rating . ",'" . $comment . "');";

            if (query($query)) {
                print "<b>Successfully added review!</b>";
            } else {
                print "<b>Sorry bro, bad SQL. Maybe invalid syntax, or the command violated a CHECK constraint. Bummer.</b>";
            }
        }
    ?>
<html>
