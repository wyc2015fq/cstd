# preg_replace正则表达式的使用 - 在思索中前行！ - CSDN博客





2015年01月05日 08:54:29[_Tham](https://me.csdn.net/txl16211)阅读数：756








```php
<?php
$str="as2223adfsf0s4df0sdfsdf";
echo preg_replace("/0/","",$str);//去掉0字符，此时相当于 replace的功能, preg_replace("/0/","A",$str); 这样就是将0变成A的意思了
echo preg_replace("/[0-9]/","",$str);//去掉所有数字
echo preg_replace("/[a-z]/","",$str); //这样是去掉所有小写字母
echo preg_replace("/[A-Z]/","",$str); //这样是去掉所有大写字母
echo preg_replace("/[a-z,A-Z]/","",$str); //这样是去掉所有字母
$str="as2223adfsAAf0s4df0s中国人dD中南海DDfsdf";
echo preg_replace("/[a-z,A-Z,0-9]/","",$str); //去掉所有字母和数字
?>
经过以上的例子，相信大家知道，[ ] 和里面的,有什么作用了。也可以看到，匹配的字符串必须加 / /（看例子的第一个参数）

<?php
$str="acsdcs<55555555>sc<6666>sdcd";
echo preg_replace("/<.*>/","",$str);

//这个是表示去除以<开头，以>结尾的那部份，输出结果是：acsdcssdcd
注意：上面的 .* 是表示任何字符，也就是说不管<>包住的是什么都去掉其中. 表示任意字符， * 表示任意个数

现在我们来改动一下，如果不想是任何个数呢？
$str="acsdcs<55555555>sc<6666>sdcd";
echo preg_replace("/<.{4}>/","",$str); //此时输出：acsdcs<55555555>scsdcd因为{4}指定了条件：<>内为4个字符的才满足条件，所以<55555555>不符合条件，没有被替换。

注意：这时我们又学到了一个知识点{数字} 表示指定前面的个数，*就表示是任意个(0--无限个)

表示重复次数的除了 *, {指定次数} 表示，还有很多表达形式：

 
<?php
$str="acsdcs<55555555>sc<6666>sd<>cd";
echo preg_replace("/<[0-9]*>/","",$str);
//输出acsdcscd
echo "<hr>";
echo preg_replace("/<[0-9]+>/","",$str);
//输入acsdcsscsd<>cd
?>

上面的例子只要是为了 表达 * 与+的区别 ,* 表示重复0数或n 次，而+ 表示1次以上，即一例中<[0-9]+ >表示<>里面至少要有一个数字才符合条件。

相信这时大家知道，为什么上例中用* 和用+输出的结果不同了吧

再来：

<?php
$str="acsdcs<55555555>sc<6666>sd<>cd";
echo preg_replace("/<[0-9]?>/","",$str);
//输出acsdcs<55555555>sc<6666>sdcd
?>

看[0-9]? 这里的?表示要是0次或1 次，超过1次又不符合条件了。
总结一下，上面我们学会了 * + ? 和大括号{}表示重复次数的方法。

----------------------------------------------------------------------------------------------------

    $s=preg_replace("/（.*?[月票|求|更].*?）/i","",$s);
    preg_match_all('/href=\"([0-9]+)\.shtm">(.+?)<\/a>/i',$s,$arr_dstorycate);    
    print_r($arr_dstorycate);

----------------------------------------------------------------------------------------------------

   preg_match_all("/<img.*?src=[\\\'| \\\"](.*?(?:[\.gif|\.jpg]))[\\\'|\\\"].*?[\/]?>/i",$content,$arr_dstorycate);            
   print_r($arr_dstorycate);
```




