# 这些 Linux 技巧大大提高你的工作效率 - Koma Hub - CSDN博客
2019年04月13日 16:49:16[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：56
个人分类：[Linux																[Shell](https://blog.csdn.net/Rong_Toa/article/category/7346222)](https://blog.csdn.net/Rong_Toa/article/category/7156254)
**Table of Contents**
[前言](#%E5%89%8D%E8%A8%80)
[命令编辑及光标移动](#%E5%91%BD%E4%BB%A4%E7%BC%96%E8%BE%91%E5%8F%8A%E5%85%89%E6%A0%87%E7%A7%BB%E5%8A%A8)
[历史命令快速执行](#%E5%8E%86%E5%8F%B2%E5%91%BD%E4%BB%A4%E5%BF%AB%E9%80%9F%E6%89%A7%E8%A1%8C)
[实时查看日志](#%E5%AE%9E%E6%97%B6%E6%9F%A5%E7%9C%8B%E6%97%A5%E5%BF%97)
[磁盘或内存情况查看](#%E7%A3%81%E7%9B%98%E6%88%96%E5%86%85%E5%AD%98%E6%83%85%E5%86%B5%E6%9F%A5%E7%9C%8B)
[根据名称查找进程id](#%E6%A0%B9%E6%8D%AE%E5%90%8D%E7%A7%B0%E6%9F%A5%E6%89%BE%E8%BF%9B%E7%A8%8Bid)
[根据名称杀死进程](#%E6%A0%B9%E6%8D%AE%E5%90%8D%E7%A7%B0%E6%9D%80%E6%AD%BB%E8%BF%9B%E7%A8%8B)
[查看进程运行时间](#%E6%9F%A5%E7%9C%8B%E8%BF%9B%E7%A8%8B%E8%BF%90%E8%A1%8C%E6%97%B6%E9%97%B4)
[快速目录切换](#%E5%BF%AB%E9%80%9F%E7%9B%AE%E5%BD%95%E5%88%87%E6%8D%A2)
[多条命令执行](#%E5%A4%9A%E6%9D%A1%E5%91%BD%E4%BB%A4%E6%89%A7%E8%A1%8C)
[查看压缩日志文件](#%E6%9F%A5%E7%9C%8B%E5%8E%8B%E7%BC%A9%E6%97%A5%E5%BF%97%E6%96%87%E4%BB%B6)
[删除乱码文件](#%E5%88%A0%E9%99%A4%E4%B9%B1%E7%A0%81%E6%96%87%E4%BB%B6)
[清空文件内容](#%E6%B8%85%E7%A9%BA%E6%96%87%E4%BB%B6%E5%86%85%E5%AE%B9)
[将日志同时记录文件并打印到控制台](#%E5%B0%86%E6%97%A5%E5%BF%97%E5%90%8C%E6%97%B6%E8%AE%B0%E5%BD%95%E6%96%87%E4%BB%B6%E5%B9%B6%E6%89%93%E5%8D%B0%E5%88%B0%E6%8E%A7%E5%88%B6%E5%8F%B0)
[终止并恢复进程执行](#%E7%BB%88%E6%AD%A2%E5%B9%B6%E6%81%A2%E5%A4%8D%E8%BF%9B%E7%A8%8B%E6%89%A7%E8%A1%8C)
[计算程序运行时间](#%E8%AE%A1%E7%AE%97%E7%A8%8B%E5%BA%8F%E8%BF%90%E8%A1%8C%E6%97%B6%E9%97%B4)
[查看内存占用前10的进程](#%E6%9F%A5%E7%9C%8B%E5%86%85%E5%AD%98%E5%8D%A0%E7%94%A8%E5%89%8D10%E7%9A%84%E8%BF%9B%E7%A8%8B)
[快速查找你需要的命令](#%E5%BF%AB%E9%80%9F%E6%9F%A5%E6%89%BE%E4%BD%A0%E9%9C%80%E8%A6%81%E7%9A%84%E5%91%BD%E4%BB%A4)
[命令行下的复制粘贴](#%E5%91%BD%E4%BB%A4%E8%A1%8C%E4%B8%8B%E7%9A%84%E5%A4%8D%E5%88%B6%E7%B2%98%E8%B4%B4)
[搜索包含某个字符串的文件](#%E6%90%9C%E7%B4%A2%E5%8C%85%E5%90%AB%E6%9F%90%E4%B8%AA%E5%AD%97%E7%AC%A6%E4%B8%B2%E7%9A%84%E6%96%87%E4%BB%B6)
[屏幕冻结](#%E5%B1%8F%E5%B9%95%E5%86%BB%E7%BB%93)
[无编辑器情况下编辑文本文件](#%E6%97%A0%E7%BC%96%E8%BE%91%E5%99%A8%E6%83%85%E5%86%B5%E4%B8%8B%E7%BC%96%E8%BE%91%E6%96%87%E6%9C%AC%E6%96%87%E4%BB%B6)
[查看elf文件](#%E6%9F%A5%E7%9C%8Belf%E6%96%87%E4%BB%B6)
[总结](#%E6%80%BB%E7%BB%93)
> 
作者：守望（本文来自作者投稿，简介见末尾）
www.yanbinghu.com/2019/03/24/44171.html
# **前言**
linux中的一些小技巧可以大大提高你的工作效率，本文就细数那些提高效率或者简单却有效的linux技巧。
## **命令编辑及光标移动**
这里有很多快捷键可以帮我们修正自己的命令。接下来使用光标二字代替光标的位置。
**删除从开头到光标处的命令文本**
ctrl + u，例如：
```
$ cd /proc/tty;ls -al光标
```
如果此时使用ctrl + u快捷键，那么该条命令都会被清除，而不需要长按backspace键。
**删除从光标到结尾处的命令文本**
ctrl+k，例如：
```
$ cd /proc/tty光标;ls -al
```
如果此时使用ctrl + k快捷键，那么从光标开始处到结尾的命令文本将会被删除。
还有其他的操作，不再举例，例如：
- 
ctrl + a:光标移动到命令开头
- 
ctrl + e：光标移动到命令结尾
- 
alt  f:光标向前移动一个单词
- 
alt  b：光标向前移动一个单词
- 
ctrl w：删除一个词（以空格隔开的字符串）
## **历史命令快速执行**
我们都知道history记录了执行的历史命令，而使用!＋历史命令前的数字，可快速执行历史命令。具体可参考阅读：《[Linux中“!”的惊叹用法](http://mp.weixin.qq.com/s?__biz=MzAxODI5ODMwOA==&mid=2666543634&idx=1&sn=5e39e7c6328e4852496608bbe34c106c&chksm=80dcfeb9b7ab77afd3096b325842b0ca6f8eaecd92235732ac30ca728f0c07d40adfebfecc16&scene=21#wechat_redirect)》。另外，还可以使用ctrl+r搜索执行过的命令。
**部分历史命令查看**
history会显示大量的历史命令，而fs -l只会显示部分。
## **实时查看日志**
```
$ tail -f filename.log
```
tail -f 加文件名，可以实时显示日志文件内容。当然，使用less命令查看文件内容，并且使用shift+f键，也可达到类似的效果。
## **磁盘或内存情况查看**
**怎么知道当前磁盘是否满了呢？**
```
$ df -h
/dev/sda14      4.6G   10M  4.4G   1% /tmp
/dev/sda11      454M  366M   61M  86% /boot
/dev/sda15       55G   18G   35G  35% /home
/dev/sda1       256M   31M  226M  12% /boot/efi
tmpfs           786M   64K  786M   1% /run/user/1000
```
使用df命令可以快速查看各挂载路径磁盘占用情况。
**当前目录各个子目录占用空间大小**
如果你已经知道home目录占用空间较大了，你想知道home目录下各个目录占用情况：
```
$ du -h –-max-depth=1 /home(或者-d 1)
18G    /home/hyb
16K    /home/lost+found
18G    /home/
```
这里指定了目录深度，否则的话，它会递归统计子目录占用空间大小，可自行尝试。
**当前内存使用情况**
```
$ free -h
              total        used        free      shared  buff/cache   available
Mem:           7.7G        3.5G        452M        345M        3.7G        3.5G
Swap:          7.6G          0B        7.6G
```
通过free的结果，很容易看到当前总共内存多少，剩余可用内存多少等等。
**使用-h参数**
不知道你是否注意到，我们在前面几个命令中，都使用了-h参数，它的作用是使得结果以人类可读的方式呈现，所以我们看到它呈现的单位是G,M等，如果不使用-h参数，可以自己尝试一下会是什么样的结果呈现。
## **根据名称查找进程id**
想快速直接查找进程id，可以使用：
```
$ pgrep hello
22692
```
或者：
```
$ pidof hello
22692
```
其中，hello是进程名称。
## **根据名称杀死进程**
一般我们可以使用kill　-9 pid方式杀死一个进程，但是这样就需要先找到这个进程的进程id，实际上我们也可以直接根据名称杀死进程，例如：
```
$ killall hello
```
或者：
```
$ pkill hello
```
## **查看进程运行时间**
可以使用下面的命令查看进程已运行时间：
```
$ ps -p 24525 -o lstart,etime 
                 STARTED     ELAPSED
Sat Mar 23 20:52:08 2019       02:45
```
其中24525是你要查看进程的进程id。
## **快速目录切换**
- 
cd -　回到上一个目录
- 
cd  回到用户家目录
## **多条命令执行**
我们知道使用分号隔开可以执行多条命令，例如：
```
$ cd /temp/log/;rm -rf *
```
但是如果当前目录是/目录，并且/temp/log目录不存在，那么就会发生激动人心的一幕：
```
bash: cd: /temp/log: No such file or directory
（突然陷入沉默）
```
因为;可以执行多条命令，但是不会因为前一条命令失败，而导致后面的不会执行，因此，cd执行失败后，仍然会继续执行rm -rf *，由于处于/目录下，结果可想而知。
所以你还以为这种事故是对rf -rf *的力量一无所知的情况下产生的吗？
如果解决呢？很简单，使用&&，例如:
```
$ cd /temp/log/&&rm -rf *
```
这样就会确保前一条命令执行成功，才会执行后面一条。
## **查看压缩日志文件**
有时候日志文件是压缩的，那么能不能偷懒一下，不解压查看呢？当然可以啦。
例如：
```
$ zcat test.gz
test log
```
或者：
```
$ zless test.gz
test log
```
## **删除乱码文件**
无论是自己意外创建还是程序异常创建，难免会出现一些命名奇怪或者乱码文件，如何删除呢？参考《[linux中删除特殊名称文件的多种方式](http://mp.weixin.qq.com/s?__biz=MzI2OTA3NTk3Ng==&mid=2649284371&idx=1&sn=0ff175e630c82cb9be2c29850d0a3e1a&chksm=f2f9ac74c58e2562f6c4b98bc0b785e89bff18ec8ee814f9578dfa9346473c8495f3cfe6edb6&scene=21#wechat_redirect)》。
## **清空文件内容**
比如有一个大文件，你想快速删除，或者不想删除，但是想清空内容：
```
>filename
```
## **将日志同时记录文件并打印到控制台**
在执行shell脚本，常常会将日志重定向，但是这样的话，控制台就没有打印了，如何使得既能记录日志文件，又能将日志输出到控制台呢？
```
$ ./test.sh |tee test.log
```
## **终止并恢复进程执行**
我们使用ctrl+z 暂停一个进程的执行，也可以使用fg恢复执行。例如我们使用
```
$ cat filename
```
当我们发现文件内容可能很多时，使用ctrl+z暂停程序，而如果又想要从刚才的地方继续执行，则只需要使用fg命令即可恢复执行。或者使用bg使得进程继续在后台执行。
## **计算程序运行时间**
我们可能会进程写一些小程序，并且想要知道它的运行时间，实际上我们可以很好的利用time命令帮我们计算，例如：
```
$ time ./fibo 30
the 30 result is 832040
real    0m0.088s
user    0m0.084s
sys    0m0.004s
```
它会显示系统时间，用户时间以及实际使用的总时间。
## **查看内存占用前10的进程**
```
$ ps -aux|sort -k4nr |head -n 10
```
这里综合使用了ps，sort，head命令，可分别参考《[ps命令详解](http://mp.weixin.qq.com/s?__biz=MzI2OTA3NTk3Ng==&mid=2649283839&idx=1&sn=62bb18d464ba24c660f69dc1800e880b&chksm=f2f9af98c58e268e12cae70085511bb58ea0955d74e7f6bac28e3bb3adc13ca0e8fc2e5cc66e&scene=21#wechat_redirect)》，《[一个命令帮你对文本排序](http://mp.weixin.qq.com/s?__biz=MzI2OTA3NTk3Ng==&mid=2649284038&idx=1&sn=9230f76a4cbe2503eae4157cae5058c2&chksm=f2f9aea1c58e27b72771459162c5cff450a6eed5e691acdd5c9e8c4ba0eb7fec34e2136fe37c&scene=21#wechat_redirect)》和《[linux常用命令－文本查看篇](http://mp.weixin.qq.com/s?__biz=MzI2OTA3NTk3Ng==&mid=2649283920&idx=1&sn=ae8879b8982b8e81fccceeb48aeba317&chksm=f2f9ae37c58e2721899c81f1c92c35b05cc9e8396922aea492c8fb02a35fdbc3617907a7ca56&scene=21#wechat_redirect)》。
## **快速查找你需要的命令**
我们都知道man可以查看命令的帮助手册，但是如果我们想要某个功能却不知道使用哪个命令呢？别着急，还是可以使用man：
```
$ man -k "copy files"
cp (1)               - copy files and directories
cpio (1)             - copy files to and from archives
git-checkout-index (1) - Copy files from the index to the working tree
gvfs-copy (1)        - Copy files
gvfs-move (1)        - Copy files
install (1)          - copy files and set attributes
```
使用-k参数，使得与copy files相关的帮助手册都显示出来了。
## **命令行下的复制粘贴**
我们知道，在命令行下，复制不能再是ctrl + c了，因为它表示终止当前进程，而控制台下的复制粘贴需要使用下面的快捷键：
- 
ctrl +  insert
- 
shift + insert
## **搜索包含某个字符串的文件**
例如，要在当前目录下查找包含test字符串的文件：
```
$ grep -rn "test"
test2.txt:1:test
```
它便可以找到该字符串在哪个文件的第几行。
## **屏幕冻结**
程序运行时，终端可能输出大量的日志，你想简单查看一下，又不想记录日志文件，此时可以使用ctrl+s键，冻结屏幕，使得日志不再继续输出，而如果想要恢复，可使用ctrl+q退出冻结。
## **无编辑器情况下编辑文本文件**
如果在某些系统上连基本的vi编辑器都没有，那么可以使用下面的方式进行编辑内容：
```
$ cat >file.txt
some words
(ctrl+d)
```
编辑完成后，ctrl+d即可保存。
## **查看elf文件**
**查看elf文件头信息**
例如：
```
$ readelf -h filename
```
我们在显示结果中，可以看到运行的平台，elf文件类型，大小端情况等。
**查看库中是否包含某个接口**
```
$ nm filename |grep interface
```
这里是从文件filename中查看是否包含interface接口，前提是该文件包含符号表。
更多相关内容，可参考《[linux常用命令-开发调试篇](http://mp.weixin.qq.com/s?__biz=MzAxODI5ODMwOA==&mid=2666543180&idx=2&sn=63c595b13cc76b8655c7023dbc0d9a56&chksm=80dcfce7b7ab75f1030d6903e563087d85af3128a5973eadccb8ba547f4307acaedb31d4e301&scene=21#wechat_redirect)》
## **总结**
本文所提到的内容建议自己上机操作，体验效果。本文总结了一些常用的linux小技巧，你还有哪些linux小技巧？欢迎留言分享。
【本文作者】
守望：一名好文学，好技术的开发者。在个人公众号【编程珠玑】分享Linux/C/C++/算法/工具等原创技术文章和学习资源
