<?php 
	include 'login.php';
	//error_reporting(E_ALL);
	//ini_set('display_errors','On');
	//ob_start();
?>
<!DOCTYPE html>
<html>
	<head>
		<meta charset="UTF-8">
		<title>Video Inventory</title>
		<link rel="stylesheet" href="style.css">
	</head>
	<body>
		<h4>Add video to the collection</h4>
		<form action="add.php" method="post">
			<label>Title: </label><input id="title1" type="text" name="title" placeholder="Star Wars"><br>
			<label>Category: </label><input id="cat1" type="text" name="cat" placeholder="Fiction"><br>
			<label>Run time: </label><input id="time1" type="number" name="time" placeholder="in minutes"><br>
			<label>Studio: </label><input id="url1" type="url" name="studioURL" placeholder="http://www.production.com"><br>
			<label>English subtitle: </label><input id="sub1" type="checkbox" name="subtitle"><br><br>
			<label>Availability: </label>
				<input type="radio" name="status" value="0"> Checked out
				<input type="radio" name="status" value="1"> Available<br><br>
			<input type="submit" name="addVid" value="Add">
		</form>
		<hr>
		<h4>Update video from the collection</h4>
		<form action="updateVid.php" method="post">
			<label>Movie ID: </label><input id="id2" type="number" name="id" placeholder="10"><br>
			<label>Category: </label><input id="cat2" type="text" name="cat" placeholder="Fiction"><br>
			<label>Run time: </label><input id="time2" type="number" name="time" placeholder="in minutes"><br>
			<label>Studio: </label><input id="url2" type="url" name="studioURL" placeholder="http://www.production.com"><br>
			<label>English subtitle: </label><input id="sub2" type="checkbox" name="subtitle"><br><br>
			<label>Availability: </label>
				<input type="radio" name="status" value="0"> Checked out
				<input type="radio" name="status" value="1"> Available<br><br>
			<input type="submit" name="updateVid" value="Update">
		</form>
		<hr>
	
		<?php 
			include 'display.php';
	//		ob_end_clean();
		?>
		<form action="delete_all.php" method="post">
			<input type="submit" value="Delete all videos??" onCLick="return confirm('Are you SURE you want to delete all videos?')">
		</form>
	</body>
</html>
