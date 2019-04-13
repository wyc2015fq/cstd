
# electron开发记录(五)：调用jquery,bootstrap并对其一些用法的总结 - Multiangle's Notepad - CSDN博客


2016年12月13日 16:05:40[multiangle](https://me.csdn.net/u014595019)阅读数：8344



---本文主要分两个部分，分别是在electron中调用jquery和bootstrap的方法以及为什么要这么调用，以及对jquery一些用法的总结。

---electron中调用jquery及bootstrap的方法

---jquery作为一个js库，能够很大程度上的简化对页面中元素的控制。而bootstrap作为css库，能够方便的写出好看的界面。调用bootstrap需要先调用jquery。在调用bootstrap的时候，我一开始是按照网页中调用bootstrap的方法来的，也就是照着bootstrap官网上的方法：

---<

---link

---href

---=

---"http://cdn.static.runoob.com/libs/bootstrap/3.3.7/css/bootstrap.min.css"

---rel

---=

---"stylesheet"

--->

---<

---script

---src

---=

---"http://cdn.static.runoob.com/libs/jquery/2.1.1/jquery.min.js"

--->

---</

---script

--->

---<

---script

---src

---=

---"http://cdn.static.runoob.com/libs/bootstrap/3.3.7/js/bootstrap.min.js"

--->

---</

---script

--->

---首先调用bootstrap的css库，再调用jquery,再调用bootstrap的js文件。看起来没问题对不对？但是在实际运行的时候，报错了：

---Uncaught Error: Bootstrap's JavaScript requires jQuery

---。但是我明明是先引入的jQuery再引入bootstrap的呀？

---再查了查资料，哦，原来是

---electron 的 Renderer 端因为注入了 Node 环境，存在全局函数 require，导致jquery 内部环境判断出现问题

---。对应的解决方法是，在你引入jQuery之前添加这段代码

---window

---.nodeRequire =

---require

---;

---delete

---window

---.

---require

---;

---delete

---window

---.

---exports

---;

---delete

---window

---.

---module

---;

---这样，就可以用nodeRequire来使用node模块，又能使用jQUery了

---另外还有一种方法，先用npm安装好bootstrap和jquery，然后使用require来调用(如果无法安装，则加上–global参数)

---window

---.$ =

---window

---.jQuery =

---require

---(

---'../node_modules/jquery/dist/jquery.min.js'

---);

---require

---(

---'../node_modules/bootstrap/dist/js/bootstrap.min.js'

---);

---然后将html页面改成

---<

---link

---href

---=

---"http://cdn.static.runoob.com/libs/bootstrap/3.3.7/css/bootstrap.min.css"

---rel

---=

---"stylesheet"

--->

---<

---script

--->

---window.$ = window.jQuery =

---require

---(

---'../node_modules/jquery/dist/jquery.min.js'

---);

---require

---(

---'../node_modules/bootstrap/dist/js/bootstrap.min.js'

---);

---</

---script

--->

---再运行，报错信息就消失了。

---jquery的一些用法总结

---选中当前对象

---是使用jquery的时候，有时候会需要选中当前的对象。有个例子，我点击一个按钮，那么我如果相对这个按钮进行操作，肯定首先是要选中这个按钮。关于如何选中当前组件，很多地方都说使用

---$

---(

---"button"

---)

---.

---click

---(()=>{

---var

---btn = $(

---this

---) ;
})

---就可以了，但是我这里好像不行，原因不明。后来我查了查，发现改成

---$

---(

---"button"

---)

---.

---click

---((e)=>{

---var

---btn = e.target ;
})

---这样就行了，此时btn代表了你点击的对象。如果是要jquery对象，那就稍微改一下,

---var btn = $(e.target);

---即可。

---修改属性(attr)值

---什么是属性值？就是出现在div或者其他块中的内容。举个例子，

---<

---div

---id

---=

---"hehe"

---class

---=

---"hahaha"

--->

---23333

---</

---div

--->

---那么这个div块就有一个名叫id的属性，值为hehe,以及名叫class的属性，值为hahaha。那么当该div块被选中并且被转化为jquery对象以后，能够使用.attr函数来查询和修改块的属性值。

---var

---btn = $(

---"\#id"

---) ;
btn.attr(

---"class"

---) ;

---//

---查询属性

---class

---的值。

---btn.attr(

---"class"

---,

---"heiheihei"

---)

---//

---将属性

---class

---的值改为

---heiheihei

---修改value值

---类似的，jquery对象还可以使用.val来控制value属性的值。即下面两行的功能等价

---$(

---"div"

---).val() ;

---$(

---"div"

---).attr(

---"value"

---) ;

---下面两行的功能也等价

---$(

---"div"

---).val(

---"2333"

---) ;

---$(

---"div"

---).attr(

---"value"

---,

---"2333"

---) ;

---修改text值

---使用.text()能够获得div块中间的文本内容。比如说下面这个

---<

---div

---id

---=

---"hehe"

---class

---=

---"hahaha"

--->

---23333

---</

---div

--->

---那么$(“div”).text()返回的内容就是’23333’了。


