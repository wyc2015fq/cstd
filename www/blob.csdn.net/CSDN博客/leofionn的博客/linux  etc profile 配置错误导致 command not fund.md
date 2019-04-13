
# linux -etc-profile 配置错误导致 command not fund - leofionn的博客 - CSDN博客


2018年03月06日 12:32:48[leofionn](https://me.csdn.net/qq_36142114)阅读数：69个人分类：[centos																](https://blog.csdn.net/qq_36142114/article/category/7383246)


第一步：\# export PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin
第二部：\#/bin/vi /etc/profile
第三部：执行完以上的命令后，你可以重新执行 source /etc/profile命令来让你的配置文件生效
【来自@若泽大数据】


