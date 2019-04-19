# Permissions 0644 for ‘/root/.ssh/id_rsa’ are too open处理 - xqhrs232的专栏 - CSDN博客
2017年06月23日 10:31:43[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：725
原文地址::[http://www.cppblog.com/gezidan/archive/2011/08/19/153826.html](http://www.cppblog.com/gezidan/archive/2011/08/19/153826.html)
相关文章
1、[Permissions 0644 for ‘/root/.ssh/id_rsa’ are too open处理](http://blog.csdn.net/menglei8625/article/details/7395087)----[http://blog.csdn.net/menglei8625/article/details/7395087](http://blog.csdn.net/menglei8625/article/details/7395087)
之前已经在freebsd主机登陆到dreamhost.com的ssh密钥实验是成功的，参考网址
http://wiki.dreamhost.com/SSH#Passwordless_Login
但今天尝试把密钥放到debian系统的/root/.ssh/id_rsa中，然后在debian的终端界面输入
ssh xxxx@xxxx.dreamhost.com
这样就出现 Permissions 0644 for ‘/root/.ssh/id_rsa’ are too open. 等错误显示了，经google查询下，原来只要把权限降到0600就ok了
输入命令
chmod 0600 /root/.ssh/id_rsa
然后再
ssh xxxx@xxxx.dreamhost.com
这样就可以密钥登陆了.
