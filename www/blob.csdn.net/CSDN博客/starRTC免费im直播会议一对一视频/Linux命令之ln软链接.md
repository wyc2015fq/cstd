# Linux命令之ln软链接 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年05月22日 18:44:13[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：563
用途:链接文件
默认情况下，ln命令产生硬链接。
最常用的参数是-s（建立符号连接Symbolic Link，也叫软连接）,具体用法是：
**ln**-s 源文件 目标文件
当我们需要在不同的目录用到相同的文件时，我们不需要在每一个目录下都放一个相同的文件，我们只需要在某个固定的目录放上该文件，然后在其它目录下用ln命令链接（link）它就可以。类似Windows的快捷方式。
## 删除软链接
ls后：
myphpadmin -> /opt/nginx/html/phpmyadmin
前面的是目标，所以rm -rf myphpadmin即可，注意后面千万不要加/
## 艺搜参考
[http://man.he.net/?topic=ln§ion=all](http://man.he.net/?topic=ln&section=all)
