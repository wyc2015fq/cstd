# linux压缩和解压缩命令大全 - =朝晖= - 博客园
# [linux压缩和解压缩命令大全](https://www.cnblogs.com/dhcn/p/7590698.html)
http://jingyan.baidu.com/article/6d704a13f9981a28da51ca70.html
tar命令
　　解包：tar zxvf FileName.tar
　　打包：tar czvf FileName.tar DirName
gz命令
　　解压1：gunzip FileName.gz
　　解压2：gzip -d FileName.gz
　　压缩：gzip FileName
　　.tar.gz 和 .tgz
　　解压：tar zxvf FileName.tar.gz
　　压缩：tar zcvf FileName.tar.gz DirName
压缩多个文件：tar zcvf FileName.tar.gz DirName1 DirName2 DirName3 ...
bz2命令
　　解压1：bzip2 -d FileName.bz2
　　解压2：bunzip2 FileName.bz2
　　压缩： bzip2 -z FileName
　　.tar.bz2
　　解压：tar jxvf FileName.tar.bz2
　　压缩：tar jcvf FileName.tar.bz2 DirName
bz命令
　　解压1：bzip2 -d FileName.bz
　　解压2：bunzip2 FileName.bz
　　压缩：未知
　　.tar.bz
　　解压：tar jxvf FileName.tar.bz
Z命令
　　解压：uncompress FileName.Z
　　压缩：compress FileName
　　.tar.Z
　　解压：tar Zxvf FileName.tar.Z
　　压缩：tar Zcvf FileName.tar.Z DirName
zip命令
　　解压：unzip FileName.zip
　　压缩：zip FileName.zip DirName

