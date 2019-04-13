
# Python字典,函数,全局变量代码解析 - jiahaowanhao的博客 - CSDN博客


2018年02月12日 20:41:38[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：386


[Python字典,函数,全局变量代码解析](http://cda.pinggu.org/view/24718.html)
字典
dict1 = {'name':'han','age':18,'class':'first'}
print(dict1.keys())    \#打印所有的key值
print(dict1.values())   \#打印所有的values值
print("dict1['name']:",dict1['name'])   \#打印name相对应的value值
print(dict1.get('name'))  \#通过字典的get方法得到name相对应的value值
dict1['age']=28  \#字典的修改相当于重新赋值！！！
dict1['address']='beijing'  \#字典的增加是：dict[key] = value 这样的形式
del dict1['name']  \#删除字典中的一个元素
dict1.clear()    \#字典的清空，返回一个空字典
\# del dict1     \#删除字典，字典就完全删除了
字典用法注意：
1、键是不允许相同的，如果相同，后面的会覆盖前面的
2、键是不可变的，所以只能用数字、字符串、元组来担当
dict2 = {(1,2):5,"元组":(4,5)}  \#字典里的元素只能用不可变的数据类型！！！
print(dict2)
print(dict2['元组'])
print(dict2[(1,2)])
for i in dict2.keys():      \#取出的数值更干净！！！谨记老师教导
print("字典中的key值为：",i)
for j in dict2.values():
print("字典中的values值为：",j)
函数
1、函数的定义
函数是实现特定功能而封装起来的一组语句块，可以被用户调用
2、函数的分类
自定义函数；预定义函数（系统自带，lib自带）
3、使用函数的好处
降低编程难度、将大问题分解为若干小问题、可以多次调用
4、函数语法
定义
def函数名字(参数):
函数体
return语句\#不带表达式的return相当于返回none
调用
函数名字
以下是函数的几种：
\#定义函数，函数名最好以_分割
def print_str(str):
print(str)
return
\# 调用函数
print_str("调用一下")
\# 函数传递
\#所有参数在python里都是按引用传递
\#一句话：要变都变！！！
def charge_me(mylist):
mylist.append([1,2,3,4])
print("函数内取值：",mylist)
return
mylist = [10,20,30]
charge_me(mylist)
print("函数外取值：",mylist)   \#函数外和函数内打印是相同的！！！
\#函数的赋值引用
def print_info(name,age=3):
print("name",name)
print("age",age)
return
\# print_info(name="xiao",age=18)
print_info(age=50,name="xiao")    \#python中颠倒是可以的！！！
print_info(name='haha')
\#函数的不定长参数
def p_info(arg1,*vartuple):
print("输出：",arg1)
for var in vartuple:
print(var)
return
p_info(10)
p_info(70,60,50,40,30)
匿名函数lambda，了解即可
\# 1、lambda只是一个表达式，而不是一个代码块，函数体比def简单很多。仅仅能在lambda表达式中封装有限的逻辑
\# 2、lambda[arg1[,arg2,...argn]]:expression
sum1 = lambda arg1,arg2:arg1+arg2
print("相加后的值为：",sum1(10,20))
\# return语句
def sum2(arg1,arg2):
total = arg1+arg2
print("函数内：",total)
return total           \#把total去掉之后返回none
abc = sum2(10,40)
print("函数外：",abc)
\#全局变量和局部变量
\#全局变量比较容易出问题，能不用就不用
total = 0
def sum3(a,b):
total = a+b
print("函数内(局部变量)的值为：",total)
return total
\# total = sum3(10,400)
sum3(20,70)
print("函数外(全局变量)的值为：",total)
count = 1
def do_st():
global count     \#全局变量：global
for i in (3,4,5):  \#循环三次
count += 1
\# print(count)
do_st()
print(count)
\# 思路：当count=1时进入循环+1并赋值给count
\#     在for循环三次后为3+1=4
\#     count是全局变量，最后打印出的结果为4
小练习
\# 不能创建字典的语句是C （字典中的元素不能以列表作为key）
\# A、dict1 = {}
\# B、dict2 = { 3 : 5 }
\# C、dict3 = {[1,2,3]: “uestc”}
\# D、dict4 = {(1,2,3): “uestc”}
\#以下代码输出什么？输出的是6
\# 思路：原始key的值为1，
\# 　　 copy给另一个字典值为1，
\# 　　 重新赋值原来的字典值为5，
\# 　　 所以相加等于6
dict1={'1':1,'2':2}
theCopy=dict1.copy()
dict1['1']=5
sum=dict1['1']+theCopy['1']
print(sum)
\# 合并生成新的字典
dict1 = {3:"c", 4:"d"}
dict2 = {1:"a", 2:"b"}
dict2.update(dict1)    \#更新添加dict1进dict2
print(dict2)
\# 标准日期输出
a = "20170303"
b = a[:4]
c = a[4:6]      \#构思：通过列表分割的方式实现
d = a[6:]
print("格式化后输出的日期是：%s年%s月%s日"%(b,c,d))
无return函数，返回什么？
答：在函数中无return函数，返回none
如何在一个function里面设置一个全局的变量？
答：在函数体内定义一个全局的函数global
\#随机生成验证码的两种方式：
import random
list1=[]
for i in range(65,91):
list1.append(chr(i))  \#通过for循环遍历asii追加到空列表中
for j in range(97,123):
list1.append(chr(j))
for k in range(48,58):
list1.append(chr(k))
ma = random.sample(list1,6)
print(ma)     \#获取到的为列表
ma = ''.join(ma)  \#将列表转化为字符串
print(ma)
import random,string
str1 = "0123456789"
str2 = string.ascii_letters
str3 = str1+str2
ma1 = random.sample(str3,6)
ma1 = ''.join(ma1)
print(ma1)     \#通过引入string模块和random模块使用现有的方法
总结
以上就是本文关于Python字典,函数,全局变量代码解析的全部内容，希望对大家有所帮助

