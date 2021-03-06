<!DOCTYPE html>
<html>
    <head>
        <title>BBDB - Bakshi Bander Database</title>
        <style>
        	body{
        		width: 40em;
        		margin: 0 auto;
        	}
            #nav li{
                float: left;
                width:24%;
                display: inline;
                margin-bottom: 10px;
   
            }
        </style>
    </head>
    <body>
        <h1>BBDB - Bakshi Bander Database</h1>
        <div id="nav">
            <ul>
                <li><a href="./index.php">Index</a></li>
            </ul>
        </div>
        <form action="./search.php" method="GET">
            Search : <input type="text" name="search">
            <input type="submit" value="Search Actor/Movie">
        </form>
    </body>
    <?php
                if (!empty($_GET['search'])) {
                    include_once 'exe_q.php';

                    $searchString = $_GET['search'];
                     $searchTerms = explode(' ', $searchString);
                    if(!$searchTerms || count($searchTerms) == 0){
                        //TODO: do nothing 
                        print "Nothing in search terms";
                        exit(1);
                    }
                    //search actors

                    $searchActorQuery = "SELECT DISTINCT id, first, last, dob FROM Actor WHERE first LIKE '%" . $searchTerms[0] . "%' OR last LIKE '%" . $searchTerms[0] . "%' ";
                    for($i = 1; $i < count($searchTerms) ; $i++)
                    {
                        $searchActorQuery .= "OR first LIKE '%" . $searchTerms[$i] . "%' OR  last LIKE '%" . $searchTerms[$i] . "%' " ;
                    }
                    
                    $searchActorQuery .= ";";
                    print "<br>";

                    $actorResult = query($searchActorQuery);
                    if($actorResult){
                        print "<ul>";
                        while($row = mysql_fetch_array($actorResult)){
                            print "<li>Actor: <a href='./actorInfo.php?id=" . $row['id'] . "'>" . $row['first'] . " " . $row['last'] . " (" . $row['dob'] . ")</a></li>";
                        }
                        print "</ul>";

                    }
                    else{
                        print "Failed actor search. Go to LA and find some!";
                    }

                    //search movies now

                    $searchMovieQuery = "SELECT DISTINCT id, title, year FROM Movie WHERE title LIKE '%" . $searchTerms[0] . "%' ";
                    
                    for($i = 1; $i < count($searchTerms) ; $i++)
                    {
                        $searchMovieQuery .= "OR title LIKE '%" . $searchTerms[$i] . "%' ";
                    }
                    $searchMovieQuery .= ";";
                    
                    $movieResult = query($searchMovieQuery);
                    
                    if($movieResult){
                        print "<ul>";
                        while($row = mysql_fetch_array($movieResult)){
                             print "<li>Movie: <a href='./movieInfo.php?id=" . $row['id'] . "'>" . $row['title'] . " (" . $row['year'] . ")</a></li>";
                        }
                        print "</ul>";
                    }
                    else{
                        print "Failed movie search. Go to LA and find some!";
                    }

                }
                else{
                    //do nothing
                }

    ?>
<html>
