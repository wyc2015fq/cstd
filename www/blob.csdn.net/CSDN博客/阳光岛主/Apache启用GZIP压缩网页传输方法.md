
# Apache启用GZIP压缩网页传输方法 - 阳光岛主 - CSDN博客

2014年04月27日 22:32:52[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：10592个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)



一、gzip介绍
Gzip是一种流行的文件压缩算法，现在的应用十分广泛，尤其是在Linux平台。当应用Gzip压缩到一个纯文本文件时，效果是非常明显的，大约可以减少70％以上的文件大小。这取决于文件中的内容。 利用Apache中的Gzip模块，我们可以使用Gzip压缩算法来对Apache服务器发布的网页内容进行压缩后再传输到客户端浏览器。这样经过压缩后实际上降低了网络传输的字节数，最明显的好处就是可以加快网页加载的速度。
网页加载速度加快的好处不言而喻，除了节省流量，改善用户的浏览体验外，另一个潜在的好处是Gzip与搜索引擎的抓取工具有着更好的关系。
二、Web服务器处理
HTTP压缩的过程如下：
Web服务器接收到浏览器的HTTP请求后，检查浏览器是否支持HTTP压缩（Accept-Encoding 信息）；
如果浏览器支持HTTP压缩，Web服务器检查请求文件的后缀名；
如果请求文件是HTML、CSS等静态文件，Web服务器到压缩缓冲目录中检查是否已经存在请求文件的最新压缩文件；
如果请求文件的压缩文件不存在，Web服务器向浏览器返回未压缩的请求文件，并在压缩缓冲目录中存放请求文件的压缩文件；
如果请求文件的最新压缩文件已经存在，则直接返回请求文件的压缩文件；
如果请求文件是动态文件，Web服务器动态压缩内容并返回浏览器，压缩内容不存放到压缩缓存目录中。
三、开启Gzip
Apache利用Gzip压缩算法进行压缩的模块有两种：mod_gzip 和mod_deflate。
现在浏览器本身也自动Gzip压缩功能，支持Accept-Encoding: gzip,deflate ，这里我在firefox浏览器下测试。
![](http://blog.chinaunix.net/attachment/201301/21/25266990_135875559456ad.jpg)
通过查看HTTP头，我们可以快速判断使用的客户端浏览器是否支持接受gzip压缩。
若发送的HTTP头中出现以下信息，则表明你的浏览器支持接受相应的gzip压缩：
Accept-Encoding: gzip 支持mod_gzip
Accept-Encoding: deflate 支持mod_deflate
Accept-Encoding: gzip,deflate
同时支持mod_gzip 和mod_deflate Apache内置有mod_deflate模块来启用gzip功能，但假如安装apache的时候没有编译相关模块，就需要你手动安装一次，以启用它：
首先到你的apache源码目录，查找到mod_deflate.c文件 ,通常位置：apachehttpd源码目录/modules/filters/mod_deflate.c，进入上面找到的目录运行下面的命令：
/usr/local/apache2/bin/apxs -i -c -a mod_deflate.c
注：apxs目录请参照您自己的机器，通常在apache安装目录的bin目录下。
安装完成，到apache的modules目录看看是不是有了mod_deflates.so，httpd.conf中打开deflate_Module和headers_Module模块：
LoadModule deflate_module modules/mod_deflate.so
加载mod_deflate.so模块,默认安装会自动写入httpd.conf。
如果服务器开启了对Gzip组件的支持，那么我们就可以在http.conf定制文件压缩，下面是一个配置的简单实例：
1、mod_gzip方式
\# mod_gzip
<ifModule mod_gzip.c>
mod_gzip_on Yes
mod_gzip_dechunk Yes
mod_gzip_item_include file \.(html?|txt|css|js|php|pl)$
mod_gzip_item_include handler ^cgi-script$
mod_gzip_item_include mime ^text/.*
mod_gzip_item_include mime ^application/x-javascript.*
mod_gzip_item_exclude rspheader ^Content-Encoding:.*gzip.*
<ifModule>
2、deflate_Module方式
(1)严格匹配文件类型
\# mod_deflate：
<ifmodule mod_deflate.c>
DeflateCompressionLevel 6 \#压缩率, 6是建议值.
AddOutputFilterByType DEFLATE text/plain
AddOutputFilterByType DEFLATE text/html
AddOutputFilterByType DEFLATE text/php
AddOutputFilterByType DEFLATE text/xml
AddOutputFilterByType DEFLATE text/css
AddOutputFilterByType DEFLATE text/javascript
AddOutputFilterByType DEFLATE application/xhtml+xml
AddOutputFilterByType DEFLATE application/xml
AddOutputFilterByType DEFLATE application/rss+xml
AddOutputFilterByType DEFLATE application/atom_xml
AddOutputFilterByType DEFLATE application/x-javascript
AddOutputFilterByType DEFLATE application/x-httpd-php
AddOutputFilterByType DEFLATE image/svg+xml
AddOutputFilterByType DEFLATE image/gif image/png  image/jpe image/swf image/jpeg image/bmp
\# Don’t compress images and other  \#排除不需要压缩的文件
BrowserMatch ^Mozilla/4 gzip-only-text/html
BrowserMatch ^Mozilla/4\.0[678] no-gzip
BrowserMatch \bMSIE !no-gzip !gzip-only-text/html
SetEnvIfNoCase Request_URI .(?:html|htm)$ no-gzip dont-varySetEnvIfNoCase
\#SetEnvIfNoCase Request_URI .(?:gif|jpe?g|png)$ no-gzip dont-vary
SetEnvIfNoCase Request_URI .(?:exe|t?gz|zip|bz2|sit|rar)$ no-gzip dont-vary
SetEnvIfNoCase Request_URI .(?:pdf|doc)$ no-gzip dont-vary
</ifmodule>
(2)过滤文件类型
\# mod_deflate：
<ifmodule mod_deflate.c>
DeflateCompressionLevel 6
SetOutputFilter DEFLATE    \#压缩所有文件
\#Don’t compress images and other  \#过滤不需要压缩文件
\#SetEnvIfNoCase Request_URI .(?:gif|jpe?g|png)$ no-gzip dont-vary
SetEnvIfNoCase Request_URI .(?:html|htm)$ no-gzip dont-vary
SetEnvIfNoCase Request_URI .(?:exe|t?gz|zip|bz2|sit|rar)$ no-gzip dont-vary
SetEnvIfNoCase Request_URI .(?:pdf|doc)$ no-gzip dont-vary
</IfModule>
文件MIME类型可以根据自己情况添加，也可以通过浏览器查看connect-type：
![](http://blog.chinaunix.net/attachment/201301/21/25266990_1358756416or7u.jpg)
四、mod_gzip 和mod_deflate的主要区别是什么？（来自互联网）
首先一个区别是安装它们的Apache Web服务器版本的差异。Apache 1.x系列没有内建网页压缩技术，所以才去用额外的第三方mod_gzip 模块来执行压缩。而Apache 2.x官方在开发的时候，就把网页压缩考虑进去，内建了mod_deflate 这个模块，用以取代mod_gzip。虽然两者都是使用的Gzip压缩算法，它们的运作原理是类似的。 第二个区别是压缩质量。mod_deflate 压缩速度略快而mod_gzip 的压缩比略高。一般默认情况下，mod_gzip 会比mod_deflate 多出4%~6％的压缩量。 那么，为什么使用mod_deflate？第三个区别是对服务器资源的占用。 一般来说mod_gzip 对服务器CPU的占用要高一些。mod_deflate 是专门为确保服务器的性能而使用的一个压缩模块，mod_deflate 需要较少的资源来压缩文件。这意味着在高流量的服务器，使用mod_deflate 可能会比mod_gzip 加载速度更快。
五、效果
chrome 查看： F12 ——》调试模式　——》　网络　——》选中左侧一个请求文件，如下图
![](https://img-blog.csdn.net/20140320170420312)

没有启动Gzip压缩：
![](http://blog.chinaunix.net/attachment/201301/21/25266990_1358756598r66y.png)
启动Gzip压缩：
![](http://blog.chinaunix.net/attachment/201301/21/25266990_13587566454KqK.png)
大小由10.7K到1.5K，如果文件较大，那么效果会更明显。

参考推荐：
[Apache启用GZIP压缩网页传输方法](http://mg123.lofter.com/post/1a5f3e_c15cd9)
[php gzip压缩输出的实现方法](http://www.jb51.net/article/36176.htm)



