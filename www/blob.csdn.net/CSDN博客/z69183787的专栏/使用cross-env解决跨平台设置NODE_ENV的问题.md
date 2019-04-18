# 使用cross-env解决跨平台设置NODE_ENV的问题 - z69183787的专栏 - CSDN博客
2017年01月06日 15:49:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：20638
在搭建公司新的前端工程的架构中，需要在在`package.json`的`scripts`标签下配置一系列命令，如下所示：
```
"scripts": {
    "clear": "rm -rf build&& mkdir build",
    "start": "npm run clear&& NODE_ENV=development webpack-dev-server --host 0.0.0.0 --devtool eval --progress --color --profile",
    "deploy": "npm run pre&& npm run clear&& NODE_ENV=production webpack -p --progress"
  },
```
上面配置中的的`&&`最开始使用的是`;`，后来发现`;`在windows环境中无法正常运行，于是改成了*unix和windows都兼容的`&&`。但是公司的部分使用windows的同事在运行
```
npm
 start
```
的时候，依然会报错：
```
'NODE_ENV' 不是内部或外部命令，也不是可运行的程序
或批处理文件。
npm ERR! Windows_NT 6.1.7601
npm ERR! argv "D:\\nodejs\\node.exe" "D:\\nodejs\\node_modules\\npm\\bin\\npm-cli.js" "start"
npm ERR! node v4.0.0-rc.5
npm ERR! npm  v2.14.2
npm ERR! code ELIFECYCLE
npm ERR! yy-ydh-web@1.0.7 start: `npm run clear&& NODE_ENV=development && webpack-dev-server --host 0.0.0.0 --devtool ev
al --progress --color --profile`
npm ERR! Exit status 1
npm ERR!
npm ERR! Failed at the yy-ydh-web@1.0.7 start script 'npm run clear&& NODE_ENV=development && webpack-dev-server --host
0.0.0.0 --devtool eval --progress --color --profile'.
npm ERR! This is most likely a problem with the yy-ydh-web package,
npm ERR! not with npm itself.
npm ERR! Tell the author that this fails on your system:
npm ERR!     npm run clear&& NODE_ENV=development && webpack-dev-server --host 0.0.0.0 --devtool eval --progress --color
 --profile
npm ERR! You can get their info via:
npm ERR!     npm owner ls yy-ydh-web
npm ERR! There is likely additional logging output above.
npm ERR! Please include the following file with any support request:
npm ERR!     D:\workspace\node_modules\yy-ydh-web\npm-debug.log
```
简单来说，就是windows不支持`NODE_ENV=development`的设置方式。
但是，又不想放弃这样简单方便的方式，于是只好求助于Google了。
### 解决方式
功夫不负有心人，在万能的google上，我找到了解决方法：[cross-env](https://www.npmjs.com/package/cross-env)。
这个迷你的包能够提供一个设置环境变量的scripts，让你能够以unix方式设置环境变量，然后在windows上也能兼容运行。
### 使用方法：
- 
安装across-env:
```
npm install cross-env
 --save-dev
```
- 
在`NODE_ENV=xxxxxxx`前面添加`cross-env`就可以了。
``set NODE_ENV=development && webpack --config webpack.config.dev.js``
也可以实现一样的效果
