
# svn使用常识 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月03日 09:01:21[seven-soft](https://me.csdn.net/softn)阅读数：187个人分类：[项目管理																](https://blog.csdn.net/softn/article/category/6294553)



# (1)svn服务器，安装TortoiseSVN客户端，提示：svn:MKACTIVITY ... 403 Forbidden 。
原因：SVN在Check out 等操作时，是URL大小写不敏感的，可在Commit /delete时却是在大小写敏感的，所以在设定SVN的服务器路径时，一定要注意大小写。
通俗一点讲，Check out时，URL的大小写无所谓，但是在提交、合并、删除等操作时，就必须要大小写匹配，否则就会报以上的错误。


