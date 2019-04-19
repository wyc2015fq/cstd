# C语言变长数组data[0]【总结】 - maopig的专栏 - CSDN博客
2014年06月19日 10:00:56[maopig](https://me.csdn.net/maopig)阅读数：1002
个人分类：[c/C++](https://blog.csdn.net/maopig/article/category/847675)
# [C语言变长数组data[0]【总结】](http://www.cnblogs.com/Anker/p/3744127.html)
1、前言
　　今天在看代码中遇到一个结构中包含char data[0]，第一次见到时感觉很奇怪，数组的长度怎么可以为零呢？于是上网搜索一下这样的用法的目的，发现在linux内核中，结构体中经常用到data[0]。这样设计的目的是让数组长度是可变的，根据需要进行分配。方便操作，节省空间。
2、data[0]结构
经常遇到的结构形状如下：
```
struct buffer
{
    int data_len;   //长度
    char data[0];  //起始地址
};
```
  在这个结构中，data是一个数组名；但该数组没有元素；该数组的真实地址紧随结构体buffer之后，而这个地址就是结构体后面数据的地址（如果给这个结构体分配的内容大于这个结构体实际大小，后面多余的部分就是这个data的内容）；这种声明方法可以巧妙的实现C语言里的数组扩展。
　　写个程序对比char data[0]，char *data, char data[]，如下所示：
```
[](http://www.cnblogs.com/Anker/p/3744127.html)
 1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <string.h>
 4 #include <stdint.h>
 5 
 6 typedef struct
 7 {
 8     int data_len;
 9     char data[0];
10 }buff_st_1;
11 
12 typedef struct
13 {
14     int data_len;
15     char *data;
16 }buff_st_2;
17 
18 typedef struct 
19 {
20     int data_len;
21     char data[];
22 }buff_st_3;
23 
24 int main()
25 {
26     printf("sizeof(buff_st_1)=%u\n", sizeof(buff_st_1));
27     printf("sizeof(buff_st_2)=%u\n", sizeof(buff_st_2));
28     printf("sizeof(buff_st_3)=%u\n", sizeof(buff_st_3));
29 
30     buff_st_1 buff1;
31     buff_st_2 buff2;
32     buff_st_3 buff3;
33 
34     printf("buff1 address:%p,buff1.data_len address:%p,buff1.data address:%p\n",
35         &buff1, &(buff1.data_len), buff1.data);
36 
37     printf("buff2 address:%p,buff2.data_len address:%p,buff2.data address:%p\n",
38         &buff2, &(buff2.data_len), buff2.data);
39 
40     printf("buff3 address:%p,buff3.data_len address:%p,buff3.data address:%p\n",
41         &buff3, &(buff3.data_len), buff3.data);
42 
43     return 0;
44 }
[](http://www.cnblogs.com/Anker/p/3744127.html)
```
![](http://images.cnitblog.com/i/305504/201405/212314244022730.jpg)
从结果可以看出data[0]和data[]不占用空间，且地址紧跟在结构后面，而char *data作为指针，占用4个字节，地址不在结构之后。
3、实际当中的用法
     在实际程序中，数据的长度很多是未知的，这样通过变长的数组可以方便的节省空间。对指针操作，方便数据类型的转换。测试程序如下：
```
[](http://www.cnblogs.com/Anker/p/3744127.html)
 1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <string.h>
 4 #include <stdint.h>
 5 
 6 typedef struct
 7 {
 8     int data_len;
 9     char data[0];
10 }buff_st_1;
11 
12 typedef struct
13 {
14     int data_len;
15     char *data;
16 }buff_st_2;
17 
18 typedef struct 
19 {
20     int data_len;
21     char data[];
22 }buff_st_3;
23 
24 typedef struct 
25 {
26     uint32_t id;
27     uint32_t age;
28 }student_st;
29 
30 
31 void print_stu(const student_st *stu)
32 {
33     printf("id:%u,age:%u\n", stu->id, stu->age);
34 }
35 
36 int main()
37 {
38     student_st *stu = (student_st *)malloc(sizeof(student_st));
39     stu->id = 100;
40     stu->age = 23;
41 
42     student_st *tmp = NULL;
43 
44     buff_st_1 *buff1 = (buff_st_1 *)malloc(sizeof(buff_st_1) + sizeof(student_st));
45     buff1->data_len = sizeof(student_st);
46     memcpy(buff1->data, stu, buff1->data_len);
47     printf("buff1 address:%p,buff1->data_len address:%p,buff1->data address:%p\n",
48         buff1, &(buff1->data_len), buff1->data);
49 
50     tmp = (student_st*)buff1->data;
51     print_stu(tmp);
52 
53     buff_st_2 *buff2 = (buff_st_2 *)malloc(sizeof(buff_st_2));
54     buff2->data_len = sizeof(student_st);
55     buff2->data = (char *)malloc(buff2->data_len);
56     memcpy(buff2->data, stu, buff2->data_len);
57     printf("buff2 address:%p,buff2->data_len address:%p,buff2->data address:%p\n",
58         buff2, &(buff2->data_len), buff2->data);
59 
60     tmp = (student_st *)buff2->data;
61     print_stu(tmp);
62 
63     buff_st_3 *buff3 = (buff_st_3 *)malloc(sizeof(buff_st_3) + sizeof(student_st));
64     buff3->data_len = sizeof(student_st);
65     memcpy(buff3->data, stu, buff3->data_len);
66     printf("buff3 address:%p,buff3->data_len address:%p,buff3->data address:%p\n",
67         buff3, &(buff3->data_len), buff3->data);
68 
69     tmp = (student_st*)buff1->data;
70     print_stu(tmp);
71 
72     free(buff1);
73 
74     free(buff2->data);
75     free(buff2);
76 
77     free(buff3);
78     free(stu);
79     return 0;
80 }
[](http://www.cnblogs.com/Anker/p/3744127.html)
```
程序执行结果如下：![](http://images.cnitblog.com/i/305504/201405/212322263096473.png)
  采用char *data，需要进行二次分配，操作比较麻烦，很容易造成内存泄漏。而直接采用变长的数组，只需要分配一次，然后进行取值即可以。
