# nothing to be done for all - Orisun - 博客园







# [nothing to be done for all](https://www.cnblogs.com/zhangchaoyang/articles/2039600.html)





上次安装Xapian的时候我们说编译一个xapian的C++程序时总是说"undefined reference to xxx".其实我们仔细观察在./configure    xapian-core的时候有这么一句：nothing to be done for all，然后在make的时候也有：nothing to be done for all，nothing to be done for all-am。


这句提示是说明你已经编译好了，而且没有对代码进行任何改动。

make clean 清除安装时留下的文件

再运行一下ldconfig,

更新链接库缓存。 












