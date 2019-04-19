# ssh登陆之忽略known_hosts文件 - xqhrs232的专栏 - CSDN博客
2017年06月13日 15:51:40[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：227
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/yasaken/article/details/7348441](http://blog.csdn.net/yasaken/article/details/7348441)
在平时工作中，有时候需要SSH登陆到别的[Linux](http://lib.csdn.net/base/linux)主机上去，但有时候SSH登陆会被禁止，并弹出如下类似提示：
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ WARNING: REMOTE HOST IDENTIFICATION HAS CHANGED! @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@IT IS POSSIBLE THAT SOMEONE IS DOING SOMETHING
 NASTY!Someone could be eavesdropping on you right now (man-in-the-middle attack)!It is also possible that the RSA host key has just been changed.The fingerprint for the RSA key sent by the remote host is36:68:a6:e6:43:34:6b:82:d7:f4:df:1f:c2:e7:37:cc.Please
 contact your system administrator.Add correct host key in /u/xlian008/.ssh/known_hosts to get rid of this message.Offending key in /u/xlian008/.ssh/known_hosts:2RSA host key for 135.1.35.130 has changed and you have requested strict checking.Host key verification
 failed.
比较奇怪，于是研究了一下。
ssh会把你每个你访问过计算机的公钥(public key)都记录在~/.ssh/known_hosts。当下次访问相同计算机时，OpenSSH会核对公钥。如果公钥不同，OpenSSH会发出警告， 避免你受到DNS Hijack之类的攻击。我在上面列出的情况，就是这种情况。
原因：一台主机上有多个Linux系统，会经常切换，那么这些系统使用同一ip，登录过一次后就会把ssh信息记录在本地的~/.ssh/known_hsots文件中，切换该系统后再用ssh访问这台主机就会出现冲突警告，需要手动删除修改known_hsots里面的内容。
有以下两个解决方案：
1. 手动删除修改known_hsots里面的内容；
2. 修改配置文件“~/.ssh/config”，加上这两行，重启服务器。
   StrictHostKeyChecking no
   UserKnownHostsFile /dev/null
优缺点：
1. 需要每次手动删除文件内容，一些自动化脚本的无法运行（在SSH登陆时失败），但是安全性高；
2. SSH登陆时会忽略known_hsots的访问，但是安全性低；
