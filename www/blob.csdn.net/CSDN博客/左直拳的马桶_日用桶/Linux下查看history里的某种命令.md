# Linux下查看history里的某种命令 - 左直拳的马桶_日用桶 - CSDN博客
2014年11月21日 10:49:42[左直拳](https://me.csdn.net/leftfist)阅读数：7333
个人分类：[Linux](https://blog.csdn.net/leftfist/article/category/2605933)
Linux下，直接键入history命令，会将当前账户此前所有的命令都显示出来，未免太多了些。如果我只想查找某种命令，怎么办？
比如说，我只想查找我之前运行过的 “git” 命令
可以这样写：
$ history | grep "git"
这样出来的就全部是git命令
