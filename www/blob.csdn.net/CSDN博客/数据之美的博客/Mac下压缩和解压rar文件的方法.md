# Mac下压缩和解压rar文件的方法 - 数据之美的博客 - CSDN博客
2018年10月21日 23:07:11[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：174
个人分类：[linux(ubuntu)](https://blog.csdn.net/oppo62258801/article/category/6738406)
命令行
使用工具rarosx，下载地址 
- 选择系统和版本，本文下载的是rarosx-5.4.0.tar.gz 
- 解压缩：tar zxvf rarosx-5.4.0.tar.gz 
其中 tar 是Mac 系统自带的命令。 
- 从终端进入到解压文件夹rar：cd Downloads/rar 
- 执行安装rar和unrar工具的命令 
rar：sudo install -c -o $USER rar /usr/local/bin/ 
unrar：sudo install -c -o $USER unrar /usr/local/bin 
- 使用 
解压：unrar x 课件.rar 或者 rar x 课件.rar 都行 
压缩文件A和B：rar a 压缩后.rar A B
--------------------- 
作者：johnny233 
来源：CSDN 
原文：https://blog.csdn.net/lonelymanontheway/article/details/79684116 
版权声明：本文为博主原创文章，转载请附上博文链接！
