
# python web py入门（71）- jQuery - 通过键盘实现页面显示字体的格式切换 - 大坡3D软件开发 - CSDN博客

2018年08月12日 11:37:25[caimouse](https://me.csdn.net/caimouse)阅读数：244


上一次演示了怎么样通过jQuery来监听键盘事件，这一次只要把键盘响应的按键值与对应字体进行关联，就可以实现页面字体的切换。因此，先要定义键值与字体的映射关系，怎么样来做呢？最简单的方法，就是使用json对象来关联，如下：
var triggers = {
D: 'default',
N: 'narrow',
L: 'large'
};
在这里定义triggers对象，它的键是D,N,L，也就是响应按键D,N,L三个，当按D键时，就可以返回'default'字符串，其它两个也是一样。这样就可以构造出来不同的样式出来，从而达到可以修改页面字体的大小。
因而把上次的代码：
```python
//键盘事件响应
         $(document).keyup(function(event) {
           var key = String.fromCharCode(event.which);
           console.log(key);
         });
```
修改为这样：
```python
//键盘映射表
         var triggers = {
            D: 'default',
            N: 'narrow',
            L: 'large'
         };
         //键盘事件响应
         $(document).keyup(function(event) {
           var key = String.fromCharCode(event.which);
           console.log(key);
           if (key in triggers) {
             setBodyClass(triggers[key]);
           }
         });
```
通过这样修改之后，就可以运行这个页面，可以从键盘里按下D键，页面就回到默认字体，按L键，就会变成大字体。显示如下：
![](https://img-blog.csdn.net/20180812113546351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2NhaW1vdXNl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
完整的HTML代码如下：
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
         //键盘映射表
         var triggers = {
            D: 'default',
            N: 'narrow',
            L: 'large'
         };
         //键盘事件响应
         $(document).keyup(function(event) {
           var key = String.fromCharCode(event.which);
           console.log(key);
           if (key in triggers) {
             setBodyClass(triggers[key]);
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
单片机编译器之链接器
[https://edu.csdn.net/course/detail/8437](https://edu.csdn.net/course/detail/8437)
### 比特币源码入门教程
### [https://edu.csdn.net/course/detail/6998](https://edu.csdn.net/course/detail/6998)
### 深入浅出Matplotlib
### [https://edu.csdn.net/course/detail/6859](https://edu.csdn.net/course/detail/6859)
### 深入浅出Numpy
### [http://edu.csdn.net/course/detail/6149](http://edu.csdn.net/course/detail/6149)
### Python游戏开发入门
[http://edu.csdn.net/course/detail/5690](http://edu.csdn.net/course/detail/5690)

