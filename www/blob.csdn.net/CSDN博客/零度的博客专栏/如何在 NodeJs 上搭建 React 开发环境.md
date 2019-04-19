# 如何在 NodeJs 上搭建 React 开发环境 - 零度的博客专栏 - CSDN博客
2017年03月09日 11:49:59[零度anngle](https://me.csdn.net/zmx729618)阅读数：12781
**步骤：**
1. 安装 nodejs ：根据你的 Windows 类型(x86或x64)下载相应的[安装包](https://nodejs.org/en/)，安装完成后，在任一目录下打开 cmd 窗口并运行 node -v 命令，则会输出 node 的版本信息。
    --注：在 windows 操作系统中，如果把 nodejs 安装在系统盘（如：C盘），初始化时会提示 nodejs 没有操作文件的权限（无法新建 package.json 文件）。
   （建议在非系统盘中安装 nodejs ，本例安装路径：D:\Program Files）
2. 初始化 npm：安装 nodejs 时会默认安装 npm，在 cmd 窗口中运行 npm -v 命令会输出 npm 的版本信息。
    接着运行 npm init命令，并设置相关参数即完成 npm 初始化。这时 nodejs 会在当前目录下（这里是：D:\Program Files\nodejs 目录）新建一个 package.json 文件。    
3. 利用 npm 安装 react 相关的包：  
  3.1 运行 npm install react react-dom --save-dev 命令，安装 react 和 react-dom 包。  
其中 --save-dev 表示在 package.json 文件的 devDependencies 节点下添加包的引用，--save 表示在 package.json 文件的 dependencies 节点下添加包的引用
  3.1 运行 npm install webpack webpack-dev-server --save-dev 命令，安装 webpack 和 webpack-dev-server 包。
webpack 用来打包并压缩 js 文件，webpack-dev-server 用来实时同步修改过的 js，html，css 文件到浏览器。
  3.5 运行npm install jsx-loader --save-dev 命令，安装 jsx-loader 包（用来把按 ES5 规范编写的 .jsx 文件转成 .js 文件）。
       如果需要支持符合 ES6 规范的脚本文件（.js 或 .jsx），还需安装下面 4 个包：babel-core babel-loader babel-preset-es2015 babel-preset-react。
       如果需要用到路由，还需安装：react-router。
**配置 Webpack：**
1. Webpack 使用一个名为 webpack.config.js 的配置文件，可以手动添加一个 webpack.config.js 文件到 nodejs 根目录下，并按如下方式添加相关节点：
```
[]()
var path = require("path");
var webpack = require('webpack');
module.exports = {
  entry: { app: ['./app/main.jsx'] },
  output: {
    path: path.resolve(__dirname, "./build"),
    publicPath: "http://127.0.0.1:8080/build/",
    filename: "bundle.js"
  },
  resolve: {
    extensions: ['','.js','.jsx']
  },  
  module: {
    loaders: [
      {test:/\.jsx$/, loaders:['jsx?harmony']}
    ],
    //使用ES6时，才需要添加此loaders
    loaders:[
      {
        test: /\.jsx?$/,
        loader: 'babel',
        include: ROOT_PATH,
        query: {
          //添加两个presents 使用这两种presets处理js或者jsx文件
          presets: ['es2015', 'react']
        }
      }
    ]
  },
  devServer: {
    historyApiFallback: true,
    hot: true,
    inline: true,
    progress: true
  },
  plugins: [
    new webpack.DefinePlugin({
    'process.env.NODE.ENV':"development"
    }),
    new webpack.HotModuleReplacementPlugin()
  ]
};
[]()
```
resolve指定可以被 require 的文件后缀。比如 Hello.jsx 这样的文件就可以直接用 require(./Hello) 引用。
loaders指定 jsx-loader 编译后缀名为 .jsx 的文件，建议给含有 JSX 的文件添加 .jsx 后缀，当然你也可以直接使用 .js 后缀， 相应的 test 节点下的正则表达式也要修改。
Webpack 内置支持 CommonJS，所以可以直接用 npm 下载安装模块，然后直接 require 使用模块。
*  安装 React: npm install react --save    使用 React: var React = require('react');*
2. 修改 package.json 文件中的 scripts 节点为：
```
[]()
{
  "name": "nodejs",
  "version": "1.0.0",
  "description": "",
  "main": "",
  "dependencies": {
    "npm": "^3.10.8"
  },
  "devDependencies": {
    "jsx-loader": "^0.13.2",
    "react": "^15.3.2",
    "react-dom": "^15.3.2",
    "webpack": "^1.13.3",
    "webpack-dev-server": "^1.16.2",
    //如果需要支持ES6，还需添加以下引用
    "babel-core": "^6.18.2",
    "babel-loader": "^6.2.7",
    "babel-preset-es2015": "^6.18.0",
    "babel-preset-react": "^6.16.0",
  },
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1",
    "start": "webpack-dev-server --inline"
  },
  "author": "",
  "license": "ISC"
}
[]()
```
**添加项目文件：**
1. 添加 app/main.jsx，main.jsx 文件的内容如下：
```
var React = require('react');
var ReactDOM = require('react-dom');
var AppComponent = require('./components/Comment.jsx');
ReactDOM.render(<AppComponent />, document.getElementById('content'));
```
2. 添加 app/components/Comment.jsx， Comment.jsx 文件的内容如下：
```
[]()
var React = require('react');
var Comment = React.createClass({
    render: function(){
        return (
            <div className="comment">
                This is a React component!
            </div>
            );
    }
});
module.exports = Comment;
[]()
```
    这时的 app 文件目录如下：
![](http://images2015.cnblogs.com/blog/590980/201611/590980-20161109111004702-1904553947.png)
3. 添加 build/index.html，index.html 文件的内容如下：
```
[]()
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>React Test</title>
</head>
<body>
    <div id="content"></div>
    <script src="http://127.0.0.1:8080/build/bundle.js"></script>
</body>
</html>
[]()
```
**编译与运行：**
   先在 cmd 窗口中运行 npm run start 命令，然后在浏览器中输入：http://localhost:8080/build/index.html 来查看页面。如果修改了 js 文件，只需保存一下浏览器就会自动刷新。
**ES6的例子：**
1. 添加 app/app.js 文件的内容如下：
```
[]()
import React from 'react';
import ReactDOM from 'react-dom';
class App extends React.Component{
    constructor() {
        super();
    }
    render() {
        //JSX here!
        return (
          <div className="container">
            <section className="jumbotron">
              <h3 className="jumbotron-heading">Search Github Users</h3>
            </section>
          </div>
        )
    }
};
const app = document.createElement('div');
document.body.appendChild(app);
ReactDOM.render(<App />, app);
[]()
```
2. 添加 build/index.html，index.html 文件的内容如下：
```
[]()
<!DOCTYPE html>
<html>
<head>
    <title></title>
</head>
<body>
    <div id="content"></div>
    <script src="http://127.0.0.1:8080/build/bundle.js"></script>
</body>
</html>
[]()
```
**备注：**
    由于 Babel 可以转换 ES5 和 ES6 格式的脚本，所以可以在一个 .js 文件里，同时运用两种语法，如下所示：
```
[]()
import React from 'react';
import ReactDOM from 'react-dom';
const About = React.createClass({
  render(){
    return(<div>About</div>)
  }
})
const Inbox = React.createClass({
  render(){
    return(<div>Inbox</div>)
  }
})
const Home = React.createClass({
  render(){
    return(<div>Home</div>)
  }
})
const App = React.createClass({
  getInitialState() {
    return {
      route: window.location.hash.substr(1)
    }
  },
  componentDidMount() {
    window.addEventListener('hashchange', () => {
      this.setState({
        route: window.location.hash.substr(1)
      })
    })
  },
  render() {
    let Child
    switch (this.state.route) {
      case '/about': Child = About; break;
      case '/inbox': Child = Inbox; break;
      default:      Child = Home;
    }
    return (
      <div>
        <h1>App</h1>
        <ul>
          <li><a href="#/about">About</a></li>
          <li><a href="#/inbox">Inbox</a></li>
        </ul>
        <Child/>
      </div>
    )
  }
})
ReactDOM.render(<App />, document.body)
```
