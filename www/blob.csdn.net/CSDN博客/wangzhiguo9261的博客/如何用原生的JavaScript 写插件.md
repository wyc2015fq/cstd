
# 如何用原生的JavaScript 写插件 - wangzhiguo9261的博客 - CSDN博客


2018年04月13日 09:31:18[码里偷闲王果果](https://me.csdn.net/wangzhiguo9261)阅读数：121


#### 一、前言
通过 "WWW" 原则我们来了解 JavaScript 插件这个东西
第一个 W "What" -- 是什么?什么是插件,我就不照搬书本上的抽象概念了,我个人简单理解就是,能方便实现某个功能的扩展工具.(下面我会通过简单的例子来帮助读者理解)
第二个 W "Why" -- 为什么? 为什么要有插件这种东西,首先结合第一个 W 来理解就是,使用插件的目的是方便我们实现某一个功能. 也就是说在编程过程中我们只需要找轮子,或者改轮子而不需要重新造轮子.节省开发时间,并且各司其职会更加专业(做得更好)。其次就是方便维护，因为每个功能模块可以分得更清楚，所谓的松耦合。
第三个 W "How" -- 如何做?我们如何开发 JavaScript 插件?这是我们这片文章要谈论的重点.
#### 二、准备知识
在讨论如何做之前我们不妨先通过反向思维来看看插件的特点。我们从如何使用 Javascript 插件开始。
假设我们现在要使用插件 js-plugin.js
第一步：引入插件，注意依赖项，例如有些插件是基于 jquery 编写的，先引入 jquery
第二步：通过插件提供的 API 实现我们所要的业务
以经典的 jquery 使用方法为例
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<script src="//cdn.bootcss.com/jquery/3.1.0/jquery.min.js"></script>
<script>$(function(){
        $("\#Id").html('hello world!');
    })</script>![复制代码](http://common.cnblogs.com/images/copycode.gif)
顺便说一句，能使用CDN的尽量使用CDN,这将使你的资源加载得更快.并节省你主机的带宽开销 传送门:[BootCDN](http://www.bootcdn.cn/)
上述两点其实也就是说我们的插件要做到，引入相关文件就可以方便地进行使用。换句话说插件必须满足下面的特点：
首先，我觉得插件最重要的一点 -- 复用性。就是说你这个插件在这个项目中是能用的，搬到另一个项目中它也是能用的（废话），并且原则上依赖项越少越好
其次，我觉得这是插件的宗旨 -- 易用性。开发一个插件，如果使用繁琐，倒不如重新造轮子，那就失去了插件的意义。
除此之外，当然还有高效性，考虑执行的效率还有内存的优化。
#### 三、Module 模式
插件开发不得不提的是 Modlule 模式，Module -- 模块，模块化开发，是在编程中十分通用的模式。说白了就是把业务需求分模块。每一个模块负责一个功能的实现。有点像其他面向对象编程语言中的类。例如 JsonHelper 专门负责 json 解析，FilesUpload,专门用来做文件上传的，等等这些。
插件就是用这样一种模块化思想来进行开发的，下面我们通过代码来简单解释下 Module 模式。
![复制代码](http://common.cnblogs.com/images/copycode.gif)
varHelloWorld =function(objId){var_get_dom =function(Id){returndocument.getElementById(Id);
    }var_aim_obj =_get_dom(objId);var_say_hello =function(str){
        _aim_obj.innerHTML= str;}return{
        sayHello:_say_hello
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
由上述代码可见，我们将某些功能，如 “sayHello” 给归到 HelloWorld （模块）中了。当然我们可以继续在下面添加其他功能，但都归于模块 HelloWorld 来管理。这就是 Module 的体现。
使用方法（注意这里使用了 new ）
varHei =newHelloWorld('hello');
Hei.sayHello('Hello Word');varHei2 =newHelloWorld('hi');
Hei2.sayHello('hi');
更直观点，我们来看下完整的代码
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html>
    <html>
    <head>
        <title>Module</title>
    </head>
    <body>
        <div Id="hello"></div>
        <div Id="hi"></div>
        <script type="text/javascript">varHelloWorld =function(objId){var_get_dom =function(Id){returndocument.getElementById(Id);
                }var_aim_obj =_get_dom(objId);var_say_hello =function(str){
                    _aim_obj.innerHTML=str;
                }return{
                    sayHello:_say_hello
                }
            }varHei =newHelloWorld('hello');
            Hei.sayHello('Hello World');varHei =newHelloWorld('hi');
            Hei.sayHello('hi');</script>
    </body>
    </html>![复制代码](http://common.cnblogs.com/images/copycode.gif)
运行结果如下
![](https://images2015.cnblogs.com/blog/783708/201611/783708-20161104121622768-1123960535.png)
我们这里需要注意的是，每使用 new 创建出来的新对象都将开辟新的内存空间（新的一份copy），只要引用没有释放，那么该对象的占用的内存空间将不会被回收。那么如何避免过多浪费内存呢？一句话“释放引用”，只需要释放对该对象的`所有`引用，垃圾回收机制就会将该对象占用的内存空间回收。
varHei =newHelloWorld('hello');
Hei.sayHello('Hello World');
    
Hei=null;//解除引用
这样还要“手动”内存管理，麻烦。如何让该模块在内存中只保留一份（copy）呢？请看下面一段代码
![复制代码](http://common.cnblogs.com/images/copycode.gif)
varHelloWorld = (function(){var_getDom =function(Id){returndocument.getElementById(Id)                    
    }var_sayHello =function(Id,str){
        _getDom(Id).innerHTML=str;
    }return{
        getDom:_getDom,
        sayHello:_sayHello
    }
}())![复制代码](http://common.cnblogs.com/images/copycode.gif)
使用方法
HelloWorld.sayHello('hello','hello text');
是的,正如你所见到的,不需要 new 了。使用时不再需要创建新对象，也就是说我们只保持了该对象在内存中的一份引用，也就是HelloWorld 对它的引用。当 HelloWorld 对其引用解除时其所占用的内存将得到释放。上述代码实质上是一个匿名闭包。如果对闭包不是很理解的朋友可以看看我写的上一篇文章[《浅析 JavaScript 中的闭包(Closures)》](http://www.cnblogs.com/cboyce/p/6003269.html#3542151)
#### 四、插件基础代码
了解了上面的种种之后我们要开始直切主题了。
首先我们创建一个 js 文件 取名为 first-js-plugin.js(啥名字都行)，键入以下代码
![复制代码](http://common.cnblogs.com/images/copycode.gif)
;varplugin =(function(){function_firstFunc(str){
        console.log(str);
    };return{
        firstFunc: _firstFunc,
    };
})();![复制代码](http://common.cnblogs.com/images/copycode.gif)
再创建一个 HTML页面 取名为 pluginTest.html (啥名字都行)
完整代码如下
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!DOCTYPE html>
<html>    
<head>
    <title></title>
    <script type="text/javascript" src="./first-js-plugin.js"></script>
    <script type="text/javascript">plugin.firstFunc("Hello ! I am firstFunc");</script>
</head>
<body>
</body>
</html>![复制代码](http://common.cnblogs.com/images/copycode.gif)
运行结果如下图显示
![](https://images2015.cnblogs.com/blog/783708/201611/783708-20161104121610721-342045453.png)
通过这个简单的插件，我们来分析一下里面的代码.
在分析代码之前我们先来了解另一个东西,自调用匿名函数(防止插件用户定义函数与插件冲突)
(function(){//code })();
可能有些童鞋会觉得有点陌生,那看下下面的代码
varfunc =function(){//code } func();
其实这两段代码是等价的,当然有点差别,第一个是匿名函数.作用都是定义一个函数并立即执行.
(function(){//code })();
代码分析:
最后面的小括号 () 表示执行该函数
(匿名函数) 小括号(分组表达式)包起来匿名函数的声明,作用相当是将函数声明转为表达式,这样才能执行,仅此而已
如果采取以下写法
function(){//code }();
编译器报错,问题是函数声明无法执行,表达式才能执行
搞清楚这些之后我们回头给下面的代码加上分析,如下
![复制代码](http://common.cnblogs.com/images/copycode.gif)
;//JavaScript 弱语法的特点,如果前面刚好有个函数没有以";"结尾,那么可能会有语法错误/*plugin.api_funcs 给对象设置属性,属性值为 自调用匿名函数
     这里涉及到js作用域链以及闭包的知识点*/varplugin =(function(){function_firstFunc(str){
            alert(str);
        };//返回APIreturn{
            firstFunc: _firstFunc
        };
    })();![复制代码](http://common.cnblogs.com/images/copycode.gif)
我们将代码抽取一下(只为帮助理解，已经理解的朋友请忽略)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//01.定义变量varplugin =某对象;//02.创建对象并返回(function(){//code;return ...})();//匿名执行函数 return 某对象//然后看核心的返回return{firstFunc: _firstFunc};//说白了就是,通过某个key将一个函数存储起来.使用时通过key访问到这个函数varplugin = {key:function(){//code}}//所以最终的体现如下varplugin = {firstFunc: “具体的函数引用”}![复制代码](http://common.cnblogs.com/images/copycode.gif)
所以我们最后才能通过，`插件名.属性`来使用插件，正如：
plugin.firstFunc("Hello ! I am firstFunc");
四、插件的几种写法
这里我就不墨迹了,直接上代码,关键处会给注释
面向对象思想 类方式
![复制代码](http://common.cnblogs.com/images/copycode.gif)
//自定义类functionplugin(){}//提供默认参数plugin.prototype.str = "default param";//提供方法(如果不传参,则使用默认参数)plugin.prototype.firstFunc =function(str =this.str){
    alert(str);
}//创建"对象"varp =newplugin();//调用方法p.firstFunc("Hello ! I am firstFunc");//Hello ! I am firstFuncp.firstFunc();//default param![复制代码](http://common.cnblogs.com/images/copycode.gif)

闭包方式
闭包方式就是我们刚刚一直在介绍
![复制代码](http://common.cnblogs.com/images/copycode.gif)
varplugin =(function(){function_firstFunc(str){
        alert(str);
    };return{
        firstFunc: _firstFunc,
    };
})();![复制代码](http://common.cnblogs.com/images/copycode.gif)

第二种方式上的一些变化
![复制代码](http://common.cnblogs.com/images/copycode.gif)
(function(){//定义一些默认参数var_options={
        default_word:"default hello"}//定义一些apivar_plugin_api ={
        firstFunc:function(str =_options.default_word){
            alert(str);returnthis;//返回当前方法},
        secondFunc:function(){
            alert("secondFunc");returnthis;//返回当前方法}
    }//这里确定了插件的名称this.CJPlugin =_plugin_api;
})();
CJPlugin.firstFunc("hello");//helloCJPlugin.firstFunc();//default helloCJPlugin.secondFunc();//secondFunc![复制代码](http://common.cnblogs.com/images/copycode.gif)

##### 结语
JavaScript 插件的相关知识今天暂时聊到这了．下篇文章笔者将通过实例来介绍如何开发一款属于自己的省市区三级联动插件．如果有朋友正在学习插件开发．那么下篇文章可能我们有更多可以探讨的话题。
限于笔者技术，文章观点难免有不当之处，希望发现问题的朋友帮忙指正，笔者将会及时更新。也请转载的朋友注明文章出处并附上原文链接，以便读者能及时获取到文章更新后的内容，以免误导读者。笔者力求避免写些晦涩难懂的文章（虽然也有人说这样显得高逼格，专业），尽量使用简单的用词和例子来帮助理解。如果表达上有好的建议的话也希望朋友们在评论处指出。
本文为作者原创,转载请注明出处![Cboyce](http://www.cnblogs.com/cboyce/p/6029782.html)

