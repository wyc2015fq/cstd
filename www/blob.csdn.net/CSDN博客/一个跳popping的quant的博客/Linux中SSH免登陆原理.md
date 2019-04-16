# Linux中SSH免登陆原理 - 一个跳popping的quant的博客 - CSDN博客





2017年08月31日 21:12:15[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：592








在配置Hadoop集群的时候，最后一步就是要设置ssh的免登陆了，而这个免登陆理解了，后面配置多台机器进行分布式就好说了。 

  首先，SSH 为 Secure Shell 的缩写，由 IETF 的网络小组（Network Working Group）所制定；SSH 为建立在应用层基础上的安全协议。SSH 是目前较可靠，专为远程登录会话和其他网络服务提供安全性的协议。在linux上，通过ls -la可以看到在~目录下有个.ssh的隐藏文件。 

  先看一下配置ssh免登陆的步骤： 

    1、生成ssh免登陆密钥 

    cd ~，进入到我的home目录 

    cd .ssh/ 

        ssh-keygen -t rsa （四个回车） 

    执行完这个命令后，会生成两个文件id_rsa（私钥）、id_rsa.pub（公钥） 

    2、将公钥拷贝到要免登陆的机器上 

    拷贝到本地机器 

       cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys 

    或 

    ssh-copy-id -i localhost  

        拷贝到其他机器 

        ssh-copy-id 192.168.8.88 拷贝到其他机器上
假设有serverA和serverB，他们两者要进行通信，那么A通过ssh登录的时候要输入B的密码，通过上面的方法后就可以面密码登录了。那么原理是什么呢？ 

   1、首先，在A上生成一对密钥，公钥和私钥。 

   2、A将公钥拷贝到了B上，并重命名为authorized_keys。 

   3、A向B发送一个连接请求，包括A的用户信息，ip。 

   4、B得到了A的信息后，在authorized_keys中进行查找，如果有相应的用户名和IP，B会随机生成一个字符串。 

   5、B将使用公钥（authorized_keys）对字符串进行加密，然后发送给A。 

   6、A得到来自B的消息后，会使用私钥进行解密，然后将解密后的字符串发送给B。 

   7、B接收到解密的字符串后会跟之前生成的随机字符串进行比对，如果一致就允许免登陆。




