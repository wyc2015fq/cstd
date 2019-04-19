# Event Accessing - 定义域 - CSDN博客
2016年02月22日 10:53:52[定义域](https://me.csdn.net/u010278882)阅读数：138标签：[事件访问模型](https://so.csdn.net/so/search/s.do?q=事件访问模型&t=blog)
个人分类：[片段](https://blog.csdn.net/u010278882/article/category/5886909)
# Event Accessing
> 
FROM: [http://quirksmode.org/js/events_access.html](http://quirksmode.org/js/events_access.html)
  AUTHOR: Peter-Paul Koch
我们注册了事件处理函数后，现在我们想要知道事件的信息。我们想要知道发生事件时的鼠标位置，我们想知道用户任意时刻按下的按键。这些信息我们都能获取。与此同时，你要知道事件处理相关的内容是浏览器兼容性最糟糕的地方。（想要概览一下事件处理的兼容性，查看[事件兼容性表格](http://quirksmode.org/js/events_compinfo.html)）。
## 浏览器兼容性
在浏览器大战最激烈的时候，Netscape发明了一种访问模型（后来被W3C抄袭）和相关的许多事件属性，同时 Microsoft 也发明了一种访问模型及相关的许多事件属性。当然，这两种模型完全不兼容。但是，正如我们在[事件简介](http://quirksmode.org/js/introevents.html)那一节所说的，向下面这样的按照浏览器来区分对待的分支代码
```java
if(W3C/Netscape) {
    // 使用 W3C/Netscape 模型来访问事件和事件属性
} else if(Explorer) {
    // 使用 Microsoft 模型来访问事件和事件属性s
}
```
并不是正确的解决浏览器兼容性问题的方法，因为这种方法可能会漏掉一些小众浏览器。因此我们得先访问事件，然后分别根据模型读取模型相关的属性。
下面让我开始访问事件吧，事件属性的访问我们会在[事件属性](http://quirksmode.org/js/events_properties.html)那一节来介绍。
## W3C/Netscape
在 W3C/Netscape 事件访问模型中，事件以事件处理器的参数被传入。所以，如果定义了一个下面这样的事件处理器
```java
element.onclick = doSomething;
```
该事件处理器会接收一个事件对象作为参数。按照惯例我们通常将传入的事件对象命名为 e，当然你也可以使用你喜欢的名称，比如
```java
function doSomething(e) {
    // e 用来访问事件
}
```
事件的传入是自动完成的，不需要任何其他代码。匿名函数中可以这样获取
```java
element.onclick = function(e) { alert('Event type is ' + e.type); };
```
## Microsoft
微软的事件访问模型中，有一个特殊的属性 window.event，它包含最后发生的事件。
```java
element.onclick = doSomething;
function doSomething() {
    // 通过 window.event 来访问事件
}
```
或者
```java
element.onclick = function() { alert('Event type is ' + window.event.type) };
```
### Event与event
需要注意一点，在旧版的 Netscape 浏览器中，存在一个 window.Event 属性。Explorer 不能理解而 Netscape 4 会错误的解释它。因此确保总是使用带有小写字母 e 开头的 event。
## 跨浏览器事件访问
幸运的是，编写一个跨浏览器的脚本很容易
```java
element.onclick = doSomething;
function doSomething(e) {
    if(!e) var e = window.evnet;
    // 这里的 e 在所有的浏览器中都能正确访问
}
```
如果变量 e 不存在（如果没有传入给函数），就让它引用 window.event。现在 e 就能引用所有浏览器中的事件。
## 结合使用行内事件处理器
在行内注册模型中，你得将 event 传递给函数
```java
<pre onclick="doSomething(event)">
function doSomething(e) {
    alert(e.type);
}
```
不管在哪个浏览器中 (window.) event 在微软模型中总是正确的属性，而其他浏览器也能够识别。
