# C++实现成绩分类的算法 - fanyun的博客 - CSDN博客
2018年11月28日 21:26:35[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：197标签：[成绩分类](https://so.csdn.net/so/search/s.do?q=成绩分类&t=blog)
个人分类：[C++																[算法](https://blog.csdn.net/fanyun_01/article/category/7113106)](https://blog.csdn.net/fanyun_01/article/category/6102444)
1.实现一个简单算法，对学习成绩进行分类
学习成绩>=90分的同学用A表示，80-90分之间的用B表示，70-80分之间的用C表示，60分以下的用C表示。
2.基本思路：使用条件运算符的嵌套
3.代码实现
实现1：
```cpp
#include "stdio.h"
int main(void)
{
    int score = 0;
    printf("请输入你的成绩：");
    scanf_s("%d", &score);
    if (0 <= score && score <= 100)
    {
        if (90 <= score && score <= 100)
            printf("A\n");
        else if (80 <= score && sore < 90)
            printf("B\n");
        else if (70 <= score && score < 80)
            printf("C\n");
        else if (60 <= score && score < 70)
            printf("D\n");
        else
            printf("E\n");
    }
    else
        printf("分数不得超出[0, 100]区间\n");
    return 0;
}
```
实现2：
```cpp
switch语句实现：
 #include<stdio.h>
int main()
{
int score;
printf("输入学生的成绩:\n");
scanf("%d",&score);
if (score<0 || score>100)
{
printf("输入的数据非法!\n");
}
else
{
switch(score/10)
{
case 0:
case 1:
case 2:
case 3:
case 4:
case 5:printf("该同学的等级为:E\n");break;
case 6:printf("该同学的等级为:D\n");break;
case 7:printf("该同学的等级为:C\n");break;
case 8:printf("该同学的等级为:B\n");break;
case 9:
case 10:printf("该同学的等级为:A\n");break;
}
}
return  0;
}
```
4.代码输出
![](https://img-blog.csdnimg.cn/20181127170656341.png)
