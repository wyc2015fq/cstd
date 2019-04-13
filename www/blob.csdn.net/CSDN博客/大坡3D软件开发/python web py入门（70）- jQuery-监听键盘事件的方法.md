
# python web py入门（70）- jQuery - 监听键盘事件的方法 - 大坡3D软件开发 - CSDN博客

2018年06月02日 17:53:22[caimouse](https://me.csdn.net/caimouse)阅读数：575


在前面实现了通过鼠标点击实现切换不同的字体大小，在本文里将要学会使用键盘来实现字体大小的切换。要从键盘上获得信息，就必须监听键盘。那么在jQuery里怎么样来监听键盘呢？直接来看代码：
```python
//键盘事件响应
         $(document).keyup(function(event) {
           var key = String.fromCharCode(event.which);
           console.log(key);
         });
```
jquery的方式监听键盘事件
jquery的也分为三个过程，但是在事件名称上有所不同：
keyup：某个键盘的键被松开
keydown：某个键被按下
keypress：某个键盘的键被按下或按住
event.which ： which 属性指示按了哪个键或按钮。

从这段代码里，就可以把UNICODE的ASCII码转换为字符串，因此输出如下：
![](https://img-blog.csdn.net/20180602175220834)
完整的代码如下：
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
         //键盘事件响应
         $(document).keyup(function(event) {
           var key = String.fromCharCode(event.which);
           console.log(key);
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
### 比特币源码入门教程

### [https://edu.csdn.net/course/detail/6998](https://edu.csdn.net/course/detail/6998)

### 深入浅出Matplotlib
### [https://edu.csdn.net/course/detail/6859](https://edu.csdn.net/course/detail/6859)

### 深入浅出Numpy
### [http://edu.csdn.net/course/detail/6149](http://edu.csdn.net/course/detail/6149)


