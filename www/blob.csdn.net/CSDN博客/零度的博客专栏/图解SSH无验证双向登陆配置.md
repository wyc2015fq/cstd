# 图解SSH无验证双向登陆配置 - 零度的博客专栏 - CSDN博客
2016年10月11日 11:18:10[零度anngle](https://me.csdn.net/zmx729618)阅读数：389标签：[ssh																[hadoop](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)](https://so.csdn.net/so/search/s.do?q=ssh&t=blog)
个人分类：[Hadoop](https://blog.csdn.net/zmx729618/article/category/6452458)
        配置SSH无登陆验证，在很多场景下是非常方便的，尤其是在管理大型集群服务时，避免了繁琐的密码验证，在安全级别越高的服务器上，通常密码的设置更复杂，配置SSH，不仅可以用密钥保证节点间通信的安全性，同时也降低了频繁输入密码登陆的耗时，大大提高了管理效率。散仙写此篇文章，也是给准备入手Hadoop的道友们先做好一个基础的准备，当然你也可以不配置SSH，只要你愿意频繁输入Slave节点的密码来登陆。 
        1. SSH无密码验证的原理 
        Master作为客户端，要实现无密码公钥认证，连接到服务器Salve上时，需要在Master上生成一个密钥对，包括一个公钥和一个私钥，而后将公钥复制到所有的Salve上。当Master通过SSH链接到Salve上时，Salve会生成一个随机数并用Master的公钥对随机数进行加密，并发送给Master。Master收到加密数之后再用私钥解密，并将解密数回传给Salve，Salve确认解密数无误之后就允许Master进行连接了。这就是一个公钥认证过程，期间不需要手工输入密码，重要的过程是将Master上产生的公钥复制到Salve上。
       2.SSH无密码登陆的几种关系 
       通常情况下，一个集群服务下至少有一个Master和若干个Slave ，那么无密码登陆通常指的是由Master到任意一个Slave的无验证的单向登陆，意思就是只能从Master登陆到Slave是不需要密码的，但是如果你想从Slave无验证登陆到Master，或者你想在Slave与Slave之间进行无验证登陆，这些都是不可行的，除非，你进行了密钥对的双向验证，才可以双向登陆，我们在这里先不去议论相互之间登陆有没有意义，可能某些情况下或许需要这些方式。
|节点名|IP地址|
|----|----|
|Master|10.2.143.5|
|Slave|10.2.143.36|
        下面开始步入正题，散仙使用的是CentOS6.4版本的，配置的是2个节点之间的双向的SSH无验证登陆，其他几种系统的也大同小异，我们首先使用root用户登陆，在network中修改机器名，并在hosts文件中添加映射信息，然后执行保存退出，Slave机按同样方法配置，具体操作见下图：
![](http://dl2.iteye.com/upload/attachment/0090/2045/618cc1be-6278-3997-82ff-2c79cfbd2c33.jpg)
![](http://dl2.iteye.com/upload/attachment/0090/2063/c93e6551-7f69-3504-a1a7-a0b0835dc18e.jpg)
![](http://dl2.iteye.com/upload/attachment/0090/2065/17fe3ff1-3555-34d5-99fc-7bae9d8b2863.jpg)
![](http://dl2.iteye.com/upload/attachment/0090/2067/d4fafa00-e9de-3906-83fb-3e7bbddc632c.jpg)
然后我们在Master，Slave机上分别用root用户建一个hadoop用户，并设置密码，注意用户名，密码保持一致。 
![](http://dl2.iteye.com/upload/attachment/0090/2079/474dc551-f0dd-3787-bba5-b3b7d8e17b8e.jpg)
    然后，登入hadoop用户，执行以下命令，生成密钥对，并把公钥文件写入授权文件中，并赋值权限， 
    ssh-keygen –t rsa –P '' 
    cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys 
    chmod 600 ~/.ssh/authorized_keys 
    最后切换root用户，配置sshd，取消被注释的公钥字段， 
    RSAAuthentication yes # 启用 RSA 认证 
    PubkeyAuthentication yes # 启用公钥私钥配对认证方式 
    AuthorizedKeysFile .ssh/authorized_keys # 公钥文件路径（和上面生成的文件同） 
并保存设置，然后重启sshd，即可测试本机的SSH，如下图所示。 
![](http://dl2.iteye.com/upload/attachment/0090/2091/87c55ecc-a25d-3f77-9ae5-b57ff74fe27c.jpg)
![](http://dl2.iteye.com/upload/attachment/0090/2095/377cc726-670e-3314-bcde-855c693bf9b3.jpg)
![](http://dl2.iteye.com/upload/attachment/0090/2097/92582878-e7d7-30f7-93f0-a2d359ab71f2.jpg)
![](http://dl2.iteye.com/upload/attachment/0090/2099/76aeb219-4ff7-30b1-bb90-3be491b7f46e.jpg)
至此，我们本机的SSH已经配置完毕，下面给出SSH登陆本机图 
![](http://dl2.iteye.com/upload/attachment/0090/2103/4e9b9c4d-b6a6-39e2-ba20-af706386b5f3.jpg)
       在上图中，我们可以看出，无论使用localhost，还是IP地址，或者是主机名，我们都可以顺利的进行本机的无验证登陆。 下面，散仙进行把Master的公钥文件通过scp拷贝到已经创建好的Slave节点的hadoop用户上，需要注意的是，在这个用户上不一定有.ssh文件夹，如果没有的话，也没关系，创建一个.ssh文件夹，并赋予700的管理权限，最后将公钥追加到授权文件中，并赋予600的权限，这两步比较重要，切记！
![](http://dl2.iteye.com/upload/attachment/0090/2143/a3bb3261-dd75-3e9d-9ef6-5765d08aed1e.jpg)
拷贝完成之后，去Slave机上进行，公钥追加授权文件，并赋值权限，然后切换root用户，进行sshd配置，并重启ssh服务，步骤，如Master机的配置。 
![](http://dl2.iteye.com/upload/attachment/0090/2155/31ad9fb5-a533-3e8d-bd92-c5917d893403.jpg)
![](http://dl2.iteye.com/upload/attachment/0090/2159/fe6f0637-0ee0-3325-993f-436acf6037a7.jpg)
然后，回到Master机的hadoop用户上，进行测试 
![](http://dl2.iteye.com/upload/attachment/0090/2161/8f8f3106-5b54-301b-a8a4-7dd54a74381d.jpg)
![](http://dl2.iteye.com/upload/attachment/0090/2165/35166f31-bfbd-3093-968c-241f0903adfa.jpg)
        我们发现此时，已经不需要密码验证了。当然现在只是单向登陆从Master到Slave的可以，如果从Slave到Master的你会发现不行，这个是正常的，想要双向登陆，必须得两台机器互相认证彼此的公钥文件，以此类推无论是你有多少台节点，只要机器双方之间有认证，就可以任意实现无验证登陆，当然在我们实际的生产环境中，只考虑有意义的认证，下面散仙在Slave节点下的hadoop用户里，生成自己的公钥文件，并用scp拷贝到Master上，然后将公钥追加的授权文件中，以此实现双向认证。
Slave机上的hadoop用户生成密钥对 
![](http://dl2.iteye.com/upload/attachment/0090/2188/81830297-5b66-39ef-9e7f-85206933081f.jpg)
Slave机上拷贝公钥到Master机上的hadoop用户上 
![](http://dl2.iteye.com/upload/attachment/0090/2191/e945c961-855a-3834-8604-3eabfb307629.jpg)
然后与上面相同的方式追加到授权文件authorized_keys里面。然后就大功搞成了，散仙给出截图证明双向无验证登陆。 
Master机上hadoop用户的双向认证 
![](http://dl2.iteye.com/upload/attachment/0090/2208/c386bfc5-735e-3a47-8f1b-879daf4a12db.jpg)
Slave机上hadoop用户的双向认证 
![](http://dl2.iteye.com/upload/attachment/0090/2210/68cbdabd-6c8a-3553-bb47-8819813e3ec8.jpg)
       至此，我们的双向无登陆验证，已经搞定了，对于多节点的，也是这个道理，只有节点之间都有相互的公钥认证，就可以双向认证了，如果是单向管理的，只需要配置一方的公钥就可以了，当然，在实际的环境中，大部分都是单向的。这个我们要根据实际的情况来处理。
