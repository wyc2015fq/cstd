# 30分钟手把手教你学webpack实战 - z69183787的专栏 - CSDN博客
2017年01月06日 15:08:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：616
http://www.cnblogs.com/Leo_wl/p/4793722.html
**阅读目录**
- [一：什么是webpack? 他有什么优点？](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe0)
- [二：如何安装和配置](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe1)
- [三：理解webpack加载器](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe2)
- [四：理解less-loader加载器的使用](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe3)
- [五：理解babel-loader加载器的含义](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe4)
- [六：了解下webpack的几个命令](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe5)
- [七：webpack对多个模块依赖进行打包](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe6)
- [八：如何独立打包成样式文件](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe7)
- [九：如何打包成多个资源文件](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe8)
- [十：关于对图片的打包](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe9)
- [十一：React开发神器：react-hot-loader](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labe10)
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
什么是webpack? 他有什么优点？
    首先对于很多刚接触webpack人来说，肯定会问webpack是什么？它有什么优点？我们为什么要使用它？带着这些问题，我们来总结下如下：
    Webpack是前端一个工具，可以让各个模块进行加载，预处理，再进行打包，它能有Grunt或Gulp所有基本功能。优点如下：
- 支持commonJS和AMD模块。
- 支持很多模块加载器的调用，可以使模块加载器灵活定制，比如babel-loader加载器，该加载器能使我们使用ES6的语法来编写代码。
- 可以通过配置打包成多个文件，有效的利用浏览器的缓存功能提升性能。
- 使用模块加载器，可以支持sass，less等处理器进行打包且支持静态资源样式及图片进行打包。
- 更多等等。。。带着这些问题我们慢慢来学习webpack。
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
二：如何安装和配置
首先我的项目目录结构是：文件名叫webpack，里面只有一个main.html，代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!doctype html>
<html lang="en">
 <head>
  <meta charset="UTF-8">
  <title>Document</title>
  <script src="src/react.min.js"></script>
 </head>
 <body>
    <div id="content"></div>
    <script src="build/build.js"></script>
 </body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
还有一个文件夹src，该文件夹存放了二个js文件；react.min.js源文件和main.js文件，main.js源码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/* 内容区模块代码 */
var ContentMode = React.createClass({
        render: function(){
            return (
                <div className="ContentMode">
                    <div class="contents">{this.props.contents}</div>
                    {this.props.children}
                </div>
            )
        }
});
/* 页面div封装 上面三个模块 */
var Page = React.createClass({
    render: function(){
        return (
            <div className="homepage">
                <ContentMode  contents ="longen">this is one comment</ContentMode >
                <ContentMode  contents ="longen2">this is two comment</ContentMode >
            </div>
            )
        }
});
/* 初始化到content容器内 */
React.render(
       React.createElement(Page,null),document.getElementById("content")
);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
该代码是React.js代码，是react.js入门学习一中的代码复制过来的 为了演示；
**安装步骤如下：**
- **生成package.json文件；**
首先我们需要在根目录下生成package.json文件，需要进入项目文件内根目录下执行如下命令：npm init
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220019122-1508746048.png)
如上通过一问一答的方式后会在根目录下生成package.json文件，如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220049169-144209595.png)
**2 . 通过全局安装webpack**
执行命令如下：npm install -g webpack 如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220147028-1005405131.png)
在c盘下会生成node_modules文件夹中会包含webpack，此时此刻我们可以使用webpack命令了；
**3. 配置webpack**
每个目录下都必须有一个webpack.config.js，它的作用就好比Gulpfile.js、或者 Gruntfile.js，就是一个项目配置，告诉webpack需要做什么。
如下是我的webpack.config.js代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
module.exports = {
  entry: "./src/main.js",
  output: {
    filename: "build/build.js"
  },
  module: {
    loaders: [
       //.css 文件使用 style-loader 和 css-loader 来处理
      { test: /\.css$/, loader: "style!css" },
      //.js 文件使用 jsx-loader 来编译处理
      { test: /\.js$/,    loader: "jsx-loader" }
    ]
  },
  resolve: {
    extensions: ['', '.js', '.jsx']
  },
  plugins: []
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**entry** 是页面中的入口文件，比如我这边的入口文件时main.js
**output:** 是指页面通过webpack打包后生成的目标文件放在什么地方去，我这边是在根目录下生成build文件夹，该文件夹内有一个build.js文件；
**resolve:** 定义了解析模块路径时的配置，常用的就是extensions; 可以用来指定模块的后缀，这样在引入模块时就不需要写后缀，会自动补全。
**plugins: **定义了需要使用的插件，比如commonsPlugin在打包多个入口文件时会提取公用的部分，生成common.js;
**module.loaders：**是文件的加载器，比如我们之前react需要在页面中引入jsx的js源码到页面上来，然后使用该语法，但是通过webpack打包后就不需要再引入JSXTransformer.js；看到上面的加载器；比如jsx-loader加载器就是代表JSXTransformer.js的，还有style-loader和css-loader加载器；因此在使用之前我们需要通过命令把它引入到项目上来；因此需要如下命令生成下；
jsx-loader加载器 **npm install jsx-loader --save-dev** 如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220321215-1214366069.png)
Style-loader加载器 **npm install style-loader --save-dev** 如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220358669-1701729615.png)
css-loader 加载器 **npm install css-loader --save-dev** 如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220433059-1255280636.png)
局部安装webpack 执行命令：npm install webpack --save-dev
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220455215-439366299.png)
我们这边干脆把gulp的全局安装和在项目中局部安装也安装下，稍后有用~
**Gulp全局安装 npm install -g gulp 如下：**
**![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220529512-23131625.png)**
在项目文件内，gulp局部安装 使用命令 npm install gulp --save-dev 如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220551106-414560392.png)
因此在我们文件夹node_modules下生成文件如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220625153-1596231582.png)
现在我们来执行命令 webpack; 如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220741544-2118808707.png)
 即可在根目录下生成一个build文件夹中build.js 如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220906825-1674695684.png)
