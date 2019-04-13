
# python web py入门（53）- javascript - 对象 - 大坡3D软件开发 - CSDN博客

2018年04月14日 19:57:07[caimouse](https://me.csdn.net/caimouse)阅读数：248


在前面学习了怎么样在页面里输出内容，后面需要检查邮箱是否正确，这个功能如果放到一个函数定义，那么需要返回一个结果，而返回结果又是一个对象，那么就得来学习js的对象。js的对象定义如下：
JavaScript 中的所有事物都是对象：字符串、数值、数组、函数...
此外，JavaScript 允许自定义对象。
JavaScript 对象
JavaScript 提供多个内建对象，比如 String、Date、Array 等等。
对象只是带有属性和方法的特殊数据类型。
访问对象的属性
属性是与对象相关的值。
访问对象属性的语法是：
objectName.propertyName
这个例子使用了 String 对象的 length 属性来获得字符串的长度：
var message="Hello World!";
var x=message.length;

创建对象的方法有很多，常常用到有两种，第一种是创建JSON对象，这个就是大名顶顶的JSON对象，它是非常简单的关系型的数据结构，其实还是树形的结构。它的语法是采用一个大括号，比如：
var bd = {
bind**:**function(input_element, tip_element, validate_func, null_tip, invalid_tip) {
var result = new Object();
result.valid = false;
//$("form\#register \#email")找到span
$(tip_element).html(null_tip);
}
}
在这里只定义一个属性bind，并且它是一个函数对象。使用了大括号的方式来创建，可以这样调用它：
bd.**bind**($("form\#register \#email"),
$("form\#register \#email").next(),
function(val) {return val.match("/^([.a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+((\.[a-zA-Z0-9_-]{2,3}){1,2})$/");},
"< 请输入邮箱",
"< 邮箱格式不对"
);
另外一种创建对象的方法，如下：
**var result = new Object();**
result.valid = false;
在这里创建一个Object对象，以上两种创建对象方式的缺点:不能作为对象创建的模板，也就是不能用new进行构造新对象。
综合上面两种方式，可以把检验的代码写成这样：
```python
<html>
  <head>
    <meta charset="utf-8">
    <title>
      页面提示的例子
    </title>
    <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
    <script type="text/JavaScript">
      $(document).ready(function(){
        var bd = {
              bind :  function(input_element, tip_element, validate_func, null_tip, invalid_tip) {
                var result = new Object();
                result.valid = false;
                //$("form#register #email")找到span
                $(tip_element).html(null_tip);
              }
        }
        //测试一下对象运行
        bd.bind($("form#register #email"),
          $("form#register #email").next(),
          function(val) {return val.match("/^([.a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+((\.[a-zA-Z0-9_-]{2,3}){1,2})$/");},
          "< 请输入邮箱",
          "< 邮箱格式不对"
        );
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
          <tr><td><input type="submit" id="register_btn" value="注册" /></td></tr>
        </tbody>
      </table>
    </form>
  </body>
</html>
```
运行结果如下：
![](https://img-blog.csdn.net/20180414195639279)

### TensorFlow API攻略
### [http://edu.csdn.net/course/detail/4495](http://edu.csdn.net/course/detail/4495)
### TensorFlow入门基本教程
### [http://edu.csdn.net/course/detail/4369](http://edu.csdn.net/course/detail/4369)
### C++标准模板库从入门到精通
### [http://edu.csdn.net/course/detail/3324](http://edu.csdn.net/course/detail/3324)




