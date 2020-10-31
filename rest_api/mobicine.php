<?php

echo "hello world \n";
$servername = "94.73.151.201";
$database = "datayusuf";
$username = "payitaht";
$password = "YBdt115177.";
// Create connection
$conn = mysqli_connect($servername, $username, $password, $database);
// Check connection
if (!$conn) {
    die("Connection failed: " .
mysqli_connect_error());
}
echo "Connected successfully\n";

$sql = "SELECT symtomps FROM mobicine WHERE symptoms LIKE 'In%' ";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
  // output data of each row
  while($row = $result->fetch_assoc()) {
    echo "symtomps: " . $row["symptoms"]. "<br>";
  }
} else {
  echo "0 results";
}
$conn->close();
 ?>
