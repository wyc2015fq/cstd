
# python web py入门（67）- jQuery - 按钮的事件里获取按钮的名称 - 大坡3D软件开发 - CSDN博客

2018年05月06日 09:10:11[caimouse](https://me.csdn.net/caimouse)阅读数：1180


在前面学习了隐藏显示，以便根据需要来显示页面的内容，可以看到页面上有三个按钮，当按下这三个按钮时，就可以处理为三种不同的CSS样式，从而显示内容不一样。当我们把三个按钮的事件作响应时，是绑定三个响应事件的函数，还是只绑定一个函数呢？这里选择只绑定一个事件响应函数，那么这个函数只有一个，怎么样来区分三个不同的按钮呢？如果熟悉响应的函数，可以看到它长得这个样子：
$('\#switcher').click(function(event) {
在这个点击函数click里，可以看到传递了一个函数对象，这个对象有一个参数event，因此不同的按钮的区分，就要从这个event参数来作文章。可以从文档里看到event事件对象，一般有下面公共属性：
**target  返回最初触发事件的DOM元素。**
**relatedTarget  用于返回当前事件涉及到的其他DOM元素，如果有的话。**
**pageX 获取鼠标当前相对于页面的坐标，可以确定元素在当前页面的坐标值，是以页面为参考点,不随滑动条移动而变化**
**pageY 获取鼠标当前相对于页面的坐标，可以确定元素在当前页面的坐标值，是以页面为参考点,不随滑动条移动而变化**
**which 获取在鼠标单击事件中鼠标的左、中、右键（左键1，中间键2，右键3），在键盘事件中键盘的键码值**
**metaKey  用于指示事件触发时是否按下了Meta键。**

从上面属性里，可以看到target，就可以说明点击的来源，因此通过个属性就可以获取到哪一个按钮被点击的。当然，为了确认是那一个更具体的按钮，就得获取按钮的名称了，从前面的HTML里可以看到它们定义如下：
<button id="switcher-default">
默认
</button>
<button id="switcher-narrow">
一般显示
</button>
<button id="switcher-large">
大号显示
</button>
从上面定义可以看出，这里的id是经过精心安排的，把每个id使用横线分隔成两段，比如"switcher-default"可以很容易分开成switcher、default，这需要使用String对象的split() 方法，用于把一个字符串分割成字符串数组。
综合上面的知识点，就可把按钮的点击响应函数写成下面的样子：
```python
<html>
  <head>
    <meta charset="utf-8">
    <title>文章显示系统</title>
     <link rel="stylesheet" href="page/page.css" type="text/css" />
     <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
     <script>
       $(document).ready(function(){
         //鼠标位置在切换窗口上显示不同的颜色
         $('#switcher').hover(
           function(){ $(this).addClass('hover');    },
           function(){ $(this).removeClass('hover'); }
         );
         // 隐藏按钮，缩小显示位置
         var toggleSwitcher = function(event) {
           if (!$(event.target).is('button')) {
             $('#switcher button').toggleClass('hidden');
           }
         };
         $('#switcher').on('click', toggleSwitcher);
         // 模拟一次点击，让它在收缩状态
         $('#switcher').click();
         //按钮的事件响应
         $('#switcher').click(function(event) {
           if ($(event.target).is('button')) {
             var bodyClass = event.target.id.split('-')[1];
             console.log(bodyClass);
           }
         });
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
这样就可以完成不同的按钮处理了。

### 跟老菜鸟学python
### [http://edu.csdn.net/course/detail/2592](http://edu.csdn.net/course/detail/2592)
### 在VC2015里学会使用tinyxml库
### [http://edu.csdn.net/course/detail/2590](http://edu.csdn.net/course/detail/2590)
### 在Windows下SVN的版本管理与实战
### [http://edu.csdn.net/course/detail/2579](http://edu.csdn.net/course/detail/2579)
### Visual Studio 2015开发C++程序的基本使用


