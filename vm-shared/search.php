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
        <form>
            Search : <input type="text" name="search">
            <input type="submit" value="Search Actor/Movie">
        </form>
    </body>
    <?php
                if (!empty($_GET['search'])) {
                    $searchString = $_GET['search'];
                    $searchTerms = explode(' ', $searchString);
                    if(!$searchTerms || count($searchTerms) == 0){
                        //TODO: do nothing 
                        exit(1);
                    }

                    
                }

    ?>
<html>

//Get search string
//tokenize based on 'spaces'
//conduct search on actor and movie based on tokens
//merge actor results into one
//merge movie results into one
//wildcards at the end? what about beginning?