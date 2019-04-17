# 学员笔记|Python数据分析之numpy入门（三） - 知乎
# 



> 写在最前：在前两篇笔记中，记录了ndarray的创建，读取与维度的转换。本篇笔记中，我将记录ndarray最重要的功能，运算。







笔记来源视频：[http://www.julyedu.com/video/play/65/599](https://link.zhihu.com/?target=http%3A//www.julyedu.com/video/play/65/599)




首先，我们创建两个矩阵x与y




import numpy as np

x=np.array([[1,2],[3,4]],dtype=np.float64)

y=np.array([[5,6],[7,8]],dtype=np.float64)

x,y




#x,y为两个shape均为(2,2)的矩阵






![](https://pic3.zhimg.com/v2-7a9c251fca34b508f50d98b88f703586_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='471' height='285'></svg>)






## 逐元素运算

**numpy ufunc运算**

ufunc是universal function的缩写，它是一种能对数组的每个元素进行操作的函数。numPy内置的许多ufunc函数都在C语言级别实现的，因此它们的计算速度非常快。以下讲到的add,subtract等都是numpy提供的ufunc函数。 

**逐元素运算：**就是两个shap一致的矩阵，相同位置上的元素的运算。




**求和**




我们可以像在python里进行数值运算一样 ，直接运行：x+y

该图用颜色标识出了，**逐元素求和的过程**。这个很容易理解。






![](https://pic2.zhimg.com/v2-5a488448a15c3cd4125e36dc44d80761_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='394' height='373'></svg>)



也可以使用np.add()函数，传入x,y后得到求和结果。




**求差**

与求和类似，两个矩阵直接减，或使用np.subtract()函数






![](https://pic4.zhimg.com/v2-ceef06c181ec5dd5d64366940be2a49b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='316' height='155'></svg>)






**乘与除**




乘法：x*y或者np.multiply(x,y)

除法：x/y或者np.divide(x,y)




## Matrix Multiplication

Numpy可以轻松进行矩阵的乘法，矩阵的乘积使用dot函数进行计算。而对于一维数组，它计算的是其点积。




**向量的内积inner product**






![](https://pic4.zhimg.com/v2-cee5496eb89c050bb2a565d3b40f380f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='418' height='174'></svg>)



即9*10+10*11,最后的结果是200

也可以这样来写np.dot(v,w)

#说明，一维的vector转置后还是自己，上一篇笔记中也提到过这个问题。




**矩阵乘积**

x=np.array([[1,2],[3,4]])

y=np.array([[5,6],[7,8]])

print(x,y)






![](https://pic3.zhimg.com/v2-e3444d4945302cf0c8e0c18e2f461642_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='423' height='328'></svg>)









x.dot(y)#或者是np.dot(x,y)，np.matmul(x,y)






![](https://pic4.zhimg.com/v2-5cdd4a3a39befb74c8dade2d1af4e73b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='476' height='183'></svg>)









dot与matmul的区别






![](https://pic3.zhimg.com/v2-1e8e7e930c847ec53da6753ff972edf2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='540' height='273'></svg>)



#重点：矩阵运算是机器学习中核心的核心，必须多多练习熟悉。




**ndarray对象的转置**

np.arange(16).reshape(2,8).shape






![](https://pic1.zhimg.com/v2-943cdf59d30589b6b5f20e1e33d9fcac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='120'></svg>)



这里先用arange生成了从0到15的一维数组，然后使用reshape，将这个数组的形态改变为有行与列两个维度了（2行8列）#注意这个形状的乘积必须和总的元素个数相同，否则将会报错。

可以把这个数组变更shape为(1，16)，这也是有两个维度，

也可以用reshape转为(2,2,4)，这代表有3个维度，也可以这样去理解：两行两列，每个元素有4个维度来表示。




## ndarray对象内部运算

**求和sum**
![](https://pic1.zhimg.com/v2-2515d794696dca0cc1c27fc92c9aae84_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='441' height='131'></svg>)



其它的运算如np.mean()等运算。

**按矩阵的维度进行运算**

x=np.array([[1,2],[3,4]])

print(x)

print('----------')

print('axis=0:',np.sum(x,axis=0))

print('axis=1:',np.sum(x,axis=1))






![](https://pic1.zhimg.com/v2-7bbaf6b5a7638c4e6e6dff9015159db8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='459' height='272'></svg>)






**#划重点：axis=0时指定是第一个维度，在维度数为2的时候（即一个矩阵），第一个维度就是行，第二个维度就是列。将axis设置为0，即表示消除了行个维度，保留列的维度。所以结果是[4,6]（褚老师原话是，把第0维压扁了）。如果axis设置为1，代表消除了列的维度，保留行的维度。**




## 排序

**一维数组的排序**

使用sort方法可以对元素进行排序。这和python下的排序很像 。






![](https://pic4.zhimg.com/v2-78569c78be63b63a0b0e75a5605e53ab_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='260'></svg>)



arr=np.random.randn(8)*10

print(arr)

arr.sort()

print(arr)




**矩阵的排序**

如果ndarray对象的ndim是2(也就是一个矩阵)那么排序就可以在不同的维度上进行。




arr=np.random.randn(5,3)*10

print(arr)

arr.sort(axis=0)

print(arr)   




#sort指定axis=0，即是排序的时候，依据的维度是第一维（即行），于是我们可以观察到排序后矩阵，在行的方向上，是从小到大的。
![](https://pic4.zhimg.com/v2-7bf8db638cb7ee3f5ff302ca3d7f1a37_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='448'></svg>)



axis=1的情况不再演示了，小伙伴们自己试下。




**排序小实验**

找出排序后位置处于在5%的数字

large_arr =np.random.randn(1000)

large_arr.sort()      

print(Large_arr)

print(large_arr[int(0.05*len(large_arr))])






![](https://pic3.zhimg.com/v2-11b5ae6850c01dc5f40ec328c517ff4e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='133'></svg>)



## 广播算法




当我们使用ufunc函数对两个数组进行计算时，ufunc函数会对这两个数组的对应元素进行计算，因此它要求这两个数组有相同的大小(shape相同)。如果两个数组的shape不同的话，会进行如下的广播(broadcasting)处理。




a=np.random.randn(3,5)

a+10
![](https://pic2.zhimg.com/v2-c6aef1018a1b48cc3af7c4c4ec8df3d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='224'></svg>)






这里的a的shape为(3,5)，第二行和a进行运算的是一标量对象10，这时就会用到广播算法。具体步骤是，将10的在第一个维度扩展3次，第二个维度扩展5次。这时两个对象的shape一致，就可以进行运算了。




**什么时候会进行广播呢：**

当操作两个array时，numpy会 比较它们的shape,当出现下面两种情况时，两array会 兼容和输出broadcasting结果：
- 相等
- 其中一个为1




以下情况会 进行广播：
![](https://pic3.zhimg.com/v2-17e24be537f4740053be8c7b991961ee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='410'></svg>)






写在最后：本节记录了褚老师numpy课程中的ndarray对象的运算，矩阵乘法，转置，排序与广播算法。比前两篇内容多，小伙伴们需要更多的练习，才能理解。下一篇将介绍 ndarray对象的高级处理，就将结束对Numpy的探索了。


