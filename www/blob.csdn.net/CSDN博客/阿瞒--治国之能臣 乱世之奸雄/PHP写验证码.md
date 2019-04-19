# PHP写验证码 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2012年05月31日 12:10:01[baofenny](https://me.csdn.net/jiaminbao)阅读数：267标签：[php																[session																[im																[xhtml																[header																[action](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=header&t=blog)](https://so.csdn.net/so/search/s.do?q=xhtml&t=blog)](https://so.csdn.net/so/search/s.do?q=im&t=blog)](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=php&t=blog)
个人分类：[学习实例开发](https://blog.csdn.net/jiaminbao/article/category/1158383)
生成图片页面：index.php
<?php
 session_start();
 for($i=0;$i<4;$i++)
 {
  @$rand.=dechex(rand(1,15));
 }
 $im=imagecreatetruecolor(100,30);
 $bg=imagecolorallocate($im,0,0,0);
 $te=imagecolorallocate($im,255,255,255);
 imagestring($im,rand(1,6),rand(3,70),rand(0,15),$rand,$te);
 for($i=0;$i<3;$i++)
 {
 $tes=imagecolorallocate($im,rand(0,255),rand(0,255),rand(0,255));
 imageline($im,0,rand(0,15),100,rand(0,15),$tes);
 }
 for($i=0;$i<200;$i++)
 {
  imagesetpixel($im,rand()%100,rand()%30,$tes); 
 }
 //$str=iconv("gbk","UTF-8","新年快乐");
 //imagettftext($im,20,0,20,20,$te,'convalescence.ttf',$str);
 header("Content-type: image/jpeg");
 imagejpeg($im);
 $_SESSION[check_pic]=$rand;
?>
显示图片页面：sub.php
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "[http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd](http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd)">
<html xmlns="[http://www.w3.org/1999/xhtml](http://www.w3.org/1999/xhtml)">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>无标题文档</title>
</head>
<body>
<?php
 session_start();
 if(isset($_POST['check']))
 {
  if($_POST['check']!=$_SESSION['check_pic'])
   {
    echo "验证码有误".$_SESSION['check_pic'];
   }else
   {
    echo "验证成功".$_SESSION['check_pic'];
    }
  }
?>
<form action="" method="post">
<img src="index.php"><br>
<input type="text" name="check"> 
<input type="submit" value="提交">
</form>
</body>
</html>
