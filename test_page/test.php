<?php
  ini_set('max_execution_time', 1000);
  $in_text = $_POST['text'];
  $fp = fopen("input.txt",'w');
  fwrite($fp, $in_text);
  fclose($fp);

  $command1 = "ranking_1.exe";
  $result1 = shell_exec($command1);
  $result_arr1 = explode(':', $result1);
  $command2 = "ranking_2.exe";
  $result2 = shell_exec($command2);
  $result_arr2 = explode(':', $result2);
  $command3 = "ranking_3.exe";
  $result3 = shell_exec($command3);
  $result_arr3 = explode(':', $result3);
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
            <li class="active"><a href="index.php">Test</a></li>
            <li><a href="result.php">Result</a></li>
          </ul>
        </div><!--/.nav-collapse -->
      </div>
    </nav>

    <div class="container theme-showcase" role="main">

      <!-- Main jumbotron for a primary marketing message or call to action -->
      <div class="jumbotron">
        <h1>Text Understading Test</h1>
        <p>주어진 지문을 읽고 맞게 분류된 결과를 중복선택하여 제출해주세요.</p>
      </div>

      <form name=run_exe action="index.php" method="post">
        <div class="page-header">
          <h1>Text</h1>
        </div>
        <textarea class="well" name="text" style="width: 100%;height: 150px;"><?php print($in_text); ?></textarea>
      </form>

      <div class="page-header">
        <h1>Choice</h1>
      </div>
      <div class="row">
        <div class="col-sm-4">
          <div class="panel panel-info">
            <div class="panel-heading">
              <h3 class="panel-title">METHOD A</h3>
            </div>
            <div class="panel-body" style="overflow:hidden;word-wrap:break-word;"><?php for($i=0; $i<count($result_arr1); $i++){ print($result_arr1[$i]."<br>"); } ?></div>
          </div>
        </div><!-- /.col-sm-4 -->
        <div class="col-sm-4">
          <div class="panel panel-success">
            <div class="panel-heading">
              <h3 class="panel-title">METHOD B</h3>
            </div>
            <div class="panel-body" style="overflow:hidden;word-wrap:break-word;"><?php for($i=0; $i<count($result_arr2); $i++){ print($result_arr2[$i]."<br>"); } ?></div>
          </div>
        </div><!-- /.col-sm-4 -->
        <div class="col-sm-4">
          <div class="panel panel-warning">
            <div class="panel-heading">
              <h3 class="panel-title">METHOD C</h3>
            </div>
            <div class="panel-body" style="overflow:hidden;word-wrap:break-word;"><?php for($i=0; $i<count($result_arr3); $i++){ print($result_arr3[$i]."<br>"); } ?></div>
          </div>
        </div><!-- /.col-sm-4 -->
      </div>

      <form method='post' action='select.php'>
        <input TYPE='checkbox' id='r1' name='check[]' value='1' /><label for='r1' style="margin-left: 5px;vertical-align: middle;">METHOD A</label><br>
        <input TYPE='checkbox' id='r2' name='check[]' value='2' /><label for='r2' style="margin-left: 5px;vertical-align: middle;">METHOD B</label><br>
        <input TYPE='checkbox' id='r3' name='check[]' value='3' /><label for='r3' style="margin-left: 5px;vertical-align: middle;">METHOD C</label><br>
        <input class="btn btn-primary" type='submit' value='Submit »' />
      </form>

    </div> <!-- /container -->
    <br><br><br><br>

    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js"></script>
    <script>window.jQuery || document.write('<script src="../../assets/js/vendor/jquery.min.js"><\/script>')</script>
    <script src="../../dist/js/bootstrap.min.js"></script>
    <script src="../../assets/js/docs.min.js"></script>
  </body>
</html>
