# IE7下浮动（float）层不能实现环绕的问题 - 左直拳的马桶_日用桶 - CSDN博客
2012年11月02日 14:17:11[左直拳](https://me.csdn.net/leftfist)阅读数：2228
有2个DIV，左面那个浮动，放广告；右边那个放正文内容。希望的效果是正文右环绕着这个广告。代码如下
```
<div><div style="float:left;width:300px;height:250px;margin-
right:10px;padding:10px;">广告</div><div id="divContent" style="background
-color:#FAFAFA;padding:10px;border:solid 1px #ccc;min-height:300px;"><p>正
文内容</p></div></div>
```
GG、FF、IE6\IE8都正常，但IE7下却变成这个样子：
![](https://img-my.csdn.net/uploads/201211/02/1351836468_4681.png)
变成广告自己独占一列，正文自己一列，泾渭分明，根本没有环绕的效果。
后来将正文所在DIV的“
`min-height:300px;`”去掉，则施施然环绕矣。
![](https://img-my.csdn.net/uploads/201211/02/1351837207_6703.png)
一个最小高度限制，居然会影响浮动重叠效果，谁想得到呢。九不搭八。
我操你，微软，我操你姥姥。
