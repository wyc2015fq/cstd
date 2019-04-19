# 挂载ios，error tip：mount: wrong fs type, bad option, bad superblock on /dev/loop0, - =朝晖= - 博客园
# [挂载ios，error tip：mount: wrong fs type, bad option, bad superblock on /dev/loop0,](https://www.cnblogs.com/dhcn/p/9868832.html)
       挂载ios，tip：
```
mount -t iso9660 -o loop 111.iso /isofiles
```
      有可能是-t参数有问题，把-t参数去掉，然后挂载，就成功了

