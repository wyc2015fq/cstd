# PHP登陆页面 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2012年05月31日 12:09:41[baofenny](https://me.csdn.net/jiaminbao)阅读数：325
<?php
 header('Content-Type: text/html; charset=utf-8');
 if(isset($_GET['out']))
 {
  setcookie('id','');
  setcookie('pass','');
  echo "<script> window.location.href='login.php'</script>" ;
 }
 if(isset($_POST['id']) && isset($_POST['pass']))
 {
  setcookie('id',$_POST['id']); 
  setcookie('pass',$_POST['pass']);
  echo "<script> window.location.href='login.php' </script>";
 }
 if($_COOKIE['id'] && $_COOKIE['pass'])
 {
  echo "登陆成功<br>用户ID为：".$_COOKIE['id']."<br>密码：".$_COOKIE['pass'];
  echo "<br><a href='login.php?out=out'>退出</a>";
 }
?>
<form action="" method="post">
用户ID:<input type="text" name="id"><br/><br/>
密码：<input type="password" name="pass"><br/><br/>
<input type="submit" value="登陆">
</form>
