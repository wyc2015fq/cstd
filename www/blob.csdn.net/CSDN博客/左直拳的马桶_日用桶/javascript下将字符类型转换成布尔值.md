# javascript下将字符类型转换成布尔值 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月27日 10:44:34[左直拳](https://me.csdn.net/leftfist)阅读数：6325
办不到！孙子，我告诉你，这办不到！
比如说，
```java
var sb = "false";
alert(!!sb);//结果显示true!
```
事前是想得很美的：
sb == "false"
那么
!sb == true
那么
!!sb == false
这样，就将 sb 由"false" 转换成真正的布尔值 false 矣。
但铁一般的事实证明：想得美，但长得丑：
sb == "false"，!sb == false ！因为sb是一个非空字符串，非空字符串的结果永远是真！系统不会因为这个字符串是"false"就特殊对待！
以往，我们要将一个字符串转换成数字，其实也可以 乘以1或者乘以-1，但布尔值不行。
网上的答案是不转换，可以用恒等来比较
if( sb === "false")
有关恒等（===）和相等（==）的比较，可以看参考文章：
[http://www.cnblogs.com/litsword/archive/2010/07/22/1782933.html](http://www.cnblogs.com/litsword/archive/2010/07/22/1782933.html)
