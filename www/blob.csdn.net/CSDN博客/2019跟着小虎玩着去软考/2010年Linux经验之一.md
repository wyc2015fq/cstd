# 2010年Linux经验之一 - 2019跟着小虎玩着去软考 - CSDN博客
2010年03月30日 18:33:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：1246
**2010****年****Linux****经验之一**
## 一.如何将命令输往屏幕的内容写到文件中：
>& test.txt
如将ls的命令输出到test.txt中，如：ls >&test.txt.
## 二.make all出现如下错误信息，如何解决：
make: Nothing to be done for 'all'
解决办法：先运行make clean,再运行make all命令
## 三.如何在当前目录查找文件：
find ./ -name libbluetooth.so
## 四.查看目标文件或者可执行的目标文件的构成:
objdump -x libmtv.so
