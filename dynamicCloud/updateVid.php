<?php
	error_reporting(E_ALL);
	ini_set('display_errors','On');
	include_once 'login.php';
	ob_start();
?>
<?php
/*
	This function will validate add request 
*/
	function updateValidate(&$inArr) {
		$validAdd = true;
		$inID;
		$inTime; 
		$inCat; 
		$inStat;
		$inSub = 0;
		$inStudio;	
		if(isset($_POST['id'])) {
			$inID = $_POST['id'];
			if(!ctype_digit($inID)) {
				echo "Error - ID needs to be numeric.<br>";
				$validAdd = false;
			} else {
				echo "ID: ".$inID."<br>";	
			}
		} else {
			echo "Error - missing ID.<br>";
			$validAdd = false;
		}

		if(isset($_POST['cat'])) {
			$inCat = $_POST['cat'];
			if($inCat != "") {
				echo "Category: ".$inCat."<br>";
			} else {
				echo "Error - Invalid category.<br>";
			}
		} else {
			echo "Error - missing category.<br>";
			$validAdd = false;	
		}

		if(isset($_POST['time'])) {
			$inTime = $_POST['time'];
			if(!ctype_digit($inTime)) { 
				echo "Error - Run-time needs be numeric.<br>";
				$validAdd = false;			
			} else {
				echo "Run-time: ".$inTime."<br>";
			}
		} else {
			echo "Error - missing run-time.<br>";
			$validAdd = false;		
		}

		if(isset($_POST['status'])) {
			$inStat = $_POST['status'];
			echo "Available: ".$inStat."<br>";
		} else {
			echo "Error - missing availability.<br>";
			$validAdd = false;			
		}

		if(isset($_POST['subtitle'])) {
			$temp = $_POST['subtitle'];
			if($temp == 'on')
				$inSub = 1;
			echo "Subtitle: ".$inSub."<br>";
		} else {
			echo "Setting subtitle to no.<br>";
		}

		if(isset($_POST['studioURL'])) {
			$inStudio = $_POST['studioURL'];
			echo "URL: ".$inStudio."<br>";
		} else {
			echo "Error - missing studio.<br>";
			$validAdd = false;			
		}
		if($validAdd) {
			$inArr[0] = $inID;
			$inArr[1] = $inCat;
			$inArr[2] = $inTime;
			$inArr[3] = $inStat;
			$inArr[4] = $inSub;
			$inArr[5] = $inStudio;	
			return true;
		} else {
			return false;
		}
	}
/*
	This function will add the entry to the table
*/
	function updateVideo($inArr, $inConn) {
		$sql = "INSERT INTO video_inventory (name, category, length, rented, subtitle, studio) VALUES ('$inArr[0]', '$inArr[1]', '$inArr[2]', '$inArr[3]', '$inArr[4]', '$inArr[5]');";
		$sql = "UPDATE video_inventory SET category = '$inArr[1]', length = '$inArr[2]', rented = '$inArr[3]', subtitle = '$inArr[4]', studio = '$inArr[5]' WHERE id = '$inArr[0]';";
		if(mysqli_query($inConn, $sql)) {
// 	Close the database connection
			$inConn->close();
			ob_end_clean();
//	Redirect to homepage after the success add
			header('Location: index.php');
		} else {
			echo "Error: ".$sql."<br>";
			echo "Error code:".mysqli_error($inConn)."<br>";
			$inConn->close();
			echo "It looks like you have selected an invalid movie ID. Click <a href='index.php'> here</a> to go back and correct your entry or add a different title.<br>";
		}
	}

/*
	Main logic of the script
*/
	$addArray = array();
// 	Connecting to the database ;
	$mysqli = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
	
	if (mysqli_connect_error()) {
	    die('Connect Error (' . mysqli_connect_errno() . ') '
	            . mysqli_connect_error());
	}
//	Add new entry to the table
	if(isset($_POST['updateVid'])) {
		if(updateValidate($addArray)) {
			updateVideo($addArray, $mysqli);
		} else {
// 	Close the database connection
			$mysqli->close();
			echo "There are errors with the input. Click <a href='index.php'> here</a> to go back and correct your entry.<br>";
		}
	}
?>
