# linux 下VSFTP 530 Permission denied错误的解决方法 - xqhrs232的专栏 - CSDN博客
2016年03月23日 10:59:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：4338
原文地址：[http://www.jb51.net/LINUXjishu/182697.html](http://www.jb51.net/LINUXjishu/182697.html)
相关文章
1、[FTP服务FileZilla
 Server上传提示550 Permission denied](https://help.aliyun.com/knowledge_detail/5989224.html#FTP%E6%9C%8D%E5%8A%A1FileZilla%20Server%E4%B8%8A%E4%BC%A0%E6%8F%90%E7%A4%BA550%20Permission%20denied)----[https://help.aliyun.com/knowledge_detail/5989224.html](https://help.aliyun.com/knowledge_detail/5989224.html)

虚拟机装好RedHat后，准备使用filezilla连接,输入IP地址，root用户，密码，快速连接，报错：
530 Permission denied。
故障排除：
**1.首先检查系统是否开启了vsftp服务，如果没有开启，先开启该服务。**
  方法1.setup--系统服务--自启动服务
  方法2.界面设置，service vsftpd restart
**2.查看配置**
vsftpd的配置，配置文件中限定了vsftpd用户连接控制配置。
vsftpd.ftpusers：位于/etc目录下。它指定了哪些用户账户不能访问FTP服务器，例如root等。
vsftpd.user_list：位于/etc目录下。该文件里的用户账户在默认情况下也不能访问FTP服务器，仅当vsftpd .conf配置文件里启用userlist_enable=NO选项时才允许访问。
vsftpd.conf：位于/etc/vsftpd目录下。来自定义用户登录控制、用户权限控制、超时设置、服务器功能选项、服务器性能选项、服务器响应消息等FTP服务器的配置。
**3.配置修改完成后，执行service vsftpd restart重启vsftpd服务。**
