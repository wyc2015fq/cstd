# 字符串倒序输出python - miner_zhu的博客 - CSDN博客





2018年07月23日 15:52:01[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：1803








**对于一个给定的字符串，逆序输出，这个任务对于python来说是一种很简单的操作，毕竟强大的列表和字符串处理的一些列函数足以应付这些问题 了，今天总结了一下python中对于字符串的逆序输出的几种常用的方法，一共总结出来了六种方法，个人认为比较全面了，如有其他方法欢迎前来补充**

**方法一：**

**直接使用字符串切片功能逆转字符串**

**方法二：**

**将字符串转换为列表使用reverse函数**

**方法三：**

**新建一个列表，从后往前添加元素**

**方法四：**

**借助于collections模块现成的方法extendleft**

**方法五：**

**递归实现**

**方法六：**

**借助基本的Swap操作，以中间为基准交换对称位置的字符**

**    下面是具体的实现：**

```python
#!usr/bin/env python  
#encoding:utf-8  
  
''''' 
__Author__:沂水寒城 
功能：输入一个字符串，倒叙输出
''' 
 
import collections
 
 
 
def func1(one_str):
    '''
    直接使用字符串切片功能逆转字符串
    '''
    return one_str[::-1]
 
 
def func2(one_str):
    '''
    将字符串转换为列表使用reverse函数
    '''
    one_str_list = list(one_str)
    one_str_list.reverse()
    return ''.join(one_str_list)
 
 
def func3(one_str):
    '''
    新建一个列表，从后往前添加元素
    '''
    one_list=[]
    for i in range(len(one_str)-1,-1,-1):
        one_list.append(one_str[i])
    return ''.join(one_list)
 
 
def func4(one_str):
    '''
    借助于collections模块现成的方法extendleft
    '''
    deque1=collections.deque(one_str)
    deque2=collections.deque()
    for one_char in deque1:
        deque2.extendleft(one_char)
    return ''.join(deque2)
 
 
def func5(one_str):
    '''
    递归实现
    '''
    if len(one_str)<=1:
        return one_str 
    return one_str[-1]+func5(one_str[:-1])
 
 
def func6(one_str):
    '''
    借助基本的Swap操作，以中间为基准交换对称位置的字符
    '''
    one_str_list=list(one_str)
    if len(one_str_list)==0 or len(one_str_list)==1:
        return one_str_list
    i=0
    length=len(one_str_list)
    while i < length/2:
        one_str_list[i], one_str_list[length-i-1]=one_str_list[length-i-1], one_str_list[i]
        i+=1
    return ''.join(one_str_list)
 
 
def main_func(str_list):
    '''
    主调用函数
    '''
    for one_str in str_list:
        one_list=[]
        one=[]
        one_list.append(func1(one_str))
        one_list.append(func2(one_str))
        one_list.append(func3(one_str))
        one_list.append(func4(one_str))
        one_list.append(func5(one_str))
        one_list.append(func6(one_str))
        print '字符串{0}逆序为：'.format(one_str)
        print one_list
 
 
if __name__ == '__main__':
    str_list=['123456','abcdefg','zyxvuw','Together_CZ']
    main_func(str_list)
```



转载网址：：[https://blog.csdn.net/together_cz/article/details/76222558](https://blog.csdn.net/together_cz/article/details/76222558)



