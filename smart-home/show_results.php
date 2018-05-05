<html>
   
   <head>
      <title>Roy's Project Database</title>
	  <style>
		table td {border-style:solid;}
	</style>
   </head>
   
   <body>
	<table class="results">
		<tr><td>bsdID</td><td>Time</td><td>alarmSystem</td><td>waterDetect</td></tr>
		<form action="test.php" method="get"/>
      <?php
         $dbhost = '127.0.0.1:3306';
         $dbuser = 'root';
         $dbpass = '1234';
         
         $rec_limit = 15;
         $conn = @mysql_connect($dbhost, $dbuser, $dbpass);
         
         if(! $conn ) {
            die('Could not connect: ' . mysql_error());
         }
         mysql_select_db('testdb');
         
         /* Get total number of records */
         $sql = "SELECT count(bsdID) FROM testdata ORDER BY bsdID ";
         $retval = mysql_query( $sql, $conn );
         
         if(! $retval ) {
            die('Could not get data: ' . mysql_error());
         }
         $row = mysql_fetch_array($retval, MYSQL_NUM );
         $rec_count = $row[0];
         
         if( isset($_GET{'page'} ) ) {
            $page = $_GET{'page'};
            $offset = $rec_limit * $page ;
         }else {
            $page = 1;
            $offset = 0;
         }
         
         $left_rec = $rec_count - ($page * $rec_limit);
         $sql = "SELECT bsdID, time, alarmSystem, waterDetect ". 
            "FROM testdata ".
            "LIMIT $offset, $rec_limit";
            
         $retval = mysql_query( $sql, $conn );
         
         if(! $retval ) {
            die('Could not get data: ' . mysql_error());
         }
         
         while($row = mysql_fetch_array($retval, MYSQL_ASSOC)) {
            echo "<tr><td> {$row['bsdID']}  </td> ".
               "<td> {$row['time']} </td> ".
               "<td> {$row['alarmSystem']} </td> ".
			   "<td> {$row['waterDetect']} </td>> ".
               "</tr>";
         }

       if( $page > 0 ) {
			$last = $page - 1;
			$nextpage = $page + 1;
            echo "<a href = \"test.php?page=$last\">Last 10 Records</a> |";
            echo "<a href = \"test.php?page=$nextpage\">Next 10 Records</a>";
         }else if( $page == 0 ) {
			 $nextpage = $page + 1;
            echo "<a href = \"test.php?page=$nextpage\">Next 10 Records</a>";
         }else if(( $left_rec < $rec_limit ) && ($page > 0)) {
            $last = $page - 1;
            echo "<a href = \"test.php?page=$last\">Last 10 Records</a>";
         }
		 mysql_close($conn);
		 ?>
		 <?php
		 if (isset($_GET['submit_button'])) {
			$dbhost = '127.0.0.1:3306';
			$dbuser = 'root';
			$dbpass = '1234';
			
			$conn = @mysql_connect($dbhost, $dbuser, $dbpass);
			
			if(! $conn ) {
				die('Could not connect: ' . mysql_error());
			}
			mysql_select_db('testdb');
			
			/* Get total number of records */
			$sql = "TRUNCATE TABLE `testdata`";
			$retval = mysql_query( $sql, $conn );
			        mysql_close($conn);
			}
		echo "</table></br>";
		echo "<input name='submit_button' type='submit' value='Reset' />";

      ?>
	  </form>
   </body>
</html>