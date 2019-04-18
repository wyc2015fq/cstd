# webpack构建与loaders - z69183787的专栏 - CSDN博客
2017年08月03日 11:40:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：522
## [http://www.cnblogs.com/yangjunhua/p/5680168.html](http://www.cnblogs.com/yangjunhua/p/5680168.html)
## loaders 定义
先了解一下webpack，webpack是一个用于针对js文件的构建工具，在被构建的js文件中，我们可以使用require语句和webpack loader，如下：
```
var css = require("css!./file.css");
// => returns css code from file.css, resolves imports and url(...)
```
css代码中的 @import与url()类似js的require()请求资源文件，css-loader 将会对 file.css文件中的 @import 和 url(...) 请求的资源进行解析处理，并转交
给适合处理请求资源文件的file-loader 或 url-loader。详情请看官网：[https://www.npmjs.com/package/css-loader](https://www.npmjs.com/package/css-loader)
基于上面示例，我对loader的定义是 转译器 
## loaders 特征
1、loaders 支持链式，它们被应用在资源的的管道中，最后一个loader预期返回javascript；每个laoder会返回任意格式的资源，并交给下一个loader进行处理.
2、loaders 支持同步和异步
3、loaders 运行在node.js环境中，可以做任何可能的事情(使用node.js api)
4、loaders 接受查询参数，可以通过配置loader来使用
5、loaders 可以在配置中通过正则匹配的文件扩展名来绑定
6、loaders 可以通过npm发布，安装
7、除了在package.json文件配置 main入口(指定js文件)之外，一般的js模块都可以作为导出的loader
8、loaders 可以访问配置
9、plugins 可以给予 loaders 更多的特征
10、loaders 可以处理额外的任意文件
...
如果你对一些loader示例感兴趣，请查看loaders列表：[http://webpack.github.io/docs/list-of-loaders.html](http://webpack.github.io/docs/list-of-loaders.html)
## loaders 解析
loaders解析类似于模块解析，一个loader模块 就是一个执行于node.js环境中的函数；一般情况下，你应该通过使用npm来
管理你的loaders，但是在你的项目中，也可以使用单个js模块来作为你的loader  
1、引用loaders
loaders经常被命名为XXX-loader，XXX 代指文件mime类型的名称，如：json-loader
我们可以通过全名(如：json-loader)，也可以通过简写名(如：json)来引用loaders
2、loaders 安装
如果loader存在于npm中，你可以像下面代码来安装loader：
```
npm install xxx-loader --save
```
or
```
npm install xxx-loader --save-dev
```
3、用法
使用require语句指定loaders
我们使用 感叹号! 来将资源文件与loaders分开，使用多个loaders，应在所有的转换规则(loader)之前加上感叹号!，如：
```
require(!style!css!less!./src/css/index.less);
```
注：使用多个loaders是由右向左处理的，文件依次按less-loader，css-loader，style-loader处理
通过在webpack.config.js文件中配置指定loaders
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
{
    module: {
        loaders: [
            { test: /\.jade$/, loader: "jade" },
            // => "jade" loader is used for ".jade" files
            { test: /\.css$/, loader: "style!css" },
            // => "style" and "css" loader is used for ".css" files
            // Alternative syntax:
            { test: /\.css$/, loaders: ["style", "css"] },
        ]
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
通过在命令行中配置使用
通过命令行你可以将loaders绑定到扩展的文件进行转换，如：
```
webpack --module-bind jade --module-bind 'css=style!css'
```
这里使用jade-loader绑定到 扩展的jade文件进行转换，使用style-loader，css-loader绑定到css文件进行转换 
4、查询字符串
Loader可以通过查询字符串(类似web页面url中的查询字符串)传递查询参数，使用问号?将 loader与查询字符串连接，loader后跟查询字符串，如：
```
url-loader?mimetype=image/png
```
or 
```
url-loader?{"mimetype"="image/png"}
```
require写法
```
require("url-loader?mimetype=image/png!./file.png");
```
webpack.config.js配置
```
{ test: /\.png$/, loader: "url-loader?mimetype=image/png" }
```
or
```
{
    test: /\.png$/,
    loader: "url-loader",
    query: { mimetype: "image/png" }
}
```
命令行绑定
```
webpack --module-bind "png=url-loader?mimetype=image/png"
```
[http://blog.csdn.net/liwusen/article/details/53519342?utm_source=itdadao&utm_medium=referral](http://blog.csdn.net/liwusen/article/details/53519342?utm_source=itdadao&utm_medium=referral)
简介：在Webpack中，通过css-loader，可以实现在[js](http://lib.csdn.net/base/javascript)文件中通过require的方式，来引入css。
1.css-loader
我们需要在js文件里，通过require的方式来引入css，我们来看具体的方法，首先需要安装css-loader， 
style-loader(安装style-loader的目的是为了在html中以style的方式嵌入css)。
`npm install css-loader,style-loader --save-dev`- 1
- 1
然后在main.js中：
`require('./app.css');`- 1
- 1
在app.css中：
```
#test{
  background:red;
  width:100px;
  height:100px;
  color:blue;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 1
- 2
- 3
- 4
- 5
- 6
- 7
在webpack.config.js增加：
```
module.exports = {
  entry: './main.js',
  output: {
    filename: 'bundle.js'
  },
  module: {
    loaders:[
      { test: /\.css$/, loader: 'style-loader!css-loader' },
    ]
  }
};
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
在html中引入：
```xml
<html>
  <head>
    <script type="text/javascript" src="bundle.js"></script>
  </head>
  <body>
    <div id="test">Hello World</div>
  </body>
</html>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
效果为： 
![这里写图片描述](https://img-blog.csdn.net/20161208154821959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl3dXNlbg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
2.less-loader 
同样的，如果我们需要在js中，require，.less文件，那么我们需要增加包less-loader和less包
`npm install less,less-loader --save-dev`- 1
- 1
在webpack.config.js修改：
```
module: {
        loaders: [
            {test: /\.less$/, loader: 'style-loader!css-loader!less-loader'},
        ]
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 1
- 2
- 3
- 4
- 5
- 6
在module的loaders中，增加了！less-loader。
如此便可以在js中，require .less文件。
