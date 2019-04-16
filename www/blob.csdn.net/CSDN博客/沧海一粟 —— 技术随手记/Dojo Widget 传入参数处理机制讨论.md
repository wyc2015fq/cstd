# Dojo Widget 传入参数处理机制讨论 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年07月08日 15:10:07[慢游](https://me.csdn.net/eengel)阅读数：3388








    前一阵子被Dojo Widget的参数处理机制搞得头大，最近终于腾出手来好好整理了下。



    熟悉Dojo的人，都非常喜欢用Dijit组件模型进行开发。不仅可以通过异常简便的方式在网页上创建具有特殊功能的页面元素，也可以很方便地对这些元素进行重构。下面就是一个自定义dijit.layout.ContentPane的例子（Dojo 1.5.0,Firefox 3.6.15, Firebug 1.5.4）：

```java
dojo.require("dijit.layout.ContentPane");
dojo.declare("Test", [ dijit.layout.ContentPane ], {
	tagattr1: "",
	
	constructor: function(){
		console.log("=============constructor=============");
		console.log("this.title: ", this.title);
		console.log("this.tagattr1: ", this.tagattr1);
		console.log("arguments[0].title: ", arguments[0].title);
		console.log("arguments[0].tagattr1: ", arguments[0].tagattr1);
	},
	
	postCreate: function(){
		console.log("=============postCreate=============");
		console.log("this.title: ", this.title);
		console.log("this.tagattr1: ", this.tagattr1);
	},
	
	startup: function(){
		console.log("=============startup=============");
		console.log("this.title: ", this.title);
		console.log("this.tagattr1: ", this.tagattr1);
	}
});
```


    其中的几个方法都是dijit本身的生命周期方法，调用顺序为constructor、postCreate、startup。通过观察firebug console中的输出，可以检测Test对不同参数的处理方式。



    Dojo为了方便开发人员，提供了两种构建 Dijit的方式[1,2]：声明式(Declaratively)和编程式(Programmatically)。下面就分别介绍一下两种情况下Dijit是如何处理参数的。



**声明式(Declaratively)**
`<div dojoType="Test" title="Space" tagattr1="tag gogo">This is a customized content pane.</div>`

    当上面的div被解析时，所有的属性会被当做Test的参数传入。运行后的console输出如下：

```
=============constructor=============
this.title:
this.tagattr1:
arguments[0].title: Space
arguments[0].tagattr1: tag gogo
=============postCreate=============
this.title: Space
this.tagattr1: tag gogo
=============startup=============
this.title: Space
this.tagattr1: tag gogo
```


观察结果：

（1）constructor中，参数的值还没有被赋到this中，但参数本身已经存在（是空字符串）; 到postCreate中，参数的值就被赋到this中了。

（2）如果想在constructor中就拿到参数的值，可以通过arguments[0]得到。



    如果将Test中的 tagattr1: "",声明去掉，将会得到不同的console输出：

```
=============constructor=============
this.title:
this.tagattr1: undefined
arguments[0].title: Space
arguments[0].tagattr1: undefined
=============postCreate=============
this.title: Space
this.tagattr1: undefined
=============startup=============
this.title: Space
this.tagattr1: undefined
```


观察结果：

（1）虽然在div中定义了属性tagattr1，但在Test的所有生命周期函数中，tagattr1还是被当做是未定义的，包括在constructor的arguments[0]中。



    其实Test的参数可以被分成两类：固有参数，自定义参数。固有参数包括：id, class,style, title, href [3]等，基本上涵盖了div标签的标准属性 。这些参数在dijit.layout.ContentPane及其祖先中已经被定义好了，因此在Test中可以直接使用。但自定义参数不仅需要先在html tag中添加，而且在Test中也需要进行预定义。



**编程式(Programmatically)**

    再来看看编程式创建Test实例的方法。

```java
dojo.ready(function(){
	var test = new Test({
		title: "Space",
		tagattr1: "js gogo"
	}, dojo.byId("container"));
	test.startup();
});
```


    注意这儿test.startup();必须有，否则Test的startup方法将不会被触发。

    console输出如下。和上面声明式得到的结果一样。

```
=============constructor=============
this.title:
this.tagattr1:
arguments[0].title: Space
arguments[0].tagattr1: js gogo
=============postCreate=============
this.title: Space
this.tagattr1: js gogo
=============startup=============
this.title: Space
this.tagattr1: js gogo
```


    有趣的来了：如果将Test中的 tagattr1: "",声明去掉，则console输出为：

```
=============constructor=============
this.title:
this.tagattr1: undefined
arguments[0].title: Space
arguments[0].tagattr1: js gogo
=============postCreate=============
this.title: Space
this.tagattr1: js gogo
=============startup=============
this.title: Space
this.tagattr1: js gogo
```


观察结果：

（1）虽然在constructor中，this.tagattr1是未定义的，但到了postCreate中，它就被定义并赋值了。

（2）在Test中，虽然tagattr1没有定义，但在constructor的arguments[0]中却已存在。



    在实际应用中，创建一个自定义dijit如Test，一般都会创建多个实体，也不拘通过声明式还是编程式。因此在处理参数时，最好确保在两种方式下得到的结果都一样。从上面的讨论，可以总结出几条需要注意的地方：

（1）在constructor中，不要通过this访问传入的参数的值，而要通过arguments[0]。

（2）从postCreate开始，可以通过this访问传入的参数的值。包括在它后面的startup、onDownloadEnd（当设置href属性时，该方法会在内容加载完成后触发）。

（3）如果要使用自定义的参数，除了在声明的时候声明它并赋值，还必须在dijit的定义主体中定义该属性。

（4）如果不确定外部声明的时候是否会传入某个参数，可以在constructor中事先定义其初始值。如果声明的时候未声明该参数的值，则初始值一直都会存在;如果声明的时候声明了该参数的值，则从postCreate开始，该参数的值都变成了声明中定义的值。





**参考文献：**

[1] 使用Dijit实现界面组件化开发, [http://blog.csdn.net/dojotoolkit/article/details/6152243](http://blog.csdn.net/dojotoolkit/article/details/6152243)

[2] Creating Dojo Widgets Programmatically orDeclaratively,  [http://davidwalsh.name/dojo-widget](http://davidwalsh.name/dojo-widget)

[3] Dojo toolkit, [http://dojotoolkit.org/api/1.6/dijit/layout/ContentPane](http://dojotoolkit.org/api/1.6/dijit/layout/ContentPane)




