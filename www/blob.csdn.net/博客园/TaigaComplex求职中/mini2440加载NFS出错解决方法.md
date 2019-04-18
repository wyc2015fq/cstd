# mini2440加载NFS出错解决方法 - TaigaComplex求职中 - 博客园







# [mini2440加载NFS出错解决方法](https://www.cnblogs.com/TaigaCon/archive/2012/11/01/2750327.html)





加载完内核后开始加载文件系统

结果出现以下错误

Root-NFS: Server returned error -13 while mounting

解决方法是添加可被外界挂载的目录，如：/nfs

先进虚拟机的Linux  /etc文件夹内

修改exports文件：

在结尾添加

/nfs *(rw,sync,no_root_squash)

然后重启虚拟机就OK












