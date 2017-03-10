<?php
	error_reporting(E_ALL);
	ini_set('display_errors','On');
	ob_start();
?>
<?php
	function updateItem($inItem, $inStat) {
		include 'login.php';
//	Connect to the database
		$mysqli = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
		
		if (mysqli_connect_error()) {
		    die('Connect Error (' . mysqli_connect_errno() . ') '
		            . mysqli_connect_error());
		}
//	Proceed with the update
		$sql="UPDATE video_inventory SET rented = '$inStat' WHERE id = '$inItem';";

		if(mysqli_query($mysqli, $sql)) {
		ob_end_clean();
		//The update is successful, redirect to home page
			header ('Location: index.php');
		} else {
			echo "Error: ".$sql."<br>".mysql_error($mysqli);
		}
		$mysqli->close();
	}

	$validUpdate = true;
	$inID;
	$inStat;

	if(isset($_GET['id'])) {
		$inID = $_GET['id'];
		if(!ctype_digit($inID)) {
			echo "Invalid id for update<br>";
			$validUpdate = false;
		} 
	}


	if(isset($_GET['stat'])) {
		$inStat = $_GET['stat'];
		if(!ctype_digit($inStat)) {
			echo "Invalid stat for update<br>";
			$validUpdate = false;
		} 
	}

	if($validUpdate) {
			echo "Updating item #".$inID." with ".$inStat."<br>";
			updateItem($inID, $inStat);
	} else {
			$mysqli->close();
			echo "There are errors with the input. Click <a href='index.php'> here</a> to go back and correct your entry.<br>";
	}
?>
