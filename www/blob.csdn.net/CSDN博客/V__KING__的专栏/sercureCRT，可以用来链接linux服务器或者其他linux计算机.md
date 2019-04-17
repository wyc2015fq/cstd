# sercureCRT，可以用来链接linux服务器或者其他linux计算机 - V__KING__的专栏 - CSDN博客





2013年11月07日 21:50:59[v__king__](https://me.csdn.net/V__KING__)阅读数：888
个人分类：[linux](https://blog.csdn.net/V__KING__/article/category/1733387)









Windows端：


      安装sercureCRT，可以用来链接服务器或者其他计算机

linux端：



安装OpenSSH Server：
sudo [apt-get](http://zhidao.baidu.com/search?word=apt-get&fr=qb_search_exp&ie=utf8) install openssh-server
如果提示依赖对象openssh-client，则需要先移除client再安装：
sudo [apt-get](http://zhidao.baidu.com/search?word=apt-get&fr=qb_search_exp&ie=utf8) remove openssh-client
sudo [apt-get](http://zhidao.baidu.com/search?word=apt-get&fr=qb_search_exp&ie=utf8) install openssh-server（会将client与server一并装上）。
最后：
sudo /etc/init.d/ssh start 开启 sshd 服务








