# Linux下压缩与解压命令tar - starRTC免费im直播会议一对一视频 - CSDN博客
2016年08月12日 17:11:59[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：323
个人分类：[未分类](https://blog.csdn.net/elesos/article/category/6361263)
## Linux下常见压缩文件的扩展名
*****.gz：**gzip**压缩的；*****.bz2：**bzip2**压缩的；*****.tar：**tar**程序打包但没有压缩的；*****.tar.gz：打包后并经过**gzip**压缩的；*****.tar.bz2：打包后并经过**bzip2**压缩的；
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux%E4%B8%8B%E5%8E%8B%E7%BC%A9%E4%B8%8E%E8%A7%A3%E5%8E%8B%E5%91%BD%E4%BB%A4tar&action=edit&section=2)]语法
**tar****[**-j或-z**]****[**cv**]****[**-f 生成的压缩文件名**]**  要被压缩的文件或目录   **//**打包与压缩**tar****[**-j或-z**]****[**xv**]****[**-f 生成的压缩文件名**]****[**-C  解压目录**]****//**解压**tar****[**-j或-z**]****[**tv**]****[**-f 生成的压缩文件名**]****//**查看
参数解释：
c建立打包档案，x解压, t查看含有哪些文件，（cxt不能同时使用）v显示处理信息
j用于tar.bz2
z用于.tar.gz，tgz
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux%E4%B8%8B%E5%8E%8B%E7%BC%A9%E4%B8%8E%E8%A7%A3%E5%8E%8B%E5%91%BD%E4%BB%A4tar&action=edit&section=3)]排除某些文件夹
tar zcvf tomcat.tar.gz --exclude=tomcat/logs --exclude=tomcat/webapp/xxxx//WEB-INF/logs tomcat
将tomcat目录中，除了tomcat/logs 和 tomcat/webapp/xxxx//WEB-INF/logs 之外的文件压缩成压缩包：tomcat.tar.gz
注意，去除的文件夹后面不能带/,否则该文件夹依然会被压缩！
又如：
如果123目录下有3个子目录，aa、bb、cc。
我现在想只对aa和bb目录打包压缩，命令如下：
tar -zcvf 123.tar.gz --exclude=cc 123
（在123目录的外面运行）
使用exclude参数来过滤不需要的目录或文件，排除某个文件的操作和目录一样。
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux%E4%B8%8B%E5%8E%8B%E7%BC%A9%E4%B8%8E%E8%A7%A3%E5%8E%8B%E5%91%BD%E4%BB%A4tar&action=edit&section=4)]示例
以test目录为例：
压缩:
首先进入上一层目录
**tar**-jcv-f test.tar.bz2 **test**
查看压缩包里有什么文件:（t重点在查看含有哪些文件，v显示详细信息）
**tar**-jtv-f test.tar.bz2**tar**-jt-f test.tar.bz2
解压：
**tar**-jx-f test.tar.bz2 -C**/**tmp   **//**熟悉后，可以直接**tar**-jxf test.tar.bz2 -C**/**tmp
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux%E4%B8%8B%E5%8E%8B%E7%BC%A9%E4%B8%8E%E8%A7%A3%E5%8E%8B%E5%91%BD%E4%BB%A4tar&action=edit&section=5)]解压xz
解压tar.xz文件：
**tar**-xvf 【文件名】
如果不能解压，请先执行
yum -y install xz xz-devel 
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux%E4%B8%8B%E5%8E%8B%E7%BC%A9%E4%B8%8E%E8%A7%A3%E5%8E%8B%E5%91%BD%E4%BB%A4tar&action=edit&section=6)]解压rar
rarlinux包备份下载链接：[http://pan.baidu.com/s/1bnFGCbH](http://pan.baidu.com/s/1bnFGCbH)密码：mtx3
**wget** http:**//**www.rarlab.com**/**rar**/**rarlinux-3.8.0.tar.gz **tar** zxvf rarlinux-3.8.0.tar.gz   **cd** rar **make****install**
解压rar文件，可以使用unrar x **.rar
如果报错：unrar: error while loading shared libraries: libstdc++.so.6: cannot open shared object file: No such file or directory
解决：
**yum**-y**install** libstdc++.so.6
zip文件
yum install -y unzip zip
## [[编辑](http://192.168.1.100/wiki_elesos_com/index.php?title=Linux%E4%B8%8B%E5%8E%8B%E7%BC%A9%E4%B8%8E%E8%A7%A3%E5%8E%8B%E5%91%BD%E4%BB%A4tar&action=edit&section=7)]艺搜参考
[http://www.ha97.com/4024.html](http://www.ha97.com/4024.html)
[http://dl528888.blog.51cto.com/2382721/739871](http://dl528888.blog.51cto.com/2382721/739871)
[http://blog.is36.com/rar_for_linux/](http://blog.is36.com/rar_for_linux/)
