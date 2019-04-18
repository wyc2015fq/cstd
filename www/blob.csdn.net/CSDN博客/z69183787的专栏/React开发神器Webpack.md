# React开发神器Webpack - z69183787的专栏 - CSDN博客
2016年07月15日 12:04:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1778
编者按：自2013年Facebook发布以来，React吸引了越来越多的开发者，基于它的衍生技术，如React Native、React Canvas等也层出不穷。InfoQ精心策划“[深入浅出React](http://www.infoq.com/cn/React1)”系列文章，为读者剖析React开发的技术细节。
上一篇我们对React有了一个总体的认识，在介绍其中的技术细节之前，我们首先来了解一下用于React开发和模块管理的主流工具Webpack。称之为React开发神器有点标题党了，不过Webpack确实是笔者见过的功能最为强大的前端模块管理和打包工具。虽然Webpack是一个通用的工具，并不只适合于React，但是很多React的文章或者项目都使用了Webpack，尤其是[react-hot-loader](https://github.com/gaearon/react-hot-loader)这样的神器存在，让Webpack成为最主流的React开发工具。
CommonJS和AMD是用于JavaScript模块管理的两大规范，前者定义的是模块的同步加载，主要用于NodeJS；而后者则是异步加载，通过requirejs等工具适用于前端。随着npm成为主流的JavaScript组件发布平台，越来越多的前端项目也依赖于npm上的项目，或者自身就会发布到npm平台。因此，让前端项目更方便的使用npm上的资源成为一大需求。于是诞生了类似[browserify](http://browserify.org/)这样的工具，代码中可以使用require函数直接以同步语法形式引入npm模块，打包后再由浏览器执行。
Webpack其实有点类似browserify，出自Facebook的Instagram团队，但功能比browserify更为强大。其主要特性如下：
- 
同时支持[CommonJS](http://wiki.commonjs.org/wiki/Modules/1.1)和[AMD](https://github.com/amdjs/amdjs-api/wiki/AMD)模块（对于新项目，推荐直接使用CommonJS）；
- 
串联式模块加载器以及插件机制，让其具有更好的灵活性和扩展性，例如提供对CoffeeScript、ES6的支持；
- 
可以基于配置或者智能分析打包成多个文件，实现公共模块或者按需加载；
- 
支持对CSS，图片等资源进行打包，从而无需借助Grunt或Gulp；
- 
开发时在内存中完成打包，性能更快，完全可以支持开发过程的实时打包需求；
- 
对sourcemap有很好的支持，易于调试。
Webpack将项目中用到的一切静态资源都视之为模块，模块之间可以互相依赖。Webpack对它们进行统一的管理以及打包发布，其官方主页用下面这张图来说明Webpack的作用：
![](http://cdn2.infoqstatic.com/statics_s2_20160712-0252/resource/articles/react-and-webpack/zh/resources/0602005.jpg)
可以看到Webpack的目标就是对项目中的静态资源进行统一管理，为产品的最终发布提供最优的打包部署方案。本文就将围绕React对其相关用法做一个总体介绍，从而能让你将其应用在自己的实际项目之中。
#### 安装Webpack，并加载一个简单的React组件
Webpack一般作为全局的npm模块安装：
npm install -g webpack
之后便有了全局的webpack命令，直接执行此命令会默认使用当前目录的webpack.config.js作为配置文件。如果要指定另外的配置文件，可以执行：
webpack —config webpack.custom.config.js
尽管Webpack可以通过命令行来指定参数，但我们通常会将所有相关参数定义在配置文件中。一般我们会定义两个配置文件，一个用于开发时，另外一个用于产品发布。生产环境下的打包文件不需要包含sourcemap等用于开发时的代码。配置文件通常放在项目根目录之下，其本身也是一个标准的CommonJS模块。
一个最简单的Webpack配置文件webpack.config.js如下所示：
```java
```java
module.exports = {
  entry:[
    './app/main.js'
  ],
  output: {
    path: __dirname + '/assets/',
    publicPath: "/assets/",
    filename: 'bundle.js'
  }
};
```
```
其中entry参数定义了打包后的入口文件，数组中的所有文件会按顺序打包。每个文件进行依赖的递归查找，直到所有相关模块都被打包。output参数定义了输出文件的位置，其中常用的参数包括：
- path: 打包文件存放的绝对路径
- publicPath: 网站运行时的访问路径
- filename: 打包后的文件名
现在来看如何打包一个React组件。假设有如下项目文件夹结构：
- react-sample
  + assets/
   - js/
     Hello.js
     entry.js
   index.html
   webpack.config.js
其中Hello.js定义了一个简单的React组件，使用ES6语法：
```java
```java
var React = require('react');
class Hello extends React.Component {
  render() {
    return (
      <h1>Hello {this.props.name}!</h1>
    );
  }
}
```
```
entry.js是入口文件，将一个Hello组件输出到界面：
```java
```java
var React = require('react');
var Hello = require('./Hello');
React.render(<Hello name="Nate" />, document.body);
```
```
index.html的内容如下：
```
<html>
<head></head>
<body>
<script src="/assets/bundle.js"></script>
</body>
</html>
```
在这里Hello.js和entry.js都是JSX组件语法，需要对它们进行预处理，这就要引入webpack的JSX加载器。因此在配置文件中加入如下配置：
```java
```java
module: {
  loaders: [
    { test: /\.jsx?$/, loaders: ['jsx?harmony']}
  ]
}
```
```
加载器的概念稍后还会详细介绍，这里只需要知道它能将JSX编译成JavaScript并加载为Webpack模块。这样在当前目录执行webpack命令之后，在assets目录将生成bundle.js，打包了entry.js的内容。当浏览器打开当前服务器上的index.html，将显示“Hello Nate!”。这是一个非常简单的例子，演示了如何使用Webpack来进行最简单的React组件打包。
## 加载AMD或CommonJS模块
在实际项目中，代码以模块进行组织，AMD是在CommonJS的基础上考虑了浏览器的异步加载特性而产生的，可以让模块异步加载并保证执行顺序。而CommonJS的`require`函数则是同步加载。在Webpack中笔者更加推荐CommonJS方式去加载模块，这种方式语法更加简洁直观。即使在开发时，我们也是加载Webpack打包后的文件，通过sourcemap去进行调试。
除了项目本身的模块，我们也需要依赖第三方的模块，现在比较常用的第三方模块基本都通过npm进行发布，使用它们已经无需单独下载管理，需要时执行`npm install`即可。例如，我们需要依赖jQuery，只需执行：
npm install jquery —save-dev
更多情况下我们是在项目的package.json中进行依赖管理，然后通过直接执行npm install来安装所有依赖。这样在项目的代码仓库中并不需要存储实际的第三方依赖库的代码。
安装之后，在需要使用jquery的模块中需要在头部进行引入：
```java
```java
var $ = require('jquery');
$('body').html('Hello Webpack!');
```
```
可以看到，这种以CommonJS的同步形式去引入其它模块的方式代码更加简洁。浏览器并不会实际的去同步加载这个模块，require的处理是由Webpack进行解析和打包的，浏览器只需要执行打包后的代码。Webpack自身已经可以完全处理JavaScript模块的加载，但是对于React中的JSX语法，这就需要使用Webpack的扩展加载器来处理了。
## Webpack开发服务器
除了提供模块打包功能，Webpack还提供了一个基于Node.js Express框架的开发服务器，它是一个静态资源Web服务器，对于简单静态页面或者仅依赖于独立服务的前端页面，都可以直接使用这个开发服务器进行开发。在开发过程中，开发服务器会监听每一个文件的变化，进行实时打包，并且可以推送通知前端页面代码发生了变化，从而可以实现页面的自动刷新。
Webpack开发服务器需要单独安装，同样是通过npm进行：
npm install -g webpack-dev-server
之后便可以运行webpack-dev-server命令来启动开发服务器，然后通过localhost:8080/webpack-dev-server/访问到页面了。默认情况下服务器以当前目录作为服务器目录。在React开发中，我们通常会结合react-hot-loader来使用开发服务器，因此这里不做太多介绍，只需要知道有这样一个开发服务器可以用于开发时的内容实时打包和推送。详细配置和用法可以参考[官方文档](http://webpack.github.io/docs/webpack-dev-server.html)。
## Webpack模块加载器（Loaders）
Webpack将所有静态资源都认为是模块，比如JavaScript，CSS，LESS，TypeScript，JSX，CoffeeScript，图片等等，从而可以对其进行统一管理。为此Webpack引入了加载器的概念，除了纯JavaScript之外，每一种资源都可以通过对应的加载器处理成模块。和大多数包管理器不一样的是，Webpack的加载器之间可以进行串联，一个加载器的输出可以成为另一个加载器的输入。比如LESS文件先通过less-load处理成css，然后再通过css-loader加载成css模块，最后由style-loader加载器对其做最后的处理，从而运行时可以通过style标签将其应用到最终的浏览器环境。
对于React的JSX也是如此，它通过jsx-loader来载入。jsx-loader专门用于载入React的JSX文件，Webpack的加载器支持参数，jsx-loader就可以添加?harmony参数使其支持ES6语法。为了让Webpack识别什么样的资源应该用什么加载器去载入，需要在配置文件进行配置：通过正则表达式对文件名进行匹配。例如：
```java
```java
module: {
    preLoaders: [{
      test: /\.js$/,
      exclude: /node_modules/,
      loader: 'jsxhint'
    }],
    loaders: [{
      test: /\.js$/,
      exclude: /node_modules/,
      loader: 'react-hot!jsx-loader?harmony'
    }, {
      test: /\.less/,
      loader: 'style-loader!css-loader!less-loader'
    }, {
      test: /\.(css)$/,
      loader: 'style-loader!css-loader'
    }, {
      test: /\.(png|jpg)$/,
      loader: 'url-loader?limit=8192'
    }]
  }
```
```
可以看到，该使用什么加载器完全取决于这里的配置，即使对于JSX文件，我们也可以用js作为后缀，从而所有的JavaScript都可以通过jsx-loader载入，因为jsx本身就是完全兼容JavaScript的，所以即使没有JSX语法，普通JavaScript模块也可以使用jsx-loader来载入。
加载器之间的级联是通过感叹号来连接，例如对于LESS资源，写法为style-loader!css-loader!less-loader。对于小型的图片资源，也可以将其进行统一打包，由url-loader实现，代码中`url-loader?limit=8192`含义就是对于所有小于8192字节的图片资源也进行打包。这在一定程度上可以替代[Css
 Sprites](https://developer.mozilla.org/en-US/docs/Web/Guide/CSS/CSS_Image_Sprites)方案，用于减少对于小图片资源的HTTP请求数量。
除了已有加载器，你也可以自己[实现自己的加载器](http://webpack.github.io/docs/how-to-write-a-loader.html)，从而可以让Webpack统一管理项目特定的静态资源。现在也已经有很多第三方的加载器实现常见静态资源的打包管理，可以参考Webpack主页上的[加载器列表](http://webpack.github.io/docs/list-of-loaders.html)。
## React开发神器：react-hot-loader
Webpack本身具有运行时模块替换功能，称之为[Hot Module Replacement](http://webpack.github.io/docs/hot-module-replacement.html) (HMR)。当某个模块代码发生变化时，Webpack实时打包将其推送到页面并进行替换，从而无需刷新页面就实现代码替换。这个过程相对比较复杂，需要进行多方面考虑和配置。而现在针对React出现了一个第三方[react-hot-loader](https://github.com/gaearon/react-hot-loader)加载器，使用这个加载器就可以轻松实现React组件的热替换，非常方便。其实正是因为React的每一次更新都是全局刷新的虚拟DOM机制，让React组件的热替换可以成为通用的加载器，从而极大提高开发效率。
要使用react-hot-loader，首先通过npm进行安装：
npm install —save-dev react-hot-loader
之后，Webpack开发服务器需要开启HMR参数hot，为了方便，我们创建一个名为server.js的文件用以启动Webpack开发服务器：
```java
```java
var webpack = require('webpack');
var WebpackDevServer = require('webpack-dev-server');
var config = require('../webpack.config');
new WebpackDevServer(webpack(config), {
  publicPath: config.output.publicPath,
  hot: true,
  noInfo: false,
  historyApiFallback: true
}).listen(3000, '127.0.0.1', function (err, result) {
  if (err) {
    console.log(err);
  }
  console.log('Listening at localhost:3000');
});
```
```
为了热加载React组件，我们需要在前端页面中加入相应的代码，用以接收Webpack推送过来的代码模块，进而可以通知所有相关React组件进行重新Render。加入这个代码很简单：
```java
```java
entry: [
  'webpack-dev-server/client?http://127.0.0.1:3000', // WebpackDevServer host and port
  'webpack/hot/only-dev-server',
  './scripts/entry' // Your appʼs entry point
]
```
```
需要注意的是，这里的client?[http://127.0.0.1:3000](http://127.0.0.1:3000/)需要和在server.js中启动Webpack开发服务器的地址匹配。这样，打包生成的文件就知道该从哪里去获取动态的代码更新。下一步，我们需要让Webpack用react-hot-loader去加载React组件，如上一节所介绍，这通过加载器配置完成：
```java
```java
loaders: [{
    test: /\.js$/,
    exclude: /node_modules/,
    loader: 'react-hot!jsx-loader?harmony'
  },
  …
]
```
```
做完这些配置之后，使用Node.js运行server.js：
node server.js
即可启动开发服务器并实现React组件的热加载。为了方便，我们也可以在package.json中加入一节配置：
```java
```java
"scripts": {
  "start": "node ./js/server.js"
}
```
```
从而通过npm start命令即可启动开发服务器。示例代码也上传在[Github](https://github.com/supnate/react-tab-selector)上，大家可以参考。
这样，React的热加载开发环境即配置完成，任何修改只要以保存，就会在页面上立刻体现出来。无论是对样式修改，还是对界面渲染的修改，甚至事件绑定处理函数的修改，都可以立刻生效，不得不说是提高开发效率的神器。
## 将Webpack开发服务器集成到已有服务器
尽管Webpack开发服务器可以直接用于开发，但实际项目中我们可能必须使用自己的Web服务器。这就需要我们能将Webpack的服务集成到已有服务器，来使用Webpack提供的模块打包和加载功能。要实现这一点其实非常容易，只需要在载入打包文件时指定完整的URL地址，例如：
`<script src="http://127.0.0.1:3000/assets/bundle.js"></script>`
这就告诉当前页面应该去另外一个服务器获得脚本资源文件，在之前我们已经在配置文件中指定了开发服务器的地址，因此打包后的文件也知道应该通过哪个地址去建立Socket IO来动态加载模块。整个资源架构如下图所示：
![](http://cdn2.infoqstatic.com/statics_s2_20160712-0252/resource/articles/react-and-webpack/zh/resources/0602006.jpg)
## 打包成多个资源文件
将项目中的模块打包成多个资源文件有两个目的：
- 
将多个页面的公用模块独立打包，从而可以利用浏览器缓存机制来提高页面加载效率；
- 
减少页面初次加载时间，只有当某功能被用到时，才去动态的加载。
Webpack提供了非常强大的功能让你能够灵活的对打包方案进行配置。首先来看如何创建多个入口文件：
```java
```java
{
  entry: { a: "./a", b: "./b" },
  output: { filename: "[name].js" },
  plugins: [ new webpack.CommonsChunkPlugin("init.js") ]
}
```
```
可以看到，配置文件中定义了两个打包资源“a”和“b”，在输出文件中使用方括号来获得输出文件名。而在插件设置中使用了CommonsChunkPlugin，Webpack中将打包后的文件都称之为“Chunk”。这个插件可以将多个打包后的资源中的公共部分打包成单独的文件，这里指定公共文件输出为“init.js”。这样我们就获得了三个打包后的文件，在html页面中可以这样引用：
```
<script src="init.js"></script>
<script src="a.js"></script>
<script src="b.js"></script>
```
除了在配置文件中对打包文件进行配置，还可以在代码中进行定义：require.ensure，例如：
```java
```java
require.ensure(["module-a", "module-b"], function(require) {
  var a = require("module-a");
  // ...
});
```
```
Webpack在编译时会扫描到这样的代码，并对依赖模块进行自动打包，运行过程中执行到这段代码时会自动找到打包后的文件进行按需加载。
## 小结
本文结合React介绍了Webpack的基本功能和用法，希望能让大家对这个新兴而强大的模块管理工具有一个总体的认识，并能将其应用在实际的项目开发中。笔者也将其应用在之前提供的[React示例组件](https://github.com/supnate/react-tab-selector)项目中，大家可以参考。除了这里介绍的功能，Webpack还有许多强大的特性，例如插件机制、支持动态表达式的require、打包文件的智能重组、性能优化、代码混淆等等。限于篇幅不再一一介绍，其[官方文档](http://webpack.github.io/docs/)也非常完善，需要时可以参考。
