
# php 查询mysql数据批量转为PDF文件二（批量使用wkhtmltopdf html导出PDF） - しばかいよう - 博客园
[しばかいよう](https://www.cnblogs.com/chaihy/)

皆さん、こんにちは 欢迎加入技术讨论群599606903


随笔 - 88 文章 - 9 评论 - 2

[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/chaihy/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E3%81%97%E3%81%B0%E3%81%8B%E3%81%84%E3%82%88%E3%81%86)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/chaihy/rss)![订阅](//www.cnblogs.com/images/xml.gif)
[php 查询mysql数据批量转为PDF文件二（批量使用wkhtmltopdf html导出PDF）](https://www.cnblogs.com/chaihy/p/10248478.html)
上节讲到配置wkhtmltopdf，这节讲下如何批量操作
首先讲下wkhtmltopdf如何使用
直接命令行输入：
wkhtmltopdf[http://www.baidu.com/](http://www.baidu.com/)baidu.pdf
php使用
<?php
shell_exec('wkhtmltopdf[http://www.baidu.com/](http://www.baidu.com/)baidu.pdf')  //注：中间两个空格隔开
?>
当前路径下会生成一个PDF文件：
批量操作：
<?php
header("Content-type:text/html;charset=utf-8");
$host='127.0.0.1';
$user='root';
$password='***';
$dbName= "***";
$db=new MySQLi($host,$user,$password,$dbName);
mysqli_set_charset($db,'utf8');
if ($db->connect_error){
die("连接失败：".$db->connect_error);
}
$sql = '***';
$result = $db->query($sql);//返回结果集对象
$n = $result->fetch_all();
echo '<pre>';
$s = 0;
foreach ($n as $v){
$s ++;//判断是否生成
$file = "test_pdf/".$v[0].".pdf";
if(file_exists($file)){
continue;
}$path="test_html/".$v[0].".html"; //命名方式可以id命名，唯一即可
$tem = 'cq/template.html'; //模板HTML文件
$fp=fopen($tem,"r");
//读取模板中内容
$str=fread($fp,filesize($tem));
//替换内容
$str=str_replace("{indentity_code}",$v[0],$str);
$str=str_replace("{name}",$v[1],$str);
***
fclose($fp);
//写入方式打开路径
$handle=fopen($path,"w");
//把刚才替换的内容写进生成的HTML文件
fwrite($handle,$str);
fclose($handle);
//存PDF
shell_exec("wkhtmltopdf localhost/test_html/".$v[0].".html "."test_pdf/".$v[0].".pdf");
echo $v[0].'<br/>';
//检测生成PDF
// if($s == 2  ){
//      break;
//  }
}
通过循环方式执行：<!DOCTYPE HTML>
<meta http-equiv="content-type" content="text/html;charset=utf-8">  <!-- 注：这句加上防止HTML乱码，导致导出PDF乱码-->
<html>
<body>
<link rel="stylesheet" type="text/css" href="../ks.css">
<div class="box">
<span style="width: 30%">姓名：{name}</span>
</div>
</body>
</html>
注：css文件要和生成html文件引入方式一样，这样确保生成后的html文件能在本地访问
以上方式基本上能实现批量数据导出PDF，如有不懂，或者过程中遇到别的问题，乱码等问题加群讨论




posted on2018-10-09 10:34[しばかいよう](https://www.cnblogs.com/chaihy/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=10248478)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)







## 公告



Copyright ©2019 しばかいよう

