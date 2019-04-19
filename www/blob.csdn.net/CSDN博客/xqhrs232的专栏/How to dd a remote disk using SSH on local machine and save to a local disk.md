# How to dd a remote disk using SSH on local machine and save to a local disk - xqhrs232的专栏 - CSDN博客
2016年08月19日 09:51:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：281
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://unix.stackexchange.com/questions/132797/how-to-dd-a-remote-disk-using-ssh-on-local-machine-and-save-to-a-local-disk](http://unix.stackexchange.com/questions/132797/how-to-dd-a-remote-disk-using-ssh-on-local-machine-and-save-to-a-local-disk)
相关文章
1、[tar+ssh实现异地备份](http://blog.chinaunix.net/uid-8116903-id-3914169.html) ----[http://blog.chinaunix.net/uid-8116903-id-3914169.htm](http://blog.chinaunix.net/uid-8116903-id-3914169.html)


//=======================================================================================
备注：：
1〉dd if=/dev/sda | gzip -1 - | ssh root@192.168.1.19 dd of=/home/image.gz------适用于2台linux环境下！！！
