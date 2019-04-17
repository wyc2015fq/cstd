# Mac下使用python实现简单的目录树展示 - 小灰笔记 - CSDN博客





2017年04月27日 01:16:03[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：716








在Linux或者Windows下想要查看目录树都可以通过tree命令来实现，两个操作系统中的操作也很相似。使用Linux时，最初以为这是shell中都有这个命令可用。结果使用Mac的时候发现并没有这个命令。

为了实现类似的功能，使用python做了一个简单的小脚本。实现不了漂亮的目录树，但是可以实现文件以及所在目录的列表打印。

编写代码如下：

  1 #!/usr/bin/python  

  2 

  3 import os

  4 

  5 pwd = os.getcwd()

  6 

  7 for root,dirs,files
in os.walk(pwd):

  8     os.chdir(root)

  9 for f
in files:

 10 ifnot(f ==
'file_list.py'and root == pwd):

 11 print("%s<->%s" %(f,root))

 12     os.chdir(pwd)

 13 




程序执行结果如下：

python file_list.py 

.DS_Store<->/Users/greyzhang/Downloads/vim-autocomplpop

acp.vim<->/Users/greyzhang/Downloads/vim-autocomplpop/autoload

acp.jax<->/Users/greyzhang/Downloads/vim-autocomplpop/doc

acp.txt<->/Users/greyzhang/Downloads/vim-autocomplpop/doc

acp.vim<->/Users/greyzhang/Downloads/vim-autocomplpop/plugin




虽说结构以及格式很不漂亮，但是好歹还是能够使用了。



