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
            $sanitized_query = mysql_real_escape_string($query, $db_connection);
            $rs = mysql_query($sanitized_query, $db_connection);
            $nFields = mysql_num_fields($rs);

            print "<b>YOUR QUERY: </b>$sanitized_query<br><br>";
            
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
		    // close the connection when done
		    mysql_close($db_connection);
        }
	?>

	</body>
</html>
