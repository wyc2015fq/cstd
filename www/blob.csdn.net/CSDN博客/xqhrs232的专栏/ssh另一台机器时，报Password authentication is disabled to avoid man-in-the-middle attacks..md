# ssh另一台机器时，报Password authentication is disabled to avoid man-in-the-middle attacks. - xqhrs232的专栏 - CSDN博客
2017年05月02日 11:54:28[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1417
原文地址::[http://www.blogjava.net/anchor110/articles/418195.html](http://www.blogjava.net/anchor110/articles/418195.html)
相关文章
1、[好记心不如烂笔头，ssh登录　The
 authenticity of host 192.168.0.xxx can't be established.的问题](http://blog.csdn.net/kimsoft/article/details/5865418)----[http://blog.csdn.net/kimsoft/article/details/5865418](http://blog.csdn.net/kimsoft/article/details/5865418)
想ssh至192.168.1.35，报如下错误：
[root@Surf-BJ-CAC-233 pusher]# ssh 192.168.1.35
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
@    WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED!     @
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING NASTY!
Someone could be eavesdropping on you right now (man-in-the-middle attack)!
It is also possible that the RSA host key has just been changed.
The fingerprint for the RSA key sent by the remote host is
7c:c5:c4:7b:78:e3:13:60:4a:4d:ff:8c:46:3b:e6:0d.
Please contact your system administrator.
Add correct host key in /root/.ssh/known_hosts to get rid of this message.
Offending key in /root/.ssh/known_hosts:9
Password authentication is disabled to avoid man-in-the-middle attacks.
Keyboard-interactive authentication is disabled to avoid man-in-the-middle attacks.
Permission denied (publickey,password).
解决办法：
[root@Surf-BJ-CAC-233 pusher]# mv /root/.ssh/known_hosts /root/.ssh/known_hosts_bak
