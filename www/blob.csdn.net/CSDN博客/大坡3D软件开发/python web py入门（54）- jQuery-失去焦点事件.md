
# python web py入门（54）- jQuery - 失去焦点事件 - 大坡3D软件开发 - CSDN博客

2018年04月15日 11:57:23[caimouse](https://me.csdn.net/caimouse)阅读数：392


前面学习了获取输入文本框的内容，并且实现在弹出窗口里提示显示出来。接着下来需要对内容进行检查，又学习了使用对象的方式来构造通用的对象。这些内容都准备好了，接着要怎么样做呢？其实可以这样思考，有了内容，有了检验的方法，就剩下什么时候运行，以及运行的动机了。这个时刻非常重要，如果运行过早，内容还没有输入完成，或者效率低下；如果运行过迟，又会导致用户感觉不舒服。在页面进行输入EMAIL地址时，什么时候检查EMAIL地址的合法性呢?可以放在提交时一起检查，也可以放在每输入一个字符时检查。如果放在提交时检查，有点偏后，因为输入很多内容之后，再回到前面去修改，要滚动比较麻烦。如果放在键入时检查，显然用户还没有输入完整，判断结果肯定不正常。查看JS里，有一个失去焦点的事件，叫做onblur（事件会在对象失去焦点时发生），它可以这样使用：
<input type="text" id="fname" onblur="upperCase()" />
在这里onblur被赋值为upperCase()函数运行。
而在jQuery里简化了这种操作，可以按下面的代码来使用：
$( "form\#register \#email" ).blur(function() {
alert( "邮箱输入框已经失去焦点，调用.blur()事件触发！" );
});
在jQuery定义如下：
触发被选元素的 blur 事件。
语法
$(selector).blur()

为了体验这个事件的触发，在页面里放置两个输入文本框，然后当第一个文本框失去焦点时，就会触这个事件的响应，并弹出提示窗口。代码如下：
```python
<html>
  <head>
    <meta charset="utf-8">
    <title>
      失去焦点的例子
    </title>
    <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
    <script type="text/JavaScript">
      $(document).ready(function(){
        $( "form#register #email" ).blur(function() {
            alert( "邮箱输入框已经失去焦点，调用.blur()事件触发！" );
        });
      });
    </script>
  </head>
  <body>
    <form id="register" action="" method="POST">
      <table>
        <tbody>
          <tr>
            <td><label for="email">邮箱</label></td>
            <td><input type="text" id="email" name="email" /><span class="validate_tip"></span></td>
          </tr>
          <tr>
            <td><label for="username">帐号</label></td>
            <td><input type="text" id="username" name="username" /><span class="validate_tip"></span></td>
          </tr>
          <tr><td><input type="submit" id="register_btn" value="注册" /></td></tr>
        </tbody>
      </table>
    </form>
  </body>
</html>
```
运行结果如下：
![](https://img-blog.csdn.net/20180415115556889)
当你选择第二个文本框时，就会触发这个事件运行。

### Arduino入门基础
### [http://edu.csdn.net/course/detail/4931](http://edu.csdn.net/course/detail/4931)
### Unity5.x游戏基础入门
### [http://edu.csdn.net/course/detail/4810](http://edu.csdn.net/course/detail/4810)
### TensorFlow API攻略
### [http://edu.csdn.net/course/detail/4495](http://edu.csdn.net/course/detail/4495)


