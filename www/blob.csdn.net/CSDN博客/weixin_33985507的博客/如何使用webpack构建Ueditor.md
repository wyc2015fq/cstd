# 如何使用webpack构建Ueditor - weixin_33985507的博客 - CSDN博客
2018年08月31日 07:02:31[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
由于种种原因，我们选择了Ueditor作为我们的富文本编辑器选型。
Ueditor不支持模块化，所以无法在代码中使用import去引入。一开始我们在项目中是将Ueditor的js文件直接通过script标签引入，在React的代码里直接使用window.UE去使用编辑器。但是这就有一个问题，我们对UE的源码进行了改动，加入了定制化的功能。而直接引入的UE文件在浏览器是有缓存的，我们每次改动都要清除缓存才能生效。
我们要解决缓存这个问题，webpack配置就必须满足以下条件：
- 每次改动代码后，能自动给UE的文件名加hash
- 能自动插入html模板文件并在主入口文件加载之前加载完成
## 第一步
为了能让UE的文件进入打包流程，我们将它作为一个新的入口文件
```
const entry = {
    main: ['babel-polyfill', './src/main.js'],
    ueditor_config: ['./src/common/UEditor/ueditor.config.js'],
    ueditor_all: ['./src/common/UEditor/ueditor.all.js']
};
new HtmlWebpackPlugin({
    template: `./src/app/${key}/templates/${filename}`,
    filename: `../view/${targetHtml}`,
    hash: true,
    chunks: [ueditor_all, ueditor_config, main]
})
复制代码
```
按上面的配置构建完成之后，会发现效果并不是我们想要的
```bash
<script type="text/javascript" src="/public/main.xxxx.js"></script> 
<script type="text/javascript" src="/public/ueditor.config.xxxx.js"></script> 
<script type="text/javascript" src="/public/ueditor.all.xxxx.js"></script>
复制代码
```
main.js在UE的前面，这样main中使用window.UE就会报错。显然，我们需要一种方式来让这个顺序符合我们的预期。
## 第二步
HtmlWebpackPlugin的chunksSortMode属性是用来控制插入模板html的script标签的顺序的，默认是auto，会根据webpack给每个chunk生成的id去排序，在entry中排的越前的，id就越小，那在html中就排在前面。所以这里我们第一种解决方案是，调换一下entry顺序
```
const entry = {
	ueditor_config: ['./src/common/UEditor/ueditor.config.js'],
    ueditor_all: ['./src/common/UEditor/ueditor.all.js']
    main: ['babel-polyfill', './src/main.js']
};
复制代码
```
但是这个方法有缺陷，当项目中有多个模板html需要引用入口的时候，在entry里面去控制这个排序就会遇到冲突的情况，不够灵活。
所以我们把顺序的控制权下方到每个**HtmlWebpackPlugin**中，通过把**chunksSortMode**设置为**manual**，按chunks的顺序去排序，例如
```
new HtmlWebpackPlugin({
	...
    chunks: [ueditor_config, ueditor_all, main]
})
复制代码
```
这样，生成的html中srcipt就会是下面的顺序
```
<script type="text/javascript" src="/public/ueditor.config.xxxx.js"></script> 
<script type="text/javascript" src="/public/ueditor.all.xxxx.js"></script>
<script type="text/javascript" src="/public/main.xxxx.js"></script> 
复制代码
```
现在看上去顺序是ok了，但是运行的时候，我们发现控制台报错
**regeneratorRuntime is not defined**
## 第三步
第二步最后出现的错误，是我们使用的ES6新API没有被转换导致的。由于之前我们只是在main的入口加了babel-polyfill，而main又是在UE的后面加载的，所以导致了报错。所以需要将babel-polyfill放到入口第一个文件
```
const entry = {
	ueditor_config: ['babel-polyfill', './src/common/UEditor/ueditor.config.js'],
    ueditor_all: ['./src/common/UEditor/ueditor.all.js']
    main: ['./src/main.js']
};
复制代码
```
继续运行后，第二步的错误已经解决了。不过，新的错误又出现了
```bash
TypeError: 'caller', 'callee', and 'arguments' 
properties may not be accessed on strict mode functions or the arguments objects for calls to them
复制代码
```
## 第四步
bable会默认给编译的js加上use strict;
严格模式下**caller**、**callee** 和**arguments** 是不能使用的，追溯到UE的源码中，我们发现里面大量使用了**arguments.callee**这种写法。
直接把源码都改了不现实，所以只能通过配置让bable忽略这个文件。在.babel中我们加入如下配置，
```
"presets": [
    "react"
 ],
 "ignore": [
     "./src/common/UEditor/ueditor.all.js"
 ],
复制代码
```
到此webpack就能按照我们的预期构建UE模块了。
