# 看几道JQuery试题后总结（上篇） - z69183787的专栏 - CSDN博客
2014年03月07日 16:03:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：813
### 无意中拿到的JQuery题目，拿来分享顺便总结总结
- 在JQuery对象中区分.text();.val();.html();.innerHTML;.innerTEXT()的用法与区别，用例子证明
- 在JQuery选择器中空格与大于号的区别
- 在一个表单中，查找所有选中的input元素,JQ的写法
- 指定的为某个div内部动态添加一个div的写法，外部添加一个div的写法(包括在该节点的前面以及后面)
- JQuery对象与dom对象的区别，以及两者互相转换的方法
- 如何动态的为某个div添加class属性，如何移除class属性，以及如何获取div中某个属性的值，比如说Id的名称或者其他属性的值
- JQuery里面的动画函数一般用什么？时间设置函数都有哪些,setTimeout()与setInterval()有何区别
- 写一个ajax例子出来，要标清楚各个参数的含义。以及ajax数据传输的时候有几种数据格式
- 要做出一个demo具体功能如下：页面中有一个按钮，点击按钮后弹出一个遮罩层，在遮罩层上面有一个div,在点击按钮后5秒后div内部出现内容。 在div下面有一个隐藏按钮，点击后，遮罩层消失，div缓慢消失。回到页面打开状态
友情提示：遮罩要用到层级z-index,以及透明效果，5秒要用到时间函数，缓慢消失要用到动画效果，自己查询API文档。
### 第一题
- 题目：在JQuery对象中区分.text();.val();.html();.innerHTML;.innerTEXT()的用法与区别，用例子证明
![jquery](http://d.lanrentuku.com/down/png/1305/pixelophilia-icons/chat.png) 无参数情况
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif) View
 Code
```
![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_e.jpg)
- 区别如下：
- $("#x1").val();上述例子获取的是空字符串，但是如果我们改成<input />标签我们会发现返回的时input的value值
- $("#x1").text();上述例子获取的是标记的内容，但是如果我们改成<input />标签我们会发现返回的是空字符串
- Jquery没有innerHTML这个属性，当我们使用$("#x1").innerHTML得到的是undefined；在这里我们可以通过$("#x1")[0].innerHTML或者使用document.getElementById("x1").innerHTML获取标记中的值
- 下面一个就用个例子吧
```
<p><b>Hello</b> world</p>
<p>火云邪神！</p>
```
JQuery代码：$("p").text();
结果：Helloworld火云邪神！
![](http://d.lanrentuku.com/down/png/1305/pixelophilia-icons/chat.png) 有参数情况
- .html有参数情况
```
<div></div>
```
```
$("div").html("<p>Bigod超神</p>");
```
![html](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_5.jpg)
- .text有参数情况
```
<p>火云邪神超神！</p>
```
```
$("p").text("<i>断桥残雪</i>哇哇哇");
```
　　结果：[ <p><i>断桥残雪</i>哇哇哇</p> ]
- val有参数情况
```
<input type="text"/>
```
```
$("input").val("你们跌倒了没？崩溃了没？");
```
　　结果：你们跌倒了没？崩溃了没？
### 第二题
- 题目：在JQuery选择器中空格与大于号的区别
　　看到这个是不是马上想到了层级选择器，说句实话额，在JQuery中的组合选择器，层级选择器，基本过滤选择器以及子元素选择器，我这个记得最清楚，不是别的什么原因，而是这个用的方法最少，貌似只有4个来着。
　　还是给个例子吧：
![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_6.jpg)
 然后将JQuery中的div>p改成div p，结果如下图：
![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_4.jpg)
　　我们不难发现使用空格可以选取#main下的所有子孙元素的p标记，而使用>只能选取#main下的子元素p标记，注意前一个是子孙，后一个是子
### 第三题
- 题目：在一个表单中，查找所有选中的input元素,JQuery的写法
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 <html xmlns="http://www.w3.org/1999/xhtml">
 2 <head runat="server">
 3     <title></title>
 4     <script src="jQuery/jquery-1.7.1.js" type="text/javascript"></script>
 5     <script type="text/javascript">
 6         $(document).ready(function () {
 7             var x = $("input:not(:checked)+span");
 8             alert(x.length);
 9         });
10     </script>
11 </head>
12 <body>
13     <form id="myform">
14         <input type="text" /><br />
15         <input type="checkbox" /><span>篮球</span><input type="checkbox" /><span>轮滑</span><br />
16         <input type="password" /><br />
17         <input type="submit" />
18     </form>
19 </body>
20 </html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_QQ%E6%88%AA%E5%9B%BE20130708155832.jpg)![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_QQ%E6%88%AA%E5%9B%BE20130708155922.jpg)![](http://images.cnblogs.com/cnblogs_com/vchenpeng/494367/o_QQ%E6%88%AA%E5%9B%BE20130708155948.jpg)
PS：使用JQuery的过滤选择器：not()
### 第四题
- 指定的为某个div内部动态添加一个div的写法，外部添加一个div的写法(包括在该节点的前面以及后面)
内部添加一个div
```
<div id="parent"></div>
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<script type="text/javascript">
        $(document).ready(function () {
            var parent = $("#parent");
            var obj = $("<div>火云邪神</div>");
            obj.appendTo(parent);
        });
</script>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　在某个div前面和后面分别添加一个div
　　思想：首先找到当前div的父节点，然后然后使用对应的函数（后面我会提到）添加节点
```
<div id="parent">xyz</div>
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    
　　/*节点前*/
　　<script type="text/javascript">
        $(document).ready(function () {
            var parent = $("#parent");
            var obj = $("<div>火云邪神</div>");
            parent.parent().prepend(obj);
        });
    </script>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    
　　/*节点后*/
　 <script type="text/javascript">
        $(document).ready(function () {
            var parent = $("#parent");
            var obj = $("<div>火云邪神</div>");
            parent.parent().append(obj);
        });
   </script>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
引申一下操作DOM对象(以下15点引申并非本人总结，来自[http://www.jb51.net/article/35606.htm](http://www.jb51.net/article/35606.htm))
- Append方法用来在元素的末尾追加元素（最后一个子节点）
- Prepend在元素的开始添加元素（第一个子节点）
- After:在元素之后添加元素（添加兄弟节点）
- Before：在元素之前添加元素（添加兄弟节点）
- 子元素.appendTo(父元素)：在元素的末尾追加一个子元素
- 子元素.prependTo(父元素):在元素的开始追加一个子元素
- A.insertBefore(B)    将A插入到B的前面，等同于B.before(A)
- X.insertAfter(B)   将X插入到Y的后面，等同于Y.after(X)
- Empty():清空该元素下的所有子节点
- Clone()克隆节点，支复制节点，不复制节点
- Clone(true):克隆节点带事件
- $("br").replaceWith("<hr/>");
- $("br").replaceAll("<hr/>");
- $("b").wrap("<font color='red'></font>");将所有粗体字红色显示
- wrapInner();在内部围绕
### 停电，伤不起！
　　近两天5点钟光停电，木有写完，本来把这些想一次写完的，来不及了，木有办法，下次写完，各位就原谅下咯！
　　------如果你觉得此文对你有所帮助，别忘了点击下右下角的推荐咯，谢谢！------
