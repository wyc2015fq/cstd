
# python web py入门（59）- jQuery - RequireJS库定义模块 - 大坡3D软件开发 - CSDN博客

2018年04月20日 18:21:54[caimouse](https://me.csdn.net/caimouse)阅读数：288


在前面学习RequireJS库基本功能，现在来学习使用它定义一个模块，这个模块有依赖别的模块，并且作为参数传递过来使用。
先来看看这个工程的目录结构如下：
myweb20.html
--static\js
--static\js--main.js
--static\js--app--jquery-3.3.1.min.js
--static\js--app--require.min.js
--static\js--lib--register.js

在根目录下是文件myweb20.html，这个浏览器打开的页面文件，在这个页面里调用main.js，这行代码如下：
<script src="static/js/lib/require.min.js" data-main="static/js/main.js" ></script>
与前面的例子是一样的，首先会加载require.min.js运行，接着它从参数data-main获取配置的文件main.js，这样就可以运行main.js文件。
在这个文件里代码如下：
```python
// 全局配置
require.config({
    baseUrl: 'static/js',
    paths: {
        jquery: 'lib/jquery-3.3.1.min',
        register: 'app/register'
    }
});
require(["register"],function(){
    alert("加载模块完成");
});
```

在这里使用require.config（）来配置相关模块的路径，以及模块的名称。baseUrl是指明RequireJS库加载模块的相对路径，在这里是指定为static/js，比如后面要加载jquery库时，就是从路径static/js/lib/jquery-3.3.1.min.js里加载。paths是用来定义不在根目录下的模块，比如jquery代表了lib/jquery-3.3.1.min.js模块， register代表了app/register.js模块。在后面的使用中，就可以直接使用jquery和register。
接着下来，就是使用require（）函数来加载运行的模块，这个函数有两个参数，第一个参数是表示需要加载的模块名称数组，这里是["register"]，就是表示加载register模块。第二个参数是表示加载模块完成时做什么事情，在这里定义一个函数：
function(){
alert("加载模块完成");
}
通过这个文件，就完成RequireJS库的配置，以及需要加载的模块。

在register.js文件里定义了一个模块，代码如下：
```python
define(['jquery'], function(){
    function fun1($){
      alert("加载register.js模块");
    }
    fun1();
})
```

它是通过define（）函数来实现的，这个函数有两个参数，第一个参数是表示本模块依赖的模块，也就是说需要把这些模块都加载完成，才可以运行后面的函数，同样采用数组的方式定义，比如['jquery']，它是表示加载jquery模块，第二参数是模块返回的函数对象，函数的参数列表可以传送模块顺序，比如：
define(['jquery', 'validator', 'common'],
function($, validator, common) {
这里前面加载模块名称的顺序，后面是函数里使用这些模块的名称的顺序，这是一一对应的。
通过这种方式，就可以知道每个模块运行需要加载什么模块，同时提供调用这些模块的对象名称。
myweb20.html的代码：

```python
<html>
  <head>
    <meta charset="utf-8">
    <title>
      使用require的例子
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
运行结果如下：
![](https://img-blog.csdn.net/20180420182007299)
![](https://img-blog.csdn.net/20180420182107721)

### RPG游戏从入门到精通
### [http://edu.csdn.net/course/detail/5246](http://edu.csdn.net/course/detail/5246)
### WiX安装工具的使用
### [http://edu.csdn.net/course/detail/5207](http://edu.csdn.net/course/detail/5207)
### 俄罗斯方块游戏开发
### [http://edu.csdn.net/course/detail/5110](http://edu.csdn.net/course/detail/5110)
### boost库入门基础
### [http://edu.csdn.net/course/detail/5029](http://edu.csdn.net/course/detail/5029)



