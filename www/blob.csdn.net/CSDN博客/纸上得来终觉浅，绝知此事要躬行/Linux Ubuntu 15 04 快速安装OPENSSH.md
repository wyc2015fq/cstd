# Linux Ubuntu 15.04 快速安装OPENSSH - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月17日 09:41:01[boonya](https://me.csdn.net/boonya)阅读数：1014








Linux Ubuntu 15.04 默认未安装openssh-server.



安装命令：



`$sudo apt-get install openssh-server`

中途提示是否继续:y






启动ssh-server：
`$sudo /etc/init.d/ssh restart`


确认ssh-server已经正常工作： netstat -tlp




安装好了之后就可以直接用XShell进行测试了。

![](https://img-blog.csdn.net/20170317094005408)




