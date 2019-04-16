# 第一课：jquery入门 - PeterBishop - CSDN博客





2018年12月11日 19:50:26[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：21








  这节课开始我们开始学jquery

![](https://img-blog.csdnimg.cn/2018121119452535.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)





jquery实际上是一个js文件,里面定义了很多函数供我们调用



要使用jquery，首先要引入jquery文件，jquery分为很多个版本，我这里用1.9版本吧

**注意，必须先把jquery****的js****文件复制粘贴到项目中**




//引入jquery

//在html中引入js文件的语法为 <script src = ""></script>

//js中引入其他js文件

document.write("<script src='jquery1.9.js'></script>");



//1 jquery获取标签的函数



//1.1 根据id

**var **aElement = $("#firsta");



//1.2 根据name,返回的是数组

**var **$a = $("a");



//1.3 根据TagName，返回的是数组

**var **aElementArray = $("<a></a>");



//1.4 根据class属性

**var **$center = $(".center");



//注意，$()获取的是jquery对象，不是dom对象，二者之间可以相互转化

//jquery对象转dom对象

**var **dom_a = aElement[0];



//dom对象转jquery对象

**var **jquery_a = $(dom_a);



//2 改变属性



//2.1 获取属性，调用attr(attrName)

**var **a_id =jquery_a.attr("id");

console.log(a_id);

a_id = "first_a";



//2.2 设置属性，调用attr(attrName,attrValue)

jquery_a.attr("id",a_id);



//3. 改css样式 调用css(cssAttrName, cssAttrValue)

jquery_a.css("color","red");



//4. 添加css样式addClass(className) 删除css样式removeClass(className)

jquery_a.addClass("center");



jquery_a.removeClass("center");



//5. 设置事件 对象.事件(function(){});  这个事件名就是onXXX 去掉on

jquery_a.click(**function **() {



});



//6. 设置innerHTML 调用html(text),  获取用html()

jquery_a.html("百度一下，你就知道");



**var **text = jquery_a.html();

console.log(text);




