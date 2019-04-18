# linux压缩文件解压 - happyhorizon的算法天空 - CSDN博客
2019年03月04日 19:55:32[lxy_Alex](https://me.csdn.net/happyhorizion)阅读数：28
|文件格式|解压方法|
|----|----|
|.zip|unzip FileName.zip|
|.xz|xz -d FileName.tar.xz  或者 tar xvJf  FileName.tar.xz|
|.bz|bzip2 -d [FileName.bz](http://FileName.bz) 或者 bunzip2 [FileName.bz](http://FileName.bz)|
|.bz2|bzip2 -d FileName.bz2  或者 bunzip2 FileName.bz2  或者 tar jxvf FileName.tar.bz2  或者 tar --bzip xvf FileName.tar.bz2|
总结
1、*.tar 用 tar –xvf 解压2、*.gz 用 gzip -d或者gunzip 解压
3、*.tar.gz和*.tgz 用 tar –xzf 解压
4、*.bz2 用 bzip2 -d或者用bunzip2 解压5、*.tar.bz2用tar –xjf 解压
6、*.Z 用 uncompress 解压7、*.tar.Z 用tar –xZf 解压
8、*.rar 用 unrar e解压9、*.zip 用 unzip 解压
参考:
[https://blog.csdn.net/nichng/article/details/78231358](https://blog.csdn.net/nichng/article/details/78231358)
[https://blog.csdn.net/u013439115/article/details/77935602](https://blog.csdn.net/u013439115/article/details/77935602)
