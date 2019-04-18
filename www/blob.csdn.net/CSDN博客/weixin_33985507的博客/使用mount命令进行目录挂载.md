# 使用mount命令进行目录挂载 - weixin_33985507的博客 - CSDN博客
2018年06月30日 15:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
**在进行mount目录之前，先检查服务器是否已经启动rpcbind服务和nfs服务，mount命令是基于这两个服务实现目录挂载的。**
如果应用服务器需要mount其他的文件服务器共享目录，首先用root的用户登录文件服务器，
打开/etc/exports文件，添加
/data/app/tkmFile 132.12.30.222(rw,all_squash,anonuid=501,anongid=100)
其中anonuid=501是文件服务器有权限访问这个目录的用户ID，anongid=100是组ID
保存完后，执行exportfs -rv，
如果还是不生效可重启nfs服务
/etc/init.d/nfs restart
**查看共享目录命令**
showmount -e ip
然后用root的用户登录应用服务器，执行以下命令：
mount -t nfs -o rw 132.96.63.5:/data/app/tkmFile  /data/app/tkmFile
#umount /mnt  --mnt 文件夹
mount -t nfs -o rw 221.28.3.7:/data/szkmc  /data/szkmc -v
(如果无法挂载成功检查防火墙问题，nfs服务，ip网段是否做了限制，即使ping得通)
