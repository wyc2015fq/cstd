
# jQuery学习笔记（一）使用jQuery选择器（荐） - 萌萌的It人 www.itmmd.com - CSDN博客


2015年01月21日 13:14:05[Jlins](https://me.csdn.net/dyllove98)阅读数：2094


jQuery预先的javascript的编程，提供了计划所有css3下的标准选择器，开发者可以利用这些选择器轻松选择各种元素，供javascript使用。
重要的是jQuery对这些选择器的兼容性特别好，主流的浏览器都测试通过，这使得理论上的css3选择器一下编程了事实。开发者可以按照以前的方法定义各种css类别，然后通过addClass()方法或者className属性将其添加到指定的元素集合中。
1.属性选择器
（属性可以参考jQuery文档或者http://www.w3school.com.cn/jquery/jquery_ref_selectors.asp）
属性选择器的语法是在标记的后面用中括号"[" 和"]"添加相关属性，然后在赋予不同的逻辑关系。
以下为HTML
<li>第一个</li><li><ahref="1-1.html">第二个</a></li><li><ahref="1-2.html"title="第三个">第三个</a></li><li><ahref="1-3.html">第四个</a></li><li><ahref="1-4.html"title="制作">第五个</a></li>
如果希望选择设置了title属性的标记，则使用以下语法
$("a[title]")
给设置了title属性的两个超链接添加css样式
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<style>.altcss{/*设定某个CSS类别*/background-color:\#005890;color:\#4eff00;}</style><scripttype="text/javascript">$(function() {
                $("a[title]").addClass("altcss")
            });</script>![复制代码](http://common.cnblogs.com/images/copycode.gif)
显示样式：
![](http://images.cnitblog.com/blog/691294/201501/180237085586480.png)
属性选择器还有很多种，大家可以参考各版本的jQuery的手册灵活使用，常用的有以下几类。
[attribute]
[attribute=value] //匹配给定的属性是某个特定值的元素
[attribute!=value] //匹配所有不含有指定的属性，或者属性不等于特定值的元素。
[attribute^=value] //匹配给定的属性是以某些值开始的元素
[attribute$=value] //匹配给定的属性是以某些值结尾的元素
[attribute*=value] //匹配给定的属性是以包含某些值的元素
例如匹配超链接以pdf结尾的元素，可以使用
$(function() {
        $("li a[href$='pdf']")
    });
2.包含选择器
jQuery还提供了包含选择器用来选择包含了某种特殊标记的元素，比如以下代码：
$("li:has(a)")
表示给包含了超链接的li元素
例如
$("div:has(p)").addClass("test");
表示给所有包含 p 元素的 div 元素添加一个 test 类
最后整理了jQuery支持的css3属性选择器，贡献查询。
|选择器|说明|
|*|所有的标记|
|E|所有名称为E的标记|
|EF|所有名称为F的标记，并且是E标记的子标记（包括孙，重孙标记）|
|E>F|所有名称为F的标记，并且是E标记的子标记（不包括孙标记）|
|E+F|所有名称为F的标记，并且该标记紧接着前面的E标记|
|E~F|所有名称为F的标记，并且该标记前面有一个E标记|
|E:has(F)|所有名称为E的标记，并且该标记包含F标记|
|E.C|所有名称为E的标记，属性分类为C，如果去掉E，就是属性选择器.C|
|E\#I|所有名称为E的标记，id为I，如果去掉E，就是ＩＤ选择器I|
|E[A]|所有名称为E的标记，并且设置了属性A|
|E[A=V]|所有名称为E的标记，并且属性A的值等于V|
|E[A^=V]|所有名称为E的标记，并且属性A的值以V开头|
|E[A$=V]|所有名称为E的标记，并且属性A的值以V结尾|
|E[A *=V]|所有名称为E的标记，并且属性A的值包含V|
|
|纯属手打，如有错误，请指正|
3.位置选择器
css3还允许通过标记所处的位置来进行选择，例如选择第一个li元素
$("li:first-child")
选择奇数行的li元素
$("li:nth-child(odd)")
需要注意的是:nth-child(odd)的选择器和li:odd选择出的结果是一样的，这是因为：nth-child的相关css 选择器是从1开始计数的，而其他的选择器是从0开始计数的。
例如以下
$("p:eq(4)").addClass()
匹配添加className的元素有5个。
以下整理了jQuery支持的css3位置选择器
|选择器|说明|
|:first|第一个元素|
|:last|最后一个元素|
|:first-child|第一个子元素|
|:last-child|最后一个子元素|
|:only-child|所有没有兄弟的元素，例如：p:only-child选中所有的p元素，如果该p元素是父元素的唯一子元素|
|:nth-child(n)|第n个子元素（从1开始计数）|
|:nth-child(odd/even)|所有奇数号或者偶数号子元素（从1开始计数）|
|:nth-child(nX+Y)|利用公式来计算元素的位置，例如li:nth-child(5n+1)，表示选中所有的li元素，并且这些li元素为父元素的的5n+1个元素（1,6,11,16...）|
|:odd或者:even|奇数或者偶数号元素|
|:eq(n)|第n个元素（从0开始计数）|
|:gt(n)|第n个之后的所有元素（n从0开始计数，并且计算不包含第n本身）|
|:lt(n)|第n个之前的所有元素（n从0开始计数，并且计算不包含第n本身）|
|
|纯属手打，如有错误，请指正|
4.过滤选择器
除了css3中的一些选择器外，jQuery还提供了很多自定义的过滤选择器，用来处理更复杂的选择。例如很多时候希望知道用户所选中的复选框，如果通过属性值来判断，那么只能获得初始状态下选中的情况，而不是真实的选择情况。利用jQuery的:checked选择器则可以轻松获得用户的选择。
实例代码如下
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scriptsrc="http://apps.bdimg.com/libs/jquery/1.10.0/jquery.min.js"></script><scripttype="text/javascript">functionshowChecked(oCheckBox){//使用checked过滤出被用户选中的添加altcss$("input[name="+oCheckBox+"]:checked").addClass("altcss");
        }</script><style>.altcss{/*设定某个CSS类别*/}</style><formname="myform"><p><labelfor="football">足球<inputtype="checkbox"name="like"id="football"/></label></p><p><labelfor="basketball">篮球<inputtype="checkbox"name="like"id="basketball"/></label></p><p><labelfor="volleyball">排球<inputtype="checkbox"name="like"id="volleyball"/></label></p><p><inputtype="button"value="显示选择"onclick="showChecked('like')"></p></form>![复制代码](http://common.cnblogs.com/images/copycode.gif)
另外，过滤器之间可以迭代使用。
:checkbox:checked:enabled
表示input type="checkbox"中所有被用户选中而且没有被禁用的。
以下整理了jQuery中常用的选择过滤器
|选择器|说明|
|:animated|所有处于动画中的元素|
|:button|所有按钮，包括input[type=button]、[type=submit]、[type=reset]和<button>标记|
|:checkbox|所有复选框，等同于input[type=checkbox]标记|
|:contains(foo)|选择了包含文本"foo"的元素|
|:disabled|页面中被禁用了的元素|
|:enabled|页面中没有被禁用的元素|
|:file|上传文件元素|
|:header|选中所有的标题元素，如<h1><h6>|
|:hidden|页面中被隐藏了的元素|
|:image|图片提交按钮，等同于input[type=image]|
|:input|表单元素，包括<input>,<select>,<textarea>,<button>|
|:not(filter)|反向选择|
|:parent|选择所有子元素（包括文本）的元素，空元素将被排除|
|:password|密码文本框,等同于input[type=password]|
|:radio|单选按钮,等同于input[type=radio]|
|:reset|重置按钮，等同于input[type=reset]|
|:selected|下拉菜单被选中的项|
|:submit|提交按钮，包括input[type=submit]和button[type=submit]|
|:text|文本输入框，等同于input[type=text]|
|:visble|页面中的所有可见元素|
|纯属手打，如有错误，请指正|
5.实现反向过滤
上述过滤器中的:not(filter)的过滤器可以进行反向选择，其中filter参数可以是任意其它的过滤选择器，例如下面代码表示<input>标记中所有非radio元素
input:not(:radio)
另外过滤选择器还可以迭代使用。
以下的例子就是使用jQuery的反向过滤功能。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<scriptsrc="http://apps.bdimg.com/libs/jquery/1.10.0/jquery.min.js"></script><scripttype="text/javascript">$(function() {//迭代使用选择器$(":input:not(:checkbox):not(:radio)").addClass("altcss");
            });</script><style>.altcss{background-color:\#ffbff4;text-decoration:underline;border:1px solid \#0000FF;font-family:Arial, Helvetica, sans-serif;font-size:12px;}</style><formmethod="post"name="myForm1"action="addInfo.aspx"><p><labelfor="name">姓名:</label><inputtype="text"name="name"id="name"></p><p><labelfor="passwd">密码:</label><inputtype="password"name="passwd"id="passwd"></p><p><labelfor="color">最喜欢的颜色:</label><selectname="color"id="color"><optionvalue="red">红</option><optionvalue="green">绿</option><optionvalue="blue">蓝</option><optionvalue="yellow">黄</option><optionvalue="cyan">青</option><optionvalue="purple">紫</option></select></p><p>性别:<inputtype="radio"name="sex"id="male"value="male"><labelfor="male">男</label><inputtype="radio"name="sex"id="female"value="female"><labelfor="female">女</label></p><p>你喜欢做些什么:<br><inputtype="checkbox"name="hobby"id="book"value="book"><labelfor="book">看书</label><inputtype="checkbox"name="hobby"id="net"value="net"><labelfor="net">上网</label><inputtype="checkbox"name="hobby"id="sleep"value="sleep"><labelfor="sleep">睡觉</label></p><p><labelfor="comments">我要留言:</label><br><textareaname="comments"id="comments"cols="30"rows="4"></textarea></p><p><inputtype="submit"name="btnSubmit"id="btnSubmit"value="Submit"><inputtype="reset"name="btnReset"id="btnReset"value="Reset"></p></form>![复制代码](http://common.cnblogs.com/images/copycode.gif)
以上代码中的选择器如下：
$(":input:not(:checkbox):not(:radio)").addClass("altcss");
表示所有的<input><select><textarea>或<button>中非checkbox和非radio的元素（这里要注意input和:input的区别），除了单选按钮和复选框，表单的其它元素都添加了altcss样式风格。
注意：在:not(filter)中，filter参数必须是过滤选择器，而不能是其它的选择器 ，下面的代码是典型的语法错误。
div:not(p:hidden)
正确的写法是
div p:not(:hidden)

**其他精彩文章**
## [jQuery教程(29)-jQuery插件开发之为插件方法指定参数](http://www.itmmd.com/201501/519.html)
## [jQuery教程(28)-jQuery插件开发之使用插件](http://www.itmmd.com/201501/518.html)
## [jQuery教程(27)-jQueryajax操作之修改默认选项](http://www.itmmd.com/201501/515.html)
## [jQuery教程(26)-ajax操作之使用JSONP加载远程数据](http://www.itmmd.com/201501/512.html)
## [jQuery教程(25)-ajax操作之安全限制](http://www.itmmd.com/201501/511.html)


