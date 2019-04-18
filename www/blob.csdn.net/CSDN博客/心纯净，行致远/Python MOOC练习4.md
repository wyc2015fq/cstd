# Python MOOC练习4 - 心纯净，行致远 - CSDN博客





2018年09月08日 20:18:28[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：122标签：[mooc																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)](https://so.csdn.net/so/search/s.do?q=mooc&t=blog)
个人分类：[【Python】](https://blog.csdn.net/zhanshen112/article/category/7930576)










# 2、求100以内的素数之和并输出。 

## **输入格式**

 该题目没有输入

## **输入输出示例**


||输入|输出|
|----|----|----|
|示例 1|无（红色字体不是OJ的输入）|说明：直接输出100以内的素数之和。（红色字体不是OJ的输出）|



```python
#素数相加
import math
N=100
i=2
num=2
s=0
for i in range(2,100):
    for num in range(2,int(math.sqrt(i)+1)):
        if (i%num==0):
            break
    else:
        s+=i
print(s)
```

# 3、合格率计算

## 描述

输入一个数字n作为合格标准，然后，输入一系列的数字，每次输入换行表示，空换行结束，输出合格率。

合格率指输入元素中合格元素与全部元素的比值。

## 输入输出示例
|示例|输入|输出|
|----|----|----|
|示例1|60(enter)50(enter)75(enter)90 (这里有一个换行 )|合格率为66.67%|
|示例2|75(enter)65(enter)70(enter)90 (这里有一个换行 )|合格率为33.33%|



```python
n=eval(input())
list=[]
i=input()
while i!='':
    list.append(eval(i))
    i=input()
sum=0
for i in list:
    if i>=n:
        sum+=1
if len(list)==0:
    print("合格率为100.00%")
else:
    x=100*(sum/len(list))
    print("合格率为{:.2f}%".format(x))
```

# 4、**四叶玫瑰数**

## **描述**

四叶玫瑰数是4位数的自幂数。自幂数是指一个 n 位数，它的每个位上的数字的 n 次幂之和等于它本身。（例如：当n为3时，有1^3 + 5^3 + 3^3 = 153，153即是n为3时的一个自幂数，3位数的自幂数被称为水仙花数）


## **输入格式**

该题目没有输入。

**输入输出示例**
||输入|输出|
|----|----|----|
|示例 1|无** （红色字体只表示输入格式）**|**1111****2222****3333 （红色字体只表示输出格式，不是四叶玫瑰数）**|

```python
for i in range(1000,9999):
    a=i//1000
    b=i//100%10
    c=i//10%10
    d=i%10
    if a**4+b**4+c**4+d**4==i:
        print(i)
```

# 5、**用户登录（三次机会）**

## **描述**

给用户三次输入用户名和密码的机会，要求如下：

1）如输入第一行输入用户名为‘Kate’,第二行输入密码为‘666666’，输出‘登录成功！’，退出程序；

2）当一共有3次输入用户名或密码不正确输出“3次用户名或者密码均有误！退出程序。”。



## **输入输出示例**


||输入|输出|
|----|----|----|
|示例 1|Kate			666666|登录成功！|
|示例 2|kate			123			alice			456			john			111111|3次用户名或者密码均有误！退出程序。|

```python
i=0
for i in range(3):
    name=input()
    password=input()
    if name=="Kate" and password=="666666":
        print("登录成功！")
        break;
    i=i+1
if i==3:
    print("3次用户名或者密码均有误！退出程序。")
```






