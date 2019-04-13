
# linux下的缓存机制 - lpty的博客 - CSDN博客

2018年10月11日 16:56:27[lpty](https://me.csdn.net/sinat_33741547)阅读数：88



## 一、缓存
在linux系统下，free命令可以看到系统内存使用情况：
![在这里插入图片描述](https://img-blog.csdn.net/20181011164532492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
上图各个参数解释如下：
[
](https://img-blog.csdn.net/20181011164532492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### 1、Mem
[
](https://img-blog.csdn.net/20181011164532492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3NpbmF0XzMzNzQxNTQ3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)total：总内存
used：已使用的内存
free：空闲的内存
shared：当前已废弃内存
buffers Buffer：I/O缓存，用于内存和硬盘的缓冲
cached Page：高速缓存，用于CPU和内存之间的缓冲
关系：total = used + free
### 2、Swap
(-buffers/cache) used内存：第一部分Mem中的 used – buffers – cached
(+buffers/cache) free内存: 第一部分Mem中的 free + buffers + cached
-buffers/cache反映的是被程序实实在在吃掉的内存
+buffers/cache反映的是可以挪用的内存总数
## 二、手动清理
Linux 内存管理对dentry进行缓存（用于VFS，加速文件路径名到inode的转换），还有两种主要Cache方式：Buffer Cache和Page Cache，目的就是为了提升磁盘IO的性能。从低速的块设备上读取数据会暂时保存在内存中，即使数据在当时已经不再需要了，但在应用程序下一次访问该数据时，它可以从内存中直接读取，从而绕开低速的块设备，从而提高系统的整体性能。
`#查看当前设置
cat /proc/sys/vm/drop_caches 
1、需要root权限
#释放pagecache
echo 1 > /proc/sys/vm/drop_caches  
#释放inode和dentry等metadata  
echo 2 > /proc/sys/vm/drop_caches  
#释放pagecache和inode/dentry
echo 3 > /proc/sys/vm/drop_caches
2、需要sudo权限
# 同步数据到磁盘
sync
sudo sysctl -w vm.drop_caches=3
sudo sysctl -w vm.drop_caches=0 #recovery drop_caches
# 查看是否修改成功
sudo sysctl -a | grep drop_caches
3、永久生效，修改/etc/sysctl.conf 文件
# 相当于关闭cache
vm.dirty_ratio = 1
vm.dirty_background_ratio=1
vm.dirty_writeback_centisecs=2
vm.dirty_expire_centisecs=3
vm.drop_caches=3
vm.swappiness =100
vm.vfs_cache_pressure=163
vm.overcommit_memory=2
vm.lowmem_reserve_ratio=32 32 8
kern.maxvnodes=3`

