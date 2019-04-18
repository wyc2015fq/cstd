# 手动创建基于webpack+babel的react应用(babel7) - weixin_33985507的博客 - CSDN博客
2019年02月20日 12:49:48[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
## 搭建一个React项目环境
首先`npm init`生成`package.json`文件.
至此我们就可以安装`react`的相关依赖了。
```
npm install react --save
npm install react-dom --save
```
安装完react相关，下一步安装`webpack`
```
npm install webpack --save-dev
npm install webpack-dev-server --save-dev //实现热加载
```
webpack安装完babel是重头戏！`babel`更新了7版本后，舍弃了以前的 babel-*-* 的命名方式，改成了@babel/-*-*的命名方式，通过试错后，将`常用依赖`，以及`.babelrc文件配置`在此列出以供参考。
```
//devDependencies下的依赖
npm install @babel/core --save-dev  //babel核心部分，把ES6转化成ES5
npm install @babel/plugin-transform-runtime --save-dev  //外部引用辅助函数和内置函数，自动填充代码而不会污染全局变量。
npm install @babel/preset-env --save-dev  //提供环境的映射获得插件列表并传递给babel
npm install @babel/preset-react --save-dev  //解析JSX
npm install babel-loader --save-dev  //webpack需要用到这个loader
//dependencies下的依赖
npm install @babel/polyfill --save  //转义es6的API
npm install @babel/runtime --save  //babel变换运行时插件的依赖
```
上述是babel的环境安装，下面来看下`.babelrc的文件配置`，在根目录下创建.babelrc文件
```
//.babelrc文件内容
{
"presets": ["@babel/preset-env", "@babel/preset-react"],
"plugins": ["@babel/plugin-transform-runtime"]
}
```
下面在根目录新建一个webpack.config.js文件，加入以下内容
```
var webpack = require('webpack');
module.exports = {
  entry: './src/app.js',
  output: {
      path: __dirname + '/build',
      filename: "bundle.js"},module: {
      rules: [
        { test: /\.js$/, exclude: /node_modules/, loader: "babel-loader" }
             ]      
      }
}
```
基本上配置内容都OK了！下面添加页面内容
## 添加页面内容
基本配置都有了，现在要往里面增加内容了，首先第一步在`根目录下创建index.html文件`，之后`在根目录创建src文件夹`用来放我们新增的组件。
入口文件我在webpack中配的`./src/app.js`，所以在src下新建app.js文件，添加我们的第一个组件。
```
//根目录下index.html文件内容
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <title>Document</title>
    </head>
    <body>
        <div id="app"></div>
        <script src="bundle.js"></script>
    </body>
</html>
//src下的app.js文件内容
import React from 'react';
import ReactDOM from 'react-dom';
class App extends React.Component {
render(){
  return <div>hello world</div>
    }
}
ReactDOM.render(<App />, document.getElementById('app'));
```
现在内容有了，我们把package.json中的script部分改成这样：
```
"scripts": {
    "start": "webpack-dev-server --hot --inline --mode development --colors --content-base",
    "build": "webpack --progress --colors --mode production"
}
```
大功告成啦！现在使用`npm start`启动你的项目吧！效果如图
![clipboard.png](https://image-static.segmentfault.com/309/774/3097741217-5c6d366d12251_articlex)
