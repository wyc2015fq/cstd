# 使用X-UA-Compatible来设置IE浏览器兼容模式 - z69183787的专栏 - CSDN博客
2013年12月20日 10:45:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12116
# 若项目中使用frameset 或者 ifame ，则需要设置最顶层页面的
# X-UA-Compatible属性，之后内嵌的所有页面都会继承，不用再单独设置。
```
<!DOCTYPE html>
<html lang="cn">
<head>
<meta charset="utf-8" />
<meta http-equiv="x-ua-compatible" content="IE=8">
<title>上海申通地铁集团综合业务协同平台</title>
<frameset rows="79,*,35" frameborder="no" border="0" framespacing="0">
  <frame src="/portal/mainFrame/topp.jsp" name="topFrame" scrolling="No" noresize="noresize" id="topFrame" title="topFrame" />
  <!--<frame src="main_frame.html" name="mainFrame" id="mainFrame" title="mainFrame" />  -->
  <frameset id="main" cols="7,*" frameborder="no" border="0" framespacing="0">
  	<frame src="" class="frame_bl" name="leftFrame" scrolling="No" noresize="noresize" id="leftFrame" title="leftFrame" />
  	<frame src="" class="frame_br" name="mainFrame" id="mainFrame" title="mainFrame" />
	</frameset>
  <frame src="bottom.jsp" name="bottomFrame" scrolling="No" noresize="noresize" id="bottomFrame" title="copyright" />
</frameset>
<noframes><body>
</body></noframes>
</html>
```
# X-UA-Compatible 无效
近做个网站的项目中，遇到X-UA-Compatible无效的问题。
经测试，X-UA-Compatible只有在前面也有meta的时候，才有效。
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7" />
这是给IE8看的，以兼容（IE7）模式呈现网页。
翻了下MSDN《[定义文档兼容性](http://msdn.microsoft.com/zh-cn/library/cc288325(VS.85).aspx)》，里面有这么一句：
> 
X-UA-compatible 标头不区分大小写；不过，它必须显示在网页中除 [title](http://msdn.microsoft.com/library/ms535910.aspx) 元素和其他 meta
 元素以外的所有其他元素之前的标头（[HEAD 节（可能为英文网页）](http://msdn.microsoft.com/library/ms535252.aspx)）中。
MSDN有机器翻译的传统，再看下[英文版](http://msdn.microsoft.com/en-us/library/cc288325(v=VS.85).aspx)：
> 
The X-UA-compatible header is not case sensitive; however, it must appear in the Web page’s header (the [HEAD
 section](http://msdn.microsoft.com/en-us/library/ms535252(v=VS.85).aspx)) before all other elements, except for the [title](http://msdn.microsoft.com/en-us/library/ms535910(v=VS.85).aspx) element
 and other metaelements.
**x-ua-compatible 用来指定IE浏览器解析编译页面的model**
x-ua-compatible 头标签大小写不敏感，必须用在 head 中，必须在除 title 外的其他 meta 之前使用。
1、使用一行代码来指定浏览器使用特定的文档模式。
<meta http-equiv="x-ua-compatible" content="IE=9" >
<meta http-equiv="x-ua-compatible" content="IE=8" >
<meta http-equiv="x-ua-compatible" content="IE=7" >
2、在一些情况下，我们需要限定浏览器对文档的解析到某一特定版本，或者将浏览器限定到一些旧版本的表现中。可以用如下的方式：
<meta http-equiv="x-ua-compatible" content="IE=EmulateIE9" >
<meta http-equiv="x-ua-compatible" content="IE=EmulateIE8" >
<meta http-equiv="x-ua-compatible" content="IE=EmulateIE7" >
使用这种写法，浏览器或者使用标准模式进行解析，或者使用 IE5 Quirks 模式进行解析。
3、为了测试，我们也可以使用下面的语句指定浏览器按照最高的标准模式解析页面。
<meta http-equiv="x-ua-compatible" content="IE=edge" >
4、多个模式的指定。我们可以用逗号分割多个版本，这种情况下，浏览器会从这个列表中选择一个他所支持的最高版本来使用标准模式进行渲染。如下面的例子，在IE8进行浏览时，将会使用IE7的标准模式进行渲染，因为他本身不支持IE9和IE10。
<meta http-equiv="x-ua-compatible" content="IE=7,9,10" >
**参考：**
[http://www.cnblogs.com/cocowool/archive/2013/04/25/3043832.html](http://www.cnblogs.com/cocowool/archive/2013/04/25/3043832.html)
[http://www.cnblogs.com/nidilzhang/archive/2010/01/09/1642887.html](http://www.cnblogs.com/nidilzhang/archive/2010/01/09/1642887.html)
[http://msdn.microsoft.com/en-us/library/jj676915(v=vs.85).aspx](http://msdn.microsoft.com/en-us/library/jj676915(v=vs.85).aspx)
