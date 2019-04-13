
# unix dos mac 文件格式不同导致问题 - 嵌入式Linux - CSDN博客

2018年11月01日 10:15:33[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：32个人分类：[嵌入式学习																](https://blog.csdn.net/weiqifa0/article/category/1385985)



## 问题：
运行该脚本出现/bin/sh^M: bad interpreter: No such file or directory

## 解决：
错误中脚本文件的一个非常可能的原因是DOS格的, 即每一行的行尾以**\r\n**来标识, 其ASCII码各自是0x0D, 0x0A.
能够有非常多种办法看这个文件是DOS格式的还是UNIX格式的, 还是MAC格式的
在Terminal中输入例如以下:
> vi filename
然后用命令
> :set ff?
能够看到dos或unix的字样. 假设的确是dos格式的, 那么你能够用set ff=mac把它强制为mac格式的, 然后存盘退出. 再执行一遍看. 就会看到"^M"的真身。这时候把对于的”^M“删除就好了。

