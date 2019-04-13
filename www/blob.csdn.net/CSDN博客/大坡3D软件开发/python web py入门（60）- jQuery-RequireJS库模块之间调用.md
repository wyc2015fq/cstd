
# python web py入门（60）- jQuery - RequireJS库模块之间调用 - 大坡3D软件开发 - CSDN博客

2018年04月24日 17:40:47[caimouse](https://me.csdn.net/caimouse)阅读数：341


在前面学习了RequireJS库模块的定义，这次要来学习模块之间进行相互调用了。比如再添加一个模块叫做common.js， 放在跟register1.js一样的目录下面。
common.js的代码如下：
```python
define(['jquery'],
function($){
  return {
    // 合法的邮件
    email_pattern : /^([.a-zA-Z0-9_-])+@([a-zA-Z0-9_-])+((\.[a-zA-Z0-9_-]{2,3}){1,2})$/
  }
});
```
register1.js的代码如下：
```python
define(['jquery', 'common'],
function($, common){
    return {
      init: function() {
            alert("加载register1.js模块");
            alert(common.email_pattern);
      }
    }
});
```

在main.js模块里修改如下：
```python
// 全局配置
require.config({
    baseUrl: 'static/js',
    paths: {
        jquery: 'lib/jquery-3.3.1.min',
        common: 'app/common',
        register: 'app/register1'
    }
});
require(['jquery','register'],function($, register){
  $(document).ready(function() {
    alert("加载模块完成");
    register.init();
  });
});
```

在这里主要增加common模块的加载，require里请求这个模块加载，然后加载事里写上register.init()来调用。通过这样的方式，就可以把公共的代码放到一个文件里，可以给多个文件进行调用。同时可以把复杂代码分离开来，容易理解和维护。
最后可以看html的代码都没有变动，如下：
```python
<html>
  <head>
    <meta charset="utf-8">
    <title>
      使用模块相互调用的例子
    </title>
    <script src="static/js/lib/require.min.js" data-main="static/js/main.js" ></script>
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
输出结果如下：
![](https://img-blog.csdn.net/20180424174015681)

### boost库入门基础
### [http://edu.csdn.net/course/detail/5029](http://edu.csdn.net/course/detail/5029)
### Arduino入门基础
### [http://edu.csdn.net/course/detail/4931](http://edu.csdn.net/course/detail/4931)
### Unity5.x游戏基础入门
### [http://edu.csdn.net/course/detail/4810](http://edu.csdn.net/course/detail/4810)
### TensorFlow API攻略
### [http://edu.csdn.net/course/detail/4495](http://edu.csdn.net/course/detail/4495)



