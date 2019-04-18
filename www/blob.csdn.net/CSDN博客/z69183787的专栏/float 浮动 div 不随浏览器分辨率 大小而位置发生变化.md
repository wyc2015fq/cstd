# float 浮动 div 不随浏览器分辨率 大小而位置发生变化 - z69183787的专栏 - CSDN博客
2014年04月10日 06:41:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8176
```
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<div style="width:900px;border:1px solid yellow;">
<div style="position:relative;display:block;width:500px;float:left;border:1px solid black">dsadsadsa</div>
<div style="position:relative;display:block;width:300px;float:right;border:1px solid red">111111111111111111</div>
<div style="clear:both;"></div>
</div>
</html>
```
在浮动层外部 增加 父DIV 设定具体宽度即可。
