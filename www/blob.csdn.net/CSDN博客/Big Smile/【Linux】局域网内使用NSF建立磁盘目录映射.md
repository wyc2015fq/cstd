# 【Linux】局域网内使用NSF建立磁盘目录映射 - Big Smile - CSDN博客
2018年04月13日 00:38:05[王啸tr1912](https://me.csdn.net/tr1912)阅读数：467
        由于项目发布时候需要搭建负载均衡的集群，所以我们的项目部署在了4个服务器上，但是由于上传的图片和文件是共享的，所以又单独拿出一个服务器当做了文件服务器，用于后台系统的部署和图片，音频的放置。
        由于对于视频和音频的截取需要一个类似的本地环境作为文件读写的平台，所以我决定把第5台文件服务器上存储文件的地方映射到其他4个服务器的目录上。
环境：
以两个服务器为例子：（a：192.168.21.1,b：192.168.21.2）
都需要安装nsf的包：yum install nfs-utils portmap
# 一、安装配置服务端
        假设服务端a（文件服务器）地址：192.168.21.1
        在/usr/local/uploadFile 是需要建立映射的文件夹（存储文件的地方），我们需要进行以下操作：
```java
1、编辑/etc/exports文件
```
加入以下内容：
`/usr/local/uploadFile 192.168.21.2(rw,sync,no_root_squash)  `
其中的IP地址是需要建立映射的b服务器的地址，表示允许b服务器访问，rw表示为有读写权限。
保存后，启动我们的两个服务，注意一定要先启动portmap后启动nfs：
```
service rpcbind start  
service nfs start
```
我们可以用：
`showmount -e`
然后我们可以看见已经可以被挂在的本地路径和挂载权限了。
# 二、挂载端配置
        安装好nfs的包之后，可以直接启动nfs和portmap
```
service rpcbind start  
service nfs start
```
        然后在服务器上建立要映射到的文件夹，依旧是/usr/local/uploadFile
`mkdir /usr/local/uploadFile`
为了保险起见提升我们建立的文件夹权限：
`chmod -r 777 /usr/local/uploadFile`
然后我们挂载a服务器的文件路径到b服务器下：
`mount -t nfs 192.168.21.1:/usr/local/uploadFile  /usr/local/uploadFile`
这样就可以了。
# 三、其它
        如果我们想要解除映射，只需要用unmount命令就可以了：
`umount /usr/local/uploadFile`
        如果碰到了mount.nfs: access denied by server while mounting  的这种错误，八成是映射源文件夹的权限，或者是映射目的文件夹的权限不够造成的，用chmod -r 777 去提权，或者加入sudo 命令去提权就好了。
如果遇到了refused mount request from  。。。的问题，八成是防火墙，或者一些别的限制原因把nsf所需要的端口占用或者限制了，这个时候我们只需要开启相应的端口就好了。
端口问题详见：[https://blog.csdn.net/armeasy/article/details/6005703](https://blog.csdn.net/armeasy/article/details/6005703)
