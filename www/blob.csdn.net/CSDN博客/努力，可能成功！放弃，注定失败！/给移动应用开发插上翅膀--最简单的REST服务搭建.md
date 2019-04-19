# 给移动应用开发插上翅膀--最简单的REST服务搭建 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月18日 22:24:41[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：484
目前智能手机越来越像只是一个网络世界的接口，如果不联网，手机仿佛都没有什么作用。很多时候，我们做应用开发的，都需要依赖服务端提供的REST服务，如果没有这些来自网络的JSON数据，仿佛一个手机应用也没有什么存在的意义。
在这样的大环境下，如果你要做网络应用的话（如社区、电商等）那不得不依赖服务端给你提供REST接口，没有这个一个Android/IOS开发就像是被砍去手脚的人，无法自由动弹。而且在实际项目中，和服务端的接口调试也耗费移动端开发很大的时间和精力。
有时，我们想做一个自己的应用，都苦于没有服务端支持。我偿试过很多服务端的解决方案，如Spring MVC，最终还是放弃了，我只想做一个简单的后台支持，代码的工作量和学习难度却都不小。不过好在Node.js出现了，曾经困扰我的问题都变得简单了，我们可以花很少的时间，用很少的代码（代码少至少意味着工作量少）就实现一个后台的REST服务。
![](http://upload-images.jianshu.io/upload_images/1685558-0aef2e58601e9b98.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
> 
Node.js®是一个基于[Chrome V8 引擎](https://developers.google.com/v8/)的JavaScript运行时。Node.js使用高效、轻量级的事件驱动、非阻塞I/O模型。Node.js的生态系统是目前最大的开源包管理系统。
Node.js的学习成本并不高，了解一些JavaScript的语法就可以动手了，我曾经要用一个月才做好的事情，我用Node.js来实现两天就好了。下边我们用一个简单的实列给大家演示一下如何用Node.js给我们的移动应用快速的搭建一个REST服务端。
- 安装Node.js & npm
到[官方网站https://nodejs.org](https://nodejs.org)上去下载安装即可。安装完成后，在命令终端输入：
> 
node -v 
能看到版本号，则说明安装成功了。新版的Node.js已经集成了npm（Node.js的第三方包管理工具），所以npm也一并安装好了。同样可以通过输入 **"npm -v" **来测试是否成功安装。
2.新建一个Node.js工程
新建一个目录，之后输入"npm init"，一路回车即可，想输点什么的话，可以自己看提示，这步主要是用于生成package.json文件，后边你还有机会改它。
> 
你现在只需要知道package.json是Node.js工程的库文件依赖配置文件就够了。
3.安装需要的JS库
> 
npm install --save express body-parser
express是一个简洁而灵活的Node.js Web应用框架；body-parser中间件用来解析http请求体。关于中间件的知识点，大家可以在express的网站学习一下:[使用中间件](http://expressjs.com/zh-cn/guide/using-middleware.html)。
4.编写server.js
```php
```php
var fs = require('fs');
var path = require('path');
var express = require('express');
var bodyParser = require('body-parser');
var app = express();
var USERS_FILE = path.join(__dirname, 'users.json'); // user.json文件的路径
app.set('port', (process.env.PORT || 3000));
app.use('/', express.static(path.join(__dirname, 'public')));
//使用body-parser中间件
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({extended: true}));
// 中间件，每个请求都会进行处理.
app.use(function(req, res, next) {
    // Set permissive CORS header - this allows this server to be used only as
    // an API server in conjunction with something like webpack-dev-server.
    res.setHeader('Access-Control-Allow-Origin', '*');
    // Disable caching so we'll always get the latest comments.
    res.setHeader('Cache-Control', 'no-cache');
    next();
});
// 处理/api/users的GET请求
app.get('/api/users', function(req, res) {
  fs.readFile(USERS_FILE, function(err, data) {
    if (err) {
      console.error(err);
      process.exit(1); //退出应用
    }
    res.json(JSON.parse(data));
  });
});
//处理/api/users的POST请求
app.post('/api/users', function(req, res) {
  fs.readFile(USERS_FILE, function(err, data) {
    if (err) {
      console.error(err);
      process.exit(1);
    }
    var users = JSON.parse(data);
    var user = {
      name: req.body.name,
      email: req.body.email
    };
    users.push(user);
    fs.writeFile(USERS_FILE, JSON.stringify(users, null, 4), function(err) {
      if (err) {
        console.error(err);
        process.exit(1);
      }
      res.json("{code: 200, message: 'Add user successful.'}");
    });
  });
});
app.listen(app.get('port'), function() {
  console.log('Server started: http://localhost:' + app.get('port') + '/');
});
```
```
简单解释一下这个代码，支持向/api/users这个path发送GET和POST的REST请求，数据会以JSON格式保存在文件中，GET请求时会从JSON文件请取。所以我们要提供一个新的REST服务时，只需要写一个请求路径和JSON格式的文件就好了。
5.运行server.js
> 
node server.js
可以在浏览器输入htpp://localhost:3000/api/users来测试一下我们的REST服务端返回的数据。用Postman插件试一下GET请求如下图所示：
![](http://upload-images.jianshu.io/upload_images/1685558-b1d00e0b396cc3cb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
下面我们演示一下用Postman插件试一下POST提交，在这个server.js里我们从req.body获取请求协带的“name”和“email”两个参数，然后将一个新的User保存的JSON文件里。
![](http://upload-images.jianshu.io/upload_images/1685558-8ab8dbe0d9d6c8b7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
看到结果返回信息显示添加用户成功，我们再用GET的请求来检测一下是否用户数据正确。
![](http://upload-images.jianshu.io/upload_images/1685558-3e5b2c2ecd937f16.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
OK，到此我们就用一个server.js和一个users.json文件就完成了一个可以添加用户和获取用户例表的REST服务。在没有添加新技术栈的情况下，我们要添加新的REST也很简单。虽然，我们的服务架设在本地，但足够我们在前期进行调试了。（只要在同一个WIFI网域，我们也可以用手机进行访问。）
完整的项目可以参看[GitHub https://github.com/goeasyway/simple_node_rest](https://github.com/goeasyway/simple_node_rest)。
当然了，这里只是为了方便大家快速能用上一个REST服务，实际中我们一般都使用MongoDB来存储数据内容，express还有更强大便捷的功能供我们使用，更详细的内容大家还查要花时间看一下Node.js和express的相关文档。
文／goeasyway（简书作者）
原文链接：http://www.jianshu.com/p/5ba667fdce91
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
