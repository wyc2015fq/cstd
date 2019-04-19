# react-redux-universal-hot-example错误 - 三少GG - CSDN博客
2016年09月16日 19:40:00[三少GG](https://me.csdn.net/scut1135)阅读数：1031
个人分类：[Web网站设计](https://blog.csdn.net/scut1135/article/category/762263)

问题：  no suitable image found
ERROR in dlopen(/Users/xxx/web/react-redux-universal-hot-example/node_modules/node-sass/vendor/darwin-x64-48/binding.node, 1): no suitable image found.  Did find:
[0]     /Users/xxx/web/react-redux-universal-hot-example/node_modules/node-sass/vendor/darwin-x64-48/binding.node: truncated mach-o error: segment __TEXT extends to 1212416 which is past end of file 69180
解决办法：[https://github.com/sass/node-sass/issues/1387](https://github.com/sass/node-sass/issues/1387)
`**npm rebuild node-sass**`
 查看：node -p "console.log(require('node-sass').info)"
问题： 
仔细查看npm install环节 有以下输出：
Start downloading binary at https://github.com/sass/node-sass/releases/download/v3.10.0/darwin-x64-48_binding.node
Saving to /var/folders/2r/h3svtmqn0rv5jpj5kzc9z8cw0000gn/T/phantomjs/phantomjs-2.1.1-macosx.zip
若npm install改为 cnpm install
Considering PhantomJS found at /Users/grantgu/code/web/react-redux-universal-hot-example/node_modules/.bin/phantomjs
Looks like an `npm install -g`
Could not link global install, skipping...
Downloading https://npm.taobao.org/mirrors/phantomjs/phantomjs-2.1.1-macosx.zip
Saving to /var/folders/2r/h3svtmqn0rv5jpj5kzc9z8cw0000gn/T/phantomjs/phantomjs-2.1.1-macosx.zip
Receiving...
#######最终解决方案 20161002##########
其余变动： 
1.更改了node版本号：6.2.0
2. 更改了node-sass 版本号: 
cnpm install 
修复问题：
  https://dev.to/ben/the-targetblank-vulnerability-by-example
In order to restrict the behavior `window.opener` access,
 the original page needs to add a `rel="noopener"` attribute
 to any link that has `target="_blank"`.
 However, Firefox does not support that tag, so you should actually use `**rel="noopener noreferrer"**` for full coverage.
