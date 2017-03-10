<?php
	//error_reporting(E_ALL);
	//ini_set('display_errors','On');
	include_once 'login.php';
	//ob_start();
	session_start();
?>
<?php
//Constant
	define("DEFAULT_CAT", "All Movies");
/*
	This function will generate the proper query then call another function to populate the
	table
*/
	function pickQuery($mysqli, $inCat) {
		$sql;
		$result;
		$sql1;
		$result1;
		if($inCat == DEFAULT_CAT) {
			$sql = "SELECT * FROM video_inventory;";
			$result = $mysqli->query($sql);
			$sql1 = "SELECT DISTINCT category FROM video_inventory;";
			$result1 = $mysqli->query($sql1);
		} else {
			$sql = "SELECT * FROM video_inventory WHERE category = '$inCat';";
			$result = $mysqli->query($sql);
			$sql1 = "SELECT DISTINCT category FROM video_inventory WHERE category = '$inCat';";
			$result1 = $mysqli->query($sql1);	
		}
		displayResult($mysqli,$sql,$result, $sql1, $result1);
	}
/*
	This function will display the current video inventory 
*/
	function displayResult($mysqli,$sql,$result, $sql1, $result1) {
		//Populate result table
		echo "<link rel='stylesheet' href='style.css'>";
		echo '<h3>',"My videos",'</h3>';
		echo '<table cellpadding="0" cellspacing="0" class="db-table">';
		echo "<tr>	<th>ID</th>
				  	<th>Title</th>
				  	<th>
						<form action='store.php'>
						<select name='cat'>";
						echo "<option value='".DEFAULT_CAT."'>".DEFAULT_CAT."</option>";
						while($row1 = mysqli_fetch_array($result1)){
							$catFilter = $row1['category'];
							echo "<option value='".$catFilter."'>".$catFilter."</option>";
						} 
						echo "<input type='submit' value='Filter by Category'>
						</select></form></th>
				  <th>Run-time</th>
				  <th>Subtitle</th>
				  <th>Studio</th>	
				  <th>Availability</th>
				  <th>Action</th></tr>";
		while($row = mysqli_fetch_array($result)) {
			$id = $row['id'];
			$title = $row['name'];
			$category = $row['category'];
			$runtime = $row['length'];
			$subtitle = $row['subtitle'];
			$studio = $row['studio'];
			$avai = $row['rented'];
			echo "<tr>";
			echo "<td>".$id."</td>";
			echo "<td>".$title."</td>";
			echo "<td>".$category."</td>";
			echo "<td>".$runtime."</td>";
			echo "<td>";
			if($subtitle==0) {
				echo "Not available";
			} else {
				echo "Available";
			}
			echo "</td>";
			echo "<td>".$studio."</td>";
			echo "<td>";
			if($avai==0) {
				echo "<a href='update.php?id=".$id."&stat=1"."' title='Click to check in'>Checked out</a>";
			} else {
				echo "<a href='update.php?id=".$id."&stat=0"."' title='Click to check out'>Available</a>";
			}
			echo "</td>";
			echo "<td> <a href='delete.php?id=".$id."'>Delete</a> </td>";
			echo"</tr>";	
		}
		echo '</table><br>';
	}
/*
	Main logic of the script
*/
// 	Connecting to the database ;
	$mysqli = new mysqli($dbhost, $dbuser, $dbpass, $dbname);
	
	if (mysqli_connect_error()) {
	    die('Connect Error (' . mysqli_connect_errno() . ') '
	            . mysqli_connect_error());
	}
//	Get category from session information to determine which query should be used for 
//	subsequence process
	if (!empty($_SESSION['catFilter'])) {
		$inCat = $_SESSION['catFilter'];
		if($inCat === DEFAULT_CAT) {
			pickQuery($mysqli, DEFAULT_CAT);
		} else {
			pickQuery($mysqli, $inCat);	
		}
	} else {
		pickQuery($mysqli, DEFAULT_CAT);
	}

	//ob_end_clean();

//	Close the connection
	$mysqli->close();
?>
