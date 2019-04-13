
# 浏览器 CSS hack - 追求卓越,做到专业 - CSDN博客


2013年07月18日 11:28:29[Waldenz](https://me.csdn.net/enter89)阅读数：625


小知识：什么是CSS hack？
由于不同的浏览器，比如IE6、IE7、IE8、Firefox等，对CSS的解析认识不一样，因此会导致生成的页面效果不一样，得不到我们所需要的页面效果。
这个时候我们就需要针对不同的浏览器去写不同的CSS，让它能够同时兼容不同的浏览器，能在不同的浏览器中也能得到我们想要的页面效果。
这个针对不同的浏览器写不同的CSS code的过程，就叫CSS hack，也叫写CSS hack。
css hack：由于不同的浏览器，比如Internet Explorer 6,Internet Explorer 7,Mozilla Firefox等，对css的解析认识不一样，因此会导致生成的页面效果不一样，得不到我们所需要的页面效果。 这个时候我们就需要针对不同的浏览器去写不同的css，让它能够同时兼容不同的浏览器，能在不同的浏览器中也能得到我们想要的页面效果。简而言之，浏览器不同于W3C关于css标准的解析就是css hack。
一般谈到css hack，最先想到的就是ie6。ie6很头疼，曾经是辉煌，如今是夕阳西下了，被进步理论和思想淘汰是早晚的事情，目前想做的就是尽最大的可能去向下兼容它。这篇博文需要说的是：主流浏览器之专属css hack。

**1、只有IE6可以正确解析执行的css hack**
/*在css属性前使用下划线(_)可以使此css属性在IE6中被解析执行*/
_color: red;

**2、只有IE7可以正确解析执行的css hack**
/*在css属性前使用星号和加号(*+)可以使此css属性在IE6中被解析执行*/
*+color: red;

**3、IE6\IE7可以正确解析执行的css hack**
/*在css属性前使用星号(*)可以使此css属性在IE6、IE7中被解析执行*/
*color: red;

**4、IE8+可以正确解析执行的css hack**
/*在css属性值后紧跟一个正斜杠0(\0)可以使此css属性在IE8+浏览器中被解析执行*/
color: red\0;

**5、只有IE9可以正确解析执行的css hack**
/*在css属性值后紧跟一个正斜杠9一个正斜杠0(\9\0)可以使此css属性在IE9中被解析执行*/
color: red\9\0;

**6、IE6+可以正确解析执行的css hack**
/*在css属性值后紧跟一个正斜杠9(\9)可以使此css属性在IE6+浏览器中被解析执行*/
color:red\9;

**7、webkit内核浏览器(chrome\safari)和IE6\IE7可以解析执行的css hack**
/*css属性前使用左方括号加分号([;)可以使此css属性在webkit浏览器和IE6、IE7中被解析执行*/
[;color:red;
css hack 虽好且方便兼容各浏览器，但是通不过 W3C 验证，所以还得自己权衡是否有必要去使用。



