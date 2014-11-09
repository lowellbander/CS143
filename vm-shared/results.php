<!DOCTYPE html>
<html>
    <head>
        <title>Search Results</title>
    </head>
    <body>
        <h1>Search Results</h1>

        <?php
            if (!empty($_GET['query'])) {
                $foo = $_GET['query'];
                print "foo: $foo";
                        
            }
        ?>
    </body>
<html>
