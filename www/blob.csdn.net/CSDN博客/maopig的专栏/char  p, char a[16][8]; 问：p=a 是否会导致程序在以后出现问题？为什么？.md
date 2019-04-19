# char **p, char a[16][8]; 问：p=a 是否会导致程序在以后出现问题？为什么？ - maopig的专栏 - CSDN博客
2011年08月24日 20:20:55[maopig](https://me.csdn.net/maopig)阅读数：2411
int (*v)[10];
在 Visual C++ 里面不能与 int **v 等同起来。
举个例子：
int **p;
int (*v)[10];
int a[10][10];
如果写 v = a 是可以的。
如果写 p = a 将会引起一个类型不匹配的编译错误。
二级指针（int **p）需要自己指向一个一级指针，而二维数组则
不是这样。编译器为了节省空间和时间，没有采用指针指向指针的方法，而是将数组现
行地存放在内存中。这样一来，int (*v)[10] 也就成了 C 语言的一种特殊语法，它只
能表示 v 指向第一维具有 10 个元素的二维数组的头部。这样 v 的类型与 a[10][10]
正好匹配，v 的类型与 b[5][10]（比如说定义了 b）也是匹配的。但是 v 不能匹配
c[5][5] 因为 c 的内存分布中每隔 5 个元素就把第二维的下标加一。
可以这样写，呵呵……
- #include<stdio.h>
- #include<stdlib.h>
- int main()
- 
- { 
-         //local declarations
- int **pointer;
- int count;
- int i;
- int *p;
- 
- //statements
-   pointer= (int **)malloc (sizeof(int *) *10);
-   for( count=0; count < 10; count++ )
-       pointer[count]=(int *)malloc(sizeof(int) * 10);
- 
-   for(count=0; count< 10; count++)
-         {
-            p=*(pointer+count);
-        for(i=0; i<10; i++)
-                   *(p+i)=count;
-         }//for
- 
-   for(count=0; count<10; count++)
-         {    
-                         for(i=0;i<10;i++)
-                                          printf("%6d",*(*(pointer+count)+i));
-                          printf("\n");  
-         }//for
- 
- return  0;
- }//main
*复制代码*
这样 pointer 也可以作为二维数组来使用，但是每两个下标相邻的元素在内存中的地址不一定
是相邻的，而且还有 10 个一维指针占用内存空间作为桥梁。
 char **str1=NULL;
 char *x="zhang";
 str1=&x;
