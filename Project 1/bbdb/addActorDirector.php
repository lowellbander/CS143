<!DOCTYPE html>
<html>
    <head>
        <title>Add An Actor or Director</title>
    </head>
    <body>
        <a href="./index.php"><- Go Home<a><br>
        <h1>Add An Actor or Director</h1>
        <form>
            Role: <input checked="checked" type="radio" name="role" value="actor">Actor
<input type="radio" name="role" value="director">Director<br>
            First Name: <input type="text" name="first"><br>
            Last Name: <input type="text" name="last"><br>
            Sex: <input type="radio" name="sex" value="male">Male
<input type="radio" name="sex" value="female">Female<br>
            Date of Birth: <input type="text" name="dob"><br>
            Date of Death: <input type="text" name="dod"><br>
            <input type="submit" value="Add Actor/Director">
        </form>
    </body>
    <?php
        include 'exe_q.php';

        //get Max person ID
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

            if ($table == "Actor") 
                $query = "INSERT INTO " . $table . " VALUES (" . $newID . ",\"" . $last . "\",\"" . $first . "\",\"" . $sex ."\",\"" . $dob ."\",\"" . $dod ."\");";
            else
                $query = "INSERT INTO " . $table . " VALUES (" . $newID . ",\"" . $last . "\",\"" . $first . "\",\"" . $dob ."\",\"" . $dod ."\");";

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
