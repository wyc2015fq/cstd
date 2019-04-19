# LevelDB使用总结 - fanyun的博客 - CSDN博客
2018年01月21日 21:20:49[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：281
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
      Leveldb在写的时候对内存要求不高，LevelDB具有很高的随机写，顺序读/写性能，但是随机读的性能很一般，读的时候根据性能要求的不同需要对应的内存，和SSD配合使用，效果和效率都非常不错，现在把一些使用经验分享给大家：
     首先：环境配置
     CPU：16核，Mem：32G  磁盘：SSD   差不多就可以充分发挥leveldb的优势。
