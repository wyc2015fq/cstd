# jQuery阻止冒泡和HTML默认操作 - z69183787的专栏 - CSDN博客
2012年10月22日 14:12:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：606标签：[jquery																[html																[框架																[javascript																[behavior](https://so.csdn.net/so/search/s.do?q=behavior&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=框架&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=jquery&t=blog)
个人分类：[Javascript-Jquery](https://blog.csdn.net/z69183787/article/category/2175111)
jQuery是一个快捷简便的JavaScript框架，说道框架可以直接理解为就是对原来底层的东西进行了封装使得开发者能够利用这个框架快速开发。 
jQuery是一个快捷简便的JavaScript框架，说道框架可以直接理解为就是对原来底层的东西进行了封装使得开发者能够利用这个框架快速开发。 
在当今的各个浏览器中都支持事件的冒泡，所谓的冒泡可以这样理解：
条件：外层有一个DIV元素，在内层有一个P元素，他两存在这样的关系:DIV是P元素的父元素，而P是外层DIV的子元素，他们之间存在包含和被包含的关系。
事件：现在我们都在这两个元素上绑定相同的事件，比如click事件。
结果：这时，当我们点击内层的P标签的时候，内层元素的click事件触发，同时外层DIV的click事件也会触发。
在HTML中有的元素被定义了一些默认的属性，比如说A元素，这个元素就是我们平时使用的超链接标签，这个标签的默认属性是实现页面的跳转。
在jQuery的每一个事件中都会有一个默认的对象作为该事件的参数(但是必须显示地被指定)，这个对象就是event对象，它包含了一些属性和方法，用于不同的场合。如下：
$('p:first').click(function(event){
//event对象可以使用了
});
有时我们不希望冒泡或默认的事件发生，这样就需要一些jQuery的的方法阻止冒泡和默认的事件了。可以通过以下三种方法做到不同程度的阻止。
A：return false
 --->In event handler ,prevents default behavior and event bubbing 。
return false 在事件的处理中，可以阻止默认事件和冒泡事件。
B：event.preventDefault()--->
 In event handler ,prevent default event (allows bubbling) 。
event.preventDefault()在事件的处理中，可以阻止默认事件但是允许冒泡事件的发生。
C：event.stopPropagation()--->
 In event handler ,prevent bubbling (allows default behavior).
event.stopPropagation()在事件的处理中，可以阻止冒泡但是允许默认事件的发生。
　　代码如：
　　 $('.menu li').click(function(){
$(this).find('ul').toggle();
return false;//去掉试试效果
})
