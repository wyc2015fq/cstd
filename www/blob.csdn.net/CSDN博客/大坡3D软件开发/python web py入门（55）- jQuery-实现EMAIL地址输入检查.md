
# python web py入门（55）- jQuery - 实现EMAIL地址输入检查 - 大坡3D软件开发 - CSDN博客

2018年04月15日 13:38:54[caimouse](https://me.csdn.net/caimouse)阅读数：327


在前面学习了焦点失去事件，学习了输入内容获取，学习了输出内容到页面等等，把前面所有学习过的内容，综合到一起就可以实现EMAIL地址检查了。先来看代码如下：
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
                // 为input_element绑定blur事件
                $(input_element).blur(function() {
                    var input_val = input_element.val();
                    if (input_val == "") {
                        result.valid = false;
                        $(tip_element).html(null_tip);
                    } else if (!validate_func(input_val)) {
                        result.valid = false;
                        $(tip_element).html(invalid_tip);
                    } else {
                        result.valid = true;
                        $(tip_element).html("");
                    }
                });
                return result;
              }
        };
        //测试一下对象运行
        var ret = bd.bind($("form#register #email"),
          $("form#register #email").next(),
          function(val) {return val.match(/^([.a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+((\.[a-zA-Z0-9_-]{2,3}){1,2})$/);},
          "< 请输入邮箱",
          "< 邮箱格式不对"
        );
        console.log(ret);
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
在这段代码，先构造一个表单，在里面有两个输入框，一个是邮箱，一个是帐号。定义bd对象，这个对象只有一个键，这个键对应一个方法，就是对检查功能函数进行绑定。 function(input_element, tip_element, validate_func, null_tip, invalid_tip)输入五个参数，第一个参数是输入的元素，第二个是提示的元素，第三个是检查功能函数，第四是没有输入时提示信息，第五是输入非法时提示信息。接着下来就是创建一个对象result来返回函数判断的结果值，如果result.valid是true说明正确，否则就是出错。在函数判断出错，会通过  $(tip_element).html()来提示出错信息到页面上面。
运行的结果如下：
![](https://img-blog.csdn.net/20180415133732782)
通过这个例子，就把这一阶段学习的内容全部使用上了，如果不明白，就回头去看看前面的内容了。

### TensorFlow入门基本教程
### [http://edu.csdn.net/course/detail/4369](http://edu.csdn.net/course/detail/4369)
### C++标准模板库从入门到精通
### [http://edu.csdn.net/course/detail/3324](http://edu.csdn.net/course/detail/3324)
### 跟老菜鸟学C++
### [http://edu.csdn.net/course/detail/2901](http://edu.csdn.net/course/detail/2901)
### 跟老菜鸟学python
### [http://edu.csdn.net/course/detail/2592](http://edu.csdn.net/course/detail/2592)



