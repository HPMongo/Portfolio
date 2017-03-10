<?php
	error_reporting(E_ALL);
	ini_set('display_errors','On');
	ob_start();
?>
<?php
	function deleteItem($inItem) {
		include 'login.php';
//	Connect to the database
		$mysqli = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
		
		if (mysqli_connect_error()) {
		    die('Connect Error (' . mysqli_connect_errno() . ') '
		            . mysqli_connect_error());
		}
//	Proceed with the deletion
		$sql="DELETE FROM video_inventory WHERE id = '$inItem';";

		if(mysqli_query($mysqli, $sql)) {
			ob_end_clean();
		//The deletion is successful, redirect to home page
			header ('Location: index.php');
		} else {
			echo "Error: ".$sql."<br>".mysql_error($mysqli);
		}
		$mysqli->close();
	}

	if(isset($_GET['id'])) {
		$inID = $_GET['id'];
		if(!ctype_digit($inID)) {
			echo "Invalid input for delete<br>";
		} else {
			echo "Deleting item #".$inID."<br>";
			deleteItem($inID);
		}
	}
?>
