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
            Rating: <input type="text" name="rating"><br>
            Company: <input type="text" name="company"><br>
            <input type="submit" value="Add Movie">
        </form>
    </body>
    <?php
        include 'exe_q.php';

        //get Max movie ID
        $getCurrentMaxQuery = "SELECT * FROM MaxPersonID";
        $maxID = '';
        $result = query($getCurrentMaxQuery);
        if(is_resource($result)){
            $row1 = mysql_fetch_row($result);
            $maxID = $row1[0];
        }
        else if($result){
            print "<br>Fetching the maxPersonID returned true instead of resource";
        }
        else{
            print "Getting current max query returned false";
        }
        
        if (!empty($_GET['role'])) {

            //build SQL query from URL params
            
            $table = '';
            if ($_GET['role'] == "actor") $table = 'Actor';
            else $table = 'Director';

            $first = $_GET['first'];
            $last = $_GET['last'];
            
            $sex = "";
            if ($_GET['sex'] == "male") $sex = 'Male';
            else $sex = 'Female';

            $dob = $_GET['dob'];
            $dod = $_GET['dod'];
            $newID = $maxID + 1;
            $query = "INSERT INTO " . $table . " VALUES (" . $newID . ",\"" . $last . "\",\"" . $first . "\",\"" . $sex ."\",\"" . $dob ."\",\"" . $dod ."\");";

            // execute query
            
            $rs = query($query);
            
            // echo success or failure
            if ($rs) {
                print "<b>Sucessfully added Actor/Director!</b>";

                //Updating the maxID now
                $updateIDQuery = "UPDATE MaxPersonID SET id = " . $newID . " WHERE id = " . $maxID . ";";
                $updateIDresult = query($updateIDQuery);
                if(!$updateIDresult)
                {
                    print "Failed to update ID. Further updates will likely fail.";
                }

            } else {
                    print "<b>Sorry bro, bad SQL. Maybe invalid syntax, or the command violated a CHECK constraint. Bummer.</b>";
            }
        }
    ?>
<html>
