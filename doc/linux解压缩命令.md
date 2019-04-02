# linux解压缩命令





------

# 常用命令：

| **功能**                                 | **压缩**                           | **解压**               |
| ---------------------------------------- | ---------------------------------- | ---------------------- |
| 纯打包                                   | tar -cvf dst.tar dirPath/*.jpg     | tar -xvf dst.tar       |
| **gzip+打包**                            | tar -czf dst.tar.gz dirPath/*.jpg  | tar -xzvf dst.tar.gz   |
| **bzip2+打包**                           | tar –cjf dst.tar.bz2 dirPath/*.jpg | tar -xjvf file.tar.bz2 |
| **compress+打包**                        | tar –cZvf dst.tar.Z dirPath/*.jpg  | tar -xZvf file.tar.Z   |
| **rar压缩(**需要先下载rar for linux**)** | rar a dst.rar dirPath/*.jpg        | unrar e dst.rar        |
| **zip压缩(**需要先下载zip for linux**)** | zip  dst.zip dirPath/*.jpg         | unzip dst.zip          |
| **p7zip(**需要先下载p7zip for linux**)** | 详见参考资料1                      | 详见参考资料1          |
|                                          |                                    |                        |

## **tar 解压缩命令**

**这五个是独立的命令，可以和别的命令连用但只能用其中一个。**
-c: 建立压缩档案
-x：解压
-t：查看内容
-r：向压缩归档文件末尾追加文件
-u：更新原压缩包中的文件

**下面的参数是根据需要在压缩或解压档案时可选的。**

-z：有gzip属性的
-j：有bz2属性的
-Z：有compress属性的
-v：显示所有过程
-O：将文件解开到标准输出
-f: 使用档案名字，切记，这个参数是最后一个参数，后面只能接档案名。

**参考资料：**

1. [Linux下文件的打包、解压缩指令——tar，gzip，bzip2，unzip，rar](https://www.cnblogs.com/yhjoker/p/7568680.html)
2. 

------

有问题请联系：knowledgebao@163.com