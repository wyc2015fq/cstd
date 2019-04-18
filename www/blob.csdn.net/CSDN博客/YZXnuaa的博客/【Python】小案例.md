# 【Python】小案例 - YZXnuaa的博客 - CSDN博客
置顶2018年02月06日 16:25:52[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：625
6. 斐波那契数列 ！！
# !/usr/bin/python
# -*- coding: UTF-8 -*-
def fib(n):
    a, b = 1, 1
for i in range(n - 1):
        a, b = b, a + b
    return a
# 输出了第10个斐波那契数列
print(fib(10))
55
13.排序
# !/usr/bin/python
# -*- coding: UTF-8 -*-
array = [5, 4, 3, 2, 1]
for i in range(0, 5):
    for j in range(i, 5):
        if array[i] > array[j]:
            array[i], array[j] = array[j], array[i]
print(array)
[1, 2, 3, 4, 5]
14.水仙花数
# !/usr/bin/python
# -*- coding: UTF-8 -*-
def test(n):
    a = int(n/100)
    b = int(n % 100/10)
    c = n % 10
m = a ** 3 + b ** 3 + c ** 3
print(a,b,c)
    if n == m:
        return n
    else:
        return 0
x = int(input('integer:\n'))
print(test(x))
integer:
370
3 7 0
370
19.一个数如果恰好等于它的因子之和，这个数就称为"完数"。例如6=1＋2＋3.编程找出1000以内的所有完数。
20.一球从100米高度自由落下，每次落地后反跳回原高度的一半；再落下，求它在第10次落地时，共经过多少米？第10次反弹多高？
# !/usr/bin/python
# -*- coding: UTF-8 -*-
tour = [100]
height = []
hei = 100.0  # 起始高度
tim = 2  # 次数
for i in range(2, tim + 1):
    # 从第二次开始，落地时的距离应该是反弹高度乘以2（弹到最高点再落下）
hei /= 2
tour.append(2 * hei)
    height.append(hei)
print('总高度：tour = {0}'.format(sum(tour)))
print('第10次反弹高度：height = {0}'.format(height[-1]))
总高度：tour = 299.609375
第10次反弹高度：height = 0.1953125
22.两个乒乓球队进行比赛，各出三人。甲队为a,b,c三人，乙队为x,y,z三人。已抽签决定比赛名单。有人向队员打听比赛的名单。a说他不和x比，c说他不和x,z比，请编程序找出三队赛手的名单。
#!/usr/bin/python
# -*- coding: UTF-8 -*-
for a in ['x','y','z']:
    for b in ['x', 'y', 'z']:
        for c in ['x', 'y', 'z']:
            if(a!=b)and(b!=c)and(c!=a) and (a!='x') and (c!='x') and (c!='z'):
                print ('a和%s比赛，b和%s比赛，c和%s比赛' %(a,b,c))
a和z比赛，b和x比赛，c和y比赛
23.打印
#!/usr/bin/python
# -*- coding: UTF-8 -*-
matrix = [[' ' for i in range(7)] for i in range(7)]
for i in range(7):
    for j in range(7):
        if 2 < i+j < 10 and abs(i-j) < 4:
            matrix[i][j] = '*'
print(matrix[i])
[' ', ' ', ' ', '*', ' ', ' ', ' ']
[' ', ' ', '*', '*', '*', ' ', ' ']
[' ', '*', '*', '*', '*', '*', ' ']
['*', '*', '*', '*', '*', '*', '*']
[' ', '*', '*', '*', '*', '*', ' ']
[' ', ' ', '*', '*', '*', ' ', ' ']
[' ', ' ', ' ', '*', ' ', ' ', ' ']
24.有一分数序列：2/1，3/2，5/3，8/5，13/8，21/13...求出这个数列的前20项之和。
#!/usr/bin/python
# -*- coding: UTF-8 -*-
a = 2.0
b = 1.0
s = 0
for n in range(1, 21):
    s += a / b
    a, b = a+b, a
print(s)
32.66026079864164
25.求1+2!+3!+...+20!的和。
#!/usr/bin/python
# -*- coding: UTF-8 -*-
oduct = 1
sum = 0
for i in range(1,21):
    oduct = oduct*i
    print(oduct)
    sum = sum + oduct
print(sum)
1
2
6
24
120
720
5040
40320
362880
3628800
39916800
479001600
6227020800
87178291200
1307674368000
20922789888000
355687428096000
6402373705728000
121645100408832000
2432902008176640000
2561327494111820313
! 27 利用递归函数调用方式，将所输入的5个字符，以相反顺序打印出来。
# !/usr/bin/python
# -*- coding: UTF-8 -*-
def output(s, l):
    if l == 0:
        return
print(s[l - 1],end='')
    output(s, l - 1)
s = input('Input a string:')
l = len(s)
output(s, l)
Input a string:asdfasdfasdf
fdsafdsafdsa
30.判断它是不是回文数。即12321是回文数，个位与万位相同，十位与千位相同。
# !/usr/bin/python
# -*- coding: UTF-8 -*-
n = input('input\n')
s = str(n)
flag = False
a=int(len(s))
for i in range(a):
    if s[i] == s[len(s)-1-i]:
        flag=True
if flag==True:
    print(n)
else:
    print('Error')
32.按相反的顺序输出列表的值。
# !/usr/bin/python
# -*- coding: UTF-8 -*-
a = ['one', 'two', 'three']
for i in a[::-1]:
    print(i,end=' ')
three two one 
46. 题目很简单，求输入数字的平方，如果平方运算后小于 50 则退出。但是true和false的定义问题不是很懂
# !/usr/bin/python
# -*- coding: UTF-8 -*-
TRUE = 1
FALSE = 0
def SQ(x):
    return x * x
print('如果输入的数字小于 50，程序将停止运行。')
again = 1
while again:
    num = int(input('请输入一个数字：'))
    print('运算结果为: %d' % (SQ(num)))
    if SQ(num) >= 50:
        again = TRUE
    else:
        again = FALSE
50.输出一个随机数
# !/usr/bin/python
# -*- coding: UTF-8 -*-
import random
# 生成 10 到 20 之间的随机数
print(random.uniform(10, 20))
52. | 逐位相或   ^ 逐位相与
# !/usr/bin/python
# -*- coding: UTF-8 -*-
if __name__ == '__main__':
    a = 0x66
b = a | 3
print('a | b is %d' % b)
    b |= 7
print('a | b is %d' % b)
# !/usr/bin/python
# -*- coding: UTF-8 -*-
if __name__ == '__main__':
    a = 0x66
b = a ^ 3
print('a | b is %d' % b)
    b ^= 7
print('a | b is %d' % b)
62.查找字符串。
#!/usr/bin/python
# -*- coding: UTF-8 -*-
sStr1 = 'abcdefg'
sStr2 = 'cde'
print(sStr1.find(sStr2))
69.有n个人围成一圈，顺序排号。从第一个人开始报数（从1到3报数），凡报到3的人退出圈子，问最后留下的是原来第几号的那位。
#!/usr/bin/python
# -*- coding: UTF-8 -*-
if __name__ == '__main__':
    n = int(input('请输入:'))
    num = []
    for i in range(n):
        num.append(i + 1)
    print(num)
    b = 0
while True:
        for i in range(int(len(num)/3)):
            print(i)
            del num[i*3+2-b]
        print(num)
        b = len(num)%3
if len(num)==2:
            del num[2-b]
            break
print(num)
71.编写input()和output()函数输入，输出5个学生的数据记录。
# !/usr/bin/python
# -*- coding: UTF-8 -*-
N = 3
student = []
for i in range(5):
    student.append(['', '', []])
def input_stu(stu):
    for i in range(N):
        stu[i][0] = input('input student num:\n')
        stu[i][1] = input('input student name:\n')
        for j in range(3):
            stu[i][2].append(int(input('score:\n')))
def output_stu(stu):
    for i in range(N):
        print('%-6s%-10s' % (stu[i][0], stu[i][1]))
        for j in range(3):
            print('%-8d' % stu[i][2][j])
if __name__ == '__main__':
    input_stu(student)
    print(student)
    output_stu(student)
74.列表排序及连接。
**程序分析：**排序可使用 sort() 方法，连接可以使用 
+ 号或 extend() 方法。
# !/usr/bin/python
# -*- coding: UTF-8 -*-
if __name__ == '__main__':
    a = [1, 3, 2]
    b = [3, 4, 5]
    a.sort()  # 对列表 a 进行排序
print(a)
    # 连接列表 a 与 b
print(a+b)
    # 连接列表 a 与 b
a.extend(b)
    print(a)
[1, 2, 3]
[1, 2, 3, 3, 4, 5]
[1, 2, 3, 3, 4, 5]
88. ！！打印 *** 读取7个数（1—50）的整数值，每读取一个值，程序打印出该值个数的＊。
# !/usr/bin/python
# -*- coding: UTF-8 -*-
if __name__ == '__main__':
    n = 1
while n <= 7:
        a = int(input('input a number:\n'))
        while a < 1 or a > 50:
            a = int(raw_input('input a number:\n'))
        print(a * '*')
        n += 1
91.等待一秒
# !/usr/bin/python
# -*- coding: UTF-8 -*-
import time
print(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time())))
# 暂停一秒
time.sleep(1)
print(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(time.time())))
/home/ubuntu/PycharmProjects/untitled/venv/bin/python /home/ubuntu/文档/test/argpurse_test.py
2018-02-05 13:41:56
2018-02-05 13:41:57
Process finished with exit code 0
47.
**题目：**两个变量值互换。
**程序分析：**无
程序源代码：
# !/usr/bin/python
# -*- coding: UTF-8 -*-
def exchange(a, b):
    a, b = b, a
    return (a, b)
if __name__ == '__main__':
    x = 10
y = 20
print('x = %d,y = %d' % (x, y))
    x, y = exchange(x, y)
    print('x = %d,y = %d' % (x, y))
/home/ubuntu/PycharmProjects/untitled/venv/bin/python /home/ubuntu/文档/test/argpurse_test.py
x = 10,y = 20
x = 20,y = 10
Process finished with exit code 0
100.
列表转换为字典。
# !/usr/bin/python
# -*- coding: UTF-8 -*-
i = ['a', 'b']
l = [1, 2]
print(dict([i, l]))
