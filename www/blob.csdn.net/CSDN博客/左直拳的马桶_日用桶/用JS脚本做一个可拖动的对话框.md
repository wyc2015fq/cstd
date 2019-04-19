# 用JS脚本做一个可拖动的对话框 - 左直拳的马桶_日用桶 - CSDN博客
2007年04月02日 15:57:00[左直拳](https://me.csdn.net/leftfist)阅读数：2590
用JS脚本做一个可拖动的对话框
左直拳
**一、****可拖动**
关键是能跟踪鼠标坐标的变化，从而不断修正对话框的坐标。
假设待移动的对话框为objMove，则有
pX = event.x - objMove.style.pixelLeft;
pY = event.y - objMove.style.pixelTop;
其中pixelLeft、pixelTop是对话框左边缘、顶边缘与document左边缘、顶边缘的距离，event.x、event.y是鼠标按下（未松开）的坐标值。
因此可得到鼠标抓紧点与对话框边缘的坐标差pX，pY。只要在鼠标移动过程中，保持这一坐标差恒定不变，月亮走我也走，鼠标送阿框到村口，即可实现对话框的拖动：
objMove.style.left = event.x - pX;
objMove.style.top = event.y - pY;
这只是原理，还需要一些函数进行支撑。
设置三个函数：
MDown()//鼠标按下时触发，得到pX,pY的值，放在对话框的onmousedown调用
MMove()//鼠标移动时触发,拖动，document.onmousemove
MUp()//鼠标松开时触发，释放,document.onmouseup,当然放在对话框的onmouseup调用也是可以的
注意这个对话框要用绝对定位。
**二、****对话框内的文本框等可正常编辑**
如果对话框内有文本框、下拉框等可供用户进行编辑的控件，如果不进行特别处理，则光标在这些控件内拖动同样会引起对话框的移动，这跟我们平时使用的习惯不符，给框内的编辑带来麻烦。
怎么办呢？
如果知道鼠标抓紧时落在哪种类型的控件上就好了。
嘿，这位老板，你好运了！可以用event.srcElement来获取这个控件
var hitpoint = event.srcElement;
if( hitpoint.tagName == "INPUT"
|| hitpoint.tagName == "TEXTAREA"
|| hitpoint.tagName == "SELECT" )
{
objMove = null;//不鸟它
return;
}
**三、****半透明**
这个对话框还要有点透明，拖动时下面的元素若隐若现，这样才让人够兴奋，浮想联翩。
主要是CSS里实现。style="FILTER:alpha(opacity=90);position:absolute;left:60%;……
不过这好象在IE系的浏览器里才有用，对firefox这些则没效果。
附录：drag.js
var objMove = null;
var pX = 0;
var pY = 0;
document.onmouseup = MUp;
document.onmousemove = MMove;
function MDown(objMoveId)
{
var hitpoint = event.srcElement;
if( hitpoint.tagName == "INPUT"
|| hitpoint.tagName == "TEXTAREA"
|| hitpoint.tagName == "SELECT" )
{
objMove = null;
return;
}
objMove = document.getElementById(objMoveId);
if( objMove == null )
{
return;
}
objMove.style.cursor = "move";
//objMove.setCapture();
pX = event.x - objMove.style.pixelLeft;
pY = event.y - objMove.style.pixelTop;
}
function MMove()
{
if(objMove != null)
{
objMove.style.left = event.x - pX;
objMove.style.top = event.y - pY;
}
}
function MUp()
{
if(objMove != null)
{
//objMove.releaseCapture();
objMove.style.cursor = "default";
objMove = null;
}
}
