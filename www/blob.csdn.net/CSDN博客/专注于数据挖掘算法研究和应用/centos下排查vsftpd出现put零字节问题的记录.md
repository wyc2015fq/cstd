# centos下排查vsftpd出现put零字节问题的记录 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年08月25日 09:42:26[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2465








1、问题：

     文件put到ftp服务器卡住，查看log(tail -n1000 /var/log/vsftpd.log |more)，发现客户端put的文件是0字节；

2、查核：

      1）用户和目录权限：/etc/vsftpd/vsftpd.conf上配置的vsftp用户具有/data/目录的写权限；

       2）启动vsftpd服务进程：/etc/init.d/vsftpd start，确认selinux已关闭；

       3） 其他客户端put文件成功，排除排除防火墙21端口开放、20端口未开放的可能；

             中间put出现553错误，发现put带目录上传失败，但如果lcd到本地目录再上传就可以；

              如put /tmp/test.txt失败，但先lcd /tmp，再put test.txt成功；即不能带目录上传。

      4） 查看空间：du -sk -h /data/空间已用8.1T，空间足够；

                              df -h 将/data1到/data12的空间软挂给/data；




3、正常下ftp客户端进入binary和被动模式，并不带目录的put，可以成功上传文件。

      无法重现上传零字节文件错误的客户端情况，需要检查上传脚本，并检查防火墙。有待进一步查核并处理。



