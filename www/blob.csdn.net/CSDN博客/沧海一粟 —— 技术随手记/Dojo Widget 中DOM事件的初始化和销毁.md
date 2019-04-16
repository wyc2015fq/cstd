# Dojo Widget 中DOM事件的初始化和销毁 - 沧海一粟 —— 技术随手记 - CSDN博客





2011年07月11日 22:18:52[慢游](https://me.csdn.net/eengel)阅读数：4761








这篇帖子还是属于Dojo Widget系列吧。从上一篇[dijit.layout.ContentPane 中的标准方法/事件在不同情况下的调用](http://blog.csdn.net/eengel/article/details/6595473)中，我们看到了dijit.layout.ContentPane 中的标准方法/事件在不同情况下是如何被调用的，这儿就可以基于其结论探讨一下，如何在一个自定义dijit.layout.ContentPane中进行DOM事件的初始化与销毁。



每个开发者用dijit.layout.ContentPane各有各的理由，我看中的是它良好的封装，及丰富的属性、方法和事件。基本上能够满足对一块“专门的页面内容”的大部分操作，比如刷新，改变内容，变成tab list中的一个tab等。

为了让这块内容的行为相对独立，我一般习惯把这块内容中DOM事件的操作全都放在对应的content pane实例中，而不是整个页面实例。

当然，这样的实现方式一般都是针对用户交互很多的网站。如果网站大多都是静态内容，也没有太多交互功能，就没有必要搞这么复杂了。



自定义dijit.layout.ContentPane实例的创建有很多种，这儿采用的是：

（1）声明式

（2）Test实例挂在TabContainer下，并在一开始隐藏

（3）在html tag中设置href，preload为true

其他的创建方式得到的结果和这种大同小异。不过如果不是通过设置href获取内容，而是tag下面本身带内容，则会有一些地方不同。最后会统一讨论一下。



另外，为了操作众多DOM事件，有必要将其handler记录在案。这样就可以在不用的时候集中销毁。

可以在自定义组件中定义一个array变量 handlers。



`handlers: [],`




1. 什么时候进行DOM事件初始化？

有两个条件：

（1）内容都加载完毕了

（2）越迟越好

从 中可以看到，设置href的情景中，满足上述两个条件的就是onDownloadEnd。因此我们在onDownloadEnd中进行DOM事件的初始化。



2. 什么时候销毁DOM事件？

这个问题就必须区分多种情况了。DOM事件和DOM节点息息相关，如果DOM节点没有了存在的必要，就需要将其对应的DOM事件预先销毁。这儿列举了5大会导致DOM节点消失的情况，及相应调用的标准方法和事件：



（1）关闭Test实例所在的tab （点击tab上的X，或者调用tabcontainer closeChild()）



```
closeChild()）
destroyRecursive  
onUnload  
destroy  
uninitialize  
cancel  
destroyRendering
```

（2）调用tabcontainer destroyDescendants()




```
onShow  
destroyRecursive  
onUnload  
destroy  
uninitialize  
cancel  
destroyRendering
```




（3）调用Test destroyRecursive()

```
destroyRecursive
onUnload
destroy
uninitialize
cancel
destroyRendering
```


（4）调用Test refresh()

```
refresh  
cancel  
onDownloadStart  
onUnload  
onLoad  
onDownloadEnd
```


（5）设置一个新的href

```
cancel  
onDownloadStart  
onUnload  
onLoad  
onDownloadEnd
```


前面三种情况很好理解，连content pane都被销毁了，必然相应的DOM节点也会销毁。后两种情况中，可以看到都会调用onDownloadEnd。由于我们在onDownloadEnd中进行DOM事件的初始化，这样一来，无论DOM节点还是不是和原来一样，必然会导致每次都会初始化新的DOM事件，handlers列表会越来越长。更严重的后果是，会造成同一个DOM节点上挂载了多个功能相同的事件，也就是同一个事件会被同时触发多次。由此导致的交互响应混乱的错误会很致命，且非常隐蔽，难以查找。

综合上面5种情况，可以将DOM事件的销毁放在onUnload 中。



如果不是通过设置href获取内容，而是tag下面本身带内容，在加载的时候就不会调用onDownloadEnd。根据上面说的两个条件，可以把DOM事件的初始化放到postCreate()或startup()中。而由于不设置href，销毁事件中的后两种情况就不会发生。根据前三种情况，可以把DOM事件的销毁放到uninitialize或destroyRendering中。



**结论：**



设置href的情况：

1. 建议在onDownloadEnd中初始化DOM事件。

```java
onDownloadEnd: function(){
	var handle = dojo.connect(dijit.byId(“test”), “onclick”, function(){
		//add code here
	});
	this.handles.push(handle);
}
```


2. 建议在onUnload中销毁 DOM事件。
```java
onUnload: function(){
            dojo.forEach(this.handles,dojo.disconnect);
            this.handles= [];                  
            this.inherited(arguments);
}
```


 不设置href的情况：

1. 建议在postCreate()或startup()中初始化DOM事件。

2. 建议在uninitialize或destroyRendering中销毁 DOM事件。




