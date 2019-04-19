# SIOCADDRT: File exists - xqhrs232的专栏 - CSDN博客
2017年01月04日 08:31:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2393
原文地址::[http://blog.csdn.net/carolzhang8406/article/details/6201504](http://blog.csdn.net/carolzhang8406/article/details/6201504)
相关文章
1、[route: SIOCADDRT: File exists 问题](http://blog.chinaunix.net/uid-8048969-id-3369102.html)----[http://blog.chinaunix.net/uid-8048969-id-3369102.html](http://blog.chinaunix.net/uid-8048969-id-3369102.html)
2、RF 6 SP2 下无线网络设置----[http://www.linuxdiyf.com/viewarticle.php?id=158446](http://www.linuxdiyf.com/viewarticle.php?id=158446)
# 添加路由遇到的问题：SIOCADDRT: File exists
原因：路由已经存在了。
解决方法：
从网上搜到的：
1. Open ifcfg file `vi /etc/sysconfig/network-scripts/ifcfg-eth0:7`
2. **NETWORK=217.174.242.0** Make sure this address is unique to each ifcfg file; if you use the same address in another ifcfg file you will receive the aforementioned error.
3. Edit if required by pressing `i` to begin insert mode, make the changes, then pressing `Esc `to return to view mode.
3. Save the document by using a `SHIFT ZZ` combination.
4. Restart the network: `service network restart`
