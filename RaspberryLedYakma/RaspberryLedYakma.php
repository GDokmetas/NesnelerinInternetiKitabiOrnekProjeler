<?php
    if (isset($_POST['on']))  
    {  exec("sudo killall python");
	   exec("sudo python /var/www/mystuff/ledON.py"); 
	}
    else if (isset($_POST['off']))  
	{  
       exec("sudo killall python"); 
	   exec("sudo python /var/www/mystuff/ledOFF.py"); 
	}
    else if (isset($_POST['blink']))  
	{               
	    exec("sudo python /var/www/mystuff/ledBLINK.py");
    } 
	?>
	<html> <style type="text/css"> 

	</style>
	<body> 
	<div id="container"> 
	<form id="form" method="post"> 
	<center>
	<button name="bell"><h1>Zil</h1></button> <br><br>
	<button name="on"><h1>Led ACIK</h1></button>  
	<button name="off"><h1>Led KAPALI</h1></button> 
	<button name="blink"><h1>Led YAKSONDUR</h1></button>
	</form>
	</div> 
	</body>
	</html>