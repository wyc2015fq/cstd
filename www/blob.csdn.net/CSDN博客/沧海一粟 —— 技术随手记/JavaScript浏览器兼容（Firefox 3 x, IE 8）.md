# JavaScript浏览器兼容（Firefox 3.x, IE 8） - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月12日 10:42:00[慢游](https://me.csdn.net/eengel)阅读数：1256标签：[javascript																[浏览器																[ie																[firefox																[dojo																[browser](https://so.csdn.net/so/search/s.do?q=browser&t=blog)
个人分类：[Dojo](https://blog.csdn.net/eengel/article/category/773224)





关于JavaScript浏览器兼容的问题，已经有很多前人提供了大量有用的解决方案。下面列举的是其中几篇：

1. 【javascript】火狐浏览器兼容原则
[http://blog.csdn.net/donny_zhang/archive/2008/12/01/3420508.aspx](%E3%80%90javascript%E3%80%91%E7%81%AB%E7%8B%90%E6%B5%8F%E8%A7%88%E5%99%A8%E5%85%BC%E5%AE%B9%E5%8E%9F%E5%88%99%20http%3A//blog.csdn.net/donny_zhang/archive/2008/12/01/3420508.aspx)

2. JavaScript js 兼容浏览器问题 兼容FireFox(FF)、IE的解决方法
[http://blog.csdn.net/IBM_hoojo/archive/2010/07/02/5708440.aspx](http://blog.csdn.net/IBM_hoojo/archive/2010/07/02/5708440.aspx)

3. 浏览器兼容性问题 及解决办法若干
[http://blog.csdn.net/gishero/archive/2010/07/05/5714247.aspx](http://blog.csdn.net/gishero/archive/2010/07/05/5714247.aspx)



这儿主要记录了除开上述文章之外的一些兼容问题，作为其补充。

JavaScript Browser Compatibility
(Tested in FF 3.6 & IE 8)
1. 在dojo.provide声明的js文件中定义全局变量
dojo.provide("my.c");
c1 = "global1";		//Is global in FF & IE
var c2 = "global2"	//Is global in FF, but can not be visited neither as global nor as local in IE
2. JavaScript对象定义时，最后一个成员是否以","结尾
var person = {
	name: "echo",
	phone: "1234",	//Ok in FF but error occurred in IE
}
var person = {
	name: "echo",
	phone: "1234"	//Both Ok in FF & IE
}
3. 定义变量时，是否以";"结尾
var a = "hello "	//Ok in FF but error occurred in IE
var b = "world";
var a = "hello ";	//Both Ok in FF & IE
var b = "world";
4. dojo ContentPane中JavaScript代码的调用
main.html
<div dojoType="dijit.layout.ContentPane" minSize="400" maxSize="1000" href="part.html" mce_href="part.html"></div>
part.html
<div>part</div>
<mce:script type="text/javascript"><!--
	alert("part");		//Alert in FF but not in IE
// --></mce:script>
main.html
<div dojoType="dojox.layout.ContentPane" minSize="400" maxSize="1000" href="part.html" mce_href="part.html"></div>
part.html
<div>part</div>
<mce:script type="text/javascript"><!--
	alert("part");		//Alert both in FF & IE
// --></mce:script>
5. trim
" a ".trim();		//work in FF but error occurred in IE
dojo.trim(" a ");	//Both OK in FF & IE
6. firstElementChild
node.firstElementChild	//work in FF but undefined in IE
node.firstChild			//Both OK in FF & IE
7. event.target
event.target		//Both OK in FF & IE 8!
8. <input type="text" value="text" disabled>
在FF中，整个输入框及里面的内容变灰，点击禁止。
在IE中，点击禁止，但只有内容变灰；因此当输入框中没有内容时，不太容易判断disable状态。](https://so.csdn.net/so/search/s.do?q=dojo&t=blog)](https://so.csdn.net/so/search/s.do?q=firefox&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)




