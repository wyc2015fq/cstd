
# C语言程序设计实践（OJ）-字符串 - 迂者-贺利坚的专栏 - CSDN博客

2016年12月09日 09:49:33[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1154



## 2886: 大写改小写
**Description**
输入字符串（长度20以内），将字符串中大写字母改为小写字母，其他字符不变，输出改变后的字符串。
**Input**
一个字符串（长度20以内）
**Output**
输出改变后的字符串（改变规则：将字符串中大写字母改为小写字母，其他字符不变）
**Sample Input****
ABC123bus
**Sample Output**
abc123bus
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53535161)
## 2568: 字符串逆序输出
**Description**
编写一个函数，功能是使输入的字符串逆序输出。
**Input**
输入一串字符串,注意字符串中不要有空格。
**Output**
输出该字符串的逆序。
**Sample Input****
ABCDEFG
**Sample Output**
GFEDCBA
**HINT**
```python
#include<stdio.h>
```
```python
#include<string.h>
```
```python
int
```
```python
main()
{
```
```python
char
```
```python
str
```
```python
[
```
```python
100
```
```python
];
    scanf(
```
```python
"%s"
```
```python
,
```
```python
str
```
```python
);
```
```python
int
```
```python
len;
    len=strlen(
```
```python
str
```
```python
);
```
```python
int
```
```python
fuction(
```
```python
char
```
```python
*,
```
```python
int
```
```python
);
    fuction(
```
```python
str
```
```python
,len);
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
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53535384)
## 2279: 字符串分段(串)
**Description**
输入一个字符串，将其分段输出，分段符为除字母和数字之外的符号。
**Input**
输入一个字符串
**Output**
输出分段后的字符串
**Sample Input****
ad/adfa,,123d?\#a1
**Sample Output**
ad
adfa
123d
a1
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53535418)
## 2417: 字符串长度
**Description**
写一函数，求一个字符串的长度。在main函数中输入字符串，并输出其长度。
**Input**
一行字符串
**Output**
字符串长度
**Sample Input****
t9g(*&WE3@\#$fw2adECWEr
**Sample Output**
22
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
```
```python
int
```
```python
stringlen (
```
```python
char
```
```python
*);
```
```python
char
```
```python
str
```
```python
[
```
```python
100
```
```python
];
```
```python
int
```
```python
len;
    gets(
```
```python
str
```
```python
);
    len=stringlen(
```
```python
str
```
```python
);
    printf(
```
```python
"%d\n"
```
```python
,len);
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
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543005)
## 1052: 两个字符串连接
**Description**
写一函数，将两个字符串连接
**Input**
两行字符串
**Output**
链接后的字符串
**Sample Input****
123
abc
**Sample Output**
123abc
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543036)
## 2284: 按长度截断
**Description**
输入一个字符串，将其按给定的长度n格式化并输出，若n=0，则输出原字符串
**Input**
输入一个字符串
**Output**
格式化并输出
**Sample Input****
asdfasdf
3
**Sample Output**
asd
fas
Df
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543055)
## 2274: 字符串插入
**Description**
将字符串t插入到字符串s中，在位置pos后插入。不得使用字符串操作函数，输出组合成的字符串。
**Input**
输入两个字符串(t和s)和要插入的位置（pos）
**Output**
输出组合后的字符串
**Sample Input****
qwe
jij
3
**Sample Output**
jijqwe
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543084)
## 2802: 判断字符串是否为回文
**Description**
编写程序，判断输入的一个字符串是否为回文。若是则输出“Yes”，否则输出“No”。所谓回文是指順读和倒读都是一样的字符串。
**Input**
**Output**
**Sample Input****
abcddcba
**Sample Output**
Yes
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543100)
## 2569: 统计字符串种类
**Description**
用指针编写一个程序，输入字符串后，统计其中各种字符的个数，输出其中大小写字母，数字，以及其他字符的个数。
主函数已经给出，请编写统计字符种类函数。
**Input**
一串字符串
**Output**
该字符串中大小写字母，数字，以及其他字符的个数，最后输出总字符串长度。
**Sample Input****
I play LOL for 3 years.
**Sample Output**
4
12
1
6
23
**HINT**
```python
#include <stdio.h>
```
```python
int
```
```python
main()
{
```
```python
char
```
```python
str
```
```python
[
```
```python
100
```
```python
];
   gets(
```
```python
str
```
```python
);
```
```python
char
```
```python
*ptr=
```
```python
str
```
```python
;
```
```python
void
```
```python
fuction(
```
```python
char
```
```python
*);
   fuction(ptr);
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
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543105)
## 2424: 字符串比较
**Description**
写一函数，实现两个字符串的比较。即自己写一个strcmp函数，函数原型为
int stringcmp(char *p1,char *p2);
设p1指向字符串s1，p2指向字符串s2。要求当s1=s2时，返回值为0，若s1≠s2，返回它们二者第1个不同字符的ASCII码差值（如”BOY”与”BAD”，第2个字母不同，”O”与”A”之差为79-65=14）。如果s1>s2，则输出正值，如s1
```python
/* C代码 */
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
stringcmp(
```
```python
char
```
```python
*,
```
```python
char
```
```python
*);
```
```python
int
```
```python
m;
```
```python
char
```
```python
str1[
```
```python
80
```
```python
],str2[
```
```python
80
```
```python
],*p1,*p2;
```
```python
scanf
```
```python
(
```
```python
"%s"
```
```python
,str1);
```
```python
scanf
```
```python
(
```
```python
"%s"
```
```python
,str2);
    p1=&str1[
```
```python
0
```
```python
];
    p2=&str2[
```
```python
0
```
```python
];
    m=stringcmp(p1,p2);
```
```python
printf
```
```python
(
```
```python
"%d"
```
```python
,m);
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
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543113)
## 2874: 包含B的字符串
**Description**
输出n个字符串，把其中以包含字母B或b的字符串输出。
**Input**
第一行 n
第二行到第n+1行，每行一个字符串
**Output**
包含字母B或b的字符串
**Sample Input****
6
Ada
Bob
Tom
Brown
Jobs
Alice
**Sample Output**
Bob
Brown
Jobs
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543135)
## 2305: 你就成了我
**Description**
编写一个C程序实现将字符串中的所有”you”替换成”we”
**Input**
输入包含多行数据
每行数据是一个字符串，长度不超过1000
数据以EOF结束
**Output**
对于输入的每一行，输出替换后的字符串
**Sample Input****
you are what you do
**Sample Output**
we are what we do
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543173)
## 2290: 字符串的修改
**Description**
一个字符串s、一个字符串t，判断t是否是s的子串，若是则将s中所有出现了t串的地方换成指定的字符或字符串p，若不是，则输出NO
**Input**
一个字符串s、一个字符串t，要换的字符或字符串
**Output**
输出修改后的s串
**Sample Input****
asdfasdf
132
saf
**Sample Output**
NO
[参考解答](http://blog.csdn.net/sxhelijian/article/details/53543188)

