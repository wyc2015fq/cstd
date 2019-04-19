# ssh无密码登录和autossh实现反向ssh - xqhrs232的专栏 - CSDN博客
2017年06月16日 11:53:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：368
原文地址::[http://blog.csdn.net/fanwenjieok/article/details/53033317](http://blog.csdn.net/fanwenjieok/article/details/53033317)
ssh无密码登录和autossh实现反向ssh
    一. ssh无密码登录的方法(以A机器登录B机器为例)
    1.  使用ssh-keygen生成private key和public key，具体方法如下：
        在机器A上执行 ssh-keygen -t [rsa|dsa]，将会生成密钥文件和私钥文件 id_rsa,id_rsa.pub或id_dsa,id_dsa.pub
    2. 采用ssh-copy-id将key文件copy到B机器
        继续执行 ssh-copy-id  username@ip  -p xxxx
        其中， username是机器B的登录用户名，ip是机器B的IP，-p是指定ssh的端口(默认是22，如果是VPS，则不一定是22)
    3. 然后提示你输入登录机器B的密码，完成后下次就可以直接采用ssh  -p xxxx  username@remote_ip的方式来无密码登录机器B了。
    二.  autossh实现反向ssh
    应用场景：购买一台具有公网IP的VPS，然后在任何一个地方通过ssh登录到该VPS，然后再反向ssh可以登录到公司里的机器，这样就可以在家里面来工作了，前提是公司的[Linux](http://lib.csdn.net/base/linux)机器一直在开机。
    1. 假设VPS是机器B，公司里的机器是A，通过上面的方式首先在机器A上配置ssh无密码登录到B。
    2. 采用autossh来建立一条机器A和B的连接，具体方法是：
**[html]**[view
 plain](http://blog.csdn.net/fanwenjieok/article/details/53033317#)[copy](http://blog.csdn.net/fanwenjieok/article/details/53033317#)
- autossh -M 5678 -NR 8888:localhost:22    user@xxx.xxx.xxx.xxx  -p xxxx  
     上面命令行中“8888:localhost:22”表示我在VPS上反向ssh时要登录的主机是机器A，端口是8888， 后面的"user@xxx.xxx.xxx.xxx  -p
 xxxx"表示我的VPS用户名是user，-p是指定ssh的端口。
     注意：上面的命令是阻塞的，所以在后面加上 & 来在后台执行。
    3. 登录机器B上，执行： ssh  localhost  -p 8888，就可以登录到机器A了。
    因为机器A是公司里的服务器，且在公司内网里面可以访问公司其它机器，平时工作的程序等资源都在机器A上面，所以工作起来比较方便，但有时候不一定每天都去公司，想在空里办公，那怎么办呢？ 所以购买一台VPS，也就是机器B,它具有公网IP，所以可以在家里登录到B上，再通过机器B反向ssh到公司里的机器A，这样就可以愉快的干活啦。[linux](http://lib.csdn.net/base/linux)下玩起来好爽的感觉。
