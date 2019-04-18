# IE7 中 div 背景透明的问题 - z69183787的专栏 - CSDN博客
2012年11月27日 11:09:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6693
```
<!DOCTYPE html>
<html>
<div style="filter:alpha(opacity:20);opacity:0.2;background:red;color:blue;">111111111111111111111</div>
</html>
```
这种情况下是不透明的；
![](https://img-my.csdn.net/uploads/201211/27/1353985951_9844.jpg)
但
```
<!DOCTYPE html>
<html>
<div style="width:100%;filter:alpha(opacity:20);opacity:0.2;background:red;color:blue;">111111111111111111111</div>
</html>
```
或者
```
<!DOCTYPE html>
<html>
<div style="position:absolute;filter:alpha(opacity:20);opacity:0.2;background:red;color:blue;">111111111111111111111</div>
</html>
```
IE7 下就是透明的：
![](https://img-my.csdn.net/uploads/201211/27/1353985981_6382.jpg)
按照结果来看，只要设置了宽度，IE7 下的 opacity就起作用了，不知道为什么，所以一些大型网站的透明效果还都是用 透明PNG 图片来实现吧。
