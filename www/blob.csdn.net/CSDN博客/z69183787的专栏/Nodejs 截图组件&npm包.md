# Nodejs 截图组件&npm包 - z69183787的专栏 - CSDN博客
2018年05月18日 21:59:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：938
个人分类：[前端-NodeJs](https://blog.csdn.net/z69183787/article/category/5669799)
下面都是转的，自己试了一下可以保存图片
npm install webshot
var webshot = require('webshot'); 
webshot('baidu.com', 'google.png', function(err) { 
  // screenshot now saved to google.png 
}); 
![](https://img-blog.csdn.net/20180518215900964?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3o2OTE4Mzc4Nw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
写文章的时候经常需要插入图片，插入现有的图片很简单，有时候制作一些优秀的网站列表的时候需要添加网页截图， 这个过程非常枯燥乏味，可以考虑开发一个命令行工具传入一个url，然后生成页面截图。
**使用node-webshot进行网页截图**
用到的npm模块有yargs和node-webshot，关于yargs的文章参考这里[从零开始打造个人专属命令行工具集——yargs完全指南](http://developer.51cto.com/art/201608/515865.htm)。
node-webshot是调用phantomjs来生成网页截图的，phantomjs是非常有名的npm项目，相当于一个脚本版的WebKit浏览器 ，通过phantomjs可以使用脚本 和网页进行交互，所以phantomjs经常用来进行网页自动化测试。
phantomjs会和普通的浏览器一样加载完整的网页内容，然后在内存里面进行渲染，虽然肉眼看不到它渲染的页面，但是通过生成图片就可以看到了，node-webshot使用的就是phantomjs的render接口来获取网页截图的。
node-webshot生成谷歌首页的示例代码：
- var webshot = require('webshot'); 
- 
- webshot('google.com', 'google.png', function(err) { 
-   // screenshot now saved to google.png 
- }); 
- //phantomjs生成谷歌首页的示例代码： 
- 
- var page = require('webpage').create(); 
- page.open('http://github.com/', function() { 
-   page.render('github.png'); 
-   phantom.exit(); 
- }); 
那为什么不直接使用phantomjs呢?一个字懒!
另外node-webshot还对文件读写进行了简单的封装，相信任何熟悉node.js的开发人员，都能很简单的写出这样的接口，但是既然轮子好用就不要自己造了。
node-webshot流式调用的写法：
- var webshot = require('webshot'); 
- var fs      = require('fs'); 
- 
- var renderStream = webshot('google.com'); 
- var file = fs.createWriteStream('google.png', {encoding: 'binary'}); 
- 
- renderStream.on('data', function(data) { 
-   file.write(data.toString('binary'), 'binary'); 
- }); 
node-webshit还支持生成移动版的网页截图：
- var webshot = require('webshot'); 
- 
- var options = { 
-   screenSize: { 
-     width: 320 
-   , height: 480 
-   } 
- , shotSize: { 
-     width: 320 
-   , height: 'all'
-   } 
- , userAgent: 'Mozilla/5.0 (iPhone; U; CPU iPhone OS 3_2 like Mac OS X; en-us)'
-     + ' AppleWebKit/531.21.20 (KHTML, like Gecko) Mobile/7B298g'
- }; 
- 
- webshot('flickr.com', 'flickr.jpeg', options, function(err) { 
-   // screenshot now saved to flickr.jpeg 
- }); 
最后再把yargs和node-webshot进行集成，可惜这部分工作人家也帮我们做好了，直接安装就行。
- npm isntall -g webshot-cli 
**使用desktop-screenshot进行系统截图**
desktop-screenshot是一个跨平台的系统截图项目，用法和node-webshot类似，只是少了一个url参数。
- var screenshot = require('desktop-screenshot'); 
- 
- screenshot("screenshot.png", function(error, complete) { 
-     if(error) 
-         console.log("Screenshot failed", error); 
- else
-         console.log("Screenshot succeeded"); 
- }); 
这个是我的系统截图
![](http://s1.51cto.com/wyfs02/M02/86/73/wKioL1e-86yw5sVsAAGHAu0k9Qc464.png)
问题是我用调用命令行的时候这个窗口也会出现在截图上面，而且没有太好的办法来去除这个窗口，系统截图还是使用windows自带的好用点。
