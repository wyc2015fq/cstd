# 【BootStrap】web前端开发框架初探 - CD's Coding - CSDN博客





2017年01月30日 11:22:46[糖果天王](https://me.csdn.net/okcd00)阅读数：510









- [0x00 前言](#0x00-前言)
- [0x01 准备环境](#0x01-准备环境)- [获取预编译包](#获取预编译包)
- [获取源码及相关配置](#获取源码及相关配置)

- [0x02 使用](#0x02-使用)- [源码的使用](#源码的使用)
- [预编译包的使用](#预编译包的使用)

- [0xFF 相关资料](#0xff-相关资料)



# 0x00 前言

> 
**Bootstrap**

  简洁、直观、强悍的前端开发框架，让web开发更迅速、简单。

**Bootstrap** is a sleek, intuitive, and powerful front-end framework for faster and easier web development, created by Mark Otto and Jacob Thornton, and maintained by the core team with the massive support and involvement of the community.


**Bootstrap**是Twitter推出的一个用于前端开发的开源工具包。 

它由Twitter的设计师Mark Otto和Jacob Thornton合作开发,是一个CSS/HTML框架。 

目前,Bootstrap最新版本为v3。[*一些实例*](http://v3.bootcss.com/getting-started/#examples)
# 0x01 准备环境

## 获取预编译包
- 获取预编译包 
- [Bootstrap （当前版本 v3.3.5）](http://d.bootcss.com/bootstrap-3.3.5-dist.zip)
- 预编译包为用于生产环境的 Bootstrap 

编译并压缩后的 CSS、JavaScript 和字体文件。不包含文档和源码文件。 

如果不需要对源码进行操作，则无须获取源码，反之无需获取预编译包，请直接获取源码。
## 获取源码及相关配置
- 获取源码 (任选一种获取即可，推荐`Git`操作) 
- 在 [Github](https://github.com/twbs/bootstrap) 上 `download ZIP`
- `wget https://github.com/twbs/bootstrap/archive/v3.3.6.zip`
- `git clone https://github.com/twbs/bootstrap.git`

- 安装源码 (任选一种进行安装即可，推荐`npm`，可使用`pip install npm`获得) 
- Install with [Bower](http://bower.io/): `bower install bootstrap`.
- Install with [npm](https://www.npmjs.com/): `npm install bootstrap@3`.
- Install with [Meteor](https://www.meteor.com/): `meteor add twbs:bootstrap`.
- Install with [Composer](https://getcomposer.org/): `composer require twbs/bootstrap`. 

- 搭建编译环境（如果仅需要调用已有样式表可跳过此步） 
- Bootstrap 使用 **Grunt** 作为编译系统，并且对外提供了一些方便的方法用于编译整个框架。 
- `npm install -g grunt-cli`
- 在`/bootstrap/` 根目录下执行 `npm install` 命令
- 上一条指令会读取 `package.json` 文件并自动安装此文件中列出的所有被依赖的扩展包。
- [可用的Grunt命令](http://v3.bootcss.com/getting-started/#grunt-commands)`grunt dist`,`grunt watch`,`grunt test`,`grunt docs` etc.


# 0x02 使用

## 源码的使用

获取到的bootstrap源码文件结构如下：

```
bootstrap/
├── less/
├── js/
├── fonts/
├── dist/
│   ├── css/
│   ├── js/
│   └── fonts/
└── docs/
    └── examples/
```

> 
`less/`、`js/` 和 `fonts/` 目录分别包含了 CSS、JS 和字体图标的源码。`dist/` 目录包含了上面所说的预编译 Bootstrap 包内的所有文件。`docs/` 包含了所有文档的源码文件，`examples/` 目录是 Bootstrap 官方提供的实例工程。除了这些，其他文件还包含 Bootstrap 安装包的定义文件、许可证文件和编译脚本等。

- 若配置成功的情况下，可以直接在examples里找到html文件打开预览效果。
- 需要知道的是，源码用于自定义的样式和在已有样式的基础上进行修改，倘若仅仅只是需要调用已有样式，则可以直接下载预编译包而非源码。

## 预编译包的使用
- 预编译版本的文件结构如下——

```
bootstrap/
├── css/
│   ├── bootstrap.css
│   ├── bootstrap.css.map
│   ├── bootstrap.min.css
│   ├── bootstrap-theme.css
│   ├── bootstrap-theme.css.map
│   └── bootstrap-theme.min.css
├── js/
│   ├── bootstrap.js
│   └── bootstrap.min.js
└── fonts/
    ├── glyphicons-halflings-regular.eot
    ├── glyphicons-halflings-regular.svg
    ├── glyphicons-halflings-regular.ttf
    ├── glyphicons-halflings-regular.woff
    └── glyphicons-halflings-regular.woff2
```

```
上面展示的就是 Bootstrap 的基本文件结构：
预编译文件可以直接使用到任何 web 项目中, 我们提供了编译好的 CSS 和 JS (bootstrap.*) 文件，还有经过压缩的 CSS 和 JS (bootstrap.min.*) 文件。
还提供了 CSS 源码映射表 (bootstrap.*.map) ，可以在某些浏览器的开发工具中使用。
同时还包含了来自 Glyphicons 的图标字体，在附带的 Bootstrap 主题中使用到了这些图标。
```
- 如果仅需要调用而非使用Less自定义样式，使用以下给出的这份 HTML 模版，或者修改example中的实例，拷贝并粘贴下面给出的 HTML 代码，这就是一个最简单的 Bootstrap 页面了。

```xml
<!DOCTYPE html>
<html lang="zh-CN">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- 上述3个meta标签*必须*放在最前面，任何其他内容都*必须*跟随其后！ -->
    <title>Bootstrap 101 Template</title>

    <!-- Bootstrap -->
    <link href="css/bootstrap.min.css" rel="stylesheet">

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
      <script src="//cdn.bootcss.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="//cdn.bootcss.com/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>
  <body>
    <h1>你好，世界！</h1>

    <!-- jQuery (necessary for Bootstrap's JavaScript plugins) -->
    <script src="//cdn.bootcss.com/jquery/1.11.3/jquery.min.js"></script>
    <!-- Include all compiled plugins (below), or include individual files as needed -->
    <script src="js/bootstrap.min.js"></script>
  </body>
</html>
```
- 如下，列举一个实例《Starter Template for Bootstrap》，倘若需要这个实例下的样式，可以在源码或预编译包中的路径`docs/examples/starter-template`下找到该html代码，修改其代码即可，如若需要在其他地方使用，将文件夹中其特征的css一并拷贝过去即可。有任何需要的图标、js文件、样式表等，均可在源码/预编译包里找到（建议使用预编译包直接调用，源码中主要是通过将[less](http://www.runoob.com/manual/lessguide/%20Less%E8%AF%AD%E8%A8%80)文件编译成样式表，在`dist`中调试使用）。

```xml
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- The above 3 meta tags *must* come first in the head; any other head content must come *after* these tags -->
    <meta name="description" content="">
    <meta name="author" content="">
    <link rel="icon" href="../../favicon.ico">

    <title>Starter Template for Bootstrap</title>

    <!-- Bootstrap core CSS -->
    <link href="../../dist/css/bootstrap.min.css" rel="stylesheet">

    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <link href="../../assets/css/ie10-viewport-bug-workaround.css" rel="stylesheet">

    <!-- Custom styles for this template -->
    <link href="starter-template.css" rel="stylesheet">

    <!-- Just for debugging purposes. Don't actually copy these 2 lines! -->
    <!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->
    <script src="../../assets/js/ie-emulation-modes-warning.js"></script>

    <!-- HTML5 shim and Respond.js for IE8 support of HTML5 elements and media queries -->
    <!--[if lt IE 9]>
      <script src="https://oss.maxcdn.com/html5shiv/3.7.2/html5shiv.min.js"></script>
      <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
    <![endif]-->
  </head>

  <body>

    <nav class="navbar navbar-inverse navbar-fixed-top">
      <div class="container">
        <div class="navbar-header">
          <button type="button" class="navbar-toggle collapsed" data-toggle="collapse" data-target="#navbar" aria-expanded="false" aria-controls="navbar">
            <span class="sr-only">Toggle navigation</span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
            <span class="icon-bar"></span>
          </button>
          <a class="navbar-brand" href="#">Project name</a>
        </div>
        <div id="navbar" class="collapse navbar-collapse">
          <ul class="nav navbar-nav">
            <li class="active"><a href="#">Home</a></li>
            <li><a href="#about">About</a></li>
            <li><a href="#contact">Contact</a></li>
          </ul>
        </div><!--/.nav-collapse -->
      </div>
    </nav>

    <div class="container">

      <div class="starter-template">
        <h1>Bootstrap starter template</h1>
        <p class="lead">Use this document as a way to quickly start any new project.<br> All you get is this text and a mostly barebones HTML document.</p>
      </div>

    </div><!-- /.container -->


    <!-- Bootstrap core JavaScript
    ================================================== -->
    <!-- Placed at the end of the document so the pages load faster -->
    <script src="https://ajax.googleapis.com/ajax/libs/jquery/1.11.3/jquery.min.js"></script>
    <script>window.jQuery || document.write('<script src="../../assets/js/vendor/jquery.min.js"><\/script>')</script>
    <script src="../../dist/js/bootstrap.min.js"></script>
    <!-- IE10 viewport hack for Surface/desktop Windows 8 bug -->
    <script src="../../assets/js/ie10-viewport-bug-workaround.js"></script>
  </body>
</html>
```

# 0xFF 相关资料
- [BootStrap中文网](http://www.bootcss.com/)
- [Bootstrap2 中文文档(v2.3.2)](http://v2.bootcss.com/)
- [GuideBook](http://v2.bootcss.com/getting-started.html)
- [经典案例/实例](http://v2.bootcss.com/getting-started.html#examples)
- Bootstrap 3 版本并不向后兼容 v2.x 版本。提供从 v2.x 版本升级到 v3.0 版本的通用指南 
[《从 Bootstrap 2.x 版本升级到 3.0 版本》](http://v3.bootcss.com/migration/)
- [Bootstrap3 中文文档(v3.3.5)](http://v3.bootcss.com/)**`推荐使用此版本`**
- [GuideBook](http://v3.bootcss.com/getting-started/)
- [经典案例/实例](http://v3.bootcss.com/getting-started/#examples)
- [v3新增内容](http://blog.getbootstrap.com/2013/08/19/bootstrap-3-released/)

- [Bootstrap4 (v4.0.0-alpha)](http://v4.bootcss.com/)
- 当前为Alpha版本，尚未完全完善

- [bootcdn 工具库](http://www.bootcdn.cn/)
- 免费 CDN 加速服务（同时支持 http 和 https 协议）

```
<!-- 新 Bootstrap 核心 CSS 文件 -->
<link rel="stylesheet" href="//cdn.bootcss.com/bootstrap/3.3.5/css/bootstrap.min.css">

<!-- 可选的Bootstrap主题文件（一般不用引入） -->
<link rel="stylesheet" href="//cdn.bootcss.com/bootstrap/3.3.5/css/bootstrap-theme.min.css">

<!-- jQuery文件。务必在bootstrap.min.js 之前引入 -->
<script src="//cdn.bootcss.com/jquery/1.11.3/jquery.min.js"></script>

<!-- 最新的 Bootstrap 核心 JavaScript 文件 -->
<script src="//cdn.bootcss.com/bootstrap/3.3.5/js/bootstrap.min.js"></script>
```
- 
相关工具

> 
[**`Bootlint`**](https://github.com/twbs/bootlint) 是 Bootstrap 官方所支持的 HTML 检测工具。在使用了 Bootstrap 的页面上（没有对 Bootstrap 做修改和扩展的情况下），它能自动检查某些常见的 HTML 错误。纯粹的 Bootstrap 组件需要固定的 DOM 结构。Bootlint 就能检测你的页面上的这些“纯粹”的 Bootstrap 组件是否符合 Bootstrap 的 HTML 结构规则。建议将 Bootlint 加入到你的开发工具中，这样就能帮你在项目开发中避免一些简单的错误影响你的开发进度。


- 
Less语言
- Less 是一门 CSS 预处理语言，它扩充了 CSS 语言，增加了诸如变量、混合（mixin）、函数等功能，让 CSS 更易维护、方便制作主题、扩充。
- Less 可以运行在 Node、浏览器和 Rhino 平台上。网上有很多第三方工具帮助你编译 Less 源码。
- [Less语言入门教程](http://www.runoob.com/manual/lessguide/)
- [Less.js Github链接](https://github.com/less/less.js)










