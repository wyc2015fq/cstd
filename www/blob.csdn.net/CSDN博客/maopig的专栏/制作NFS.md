# 制作NFS - maopig的专栏 - CSDN博客
2012年03月22日 15:53:39[maopig](https://me.csdn.net/maopig)阅读数：1122标签：[ubuntu																[测试																[list																[终端																[n2](https://so.csdn.net/so/search/s.do?q=n2&t=blog)](https://so.csdn.net/so/search/s.do?q=终端&t=blog)](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)
最近学习NFS，用本地测试.
以下是我的测试过程
环境 ubuntu 10.4   vm 7.1
终端
ifconfig 得到 ubuntu资料
INET ADDR 192.168.0.4
BCAST   192.168.0.255
MASK 255.255.255.0
一 安装NFS
$ sudo apt-get install nfs-kernel-server
$ sudo apt-get install nfs-common
二 配置NFS
1 $ sudo dpkg-reconfigure portmap , 对Should portmap be bound to the loopback address? 选N
2 $ sudo   /etc/default/portmap 末行清除 "-i 127.0.0.1"
三 配置hosts.deny
$ sudo vi   /etc/hosts.deny
文本末添加
### NFS DAEMONS
   portmap: ALL
   lockd:     ALL
   mountd:   ALL
   rquotad: ALL
   statd:     ALL
四 配置hosts.allow
   $ sudo vi /etc/hosts.allow
文本末加入
### NFS DAEMONS
   portmap: 192.168.0.
   lockd:     192.168.0.
   rquotad: 192.168.0.
   mountd:   192.168.0.
   statd:      192.168.0.
$ sudo /etc/init.d/portmap restart 重启portmap daemon.
显示如下
Since the script you are attempting to invoke has been converted to an
Upstart job, you may also use the restart(8) utility, e.g. restart portmap
portmap start/running, process   XXXX
五 配置/etc/exports
$ sudo vi /etc/exports
文本末添加
/home/work *(rw,sync,no_root_squash)
/home/gxj/IPNC2.6/ *(rw,nohide,insecure,no_subtree_check,async,no_root_squash)
chmod 777 -R /home/work
$ sudo showmount -e
显示 clnt-create : RPC : Program not registered
$ sudo   exportfs -r 更新
$ sudo   showmount 192.168.0.4 -e
显示
Export list for 192.168.0.4:
/home/work *
六 启用
   $ sudo /etc/init.d/nfs-kernel-server restart 重启nfs服务
显示如下
* Stopping NFS kernel daemon                                                      [ OK ]
* Unexporting directories for NFS kernel daemon...                   [ OK ]
* Exporting directories for NFS kernel daemon...                            
   exportfs: /etc/exports [1]: Neither 'subtree_check' or 'no_subtree_check' specified for export "*:/home/work".
   Assuming default behaviour ('no_subtree_check').
   NOTE: this default has changed since nfs-utils version 1.0.x
* Starting NFS kernel daemon                                                          [ OK ]
挂载
mount -t nfs 192.168.0.4:/home/work   /nfsmnt
mount -t nfs 192.168.0.4:/home/work   /nfsmnt   -o nolock
