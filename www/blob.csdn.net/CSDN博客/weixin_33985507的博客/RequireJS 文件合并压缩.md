# RequireJS  文件合并压缩 - weixin_33985507的博客 - CSDN博客
2015年06月17日 14:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
RequireJS的define 以及require 对于我们进行简化JavaScript 开发，进行模块化的处理具有很大的帮助
但是请求加载的js 文件会有一些影响，一般的处理是对于文件进行压缩，但是requirejs 的压缩不是简单的
进行压缩就行。
RequireJS提供了一个打包压缩工具r.js来对模块进行合并压缩。r.js非常强大，不但可以压缩js，css，甚至
可以对整个项目进行打包。
使用的方法为：
1. 项目结构
![](https://images0.cnblogs.com/blog2015/562987/201506/171442458419661.jpg)
文件说明：
model 中的为 定义的模块
main.js  为页面加载需要使用的
appinfo.js  内容为：
```java
define(function () {
    var appinfo = { name: "dalnog", age: 22 };
    return appinfo;
});
```
refjs.js    内容为：
```java
define(["model/user"], function (user) {
    var myinfo = {
        userinfo: user
    };
    return myinfo;
});
```
user.js   内容为：
```java
define(function () {
    var user = { name: "dalnog", age: 22 };
    return user;
});
```
index.htm  页面信息如下：
```
<!DOCTYPE >
<html  >
<head>
    <title></title>
</head>
<body>
//   压缩的使用方法
  <!--  <script   data-main="dalongappinfo-built" src="Scripts/require.js" type="text/javascript"></script>-->
//   默认的使用方法
    <script   data-main="main" src="Scripts/require.js" type="text/javascript"></script>
</body>
</html>
```
压缩的方法：
1.  安装node
2.  运行npm 安装 requirejs  自带r.js    
     命令为：  npm   install  -g  requirejs 
3.   配置 命令文件  config.js
       信息为：
```java
({
baseUrl: ".",  ///  文件的路径
name: "main",  ///  入口文件  就是main.js
///  实际压缩之后使用的js 文件
out: "dalongappinfo-built.js"
})
```
4.  运行命令   node   r.js -o  config.js 
    之后生成对应的文件： dalongappinfo-built.js
5. 使用：
     见上面的 index.htm
命令参数解析：
下面我们对命令分别来做一下解释：
**-o：** 表示优化，该参数是固定的，必选。
**baseUrl：**指存模块的根目录，可选。
**name：** 模块的入口文件，这里是app,那么r.js会从baseUrl+name去查找app.js，然后找出所有依赖的模块，然后进行合并与压缩。
**out:** 指合并压缩后输出的文件路径，这里是直接输出在根目录下build.js 我们也可以输出到其他目录下 比如js/app 目录下，也可以的。
