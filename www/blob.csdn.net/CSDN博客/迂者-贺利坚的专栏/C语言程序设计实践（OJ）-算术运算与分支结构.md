
# C语言程序设计实践（OJ）-算术运算与分支结构 - 迂者-贺利坚的专栏 - CSDN博客

2016年10月12日 11:09:31[迂者-贺利坚](https://me.csdn.net/sxhelijian)阅读数：1306


**（1021）按要求输出**
**Description**
编写一个C程序，输出以下信息：
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
```
```python
Very    Good!
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
```
数＊号可看出，Very前面9空格，Good前面*也是输出的一部分，别光打印Very Good!
**Input**
无需输入
**Output**
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
```
```python
Very    Good!
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
```
**Sample Output**
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
```
```python
Very    Good!
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*****
```
```python
*
```
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52800241)
**（2493）输入两个整数，进行四则运算并输出结果**
**Description**
输入两个整数，进行加减乘除四则运算并输出结果。比如：输入a,b,进行a+b，a-b，a*b，a/b的运算，输出他们的计算结果。
**Input**
输入两个数
**Output**
输出两个数加减乘除的结果，每个结果占一行。
**Sample Input**
6 3
**Sample Output**
9
3
18
2
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52800248)
**（2679）百位数分解**
**Description**
输入一个百位数，将其百位、十位、个位上的数字分解出来，并输出
**Input**
输入一个百位数
**Output**
百位、十位、个位上的数字，数字之间用一个空格隔开
**Sample Input**
123
**Sample Output**
1 2 3
[参考解答](http://blog.csdn.net/sxhelijian/article/details/42495839)
**（2681）求三角形面积**
**Description**
输入三条边的长度，如果这三条边能构成三角形，则需要计算三角形面积，如果不能构成三角形则输出提示信息 “error input”。输出的面积按两位小数方式输出
**Input**
三条边的长度
**Output**
如果这三条边能构成三角形，则输出该三角形面积，如果这三条边不能构成三角形，则输出提示信息“error input”
**Sample Input**
3.3  4.4  5.5
**Sample Output**
area=7.26
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52800355)
**（2013）一元二次方程Ⅱ**
**Description**
求一元二次方程$ax^2+bx+c=0$的解。a,b,c为任意实数。
**Input**
输入数据有一行，包括a b c的值。
**Output**
按以下格式输出方程的根x1和x2。x1和x2之间有一个空格。 x1 x2
（1）如果x1和x2为实根，则以x1>=x2输出。
（2）如果方程是共轭复根，x1=m+ni，x2=m-ni，其中n>0。 其中x1, x2, m,n均保留2位小数。
**Sample Input**
1 2 3
**Sample Output**
-1.00+1.41i -1.00-1.41i
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52800509)
**（1030）求奖金总数**
**Description**
企业发放的奖金根据利润提成。利润低于或等于100000元的，奖金可提10%;
利润高于100000元，低于200000元（$100000<I≤200000$）时，低于100000元的部分按10％提成，高于100000元的部分，可提成 7.5%;
$200000<I≤400000$时，低于200000元部分仍按上述办法提成，（下同），高于200000元的部分按5％提成；
$400000<I≤600000$元时，高于400000元的部分按3％提成；$600000<I≤1000000$时，高于600000元的部分按1.5%提成；
$I>1000000$时，超过1000000元的部分按1%提成。从键盘输入当月利润$I$,求应发奖金总数。
**Input**
一个整数，当月利润。
**Output**
一个整数，奖金。
**Sample Input**
900
**Sample Output**
90
**HINT**
用Switch要比用if的看起来更清晰。
[参考解答](http://blog.csdn.net/sxhelijian/article/details/52800889)

