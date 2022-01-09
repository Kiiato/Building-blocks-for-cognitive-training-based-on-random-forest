<?php
    header('Content-type:application/json;charset:utf-8');
    //取出最近n条历史数据
    $mysql_server_name = 'localhost'; //改成自己的mysql数据库服务器
    $mysql_username = 'root'; //改成自己的mysql数据库用户名
    $mysql_password = '12345678'; //改成自己的mysql数据库密码
    $mysql_database = 'jm_project'; //改成自己的mysql数据库名
    $conn=mysqli_connect($mysql_server_name,$mysql_username,$mysql_password,$mysql_database); //连接数据库


    //使数据库支持中文显示
    mysqli_query($conn,"set names 'utf8'");
    //连接数据库错误提示
    if(mysqli_connect_errno($conn))
    {
    	die("连接MySQL失败：".mysqli_connect_error());
    }
    //echo "连接成功！！！\n";
	    
    //取出用户名称与选择的预测模型
    //$model = $_POST["mode"];
    $username = $_POST["username"];

    for($i=0;$i<3;$i++)
    {
	$model='mode'.($i+1);

    //更新历史数据，发回给小程序
    if(mysqli_num_rows(mysqli_query($conn,"SHOW TABLES LIKE '{$model}_{$username}'"))==1)
    {
	$date_result = mysqli_query($conn,"select distinct curr_date from {$model}_{$username}");
	$date_rows = mysqli_num_rows($date_result);
	$date_data = mysqli_fetch_all($date_result,MYSQLI_ASSOC);
	$limit_date = $date_data[$date_rows-7]['curr_date'];
	//$last_7_days = $rows-7-1;
	
	$query = "select curr_date,sum(label) as label_sum,count(label) as label_count
		from {$model}_{$username}
		where (`curr_date` >= '{$limit_date}')
		group by curr_date";
		//where('curr_datetime' > '2021-04-02 00:00:00')
	//group by label desc limit 10";

	$result = mysqli_query($conn,$query);
	$model_data = mysqli_fetch_all($result,MYSQLI_ASSOC);
	for($j=0;$j<7;$j++)
	{
	    $model_score[$j] = round($model_data[$j]['label_sum']/$model_data[$j]['label_count'],2);
	}
	//数据整理，包括日期与平均得分
	$data['score_'.$model] = $model_score;
	for($j=0;$j<7;$j++)
	    $data['date_mode'.($i+1)][$j] = $date_data[$j+$date_rows-7]['curr_date'];
	//echo "用户-{$username}-的最新数据：";
	//echo json_encode(mysqli_fetch_all($result,MYSQLI_ASSOC),JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);
        //var_dump(mysqli_fetch_all($result,MYSQLI_ASSOC));
    }
    //else
    //{
        //var_dump(mysqli_error($conn));
    //}

    }
    //发送回小程序
    echo json_encode($data,JSON_UNESCAPED_UNICODE|JSON_PRETTY_PRINT);

    mysqli_free_result($result);
    mysqli_close($conn);
?>
