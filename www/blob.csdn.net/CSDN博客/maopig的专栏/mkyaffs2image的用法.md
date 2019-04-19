# mkyaffs2image的用法 - maopig的专栏 - CSDN博客
2012年12月03日 20:38:00[maopig](https://me.csdn.net/maopig)阅读数：3332
在Ubuntu中第一次使用mkyaffs2image命令时，会提示
mkyaffs2image：找不到命令
还需要安装mkyaffs2image
http://code.google.com/p/fatplus/downloads/detail?name=yaffs2-source.tar&can=2&q=
下载yaffs2-source.tar 
解压后，进入utils文件夹，然后make
将make之后生成的mkyaffs2image文件拷贝到/usr/bin目录下
$ su
# cp mkyaffs2image /usr/bin/
# chmod 777 /usr/bin/mkyaffs2image
之后
$ mkyaffs2image 
mkyaffs2image: image building tool for YAFFS2 built Jan  6 2012
usage: mkyaffs2image dir image_file [convert]
           dir        the directory tree to be converted
           image_file the output file to hold the image
           'convert'  produce a big-endian image from a little-endian machine
表明安装成功，同时也可以看到该命令的用法
mkyaffs2image dir image_file
例如：
mkyaffs2image am1808-fs am1808-fs.yaffs//创建jffs2文件
