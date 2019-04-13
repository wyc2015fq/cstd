
# python web py入门（65）- jQuery - 根据鼠标位置来选择不同的样式显示 - 大坡3D软件开发 - CSDN博客

2018年05月01日 14:57:30[caimouse](https://me.csdn.net/caimouse)阅读数：403


在前面学习了窗口滚动事件的响应，现在来学习根据鼠标在不同的位置选择不同的样式显示，这样可以动态地区分鼠标所在区域，也起到提醒的作用。通过例子可以学习到怎么样使用jQuery来操作CSS，以及鼠标的hover() 方法，这个例子的代码如下：
```python
<html>
  <head>
    <meta charset="utf-8">
    <title>文章显示系统</title>
     <link rel="stylesheet" href="page/page.css" type="text/css" />
     <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
     <script>
       $(document).ready(function(){
         $('#switcher').hover(
           function(){ $(this).addClass('hover');    },
           function(){ $(this).removeClass('hover'); }
         );
       });
     </script>
  </head>
  <body>
    <div id="container">
      <div id="switcher" class="switcher">
        <h3>页面布局选择</h3>
        <button id="switcher-default">
          默认
        </button>
        <button id="switcher-narrow">
          一般显示
        </button>
        <button id="switcher-large">
          大号显示
        </button>
      </div>
    </div>
    <span></span>
  </body>
</html>
```

在这个例子里，在头部元素里加载css的page/page.css，这样就决定页面显示颜色和字体的大小，以及字体的对齐方式。接着加载jquery库，运行编写的js代码，这段代码如下：
$(document).ready(function(){
$('\#switcher').hover(
function(){ $(this).addClass('hover');    },
function(){ $(this).removeClass('hover'); }
);
});
在这里使用了方法：
hover() 方法规定当鼠标指针悬停在被选元素上时要运行的两个函数。
方法触发 mouseenter 和 mouseleave 事件。
注意: 如果只指定一个函数，则 mouseenter 和 mouseleave 都执行它。
这个函数第一个参数是响应鼠标进入的事件，第二个参数是响应鼠标离开的事件。
注意这里使用 $(this)，它是this这个字符串对应的标签对象，也就是指向ID为switcher的div分区。
同时在这里添加样式的函数addClass和removeClass：
addClass() 方法向被选元素添加一个或多个类名。
该方法不会移除已存在的 class 属性，仅仅添加一个或多个类名到 class 属性。
提示：如需添加多个类，请使用空格分隔类名。
removeClass() 方法从被选元素移除一个或多个类。
注意：如果没有规定参数，则该方法将从被选元素中删除所有类。
这两个函数就是实现动态地增加和删除样式，那么页面就会动态地显示不一样的颜色、字体等等。
运行的结果如下：
![](https://img-blog.csdn.net/20180501145535146)
![](https://img-blog.csdn.net/20180501145548763)
page.css的代码如下：
```python
/***************************************
   Default Styles
************************************** */
html, body {
  margin: 0;
  padding: 0;
}
body {
  font: Arial,"Microsoft YaHei","黑体","宋体",sans-serif;
  color: #000;
  background: #fff;
}
#container {
  font-size: 1.2em;
  margin: 10px 2em;
}
h1 {
  font-size: 2.5em;
  margin-bottom: 0;
}
h2 {
  font-size: 1.3em;
  margin-bottom: .5em;
}
h3 {
  font-size: 1.1em;
  margin-bottom: 0;
}
code {
  font-size: 1.2em;
}
a {
  color: #06581f;
}

/***************************************
   page Styles
************************************** */
.poem {
  margin: 0 5em;
}
.chapter {
  margin: 1em;
}
.switcher {
  float: right;
  background-color: #c0c0c0;
  border: 1px solid #000000;
  margin: 10px;
  padding: 10px;
  font-size: .9em;
}
.switcher h3 {
  margin: .5em 0;
}
#header {
  clear: both;
}
body.large .chapter {
  font-size: 1.5em;
}
body.narrow .chapter {
  width: 250px;
}
.selected {
  font-weight: bold;
}
.hidden {
  display: none;
}
.hover {
  cursor: pointer;
  background-color: #20b2aa;
}
```

### TensorFlow入门基本教程
### [http://edu.csdn.net/course/detail/4369](http://edu.csdn.net/course/detail/4369)
### C++标准模板库从入门到精通
### [http://edu.csdn.net/course/detail/3324](http://edu.csdn.net/course/detail/3324)
### 跟老菜鸟学C++
### [http://edu.csdn.net/course/detail/2901](http://edu.csdn.net/course/detail/2901)





