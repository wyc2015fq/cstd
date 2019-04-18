# input type为checkbox或radio时的click默认事件 - z69183787的专栏 - CSDN博客
2014年03月14日 11:13:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：36686
　　在input中，如果type为checkbox或radio时，浏览器会将该input渲染成为系统的单选或多选组件，如果这时，我们在这个input上绑定click事件，那就要小心谨慎使用e.preventDefault()这个方法（jQuery中整合了这个方法使得它能够兼容去掉浏览器中的默认事件）。之所以要说谨慎使用，就是，如果你在这个事件的响应程序中判断该checkbox是否选中时，得到的结果和真正的选中状态会有所不同。下面先从一个简单的示例说明这个现象。（为了简单起见，我使用了jquery，他能节省选择器部分的代码，但不影响本文需要说明的问题）。代码段如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<script type="text/javascript">
window.onload=function(){
    $('#chk').on('click',function(e){
        e.preventDefault();
        alert($(this).attr('checked'));
    });
}
</script>
</head>
<body>
<input type="checkbox" id="chk" />
</body>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　示例很简单，就是在一个checkbox上绑定一个click事件，这个click事件是判断当前的checkbox是否选上了。通过在浏览器上实际运行，结果是，在鼠标点击checkbox的时候，弹出checked。但是关掉alert之后，页面上的checkbox是未选中的状态。我们将这个form提交到后台也是取不到值的。既然没选上，那我们在alert的后面加上这句：$(this).attr('checked',true);让它选上，刷新下，再点击，你会发现，在我们关闭alert之后，页面上的checkbox仍然是未选中的。问题就在这，我们通过程序选中了checkbox，为什么在响应函数执行完之后，checkbox还是未选中的状态呢？
　　为解释这个问题，我们需要修改上面的代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
window.onload=function(){
    $('#chk').on('click',function(e){
        e.preventDefault();
        $(this).attr('checked',true);
        alert($(this).attr('checked'));
    });
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
上面的代码我们先将设置check为true的语句放到alert前面，运行下看看，当我们点击checkbox时：
![](http://images.cnitblog.com/blog/371714/201304/08092017-a192258a078a4444b23d4a8729e76f5c.jpg)
　　可以看到，在执行完$(this).attr('checked',true);后到alert时，checkbox的状态是选中的，而且弹出的状态也是checked，也就是说，这个时候，checkbox是选中的。但是为什么我们在点击确定之后，checkbox又变回了未选中了呢？在相应事件结束之后，一定还有什么东西让它改回去了？其实，这是checkbox的默认click事件的做的。也许大家会问，我不是通过e.preventDefault()把默认事件去掉了吗？为什么还有默认事件？难道e.preventDefault()没有生效？
　　在这里需要说明下：一般的默认事件，会在我们绑定的响应事件**之后**执行（这样说不准确，默认事件和自己绑定的响应事件是在一起执行的，只不过，它的真正的作用一般会在所有事件内容做完之后才会体现出来，因此，在这就简单理解成在绑定的事件之后执行吧）。有了这个说明，我们再来说下这个现象。这不是浏览器的bug，也不是没有禁掉默认事件。由于checkbox是系统的组件，因此它的实现机理比较复杂，在这里不讨论，这个问题的主要原因是：**checkbox的click事件的默认事件是将修改的选中状态生效**。解释一下，如果在鼠标点击之前，checkbox的状态是选中，那么在点击checkbox时，它会将当前的状态取反即取消选中，然后执行响应事件，最后默认事件将这个状态生效。
　　上面的这个代码中，由于去掉了默认事件，因此，在执行响应事件时，checkbox确实选中了，alert出来的也是选中的，但是在事件结束时，由于没有没有默认事件，那么这个checkbox就回到了点击之前的状态。我们如果将input写成：<input type="checkbox" id="chk" checked />，即在点击之前就选中它，那么点击之后，checkbox就是选中的状态了。当然，这个选中状态不是因为$(this).attr('checked',true);造成的，而是没有默认事件，它又重置回之前的状态了。这里我们将代码改成$(this).attr('checked',false);在执行完之后，仍然会是选中状态的。
　　这个问题充分体现出了默认事件的重要性，我们之前去掉过很多默认事件，但是大部分的默认事件如我们想象的那样，去掉之后，基本功能就不存在了。但是checkbox的click默认事件不是让它没有反应，而是将修改后的状态生效（注意：仅仅是click事件，mousedown和mouseup事件是没有这个特点的）。因此，在我们不知道这个问题之前，如果禁掉了它的click事件，就会出现一些问题了。当然，这知识针对checkbox和radio而言。w3c的文档也证实了这一点，它的文档上有明确的说明：
Note: During the handling of a click event on an input element with a type attribute that has the value "radio" or "checkbox", some implementations may change the value of this property before the event is being dispatched in the document. If the default action
 of the event is canceled, the value of the property may be changed back to its original value. This means that the value of this property during the handling of click events is implementation dependent.
原文地址：http://www.w3.org/TR/DOM-Level-2-HTML/html.html#ID-6043025
