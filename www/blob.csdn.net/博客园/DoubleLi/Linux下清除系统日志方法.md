# Linux下清除系统日志方法 - DoubleLi - 博客园





|**摘要：相信大家都是用过Windows的人。对于Windows下饱受诟病的各种垃圾文件都需要自己想办法删除，不然你的系统将会变得越来越大，越来越迟钝！windows怎么清理垃圾相信大家都知道的，那么linux下怎么清理系统垃圾呢？**|
|----|

 Linux的文件系统比Windows的要优秀，不会产生碎片，对于长时间运行的服务器来说尤为重要，而且Linux系统本身也不会像Windows一样产生大量的垃圾文件。不知道这个说法有没有可信度！至少我们可以确定的是linux系统的文件系统是比较优秀的！

**错误**

rm -f logfile  

**原因**

应用已经打开文件句柄，直接删除会造成：

       1.应用无法正确释放日志文件和写入

       2.显示磁盘空间未释放

**正确**

cat /dev/null > logfile

把一下代码保存为.sh后缀脚本来运行即可清除Linux系统日志

#!/bin/sh 
cat /dev/null > /var/log/syslog 
cat /dev/null > /var/adm/sylog 
cat /dev/null > /var/log/wtmp 
cat /dev/null > /var/log/maillog 
cat /dev/null > /var/log/messages 
cat /dev/null > /var/log/openwebmail.log 
cat /dev/null > /var/log/maillog 
cat /dev/null > /var/log/secure 
cat /dev/null > /var/log/httpd/error_log 
cat /dev/null > /var/log/httpd/ssl_error_log 
cat /dev/null > /var/log/httpd/ssl_request_log 
cat /dev/null > /var/log/httpd/ssl_access_log

**另外清理linux系统垃圾还有以下命令**

sudo apt-get autoclean 清理旧版本的软件缓存
sudo apt-get clean 清理所有软件缓存
sudo apt-get autoremove 删除系统不再使用的孤立软件









