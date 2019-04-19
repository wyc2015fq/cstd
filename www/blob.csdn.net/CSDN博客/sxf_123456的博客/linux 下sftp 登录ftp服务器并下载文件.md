# linux 下sftp 登录ftp服务器并下载文件 - sxf_123456的博客 - CSDN博客
2019年03月13日 20:00:21[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：53
1、使用sftp命令登录到ftp 服务器
sftp   -oport=端口   用户名@ftp服务器
之后出现 ![](https://img-blog.csdnimg.cn/20190313195820271.png)输入ftp密码，
2、使用 get 命令从 ftp服务器上将文件下载到本地
get  ftp服务器上文件   本地路径
