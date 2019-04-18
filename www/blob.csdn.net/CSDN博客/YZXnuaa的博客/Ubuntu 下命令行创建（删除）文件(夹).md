# Ubuntu 下命令行创建（删除）文件(夹) - YZXnuaa的博客 - CSDN博客
2018年03月16日 09:51:21[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：520
### 很多时候我们都会在终端进行文件/文件夹的创建与删除
使用快捷键ctrl + alt + t 打开终端：
- 创建文件： `touch a.txt`
- 创建文件夹： `mkdir``NewFolder`
- 删除文件： `rm``a.txt`
- 删除文件夹： `rmdir``NewFolder`
- 删除带有文件的文件夹： rm -r  NewFolder
> 
记笔记是为了以后自己能牢牢的掌握，也是给后人填坑， 
  用终端是为了适应依赖win的`GUI`界面
### 下面是在终端的练习：
```
snakeson@snakeson-Inspiron-5421:~$ mkdir NewFolder
snakeson@snakeson-Inspiron-5421:~$ ls
electronic-wechat  NewFolder  模板  图片  下载  桌面
examples.desktop   公共的     视频  文档  音乐
snakeson@snakeson-Inspiron-5421:~$ touch a.txt
snakeson@snakeson-Inspiron-5421:~$ ls
a.txt              examples.desktop  公共的  视频  文档  音乐
electronic-wechat  NewFolder         模板    图片  下载  桌面
snakeson@snakeson-Inspiron-5421:~$ rm a.txt
snakeson@snakeson-Inspiron-5421:~$ ls
electronic-wechat  NewFolder  模板  图片  下载  桌面
examples.desktop   公共的     视频  文档  音乐
snakeson@snakeson-Inspiron-5421:~$ rmdir NewFolder
snakeson@snakeson-Inspiron-5421:~$ ls
electronic-wechat  公共的  视频  文档  音乐
examples.desktop   模板    图片  下载  桌面
snakeson@snakeson-Inspiron-5421:~$
```
