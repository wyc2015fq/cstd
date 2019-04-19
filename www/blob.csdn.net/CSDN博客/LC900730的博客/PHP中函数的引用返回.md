# PHP中函数的引用返回 - LC900730的博客 - CSDN博客
2018年03月09日 23:01:21[lc900730](https://me.csdn.net/LC900730)阅读数：632
```php
function &test(){
    static $b=0;
    $b=$b+1;
    echo $b;
    return $b;
$a=test();  //输出$b的值为1
$a=5;
$a=test();  //$b的值为2；
$a=&test();   
$a=5;
$a=test();  //这条语句会输出$b=6。
```
注意： 
1.$a=test()得到的不是函数的引用返回，跟普通函数调用没有不同，PHP规定；
2.&test()才是，此时$a与$b变量的内存指向了同一个地方
