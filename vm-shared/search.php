<!DOCTYPE html>
<html>
    <head>
        <title>BBDB - Bakshi Bander Database</title>
        <style>
        	body{
        		width: 800px;
        		margin: 0 auto;
        	}
            #nav li{
                float: left;
                width:24%;
                display: inline;
                margin-bottom: 10px;
   
            }
            .content {
                float: right;
                width:74%;
            }
        </style>
    </head>
    <body style="overflow:hidden">
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

                    $searchActorQuery = 'SELECT id,first, last, dob FROM Actor WHERE ';
                    for($i = 0; $i < count($searchTerms) ; $i++)
                    {
                         $searchActorQuery .= "first LIKE '%" . $searchTerms[$i] . "%' OR  last LIKE '%" . $searchTerms[$i] . "%'" ;
                    }
                    
                    $searchActorQuery .= ";";
                    print $searchActorQuery;
                    
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

                    $searchMovieQuery = "SELECT id,title,year FROM Movie WHERE title LIKE '%" . $searchTerms[0] . "%'";
                    for($i = 1; i < count($searchTerms) ; $i++)
                    {
                        $searchMovieQuery .= "OR title LIKE '%" . $searchTerms[$i] . "%'";
                    }
                    $searchMovieQuery .= ";";
                    print $searchMovieQuery;
                    
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
