# dojo.require()的相关理解 - 毛小亮 - 博客园
# [dojo.require()的相关理解](https://www.cnblogs.com/xianerwonder/p/4588624.html)
Dojo 提供了一个非常强大的javascript控件库. 在使用dojo之前,用户基本上不需要具备任何基础知识. 你可以用script远程链接到dojo(dojo.js), 也可以把dojo.js下载到本地并用script标签加载.
如果你不太了解dojo, 可以参考一下如下资料:
- [Dojo 1.4 Cheat Sheet](http://download.dojotoolkit.org/release-1.4.0/cheat.html)
- [Dojo Base Source Tree (1.4.3)](http://download.dojotoolkit.org/release-1.4.3/dojo-release-1.4.3/dojo/_base/)
- [Dojo Reference Guide](http://www.dojotoolkit.org/reference-guide/dojo/index.html)
大体上,dojo.js和jquery.js 或者 prototype js, 里面有很多开发web应用的常用的特性: 包括:
- JavaScript Language Helpers
- Object 工具
- Array 工具
- DOM 操作
- 标准的事件机制
- Ajax & 跨域请
- JSON 工具
- 简单特效
- 浏览器兼容
不仅如此, dojo还有很多其他javascript库(jquery, ext等等)所不具有的功能. 其中一个很重要的功能就是模块化的机制 - 模块系统(`dojo.require()` ). JavaScript 和 浏览器本身以及其他的javascript库并不支持这种特性, dojo很好的解决了这种问题.
## 模块系统
`dojo.require('my.module')` 用于加载javascript文件, 功能类似于script标签的作用.
假设你有一个本地的开发环境,目录结构如下:(`http://localhost:8888` .)
![file1](http://www.sitepen.com/blog/wp-content/uploads/2010/03/file1.png)
`index.html` 是一个包含 `dojo.js 简单页面` .
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”dojo/dojo.js”></script> 
</body>
</html>
假设我们要用Flickr API获取数据, 这时候,我们就要用到跨域请求, 但是这些功能模块并不是在dojo base库里面, 我们需要另外加载所需的dojo模块:
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”/dojo/dojo.js”></script>
<script src=”/dojo/io/script.js”></script> 
</body>
</html>  
这里我们可以用script标签解决这种问题, 同样还有另外一种方式, 这种方式体现了模块系统的宗旨: 我们用`dojo.require()` 加载 `dojo.io.script`
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”/dojo/dojo.js”></script>
<script>
dojo.require(”dojo.io.script”); 
//Note: do not include the .js
</script>
</body>
</html>
`通过``dojo.require()` 我们获得了一个模块系统, 它提供了一系列我们开发复杂web2.0页面所需要的组件. 接下来的内容我们会着重介绍模块系统的特性:
- 缓存管理
- 命名空间管理
- 路径管理
- 依赖管理
首先, `dojo.require()会避免重复加载, 如果script脚本被浏览器缓存了, dojo会调用缓存的资源从而避免不必要的http请求, 事实上,你可以随便调用``dojo.require(), 不管调用了多少`
```
dojo.require(), dojo都会保证同样的模块只会被加载一次.
```
我们也可以创建自己的模块. 让我们回到Flickr API的例子, 我们要开发一个用Flickr数据的大型web应用, 我们需要能很好的组织和管理这些javascript代码. 归根结底,我们需要创建名为flickrApp的命名空间用于保存所有该应用的逻辑功能. 为了达到这个目的, 我们更新原有的目录结构, 创建一个`flickrApp.js文件:`
![file2](http://www.sitepen.com/blog/wp-content/uploads/2010/03/file2.png)
`flickrApp.js看起来似乎仅仅是一个js文件, 但如果你用dojo的眼光来看他, 你会发现他其实应该是一个模块, 为了让dojo识别他是这个模块, 我们用``[dojo.provide()](http://www.dojotoolkit.org/reference-guide/dojo/provide.html) `方法初始化这个js文件,将其变为一个dojo的模块. 我们加入如下代码到 flickrApp.js 文件中:
**[javascript]**[view plain](http://blog.csdn.net/dojotoolkit/article/details/5935844)[copy](http://blog.csdn.net/dojotoolkit/article/details/5935844)
- dojo.provide(”flickrApp “); //similar to doing flickrApp = {};  
`dojo.provide()` 创建了一个以你传入的字符串(`flickrApp` )命名的对象结构(名字空间), 我们这里是创建了一个名为`flickrApp的对象, 该对象创建后, 我们便可以像定义该对象的各个属性一样来定义该应用(``flickrApp``)` 的各个方面, 下面是 flickrApp.js 一个例子:
**[javascript]**[view plain](http://blog.csdn.net/dojotoolkit/article/details/5935844)[copy](http://blog.csdn.net/dojotoolkit/article/details/5935844)
- dojo.provide(”flickrApp”);  
- //start creating the application logic for my flickr app  
- flickrApp.getData = function(){};  
现在我们可以用 `dojo.require()` 来加载我们自定义的模块到HTML页面上:
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”/dojo/dojo.js”></script>
<script>
dojo.require(”dojo.io.script”);
dojo.require(”flickrApp”); 
</script>
</body>
</html>
问题来了， dojo是怎么知道`flickrApp.js模块的文件系统中存放的位置的呢？ 答案就是dojo的路径管理机制， dojo会根据你传入``dojo.provide()``的字符串来定位该模块的位置，基准点是dojo.js的上一级目录， 比如dojo.js在``http://localhost:8888/dojo/dojo.js，所以dojo会在``http://localhost:8888/这个目录级别（dojo.js的上一级）来定位所有的模块。 为了说明这个问题， 我们现在来再一次改变例子的文件结构，使其看起来` 更加具有组织性，如下：
![file3](http://www.sitepen.com/blog/wp-content/uploads/2010/03/file3.png)
此时，所有的应用相关的代码都在flickrApp这个目录（名字空间）下，在这个目录下，我们可以更进一步将该应用切分成不同的模块。第一个模块就是`data.js模块，包含获取` Flickr数据的逻辑功能，以及跨域，返回数据等等功能。基于这个改变，我们需要在`data.js里面加入``dojo.provide()语句来告诉他新的目录结构的改变，如下（``data.js``）：`
**[javascript]**[view plain](http://blog.csdn.net/dojotoolkit/article/details/5935844)[copy](http://blog.csdn.net/dojotoolkit/article/details/5935844)
- //below is similar to doing var flickrApp = {}; flickrApp.data = {};  
- dojo.provide(”flickrApp.data “);  
- // Note: do not include the .js  
- flickrApp.data.getData = function(){};  
就像我们之前所说的，dojo会从dojo.js（`http://localhost:8888/dojo/dojo.js` ）的上一级目录来开始定位各个模块，所以这里dojo定位我们的`data.js` 模块的路径为`http://localhost:8888/flickrApp/data.js。此时，我们的HTML代码也要做相应的改动：`
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”/dojo/dojo.js”></script>
<script>
dojo.require(”dojo.io.script”);
dojo.require(”flickrApp.data “);
</script>
</body>
</html>
好了，到此为止，我们来想一想，这样做真的有必要吗，我们为什么不能不要借助这种模块系统，而仅仅是把应用程序所有的逻辑功能都放在一个javascript文件中呢？ 当然可以。但是dojo实现模块系统的目的在于更好更方便的管理代码，也便于用工具压缩优化代码。
接下来我们讨论一下最重要的部分：依赖管理.
模块可以包含对其他模块的引用，即在模块中可以require()其他的模块， dojo会帮你管理这些模块，让我们回到我们HTML页面：
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”/dojo/dojo.js”></script>
<script>
dojo.require(”dojo.io.script”); 
dojo.require(”flickrApp.data”);
</script>
</body>
</html>
我们可以看到，这里我们会在HTML页面上require 这个`dojo.io.script.js模块， 事实上我们可以把这个``require()语句放到``data.js模块里面，其实这里``data.js模块是依赖于``dojo.io.script.js模块的，dojo会管理这个依赖关系，所以此时``data.js模块如下：`
**[javascript]**[view plain](http://blog.csdn.net/dojotoolkit/article/details/5935844)[copy](http://blog.csdn.net/dojotoolkit/article/details/5935844)
- dojo.provide(”flickrApp.data”);  
- dojo.require(”dojo.io.script”);  
- // Note: dojo.require() should be used after dojo.provide()  
- flickrApp.data.getData = function(){};  
此时，我们的HTML页面就只需要包含一个 `require()` 语句 （`data.js模块）` .
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”/dojo/dojo.js”></script>
<script>
dojo.require(”flickrApp.data”);
</script> 
</body>
</html>
dojo会管理这些依赖关系，确保`data.js依赖于``dojo.io.script.js。`
更有甚者，依赖管理还需要一个响应通知，即当所有依赖的模块都被加载完成后的一个响应通知。这件事情`可以用dojo.ready()这个函数来实现，`
```
dojo.ready()将会注册一个函数，这个函数会在所有的DOM节点加载完成，并且所有模块及其的依赖模块都加载和解析完成时 被调用。用法如下：
```
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”/dojo/dojo.js”></script>
<script>
dojo.require(”flickrApp.data”);
dojo.ready(function(){ 
// Note that dojo.ready() is a shortcut for dojo.addOnLoad() added in Dojo 1.4
// Run code from data.js and all its dependencies safely
}); 
</script>
</body>
</html>
`dojo.ready()` 可以用在任何时间，任何地方，甚至是`dojo.ready()的callback方法里面，比如` :
**[javascript]**[view plain](http://blog.csdn.net/dojotoolkit/article/details/5935844)[copy](http://blog.csdn.net/dojotoolkit/article/details/5935844)
- dojo.require(”some.module”);  
- dojo.ready(function(){  
- //run code from some.module.js and all its dependencies safely  
- dojo.require(’some.other.module’);  
- dojo.ready(function(){  
- //run code from some.other.module.js and all its dependencies safely  
- });  
- });  
就像前面提到的，当用到dojo的 `require()时，dojo会从dojo.js的上一级目录开始查找，比如``require('some.other.module')会从``some/other/module.js开始查找：`
![file4](http://www.sitepen.com/blog/wp-content/uploads/2010/03/file4.png)
其实dojo也提供了我们定义dojo的默认查找路径的方式，我们再更新一下我们应用程序的结构，如下：
![file5](http://www.sitepen.com/blog/wp-content/uploads/2010/03/file5.png)
可以看到，我们的模块 - `some.other.module.js已经在dojo默认的模块查找路径之外了,我们要告诉dojo这个结构的变化就要用到``dojo.registerModulePath()：`
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script src=”/js/dojo1.4.1/dojo.js”></script>
<script>
dojo.registerModulePath(”some”, “../../some/”); 
dojo.require(”some.other.module”);
</script>
</body>
</html>
通过`dojo.registerModulePath("some", "../../some/")` 语句，dojo便知道这个用户自定义的模块的位置，这里，我们告诉dojo我们的自定义模块some是在dojo.js的上两级（`../../some` ），这个时候，dojo便知道了如何去解析这个名字空间（`some.other.module` ）并加载`some.other.module.js文件了。此时此刻，dojo便能识别所有在“``../../some/``”路径下的自定义模块。之所以需要这样做的原因主要是安全问题，浏览器是不支持javascript访问文件系统的。 如果你对`[djConfig](http://docs.dojocampus.org/djConfig) 比较了解的话，你会发现其实这个对象也可以用来做“`dojo.registerModulePath()` ”的工作：
<!DOCTYPE html>
<html lang=”en”>
<head>
<meta charset=”utf-8″ />
<title>Dojo</title>
</head>
<body>
<script type=”text/javascript”>
var djConfig = {modulePaths:{”some”:”../../some/”}}; 
</script>
<script src=”/dojo/dojo1.4.1/dojo.js”></script>
<script>
dojo.require(”some.other.module”);
</script>
</body>
</html>
其实dojo关于模块系统的内容还有很多，这里我们主要是基于本地的dojo，其实dojo还可以以跨域的方式加载（从AOL或者Google CDN），所以模块系统也会去支持这种特性。模块系统的底层实现是基于XHR去请求模块内容的，这些在你用dojo的CDN版本是会改变，这时模块系统会转换到跨域的方式（基于script元素）。
## Dojo之外
其实这些模块系统的想法也可以在没有dojo的情况下使用。 YUI3.0也有一个类似的实现，同样，也有一些针对模块系统的想法而专门实现的独立的控件库，其中之一就是[RequireJS](http://requirejs.org/) ，他是基于dojo实现的。有兴趣不妨下载下来研究一下。

