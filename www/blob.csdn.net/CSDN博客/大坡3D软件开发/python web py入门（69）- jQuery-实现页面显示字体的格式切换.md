
# python web py入门（69）- jQuery - 实现页面显示字体的格式切换 - 大坡3D软件开发 - CSDN博客

2018年05月06日 12:24:41[caimouse](https://me.csdn.net/caimouse)阅读数：736


在前面学习按钮点击的事件处理，以及事件处理的顺序，现在继续来学习怎么样切换页面的显示，其实这个过程，就是修改样式的过程。因此，可以把代码修改如下：
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
         //
         $('#switcher-default').addClass('selected');
         //切换格式显示的函数
         var setBodyClass = function(className) {
            $('body').removeClass().addClass(className);
            $('#switcher button').removeClass('selected');
            $('#switcher-' + className).addClass('selected');
            $('#switcher').off('click', toggleSwitcher);
            if (className == 'default') {
              $('#switcher').on('click', toggleSwitcher);
            }
         };
         //按钮的事件响应
         $('#switcher').click(function(event) {
           if ($(event.target).is('button')) {
             var bodyClass = event.target.id.split('-')[1];
             console.log(bodyClass);
             setBodyClass(bodyClass);
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
      <div id="header">
       <h2>主题</h2>
       <h2 class="subtitle">副标题</h2>
       <div class="author">作者</div>
      </div>
      <div class="chapter" id="chapter-preface">
        <h3 class="chapter-title">前言</h3>
        <p>测试一下</p>
        <p>这里可以写一段</p>
        <p>时间</p>
        <p>时间</p>
      </div>
      <div class="chapter" id="chapter-1">
        <h3 class="chapter-title">章节标题</h3>
        <p>文章主要内容</p>
      </div>
    </div>
    <span></span>
  </body>
</html>
```

在这里主要添加下面这个函数：
var setBodyClass = function(className) {
$('body').removeClass().addClass(className);

$('\#switcher button').removeClass('selected');
$('\#switcher-' + className).addClass('selected');

$('\#switcher').off('click', toggleSwitcher);

if (className == 'default') {
$('\#switcher').on('click', toggleSwitcher);
}
};
这个函数的参数className，就是前面分解出来的按钮的id的一部分，比如：default，narrow，large。
$('body').removeClass().addClass(className);
这行代码，就是先把body的类属性全部删除，使用removeClass()方法；接着再使用addClass方法来添加新的类属性，其实就相当于如下代码的更改：
class = "default"
class = "narrow"
class = "large"
当浏览器渲染时，就会到page.css里找到对应的样式定义，如下：
body.large .chapter {
font-size: 1.5em;
}

body.narrow .chapter {
width: 250px;
}
当是default属性时，没有对应的样式，所以不作修改。当是large属性时，就会当body里有large和chapter属性时进行字体大小的修改。
$('\#switcher button').removeClass('selected');
这行代码是删除按钮选中的状态。
$('\#switcher-' + className).addClass('selected');
这行代码是设置对应按钮在选中的状态，这里使用'\#switcher-' + className来生成一个完整的id字符串，就可以找到对应的id按钮。
$('\#switcher').off('click', toggleSwitcher);
这行代码是删除所有点击触发隐藏的函数。
if (className == 'default') {
$('\#switcher').on('click', toggleSwitcher);
}
这几行代码是说明了，只有默认的状态之下才允许隐藏按钮出现。

### 比特币源码入门教程

### [https://edu.csdn.net/course/detail/6998](https://edu.csdn.net/course/detail/6998)

### 深入浅出Matplotlib
### [https://edu.csdn.net/course/detail/6859](https://edu.csdn.net/course/detail/6859)

### 深入浅出Numpy
### [http://edu.csdn.net/course/detail/6149](http://edu.csdn.net/course/detail/6149)


