# js 父窗口控制子窗口的行为-打开，关闭，重定位，回复 - z69183787的专栏 - CSDN博客
2014年06月05日 10:38:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1442
测试的时候，注意当前目录中，有代码中的html文件。
复制代码代码如下:
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"> 
<html xmlns="http://www.w3.org/1999/xhtml"> 
<head> 
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" /> 
<title>Windows窗口打开</title> 
<script language="javascript"> 
var winID = null; 
//打开窗口 
function openWindow() 
{ 
winID = window.open("windowTime.html","JavaScript"); 
} 
//关闭窗口 
function closeWindow() 
{ 
if(winID && winID.open && !winID.closed) 
{ 
winID.close(); 
} 
} 
//更改URL网址 
function changeURL(newURL) 
{ 
if(winID && winID.open && !winID.closed) 
winID.location.href = newURL; 
} 
</script> 
</head> 
<body onunload="closeWindow()"> 
<h2>窗口的打开与关闭 
<hr /> 
<form> 
<input type="button" value="打开窗口" onclick="openWindow()" /> 
<input type="button" value="关闭窗口" onclick="closeWindow()" /> 
<input type="button" value="显示性的url" onclick="changeURL('yes.html')" /> 
<input type="button" value="重新新窗口的URL" onclick="changeURL('displayClock.html')" /> 
</body> 
</html> 
