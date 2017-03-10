<?php
	error_reporting(E_ALL);
	ini_set('display_errors','On');
	session_start();
	ob_start();
?>
<?php
	if(isset($_GET['cat'])) {
		$catFilter = $_GET['cat'];
		if($catFilter == "" || $catFilter == "All Movies") {
			$_SESSION['catFilter'] = 'All Movies';
			echo "Store default";
		} else {
			$_SESSION['catFilter'] = $catFilter;
			echo "Store actual value";
		}
	} else {
		$_SESSION['catFilter'] = 'All Movies';
		echo "Store default";
	}

	ob_end_clean();
	//Redirect to main page
	header('Location: index.php');
?>
