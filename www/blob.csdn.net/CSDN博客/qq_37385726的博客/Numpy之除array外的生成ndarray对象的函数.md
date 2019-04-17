# Numpy之除array外的生成ndarray对象的函数 - qq_37385726的博客 - CSDN博客





2018年08月14日 13:54:27[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：37
个人分类：[Numpy](https://blog.csdn.net/qq_37385726/article/category/7935606)

所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









# **目录**

[代码](#%E4%BB%A3%E7%A0%81)

[输出](#%E8%BE%93%E5%87%BA)



# 代码

```
import numpy as np


#默认数据类型为float

#empty返回值为未初始化的ndarray对象
# 第一参数为ndarray对象的维数，第二参数为数据类型
a=np.empty([2,3])
print(a)

#ones返回值为ndarray对象，值全为1
aone=np.ones([2,3])
print(aone)

bone=np.ones([2,3],dtype=int)
print(bone)


#zeros返回值为ndarray对象，值全为0
azero=np.zeros([2,3])
print(azero)
bzero=np.zeros([2,3],dtype=int)
print(bzero)


#eye返回值为对角线或伪对角线为1其余为0的ndarray对象
#第一参数为行，第二参数为列，第二参数可缺省等于第一参数
b=np.eye(3)
print(b)

#第三参数决定伪对角线的位置，为1则上移1条为-1则下移一条，为3则全为0，因为移出去了
c=np.eye(3,3,1)   #等价于np.eye(3,k=1)
print(c)

#第四参数决定数据类型
d=np.eye(3,dtype=int)
print(d)
```

# 输出

[[2.67276450e+185 1.69506143e+190 1.75184137e+190]

 [9.48819320e+077 1.63730399e-306 0.00000000e+000]]



[[1. 1. 1.]

 [1. 1. 1.]]



[[1 1 1]

 [1 1 1]]



[[0. 0. 0.]

 [0. 0. 0.]]



[[0 0 0]

 [0 0 0]]



[[1. 0. 0.]

 [0. 1. 0.]

 [0. 0. 1.]]



[[0. 1. 0.]

 [0. 0. 1.]

 [0. 0. 0.]]



[[1 0 0]

 [0 1 0]

 [0 0 1]]



