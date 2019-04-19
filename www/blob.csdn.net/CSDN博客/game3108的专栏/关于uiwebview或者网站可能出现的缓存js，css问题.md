# 关于uiwebview或者网站可能出现的缓存js，css问题 - game3108的专栏 - CSDN博客
2015年06月18日 15:08:24[game3108](https://me.csdn.net/game3108)阅读数：644
最近项目中碰到，每次更新远程web的js和css代码，在iphone手机上总会有比较严重的缓存，基本都需要删除程序再重新安装才能刷新出来。
但本地的request方式已经使用了 
NSURLRequestReloadIgnoringLocalCacheData
最后查询其原因，应该是浏览器（包括uiwebview）本身对js和css进行了缓存。
参考了一篇文章进行了解决：
# [关于css js文件缓存问题](http://www.cnblogs.com/hongsemantuoluo/archive/2013/01/15/2860928.html)
什么情况下，要禁止静态文件缓存：
1、经常可能要改动的 js, css.
比如一个js文件引用如下
<script src="test.js"></script>
后来进行了test.js文件的修改
在这种情况下如果以前用户浏览过此页面，那么他的浏览器自动缓存了 test.js
当他浏览新版本的时候，因为使用的是 缓存js，这样将导致脚本出错。
解决方法探讨：
因为css,js 是通过 <script src=....> 这种方式加载的，所以，很难使用 asp 的那种服务器端禁止缓存的办法。也很难使用ajax的通过设置 http请求头的办法禁止使用缓存。
解决方法
1、随机数法
方法一：
document.write( " <script src='test.js?rnd= " + Math.random() + " '></s " + " cript> " )
方法二： 
var js = document.createElement( " script " )
js.src = " test.js " + Math.random()
document.body.appendChild(js)
这样采用随机数的话， js文件将永远得不到缓存，每次都必须重新从服务器加载，即使没有任何更改。
大家如果经常上国外网站的话，可以看到他们通常采用这样的方式来解决：
<script src="test.js?ver=113"></script>
其中 ver=113 的 113就是版本号
这样真正做到了应该缓存的时候缓存静态文件，当版本有更新的时候从获取最新的版本，并更新缓存。
对于图像 <img src="test.jps?ver=版本号"> 来有效利用和更新缓存.
链接：http://www.cnblogs.com/hongsemantuoluo/archive/2013/01/15/2860928.html
