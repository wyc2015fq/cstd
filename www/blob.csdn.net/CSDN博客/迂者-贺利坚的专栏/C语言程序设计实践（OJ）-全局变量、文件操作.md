
# C语言程序设计实践（OJ）-全局变量、文件操作 - 迂者-贺利坚的专栏 - CSDN博客

2016年11月22日 22:06:58[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1005个人分类：[C语言教学																](https://blog.csdn.net/sxhelijian/article/category/6420119)



## 2969：学生成绩的处理
**Description**
编写一个函数void calcscore(int n)，在函数中输入n个人的成绩，计算最高分，最低分，总分和平均分，要求在主函数中调用函数calcscore计算各种成绩，并在主函数中输出各种计算结果。(使用全局变量在函数之间传递多个数据)
```python
#include <stdio.h>
```
```python
double
```
```python
HighScore;
```
```python
/*全局变量，最高分*/
```
```python
double
```
```python
LowScore;
```
```python
/*全局变量，最低分*/
```
```python
double
```
```python
SumScore;
```
```python
/*全局变量，总分*/
```
```python
double
```
```python
AverageScore;
```
```python
/*全局变量，平均分*/
```
```python
void
```
```python
calcscore(
```
```python
int
```
```python
n);
```
```python
/*函数声明*/
```
```python
int
```
```python
main()
{
```
```python
int
```
```python
n;
```
```python
scanf
```
```python
(
```
```python
"%d"
```
```python
,&n);
    calcscore(n);
```
```python
printf
```
```python
(
```
```python
"%g %g %g %g\n"
```
```python
,HighScore,LowScore,SumScore,AverageScore);
```
```python
return
```
```python
0
```
```python
;
}
```
主程序已给出，请完成calcscore函数并提交
**Input**
学生人数n和n个学生的成绩。
**Output**
n个人的最高分，最低分，总分和平均分
**Sample Input**
5
80 90 100 70 50
**Sample Output**
100 50 390 78
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53292606)
## 2972: 从文件中读数据
**Description**
文本文件score.dic 中存储了n名学生的信息（班级编号,姓名,成绩），每个学生信息占一行，每行的数据之间使用制表符分割，如下所示：
```python
145811  fuxin 100
145811  chengxian 90
145812  zhangxue 92
145812  lijun 88
……
```
文件中存储的学生信息按照班级编号升序排列，每个班级的人数可以不同，要求读取文件中所有学生的成绩，计算每个班级的平均成绩，将班级编号和平均成绩输出。
主程序已给出，请根据提示补充缺失代码并提交该部分。
```python
#include <stdio.h>
#include <stdlib.h>
```
```python
int
```
```python
main()
{
```
```python
int
```
```python
num=
```
```python
0
```
```python
;
```
```python
/* 班级人数计数 */
```
```python
int
```
```python
sumScore=
```
```python
0
```
```python
;
```
```python
/* 累计成绩*/
```
```python
int
```
```python
curClass;
```
```python
/* 当前班级 */
```
```python
int
```
```python
curScore;
```
```python
/* 当前成绩 */
```
```python
int
```
```python
lastClass;
```
```python
/* 上一个班级*/
```
```python
int
```
```python
readItems;
```
```python
/* 正确读入数据数目 */
```
```python
FILE *fin;
```
```python
/* 输入文件 */
```
```python
fin=fopen(
```
```python
"score.dic"
```
```python
,
```
```python
"r"
```
```python
);
```
```python
/* 打开输入文件读 */
```
```python
if
```
```python
(!fin)
```
```python
/* 文件打开失败 */
```
```python
{
        fprintf(stderr,
```
```python
"error open file!\n"
```
```python
);
```
```python
/* 输出错误信息到标准错误设备 */
```
```python
exit(-
```
```python
1
```
```python
);
```
```python
/* 强制退出，并返回错误码 */
```
```python
}
```
```python
/****************/
```
```python
/*****************/
```
```python
return
```
```python
0
```
```python
;
}
```
**Input**
n名学生的信息（班级编号,姓名,成绩）（文件读入）
**Output**
每个班级的班级编号和平均成绩
**Sample Input**
无（调试时请自建文件作为输入）
**Sample Output**
145811  95
145812  90
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53292629)

