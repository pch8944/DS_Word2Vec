<?php
  $fp = fopen("result_data.txt",'r');
  $current = fgets($fp);
  fclose($fp);
  $current_arr = explode(":", $current);
  $result_arr[0] = ((int)$current_arr[1] / (int)$current_arr[0]) * 100;
  $result_arr[1] = ((int)$current_arr[2] / (int)$current_arr[0]) * 100;
  $result_arr[2] = ((int)$current_arr[3] / (int)$current_arr[0]) * 100;
?>

<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="../../favicon.ico">

    <title>DS_PROJECT_TEAM10</title>

    <!-- Latest compiled and minified CSS -->
  	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css" integrity="sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u" crossorigin="anonymous">

  	<!-- Optional theme -->
  	<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap-theme.min.css" integrity="sha384-rHyoN1iRsVXV4nD0JutlnGaslCJuC7uwjduW9SVrLvRYooPp2bWYgmgJQIXwl/Sp" crossorigin="anonymous">

  	<!-- Latest compiled and minified JavaScript -->
  	<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js" integrity="sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa" crossorigin="anonymous"></script>
    <link href="bootstrap.css" rel="stylesheet">
    <link href="theme.css" rel="stylesheet">
  </head>

  <body>

    <!-- Fixed navbar -->
    <nav class="navbar navbar-inverse navbar-fixed-top">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="#">Test Sheet</a>
        </div>
        <div id="navbar" class="navbar-collapse collapse">
          <ul class="nav navbar-nav">
            <li><a href="index.php">Test</a></li>
            <li class="active"><a href="#">Result</a></li>
          </ul>
        </div><!--/.nav-collapse -->
      </div>
    </nav>

    <div class="container theme-showcase" role="main">

      <div class="page-header">
        <h1>ACURACCY</h1>
      </div>
      ORIGINAL CLASSIFIER: <?php print ($result_arr[0]); ?> %
      <div class="progress">
        <div class="progress-bar" role="progressbar" aria-valuenow="<?php echo $result_arr[0]?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php echo $result_arr[0]?>%;"><span class="sr-only">60% Complete</span></div>
      </div>
      METHOD 1: <?php print ($result_arr[1]); ?> %
      <div class="progress">
        <div class="progress-bar progress-bar-success" role="progressbar" aria-valuenow="<?php echo $result_arr[1]?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php echo $result_arr[1]?>%"><span class="sr-only">40% Complete (success)</span></div>
      </div>
      METHOD 2: <?php print ($result_arr[2]); ?> %
      <div class="progress">
        <div class="progress-bar progress-bar-warning" role="progressbar" aria-valuenow="<?php echo $result_arr[2]?>" aria-valuemin="0" aria-valuemax="100" style="width: <?php echo $result_arr[2]?>%"><span class="sr-only">60% Complete (warning)</span></div>
      </div>

    </div> <!-- /container -->


    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
    <script>window.jQuery || document.write('<script src="../../assets/js/vendor/jquery.min.js"><\/script>')</script>
    <script src="../../dist/js/bootstrap.min.js"></script>
    <script src="../../assets/js/docs.min.js"></script>
  </body>
</html>
