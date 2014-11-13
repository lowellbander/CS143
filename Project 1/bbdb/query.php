<!DOCTYPE html>
<html>
	<head>
		<title>Query</title>
	</head>
<body>
	<h1>Query</h1>


    <p>Enter a query, such as: <b>SELECT * FROM Student;</b></p>

    <form method="GET">
        <textarea name="query" cols="60" rows="8"></textarea>
        <input type="submit" value="Submit">
    </form>
    <br>
	
	<?php
        if ( !empty( $_GET['query'])) {
		    $db_connection = mysql_connect("localhost", "cs143", "");
            if(!$db_connection) {
                $errmsg = mysql_error($db_connection);
                print "Connection failed: $errmsg <br />";
                exit(1);
            }
		    mysql_select_db("CS143", $db_connection);

            $query = $_GET["query"];
            $rs = mysql_query($query, $db_connection);

            print "<b>YOUR QUERY: </b>$query<br><br>";
            
            //TODO: can these be made into !$rs, etc.?

            if (is_resource($rs)) {
                //valid SQL query, so return the result
                $nFields = mysql_num_fields($rs);

                print "<table border=1>";
                
                for ($i = 0; $i < $nFields; $i++) {
                    $fields = mysql_fetch_field($rs, $i);
                    print "<th>";
                    print $fields->name;
                    print "</th>";
                }

		        while($row = mysql_fetch_row($rs)) {
                    $printme = "";
                    print "<tr>";
                    for ($i = 0; $i < $nFields; $i++) {
                        print "<td>";
                        $printme += $row[$i] + ", ";
                        print $row[$i];
                        print "</td>";
                    }

                    print "</tr>";
                }
                print "</table>";

            } else if  ($rs) {
                // INSERT/UPDATE/DELETE was successful
                print "<b>Successfully added Actor/Direcor! Rejoice!</b>";
            } else {
                // invalid SQL query
                print "<b>Sorry bro, bad SQL. Maybe invalid syntax, or the command violated a CHECK constraint. Bummer.</b>";
            }

		    // close the connection when done
		    mysql_close($db_connection);
        }
	?>

	</body>
</html>
