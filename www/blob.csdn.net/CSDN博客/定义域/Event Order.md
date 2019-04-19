# Event Order - 定义域 - CSDN博客
2016年02月22日 15:18:55[定义域](https://me.csdn.net/u010278882)阅读数：297
# Event Order
> 
FROM: [http://quirksmode.org/js/events_order.html](http://quirksmode.org/js/events_order.html)
  AUTHOR: Peter-Paul Koch
在[事件介绍](http://quirksmode.org/js/introevents.html)那一节我问过一个看似让人无法理解的问题：“如果元素和他的祖先对同一个事件都有事件处理器，那么哪一个先被执行呢？”答案一点也令人吃惊，没错，依浏览器而定。
基本问题非常简单。假设你有这样一个元素，其内部还有一个元素
![示例](https://img-blog.csdn.net/20160222152316810)
并且两个元素都一个onclick事件的处理器。如果用户单击 element2，那么会在 element1 和 element2 上都引发一个单击事件。但是哪个事件先被触发呢? 哪个事件处理器先被执行呢? 换句话说，也就是事件顺序是什么呢?
## 两种模型
意料之内，过去 Netscape 和 Microsoft 给出了不同的答案。
- Netscape 说 element1 元素上的事件先发生。这叫做事件捕获。
- 微软认为 element2 元素上的事件先发生。这叫做事件冒泡。
这两种模型完全相反，Explorer 仅支持事件冒泡。Mozilla，Opera 7 和 Konqueror 对两种模型都支持。早期的 Opera 和 iCab 对两种模型均不支持。
### 事件捕获
当你使用事件捕获时
![Event Capturing](https://img-blog.csdn.net/20160222152659177)
element1 的事件处理器最先被调用，然后 element2 的事件处理器随后被调用。
### 事件冒泡
当你使用事件冒泡时
![Event Bubbling](https://img-blog.csdn.net/20160222152714474)
element2 元素的事件处理器最先被调用，element1 的事件处理器随后被调用。
## W3C 模型
W3C非常明显，决定处于中立位置。任何发生在 W3C 事件模型中的事件首先被捕获，直到事件抵达目标元素，然后再冒泡。
![W3C Model](https://img-blog.csdn.net/20160222152829437)
作为Web开发者的你可以决定是否在捕获阶段或者冒泡阶段注册事件处理器。你的决定通过 addEventListner() 方法来指定（会在 [高级模型](http://quirksmode.org/js/events_advanced.html) 页面给予介绍）。如果该方法的最后一个参数是 true，事件处理器就将用于事件捕获阶段，如果是 false, 则事件处理器其就用于冒泡阶段。
假设你添加了如下的监听器
```java
element1.addEventListener('click', doSomething2, true)
eleemnt2.addEventListener('click', doSomething, false)
```
如果用户点击了 element2 元素将发生下面的情况
- click 事件从捕获阶段开始。事件将查看 element2 元素是否有任何的祖先元素注册了在捕获阶段的 onclick 事件处理器。
- 事件发现 element1 元素在捕获阶段注册了 onclick 事件处理器，所以 doSomething2 被执行。
- 事件将向下遍历知道目标对象本身，这里并没有找到任何的其他事件处理器。事件转到冒泡阶段，然后执行 doSomething（element2 注册的冒泡阶段事件处理）。
- 事件将向上遍历，检查在冒泡阶段，其父级元素是否有任何的事件处理器。这个示例中并没有，所以没有方法被执行。
相反的情况：
```java
element1.addEventListener('click', doSomething2, false);
element2.addEventListener('click', doSomehitng, false);
```
这时，当用户点击 element2 元素时，将发生如下的情况：
- click 事件从冒泡阶段开始。事件查找 element2 是否有任何的父级元素在捕获阶段注册了 onclick 事件处理器，这里并没有找到。
- 事件向下遍历到目标事件本身。事件转到冒泡阶段，执行 doSomething（element2注册的在冒泡阶段的事件处理器）。
- 事件向上遍历，检查是否有父级元素注册了在冒泡阶段的事件处理器。
- 在 element1 元素上找到一个，doSomething2 被执行。
### 传统模型的兼容性
在支持W3C DOM 这个传统事件注册模型的浏览器中
```java
element1.onclick = doSomething2;
```
被认为是在冒泡阶段进行的注册。
## 事件冒泡的使用
很少有Web开发者会明确使用事件的捕获或冒泡。在今天的 Web 页面中，通常不会让一个冒泡的事件被多个不同的事件处理器处理。在鼠标点击后，如果有多个事件发生，用户可能会感到困惑，所以通常开发者会让事件处理的代码分离开。当用户点击一个元素是，发生一些事，当用户点击另一个元素时，发生另一件事。
当然这种情况可能在未来发生改变，好的地方是现在我们又可以兼容未来需求的模型。但是今天对事件捕获和冒泡的主要实践是注册默认处理函数。
## 总是发生
你至少要知道事件捕获或者事件冒泡总是在发生。如果你在整个文档定义了一个一般性用途的onclick 事件处理器
```java
document.onclick = doSomething;
if(document.captureEvents) document.captureEvents(Event.CLICK);
```
那么在文档中的任何元素上的任何点击时间将冒泡到 document，然后引发这个一般用途的事件处理器的执行。只有当之前的事件处理器脚本明确要求时间应该停止冒泡，否则事件会传播给 document。
## 使用
因为任何事件的传播都会在 document 终止，这就为使用默认事件处理器提供了可能。假设你有一个页面，具有下面这种结构
![Demo](https://img-blog.csdn.net/20160222153055229)
```java
element1.onclick = doSomething;
element2.onclick = doSomething;
document.onclick = defaultFunction;
```
现在如果用户点击 element1 或 element2，那么 doSomething 将被执行。如果你希望的话，可以在这里终止事件的传播。如果用户单击除了 element1 或 2 外的任何位置，那么 defaultFunction 将会被执行。这点在某些情况系可能非常有用。
设置文档范围内的事件处理器在需要实现拖拽时非常有用。通常情况下，我们会这么做：让一个浮层的 mousedown 事件选择这个浮层，让后在 mousemove 事件中对拖拽予以响应。虽然 mousedown 通常注册在浮层上以便面浏览器的 bug，但是拖拽的事件处理器比如注册在文档范围内。
要记住“浏览器科学”的第一法则就是：任何可能都可能发生，并且如果你没有为某种可能做应对，那么通常那种可能就很有可能发生。比如可能发生这种情况，用户在很大范围内移动鼠标，如果脚本没有注册文档范文的事件处理器，那么鼠标将会丢失掉浮层的跟随。
- 如果 onmouseover 事假处理器注册在浮层上，那么浮层将不会对任何的鼠标移动作出响应。
- 如果 onmouseup 事件处理器注册在浮层上，那么事件也不会发生，这样浮层将一直对鼠标的移动作为响应，计时用户并不想拖拽该图层。这回引起用户更多的困惑。
所以在拖拽这个例子中，事件冒泡是非常有用的。因为在文档范围级别注册事件处理器会让事件处理器总能被执行。
## 关闭它
但是，通常你想关闭所有的捕获和冒泡，以让所有的元素的事件处理函数互相独立。此外，如果文档结构非常负责的话（比如许多内嵌表格），为了节省系统资源，也会关闭冒泡。因为浏览器得遍历事件目标的全有祖先元素，已确定是否有事件处理器。即便是没有事件处理器你，搜索过程仍然需要消耗时间。
微软模型中要关闭冒泡需要设置事件的 cancelBubble 属性值为 true。
```java
window.event.cancleBubble = true;
```
W3C模型中关闭冒泡需要调用事件的 stopPropagation() 方法。
```java
e.stopPropapagtion();
```
该方法将会终止事件在冒泡阶段的传播。要实现一个跨浏览器的关闭冒泡操作，可以这样做
```java
function doSomething(e) {
    if(!e) var e = window.event;
    e.cancelBubble = true;
    if(e.stopPropagation) e.stopPropagation();
}
```
为不支持 cancelBubble 属性的浏览器设置属性实际上并不会导致什么问题。浏览器会创建该属性。虽然创建但不会取消冒泡，但是赋值是安全的。
## 事件的 currentTarget 属性
之前介绍过，事件对象具有 target 或 srcElement 属性，其包含对所发生事件的元素的引用。在上面的例子中，就是 element2。
非常重要的一点是，要知道在捕获或冒泡阶段事件目标并不会发生改变，比如这里一直是 element2。
假设我们注册了如下的事件处理器
```java
element1.onclick = doSomething
element2.onclick = doSomething
```
若用户点击 element2 则 doSomething 会被执行两次。但是怎样才能知道当前的被处理的事件是哪一个HTML元素的呢? 从 target/srcElement 中并不能得知，因为他们总是引用 element2 元素。
为了解决该问题，W3C 为事件对象添加了一个名为 currentTareget 的属性。它包含对当前正在处理事件的HTML元素的引用。很不幸，微软模型并不包含类似的属性。
此外，还可以通过使用 this 关键字。上面的例子中 this 引用的就是当前正在处理事件的HTML元素，就像 currentTareget一样。不过微软也不支持。
## 微软模型的一些问题
不过，微软模型中 this 关键字并不引用HTML元素。也就是说
```java
element1.attachEvent('onclick', doSomething)
element2.attachEvent('onclick', doSomething)
```
中你并不能知道当前处理事件的是哪一个HTML元素。这点是使用微软事件注册模型最严重的额问题。 
我希望微软能够尽快添加一个 currentTarget —— 就像属性 —— 或者遵照规范? Web 开发者需要这个信息。
