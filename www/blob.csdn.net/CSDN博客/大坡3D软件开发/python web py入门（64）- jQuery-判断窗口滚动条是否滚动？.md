
# python web py入门（64）- jQuery - 判断窗口滚动条是否滚动？ - 大坡3D软件开发 - CSDN博客

2018年05月01日 11:19:29[caimouse](https://me.csdn.net/caimouse)阅读数：262标签：[js](https://so.csdn.net/so/search/s.do?q=js&t=blog)[webpy](https://so.csdn.net/so/search/s.do?q=webpy&t=blog)[](https://so.csdn.net/so/search/s.do?q=js&t=blog)个人分类：[webpy](https://blog.csdn.net/caimouse/article/category/7445116)



前面学习了判断窗口大小改变的判断，现在来判断窗口的滚动条是否作出滚动的判断，这时需要使用jQuery的scroll()方法来实现，它的定义如下：

当用户滚动指定的元素时，会发生 scroll 事件。
scroll 事件适用于所有可滚动的元素和 window 对象（浏览器窗口）。
scroll() 方法触发 scroll 事件，或规定当发生 scroll 事件时运行的函数。

使用下面的代码来测试这个方法的使用：
```python
<html>
  <head>
    <meta charset="utf-8">
    <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
  </head>
  <script>
    var x = 0;
    $(document).ready(function(){
      $("div").scroll(function(){
          $("span").text("响应鼠标的滚动事件:" + x++);
      });
    });
  </script>
  <body>
    <div style="border:2px solid black;width:200px;height:200px;overflow:scroll;">
      在前面学习了文本框里内容改变的事件，现在来学习当浏览器窗口发生改变时的事件。<br>
      当你在页面上进行布局时，常常会遇到浏览器的窗口大小会改变，<br>
      那么怎么样监听到这个事件呢？其实可以通过$(window).resize()来实现。<br>
      定义和用法<br>
      当调整浏览器窗口大小时，发生 resize 事件。<br>
    </div>
    <span></span>
  </body>
</html>
```
运行结果如下：
![](https://img-blog.csdn.net/20180501111858368)

### Arduino入门基础
### [http://edu.csdn.net/course/detail/4931](http://edu.csdn.net/course/detail/4931)
### Unity5.x游戏基础入门
### [http://edu.csdn.net/course/detail/4810](http://edu.csdn.net/course/detail/4810)
### TensorFlow API攻略
### [http://edu.csdn.net/course/detail/4495](http://edu.csdn.net/course/detail/4495)


