
# python web py入门（63）- jQuery - 判断浏览器窗口是否发生改变 - 大坡3D软件开发 - CSDN博客

2018年04月27日 17:13:50[caimouse](https://me.csdn.net/caimouse)阅读数：430


在前面学习了文本框里内容改变的事件，现在来学习当浏览器窗口发生改变时的事件。当你在页面上进行布局时，常常会遇到浏览器的窗口大小会改变，那么怎么样监听到这个事件呢？其实可以通过$(window).resize()来实现。
定义和用法
当调整浏览器窗口大小时，发生 resize 事件。
resize() 方法触发 resize 事件，或规定当发生 focus 事件时运行的函数。
可以使用下面的代码来演示这个功能：
```python
<html>
  <head>
    <meta charset="utf-8">
    <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
  </head>
  <script>
    $(document).ready(function(){
      $(window).resize(function(){
        $("p").text("窗口大小发生了改变.");
      });
    });
  </script>
  <body>
    <p>Ctrl + scroll 或 ctrl + ++来改变窗口大小 </p>
  </body>
</html>
```
运行结果如下：
![](https://img-blog.csdn.net/2018042717130880)
![](https://img-blog.csdn.net/20180427171320416)

### RPG游戏从入门到精通
### [http://edu.csdn.net/course/detail/5246](http://edu.csdn.net/course/detail/5246)
### WiX安装工具的使用
### [http://edu.csdn.net/course/detail/5207](http://edu.csdn.net/course/detail/5207)
### 俄罗斯方块游戏开发
### [http://edu.csdn.net/course/detail/5110](http://edu.csdn.net/course/detail/5110)


