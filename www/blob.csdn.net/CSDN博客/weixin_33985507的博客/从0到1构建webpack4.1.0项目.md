# 从0到1构建webpack4.1.0项目 - weixin_33985507的博客 - CSDN博客
2018年04月19日 16:49:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# 初始化项目
- 进入一个文件夹作为项目的根目录
- npm init
- 新建src, dist目录，package.json,webpack.config.js文件
- 安装项目需要的依赖
```
cnpm i webpack@4.1.0 webpack-cli@2.0.14 style-loader@0.21.0 css-loader@0.28.11  webpack-dev-server@3.1.3 -D
```
- 配置package.json里面的script命令
```
...
  "scripts": {
    "test": "webpack --mode development",//打包测试环境代码
    "build": "webpack --mode production" //打包开发生成环境代码
  },
...
```
- npm run dev 启动项目
# webpack配置文件
- webpack启动的时候，默认加载与package.json同级的webpack.config.js文件
- webpack详细的配置文件，里面涉及到的loader,插件；后面会详细的讲到
```
module.exports = {
    entry:"./src/index.js" ,
    output: {
        path:path.join(__dirname, 'dist'),
        filename:'[name].[hash:8].js'
    },
    module: {
        rules: [
            {
                test: /\.css$/,
                loader: ['style-loader','css-loader']
            },
            {
                test: /\.(png|jpg|gif|svg|bmp)/,
                use: {
                    loader: 'url-loader',
                    options:{
                        limit:9*1024,
                        //指定拷贝文件的输出目录
                        outputPath: 'images/'
                    }
                }
            },
            {
                test: /\.(html|htm)/,
                loader:'html-withimg-loader'
            },
            {
                test: /\.less$/,
                use: ['style-loader','css-loader', 'less-loader']
            },
            {
                test: /\.scss/,
                use: ['style-loader','css-loader', 'sass-loader']
            }
        ]
    },
    plugins: [
        new CleanWebpackPlugin([path.join(__dirname, 'dist')]),
        new HtmlWebpackPlugin({ 
            template: './src/index.html',
            filename: 'index.html',
            title: 'webpack',
            hash: true,
            chunks:['vendor', 'index'],
        }), 
    ],
    devServer: {
        contentBase: './dist',
        host: 'localhost',
        port:8080,
         //服务器返回给浏览器的时候是否启用gzip压缩
        compress: true,
    }
}
```
ps:
- entry值有三种情况
- 值为字符串，当值为字符串的时候，webpack会以该文件为入口文件分析其依赖文件，并且打包成chunk, 作为资源asset输出，默认为main.js
```
entry: './src/index.js',
```
2.值为数组，当值为数组的时候，webpack会以该数组文件依次分析里面的文件，并找出依赖模块打包成一个chunk,作为资源asset输出
```
entry: ['./src/index.js', './src/base.js']
```
- 值为对象， 当值为对象的时候，就是我们常说的多入口项目；webpack会先找到每个入口，从各个入口分别出发，找到每个入口依赖的模块，然后生成chunk(代码块),  每个chunk名称就是entry对应的key,将chunk写到文件系统中
```
entry: {
      index: './src/index.js',
      base: './src/base.js',
      vendor: 'jquery'
  },
```
- output 对象
- path:  输出的文件路径，必须是绝对路径
`path:path.join(__dirname, 'dist')`
- filename: 打包后的文件名，name名字如果是单入口默认是main, 如果是多入口是对应的Key名称；hash是根据打包之后的内容算出来的hash值，内容发生变化，hash也会变化，长度默认是20位，可以通过[hash:8]取出前8位
`filename:'[name].[hash:8].js'`
- module 对象
- rules 是一个数组， 用来自定义匹配的文件用什么Loader去处理
```
[{  
  test: /\.css$/,
  loader: ['style-loader','css-loader']
}]
```
- plugins 数组， 放入多个插件实例，用来处理相关逻辑
```
new CleanWebpackPlugin([path.join(__dirname, 'dist')]),打包前删除dist目录
```
- devServer对象
```
{
       open:true,//自动打开浏览器
       inline:true,//在打包后文件里注入一个websocket客户端，监听文件变化，自动刷新
       contentBase: './dist', //指定静态文件的根目录
       host: 'localhost',//配置主机
       port:8080, //配置端口号，默认8080
       compress: true, //服务器返回给浏览器的时候是否启用gzip压缩
   }
```
# 配置webpack静态服务器
- 安装webpack-dev-server
`cnpm i webpack-dev-server@3.1.3 -D`
- webpack.config.js文件配置
```
"scripts": {
    //开启一个静态服务器
    //--open 自动打开浏览器
    "dev": "webpack-dev-server  --mode development " 
  },
```
- package.json文件需要添加的配置
```
//配置静态服务器，可以预览打包后的项目
    devServer: {
        open:true,//自动打开浏览器
        //静态文件的根目录
        contentBase: './dist',
        //配置主机
        host: 'localhost',
        //配置端口号，默认8080
        port:8080,
        //服务器返回给浏览器的时候是否启用gzip压缩
        compress: true,
    }
```
- webpack-dev-server 打包后的文件是放在内存里面的，所以在硬盘里面是找不到这样的文件的
# 自动生成html文件
- 安装html-webpack-plugin插件
`cnpm i html-webpack-plugin@3.2.0`
- src目录下面添加一个模板文件index.html
- 配置package.json文件
```
plugins: [
        
        //此插件可以自动产出html文件，默认保存在output.path指定的目录下面
        new HtmlWebpackPlugin({
            
            //指定产出的html模板
            template: './src/index.html',
            //产出的html文件名称
            filename: 'index.html',
            //可以往模板里面传入自定义标题，模板默认使用ejs语法；所以可以在模板里面使用ejs语法，写入变量
            title: 'webpack',
            //会在插入的静态文件加上查询字符串，避免浏览器的缓存；如果是单入口文件的话，每次打包之后都是main.js，浏览器会有缓存，如果想去掉这个缓存的话，就可以加入查询字符串，避免浏览器的缓存
            hash: true,
            //在产出的html文件里面引入哪些代码块，里面的名字要跟entry里面key对应
       chunks:['index']
        })    
    ],
```
- 可以在模板文件里面以ejs语法写入传进去的变量
```
...
<head>
    <meta charset="UTF-8">
    <title><%= htmlWebpackPlugin.options.title %></title>
</head>
...
```
- 生成的目标html文件
```
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>webpack</title>
</head>
<body>
    
<script type="text/javascript" src="main.ef8b627c.js?ef8b627c68ea6ee379e1"></script></body>
</html>
```
# 构建的时候自动删除dist目录
- 安装clean-webpack-plugin插件
`cnpm i install clean-webpack-plugin@0.1.19 -D`
- 配置webpack.config.js插件
```
plugins: [
  //删除dist目录
  new CleanWebpackPlugin([path.join(__dirname, 'dist')]),
]
```
# 引入第三方模块，以jquery为例
- 如果两个模块分别依赖jquery，分别单独引入的话，会导致每个chunk都特别大
# 项目中图片的使用(css, js，html分别引入)
- 
css中引入图片
- 安装file-loader,url-loader
`cnpm i install file-loader@1.1.11 url-loader@1.0.1 -D`
- 配置webpack.config.js
```
{
     test: /\.(png|jpg|gif|svg|bmp)/,
     use: {
            loader: 'url-loader',
            options:{
            //9kb以内的图片处理成base64字符串
             limit:9*1024,
             //指定拷贝文件的输出目录
             outputPath: 'images/'
             }
          }
      }
```
- file-loader和url-loader两者关系: url-loader可以在文件比较小的时候，直接变成base64字符串内嵌到css文件里面，如果文件超过指定的限制，会调用file-loader; file-loader是解析图片地址，把图片从源位置拷贝到目标位置并修改源引用地址，可以处理任意的二进制数据，比如bootstrap里面的字体
- 
js中引入图片
- webpack.config.js配置与上面css中引入配置一样
- 使用
```
//会返回一个打包后的地址
let src = require('./images/thumb.png');
let img = new Image();
img.src = src;
document.body.appendChild(img);
```
- 
html 中引入图片
- 安装html-withimg-loader
`cnpm i html-withimg-loader -D`
- 配置webpack.config.js
```
{
     test: /\.(html|htm)/,
     loader:'html-withimg-loader'
 }
```
- 此插件可以将引入的源文件地址，更改为打包后的图片地址
# 项目中less,sass的使用
- 安装less, less-loader, node-sass sass-loader
`cnpm i less@3.0.4 less-loader@4.1.0 node-sass@4.9.0 sass-loader@7.0.1 -D`
- 配置webpack.config.js
```
{
      test: /\.less$/,
      use: ['style-loader','css-loader', 'less-loader']
   },
   {
     test: /\.scss/,
     use: ['style-loader','css-loader', 'sass-loader']
   }
```
# 提取css文件
- 安装extract-text-webpack-plugin插件
`cnpm i extract-text-webpack-plugin@next -D`
- 配置webpack.config.js
```
```
  //实例插件
 let cssExtract = new ExtractTextWebpackPlugin('css/css.css');//    
 传入的是打包后的文件名
 let lessExtract = new ExtractTextWebpackPlugin('css/less.css');
 let scssExtract = new ExtractTextWebpackPlugin('css/scss.css');
  ...
  //配置loader
   module: {
    rules: [
        {
            test: /\.css$/,
            use: cssExtract.extract({
                use: ['css-loader']
            })
        },
        {
            test: /\.less$/,
            use: lessExtract.extract({
                use: ['css-loader', 'less-loader']
            })
        },
        {
            test: /\.scss/,
            use: scssExtract.extract({
                use: ['css-loader', 'sass-loader']
            })
        }
    ]
},
  ...
  //配置插件
  plugins:[
     cssExtract,
     lessExtract,
     scssExtract
  ]
