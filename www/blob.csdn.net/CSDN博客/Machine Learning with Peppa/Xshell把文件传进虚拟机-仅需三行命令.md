# Xshell把文件传进虚拟机-仅需三行命令 - Machine Learning with Peppa - CSDN博客





2019年03月13日 22:54:14[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：361








*补充：目前没办法支持文件夹的互传，建议打包了之后再解压缩。*

这个方法比较简单，一共分4步，XShell5的用户要多一条-第2步，5以上版本的不需要，因为安装的时候已经自动配置好了。另外请注意，用Xshell配置和连接虚拟机时，请不要关闭虚拟机！不然会报错，且之前的设置会无效。每一次使用XSHELL时请务必记住要先开虚拟机和虚拟机网络。



1、首先在Linux系统的terminal终端中输入ifconfig命令查看主机IP，然后使用Xshell在文件目录下点击新建，在弹出的新建会话属性中输入主机IP，用户名和密码后，便可以连接到Linux系统，并对其进行操作。

![](https://img-blog.csdnimg.cn/20190314145204548.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0,size_16,color_FFFFFF,t_70)

2、（XSHELL6的用户可以忽略这一步）在Linux主机上，安装上传下载工具包rz及sz，使用yum install -y 包名 进行安装。

lrzsz包安装完成后包括上传rz、下载sz命令；只需在root下（使用su命令，输入密码即可进入）使用如下命令进行安装该包即可。

```bash
[root@localhost src]# yum install -y lrzsz
```

3、从Windows上传文件，上传命令为rz；从Linux上传文件指令为rz，上传的文件在当前命令行的目录下；

```bash
[root@localhost src]# rz
```

在Xshell命令行中输入rz命令后，会弹出对话框，选择你要上传的文件，选择打开要上传的文件就上传到Linux主机下。

4、从Linux主机下载文件，下载命令为sz ，后面也可跟要下载的文件名，也可以选择下载的保存文件夹；只输入sz命令时，文件一般保存在当前路径下，上传完可以使用ls 查看。

```bash
[root@localhost src]# sz
```

此方法相对来说比较简单快捷，可以满足日常需要。

![](https://img-blog.csdnimg.cn/20190314145236598.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0,size_16,color_FFFFFF,t_70)

此外更改传输设置等，进入新会话的property里就行，如下：

![](https://img-blog.csdnimg.cn/20190314145338692.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0,size_16,color_FFFFFF,t_70)






