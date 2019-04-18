# PHP 中 call_user_func 的使用 - weixin_33985507的博客 - CSDN博客
2018年09月03日 07:47:02[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
call_user_func函数类似于一种特别的调用函数的方法，使用方法如下
- 第一种情况：
```
function set_max($a,$b)  
{
   if($a>$b)
     echo $a;
   else
    echo $b;
}
call_user_func('set_max', "111","222");//结果为222
```
此时 第一个参数` set_max` 作为`call_user_func` 的回调函数使用,`$a=111,$b=222`。
- 第二种情况
```
class a
{
    function b($c)
    {
        echo $c ;
    }
}
$num = 200 ;
call_user_func(array('a','b'),$num);//结果为200
```
此时第一个参数为一个数组 那么 回调就是 `a`类里面的`b`方法，`num`作为参数传到`b`方法里面。省去了实例化`a`的过程。
说到这里就不得不再看看 `call_user_func_array`， 其实`call_user_func_array` 跟`call_user_func` 类似 使用如下：
```
function set_max($a,$b)
{ 
   if($a>$b) 
      echo $a;
   else 
      echo $b; 
}
 call_user_func_array('set_max',array(111,222));
 //结果222
```
比`call_user_func` 看起来直观一点。 而`call_user_func_array` 对类里面方法的使用则如下：
```
class test
{
    function set_max($a,$b)
   {
       if($a>$b)
          echo $a;
       else 
         echo $b;
   }
}
call_user_func_array(array('test','set_max'),array(111,222));
//结果同样为222
```
