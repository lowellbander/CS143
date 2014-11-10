<!DOCTYPE html>
<html>
    <head>
        <title>Add a Movie</title>
    </head>
    <body>
        <a href="./index.php"><- Go Home<a><br>
        <h1>Add a Movie</h1>
        <form>
            Title: <input type="text" name="title"><br>
            Year: <input type="text" name="year"><br>
            Company: <input type="text" name="company"><br>
            MPAA Rating: <select name="rating">
            <option value="G">G</option>
            <option value="PG-13">PG-13</option>
            <option value="PG">PG</option>
            <option value="R">R</option>
            <option value="NC-17">NC-17</option>
            </select>
            Genre: <input type="checkbox" name="genre_Action" value="Action">Action</input>
            <input type="checkbox" name="genre_Adult" value="Adult">Adult</input>
            <input type="checkbox" name="genre_Adventure" value="Adventure">Adventure</input>
            <input type="checkbox" name="genre_Animation" value="Animation">Animation</input>
            <input type="checkbox" name="genre_Comedy" value="Comedy">Comedy</input>
            <input type="checkbox" name="genre_Crime" value="Crime">Crime</input>
            <input type="checkbox" name="genre_Documentary" value="Documentary">Documentary</input>
            <input type="checkbox" name="genre_Drama" value="Drama">Drama</input>
            <input type="checkbox" name="genre_Family" value="Family">Family</input>
            <input type="checkbox" name="genre_Fantasy" value="Fantasy">Fantasy</input>
            <input type="checkbox" name="genre_Horror" value="Horror">Horror</input>
            <input type="checkbox" name="genre_Musical" value="Musical">Musical</input>
            <input type="checkbox" name="genre_Mystery" value="Mystery">Mystery</input>
            <input type="checkbox" name="genre_Romance" value="Romance">Romance</input>
            <input type="checkbox" name="genre_Sci-Fi" value="Sci-Fi">Sci-Fi</input>
            <input type="checkbox" name="genre_Short" value="Short">Short</input>
            <input type="checkbox" name="genre_Thriller" value="Thriller">Thriller</input>
            <input type="checkbox" name="genre_War" value="War">War</input>
            <input type="checkbox" name="genre_Western" value="Western">Western</input>
            <br>
            <input type="submit" value="Add Movie">
        </form>
    </body>
    <?php
        include 'exe_q.php';

        //get Max movie ID
        $getCurrentMaxQuery = "SELECT * FROM MaxMovieID;";
        $maxID = '';
        $result = query($getCurrentMaxQuery);
        if(is_resource($result)){
            $row1 = mysql_fetch_row($result);
            $maxID = $row1[0];
        }
        else if($result){
            print "<br>Fetching the maxMovieID returned true instead of resource";
        }
        else{
            print "Getting current max query returned false";
        }
        
        if (!empty($_GET['title'])) {

            //build SQL query from URL params

            $title = $_GET['title'];
            $year = $_GET['year'];
            $rating = $_GET['rating'];
            $company = $_GET['company'];

            $newID = $maxID + 1;

            $query = "INSERT INTO Movie VALUES (" . $newID . ",'" . $title . "'," . $year .",'" . $rating ."','" . $company ."');";

            // execute query
            
            $rs = query($query);
            
            // echo success or failure
            if ($rs) {
                print "<b>Sucessfully added Movie!</b>";

                //Updating the maxID now
                $updateIDQuery = "UPDATE MaxMovieID SET id = " . $newID . " WHERE id = " . $maxID . ";";
                $updateIDresult = query($updateIDQuery);
                if(!$updateIDresult)
                {
                    print "Failed to update ID. Further updates will likely fail.";
                }

            } else {
                    print "<b>Sorry bro, bad SQL. Maybe invalid syntax, or the command violated a CHECK constraint. Bummer.</b>";
            }


            // update the MovieGenre table
            $genres = array();
            foreach ($_GET as $key => $value) {
                if (strpos($key, 'genre_') !== false) {
                    array_push($genres, $value);
                }
            }

            foreach ($genres as $value) {
                $query = "INSERT INTO MovieGenre VALUES ($newID, '$value');";
                query($query);
            }
        }
    ?>
<html>
