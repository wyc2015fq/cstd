# NFS服务搭建 - weixin_33985507的博客 - CSDN博客
2017年11月27日 00:16:21[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
ubuntu下搭建nfs服务
参考文档：
[x] [http://www.cnblogs.com/mchina...](http://www.cnblogs.com/mchina/archive/2013/01/03/2840040.html)
[x] [http://www.linuxidc.com/Linux...](http://www.linuxidc.com/Linux/2016-04/129848.htm)
## **NFS服务器端**
1.sudo apt-get install nfs-kernel-server
2.sudo mkdir /home/share/nfs 建立nfs共享文件夹。
3.sudo vi /etc/exports 配置nfs。
4.在文档的最后一行加入/home/song/nfs *(rw,sync,no_root_squash,no_subtree_check)，保存退出。
sudo /etc/init.d/rpcbind restart 重启rpcbind
sudo /etc/init.d/nfs-kernel-server restart 重启nfs
## **NFS客户端**
- sudo apt-get install nfs-common
- 
sudo service portmap stop
`sudo service portmap start`
- mkdir /mnt/data
sudo mount -t nfs 192.168.1.122:/home/share/nfs  /mnt/data
- 问题：如果出现mount.nfs access denied
```
只要在服务器端 /etc/exports 中配置中加上insecure
         /home/song/nfs *(rw,sync,insecure,no_root_squash,no_subtree_check)
```
[x] 参考文档：[http://blog.chinaunix.net/uid...](http://blog.chinaunix.net/uid-20554957-id-3444786.html)
