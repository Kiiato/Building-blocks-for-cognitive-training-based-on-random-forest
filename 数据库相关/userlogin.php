<?php
$mysql_server_name = 'localhost'; //改成自己的mysql数据库服务器

$mysql_username = 'root'; //改成自己的mysql数据库用户名

$mysql_password = '12345678'; //改成自己的mysql数据库密码

$mysql_database = 'jm_project'; //改成自己的mysql数据库名

$conn = new mysqli($mysql_server_name, $mysql_username, $mysql_password, $mysql_database);

$name = mysqli_real_escape_string($conn,$_POST['uname']);
$pwd = mysqli_real_escape_string($conn,$_POST['pwd']);
        if ($conn->connect_error) 
 	{
	    die("Connection failed: " . $conn->connect_error);
	}
	//验证内容是否与数据库一致
	$pwd = md5($pwd);
	$query = "select * from users where (name='{$name}') and (password='{$pwd}')";	
	$result = $conn->query($query);
	
	//判断结果集的记录数是否大于0
	if ($result->num_rows > 0) 
	{
		//$_SESSION['user_account'] = $name;
		$data['ID']=$_POST['uname'];
		$data['password']=$_POST['pwd'];
		echo json_encode($data,JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);
	    //echo "你好, . {$name} . <br/>";
	}
	else
	{
	    echo -1;
	}
        $conn->close(); 
?>
