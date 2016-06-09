<?php

	include("connect.php"); 	
	
	$link=Connection();

	$result=mysql_query("SELECT * FROM `tempLog` ORDER BY `timeStamp` DESC LIMIT 48",$link);
?>
	
<html>
	<head>
		<title>Project: E-Pot - an intelligent pot for plants</title>
		<meta charset="utf-8">
		<meta name="viewport" content="width=device-width, initial-scale=1">

		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap.min.css">
		<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/css/bootstrap-theme.min.css">
		<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
    </head>
<body>
<?php
	include("header.php");
?>
	
   <h3>E-Pot</h3>

   <table class="table">
	   <thead>
			<tr>
				<th>Date</th>
				<th>Air humidity [%]</th>
				<th>Temperature [C]</th>
				<th>Light [ON/OFF]</th>
			</tr>
		</thead>
		<tbody>

      <?php 
		  if($result!==FALSE){
		     while($row = mysql_fetch_array($result)) {
		        printf("<tr><td> &nbsp;%s </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td><td> &nbsp;%s&nbsp; </td></tr>", 
		           $row["timeStamp"], $row["ahum1"], $row["temp1"], $row["light"]);
		     }
		     mysql_free_result($result);
		     mysql_close();
		  }
      ?>
		</tbody>
   </table>
</body>
</html>

