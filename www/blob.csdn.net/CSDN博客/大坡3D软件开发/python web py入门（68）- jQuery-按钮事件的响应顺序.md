
# python web py入门（68）- jQuery - 按钮事件的响应顺序 - 大坡3D软件开发 - CSDN博客

2018年05月06日 10:00:55[caimouse](https://me.csdn.net/caimouse)阅读数：704


在前面学习了按钮响应函数判断事件的来源，但是js里有一个值得注意的事件响应顺序问题。由于HTML的元素是树形结构排列，导致事件也是按树形发生的，也就是说树叶节点的事件会传送给父接点，依次传递上去，也就是逐级上传。比如HTML文档如下：
```python
<!DOCTYPE html>
<html>
<head>
<title>Event Bubbling Example</title>
</head>
<body>
<div id="myDiv">Click Me</div>
</body>
</html>
```
如果你单击了页面中的<div>元素，那么这个click 事件会按照如下顺序传播：

(1) <div>
(2) <body>
(3) <html>
(4) document

也就是说，click 事件首先在<div>元素上发生，而这个元素就是我们单击的元素。然后，click事件沿DOM树向上传播，在每一级节点上都会发生，直至传播到document 对象。如下图所示：
![](https://img-blog.csdn.net/20180506095803353)
由于这种特性，叫做事件冒泡（event bubbling）。因而在处理元素的事件响应时，就需要格外小心了，比如你只想点击body元素时才去改变页面的颜色，如果你不小心处理，就会发现点击body里任何元素都会触发这个事件响应。下面来通过代码来演示一下，如下：
```python
<html>
  <head>
    <meta charset="utf-8">
    <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
  </head>
  <script>
    $(document).ready (function() {
      $("body").click(function(event) {
        alert("body click");
        console.log(event.target.nodeName);
        console.log(this.nodeName);
      });
      $("span").click(function(event) {
        alert("span click");
      });
      $("#msg").click(function(event) {
        alert("div click");
        //event.stopPropagation();
      });
    });
  </script>
  <body>
    <span>
      <div id="msg">测试事件的冒泡顺序</div>
    </span>
  </body>
</html>
```
当你点击字符串时，发现依次弹出"div click"，"span click"，"body click"的窗口显示。
既然有这种特性，那么有没有办法来区分这个事件是点击本身发生的，还是来源于子元素发生的？方法是有的，就是使用点击函数的event对象，而不是使用this对象。下面可以看到这两者的输出的区别：
![](https://img-blog.csdn.net/20180506100011517)
另外，如果不想子元素的事件进行冒泡传递，可以在子函数里添加stopPropagation方法，如下：
event.stopPropagation();
这样这个事件就不会向父元素进行扩散传播了。

### [http://edu.csdn.net/course/detail/2570](http://edu.csdn.net/course/detail/2570)
### 在VC2015里使用protobuf协议
### [http://edu.csdn.net/course/detail/2582](http://edu.csdn.net/course/detail/2582)
### 在VC2015里学会使用MySQL数据库
### [http://edu.csdn.net/course/detail/2672](http://edu.csdn.net/course/detail/2672)


