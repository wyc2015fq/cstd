# php 中 define 定义常量和单双引号问题 - 阿瞒--治国之能臣 乱世之奸雄 - CSDN博客
2013年11月04日 10:35:57[baofenny](https://me.csdn.net/jiaminbao)阅读数：4326
一、define定义常量问题
- 
<?php  
- 
define('DATABASE', 'MYSQL');  
- 
define('DATABASE_USER', 'ROOT');  
- 
define('DATABASE_PASSWORD', 'PASSWORD');  
- 
?>
             直接使用"DATABASE"常量，代表的就是"MYSQL"。
$arr = array('fruit'=>'apple','veggie'=>'carrot');
             define（'fruit','veggie'）；
             print $arr['fruit'];   //  apple
             print $arr[fruit];  //  carrot  
            * 注意 ：[] 中没有引号。php会自动把key值当作常量来解析，然后去内存中找对应的常量，于是找到了fruit，然后解析key值。然后再去内存中找常量,没找到veggie，然后就把veggie当作真正的key值,找到对应的值。
﻿
﻿
二、单双引号问题
    error_reporting(E_ALL);
     $arr = array('fruit'=>'apple','veggie'=>'carrot');
    define('fruit','veggie');
    print $arr['fruit']; echo '<br/>';    //apple
    print $arr[fruit]; echo '<br/>';    //carrot
    print "hello $arr[fruit]"; echo '<br/>';  // apple
    print "hello {$arr[fruit]}"; echo '<br/>'; // hello carrot
    print "hello {$arr['fruit']}";  echo '<br/>';// hello apple
   //print "Hello $arr['fruit']"; // 有错误
   //print "hello $_GET['foo']"; // 有错误
   print "Hello " . $arr['fruit']; // Hello apple
注意： 数组方括号中如果没有加引号，php会首先按照常量来解析。
