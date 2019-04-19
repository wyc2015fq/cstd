# FTP服务FileZilla Server上传提示550 Permission denied - xqhrs232的专栏 - CSDN博客
2016年03月23日 11:22:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：26915
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址：[https://help.aliyun.com/knowledge_detail/5989224.html](https://help.aliyun.com/knowledge_detail/5989224.html)
相关文章
1、[filezilla通过root账户远程连接管理ubuntu
 server服务器文件](http://www.07net01.com/2014/09/137077.html)----[http://www.07net01.com/2014/09/137077.html](http://www.07net01.com/2014/09/137077.html)
2、FileZilla无法上传某些文件----[http://www.suiyuela.com/read/130331141423.shtml](http://www.suiyuela.com/read/130331141423.shtml)
在Windows系统服务器中使用FTP向FileZilla Server上传文件时提示"550 Permission denied"错误时（如下图），请确保在FileZilla Server中给相应的FTP账号设置了写入权限。
![ftp.png](https://img.alicdn.com/tfscom/TB1643FHXXXXXXpXVXXXXXXXXXX.png)
具体设置方法参见下图：
![QQ截图20150212182249.png](https://img.alicdn.com/tfscom/TB1.qgLHXXXXXadXpXXXXXXXXXX.png)
如问题还未解决,请联系[售后技术支持](https://workorder.aliyun.com/addIndex.htm)。
//========================================================================
备注：
1>今天也遇到怎样的问题，原来Filezilla有2种协议可选，一种是FTP，另外一种是SSH，选择SSH就可以把文件上传到服务器端了
