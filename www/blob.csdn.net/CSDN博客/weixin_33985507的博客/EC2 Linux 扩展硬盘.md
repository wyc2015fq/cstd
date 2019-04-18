# EC2 Linux 扩展硬盘 - weixin_33985507的博客 - CSDN博客
2014年07月29日 12:44:03[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
前面豆子学习了怎么给EC2 Linux添加一个新的硬盘，那么如何扩展一个现有的硬盘呢？基本思路很简单，备份当前的硬盘，然后根据备份创建一个新硬盘，断开旧的硬盘，连接新的硬盘，扫描并扩展分区
首先创建一个2G的硬盘连入EC2的实例
![wKiom1PXJITwQq9tAAA5Of6cdnw241.png](http://s3.51cto.com/wyfs02/M00/42/34/wKiom1PXJITwQq9tAAA5Of6cdnw241.png)
然后基本操作如下，查询当前连接的Volume，可以用fdisk -l 或者 lsblk命令，都可以显示出当前连接的磁盘
![wKioL1PXJZWyGaUvAAAh2wvu8oc055.png](http://s3.51cto.com/wyfs02/M02/42/35/wKioL1PXJZWyGaUvAAAh2wvu8oc055.png)
然后对新磁盘分区格式化
![wKiom1PXJH_j-tQfAABKWtkVQ6U490.png](http://s3.51cto.com/wyfs02/M02/42/34/wKiom1PXJH_j-tQfAABKWtkVQ6U490.png)
创建一个/ebs的挂载点，加载这个硬盘，然后在上面创建了一个testfile的空文件作为测试
![wKioL1PXJZvSvIhWAAAumuN8Kt0977.png](http://s3.51cto.com/wyfs02/M01/42/35/wKioL1PXJZvSvIhWAAAumuN8Kt0977.png)
接下来，我们看看如何扩展这个2G的硬盘
首先应该是umount这个实例
![wKioL1PXJZzhuEw3AAAJlLk3v-I884.png](http://s3.51cto.com/wyfs02/M00/42/35/wKioL1PXJZzhuEw3AAAJlLk3v-I884.png)
如何对他做一个备份
![wKioL1PXJZ7AqWENAAA7NnB_FfU111.png](http://s3.51cto.com/wyfs02/M02/42/35/wKioL1PXJZ7AqWENAAA7NnB_FfU111.png)
取名叫做test
![wKiom1PXJIXABW-SAAAiwk2NmHc023.png](http://s3.51cto.com/wyfs02/M01/42/34/wKiom1PXJIXABW-SAAAiwk2NmHc023.png)
创建之后可以在snapshot的操作面板里面看见这个快照
![wKioL1PXJaOTyxNeAABvi6c-4IA527.png](http://s3.51cto.com/wyfs02/M00/42/35/wKioL1PXJaOTyxNeAABvi6c-4IA527.png)
通过这个test快照创建一个新的volume
![wKioL1PXJaPQoe3tAAAqrPoLhoI900.png](http://s3.51cto.com/wyfs02/M00/42/35/wKioL1PXJaPQoe3tAAAqrPoLhoI900.png)
创建的时候我把空间扩展到3G
![wKiom1PXJIqAil2VAAA47RQn2lg834.png](http://s3.51cto.com/wyfs02/M02/42/34/wKiom1PXJIqAil2VAAA47RQn2lg834.png)
然后detach 原先2G的硬盘， attach 新的3G硬盘

![wKiom1PXJIaRdkrhAABlKAM5XZE679.png](http://s3.51cto.com/wyfs02/M01/42/34/wKiom1PXJIaRdkrhAABlKAM5XZE679.png)
![wKioL1PXJaKD0pxnAAA8JfkYfis522.png](http://s3.51cto.com/wyfs02/M02/42/35/wKioL1PXJaKD0pxnAAA8JfkYfis522.png)
扫描当前加载的硬盘
![wKiom1PXJIuSla1OAAAgNHciTrM975.png](http://s3.51cto.com/wyfs02/M00/42/34/wKiom1PXJIuSla1OAAAgNHciTrM975.png)
因为这个硬盘是从快照而来，里面已经有文件系统和我创建的测试文档了，可以用file来查看一下文件系统
![wKioL1PXJaXjA_4BAAARSsWes2E188.png](http://s3.51cto.com/wyfs02/M01/42/35/wKioL1PXJaXjA_4BAAARSsWes2E188.png)
不需要格式化，直接mount即可
用df -h查询加载的设备，发现size还是2G
![wKiom1PXJIuQtYZ7AAAcMXCcYMY234.png](http://s3.51cto.com/wyfs02/M01/42/34/wKiom1PXJIuQtYZ7AAAcMXCcYMY234.png)
需要扩展分区表，扩展之后再次查询尺寸就对了
![wKiom1PXJI2Tn4-iAAAwFhSWLKk189.png](http://s3.51cto.com/wyfs02/M00/42/34/wKiom1PXJI2Tn4-iAAAwFhSWLKk189.png)
之前创建的文件也都还在！
![wKioL1PXJaezDNkXAAAKimjqEwU032.png](http://s3.51cto.com/wyfs02/M02/42/35/wKioL1PXJaezDNkXAAAKimjqEwU032.png)
扩建成功。
