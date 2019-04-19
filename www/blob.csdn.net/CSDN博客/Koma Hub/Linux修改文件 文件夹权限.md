# Linux修改文件/文件夹权限 - Koma Hub - CSDN博客
2017年10月31日 14:27:27[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：183
此权限时使用命令chattr的a属性来设置的，具体命令为：
[原文地址，如有侵权，联系删除](http://blog.csdn.net/u014630623/article/details/51721032)
`sudo chattr +a file(filename) `取消此权限命令： 
`sudo chattr -a file(filename)`
若递归的设置文件夹的此种权限，使用-R属性： 
`sudo chattr +a -R file(filename) `递归取消文件夹的此属性： 
`sudo chattr -a -R file(filename)`
