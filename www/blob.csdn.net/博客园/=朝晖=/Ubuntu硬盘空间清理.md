# Ubuntu硬盘空间清理 - =朝晖= - 博客园
# [Ubuntu硬盘空间清理](https://www.cnblogs.com/dhcn/p/7125013.html)
1、删除多余的安装下载文件：
- sudo aptitude autoclean  
- sudo aptitude clean  
2、删除多余的内核版本：
- sudo apt-get autoremove  
4、去/usr/src目录，把里面旧的内核header包删除掉，指令也是用apt-get remove

