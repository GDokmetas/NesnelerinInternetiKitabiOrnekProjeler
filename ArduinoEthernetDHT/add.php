<?php
   	include("connect.php");
   	
   	$link=Connection();

	$ahum1=$_POST["ahum1"];
	$temp1=$_POST["temp1"];
	$light=$_POST["light"];

	$query = "INSERT INTO `tempLog` (`ahum1`, `temp1`, `light`) 
		VALUES ('".$ahum1."','".$temp1."','".$light."')"; 
   	
   	mysql_query($query,$link);
	mysql_close($link);

   	header("Location: index.php");
?>
