
# wget和curl中使用代理 - 阳光岛主 - CSDN博客

2016年03月23日 09:40:53[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：20063个人分类：[NetWork																](https://blog.csdn.net/sunboy_2050/article/category/698436)



**命令使用代理**
wget/curl 都支持使用代理
wget -e “http_proxy=10.1.4.43:8080″[proxy.mimvp.com](http://proxy.mimvp.com/)
curl -x 10.1.4.43:8080 proxy.mimvp.com
**环境变量使用代理**
curl、wget也支持通过环境变量http_proxy来设置要使用的代理服务器， 如下：
\# 这个帐号使用的代理服务器
http_proxy=”http://mycache.mydomain.com:3128″
export http_proxy
如果代理服务器需要用户名和密码，只要将它们加入 URL。
例如：用户名 myuser，密码 mypwd，添加设定：
\# 这个帐号使用的代理服务器和用户名/密码
http_proxy=”http://myuser:mypwd@mycache.mydomain.com:3128″
export http_proxy
然后直接wget[proxy.mimvp.com](http://proxy.mimvp.com/)或者curl[proxy.mimvp.com](http://proxy.mimvp.com/)， 就是通过代理来访问[米扑代理](http://proxy.mimvp.com/)