我们还可以使用如下命令：webpack --display-error-details 命令执行，这样的话方便出错的时候可以查看更详尽的信息；比如如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908220931544-1044036121.png)
现在我们再来刷新下页面；看到如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908221025981-2013963460.png)
可以看到页面渲染出来了，我们接着来看看页面中的请求：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908221045012-478863424.png)
可以看到只有一个文件react.min.js的源文件和build.js 我们刚刚生成的build.js文件了，因此我们通过webpack进行打包后，我们现在就不再需要和以前一样引入JSXTransformer.js了。我们还可以看看build.js内生成了那些js，这里就不贴代码了，自己可以看看了~
上面是使用webpack打包；现在我们再来看看使用第二种方案来打包~
**使用gulp来进行打包**
我们知道使用gulp来打包的话，那么我们需要在根目录下需要新建 Gulpfile.js；
因此我们这边Gulpfile.js的源码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
var gulp = require('gulp');
var webpack = require("gulp-webpack");
var webpackConfig = require("./webpack.config.js");
gulp.task('webpack', function () {
    var myConfig = Object.create(webpackConfig);
    return gulp
        .src('./src/main.js')
        .pipe(webpack(myConfig))
        .pipe(gulp.dest('./build'));
});
// 注册缺省任务
gulp.task('default', ['webpack']);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**然后webpack.config.js代码变为如下：**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
module.exports = {
  entry: "./src/main.js",
  output: {
    filename: "build.js"
  },
  module: {
    loaders: [
       //.css 文件使用 style-loader 和 css-loader 来处理
      { test: /\.css$/, loader: "style!css" },
      //.js 文件使用 jsx-loader 来编译处理
      { test: /\.js$/,    loader: "jsx-loader" }
    ]
  },
  resolve: {
    extensions: ['', '.js', '.jsx']
  },
  plugins: []
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
即可，然后再在命令行中输入gulp即可生成build/build.js了；如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908221206637-1147797598.png)
Github上的代码如下： [https://github.com/tugenhua0707/webpack/](https://github.com/tugenhua0707/webpack/)  自己可以把压缩包下载下来运行下即可。
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
三：理解webpack加载器
    Webpack提供了一套加载器，比如css-loader,less-loader,style-loader，url-loader等，用于将不同的文件加载到js文件中，比如url-loader用于在js中加载png/jpg格式的图片文件，css/style loader用于加载css文件，less-loader加载器是将less编译成css文件；
**配置加载器**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
module.exports = {
  entry: "./src/main.js",
  output: {
    filename: "build.js",
    path: __dirname + '/assets/',
    publicPath: "/assets/"
  },
  module: {
    loaders: [
      {test: /.css$/, loader: 'style!css'},
      {test: /.(png|jpg)$/, loader: 'url-loader?limit=8192'}
    ]
  }
  resolve: {
extensions: ['', '.js', '.jsx'],
//模块别名定义，方便后续直接引用别名，无须多写长长的地址
alias: {
    a : 'js/assets/a.js',  // 后面直接引用 require(“a”)即可引用到模块
    b : 'js/assets/b.js',
    c : 'js/assets/c.js'
}
  },
  plugins: [commonsPlugin, new ExtractTextPlugin("[name].css")]
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
**module.loader: **其中test是正则表达式，对符合的文件名使用相应的加载器. 
/.css$/会匹配 xx.css文件，但是并不适用于xx.sass或者xx.css.zip文件.
**url-loader** 它会将样式中引用到的图片转为模块来处理; 配置信息的参数“?limit=8192”表示将所有小于8kb的图片都转为base64形式。
**entry** 模块的入口文件。依赖项数组中所有的文件会按顺序打包，每个文件进行依赖的递归查找，直到所有模块都被打成包；
**output**：模块的输出文件，其中有如下参数：
**filename:** 打包后的文件名
**path: **打包文件存放的绝对路径。
**publicPath: **网站运行时的访问路径。
**relolve.extensions:** 自动扩展文件的后缀名，比如我们在require模块的时候，可以不用写后缀名的。
**relolve.alias: **模块别名定义，方便后续直接引用别名，无须多写长长的地址
**plugins** 是插件项;
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
四：理解less-loader加载器的使用
    我们先来理解下less-loader加载器，其他的sass-loader也是一个意思，这边不会对所有的预处理的css做讲解，less-loader加载器是把css代码转化到style标签内，动态插入到head标签内；我们先来看看我项目的结构如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908221647590-892331763.png)
我们现在css文件下有一个main.less 代码如下：
```
@base: #f938ab;
html,body {
  background:@base;
}
```
Src文件下有一个main.js文件 此js文件时入口文件；里面的代码如下：
// css
require('../css/main.less');
**webpack.config.js 代码配置如下：**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
module.exports = {
  entry: "./src/main.js",
  output: {
    filename: "build.js",
    path: __dirname
  },
  module: {
    loaders: [
      //.css 文件使用 style-loader 和 css-loader 来处理
      {
        test: /\.less$/,
        loader: "style!css!less"
      }
    ]
  },
  resolve: {
    extensions: ['', '.js', '.jsx']
  },
  plugins: []
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
Gulpfile.js代码如下(注意：这边既可以需要此文件使用gulp进行运行打包，也可以不需要此文件，直接使用webpack进行打包；二种方式任选其一)。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
var gulp = require('gulp');
var webpack = require("gulp-webpack");
var webpackConfig = require("./webpack.config.js");
gulp.task('webpack', function () {
    var myConfig = Object.create(webpackConfig);
    return gulp
        .src('./src/main.js')
        .pipe(webpack(myConfig))
        .pipe(gulp.dest('./build'));
});
// 注册缺省任务
gulp.task('default', ['webpack']);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
因此我们需要安装 style-loader css-loader 和 less-loader 如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908221858247-18352165.png)
 安装完成后，我们查看我们的项目的根目录node_modules下多了如下几个文件：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222005372-523132569.png)
