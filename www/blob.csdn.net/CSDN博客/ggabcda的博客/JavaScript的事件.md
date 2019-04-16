# JavaScript的事件 - ggabcda的博客 - CSDN博客





2017年08月25日 12:37:20[ggabcda](https://me.csdn.net/ggabcda)阅读数：107








# 一、事件绑定

要想让 JavaScript 对用户的操作作出响应，首先要对 DOM 元素绑定事件处理函数。所谓事件处理函数，就是处理用户操作的函数，不同的操作对应不同的名称。它是事件驱动编程模式的主要编程方式

在javascript中，有三种常用的绑定事件的方法：
- 在DOM元素中直接绑定； 
- Element节点的事件属性； //DOM0级事件处理程序 
- 绑定事件监听函； //DOM2级事件处理程序 

## 在DOM元素中直接绑定(HTML的on-属性)

在HTML中，我们可以在DOM元素上绑定onclick、onmouseover、onmouseout、onmousedown、onmouseup、ondblclick、onkeydown、onkeypress、onkeyup等

```
<div onclick="test()">

<script>
fucntion test(){
    alert("Hello World");
}
</script>
```

使用这个方法，只会在冒泡阶段触发。

使用这种方法时，on-属性的值是将会执行的代码，而不是一个函数。

## Element节点的事件属性（在JavaScript代码中绑定事件）

在JavaScript代码中（即script标签内）绑定事件可以使JavaScript代码与HTML标签分离，文档结构清晰，便于管理和开发。

```
<div id="test">

<script>
var div = document.getElementById("test");
div.onclick = function(){
  console.log('Hello World');
};
</script>
```

使用这个方法，只会在冒泡阶段触发。

## 使用事件监听绑定事件（addEventListener方法）

### 3个事件阶段

关于事件监听，W3C规范中定义了3个事件阶段，依次是捕获阶段、目标阶段、冒泡阶段。
- 第一阶段：从window对象传导到目标节点，称为“捕获阶段”（capture phase）。 
- 第二阶段：在目标节点上触发，称为“目标阶段”（target phase）。 
- 第三阶段：从目标节点传导回window对象，称为“冒泡阶段”（bubbling phase）。 

### EventTarget接口

DOM的事件操作（监听和触发），都定义在EventTarget接口。Element节点、document节点和window对象，都部署了这个接口。

![](http://i.imgur.com/gXIIpip.png)

该接口定义了三个方法：
- addEventListener：绑定事件的监听函数 
- removeEventListener：移除事件的监听函数 
- dispatchEvent：触发事件 

#### addEventListener：

用于在当前节点或对象上，定义一个特定事件的监听函数。（同一节点可以绑定多个事件）

```
element.addEventListener(event, function, useCapture)
```
- event : （必需）事件名，支持所有 DOM事件 。 
- function：（必需）指定要事件触发时执行的**函数**。 
- 
useCapture：（可选）指定事件是否在捕获或冒泡阶段执行。true，捕获。false，冒泡。默认false。

```
function test() {
  console.log('Hello world');
}

var div = document.getElementById('test');
div.addEventListener('click', test, false);
```


#### removeEventListener：

用来移除addEventListener方法添加的事件监听函数。（可以解除相应的绑定）

```
div.addEventListener('click', listener, false);
div.removeEventListener('click', listener, false);
```

#### dispatchEvent:

在当前节点上触发指定事件，从而触发监听函数的执行。(后面会细讲)

# 二、事件委托

事件委托就是利用冒泡的原理，把事件加到父元素或祖先元素上，触发执行效果。

1.可提高JavaScript性能

```
<ul id="list">
  <li id="item1" >item1</li>
 <li id="item2" >item2</li>
 <li id="item3" >item3</li>
</ul>

<script>
var item1 = document.getElementById("item1");
var item2 = document.getElementById("item2");
var item3 = document.getElementById("item3");

document.addEventListener("click",function(event){
 var target = event.target;
 if(target == item1){
  alert("hello item1");
 }else if(target == item2){
  alert("hello item2");
 }else if(target == item3){
  alert("hello item3");
 }
})
</script>
```

2.事件委托可用于对动态添加的元素动态的添加事件：

```
<ul id="list">
 <li id="item1" >item1</li>
 <li id="item2" >item2</li>
 <li id="item3" >item3</li>
</ul>

<script>
var list = document.getElementById("list");

document.addEventListener("click",function(event){
 var target = event.target;
 if(target.nodeName == "LI"){
  alert(target.innerHTML);
 }
})

var node = document.createElement("li");
node.id = "item4";
var textnode = document.createTextNode("item4");
node.appendChild(textnode);
list.appendChild(node);
</script>
```

# 三、自定义事件

通常情况下，事件的触发都是由用户的行为如点击、刷新等操作实现，但是，其实有的情况下，事件的触发必须又程序来实现。

dispatchEvent是作为高级浏览器(如chrome、Firfox等)的事件触发器来使用的。

```
document.createEvent();// 创建
event.initEvent();// 初始化
element.dispatchEvent();// 触发
```

createEvent参数：HTMLEvents，MouseEvents，UIEvents

initEvent(eventName, canBubble, preventDefault)。分别表示事件名称，是否可以冒泡，是否阻止事件的默认操作。

dispatchEvent()就是触发执行了，dom.dispatchEvent(eventObject), 参数eventObject表示事件对象，是createEvent()方法返回的创建的Event对象。

```
例子：
$(dom).addEvent("alert", function() {
    alert("Holle World");
});

var evt = document.createEvent("HTMLEvents");
evt.initEvent("alert", false, false);
dom.dispatchEvent(evt);
```



