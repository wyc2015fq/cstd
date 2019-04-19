# Redhat下安装yum遇到的错误 - xqhrs232的专栏 - CSDN博客
2016年04月15日 09:57:03[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1357
原文地址::[http://blog.sina.com.cn/s/blog_7983e5f10100q5om.html](http://blog.sina.com.cn/s/blog_7983e5f10100q5om.html)
相关文章
1、linux下yum安装及配置----[http://jingyan.baidu.com/article/ed2a5d1f5a9fbe09f6be17ea.html](http://jingyan.baidu.com/article/ed2a5d1f5a9fbe09f6be17ea.html)
2、
## linux系统安装yum环境
 ----[http://blog.sina.com.cn/s/blog_63d8dad80101cn2s.html](http://blog.sina.com.cn/s/blog_63d8dad80101cn2s.html)
yum
[http://yum.baseurl.org/download/2.0/](http://yum.baseurl.org/download/2.0/)
下载
第一次安装：
[root@vod-web soft]# rpm -ivh yum-2.4.2-1.src.rpm
warning: yum-2.4.2-1.src.rpm: V3 DSA signature: NOKEY, key ID 69886cc7
warning: user skvidal does not exist - using root
warning: group skvidal does not exist - using root
warning: user skvidal does not exist - using root
warning: group skvidal does not exist - using root
   1:yum                    ########################################### [100%]
于是新建skvidal组和用户：
[root@vod-web soft]# groupadd skvidal
[root@vod-web soft]# useradd -g skvidal -d /usr/skvidal skvidal
再次安装：
[root@vod-web soft]# rpm -ivh yum-2.4.2-1.src.rpm
warning: yum-2.4.2-1.src.rpm: V3 DSA signature: NOKEY, key ID 69886cc7
   1:yum                    ########################################### [100%]
运行
[root@vod-web soft]# yum
-bash: yum: command not found
还是命令未发现
无结果，看来没有安装成功
执行
rpmbuild --rebuild yum-2.4.2-1.src.rpm
然后到 /usr/src/redhat/  去找找 rpm 包中的 yum-2.4.2-1.src.rpm
再安装成功
