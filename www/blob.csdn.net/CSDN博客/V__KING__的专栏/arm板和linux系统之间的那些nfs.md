# arm板和linux系统之间的那些nfs - V__KING__的专栏 - CSDN博客





2017年01月11日 11:14:59[v__king__](https://me.csdn.net/V__KING__)阅读数：361








# 一. arm板和linux系统之间的那些nfs

## nfs 服务配置
- sudo apt-get install nfs-kernel-server
- vi  /etc/exports
`/home/zyp/rootfs  *(rw,sync,no_root_squash)`- 修改挂载点的访问权限 

chmod 777 /home/zyp/rootfs
- sodu /etc/init.d/nfs-kernel-server restart 

sudo service nfs-kernel-server restart

## nfs 客户端

mount 127.0.0.1:/home/zyp/nfsroot     /mnt

# 二. arm板和windows系统之间

可以让arm 去挂载windows下的一个共享文件夹



