
# python web py入门（61）- jQuery - 使用keyup()和keydown()函数 - 大坡3D软件开发 - CSDN博客

2018年04月27日 15:30:50[caimouse](https://me.csdn.net/caimouse)阅读数：467


在前面已经学会了使用jQuery和RequireJS来分模板开发JS的代码，接着下来继续学习jQuery一些常用的方法。比如你想当用户在输入时，进行一些变化，特别感应用户的按键输入。假如有这样的一个需求，当用户按下键时，输入框变改变背景的颜色为绿色，当用户不输入时改变背景为红色，可以使用按键响应函数：
定义和用法
与 keyup 事件相关的事件顺序：
keydown - 键按下的过程
keypress - 键被按下
keyup - 键被松开
当键盘键被松开时发生 keyup 事件。
keyup() 方法触发 keyup 事件，或规定当发生 keyup 事件时运行的函数。
keydown() 方法触发 keydown 事件，或规定当发生 keydown 事件时运行的函数。
这个例子代码如下：
```python
<html>
  <head>
    <meta charset="utf-8">
    <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
  </head>
  <script>
    $(document).ready(function(){
      $("input").keydown(function(){
        $("input").css("background-color", "green");
      });
      
      $("input").keyup(function(){
        $("input").css("background-color", "red");
      });
    });
  </script>
  <body>
    请输入: <input type="text">
  </body>
</html>
```
运行的结果如下：
![](https://img-blog.csdn.net/20180427152936767)
这个是当按键按下时的情况。
![](https://img-blog.csdn.net/20180427153016358)

### 比特币源码入门教程

### [https://edu.csdn.net/course/detail/6998](https://edu.csdn.net/course/detail/6998)

### 深入浅出Matplotlib
### [https://edu.csdn.net/course/detail/6859](https://edu.csdn.net/course/detail/6859)

### 深入浅出Numpy
### [http://edu.csdn.net/course/detail/6149](http://edu.csdn.net/course/detail/6149)

### Python游戏开发入门
[http://edu.csdn.net/course/detail/5690](http://edu.csdn.net/course/detail/5690)


