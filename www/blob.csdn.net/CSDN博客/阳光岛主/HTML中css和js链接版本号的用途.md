
# HTML中css和js链接版本号的用途 - 阳光岛主 - CSDN博客

2014年08月29日 22:25:57[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5455



**背景**
在搜索引擎中搜索关键字[.htaccess](http://blog.ithomer.net/?s=.htaccess)缓存，你可以搜索到很多关于设置网站文件缓存的教程，通过设置可以将css、js等不太经常更新的文件缓存在浏览器端，这样访客每次访问你的网站的时候，浏览器就可以从缓存中获取css、js等静态文件，而不必从你的服务器再次下载读取，这样在一定程度上加快了网站的打开速度，又可以节约一下你的服务器流量。
**问题**
最近在修改更新我的[IT-Homer博客](http://blog.ithomer.net/)时，遇到了一个问题：本地更新了style.css后，不管怎么更新CDN缓存，还是Ctrl + F5 刷新浏览器，博客仍然加载的旧的style.css文件。
现在问题来了，通过.htaccess设置的css、js缓存都有一个过期时间，如果在访客的浏览器中已经缓存了css、js，在这些css、js缓存未过期之前，浏览器只会从缓存中读取css和js，如果你在服务器上修改了css和js，那么这些更改在回头客的浏览器中是不会有变化的，除非回头客按了Ctrl + F5刷新了你的网站页面或者手动清空了浏览器的缓存。一个网站的访客成千上万，回头客也会有不少，你不可能在更新css后让每个访客都刷新一下缓存吧，那么这个问题你会怎么处理呢？
**处理办法**
**1. 更改css文件名**
其实解决这个问题很简单，缓存是通过文件名（例如：style.css，style.min.css，style.min.v2.css等）标记缓存内容的。在你更新了网站的css文件内容后，在更换一下css的文件名就可以了。如原先html中的css调用语句如下：
<link rel=”stylesheet” href=“http://blog.ithomer.net/wp-content/themes/officefolders/style.css” type=”text/css” media=”screen” />
改一下css文件名就可以了：
<link rel=“stylesheet” href=”http://blog.ithomer.net/wp-content/themes/officefolders/style.min.css” type=”text/css” media=”screen” />
另外一种从更改css文件名的方法是将版本号写到文件名中，如：
<link rel=”stylesheet” href=”http://blog.ithomer.net/wp-content/themes/officefolders/style.min.v1.0.0.css” type=”text/css” media=”screen”/>
css文件更新后，改一下文件名中的版本号即可：
<link rel=”stylesheet” href=”http://blog.ithomer.net/wp-content/themes/officefolders/style.min.v1.0.1.css” type=”text/css” media=”screen”/>
**2. 给css文件加个版本号**
其实每次修改css文件后还要修改css的文件名有点麻烦，那么我们可以在加载css语句中加入个版本号（即css链接中?后面的内容）就可以了，由浏览器自动去比较css，js链接后面的版本号大小，来自动更新客户端最新的css，js等静态文件。
例如原先html中的css调用语句如下：
<link rel=”stylesheet” href=“http://blog.ithomer.net/wp-content/themes/officefolders/style.min.css?ver=1.0.0”type=”text/css” media=”screen”/>
改一下css文件的版本号改成2012就可以了：
<link rel=”stylesheet” href=“http://blog.ithomer.net/wp-content/themes/officefolders/style.min.css?ver=1.0.1”type=”text/css” media=”screen”/>
**总结**
其实css，js文件后面的问号不起实际作用，仅能当作后缀，让浏览器自动检测更新最新的css，js等静态文件。如果用问号加参数的方法，可以添加版本号等信息，同时可以刷新一下浏览器端的缓存。一个小小的细节，可以给我们带来很大的方便。
示例请见我的博客：[IT-Homer 博客](http://blog.ithomer.net/)或[IT-Homer 河马代理](http://proxy.ithomer.net/)


