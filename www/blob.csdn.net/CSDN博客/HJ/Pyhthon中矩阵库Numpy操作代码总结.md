# Pyhthon中矩阵库Numpy操作代码总结 - HJ - CSDN博客
2018年02月04日 15:53:23[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：329
主要 内容：基本操作、数组函数操作、矩阵操作
##基本操作
from numpy import** ***            #导入numpy
a1** =** array**([**1**,**2**,**3**])**
a2** =** array**([**2**,**3**,**4**])**
print a1** +** a2                  #对于元素相加
print a1** *** 2                   #乘以一个数
print a1** *** a2                  #数组点乘操作      
print a1** **** 3                  #表示对数组中的每个数做平方
a4** =** array**([[**1**,**2**,**3**],[**4**,**5**,**6**]])**  #定义多维数组  注意最里面的维度有两层**[]**符号
print a4**[**0**,**0**]**                  #取出第一行第一个数据
print a4**[**0**][**0**]**                 #也可以这样
![](https://img-blog.csdn.net/20180204155030082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
##数组函数操作
import numpy as np
a** =** np**.**zeros**((**2**,**2**))**              #默认的是浮点型
print a
b** =** np**.**zeros**((**2**,**2**),**dtype**(**int32**))** #可以定义数据类型为**int**型
print b
c** =** np**.**full**((**3**,**3**),**'1'**)**           #这样不仅仅可以定义数据，还可以定义字符类型
print c
d** =** np**.**eye**(**3**)**                    #定义单位矩阵
print d
e** =** np**.**random**.**random**((**2**,**2**))**      #定义一个随机矩阵
print e
![](https://img-blog.csdn.net/20180204155152216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
##矩阵操作
m** =** mat**([**1**,**2**,**3**])**
print m
print m**[**0**,**1**]**          #只能按这一种唯一的方法取矩阵中的元素
list** = [**1**,**2**,**3**]**
print mat**(**list**)**       #将列表转换成NumPy的矩阵
m1** =** mat**([**1**,**2**,**3**])**
m2** =** mat**([**4**,**5**,**6**])**
print multiply**(**m1**,**m2**)** #执行点乘操作需要使用函数了，需要注意
m3** =** mat**([[**2**,**5**,**1**],          [**4**,**6**,**2**]])**
m3**.**sort**()**             #对每一行进行排序
print m3
print m3**.**min**()**        #找最小值
print m3**.**max**()**        #找最大值
print m3**.**sum**()**        #对矩阵中所有元素求和
print m3**[**1**,:]**         #取第一行的所有元素
print m3**[**1**,**0**:**1**]**       #第一行第一个（0号下标）元素  注意左闭右开
print m**[**0**][**1**]**         #注意不能像数组那样取值了
![](https://img-blog.csdn.net/20180204155237984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmVpemFvU1lVQUNN/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
