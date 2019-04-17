# linux常用命令 - 李鑫o_O - CSDN博客





置顶2017年08月04日 22:22:36[hustlx](https://me.csdn.net/HUSTLX)阅读数：395








                
查看目录下有多少个文件及文件夹需在终端输入

ls | wc -w

统计文件夹下面包括子文件夹下的所有文件个数

ls -lR|grep "^-"|wc -l


查看目录下有多少个文件需在终端输入

ls | wc -c

查看文件夹下有多少个文件，多少个子目录需在终端输入

ls -l |wc -l

指定文件夹的数目

wc -l dir

两个文本合并：

cat file1 file2>dstfile

若只想知道文件的个数，则需在终端输入

/bin/ls -l |grep ^-|wc -l

查看当前目录各文件大小

du -sh *

显示进程

ps -ef|grep java|grep -v grep


压缩

tar -czvf ***.tar.gz

tar -cjvf ***.tar.bz2

#解压缩

tar -xzvf ***.tar.gz

tar -xjvf ***.tar.bz2


保存log

sh train.sh > log_train 2>&1 


退出ssh：

exit

激活python 3

activate python3



export PATH=/opt/compiler/gcc-4.8.2/bin/:$PATH

按名字杀进程

ps aux|grep python|grep -v grep|awk '{print $2}'|xargs kill -9


pytorch FloatTensor 维度：

a.size()






