# javascript的事件机制 - 左直拳的马桶_日用桶 - CSDN博客
2016年11月17日 23:41:44[左直拳](https://me.csdn.net/leftfist)阅读数：1482
**一、事件模型**
IE 和 标准DOM的事件模型 
IE系：冒泡方式 
NETSCAPE系：捕获方式 
标准DOM：先捕获再冒泡
冒泡，从触发点向外层、顶层扩散，最后到达document、window，遇到相同注册事件立即触发执行； 
捕获则相反，从window、document向里收缩，一直到触发点，遇到相同注册事件立即触发执行；
有代码如下：
```xml
<style type="text/css">
div {
  border: solid 1px red;
}
#s1 {
  padding: 10px;
  border-color: blue;
}
</style>
<div id="s1">s1
  <div id="s2">s2</div>
</div>
<script type="text/javascript">
if(document.attachEvent){//IE浏览器
    document.getElementById("s1").attachEvent("onclick",function(e){
        alert("s1冒泡模式");
    });
    document.getElementById("s2").attachEvent("onclick",function(e){
        alert("s2冒泡模式");
    });
}
else{
    s1.addEventListener("click", function() {
      alert("s1捕获模式");
    }, true);
    s2.addEventListener("click", function() {
      alert("s2捕获模式");
    }, true);
    s1.addEventListener("click", function() {
      alert("s1冒泡模式");
    }, false);
    s2.addEventListener("click", function() {
      alert("s2冒泡模式");
    }, false);
}
</script>
```
页面如下： 
![这里写图片描述](https://img-blog.csdn.net/20161109224406638)
在非IE浏览器里，点击里层的s2，依次提示如下，清晰地体现了先捕获、后冒泡的特点：
```
s1捕获模式
s2捕获模式
s2冒泡模式
s1冒泡模式
```
如果是IE浏览器，则依次提示
```
s2冒泡模式
s1冒泡模式
```
**二、事件注册**
类似这样，
标准DOM
```java
s1.addEventListener("click", function() {
      alert("捕获模式");
    }, true);
s1.addEventListener("click", function() {
      alert("冒泡模式");
    }, false);
```
IE
```java
document.getElementById("s1").attachEvent("onclick",function(e){
        alert("IE下注册事件");
});
```
**三、阻断事件**
标准DOM下：
> 
event.stopPropagation //阻止事件的进一步传播，本节点除外  
  event.stopImmediatePropagation //立即阻断事件的传播，包括本节点  
  event.preventDefault() //阻止默认事件的发生。比如说，点击submit控件，提交就是默认事件。
IE下：
> 
window.event.cancelBubble = true; //阻止事件的进一步传播，本节点除外 
  window.event.returnValue = false;//阻止默认事件的发生
有代码如下：
```xml
<style type="text/css">
div {
  border: solid 1px red;
}
#s1 {
  padding: 10px;
  border-color: blue;
}
</style>
<form id="f1">
<div id="s1">s1
  <div id="s2">s2</div>
  <div><input type="submit" id="b1" value="submit" /></div>
</div>
</form>
<script type="text/javascript">
if(document.attachEvent){//IE浏览器
    document.getElementById("s1").attachEvent("onclick",function(e){
        alert("s1事件");
    });
    document.getElementById("s2").attachEvent("onclick",function(e){
        alert("s2事件");
    });
    document.getElementById("b1").attachEvent("onclick",function(e){
        alert("b1点击事件1");
        window.event.cancelBubble = true;//使s1事件无效；b1点击事件2不受影响，仍然执行（S2事件则与本次点击无关，始终不会被触发）
        window.event.returnValue = false;//使f1提交事件无效
    });
    document.getElementById("b1").attachEvent("onclick",function(e){
        alert("b1点击事件2");
    });
    document.getElementById("f1").attachEvent("onsubmit",function(e){
        alert("提交...");
    });
}
else{
    s1.addEventListener("click", function() {
      alert("s1事件");
    }, false);
    s2.addEventListener("click", function() {
      alert("s2事件");
    }, false);
    b1.addEventListener("click", function(evt) {
      alert("b1点击事件1");
      evt.stopPropagation();//使s1事件无效；b1点击事件2不受影响，仍然执行（S2事件则与本次点击无关，始终不会被触发）
      evt.stopImmediatePropagation();//使b1点击事件2，s1事件无效；
      evt.preventDefault();//使f1提交事件无效
    }, false);
    b1.addEventListener("click", function(evt) {
      alert("b1点击事件2");
    }, false);
    f1.addEventListener("submit", function() {
      alert("提交...");
    }, false);
}
</script>
```
![这里写图片描述](https://img-blog.csdn.net/20161117233204598)
参考资料： 
[[解惑]JavaScript事件机制](http://www.cnblogs.com/hustskyking/p/problem-javascript-event.html)
