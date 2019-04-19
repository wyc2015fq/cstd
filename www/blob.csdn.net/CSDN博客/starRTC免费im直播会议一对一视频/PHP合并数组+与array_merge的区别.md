# PHP合并数组+与array_merge的区别 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年03月09日 15:40:18[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：330
[http://www.phpernote.com/php-string/351.html](http://www.phpernote.com/php-string/351.html)
PHP中合并两个数组可以使用+或者array_merge，但这两个还是有区别的
主要区别是当两个或者多个数组中如果出现相同键名，需要注意以下两点：
首先需要说明一下php里面数组按键名大约可以分为字符串（关联数组）或者数字（数值数组），这里就不讨论多维数组了。
**（1）键名为数字（数值数组）时，array_merge()不会覆盖掉原来的值，但＋合并数组则会把****最先****出现的值作为****最终结果****返回，而把后面的数组拥有相同键名的那些值“抛弃”掉（不是覆盖）。**
**（2）键名为字符（关联数组）时，＋仍然把****最先****出现的值作为****最终结果****返回，把后面的数组拥有相同键名的那些值“抛弃”掉，但array_merge()此时会覆盖掉前面相同键名的值。**
下面通过几个具体的例子进行说明：
m:Array (
 [0]
 => a
 [1]
 => b
)
n:Array (
    [0] => c
    [1] => d
)
m+n 结果为 : Array (
    [0] => a
    [1] => b
)
array_merge(m,n)结果为 : Array (
    [0] => a
    [1] => b
    [2] => c
    [3] => d
)

m:Array (
    [1] => a
 [2]
 => b
)
n:Array (
    [2] => c
    [3] => d
)
m+n结果为 : Array (
    [1] => a
    [2] => b
    [3] => d
)
array_merge(m,n)结果为 : Array (
    [0] => a
    [1] => b
    [2] => c
    [3] => d
)
m:Array (
    [a] => a
[b] => b
)
n:Array (
    [b] => c
    [d] => d
)
m+n结果为 : Array (
    [a] => a
    [b] => b
    [d] => d
)
array_merge(m,n)结果为 : Array (
    [a] => a
    [b] =>
c
    [d] => d
)


is_integer() 此函数是 is_int() 的别名函数，判断是否是整数类型的变量，返回值为true或者false。注意：对于正整数，负整数和0的数字类型变量，该函数都返回true，其他都返回空（包括字符串类型的整数）;
is_numeric() 判断是否是数字，返回值为true或者false。这个函数判断条件非常宽松，只要是数字，无论你是字符串类型的，还是数字类型，无论是正的，负的，0，还是浮点数，都将返回true。其他则返回false。
[http://www.phpernote.com/php-function/241.html](http://www.phpernote.com/php-function/241.html)
