# sprintf用法 - sinat_31425585的博客 - CSDN博客
2016年08月24日 22:39:53[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：180
个人分类：[C++](https://blog.csdn.net/sinat_31425585/article/category/6402122)
记忆力差的孩子得勤做笔记！
sprintf最经典的用法就是将一定数据类型写入某个字符串，今天，我想要从一个文件夹中，按照文件名顺序的读出对应图片，其中涉及到将一个字符串和一个整形数字组合成为一个新的字符串。具体实现如下：
char * name = "c://nihao// "；
char  name1[128];
for(int i = 0; i  < 100; i++)
{
sprintf(name1, "%s  %d.jpg",  name,  i); // 这样就可以得到一个路径
IplImage* image = cvLoadImage(name1);
       }
