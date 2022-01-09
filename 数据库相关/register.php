<?php

$mysql_server_name = 'localhost'; //改成自己的mysql数据库服务器

$mysql_username = 'root'; //改成自己的mysql数据库用户名

$mysql_password = '12345678'; //改成自己的mysql数据库密码

$mysql_database = 'jm_project'; //改成自己的mysql数据库名

$link=mysqli_connect($mysql_server_name,$mysql_username,$mysql_password,$mysql_database); //连接数据库

//连接数据库错误提示
if (mysqli_connect_errno($link)){
        
    die("连接 MySQL 失败: ".mysqli_connect_error()); 
}

mysqli_query($link,"set names 'utf8'");

//导入数据库
$info['name'] = mysqli_real_escape_string($link,$_POST['uname']);  //对特殊字符进行转义
$info['pw'] = mysqli_real_escape_string($link,$_POST['pwd']);

$query = "insert into users(name,password) values('{$info['name']}',md5('{$info['pw']}'))";
//$result = mysqli_query($link,$squry);
if(mysqli_real_query($link,$query))
{
	echo "名字：",$info['name'],"--->";
	echo "恭喜你，"."注册成功！！！";
}
else
{
	var_dump(mysqli_error($link));
}


//mysqli_free_result($result);
mysqli_close($link);

?>
