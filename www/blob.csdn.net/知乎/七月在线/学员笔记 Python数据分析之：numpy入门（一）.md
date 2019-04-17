# 学员笔记||Python数据分析之：numpy入门（一） - 知乎
# 



写在最前

这是我学<Python数据分析>时整理出来的numpy基础速读笔记，内容大致分为：Numpy简介，ndarray对象的构建，以及对ndarray属性的理解。欢迎找茬&可劲拍砖！




Numpy简介Numpy是Python下的一个library。

numpy最主要的是支持矩阵操作与运算

非常高效是numpy的优势，core为C编写。提升了python的处理效率

numpy是一些与比较流行的机器学习框架的基础。




**名词解释：**ndarray是numpy的核心数据类型，即（n-dimensional array）多维数组

，tensorflow中的tensor（张量），它本质上也多维数组，但这个名字很高大上。因此，理解多维数组对之后的机器学习会有很大帮助。

## 导入

import numpy as np



![](https://pic3.zhimg.com/v2-01f51058b29df0859440c0e761312862_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='327' height='51'></svg>)



使用帮助

dir(np)




构建ndarray

## **从Python数组构建**




l1=[1,2,3,4,5]

print (type(l1))

l2=np.array(l1)

print (type(l2))



![](https://pic4.zhimg.com/v2-fa1b628a11591833a1974dbb5c6e6167_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='214' height='117'></svg>)



l3=list(l2) # ndarray转为Python list

print (type(l3))



![](https://pic4.zhimg.com/v2-c696a0700bd59cc28094dd31f8e4d2ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='169' height='73'></svg>)



## **构建一维数组**




n1=np.array([1,2,3])

n1.shape



![](https://pic4.zhimg.com/v2-c696a0700bd59cc28094dd31f8e4d2ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='169' height='73'></svg>)



## **构建二维数组**




n2= np.array([[1,2,3],[4,5,6]])

n2.shape



![](https://pic3.zhimg.com/v2-8f1f76aecc53640bfb107d8ec284c136_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='279' height='79'></svg>)



## **快速构建ndarray**

**序列创建：**




np.arange(15)#类似于python中的range，创建一个第一个维度为15的ndarray对象。



![](https://pic2.zhimg.com/v2-2430900cfe43fd552267117c612c02a5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='54'></svg>)



np.arange(2,3,0.1)   #起点，终点，步长值。含起点值，不含终点值。

np.linspace(1,10,10) #起点，终点，区间内点数。起点终点均包括在内。

np.arange(0,1,0.1) #0到1之间步长为0.1的数组, 数组中不包含1  

np.linspace(0, 1, 5) # 开始：0， 结束1, 元素数 5。




**填充创建：**




np.zeros((2,3)) #创建一个全零的，2x3的矩阵, 注意：传入参数是一个tuple,因此别忘了()



![](https://pic3.zhimg.com/v2-19b97ad9c62e1223c0123e71a2f017c2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='245' height='80'></svg>)



np.ones((2,3)) #创建一个全为1的，2x3的矩阵



![](https://pic3.zhimg.com/v2-7044033ce8bf0d80385425b9818a6996_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='261' height='77'></svg>)






np.identity(5) #方阵identity()，只需要一个参数，建立n*n的方阵



![](https://pic2.zhimg.com/v2-ccda0e7bb6fa7ac77db067b21c3d9839_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='305' height='126'></svg>)



a = np.random.rand(5,5)#指定数字矩阵fill()函数

a.fill(7)

a



![](https://pic4.zhimg.com/v2-cc9ab129235865745a06aa0044561bd3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='306' height='161'></svg>)



np.empty((6,6))#空的ndarray,指定其shape即可，注意：空不意味着值为0，而是任何的value,内存中没有被初始化的。



![](https://pic4.zhimg.com/v2-19b527dd19bfa3641cb31ee13133a093_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='213'></svg>)



np.eye(3)# 对角线矩阵



![](https://pic3.zhimg.com/v2-8b712e023919d6e1158070f8b33038ca_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='235' height='96'></svg>)



np.random.rand(3,2) #随机数矩阵：






![](https://pic2.zhimg.com/v2-c01165f8b72355b5a1e098650ddeb3c5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='328' height='92'></svg>)



关于随机数方法：

rand：返回均匀分布随机数

randn:返回服从正态分布的随机数 




## **numpy数据类型及转换**




d1=np.array([1,2,3,4,5])

print(d1)

pint(d1.dtype) # Numpy会自动根据ndarray对象中的值判定数据类型，这里为整型。



![](https://pic1.zhimg.com/v2-bdfe7f570bb5a1894b16009b05a8efd8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='554' height='98'></svg>)



d1.astype(np.float32)# 如果想把它强制转为浮点型，可以用astype函数转换。



![](https://pic4.zhimg.com/v2-235ac8e68812bb5bb8fc91c46781bc13_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='414' height='63'></svg>)



np.random.rand((3,2)，dtype=np.float32)# 也可以在创建ndarray时，即指定其数据类型




**#numpy数据类型参考**




类型

类型代码

说明

int8、uint8

i1、u1

有符号和无符号8位整型（1字节）

int16、uint16

i2、u2

有符号和无符号16位整型（2字节）

int32、uint32

i4、u4

有符号和无符号32位整型（4字节）

int64、uint64

i8、u8

有符号和无符号64位整型（8字节）

float16

f2

半精度浮点数

float32

f4、f

单精度浮点数

float64

f8、d

双精度浮点数

float128

f16、g

扩展精度浮点数

complex64

c8

分别用两个32位表示的复数

complex128

c16

分别用两个64位表示的复数

complex256

c32

分别用两个128位表示的复数

bool

?

布尔型

object

O

python对象

string

Sn

固定长度字符串，每个字符1字节，如S10

unicode

Un

固定长度Unicode，字节数由系统决定，如U10




ndarray基本属性




## ndarray.ndim

查看ndarray的dimension维度数。

n1=np.identity(5)

n1.ndim



![](https://pic3.zhimg.com/v2-3ecb4eb258b8b41d8be9492ed467b4ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='183' height='73'></svg>)



## **ndarray.shape**




查看ndarray的shape形状,返回值 是一个tuple，当维数为2维时，返回的是行数、列数、……数组的各个维(注意和维和维数要区分开)。它是一个数组各个维的长度构成的整数元组。对n行m列矩阵而言，shape将是(n,m)。因此，shape元组的长度也就是rank，也是维数ndim。

## ndarray.dtype

查看ndarray对象的数据类型



![](https://pic1.zhimg.com/v2-266d2f94ea5e65bde413500231f8a020_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='170' height='53'></svg>)



## **ndarray.size**




查看ndarray对象中元素的数量




———未完待续———


