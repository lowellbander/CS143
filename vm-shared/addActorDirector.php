<!DOCTYPE html>
<html>
    <head>
        <title>Add An Actor or Director</title>
    </head>
    <body>
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

            $query = "INSERT INTO " . $table . " VALUES (1237,\"" . $last . "\",\"" . $first . "\",\"" . $sex ."\",\"" . $dob ."\",\"" . $dod ."\");";

            // execute query
            
            $rs = query($query);
            
            // echo success or failure
            if ($rs) {
                print "<b>INSERT/UPDATE/DELETE was successful</b>";
            } else {
                    print "<b>Sorry bro, bad SQL. Maybe invalid syntax, or the command violated a CHECK constraint. Bummer.</b>";
            }
        }
    ?>
<html>