如上配置后，我们进入项目后 运行下 gulp或者 webpack命令既可，在build文件夹内会生成build.js，此JS是动态生成style标签并解释正常的css插入到文档head标签内；我们可以运行下页面，查看代码看下如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222028387-1432933008.png)
因此我们可以看到页面生效了；为了更好的demo测试，我把代码放到如下github上，自己可以下载下来运行下既可：[https://github.com/tugenhua0707/webpack-less-loader](https://github.com/tugenhua0707/webpack-less-loader)
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
五：理解babel-loader加载器的含义
babel-loader加载器能将ES6的代码转换成ES5代码，这使我们现在可以使用ES6了；我们在使用之前，我们需要安装babel-loader
执行命令：npm install babel-loader –save-dev 如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222348153-234885443.png)
如上安装完后，我们在根目录node_modules会生成文件，如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222438700-652578965.png)
现在我们可以在webpack.config.js里面moudle.loaders配置加载器了，如下代码：
{test: /\.js$/, loader: 'babel', exclude: '/node_modules/'} 
因此webpack.config.js代码变成如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// 使用webpack打包
module.exports = {
  entry: "./src/main.js",
  output: {
    filename: "build.js",
    path: __dirname
  },
  module: {
    loaders: [
      {test: /\.js$/, loader: 'babel', exclude: '/node_modules/'}
    ]
  },
  resolve: {
    extensions: ['', '.js', '.jsx']
  },
  plugins: []
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
下面我们再来看看我项目中的目录结构如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222520872-1631891807.png)
我们在看看src源文件有下面几个文件
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222535919-1204211354.png)
React.min.js是react源码，这个不多说，bind.js的ES6的代码如下：
```
// es6的语法 let LOADER = true; module.exports = LOADER;
```
main.js 是页面的入口文件；代码如下：
```
let loader = require('./bind');
console.log(loader);
```
let是ES6的语法 相当于js中的var定义变量的含义； 接着打印下bind模块中 打印为true；
最后执行gulp如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222658215-1321304567.png)
在控制台中打印true；我把源码放在github上，有需要的同学可以自己下载下来运行下即可；如下github(我2年没有使用github，现在重新使用，为了更好的演示demo问题)； [https://github.com/tugenhua0707/webpack-babel-loader](https://github.com/tugenhua0707/webpack-babel-loader)
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
六：了解下webpack的几个命令
- webpack         // 最基本的启动webpack的方法
- webpack -w      // 提供watch方法；实时进行打包更新
- webpack -p      // 对打包后的文件进行压缩
- webpack -d      // 提供source map，方便调式代码
我们下面来了解下 webpack -w
如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222839340-45970821.png)
比如我在js文件里面随便增加一点代码后，保存后，再刷新页面即可可以看到代码生效了，无需重新运行webpack或者gulp，使用webpack -w 可以实时打包。 webpack -p 的含义是对进行打包后的文件进行压缩代码；比如我在之前使用chrome看打包后的代码如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222928372-1217413568.png)
如上可以看到，代码是未压缩的，但是当我在控制台命令行中运行 webpack -p 命令后，如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908222944231-724173994.png)
我们现在再到控制台上看下代码变成已经压缩后的代码了，如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223013809-1728999862.png)
webpack  -d 是提供未压缩之前的源码 方便代码中的调式；如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223031122-1360480966.png)
当我运行如上所示后，我们再来看看刚才已经压缩后的代码变成什么样子呢？如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223102153-1072723036.png)
如上代码可以看到 我们进行压缩后的代码，通过运行 webpack -d 命令后，即可还原未压缩的代码，这样的话就可以方便我们线上调式代码了。
我们再来看看目录下 会生成map文件，如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223121403-1121006817.png)
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
七：webpack对多个模块依赖进行打包
   通过一刚开始我们了解到 webpack支持commonJS和AMD两种模块机制进行打包，因此我们现在来针对代码中使用commonJS和AMD机制进行做一个demo；
