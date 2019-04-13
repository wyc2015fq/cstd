
# JS控制文本框内键盘上下左右键的焦点 - anlcy - 博客园






# [JS控制文本框内键盘上下左右键的焦点](https://www.cnblogs.com/camilla/p/7592929.html)
avaScript键盘上下左右控制文本框焦点的方法有很多，这里简单说两种方法：
方法一：
创建一个table的dom元素，包含5行4列的文本框
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>keyboard control 1</title>
<style>
tr.highlight{background:\#08246B;color:white;}
</style>
</head>
<body>
<table border="1" width="70%" id="ice">
<tr>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
</tr>
<tr>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
</tr>
<tr>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
</tr>
<tr>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
</tr>
<tr>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
<td><input type='text'></td>
</tr>
</table>
<script language="javascript">
<!--
//定义初始化行列
var currentLine=0;  // 目前的行号
var currentCol=0;
var objtab=document.all.ice;
var colums = objtab.rows[4].cells.length;
var objrow=objtab.rows[0].getElementsByTagName("INPUT");
objrow[0].select();
document.body.onkeydown=function(e){
e=window.event||e;
switch(e.keyCode){
case 37: //左键
currentCol--;
changeItem(e);
break;
case 38: //向上键
currentLine--;
changeItem(e);
break;
case 39: //右键
currentCol++;
changeItem(e);
break;
case 40: //向下键
currentLine++;
changeItem(e);
break;
default:
break;
}
}
//方向键调用
function changeItem(e){
if(document.all){//如果是IE
var it=document.getElementById("ice").children[0];   //获取table表单元素
}else{//其他浏览器
var it=document.getElementById("ice");
}
if(e.keyCode==37||e.keyCode==39){
if(currentLine<0){//如果行小于0
currentLine=it.rows.length-1;
}
if(currentLine==it.rows.length){
currentLine=0;
}
var objtab=document.all.ice;
var objrow=objtab.rows[currentLine].getElementsByTagName("INPUT");

if(currentCol<0){
if(e.keyCode==37){
currentLine--;
if(currentLine<0){//如果行小于0
currentLine=it.rows.length-1;
}
objrow=objtab.rows[currentLine].getElementsByTagName("INPUT");
}
currentCol=objrow.length-1;
}
if(currentCol>=objrow.length){
if(e.keyCode==39){
currentLine++;
if(currentLine==it.rows.length){
currentLine=0;
}
objrow=objtab.rows[currentLine].getElementsByTagName("INPUT");
}
currentCol=0;
}
}else{
if(currentLine<0){//如果行小于0
currentLine=it.rows.length-1;
currentCol--;
if(currentCol<0){
currentCol=colums-1;
}
}
if(currentLine==it.rows.length){
currentLine=0;
currentCol++;
if(currentCol>=colums){
currentCol=0;
}
}
var objtab=document.all.ice;
var objrow=objtab.rows[currentLine].getElementsByTagName("INPUT");
while(e.keyCode==40&&objrow[currentCol]==undefined){
currentLine++;
objrow=objtab.rows[currentLine].getElementsByTagName("INPUT");
}
while(e.keyCode==38&&objrow[currentCol]==undefined){
currentLine--;
objrow=objtab.rows[currentLine].getElementsByTagName("INPUT");
}
}
objrow[currentCol].select();
}
//-->
</script>
</body>
</html>

基本思路是获取表单的行列数，监听键盘操作，通过DOM操作表格的行列来选取表格内的文本框。

方法二：
这个方法试用于动态创建多个文本框的情况，将所有的文本框划分为行列数组，为每个input设置id值，监听键盘操作设置，设置onmouseup方法获取当前鼠标位置，然后游当前位置确认数组的下一个相应文本框，将其选中。这里以简单粗糙的方式呈现基本思路
[html] view plain copy
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>keyborad control 2</title>
<style>
tr.highlight{background:\#08246B;color:white;}
</style>
</head>
<body>
<table border="1" width="70%" id="ice">
<tr>
<td><input id="td11" onmouseup=getMouse(1,1) ></td>
<td><input id="td12" onmouseup=getMouse(1,2)></td>
<td><input id="td13" onmouseup=getMouse(1,3)></td>
<td><input id="td14" onmouseup=getMouse(1,4)></td>
</tr>
<tr>
<td><input id="td21" onmouseup=getMouse(2,1)></td>
<td><input id="td22" onmouseup=getMouse(2,2)></td>
<td><input id="td23" onmouseup=getMouse(2,3)></td>
<td><input id="td24" onmouseup=getMouse(2,4)></td>
</tr>
<tr>
<td><input id="td31" onmouseup=getMouse(3,1)></td>
<td><input id="td32" onmouseup=getMouse(3,2)></td>
<td><input id="td33" onmouseup=getMouse(3,3)></td>
<td><input id="td34" onmouseup=getMouse(3,4)></td>
</tr>
</table>
<script language="javascript">
<!--
var mouseInfo={};      //存放鼠标的当前位置
var moveArray=new Array();     //存放文本框数组
for(var i=1;i<4;i++){
moveArray[i]=new Array();
for(var j=1;j<5;j++){
moveArray[i][j]="td"+i+j;
}
}
document.body.onkeydown=function(e){  //监听鼠标操作
e=window.event||e;
switch(e.keyCode){
case 37: //左键
moveLeft();
break;
case 38: //向上键
moveUp();
break;
case 39: //右键
moveRight();
break;
case 40: //向下键
moveDown();
break;
default:
break;
}
}

function getMouse(l,k){//鼠标所在位置
mouseInfo["row"]=l;
mouseInfo["col"]=k;
}
function moveLeft(){
var row=mouseInfo["row"];
var col=mouseInfo["col"];
var key=moveArray[row][col-1];
if(document.getElementById(key)!=undefined)
{
var textFiled=document.getElementById(key);
textFiled.focus(false, 1000);
textFiled.select();
mouseInfo["col"]=col-1;
}
}
function moveRight(){
var row=mouseInfo["row"];
var col=mouseInfo["col"];
var key=moveArray[row][col+1];
if(document.getElementById(key)!=undefined)
{
var textFiled=document.getElementById(key);
textFiled.focus(false, 1000);
textFiled.select();
mouseInfo["col"]=col+1;
}
}
function moveUp(){
var row=mouseInfo["row"];
var col=mouseInfo["col"];
var key=moveArray[row-1][col];
if(document.getElementById(key)!=undefined)
{
var textFiled=document.getElementById(key);
textFiled.focus(false, 1000);
textFiled.select();
mouseInfo["row"]=row-1;
}
}
function moveDown(){
var row=mouseInfo["row"];
var col=mouseInfo["col"];
var key=moveArray[row+1][col];
if(document.getElementById(key)!=undefined)
{
var textFiled=document.getElementById(key);
textFiled.focus(false, 1000);
textFiled.select();
mouseInfo["row"]=row+1;
}
}
//-->
</script>
</body>
</html>





