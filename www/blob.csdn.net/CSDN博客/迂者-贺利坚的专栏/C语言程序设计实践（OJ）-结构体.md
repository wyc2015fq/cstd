
# C语言程序设计实践（OJ）-结构体 - 迂者-贺利坚的专栏 - CSDN博客

2016年12月14日 07:54:52[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1125



## 2428: 计算该日在本年中是第几天
**Description**
定义一个结构体变量（包括年、月、日）。编写一个函数days，由主函数将年、月、日传递给函数days，计算出该日在本年中是第几天并将结果传回主函数输出。
**Input**
年月日
**Output**
当年第几天
**Sample Input**
2000 12 31
**Sample Output**
366
**HINT**
主函数已给定如下，提交时不需要包含下述主函数
```python
/* C代码 */
```
```python
int
```
```python
main()
{
    y_m_d
```
```python
date
```
```python
;
```
```python
int
```
```python
days(y_m_d);
```
```python
int
```
```python
day_sum;
    scanf(
```
```python
"%d%d%d"
```
```python
,&
```
```python
date
```
```python
.
```
```python
year
```
```python
,&
```
```python
date
```
```python
.
```
```python
month
```
```python
,&
```
```python
date
```
```python
.
```
```python
day
```
```python
);
    day_sum=days(
```
```python
date
```
```python
);
    printf(
```
```python
"%d\n"
```
```python
,day_sum);
    return
```
```python
0
```
```python
;
}
```
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53633368)
## 2877: 结构体—职工信息结构体
**Description**
编写程序，定义一个存放职工信息的结构体类型，职工信息包括职工姓名、工作年限、工资总额。初始化5名职工的信息，对工作年限超过30年的职工每人加100元工资，要求分别输出工资变化之前和之后的所有职工的信息。
**Input**
输入5个员工的职工姓名、工作年限、工资总额
**Output**
输出两部分，第一部分输出工资变化之前所有职工的信息，第二部分输出工资变化之后的所有职工的信息
**Sample Input**
zhang1 5 1000
zhang2 10 2000
zhang3 20 3000
zhang4 40 4000
zhang5 50 5000
**Sample Output**
原始工资
姓名    年限    工资
zhang1    5    1000
zhang2    10    2000
zhang3    20    3000
zhang4    40    4000
zhang5    50    5000
加薪后工资
姓名    年限    工资
zhang1    5    1000
zhang2    10    2000
zhang3    20    3000
zhang4    40    4100
zhang5    50    5100
**HINT**
空4格
[ 参考解答](http://blog.csdn.net/sxhelijian/article/details/53633373)
## 2878: 结构体–学生信息排序
**Description**
定义存放一个学生信息的结构体类型，学生信息包括：姓名，学号，性别，院系，要求在随意输入5个学生的相关信息后，按照学号从小到大顺序输出这些学生的信息。
**Input**
先输入5名学生的信息，按学号顺序排序输出后在输入要统计的院系名称
**Output**
先按学号从小到大的顺序输出学生信息
**Sample Input**
zhang3    20149003    M    信息系
zhang5    20149005    F    信息系
zhang10    20149010    F    管理系
zhang1    20149001    M    会计系
zhang9    20149009    M    会计系
**Sample Output**
zhang1    20149001    M    会计系
zhang3    20149003    M    信息系
zhang5    20149005    F    信息系
zhang9    20149009    M    会计系
zhang10    20149010    F    管理系
**HINT**
数据输出之间有4个空格
[ 参考解答](http://blog.csdn.net/sxhelijian/article/details/53633380)