Src源文件增加module1.js module2.js module3.js 代码分别如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
module1.js 代码：
// module1.js
require(["./module3"], function(){
    console.log("Hello Webpack!");
});
Module2.js代码如下：
// module2.js，使用的是CommonJs机制导出包
module.exports = function(a, b){
    return a + b;
}
Module3.js代码使用AMD机制
// module3.js，使用AMD模块机制
define(['./module2.js'], function(sum){
    return console.log("1 + 2 = " + sum(1, 2));
});
 // 入口文件 main.js 代码如下：
 require("./module1");
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
我们可以运行下 webpack后 在根目录下生成如下文件：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223306325-1465554481.png)
其中1.build文件夹是commonJS生成的 里面是commonJS的代码；我们再查看页面的代码如下可以看到：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223345044-497932462.png)
我们继续查看控制台输出如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223427497-609742465.png)
为止我们可以看到webpack打包可以支持commonJS模块和AMD模块。
具体的代码 可以查看我的github上的源码：
[https://github.com/tugenhua0707/webpack-multi-module-depend](https://github.com/tugenhua0707/webpack-multi-module-depend)
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
八：如何独立打包成样式文件
    有时候我们不想把样式打在脚本中，而是想独立css出来，然后在页面上外链css，这时候我们需要 [extract-text-webpack-plugin](https://github.com/webpack/extract-text-webpack-plugin) 来帮忙：我们首先需要安装 [extract-text-webpack-plugin](https://github.com/webpack/extract-text-webpack-plugin)：如下： npm install extract-text-webpack-plugin –save-dev 如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223650309-1576271915.png)
然后在目录下会生成如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223723559-2037170259.png)
现在我们需要看看webpack.config.js 配置变成如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
var ExtractTextPlugin = require("extract-text-webpack-plugin");
// 使用webpack打包
module.exports = {
  entry: "./src/main.js",
  output: {
    filename: "build.js"
  },
  module: {
    loaders: [
      //.css 文件使用 style-loader 和 css-loader 来处理
      {
        test: /\.less$/,
        loader: ExtractTextPlugin.extract(
            'css?sourceMap!' +
            'less?sourceMap'
        )
      }
    ]
  },
  resolve: {
    extensions: ['', '.js', '.jsx']
  },
  // 内联css提取到单独的styles的css
  plugins: [new ExtractTextPlugin('styles.css')]
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
配置完成后 我们gulp运行下即可，在build文件夹内会生成2个文件，一个是build.js 处理模块的文件 另一个就是我们的styles.css了；我们查看下如下所示：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223819512-912869379.png)
接着在html文件这样引入即可：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<!doctype html>
<html lang="en">
 <head>
  <meta charset="UTF-8">
  <title>Document</title>
  <script src="src/react.min.js"></script>
  <link rel="stylesheet" href="build/styles.css"/>
 </head>
 <body>
    <div id="content"></div>
    
 </body>
</html>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
在页面上运行以下；即可看到效果：我们可以看下请求数：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908223911450-328400557.png)
具体的代码demo可以看我的github 如下：
[https://github.com/tugenhua0707/extract-text-webpack-plugin](https://github.com/tugenhua0707/extract-text-webpack-plugin)
注意：node_modules模块没有上传上去，git上传不上去，老是提示Filename too long的错误，所以就没有上传，需要自己在本地安装如下模块：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908224033794-375056364.png)
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
九：如何打包成多个资源文件
    我们在开发页面的时候，有时候需要有多个入口文件，做到文件是按需加载，这样就可以使用缓存提升性能；那么我们接下来需要如何配置呢？现在我们继续做demo，现在比如我现在的项目文件结构如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908224054465-258751793.png)
