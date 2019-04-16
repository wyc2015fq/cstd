# dojo.declare/dojo.provide/dojo.require：JavaScript模块化管理工具 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年03月01日 20:47:00[慢游](https://me.csdn.net/eengel)阅读数：4231








    在中大型软件的开发过程中，代码的模块化管理非常重要。众所周知支持良好的模块化管理的语言是Java，通过package/import机制来实现。相比较而言，JavaScript作为运行在前端的解释性语言，其模块化管理一直都没有受到足够的重视。对于以静态页面为主的网站，JavaScript一般就是做做网页特效，大多数情况下都是直接写入到HTML页面中，也谈不上模块化不模块化。但是随着富客户端的蓬勃发展，特别是web应用的大量出现，客户端的功能越来越复杂。JavaScript不仅需要支持变化多端的页面呈现，也需要提供精细的用户交互逻辑支持。这样一来，造成的直接后果就是JavaScript代码量急剧增加，管理难度加大。这种情况下，当务之急就是引入模块化管理工具。

    JavaScript工具包的发展让这种需求的满足成为了可能。dojo.declare/dojo.provide/dojo.require就是dojo提供的用于管理JavaScript代码的工具。“[understanding dojo.declare, dojo.require, and dojo.provide](http://dojocampus.org/content/2008/06/03/understanding-dojodeclare-dojorequire-and-dojoprovide/)
”一文中对比了模块化管理在Java和Dojo中的不同实现方式。对Java比较熟悉而不熟悉Dojo的朋友，可以参考一下此文。

    本文首先介绍dojo.declare/dojo.provide/dojo.require分别是干什么的，然后总结一下该模块化管理工具对JavaScript开发带来了哪些便利之处。如果想了解dojo.provide的内在机制，可以参考“[dojo.provide：你到底“提供”了些什么？](http://blog.csdn.net/eengel/archive/2011/01/07/6122368.aspx)
”一文。

**介绍**

**dojo.declare**

    dojo.declare命令创建一个JavaScript对象。比如：

dojo.declare("com.test.dialog1", [dijit.Dialog], {

        a: "hello",

        title: "test dialog"

    });

    其中，“com.test.dialog1”是对象名，它继承了dijit.Dialog对象，因此是一个可自定义的网页对话框。它具有继承自dijit.Dialog的属性title，即出现在对话框上边栏的文字，以及自定义属性a。属性a和title被封装在对象com.test.dialog1中，无法被全局访问。有关dijit.Dialog对象的继承原理，可以参考“[dijit.Dialog样例及其原型关系](http://blog.csdn.net/eengel/archive/2011/01/17/6146865.aspx)
”一文。

**dojo.provide**

    创建了一个个JavaScript对象，那如何让别的JavaScript代码和HTML页面知道它们呢？这个时候dojo.provide就派上用场了。在如下的代码中：

dojo.provide(“com.test.dialog1”);

dojo.declare("com.test.dialog1", [dijit.Dialog], {

        a: "hello",

        title: "test dialog"

    });

    语句dojo.provide(“com.test.dialog1”)包含了多种含义：

1. 上述JavaScript代码被放置在文件夹/com/test/下的dialog1.js文件中；

2. 此dialog1.js文件以唯一命名“com.test.dialog1”暴露于其他JavaScript代码和HTML页面。

**dojo.require**

    有了前面dojo.provide开放的JavaScript资源，dojo.require就是引用它们的方法。一旦在页面中调用：

    dojo.require(“com.test.dialog1”);

    dojo.require也有多种功用：

1. 声明js代码之间的依赖关系；

2. 加载相应的js文件dialog1.js到客户端。


    从上面的介绍可以看出，dojo.declare/dojo.provide/dojo.require提供了模块化管理的大多数特性。下面就详细列举一下由此产生的便利之处。

**便利之处**

**统一命名、统一部署**

    由dojo.provide和JavaScript代码放置的位置和命名可以看到，只要将JavaScript代码组织到一个树形目录下，就可以用dojo.provide给每个js文件一个统一的命名方法。同样，通过dojo.provide中的名字，就可以很容易地找到js文件在服务器目录中的位置。

**加载简便**

    有了dojo.require，就不需要在HTML页面中使用大量的<script>标签了。

    以前：

    <script type="text/javascript" src="/site_media/js/com/test/dialog1.js" ></script>

    现在：

    <script type="text/javascript">

        dojo.require(“com.test.dialog1”);

    </script>

    当js文件有很多的时候效果更明显。可以减少容易出错的对js文件路径的引用，并节省很多页面空间。
**统一管理，可促进JavaScript和HTML的分离**

    既然dojo.require已经脱离了<script>标签，调用它们的地方就可以进行统一管理。比如可以专门写一个requireJSFiles()函数，把当前页面中要用到的所有js文件的调用都放到里面。这样，页面用到的js文件一目了然，将来要做修改、增删js文件也非常简便，不会因为js文件的调用散落在各处而引起不必要的麻烦。

    同样，由于requireJSFiles()函数可以放到专门的js文件中，等于将JavaScript加载的代码同HTML页面分离了。

**可划分变量及函数的定义域**

    关于全局变量的讨论，我想网上已经有很多资料了。科学的设计，是尽量避免使用全局变量。而dojo.declare具有的封装性，很好地满足了这个需求。它创建的JavaScript对象，其中的变量及函数都是内部成员，无法被全局访问。对于JavaScript代码量庞大的项目来说，这样可以避免很多不必要的麻烦。

**无重复加载**

    由于dojo.require有重复加载的检查机制，因此在一个web项目的多处引用dojo.require(“com.test.dialog1”)也不会造成性能的下降。当检测到“com.test.dialog1”已被加载后，后面的dojo.require(“com.test.dialog1”)都不会被执行。

**代码扩展性好**

    无论是想增加、修改还是删除js文件，这个机制都能够很好地帮助开发人员快速进行。比如想增加一个新的自定义网页对话框，首先在/com/test/文件夹下新建一个js文件叫dialog2.js，然后在其中声明对应的对话框：

dojo.provide(“com.test.dialog2”);

dojo.declare("com.test.dialog2", [dijit.Dialog], {

        b: "world",

        title: "test dialog 2"

    });

调用的时候，直接增加一句就可以了：

    <script type="text/javascript">

        dojo.require(“com.test.dialog1”);

dojo.require(“com.test.dialog2”);

    </script>

**小结**

    dojo.declare/dojo.provide/dojo.require为JavaScript提供了类似Java的模块化管理工具，在富客户端及web应用开发中，可以极大地帮助前端开发人员组织JavaScript代码，提高工作效率，减少出错，并有效地促进JavaScript和HTML的分离。



