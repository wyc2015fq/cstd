# 也谈分离JavaScript和HTML - 沧海一粟 —— 技术随手记 - CSDN博客





2011年02月18日 13:51:00[慢游](https://me.csdn.net/eengel)阅读数：9562








    不知道能不能将JavaScript和HTML的分离也归结为“你必将业务逻辑由显示中分离（thou shalt separate business logic from display）”[1]
的问题。但有HTML和CSS的分离在前面对照，它们的分离是如此地优雅，对于理想主义的小coder实在具有莫大的吸引力。

    从现实的角度，也可以举出很多方面来论证JavaScript和HTML分离的好处：网页设计人员和JavaScript程序员可以分工合作，尽可能降低沟通成本；当某些浏览器禁用JavaScript的时候不至于引起错误，提高网站的可访问性。

JavaScript和HTML分离的理想情况有两种：
**1.    JavaScript中无HTML**

    无法做到。DOM生来就是给JavaScript操作HTML的。JavaScript的两大功能：一是操作HTML，通过各种手段改变HTML页面外观；二是逻辑运算，随着富客户端的发展，JavaScript在这方面的运用也越来越多、越来越“正规”，甚至有如《JavaScript设计模式》[2]这样的书出来。
**2.    HTML中无JavaScript**

    这儿就从“为什么开发人员会把JavaScript代码放到HTML中去？
”这个问题的角度进行分析。

    最常见的。**为HTML的节点添加事件**
。比如给一个按钮添加事件：当点击的时候弹出对话框。

（1）JavaScript和HTML紧密结合的做法：（本文所有的代码在Firefox3.6测过）
test.html

<!DOCTYPE HTML>

<HTML>

<HEAD></HEAD>

<BODY>

<input type="button" value="click me" onclick="alert('This is a button.');"/>

</BODY>

</HTML>

（2）将散乱的JavaScript抽象到函数中的做法：
test.html

<!DOCTYPE HTML>

<HTML>

<HEAD>

    <script type="text/javascript">

        function showMessage(){

            alert('This is a button.');

        }

    </script>

</HEAD>

<BODY>

<input type="button" value="click me" onclick="showMessage();"/>

</BODY>

</HTML>

（3）JavaScript函数showMessage()似乎没必要待在.html文件中，扔到.js中去吧：
a.js

function showMessage(){

    alert('This is a button.');

}
test.html

<!DOCTYPE HTML>

<HTML>

<HEAD>

    <script type="text/javascript" src="/site_media/js/my/a.js" ></script>

</HEAD>

<BODY>

<input type="button" value="click me" onclick="showMessage();"/>

</BODY>

</HTML>

    对比（1）的情况，（3）已经简洁很多。但想象一下，对于应用型的网站，这种需要加事件的HTML节点少则几十，多则上百，其维护和更新无论是对网页设计人员还是JavaScript开发人员都是一个沉重的负担。那如何才能将各节点上的onclick事件也“隐藏”起来？

    来看看这些onclick的原理。当浏览器解析test.html的时候，遇到input按钮节点中有onclick的声明，则在对应的DOM树上加入此事件。因此当页面加载完毕后，一旦点击此按钮，对应的事件就可以被触发而弹出对话框。

    一旦我们去除input上的onclick，

（4）首先，要在另外一个地方声明input的onclick事件，即a.js中的attachEvents()函数。
a.js

function attachEvents(){

    document.getElementById("input1").onclick = showMessage;

}

function showMessage(){

    alert('This is a button.');

}

    但是，要怎么在HTML加载的时候触发attachEvents()呢，要知道，浏览器是不会主动执行.js中的JavaScript代码的。JavaScript代码需要放在HTML中，这样浏览器解析HTML时会执行包含的JavaScript代码。

    可是我们并不想把attachEvents()函数放回到test.html中去，那就必须用到某些HTML与JavaScript的“关联”手段，而且这种关联必须在HTML中完成。

    这种关联也是“为什么开发人员会把JavaScript代码放到HTML中去？”问题的一种原因。也就是**在页面生命周期中的某些时刻触发特定的JavaScript语句/函数**
。用的最多的就是在页面加载完毕时对相关资源进行初始化，也就是window.onload。上面的例子，我们打算将attachEvents()函数放到HTML加载完成后执行，即：
test.html

<!DOCTYPE HTML>

<HTML>

<HEAD>

    <script type="text/javascript" src="/site_media/js/my/a.js" ></script>

</HEAD>

<BODY>

<input id="input1" type="button" value="click me"/>

<script type="text/javascript">

    window.onload = attachEvents;

</script>

</BODY>

</HTML>

    注意，这儿JavaScript对input节点的操作是通过属性id来进行的。因此不需要在节点上显式地加入onclick属性和相应的JavaScript代码。这样，无论有多少节点需要加事件，都可以只修改a.js中的attachEvents()函数而不用管HTML页面。

    另外，JavaScript对HTML节点的访问除了id之外还有其他方式。但对于事件关联，用的最多的还是id。因此网页设计人员在写HTML时，对有可能关联事件的节点添加id属性也是一个很好的习惯。

**结论**

    理论上来说，除了<head>中包含.js文件的引用，以及在合适的地方添加类似window.onload的关联手段触发JavaScript代码的执行之外，其他形式的JavaScript混入HTML应该都可以避免。

    当然，实际情况会比本文例子中的情况复杂得多，但上面的结论也可以作为参考，让JavaScript和HTML的分离更加理想。


参考文献

[1] Parr, T.J., Enforcing Strict Model-view Separation in Template Engines. WWW ’04: Proceedings of the 13th international conference onWorldWideWeb, New York, NY, USA, ACM Press (2004) 224–233, http://www.cs.usfca.edu/~parrt/papers/mvc.templates.pdf

[2] JavaScript设计模式, http://book.douban.com/subject/3329540/



