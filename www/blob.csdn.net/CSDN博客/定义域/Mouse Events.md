# Mouse Events - 定义域 - CSDN博客
2016年02月22日 18:38:56[定义域](https://me.csdn.net/u010278882)阅读数：272标签：[事件](https://so.csdn.net/so/search/s.do?q=事件&t=blog)
个人分类：[片段](https://blog.csdn.net/u010278882/article/category/5886909)
# Mouse Events
> 
FROM: [http://quirksmode.org/js/events_mouse.html](http://quirksmode.org/js/events_mouse.html)
  AUTHOR: Peter-Paul Koch
鼠标事件是最重要的事件类型。本小节介绍最常用的问题的解决方案和技巧。
依次介绍所有的鼠标事件，包括 mousedownl, mouseup 和 click，dbclick，mousemove，还有 mouseover 和 mouseout。然后解释事件的 relatedTarget、fromElement 和 toElement 属性。最后介绍微软的私有事件类型 mouseventer 和 mouseleave。
兼容性的详细情况，请查看[事件兼容表](http://quirksmode.org/js/events_compinfo.html)。
## 示例
[示例](http://quirksmode.org/js/events_mouse.html#link1)
示例中带有下划线的文本注册了mousedown、mouseup、click和dbclick事件的处理器，点击下划线后，相应的事件发生的顺序将输出到下划线下面的文本框内。
> 
操作和输出如下
- 按下鼠标：mousedown
- 松开鼠标：先是 mouseup，然后是 click
- 单击鼠标：mousedown - mouseup - click
- 
双击鼠标：mousedown - mouseup - click - mousedown - mouseup - click - dbclick
- 
在文本上按下鼠标，然后移开释放：mousedown
- 
在非文本上按下鼠标，然后移到文本上释放：mouseup
- 
选中复选框，使用弹窗提示事件
- 按下鼠标、单击、双击：mousedown
## mousedown、mouseup、click
如果用户在一个元素上点击的话，将发生如下的事件（按照如下的顺序）
- mousedown 用户在元素上按下鼠标按钮
- mouseup 用户在元素上释放鼠标按钮
- click 在一个元素上检测到依次 mousedown 和 mouseup
通常 mousedown 和 mouseup 要比 click 要更加有用。因为有些浏览器不允许你读取 onclick 事件中鼠标按钮的信息。而且，用户用鼠标进行的操作并不会引发 click 事件发生。
假设用户用鼠标按下一个链接，然后移动鼠标离开链接，然后释放鼠标按钮。那么仅会出发 mousedown 事件。类似的，如果用户按下鼠标按钮，然后移动鼠标经过一个链接，然后在链接上释放鼠标，那么仅会出发链接的 mouseup 事件。这两种情况都没有引发 click 事件。
这种事件引发行为是不是一个问题，要根据你所希望的用户交互而定。但是通常应该注册 mousedonw/up 事件，除非你完全确定你想要用户点触发，否则不要用 click。
如果你想用提示框（alert）来处理鼠标事件，那么会出现一种非预期的情况：浏览器没有跟踪事件的发生顺序。所以，最好不要使用提示框。
## dbclick
dbclick 事件很少使用。使用该事假的时候，不要同时为一个HTML元素注册 onclick 和 ondbclick 事件处理器。这种注册方式很难获取用户究竟进行了什么操作。
毕竟，当用户双击一个元素是，click 事件会先与 dbclick 事件发生。此外，Netscape 的第二次 click 事件也会在 dbclick 事件前处理（就是说，第二次的click会被处理）。最后一点，提示框在这种情况下也是非常危险的。
所以click和dbclick分开使用，避免组合使用。
## mousemove
mousemove事件能够很好的工作，但是要注意其会占用一些系统时间用来处理器所有的鼠标移动事件。如果用户移动鼠标一个像素，mousemove 事件就会发生。即便没有什么真正的事情发生，长时间的移动和复杂的函数处理也会消耗时间，这会导致站点的可用性问题：一切都变得缓慢。在老式计算机上尤为明显。
因此，仅在你需要它的时候注册，并在用完后尽量移除：
```java
element.onmousemove = doSomething;
// later
element.onmousemove = null;
```
## mouseover and mouseout
看一下这里[示例](http://quirksmode.org/js/eventexample.html)，切换到 mousestuff。这里仅为ev3添加一个了 onmouseover 事件处理器。然而，你会注意到 mouveover 事件，不仅在鼠标移入 ev3 区域时发生，而且在鼠标进入 ev4 或者 span 的区域时也会发生。在 Mozilla 中，在版本 1.3 之前，mouseover 事件仅在鼠标移入到文本的区域时才会发生。
当然了，出现这个现象的原因就是事件冒泡。当用户进入 ev4 时也引发了一个 mouseover 事件。而该元素没有事件处理器，但是在其容器的 ev3 中有处理器。只要事件冒泡到该元素(‘ev3’)，事件处理器就会被执行。
虽然现在的这里的事件处理器的注册是正确的（为 ev3 注册 onmouseover事件处理器），但是这给我们带来了一些问题。我们的第一个问题就是目标是什么。假设鼠标移入 ev4：
![这里写图片描述](https://img-blog.csdn.net/20160222183731313)
鼠标从ev3移入ev4，那么这时的事件的 target/srcElement 就是 ev4：事件所发生的元素，因为用户的鼠标移入到 ev4.但是当下面的情况发生时：
![这里写图片描述](https://img-blog.csdn.net/20160222183746220)
鼠标从span移入到ev4。这里事件的target/srcElemenet与上一个事件完全相同。你可能想在鼠标从 ev3 移入时做一件事情，在鼠标从 span 移入时做另一件事情。所以，我们需要知道鼠标从哪儿移入的。
## relatedTarget、fromElement、toElement
W3C为 mouseover 和 mouseout 事件添加了 relatedTarget 属性。其包含鼠标在 mouseover 中从哪里如意，或者在 mouseout 事件中从移到哪里去。
微软创建了两个相应的属性：
- fromElement 引用一个元素，用于表示鼠标从哪儿来。该属性在 mouseover 事件中很有用。
- toElement 引用一个元素，用于表示鼠标移到哪里去。在 mouseout 事件中很有用。
在我们的例子中，relatedTarget/fromElement 引用 ev3，在第二个例子中引用的是 span。现在通过这个属性就能知道鼠标从哪里移入的。
## 跨浏览器兼容
所以如果你想知道鼠标从哪里移入，可以这样
```java
function doSomething(e) {
    if(!e) var e = window.event;
    var relTarg = e.relatedTarget || e.fromElement; 
}
```
如果你想知道 mouseout 事件中鼠标从移到哪里，可以这样
```java
function doSomething(e) {
    if(!e) var e = window.event;
    var relTarg = e.relatedTarget || e.toElement;
}
```
## 从浮层移出
在基于层的导航中，你可能需要知道鼠标何时离开浮层，这样你就可以关闭浮层。因此你对该元素注册了一个 onmouseout 事件处理器。然而，由于冒泡机制，当鼠标从该浮层内的任何一个元素移出时都会引发事件。
![这里写图片描述](https://img-blog.csdn.net/20160222183801293)
另一个show stopper 是当你移入浮层然后移到一个链接上时，浏览器会在浮层上引发一个mouseout事件。对我来说感到很奇怪（因为鼠标还在浮层上呢），但是所有的浏览器都这么做。
那么，我们如何取消不是真正的离开浮层的事件呢?
```java
function doSomething(e) {
    if(!e) var e = window.event;
    var tg = (window.event) ? e.srcElement : e.target;
    if(tg.nodeName != 'DIV') return;
    var reltg = (e.relatedTarget) ? e.relatedTarget : e.toElement;
    while(reltg != tg && reltg.nodeName != 'BODY')
        reltg = reltg.parentNode;
    if(reltg == tg) return;
    // 当鼠标真正的从右侧移出时mouseout 发生
    // 处理事件
}
```
首选获取事件目标（target），也就是鼠标移出的元素。如果事件目标不是一个DIV（浮层），那就终止事件处理，因为鼠标没有真正的离开浮层。
如果目标是一个浮层的话，我们还是不能确定鼠标是否离开浮层或者进入浮层内的链接。因此，我们将检查事件的 relatedTarget/toElement 属性（元素移到的元素）。
我们读取了这个元素，然后我们将向上遍历DOM树直到我们遇到了事件目标(target)或者body元素。
如果这个target是从浮层的子元素移入产生的，那么鼠标就没有离开浮层。所以终止处理。
只有排除了上面的种种情况，我们就能确定鼠标真正的离开了浮层，然后我们就能采取相应的处理（通常让浮层不可见）。
## mouseenter and mouseleave
微软还提供了另一种解决方案。提供了两个新的事件类型 mouseenter 和 mouseleave。这两个事件与 mouseover 和 mouseout 几乎一模一样，除了他们不对事件冒泡响应。因此在这两种事件中将注册事件的整个HTML元素看做一个块，并且不对 mouseover 和内部的 mouseout 响应。
所以使用这些事件也能解决问题：他们仅对注册该事件的元素的 mouseover 和 mouseout 事件响应。
此刻这些事件仅支持Windows 上的 Explorer 5.5以及更高版本。可能其他浏览器其引擎也支持这些事件。
## 结束
你现在读完了我的全部事件介绍文章。祝你在事件处理的脚本编写中好运。
