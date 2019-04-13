
# Backbone简介 - Magician的博客 - CSDN博客


2017年09月13日 08:54:21[春雨里de太阳](https://me.csdn.net/qq_16633405)阅读数：278标签：[backbone																](https://so.csdn.net/so/search/s.do?q=backbone&t=blog)[简介																](https://so.csdn.net/so/search/s.do?q=简介&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=backbone&t=blog)个人分类：[其他语言																](https://blog.csdn.net/qq_16633405/article/category/8701162)



# 1. 前言
本文的目的，是帮助大家快速理解掌握Backbone的使用，但它并不是API，因此我不会将每一个方法都详细地描述，但是我会告诉你如何学习它们。
这是一篇稍稍较长的技术文章，因为我想将我所了解的东西尽可能详细地分享给大家。但它还不至于出版为一本书籍，因为Backbone其实非常简单。当然，这并不表示Backbone提供的功能有限，相反，它能用简单的实现构建出复杂的应用，这正是Backbone的强大之处。
在正式学习Backbone之前，你需要准备一些东西：
首先，你需要获取Backbone框架源文件：http://documentcloud.github.com/backbone/
Backbone依赖于Underscore框架的基础方法，因此，你同时需要下载Underscore框架的源文件：http://documentcloud.github.com/underscore/
在Backbone中，对DOM和事件的操作依赖于第三方库（如jQuery或Zepto），你只需要选择其中一个下载：
jQuery：http://jquery.com/
Zepto：http://zeptojs.com/
看起来好像挺麻烦的样子，但Backbone的目的是在于使用简单的框架来构建复杂的应用，因此麻烦并不表示它很复杂。
在后续章节，我将详细介绍它们之间的关系和依赖，但在此之前，你可以新建一个HTML页面，并将这些框架引入进来，就像这样：
[html][view plain](http://blog.csdn.net/likun557/article/details/53159870#)[copy](http://blog.csdn.net/likun557/article/details/53159870#)
<scripttype="text/javascript"src="jquery/jquery-1.8.2.min.js"></script>
<scripttype="text/javascript"src="underscore/underscore-min.js"></script>
<scripttype="text/javascript"src="backbone/backbone-min.js"></script>
至此，你已经准备好了运行Backbone必须的环境。
如果学习过程中有任何问题，或者你有更好的建议，欢迎联系我一起交流：
# 2. 什么是Backbone?
**Backbone是一个轻量级的前端MVC框架，用于结构化管理页面中的大量JS，建立与服务器、视图间的无缝连接，为构建复杂的应用提供基础框架。**
下面我先简单地阐述下Backbone的主要特点及特性：
## 2.1 轻量级
Backbone的源码只有1000行左右（去注释和空行后），文件大小只有16KB，加上依赖库Underscore，也仅有29KB。
你只需要花一点时间，就能轻松了解Backbone内部实现；或编写少量代码，来重载Backbone的部分机制；如果你想在Backbone的基础上做二次开发，也并不是一件复杂的事情。
## 2.2 MVC结构化
Backbone可以轻松将页面中的数据、逻辑、视图解耦，依照Backbone进行代码结构组织，你可以将项目中的数据交互、业务逻辑、用户界面等工作，分配给多个同事同时开发，并能有序地组织到一起。同时，这对于大型和复杂项目的维护开发非常有帮助。
## 2.3 继承机制
在Backbone中，模块是可以被继承的，你可以通过面向对象的方式将应用中的数据模型、集合、视图有序地组织，让整个架构更加清晰；也可以方便地重载和扩展自定义方法。
## 2.4 建立与服务器的无缝连接
在Backbone中内置了一套与服务器数据的交互规则（如果你了解REST架构，就能够轻松地理解它们），而数据的同步工作会在Model中自动进行，前端开发人员只需对客户端数据的进行操作，Backbone会自动将操作的数据同步到服务器。
这是件非常有趣的事情，因为服务器数据接口对前端开发者来说是透明的，他们不需要再关心如何和服务器交互。
然而服务器提供的数据接口也需要兼容Backbone的规则，对于一个新的项目来说，我们可以尝试使用这套规则来构建接口。但如果你的项目中已经有一套稳定的接口，你可能会担心接口改造的风险。
没关系，我们可以通过重载Backbone.sync方法来适配现有的数据接口，针对不同的客户端环境，我们还可以实现不同的数据交互方式。例如：用户通过PC浏览器使用服务时，数据会实时同步到服务器；而用户通过移动终端使用服务时，考虑到网络环境问题，我们可以先将数据同步到本地数据库，在合适的时候再同步到服务器。而这些只需要你重载一个方法就可以实现。
## 2.5 界面事件管理
在MVC中，我们希望能将界面展现和业务逻辑完全分离，但对于用户产生的交互事件（如单击事件），我们却常常通过类似jQuery中的bind方法进行获取和绑定。
Backbone中的视图帮助我们将用户事件和执行方法有序的组织起来，这只需要我们声明一个简单的表达式，例如：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53159870#)[copy](http://blog.csdn.net/likun557/article/details/53159870#)
events: {
// 单击id为”save”的元素时，执行视图的add方法
'click \#save':'add',
'mousedown .button':'show',
'mouseover .button':'hide'
}
**在表达式中，事件名称可以是任意的DOM事件（如click、mouseover、keypress等），元素可以是jQuery支持的任意选择器（如标签选择器、id选择器、class选择器等）。**
视图会自动将表达式中的事件绑定到选择器元素，当元素的事件被触发后，视图会自动调用表达式中绑定的方法。
## 2.6 轻量级模板解析
模板解析是Underscore中提供的一个方法。为什么我要在介绍Backbone特性时引入Underscore中的方法？因为该方法能帮助我们更好地分离视图结构和逻辑，且Underscore是Backbone必须依赖的库。
模板解析方法能允许我们在HTML结构中混合嵌入JS代码，就像在JSP页面中嵌入JAVA代码一样：
[html][view plain](http://blog.csdn.net/likun557/article/details/53159870#)[copy](http://blog.csdn.net/likun557/article/details/53159870#)
<ul>
<% for(vari=0; i<len; i++) { %>
<li><%=data[i].title%></li>
<% } %>
</li>
通过模板解析，我们不需要在动态生成HTML结构时拼接字符串，更重要的是，我们可以将视图中的HTML结构独立管理（例如：不同的状态可能会显示不同的HTML结构，我们可以定义多个单独的模板文件，按需加载和渲染即可）。
## 2.7 自定义事件管理
在Backbone中，你可以使用on或off方法绑定和移除自定义事件。在任何地方，你都可以使用trigger方法触发这些绑定的事件，所有绑定过该事件的方法都会被执行，如：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53159870#)[copy](http://blog.csdn.net/likun557/article/details/53159870#)
varmodel =newBackbone.Model();
// 在model对象中向自定义事件custom绑定两个函数
model.on('custom',function(p1, p2) {
// todo
});
model.on('custom',function(p1, p2) {
// todo
});
// 触发custom事件，将调用上面绑定的两个函数
model.trigger('custom','value1','value2');
// 移除custom事件中绑定的所有方法
model.off('custom');
// 触发custom事件，但不会执行任何函数，已经事件中的函数已经在上一步被移除
model.trigger('custom');
如果你熟悉jQuery，你会发现它们与jQuery中的bind、unbind和trigger方法非常类似。
另外，Backbone支持一个特殊事件”all”，当在一个对象中绑定了名为”all”的事件后，该对象在触发任何事件时，都会同时触发”all”事件中绑定的方法。有时这种方法会非常有用，例如我们可以通过”all”事件监听对象状态的变化。
## 2.8 路由器
在单页应用中，我们通过JavaScript来控制界面的切换和展现，并通过AJAX从服务器获取数据。
可能产生的问题是，当用户希望返回到上一步操作时，他可能会习惯性地使用浏览器“返回”和“前进”按钮，而结果却是整个页面都被切换了，因为用户并不知道他正处于同一个页面中。
对于这个问题，我们常常通过Hash（锚点）的方式来记录用户的当前位置，并通过onhashchange事件来监听用户的“前进”和“返回”动作，但我们发现一些低版本的浏览器（例如IE6）并不支持onhashchange事件。
Backbone提供了路由控制功能，通过Backbone提供的路由器，我们能通过一个简单的表达式将路由地址和事件函数绑定在一起，例如：
[javascript][view plain](http://blog.csdn.net/likun557/article/details/53159870#)[copy](http://blog.csdn.net/likun557/article/details/53159870#)
varCustomRouter = Backbone.Router.extend({
routes: {
'':'index',//当URL Hash在根目录时执行index方法：url\#
'list':'getList',//当URL Hash在list节点时执行getList方法：url\#list
'detail/:id':'query',//当URL Hash在detail节点时执行query方法，并将detail后的数据作为参数传递给query方法：url\#list/1001
'*error':'showError'//当URL Hash不匹配以上规则时, 执行error方法
},
index:function() {
alert('index');
},
getList:function() {
alert('getList');
},
query:function(id) {
alert('query id: '+ id);
},
showError:function(error) {
alert('error hash: '+ error);
},
});
varcustom =newCustomRouter();
Backbone.history.start();
请尝试将这段代码复制到你的页面中，并依次访问以下地址（其中URL表示你的页面地址）：
URL
URL\#list
URL\#detail/1001
URL\#hash1
URL\#hash2
请再试着使用浏览器的“返回”和“前进”按钮来回切换刚刚输入的地址。
你可以看到，当URL Hash发生变化时，会执行所绑定的方法，当遇到没有定义的Hash时，都会执行showError方法，并将未定义的Hash传递给该方法。
Backbone默认会通过Hash的方式来记录地址的变化，对于不支持onhashchange的低版本浏览器，会通过setInterval心跳监听Hash的变化，因此你不必担心浏览器的兼容性问题。
对于支持HTML5 pushState特性的浏览器，Backbone还允许你通过pushState来创建个性化的URL，但是这需要你的Web服务器做一些适配。

# 3. Backbone的适用性
Backbone并不像jQuery那样具有非常强的适用性，如果你正准备构建一个大型或复杂的单页Web应用，那么Backbone再适合不过。
如果想将Backbone应用到你的网站页面中，且页面中并没有复杂的逻辑和结构，那么这只会让你的页面更加繁琐和难以维护。
如果你的项目并不复杂，但你却深深喜欢它的某个特性(可能是数据模型、视图管理或路由器)，那么你可以将这部分源码从Backbone中抽取出来，因为在Backbone中，各模块间的依赖并不是很强，你能轻易的获取并使用其中的某一个模块。

# 4. 依赖库
你不能独立使用Backbone，因为它的基础函数、DOM操作、AJAX都依赖于第三方库。
## 4.1 Underscore
（必选）
Underscore是一个用于提高开发效率的基础函数库，它封装了对集合、数组、对象、函数的常用操作，就像jQuery封装DOM对象一样，你能通过Underscore轻易地访问和操作JavaScript内部对象。
Underscore还提供了一些非常实用的函数方法，如：函数节流、模板解析等。
关于Underscore中一些主要的方法，我会在下一章详细介绍，但在此之前你必须了解：Underscore是Backbone必须依赖的库，因为在Backbone中许多实现都是基于Underscore。
## 4.2 jQuery和Zepto
（可选）
相信你对jQuery一定不会陌生，它是一个跨浏览器的DOM和AJAX框架。
而对于Zepto你可以理解为“移动版的jQuery”，因为它更小、更快、更适合在移动终端设备的浏览器上运行，它与jQuery语法相同，因此你能像使用jQuery那样使用它。
Zepto目前仅支持Webkit内核的浏览器，因此它能兼容IOS、Adnroid、塞班、黑莓和Meego等大部分移动系统，而对于Windows Phone或Firefox OS，它暂时还不支持。
因为jQuery和Zepto语法相同，因此对于Backbone来说，你无论是使用jQuery还是Zepto，都没有问题（当然，你不可能两个同时都用到）。
在Backbone中，DOM选择器、DOM事件和AJAX，都使用了jQuery的方法。这里之所以所它们是可选的，是假设你没有用到Backbone中的视图和AJAX数据同步功能，那么就不需要导入它们。
如果你不想使用jQuery或Zepto，而是使用其它的、或自定义库，只要你的库中实现了与jQuery语法相同的DOM选择器、事件管理和AJAX方法，那么就不会任何问题。
Backbone允许你通过setDomLibrary方法动态配置需要使用的第三方库，这种情况常常用于：
你的自定义库虽然包含了和jQuery相同语法的方法，但全局变量并不是$，而且你想保持现有的命名。这时你可以通过setDomLibrary方法将其设置为Backbone内部引用的对象。
你希望通过检查用户的环境，来决定更适合使用哪一个库。例如：如果用户使用PC浏览器访问，则载入jQuery，如果用户通过移动终端访问，则载入Zepto。

