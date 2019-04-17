# Linux文件打包成不同后缀的压缩文件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月09日 17:06:44[boonya](https://me.csdn.net/boonya)阅读数：592








Linux下经常会涉及对原有压缩文件的修改，我们需要知道相关的打包命令才能顺利的进行打包。下面是一些Linux打包成不同后缀问价的命令，值得参考。英文地址：[http://www.thelinuxdaily.com/2010/02/createcompressarchive-almost-any-file-in-linux-tar-tar-gz-tar-bz2-gz-bz-zip-7z-rar-etc/](http://www.thelinuxdaily.com/2010/02/createcompressarchive-almost-any-file-in-linux-tar-tar-gz-tar-bz2-gz-bz-zip-7z-rar-etc/)





## tar


tar cvf filename.tar /dir



## .tar.gz


tar czvf filename.tar.gz /dir



## .tgz


tar cvzf filename.tgz /dir



## .tar.gz2


You probably mean .tar.bz2


## .tar.bz


tar cjvf filename.tar.bz /dir



## .tbz


tar cjvf filename.tbz /dir



## .tar.bz2


tar cjvf filename.tar.bz2 /dir



## .tar.Z


.Z is an old naming convention which has moved to .gz.  See .tar.gz above.



## .tar.xz


tar cvf -  filenames | lzma > filename.tar.xz 



## .gz


gzip filename



## .gz2


You probably mean .bz2


## .bz


bzip filename



## .bz2


bzip2 filename



## .Z


.Z is an old naming convention which has moved to .gz.  See .gz above.



## .xz


lzma filename



## .zip


zip -r filename.zip /dir



## .7z


7z a -t7z filename.7z /dir



## .rar


rar a filename.rar /dir



## .dmg


This is a bit more complicated than what I have time for. Looks like somebody has already figured something out.


## .dd


Create these image files using:

dd if=inputfile.dd of=outputfile.dd






