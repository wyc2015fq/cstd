# VBS脚本和HTML DOM自动操作网页 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年06月16日 10:24:16[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：11013
个人分类：[Windows](https://blog.csdn.net/fjssharpsword/article/category/6480443)









本来是想通过JS实现对其他页面的控制，发现跨域无法获取页面DOM来操作。接着考虑bat，发现也实现不了，于是想到vbs。vbs还是很强大啊，病毒之类很多都是vbs脚本啊。vbs打开浏览器，然后通过dom来操作页面，可以实现自动填写内容和按钮点击等。摸索了几个常用的，代码如下：




1、网页文本框赋值并提交按钮



```
Dim wsh,ie,btn,con
Set ie = WScript.CreateObject("InternetExplorer.Application")
ie.Visible = true
ie.navigate "http://www.baidu.com"

Do
Wscript.Sleep 200
Loop Until ie.readyState = 4 

Set con=ie.Document.getElementById("kw")
con.value="big data"

Set btn=ie.Document.getElementById("su")
btn.click
```


2、动态js加载的网页，深入frame内自动操作





```
Dim wsh,ie,btn,obj,sel,del

Set wsh=CreateObject("wscript.shell") 
Set ie = WScript.CreateObject("InternetExplorer.Application")
ie.Visible = true
ie.navigate "http://mail.qq.com"  

'等待网页打开成功，<span style="font-family: Arial, Helvetica, sans-serif;">qq邮箱参照前面自动填写用户名和密码提交，下面的id都是登录qq邮箱后的页面才有</span>
Do
Wscript.Sleep 200
Loop Until ie.readyState = 4 

'找到指定id的a标签并点击
Set btn =ie.Document.getElementById("folder_1")
btn.focus
btn.click
wsh.SendKeys "{ENTER}" 

'等待网页打开成功
Do
Wscript.Sleep 200
Loop Until ie.readyState = 4 

'通过frame/iframe的id获取其页面
set obj = ie.document.frames("mainFrame").document
'全部勾选checkbox
Set sel=obj.getElementById("ckb_selectAll")
'sel.checked=true
sel.click
Wscript.Sleep 2000
'a标签点击
Set del=obj.getElementById("quick_completelydel")
del.click

'从页面中找到指定id的元素
'For i=0 To obj.all.length-1
'If obj.all(i).id="id no" Then 
'	msgBox "get"
'End If
'Next
```
总结：要实现对网页的自动操作，比如用于测试或者一些批量工作，首先要解析网页的元素，然后针对元素进行自动操作，HTML DOM是需要掌握的。






