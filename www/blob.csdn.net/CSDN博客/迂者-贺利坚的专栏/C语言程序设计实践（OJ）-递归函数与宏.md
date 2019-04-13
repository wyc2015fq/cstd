
# C语言程序设计实践（OJ）-递归函数与宏 - 迂者-贺利坚的专栏 - CSDN博客

2016年11月15日 10:15:08[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1392



## 2968: 递归方法实现逆序
**Description**
你知道递归么？简单来说，递归就是函数自身对自身的调用，直到遇到终止条件，再对数据做逆向处理，下面就有一个递归的简单例子等你来处理。
```python
#include <stdio.h>
```
```python
void
```
```python
inverted(
```
```python
int
```
```python
);
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
/*需要逆序的整数*/
```
```python
scanf(
```
```python
"%d"
```
```python
,&n);
    inverted(n);
```
```python
/*调用逆序函数*/
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
```python
void
```
```python
inverted(
```
```python
int
```
```python
n)
{
```
```python
if
```
```python
(n<
```
```python
10
```
```python
)
```
```python
/*当数字只剩一位的时候，即第一位数，输出，变为末位，返回*/
```
```python
{
        printf(
```
```python
"%d"
```
```python
,n);
```
```python
return
```
```python
;
    }
```
```python
/***************/
```
```python
/***************/
```
```python
return
```
```python
;
}
```
**Input**
一个int型整数
**Outputt**
转换后的整数
**Sample Input**
1234567
**Sample Outputt**
7654321
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53168703)
## 2747: 进制转换（十进制转二进制）
**Description**
输入一个非负整数(十进制)，输出此整数对应的二进制数
**Input**
正整数(十进制）
**Outputt**
对应的二进制数
**Sample Input**
6
**Sample Outputt**
110
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53168733)
## 1942: 进制转换
**Description**
输入一个十进制数N，将它转换成R进制数输出。
**Input**
输入数据包含多个测试实例，每个测试实例包含两个整数N(32位整数)和R（2<=R<=16, R<>10）。
**Outputt**
为每个测试实例输出转换后的数，每个输出占一行。如果R大于10，则对应的数字规则参考16进制（比如，10用A表示，等等）。
**Sample Input**
7 2
23 12
-4 3
**Sample Outputt**
111
1B
-11
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53168761)
## 2021: 汉诺塔
**Description**
汉诺塔（又称河内塔）问题是印度的一个古老的传说。开天辟地的神勃拉玛在一个庙里留下了三根金刚石的棒A、B和C，A上面套着n个圆的金片，最大的一个在底下，其余一个比一个小，依次叠上去，庙里的众僧不倦地把它们一个个地从A棒搬到C棒上，规定可利用中间的一根B棒作为帮助，但每次只能搬一个，而且大的不能放在小的上面。僧侣们搬得汗流满面，可惜当n很大时这辈子恐怕就很搬了 聪明的你还有计算机帮你完成，你能写一个程序帮助僧侣们完成这辈子的夙愿吗？
**Input**
输入金片的个数n。这里的n<=10。
**Outputt**
输出搬动金片的全过程。格式见样例。
**Sample Input**
2
**Sample Outputt**
Move disk 1 from A to B
Move disk 2 from A to C
Move disk 1 from B to C
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53169044)
## 1056: (用宏)输出已交换后的两个值
**Description**
定义一个带参的宏（或者模板函数），带有三个参数，第一个参数为类型，后两个参数的值互换，并写出程序，输入两个数作为使用宏时的实参。输出已交换后的两个值。
**Input**
两个短整型数，空格隔开
两个小数，空格隔开
两个长整数，空格隔开
**Outputt**
交换后的两个数，空格隔开
**Sample Input**
1 2
1.5 2.5
65535 2147483647
**Sample Outputt**
2 1
2.5 1.5
2147483647 65535
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53169066)
## 2970: 利用参数宏进行角度与弧度转换
**Description**
定义参数宏`AngleToRadian(degree,minute,second)`，将度(degree)、分(minute)、秒(second)转换为弧度。（`#define PI 3.14159`）
```python
#include <stdio.h>
#define PI
```
```python
3.14159
```
```python
/**********/
```
```python
在此补充缺少的代码
```
```python
/**********/
```
```python
int
```
```python
main()
{
```
```python
double
```
```python
degree,minute,second;
    scanf(
```
```python
"%lf %lf %lf"
```
```python
,&degree,&minute,&second);
    printf(
```
```python
"%f"
```
```python
,AngleToRadian(degree,minute,second));
```
```python
/*参数宏会被替换为定义的表达式，算出结果后输出*/
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
度，分，秒
**Outputt**
转换后的弧度
**Sample Input**
180 0 0
**Sample Outputt**
3.141590
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53169094)

