# centos 7 系统下进行多服务器的SSH免密码登录配置 - DEVELOPER - CSDN博客





2017年11月24日 14:05:05[学术袁](https://me.csdn.net/u012827205)阅读数：661
所属专栏：[Hadoop-学习](https://blog.csdn.net/column/details/20399.html)












学习ssh配置的目的就是为学习hadoop做铺垫。因为在进行hadoop完全分布式的大数据处理，需要用到客户端与各个服务器之间进行面密码登陆并配合来完成数据处理。

基于此，我就在网上搜索了几篇文章，尝试着去配置。“好事多磨”，从开始配置到我真正的配置成功，还是今天上午的事情。从前天到今天，两天半时间。经历了无数次的虚拟机中centos7系统的安装、测试。然后我就把我走过的曲折历程讲述一哈：

### 为什么使用ssh？

比如在进行大型的集群管理时候，可以避免繁琐的密码验证。这样，通过配置ssh，不仅可以保证各个节点（服务器）之间通信的安全性，而且不用频繁输入密码，提高管理效率；注意：我现在所说的是ssh的单向验证登陆，而非双向的。也就是说只能进行 master->slave1 、master->slave2 、 master->slaveX ，但是不能出现 slaveX->master or slaveX->slaveX' 的情况。除非，进行了密钥对的双向验证。




### 配置ssh的详细流程

本机系统是Windows 10

虚拟机是VMware 8


Linux系统环境是CentOS-7-x86_64-DVD-1708.iso





在这里我就在虚拟机上创建出3台虚拟服务器.

主机名分别是：h147、h148 和 h149

软件选择的是：基础设施服务器

三台服务器的管理员统一用户名：hadoop

|主机名|主机别名|用户名|IP地址|
|----|----|----|----|
|h147|master|hadoop|192.168.127.147|
|h148|slave1|hadoop|192.168.127.148|
|h149|slave2|hadoop|192.168.127.149|




















通过root权限修改hosts文件，配置主机别名与IP地址的映射关系

![](https://img-blog.csdn.net/20171124135920957?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


至此，主机h147中主机别名和IP地址的映射关系就配置好了《**slave2 对应的是192.168.127.149**》。然后分别登录h148和h149使用同样的方式进行配置。（意思就是修改h147、h148和h149的hosts一样就得）

在进行SSH配置之前有一点请注意：客户端发送到服务器端的密钥文件，一定要放到登录用户的主目录的.ssh/下面。用h147（master）、h148（slave1）和h149（slave2）这三台机器简单表示说就是：主机master上的一个管理员用户hadoop，在hadoop下生产密钥对，一定要放到目录 :/home/hadoop/.ssh 的下面。

为什么呢？因为免密码登录其实是通过文件中的一行代码：

![](https://img-blog.csdn.net/20171124141927275?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


 /etc/ssh/sshd_config 文件中红色框中的代码  （慢慢你就能感受到了）

好了，我们继续操作：

![](https://img-blog.csdn.net/20171124142917012?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


看到这里我们的密钥对就生成了！！



**注意：有的博客上说，（对于我目前的配置上）上图中的绿色的线框中 hadoop@h147 ， 实则应是 hadoop@master 不然整个配置不会成功。我已经验证过了，可以成功！其他博客说的不对！！**

**注意：上图黄色线框中的两次对于密钥的密码设置，不要输入密码，直接回车直到最后。因为这里是一个坑，当你输入密码之后，即使你的ssh都配置成功了。在你进行 master->slave 进行登录的时候，你会发现-他妈的还是需要输入密码。而不能做到免密码，当然了，让你做的密码验证登录是 rsa 的密码。这个坑我呆了一天半才出来。**

![](https://img-blog.csdn.net/20171124155031293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**注意：还有的博客说（上图），需要这种操作。我就很纳闷了，我当时就单纯的按照他们这种操作进行了，可是进入后发现，为什么我的这个文件中没有 #RAAAuthentication yes 这行代码？？心瞬间碎了一地！然后我就骄傲的自己粘了一行到这个文件中。继续，然后，最后，不成功！而且报的就是 这行代码的问题。而这里，这次配置根本没有对这个文件做任何处理，仍然是成功的！需要指出的是  三台服务器上的  AuthorizedKeysFile    .ssh/authorized_keys  这行代码必须是解注释状态，ta属于服务器文件，所有的免密码登录都是通过ta的匹配授权。**

![](https://img-blog.csdn.net/20171124155642733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


接着继续，成功生成了密钥对，进入到 /home/hadoop/.ssh/ 查看，可以看到 id_rsa 、 id_rsa.pub 两个文件。

通过命令



```java
$cat id_rsa.pub >> authorized_keys
```
将公钥上传到服务器。




通过命令



```java
$scp id_rsa_pub hadoop@slave1:/home/hadoop
```



将 主机master下 的 hadoop用户下目录文件 /home/hadoop/.ssh/id_rsa.pub 传递到 主机slave1下的hadoop用户下的目录 /home/hadoop/ 下面。
![](https://img-blog.csdn.net/20171124160919515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后我们可以看到，公钥 id_rsa.pub 已经从 h147 传递到 h148 的 /home/hadoop/ 目录下。然后再剪切到 /home/hadoop/.ssh/下面（h148的hadoop下 .ssh 目录是通过 mkdir 生成），执行 命令

```java
#cat id_rsa.pub >> authorized_keys
```
追加到文件authorized_keys中。



**注意：我最初为了图个简单，直接把h147中的 authorized_keys 传递到了 h148 对应的hadoop目录下。但是最后结果是失败的，原因可能是在slave中的 ./ssh 下也必须有文件 id_rsa.pub 导致。**

最后，在h147中的hadoop用户下执行命令



```java
$ssh hadoop@slave1
```
免密码登录成功！<如果是第一次会让你输入密码，之后就不用了>



**注意：还有一点就是有的博客说有时候需要更改 master 和 slave 的文件的权限，否则会导致无法密钥授权登录（免密码登录）， 也就是，.ssh 权限 700  .ssh/authorized_keys 权限 600 。我也遇到一次是在如果master和localhost一样时候，使用本地ssh登录需要修改权限，但是之后对各个节点的ssh登录就没遇到！本次的配置，我没有设置文件的权限，都是默认的。**

**然后，对于slave2的操作同slave1的操作！**


然后，记录一下遇到的下一个问题。但是不太确定广泛的情况是否是这样。只是在我的电脑上今天遇到了。

就是如果，我各个方面已经配置好了。但是使用master 对 slave进行ssh登录时候还是需要密码；结果我在slave上改变了 .ssh 的创建方式，并没有更改其权限则成功了了！

![](https://img-blog.csdn.net/20171125171319907?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20171125171331879?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




