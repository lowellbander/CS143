<?php
    function query($query_string) {
    
	$db_connection = mysql_connect("localhost", "cs143", "");
    if(!$db_connection) {
        $errmsg = mysql_error($db_connection);
        print "connection failed: $errmsg <br />";
        exit(1);
    }
	mysql_select_db("cs143", $db_connection);

    $query = $query_string;

    print "<br><b>your query: </b>$query<br><br>";
    mysql_query("USE CS143;");
    $rs = mysql_query($query, $db_connection);
    
    if (is_resource($rs)) {
        //valid sql query, so return the result
        return $rs;
    } else if ($rs) {
        // insert/update/delete was successful
        return True;
    } else {
        // invalid sql query
        //print mysql_error();
        return False;
    }

	// close the connection when done
	mysql_close($db_connection);
    }
?>
