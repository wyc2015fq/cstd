
# python web py入门（58）- jQuery - RequireJS库 - 大坡3D软件开发 - CSDN博客

2018年04月20日 11:48:04[caimouse](https://me.csdn.net/caimouse)阅读数：334


在前面已经学习过多个JS模块，这样方便管理代码，维护代码，但时这样分开时，必须要考虑加载不同文件之间的顺序，这是比较头痛的问题，有没有好的方法解决呢？这时，就要使用RequireJS库了，它是一个JS文件和模块加载器，它是一个在浏览器优化的模块加载器，主要改善了加载的速度和改善你维护的代码质量。
它支持以下浏览器：
IE 6+ .......... compatible ✔
Firefox 2+ ..... compatible ✔
Safari 3.2+ .... compatible ✔
Chrome 3+ ...... compatible ✔
Opera 10+ ...... compatible ✔

为什么要使用模块化？它有以下好处：
解决命名冲突
实现依赖管理
模块的版本管理
提高可维护性
前端性能优化
跨环境共享模块

下载require.js 2.3.5版本，连接地址：
[http://requirejs.org/docs/download.html](http://requirejs.org/docs/download.html)
下载库文件之后，就需要建立自己的工程目录，目录的布局决定一个项目的管理水平，见过很多人开发项目时，目录都没有按规范定义好，就开始创建代码，最后整个项目是混乱不堪。因此，先来建立一个项目的目录结构，我打算把这个项目叫做WWW工程，然后创建一个子目录<www>,如下：
![](https://img-blog.csdn.net/20180420114105616)
![](https://img-blog.csdn.net/20180420114228369)

然后在目录里创建index.html页面文件，在这个文件调用JS的模块代码，怎么样调用呢？它的代码如下：
```python
<!DOCTYPE html>
<html>
    <head>
        <script data-main="app" src="lib/require.js"></script>
    </head>
    <body>
        <h1>Hello World</h1>
    </body>
</html>
```
在这里主要关注这行代码：
<script data-main="app" src="lib/require.js"></script>
这行代码就是加载requirejs库，同时元素data-main把需要加载模块的app.js传送给requirejs库, requirejs库就可以找到要加载的模块代码。同时，可以看到requirejs库的代码是放在lib目录下面，这样可以应用的模块代码与库的代码进行分离。
接着下来，可以查看app.js的代码如下：
```python
// 对第三方的依赖库，比如jQuery，应该放在lib目录下面。
// 配置加载lib目录路径， app加载目录路径
requirejs.config({
    baseUrl: 'lib',
    paths: {
        app: '../app'
    }
});
// 开始加载main.js应用模块文件
requirejs(['app/main']);
```
baseUrl——用于加载模块的根路径。
paths——用于映射不存在根路径下面的模块路径。
shims——配置在脚本/模块外面并没有使用RequireJS的函数依赖并且初始化函数。假设underscore并没有使用  RequireJS定义，但是你还是想通过RequireJS来使用它，那么你就需要在配置中把它定义为一个shim。
deps——加载依赖关系数组

接着下来，就可以看到加载app/main.js文件了。
模块是进行了内部实现封装、暴露接口和合理限制范围的对象。ReuqireJS提供了define函数用于定义模块。按章惯例每个Javascript文件只应该定义一个模块。define函数接受一个依赖数组和一个包含模块定义的函数。通常模块定义函数会把前面的数组中的依赖模块按顺序做为参数接收。
定义一个模块代码如下：
```python
define(function (require) {
    // 使用相对路径来加载同目录的模块        
    var messages = require('./messages');
    // 加载库目录的模块    
    var print = require('print');
    print(messages.getHello());
});
```
messages.js代码如下：
```python
define(function () {
    return {
        getHello: function () {
            return 'Hello World';
        }
    };
});
```
print.js代码如下：
```python
define(function () {
    return function print(msg) {
        console.log(msg);
    };
});
```
这样就完成一个简单，不同模块代码的分开管理的例子。其它任何复杂的项目都可以在这个基础之上构建。
最后运行的结果如下：
![](https://img-blog.csdn.net/20180420114717471)

### 比特币源码入门教程

### [https://edu.csdn.net/course/detail/6998](https://edu.csdn.net/course/detail/6998)

### 深入浅出Matplotlib
### [https://edu.csdn.net/course/detail/6859](https://edu.csdn.net/course/detail/6859)

### 深入浅出Numpy
### [http://edu.csdn.net/course/detail/6149](http://edu.csdn.net/course/detail/6149)

### Python游戏开发入门
[http://edu.csdn.net/course/detail/5690](http://edu.csdn.net/course/detail/5690)




