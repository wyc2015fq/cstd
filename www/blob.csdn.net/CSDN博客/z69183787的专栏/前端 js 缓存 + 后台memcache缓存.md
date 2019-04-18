# 前端 js 缓存 + 后台memcache缓存 - z69183787的专栏 - CSDN博客
2015年02月03日 19:41:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3774
在项目中用到缓存机制，又想把部分缓存信息给放到页面中处理，刚开始想到了cookie，但是转眼想到cookie有大小限制，故而只能用javascript缓存了，ps：这里的js缓存类似于html中dom元素中存放值，只不过js交互性比较好。
    整理结果如图：
![](https://img-blog.csdn.net/20140625123310062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGl1MjUxODkwMzQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
流程步骤：
1、客户端浏览器向服务端发出请求
2、服务端判断缓存状态（是否更新了缓存）
3、如果未更新，则正常返回页面（图中未画出该过程）
4、服务器有更新，先去memcache中取的需要缓存的集合
5、服务器端向客户端浏览器返回缓存信息。
js缓存：
    页面端使用jquery ecache，具体使用方法这里不再叙述。
服务器端缓存：
    服务端使用memcache，memcache可以和web服务器在同一台机器上，也可以在不同机器上。当memcache服务器有更新时会通知web服务器更新缓存。