```
```
# 给css3添加前缀
- 安装postcss-loader autoprefix
`cnpm i postcss-loader autoprefixer -D`
- 配置webpack.config.js
```
{
     test: /\.less$/,
     use: lessExtract.extract({
          //最后面添加一个postcss-loader
           use: ['css-loader', 'less-loader', 'postcss-loader']
      })
 },
```
- 在根目录下面新建postcss.config.js,里面的内容如下
```
module.exports = {
    plugins: [require('autoprefixer')]
}
```
# 压缩css
- 配置webpack.config.js,给css-loader添加minimize参数
```
module:{
  rules:[
      {
           test: /\.less$/,
           use: lessExtract.extract({
                use: ['css-loader?minimize', 'less-loader', 'postcss-loader']
                })
        },
  ]
}
```
# 解析es6,es7代码
- 安装相关插件
```
cnpm i babel-loader@7.1.4 babel-core@6.26.3 babel-preset-env@1.6.1 babel-preset-stage-0@6.24.1 babel-preset-react@6.24.1 -D
```
- 配置webpack.config.js
```
{
   test: /\.jsx?$/,
    use:[
            {
                loader: 'babel-loader'
            }
         ],
        //只转换或者编译src目录下面的文件
        include: path.join(__dirname,'./src'),
        //不用解析node_modules
        exclude:/node_modules/
},
```
- 在package.json同级目录下面放入.babelrc文件，内容如下
```
{
  "presets":["env", "stage-0"]
}
```
# 压缩js代码
- 安装uglifyjs-webpack-plugin插件
`cnpm i uglifyjs-webpack-plugin -D`
- 配置webpack.config.js
```
const UglifyjsWebpackPlugin = require('uglifyjs-webpack-plugin');
...
plugins: [
   new UglifyjsWebpackPlugin(),
]
```
# webpack.config.js配置devtool定位源码错误位置
- `devtool:'source-map'`,//生成单独map文件，可以定位到哪一列出错了，定位的错误最准确, 但是体积最大
- `devtool:'cheap-module-source-map'`,//生成单独文件，体积更小，但只能定位到哪一行出错了
- `devtool:'eval-source-map'`,//不会生成单独文件，在文件的底部生成map文件进行映射，比较大
- `devtool:'cheap-module-eval-source-map'`,//不会生成单独文件，只能定位到行
# 拷贝文件
- 安装copy-webpack-plugin 插件
`cnpm i copy-webpack-plugin -D`
- 配置webpack.config.js
```
const CopyWebpackPlugin = require('copy-webpack-plugin');
...
plugins:[
  new CopyWebpackPlugin([{
        from:path.join(__dirname, 'src/public'),//源文件目录
        to:path.join(__dirname, 'dist/public')//目标目录
  }])
]
```
# 项目中引入bootstrap
- 安装bootstrap
`cnpm i bootstrap -D`
- 配置webpack.config.js
```
resolve:{
        //配置扩展名, 引入模块的时候可以不加扩展名,加载顺序从前往后
        extensions:[".js", ".json"],
        //配置别名，在js里面直接引用
        alias:{
            "bootstrap":"bootstrap/dist/css/bootstrap.css"
        }
 },
module:{
    rules:[
        {
          //匹配的文件添加eot|woff|woff2|ttf用来处理字体文件
           test: /\.(png|jpg|gif|svg|bmp|eot|woff|woff2|ttf)/,
           use: {
               loader: 'url-loader',
               options:{
                        limit:9*1024,
                        outputPath: 'images/'
                    }
                }
            },
    ]
}
```
- 项目中加载bootstrap
`require('bootstrap')`
- 引入的bootstrap因为加载的是以css为扩展，所以最终会被以下loader处理，文件最终会被提取到css.css文件里面，放到目标目录
```
{
   test: /\.css$/,
   use: cssExtract.extract({
         use: ['css-loader?minimize']
    })
},
```
# 项目中使用react
- 安装react react-dom babel-core babel-loader
`cnpm i react react-dom babel-core babel-loader -D`
- 配置webpack.config.js
```
{
 test: /\.jsx?$/,
 use:[
         {
              loader: 'babel-loader'
          }
        ]
},
```
- 在与package.json同级目录新建.babelrc文件，内容如下
```
{
  "presets":["env", "stage-0", "react"]
}
```
- 项目中使用
```
import React from 'react';
import ReactDom from 'react-dom';
ReactDom.render(<h1>hello world</h1>, document.getElementById('root'));
```
# (优化)配置resolve减少模块搜索范围
- 配置如下
```
resolve:{
        //加载模块的时候，会到下面指定的目录查找
        modules:['node_modules','./libs'],
        //当加载的模块是包的时候，找的入口文件是package.josn里面的main字段，如果没有会依次找browser,node字段; 主要是用来加载同一个api,但是区分不同的平台
        mainFields:['main', 'browser', 'node'],
        //配置扩展名, 引入模块的时候可以不加扩展名,加载顺序从前往后
        extensions:[".js", ".json"],
        //配置别名，在js里面直接引用
        alias:{
            //当加载第三方模块的时候，直接加载编译过后的代码，不会再去区分环境加载不同的代码
            react:'react/cjs/react.production.min.js',
            "bootstrap":"bootstrap/dist/css/bootstrap.css",
            vue: 'vue/dist/vue.js'
        }
    },
```
# (优化)module.noParse
- 配置此参数，模块中如果依赖了某第三方某块，webpack不会对此模块再进行依赖分析，提高了依赖分析速度
- 配置webpack.config.js
```
...
module:{
  noParse:[/react\.production\.min\.js/]
}
...
```
# DLL 动态链接库
- 
创建动态链接库用来包含其他模块要用到的函数和数据
- 
创建分两步
- 把基础模块独立出来打包到单独的动态链接库里面
- 当需要导入的模块在动态链接库里面的时候，模块不能再次被打包，而是去动态链接库里面去获取dll-plugin
- 
安装DLLPlugin， DLLReferencePlugin插件，这两个插件是webpack自带的
- DLLPlugin插件: 用于打包出一个动态链接库
- DLLReferencePlugin：在配置文件中引入DLLPlugin插件打包好的动态链接库
- 
具体的配置
- 创建打包dll的webpack.dll.config.js, 具体内容如下
```
const path = require('path');
const webpack = require('webpack');
/**
* target: '_dll_[name]' 指定的是导出变量的名称
*
*/
module.exports = {
entry: {
  //将react, react-dom打包到dll文件里面
    react: ['react', 'react-dom'],
},
output: {
    path: path.join(__dirname, 'dist'),//dll文件输出路劲
    filename: '[name]_dll.js',//输出动态链接库的名字，name取的是enter里面的react名称
    library: '_dll_[name]',//暴露成全局变量的名字,放到window上面;其他模块会从此变量上说去到里面的模块
    //这个属性和library是配合来使用的；默认值是var，就是将library对应的名称通过var声明(var _dll_[name]=...)
    libraryTarget: 'var'
},
plugins:[
    new webpack.DllPlugin({
        name: '_dll_[name]',//这个名字一定要与output.target保持一致，是在manifest文件中使用的
        path: path.join(__dirname, 'dist', 'manifest.json')//会生成一个描述文件，里面有个name属性，就是上面指定的
    })
]
}
```
- 配置package.json里面的打包脚本
```
...
   "scripts": {
    "build:react": "webpack --config webpack.react.config.js --mode production"
  },
...
```
- 配置webpack.config.js
```
const webpack = require('webpack');
...
plugins: [
     //这个插件指的是想在这个配置文件里面引入另一个动态链接库，会找到mainfest指定的文件，通过这个文件找到里面的name属性对应的动态链接库,当加载库的时候，会到这个动态链接库里面看有没有，有的话直接加载，不需要编译，加快了编译速度；
     // 当在业务模块里面引入react的时候，会先找manifest.json文件,看看里面有没有react模块，有的话，会找到对应的name属性(_dll_[name])，然后找到window._dll_[name]模块，从而拿到里面的编译过后的react(之所以能拿到是因为动态链接库编译了react，并且放到了window._dll_[name]属性上了),所以不用再次打包react，减少了编译的时间
      new webpack.DllReferencePlugin({
          manifest: path.join(__dirname, 'dist', 'manifest.json')
      }),
]
```
# 区分正式测试环境
# cdn设置
- HTML文件不缓存，放在自己的服务器上，关闭自己服务器的缓存，静态资源的URL变成指向CDN服务器的地址
- 静态的JavaScript、CSS、图片等文件开启CDN和缓存，并且文件名带上HASH值
- 为了并行加载不阻塞，把不同的静态资源分配到不同的CDN服务器上
```
output: {
    path: path.resolve(__dirname, 'dist'),
    filename: '[name]_[hash:8].js',
   publicPath: 'http://baidu.com'//生成的静态资源的前缀
}
```
# package.json设置脚本运行的环境变量
- window系统设置是通过
```
"test": "set NDOE_ENV=development && webpack --mode development"
```
- mac系统设置是通过
```
"test": "export NDOE_ENV=development && webpack --mode development"
```
3.如果统一的话 安装包`cnpm install cross-env -D` 并且通过以下方式设置环境变量
```
"test": "crocess_env NDOE_ENV=development && webpack --mode development"
```
ps： 以上设置的环境变量在webpack的配置文件中是可以直接通过`process.env.NODE_ENV`来获取到的
# 源代码中区分测试还是正式环境
- 配置plugin
```
...
plugins:[
  new webpack.DefinePlugin({
      'process.env':{
           NODE_ENV: JSON.stringify(process.env.NODE_ENV),
           PRODUCT_NAEM: JSON.stringify(process.env.PRODUCT_NAME)
       }
  })
]
...
```
- 源码中使用方式
```
if(process.env.NODE_ENV === 'development') {
}
```
# 模块热替换
- 模块热替换(hot module replacement)的技术可以在不刷新整个网页的情况下只更新指定的模块，原理是当一个源码发生变化时，只重新编译发生变化的模块，再用新输出的模块替换掉浏览器中老的模块
- 编译更快，需要的时间更短
- 不刷新网页可以保留网页运行状态
- webpack.config.js配置
```
const webpack = require('webpack')
...
devServer: {
    hot: true
}
plugins:[
  ...
  new webpack.HotModuleReplacementPlugin(),
  new webpack.NamedModulesPlugin()
]
```
- 源码中使用方式
```
if(module.hot) {
    //如果检测到了base更新了，则执行这个函数
    module.hot.accept('./base', function() { //如果这个文件发生变化了，则执行这个函数，不会刷新页面了
        
    }) 
}
```
# Tree Shaking
ps: 可以剔除js中不用的代码；依赖静态的es6模块化语法，比如通过import 和export 导入导出
- 比如引入一个公共库，里面有两个方法，引入到业务代码里面，只用了其中的一个，用这种方式，可以把没有用到的死代码剔除掉，减少包的大小
- 没有使用Tree-Shaking之前
```
//info.js
export function getName() {
    return 'tree-shaking'
}
export function getAge() {
    return '2.0'
}
//index.js
import { getName } from './info.js'
let name = getName;
console.log(name);
//main.b78eb869.js 打包之后的代码
(function(module, exports, __webpack_require__) {
"use strict";
eval("\n\nObject.defineProperty(exports, \"__esModule\", {\n    value: true\n});\nexports.getName = getName;\nexports.getAge = getAge;\n\nfunction getName() {\n    return 'tree-shaking';\n}\n\nfunction getAge() {\n    return '2.0';\n}\n\n//# sourceURL=webpack:///./src1/info.js?");
/***/
```
ps: index.js并没有使用getAge方法，但是在打包之后的代码里面还是有getAge这个方法的
- 我们希望的是没用用到的代码剔除
- webpack.config.js配置如下
```
module:{
        rules:[
            {
                test: /\.js$/,
                use:{
                    loader:'babel-loader',
                    options:{
                        presets:[
                            //env模块编译es6语法，但是不编译module语法
                            ["env", {modules:false}]//含义是关闭 Babel 的模块转换功能，保留原本的 ES6 模块化语法
                        ]
                    }
                },
                include:path.resolve('./src'),//转义该目录下的文件
                exclude:/node_modules/ //该目录下的文件转成es5
            }
        ]
   },
```
- 再次编译，会发现getName已经不见了
# 提取公共代码
- 为什么要提取公共代码呢
- 相同的资源被重复的加载，浪费用户的流量和服务器的成本；
- 每个页面需要加载的资源太大，导致网页首屏加载缓慢，影响用户体验。如果能把公共代码抽离成单独文件进行加载能进行优化，可以减少网络传输流量，降低服务器成本
- 如何提取
- 基础类库，长期缓存
- 页面之间的公用代码
- 各个页面单独生成文件
# es6 里面class属性的转换
- 安装插件
```
npm install --save-dev babel-plugin-transform-class-properties
```
2.配置.babelrc
```
// without options
{
  "plugins": ["transform-class-properties"]
}
// with options
{
  "plugins": [
    ["transform-class-properties", { "spec": true }]
  ]
}
```
# webpack概念
- 
功能
- 代码转换：TypeScript 编译成 JavaScript、SCSS 编译成 CSS 等
- 文件优化：压缩 JavaScript、CSS、HTML 代码，压缩合并图片等。
- 代码分割：提取多个页面的公共代码、提取首屏不需要执行部分的代码让其异步加载。
- 模块合并：在采用模块化的项目里会有很多个模块和文件，需要构建功能把模块分类合并成一个文件。
- 自动刷新：监听本地源代码的变化，自动重新构建、刷新浏览器。
- 代码校验：在代码被提交到仓库前需要校验代码是否符合规范，以及单元测试是否通过。
- 自动发布：更新完代码后，自动构建出线上发布代码并传输给发布系统。
- 
执行原理
Webpack 启动后会从Entry里配置的Module开始递归解析 Entry 依赖的所有 Module。 每找到一个 Module， 就会根据配置的Loader去找出对应的转换规则，对 Module 进行转换后，再解析出当前 Module 依赖的 Module。 这些模块会以 Entry 为单位进行分组，一个 Entry 和其所有依赖的 Module 被分到一个组也就是一个 Chunk。最后 Webpack 会把所有 Chunk 转换成文件输出。 在整个流程中 Webpack 会在恰当的时机执行 Plugin 里定义的逻辑。
