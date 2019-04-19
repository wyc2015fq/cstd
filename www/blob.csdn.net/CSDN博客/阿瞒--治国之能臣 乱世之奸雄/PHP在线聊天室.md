# PHP在线聊天室 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2012年05月31日 12:09:14[baofenny](https://me.csdn.net/jiaminbao)阅读数：811标签：[php																[xmlhttprequest																[mysql																[input																[聊天																[query](https://so.csdn.net/so/search/s.do?q=query&t=blog)](https://so.csdn.net/so/search/s.do?q=聊天&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=xmlhttprequest&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[学习实例开发](https://blog.csdn.net/jiaminbao/article/category/1158383)
数据库链接文件：conn.php
<?php
$conn=mysql_connect('localhost','root','123456') or die(mysql_error());
mysql_select_db('dev_chat',$conn) or die(mysql_error());
mysql_query('set names \'utf8\''); 
?>
显示页面：index.php
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title></title>
    </head>
    <body bgcolor="yellow">
        <h1>PHP在线聊天室</h1>
        <!--<textarea rows="10" cols="60" name="one" id="one"></textarea>-->
        <iframe src="msgs.php" width="600"height="400" scrolling="no"> </iframe>
        <br/>
        <br/>
        <form method="post" action="write.php"target="five">
            你的昵称：<input type="text" name="two"id="two" value="Guest">
            你的内容：<input type="text" name="three"id="three">
            <input type="submit" id="four" value="发送">
        </form>
        <iframe width="0"height="0"frameborder="0"name="five"> </iframe>
    </body>
</html>
将聊天内容写入数据为文件：write.php
<?php
include("conn.php");
if($_POST['three'] && $_POST['two'])
{$msg = $_POST['three'];
$name = $_POST['two'];
$dateline = date('Y-m-d H:i:s');
$sql='INSERT INTO `msgs` (`id` ,`msg` ,`name` ,`dateline`)VALUES (NULL , \''.$msg.'\', \''.$name.'\', \''.$dateline.'\')'or die(mysql_error());
mysql_query($sql);
print_r($_POST);
}
?>
从数据库中查询聊天内容并输出页面：msgs.php
<?php
include("conn.php");
$sql = "SELECT * FROM `msgs` ORDER BY id DESC LIMIT 10 ";
$query = mysql_query($sql);
$rows=array();
while($row = mysql_fetch_array($query,MYSQL_ASSOC)){
$rows[]=$row;
}
?>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<meta http-equiv="refresh" content="1;URL=msgs.php" />
<?php 
echo"<font color=red>系统消息</font>：欢迎光临PHP在线聊天系统<br><br>";
$rows = array_reverse($rows);
foreach($rows as $k=>$row){
echo '<font color=red>'.$row['name'].'</font>',':',$row['msg'],':','<font color=bule>'.$row['dateline'].'</font>',"\r\n<br>";
} 
echo date('Y-m-d H:i:s');
?>
_________________________________________________________
_________________________________________________________
另外用ajax写显示首页内容为：
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "[http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd](http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd)">
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title></title>
  <script>
var request;
function createRequest() {
  try {
    request = new XMLHttpRequest();
  } catch (trymicrosoft) {
    try {
      request = new ActiveXObject("Msxml2.XMLHTTP");
    } catch (othermicrosoft) {
      try {
        request = new ActiveXObject("Microsoft.XMLHTTP");
      } catch (failed) {
        request = false;
      }
    }
  }
  if (!request)
    alert("Error initializing XMLHttpRequest!");
}
function updatePage() {
     if (request.readyState == 4) {
       if (request.status == 200) {
         var response = request.responseText;
          document.getElementById("msgs").innerHTML = response;
       } else
         alert("status is " + request.status);
     }
   }
function update_msgs(){
 var url = 'msgs2.php';
 request.open("GET", url, true);
 request.onreadystatechange = updatePage;
 request.send(null);
  setTimeout('update_msgs()',1000);
}
</script>      
    </head>
    <body>
        <h1>PHP在线聊天系统(AJAX更新消息内容)</h1>
  <div id="msgs">
  </div>
          <br />
          <br />
        <form action="write.php" method="post" name="text" target="write_frame" >
           <!--<p> <textarea cols="80" rows="10" id="msgs"></textarea></p>-->
            你的昵称：<input type="text" id="name" name="name" value="Guest">
            说话内容：<input type="text" id="msg" name="msg">
            <input type="submit" value="发送">
            </form>
            <iframe src="" name="write_frame" id="write_frame" width="0" height="0" frameborder="0"></iframe>
  <script>
   createRequest();
   setTimeout('update_msgs()',1000);
//setInterval 
  </script>
</body>
</html>
