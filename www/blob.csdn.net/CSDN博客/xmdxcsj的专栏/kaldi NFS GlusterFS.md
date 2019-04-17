# kaldi NFS/GlusterFS - xmdxcsj的专栏 - CSDN博客





2017年02月25日 17:19:23[xmucas](https://me.csdn.net/xmdxcsj)阅读数：681








## NFS

详细介绍参考《鸟哥的LINUX私房菜-服务器架设篇》第十三章 

主要流程

### server端配置
- 安装两个软件：rpcbind和nfs-utils
- 配置/etc/exports

```xml
<DIR> *(rw,no_root_squash)
```
- 启动nfs

```
/etc/init.d/rpcbind start
/etc/init.d/nfs start
/etc/init.d/nfslock start
chkconfig rpcbind on
chkconfig nfs on
chkconfig nfslock on
```
- 如果更新了/etc/exports文件，需要重新挂载
`exportfs -arv`
### client端配置
- 安装两个软件：rpcbind和nfs-utils
- 启动相关服务

```
/etc/init.d/rpcbind start
/etc/init.d/nfslock start
```
- 查看服务器提供了哪些共享目录

```bash
showmount -e hostname
```
- 建立挂载点并且挂载

```xml
mkdir -p <DIR>
mount -t nfs hostname:<DIR> <DIR>
umount <DIR> #卸载
```

## glusterfs

官方文档：[http://gluster.readthedocs.io/en/latest/](http://gluster.readthedocs.io/en/latest/)

Brick：对应一台服务器上面的存储空间 

Volume：多个brick组成一个volume，对应于多台服务器组成的存储空间 

Trusted Storage Pool：多台服务器组成的服务器集群
### CENTOS安装

[https://wiki.centos.org/SpecialInterestGroup/Storage/gluster-Quickstart](https://wiki.centos.org/SpecialInterestGroup/Storage/gluster-Quickstart)
[http://gluster.readthedocs.io/en/latest/Quick-Start-Guide/Quickstart/](http://gluster.readthedocs.io/en/latest/Quick-Start-Guide/Quickstart/)

安装到以下机器：hpc191、hpc192
```
#1.install on hpc191/hpc192
yum install centos-release-gluster
yum --enablerepo=centos-gluster*-test install glusterfs-server
service glusterd start
service glusterd status

#2.Configure the trusted pool
gluster peer probe hpc192 #on hpc191
gluster peer probe hpc191 #on hpc192

#3.Set up a GlusterFS volume
mkdir -p /euler/glusterfs/gv1 #on hpc191/hpc192
gluster volume create gv1 hpc191:<DIR>/gv1 hpc192:<DIR>/gv1 #on hpc191
gluster volume start gv1 #on hpc191
gluster volume info #on hpc191

#4.mount
mount -t glusterfs hpc191:/gv1 /glfs #on hpc191
mount -t glusterfs hpc192:/gv1 /glfs #on hpc192
```

### 扩展节点

新增加hpc193

```
#1.在client安装
yum install centos-release-gluster
yum --enablerepo=centos-gluster*-test install glusterfs-server
service glusterd start
service glusterd status

#2.Configure the trusted pool,在hpc191或者hpc192执行
gluster peer probe hpc193

#3.add brick to volume
gluster volume add-brick gv1 hpc193:<DIR>/gv1
gluster volume status

#4.mount 
mount -t glusterfs hpc193:/gv1 /glfs

#5.rebalance：add/delete brick 以后都需要rebalance
gluster volume rebalance gv1 fix-layout start #第一种方式：不迁移已有的数据
gluster volume rebalance gv1 start            #第二种方式：迁移已有的数据
```

### 卸载

```
#hpc191
gluster volume stop gv1 
gluster volume delete gv1
gluster peer detach hpc192
gluster peer detach hpc193

#hpc191/hpc192/hpc193
service glusferfsd stop
```

### 其他操作

#### [service启动](http://gluster.readthedocs.io/en/latest/Administrator%20Guide/Start%20Stop%20Daemon/)

```
/etc/init.d/glusterd start
/etc/init.d/glusterd stop
/etc/init.d/glusterd status
```

#### [pool操作](http://gluster.readthedocs.io/en/latest/Administrator%20Guide/Storage%20Pools/)

```
#add server to pool
gluster peer probe server
#status of pool
gluster peer status
#remove from pool
gluster peer detach server
```

#### volume操作

##### [创建设置](http://gluster.readthedocs.io/en/latest/Administrator%20Guide/Setting%20Up%20Volumes/)

类型包括： 

Distributed 

Striped 

Distributed Striped 

Distributed Replicated 

Distributed Striped Replicated 

Striped Replicated 

Dispersed 

Distributed Dispersed
##### [其他操作](http://gluster.readthedocs.io/en/latest/Administrator%20Guide/Managing%20Volumes/)

操作包括： 

Tuning Volume Options 

Configuring Transport Types for a Volume 

Expanding Volumes 

Shrinking Volumes 

Replacing Bricks 

Migrating Volumes 

Rebalancing Volumes 

Stopping Volumes 

Deleting Volumes 

Triggering Self-Heal on Replicate 

Non Uniform File Allocation(NUFA)
#### [log路径](http://gluster.readthedocs.io/en/latest/Administrator%20Guide/Logging/)







