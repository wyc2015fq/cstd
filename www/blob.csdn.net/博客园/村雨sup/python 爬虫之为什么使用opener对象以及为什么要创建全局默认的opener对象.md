# python 爬虫之为什么使用opener对象以及为什么要创建全局默认的opener对象 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)









基本的urlopen()函数不支持验证、cookie或其他HTTP高级功能。要支持这些功能，必须使用build_opener()函数来创建自己的自定义[Opener](http://www.nowamagic.net/academy/tag/Opener)对象。

![](http://www.nowamagic.net/librarys/images/201311/2013_11_04_01.png)

install_opener(opener) 安装opener作为urlopen()使用的全局URL opener，即意味着以后调用urlopen()时都会使用安装的opener对象。opener通常是build_opener()创建的opener对象。

一些复杂情况详细解决办法：

1. cookie处理

如果要管理HTTP cookie，需要创建添加了HTTPCookieProcessor处理程序的opener对象。默认情况下。HTTPCookieProcessor使用CookieJar对象，将不同类型的CookieJar对象作为HTTPCookieProcessor的参数提供，可支持不同的cookie处理。如下面代码：



|`1`|`mcj``=``cookielib.MozillaCookieJar(``"cookies.txt"``)`|
|----|----|


|`2`|`cookiehand``=``HTTPCookieProcessor(mcj)`|
|----|----|


|`3`|`opener``=``urllib2.build_opener(cookiehand)`|
|----|----|


|`4`|`u``=``opener.``open``(http:``/``/``www.baidu.com)      `|
|----|----|




2. 代理

urllib2会自动检测代理设置，默认使用环境变量http_proxy 来设置 HTTP Proxy通常情况下，这是很有帮助的，因为也可能造成麻烦（因为通过代理获取本地URL资源时会被阻止，因此如果你正在通过代理访问Internet，那么使用脚本测试本地服务器时必须阻止urllib2模块使用代理）。因此，如果想在程序中明确Proxy的使用而不受环境变量的影响，可以通过创建ProxyHandler实例，并将实例作为build_opener()的参数来实现。如下面代码：



|`01`|`import``urllib2`|
|----|----|


|`02`|` `|
|----|----|


|`03`|`enable_proxy ``=``True`|
|----|----|


|`04`|`proxy_handler ``=``urllib2.ProxyHandler({``"http"``: ``'[http://some-proxy.com:8080](http://some-proxy.com:8080/)'``})`|
|----|----|


|`05`|`null_proxy_handler ``=``urllib2.ProxyHandler({})`|
|----|----|


|`06`|` `|
|----|----|


|`07`|`if``enable_proxy:`|
|----|----|


|`08`|`    ``opener ``=``urllib2.build_opener(proxy_handler)`|
|----|----|


|`09`|`else``:`|
|----|----|


|`10`|`    ``opener ``=``urllib2.build_opener(null_proxy_handler)`|
|----|----|


|`11`|` `|
|----|----|


|`12`|`urllib2.install_opener(opener)`|
|----|----|




3. 一个简单的[模拟登录](http://www.nowamagic.net/academy/tag/%E6%A8%A1%E6%8B%9F%E7%99%BB%E5%BD%95)例子：



[view source](http://www.nowamagic.net/academy/detail/1302879#viewSource)

[print](http://www.nowamagic.net/academy/detail/1302879#printSource)[?](http://www.nowamagic.net/academy/detail/1302879#about)



|`01`|`#模拟登录`|
|----|----|


|`02`|`cj ``=``cookielib.CookieJar()`|
|----|----|


|`03`|`#用户名和密码 `|
|----|----|


|`04`|`post_data ``=``urllib.urlencode({``'username'``: ``'[nowamagic]'``, ``'password'``: ``'[mypass]'``, ``'pwd'``: ``'1'``})`|
|----|----|


|`05`|`#登录路径 `|
|----|----|


|`06`|`#path = '[http://www.xiaomi.com/pass/serviceLoginAuth2](http://www.xiaomi.com/pass/serviceLoginAuth2)'`|
|----|----|


|`07`|`path ``=``'[http://www.nowamagic.net/](http://www.nowamagic.net/)'`|
|----|----|


|`08`|`cookieHandle ``=``urllib2.HTTPCookieProcessor(cj)`|
|----|----|


|`09`|`opener ``=``urllib2.build_opener(cookieHandle)`|
|----|----|


|`10`|`#url = opener.open('[http://www.baidu.com](http://www.baidu.com/)')`|
|----|----|


|`11`|`#page = url.read()`|
|----|----|


|`12`||
|----|----|


|`13`|`opener.addheaders ``=``[(``'User-agent'``, ``'Opera/9.23'``)]`|
|----|----|


|`14`|`urllib2.install_opener(opener)`|
|----|----|


|`15`|`req ``=``urllib2.Request(path, post_data)`|
|----|----|


|`16`|`conn ``=``urllib2.urlopen(req)`|
|----|----|


|`17`|`result ``=``conn.geturl()`|
|----|----|


|`18`|`#print path`|
|----|----|


|`19`|`#message = {`|
|----|----|


|`20`|`    ``#"header": conn.info(),`|
|----|----|


|`21`|`    ``#"status": conn.getcode(),`|
|----|----|


|`22`|`    ``#"url": conn.geturl(),`|
|----|----|


|`23`|`#}`|
|----|----|


|`24`||
|----|----|


|`25`|`self``.render(``"nowamagic.html"``,message``=``result)`|
|----|----|














