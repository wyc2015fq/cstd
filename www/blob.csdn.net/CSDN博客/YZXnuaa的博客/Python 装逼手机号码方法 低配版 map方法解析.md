# Python 装逼手机号码方法 低配版 map方法解析 - YZXnuaa的博客 - CSDN博客
2018年01月16日 10:12:28[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：375
看到微信文章：程序员租房那些事。甚为有趣，自行实现了一下：
#!/usr/bin/python
# -*- coding: UTF-8 -*-
arr = [0,1,6,7,8]
index= [1,4,3,2,1,2,4,2,0,2,1]
tel=""
for i in range(11):
    tel += str(arr[index[i]])
print (tel)
本人研二在读，计算机视觉方向研究，主要相关技能：R-CNN，YOLO，RNN，HMM及少量数据挖掘，欢迎各位大佬交流
/home/ubuntu/PycharmProjects/untitled/venv/bin/python /home/ubuntu/文档/test/PhoneNumber_Test.py
18761686061
Process finished with exit code 0
## 二、map函数
map()是 Python 内置的高阶函数，它接收一个函数 f 和一个 list，并通过把函数 f 依次作用在 list 的每个元素上，得到一个新的 list 并返回。
例如，对于list [1, 2, 3, 4, 5, 6, 7, 8, 9]
如果希望把list的每个元素都作平方，就可以用map()函数：
因此，我们只需要传入函数f(x)=x*x，就可以利用map()函数完成这个计算：
def f(x):
    return x*x
print map(f, [1, 2, 3, 4, 5, 6, 7, 8, 9])
输出结果：
[1, 4, 9, 10, 25, 36, 49, 64, 81]
注意：map()函数不改变原有的 list，而是返回一个新的 list。
利用map()函数，可以把一个 list 转换为另一个 list，只需要传入转换函数。
由于list包含的元素可以是任何类型，因此，map() 不仅仅可以处理只包含数值的 list，事实上它可以处理包含任意类型的 list，只要传入的函数f可以处理这种数据类型。
任务
假设用户输入的英文名字不规范，没有按照首字母大写，后续字母小写的规则，请利用map()函数，把一个list（包含若干不规范的英文名字）变成一个包含规范英文名字的list：
输入：['adam', 'LISA', 'barT']
输出：['Adam', 'Lisa', 'Bart']
def format_name(s):
    s1=s[0:1].upper()+s[1:].lower();
    return s1;
print map(format_name, ['adam', 'LISA', 'barT'])
