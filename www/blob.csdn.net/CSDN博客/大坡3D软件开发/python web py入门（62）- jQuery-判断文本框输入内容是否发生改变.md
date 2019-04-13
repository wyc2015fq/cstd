
# python web py入门（62）- jQuery - 判断文本框输入内容是否发生改变 - 大坡3D软件开发 - CSDN博客

2018年04月27日 16:43:27[caimouse](https://me.csdn.net/caimouse)阅读数：559


在前面学习文本框输入时，按键按下和弹起的事件响应，现在来学习当文本框的内容发生了改变时触发的事件。
定义和用法
当元素的值改变时发生 change 事件（仅适用于表单字段）。

change() 方法触发 change 事件，或规定当发生 change 事件时运行的函数。
注意：当用于 select 元素时，change 事件会在选择某个选项时发生。当用于 text field 或 text area 时，change 事件会在元素失去焦点时发生。

可以使用下面的例子来测试：
```python
<html>
  <head>
    <meta charset="utf-8">
    <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
  </head>
  <script>
    $(document).ready(function(){
      $("input").change(function(){
        alert("文本框的文本已经发生了改变.");
      });
    });
  </script>
  <body>
    请输入: <input type="text">
  </body>
</html>
```
运行的结果：
![](https://img-blog.csdn.net/20180427164256440)

### 你也能动手修改C编译器
[http://edu.csdn.net/course/detail/5582](http://edu.csdn.net/course/detail/5582)

### 纸牌游戏开发
### [http://edu.csdn.net/course/detail/5538](http://edu.csdn.net/course/detail/5538)
### 五子棋游戏开发

### [http://edu.csdn.net/course/detail/5487](http://edu.csdn.net/course/detail/5487)
### RPG游戏从入门到精通


