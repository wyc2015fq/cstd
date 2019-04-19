# express 与 mvc - 左直拳的马桶_日用桶 - CSDN博客
2014年12月09日 23:27:37[左直拳](https://me.csdn.net/leftfist)阅读数：5125
个人分类：[JavaScript																[node.js](https://blog.csdn.net/leftfist/article/category/2626561)](https://blog.csdn.net/leftfist/article/category/94500)
听人介绍，说express.js是一个for nodejs的mvc框架。
既然是MVC，那么，express里面，什么是M，什么是V，又什么是C？
C，很容易看出来，就是路由。express的路由机制，app.get()、app.post、app.all。。。
V呢？当我们用 express初始化一个空的文件夹时，它会帮我们生成一系列子文件夹：
2014/12/09  22:58    <DIR>          .
2014/12/09  22:58    <DIR>          ..
2014/12/09  22:58             1,478 app.js
2014/12/09  22:58    <DIR>          bin
2014/12/09  22:58               327 package.json
2014/12/09  22:58    <DIR>          public
2014/12/09  22:58    <DIR>          routes
2014/12/09  22:58    <DIR>          views
其中，views顾名思义，就是存放视图的。里面有些*.jade文件，就是jade模板文件。所以，这个就是V了。
M呢？在哪？
从文件夹中看不出来。不过我猜，
var user = require("./user")；
然后在user.js
var users = function (req,res){
   this.name = "zhang3";
  this.sex = "male";
this.age = 40;
}
module.exports = users;
这种机制，就是m了。当然，如果user.js里有涉及到数据库操作，就可以更加肯定了。是吗？
express只是服务器端的MVC框架。前端也有所谓MVC框架，比如blackbone。