我们直接看 webpack.config.js配置代码变成如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
module.exports = {
  entry: {
     "main": "./src/main.js",
     "index": "./src/index.js"
  },
  output: {
    filename: "[name].bundle.js"
  }
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
从上面的配置代码我们可以看到 entry现在变成了一个对象了，而对象名也就是key会作为下面output的filename属性的[name]。当然entry也可以是一个数组。
因此我们直接 gulp运行下即可 在build文件下 生成2个入口文件 如上面的截图所示：github源码地址如下：
[https://github.com/tugenhua0707/webpack-many-page](https://github.com/tugenhua0707/webpack-many-page)
现在我们可以根据不同的页面 引入不同的入口文件，实现按需加载文件。
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
十：关于对图片的打包
我们知道图片是使用url-loader来加载的，我们既可以在css文件里url的属性；如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
#content{
    width:170px;
    height:60px;
    background:url('../images/1.jpg') no-repeat;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
我们还可以直接对元素的src属性进行require赋值。如下代码：
```
var img = document.createElement("img"); 
img.src = require("../image/1.jpg"); 
document.body.appendChild(img);
```
我这边直接来讲第一种在css文件里的url属性进行打包；
首先来看看我项目的目录结构如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908224334840-11310186.png)
Css文件 main.css代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
#content{
    width:170px;
    height:60px;
    background:url('../images/1.jpg') no-repeat;
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
JS文件main.js代码如下：
```
require('../css/main.css');
```
Webpack.config.js配置文件代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
// 使用webpack打包
module.exports = {
  
  entry: {
     "main": "./src/main.js"
  },
  output: {
    path: './build/',
    filename: "build.js"
  },
  module: {
    loaders: [
      {test: /.css$/, loader: 'style!css'},
      {test: /.(png|jpg)$/, loader: 'url?limit=8192'}
    ]
  }
};
![复制代码](http://common.cnblogs.com/images/copycode.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
直接运行webpack 可以生成build目录，build目录下会生成2个文件 一个是图片打包后，另外一个是build.js。接着我们再在页面运行下页面，发现有一个问题，如下：
![](http://images2015.cnblogs.com/blog/561794/201509/561794-20150908224518934-1877178352.png)
页面调用图片的url是根目录下的，不是我打包后的 build文件夹下，所以会导致图片路径找不到的问题；因此这边有一点点没有完成的任务，希望有兴趣的童靴可以帮助完成~ 不过图片确实是已经打包好了，为了方便，我们还是提供github源码吧！如下所示：
[https://github.com/tugenhua0707/webpack-url-loader](https://github.com/tugenhua0707/webpack-url-loader)
[回到顶部](http://www.cnblogs.com/tugenhua0707/p/4793265.html#_labelTop)
十一：React开发神器：react-hot-loader
 待续..... 由于篇幅有限~~  这个留给下篇文章讲解。
