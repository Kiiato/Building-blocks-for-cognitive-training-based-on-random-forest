<?php
    header('Content-type:application/json;charset:utf-8');

    $data[]=0;
    foreach ($_POST as $key => $value)
    {
	
	$data[$key] = $value;
    }

    //预测结果
    //$result = 1;
    //调用预测模型进行预测
    $result = 0;
    //模型一
    if (strcmp($_POST['mode'],'mode1') == 0)
    {
	$result = shell_exec("Python /root/anaconda3/myprojects/jm/jm_mode1.py $data[0] $data[1] $data[2] $data[3] $data[4] $data[5] $data[6] $data[7] $data[8] $data[9] $data[10] $data[11] $data[12] $data[13] $data[14] $data[15] $data[16]");
    }
    //模型二
    if (strcmp($_POST['mode'],'mode2') == 0)
    {
	$result = shell_exec("Python /root/anaconda3/myprojects/jm/jm_mode2.py $data[0] $data[1] $data[2] $data[3] $data[4] $data[5] $data[6] $data[7] $data[8] $data[9] $data[10] $data[11] $data[12] $data[13] $data[14] $data[15] $data[16]");
    }
    //模型三
    if (strcmp($_POST['mode'],'mode3') == 0)
    {
	$result = shell_exec("Python /root/anaconda3/myprojects/jm/jm_mode3.py $data[0] $data[1] $data[2] $data[3] $data[4] $data[5] $data[6] $data[7] $data[8] $data[9] $data[10] $data[11] $data[12] $data[13] $data[14] $data[15] $data[16]");
    }
    //模型四
    /*if (strcmp($_POST['mode'],'mode4') == 0)
    {
	$result = shell_exec("Python /root/anaconda3/myprojects/jm/jm4.py $data[0] $data[1] $data[2] $data[3] $data[4] $data[5] $data[6] $data[7] $data[8] $data[9] $data[10] $data[11] $data[12] $data[13] $data[14] $data[15]");
    }*/

    //echo "预测结果为：",$result;
    /*
     * 将预测结果存入数据库
     * */
    
    $mysql_server_name = 'localhost'; //改成自己的mysql数据库服务器
    $mysql_username = 'root'; //改成自己的mysql数据库用户名
    $mysql_password = '12345678'; //改成自己的mysql数据库密码
    $mysql_database = 'jm_project'; //改成自己的mysql数据库名
    $conn=mysqli_connect($mysql_server_name,$mysql_username,$mysql_password,$mysql_database); //连接数据库
    
    //使数据库支持中文显示
    mysqli_query($conn,"set names 'utf8'");

    //连接数据库错误提示
    if (mysqli_connect_errno($conn)) 
    {
        die("连接 MySQL 失败: " . mysqli_connect_error());
    }
    //echo "连接成功!!! \n";
    
    //取出用户名称与选择的预测模型
    $model = $_POST["mode"];    
    $name = $_POST["username"];
    
    $currdate = date('Ymd');

    if(mysqli_num_rows(mysqli_query($conn,"SHOW TABLES LIKE '{$model}_{$name}'"))==1)
    {
        $query = "insert into {$model}_{$name}(label,curr_date) values('{$result}','{$currdate}')";
    }
    else
    {
    	$query = "CREATE TABLE {$model}_{$name} (
		id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY, 
		label INT(4) NOT NULL,
		curr_date date NOT NULL
	)";
	mysqli_query($conn, $query);
	//插入预测结果
	$query = "insert into {$model}_{$name}(label,curr_date) values('{$result}','{$currdate}')";
    }
    
    //将结果发回给小程序
    if(mysqli_real_query($conn,$query))
    {
	$result = ['result'=>$result];
	//$tmp = [1,2,3,4];
	//echo json_encode($tmp,JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);
        echo json_encode($result,JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);
    }
    else
    {
        var_dump(mysqli_error($conn));
    }

    //$output = shell_exec("Python /root/anaconda3/myprojects/jm/test01.py");
    //$output = system("Python /root/anaconda3/myprojects/jm/jm.py 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1",$ret);
    
    //调用预测模型进行预测
    //$output = shell_exec("Python /root/anaconda3/myprojects/jm/jm.py $data[0] $data[1] $data[2] $data[3] $data[4] $data[5] $data[6] $data[7] $data[8] $data[9] $data[10] $data[11] $data[12] $data[13] $data[14] $data[15]");
    

    //$output = system("python /root/anaconda3/myprojects/jm/jm.py $_GET[0] $_GET[1] $_GET[2] $_GET[3] $_GET[4] $_GET[5] $_GET[6] $_GET[7] $_GET[8] $_GET[9] $_GET[10] $_GET[11] $_GET[12] $_GET[13] $_GET[14] $_GET[15]");
    /*$timeflag = True; 
        while($timeflag) {
		sleep(3);
		if (file_exists("tmp.txt")) {
			$timeflag = False;
			break;
		}
	}
     */

    //echo "<br/>";
    //echo "输入的数据(第一个和第二个)：",$data[0],"  ",$data[1] ,"<br/>";
    //echo "命令执行状态：",$ret,"<br/>";
    //echo "判断结果为：",$output,"<br/>";
    //
    mysqli_close($conn);
?>
