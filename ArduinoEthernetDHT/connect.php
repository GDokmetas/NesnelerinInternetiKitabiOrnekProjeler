<?php

	function Connection(){
		$server="db_server";	 // veri tabanı sunucusu
		$user="username";
		$pass="password";
		$db="db_name";		// veritabanı adı

		//Hosting firmasından alacağını bilgiler.
	   	
		$connection = mysql_connect($server, $user, $pass);

		if (!$connection) {
	    	die('MySQL ERROR: ' . mysql_error());
		}
		
		mysql_select_db($db) or die( 'MySQL ERROR: '. mysql_error() );

		return $connection;
	}
?>

