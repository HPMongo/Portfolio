<?php
	error_reporting(E_ALL);
	ini_set('display_errors','On');
	ob_start();
?>
<?php
	include 'login.php';
//	Connect to the database
	$mysqli = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
		
	if (mysqli_connect_error()) {
	    die('Connect Error (' . mysqli_connect_errno() . ') '
	            . mysqli_connect_error());
	}
//	Proceed with the deletion
	$sql="DELETE FROM video_inventory;";

	if(mysqli_query($mysqli, $sql)) {
		ob_end_clean();
//The deletion is successful, redirect to home page
		header ('Location: index.php');
	} else {
		echo "Error: ".$sql."<br>".mysql_error($mysqli);
	}
	$mysqli->close();
?>
