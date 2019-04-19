# jQuery 语法 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月14日 10:36:46[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：105
个人分类：[前端js](https://blog.csdn.net/elesos/article/category/7108612)
基础语法是：**$(selector).action()**
- 美元符号定义 jQuery
- 选择符（selector）“查询”和“查找” HTML 元素
- jQuery 的 action() 执行对元素的操作
如
$("p").hide() - 隐藏所有段落
$(".test").hide() - 隐藏所有
class="test" 的所有元素
$("#test").hide() - 隐藏所有
 id="test" 的元素
**文档就绪函数**
$(document).ready(function(){--- jQuery functions go here ----});
选择元素
**元素选择器**
使用 CSS 选择器来选取 HTML 元素。
$("p") 选取 <p> 元素。
$("p.intro") 选取所有 class="intro" 的 <p> 元素。
$("p#demo") 选取所有 id="demo" 的 <p> 元素。
**属性选择器**
使用 XPath 表达式来选择带有给定属性的元素。
$("[href]") 选取所有带有 href 属性的元素。
$("[href='#']") 选取所有 href 值等于 "#" 的元素。
$("[href!='#']") 选取所有 href 值不等于 "#" 的元素。
$("[href$='.jpg']") 选取所有 href 值以 ".jpg"
结尾的元素。
CSS 选择器可用于改变 HTML 元素的 CSS 属性。
下面的例子把所有 p 元素的背景颜色更改为红色：
$("p").css("background-color","red");
$("div#intro .head") id="intro" 的 <div> 元素中的所有 class="head" 的元素
与ajax相关的
load() 方法从服务器加载数据，并把返回的数据放入被选元素中。
**get() 和 post() 方法用于通过 HTTP GET 或 POST 请求从服务器请求数据。**

参考
[http://www.w3school.com.cn/jquery/jquery_ajax_intro.asp](http://www.w3school.com.cn/jquery/jquery_ajax_intro.asp)


