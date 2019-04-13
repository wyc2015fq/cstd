
# python web py入门（56）- jQuery - HTML与JS代码分离 - 大坡3D软件开发 - CSDN博客

2018年04月15日 18:01:34[caimouse](https://me.csdn.net/caimouse)阅读数：440


前面学习了检验EMAIL合法性的功能，但是会发现HTML代码已经越来越长了，这时看起来，就不是以前那么好理解了，怎么办呢？人对复杂事物有一个很好的工具，就是分解，把复杂的事物分成简单的事物，这样理解起来就容易多了。同理，对于嵌入JS代码到HTML代码已经很长了，最好的方式是把它从HTML分离出来，把JS代码变成一个文件，纯是JS代码，这样HTML代码里就没有大段大段的JS代码了。按照这样的分离方法，可以把上一文的代码分成两个文件：myweb18.html和checkemail.js，分别如下：
```python
<html>
  <head>
    <meta charset="utf-8">
    <title>
      HTML与JS的例子
    </title>
    <script type="text/JavaScript" src="jquery/jquery-3.3.1.js"></script>
    <script type="text/JavaScript" src= "app/checkemail.js"></script>
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
checkemail.js：
```python
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
```

同样在HTML里修改为这样：
<script type="text/JavaScript" src= "app/checkemail.js"></script>
在这里也创建了一个app目录，方便后面写多个js代码文件，方便多模块管理。这样分离的方式和习惯，绝对是一个好的开发人员应该具备的最低要求，否则后面项目越来越大，就会效率越来越差。
已经分开了，再来测试一下运行结果，是否发现嵌入的JS代码和分离的JS代码，功能一致吗？
![](https://img-blog.csdn.net/20180415180103804)

### 在VC2015里学会使用tinyxml库
### [http://edu.csdn.net/course/detail/2590](http://edu.csdn.net/course/detail/2590)
### 在Windows下SVN的版本管理与实战
### [http://edu.csdn.net/course/detail/2579](http://edu.csdn.net/course/detail/2579)
### Visual Studio 2015开发C++程序的基本使用
### [http://edu.csdn.net/course/detail/2570](http://edu.csdn.net/course/detail/2570)


