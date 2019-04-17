# 学员笔记||Python数据分析之numpy入门（二） - 知乎
# 




![](https://pic3.zhimg.com/v2-530daac5e2e47b25bf95ffce95d78c5e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='209'></svg>)






写在最前：在上一篇numpy文章后，我们学会了如何创建多维数组，那本篇将学习对ndarray进行取值与赋值操作，并进行一些维度上的操作。




首先创建一个矩阵。

import numpy as np

a=np.array([[1,2,3,4],[5,6,7,8],[9,10,11,12]])



![](https://pic2.zhimg.com/v2-9df385f5c6e1eeaa6f44126badd5d971_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='375' height='150'></svg>)



手工创建一个二维矩阵名为a，shape为(3,4)

接下来开始对这个矩阵进行读取

## 读取

**切片读取**

还记得Python中对列表进行切片访问吗？numpy对矩阵进行切片访问时也是遵循着同样 的规则，只不过python的列表中是按一行（一维）方向进行访问，而在numpy中是按行，列两个维度进行切片。（如果ndarray的ndim超过2，也是遵循同时的方式进行访问）

a[:,:]#第一个冒号代表切出所有的行，第二个冒号代表切出所有的列。这样会比较容易理解。



![](https://pic4.zhimg.com/v2-225d76f5b0a16aa05b2a045d12e0af07_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='216' height='99'></svg>)



a[2:,2:]#这里的红框代表选中的行，绿框代表，选中的列。



![](https://pic3.zhimg.com/v2-7852fa146320ab7a1e0145604473b21a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='218' height='169'></svg>)



a[:,1:3]#第一个冒号代表，选中所有行，第二个参数1:3代表从第二列到第3列。同样遵循python的左闭右开的切片访问规则。



![](https://pic2.zhimg.com/v2-ea87437b04701811f85e3a98f41c3ae1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='160' height='96'></svg>)
**索引访问**

也可以对ndarray在指定维度上的进行索引式访问

a[[2],[0,2]]

#红色的行代表第一个参数[2]，代表先选中第3行。绿色的列代表第二个参数[0,2],选中了第一个和第三个元素9,11。

#注意，使用索引访问时，索引值要放进[]中，代表传入一个列表。



![](https://pic1.zhimg.com/v2-cc16cd100077bbb4c8df9d3d6ce8f198_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='243' height='164'></svg>)






#除了可以指定数字为索引，还可以用布尔类型的数组，做为索引传入，对下标进行过滤。

bool_a=a>2

bool_a

a[bool_a]






![](https://pic1.zhimg.com/v2-af3e6c41ea6549a918e98717c8b38138_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='380' height='181'></svg>)






**切片与索引混合访问**

访问ndarray的方式非常灵活，可以使用切片与索引可以混合进行访问。

a[[1,2],:]



![](https://pic4.zhimg.com/v2-2e11aaecde1086afe94785037684f1d7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='219' height='77'></svg>)






最后放出网上对ndarray读取的经典图片，更直观地去理解



![](https://pic1.zhimg.com/v2-9359ba6aca2233635e4906cc8f12be80_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='395' height='163'></svg>)








![](https://pic2.zhimg.com/v2-574e93b9e8f896f37f4af658c61e4629_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='404' height='203'></svg>)
**遍历读取**

对于ndarray，也可以像对待Python中的可迭代对象一样，对它进行遍历。

for i in a:

   print(i)



![](https://pic2.zhimg.com/v2-9d709117298abe4b0659854488cab0e1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='205' height='110'></svg>)
**深拷贝与浅拷贝**

当a1用=号赋给a2时，我们可以认为这只是建立了一个指针。当修改a2时，a1也会被同步修改。如果想完全复制一份新的a1,怎么办呢？用copy方法。我们可以用is来确定两个对象的关系。

a1=np.random.rand(3,5)

a2=a1

a2 is a1

a3=a1.copy()

a3 is a1



![](https://pic2.zhimg.com/v2-6052c76d4c58a7aa66b9281a5f60197d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='245' height='182'></svg>)






## 赋值




既然能取出来，那赋值就很简单了，直接=号 就可以。在此就不多做笔记了。




## ndarray维度操作

ndarray里的维度经常把人搞晕。多练习就好，




**增维操作np.newaxis**

import numpy as np

x=np.linspace(-1,1,100)

x.shape

x[:,np.newaxis].shape

x[np.newaxis,:].shape



![](https://pic2.zhimg.com/v2-b3e0151a0693055d9073ff4a3051b5d9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='333' height='217'></svg>)






## 维度转置np.transpose




shape为(100,)这种ndarray是无法进行转维，因为它的ndim为1 x=x[np.newaxis,:]#先增维

np.transpose(x).shape#然后可以维度转置了




全降维为一维数组：

x.flatten()







本段学习体会：对ndarray的读取是涉及到多个维度的选取，用法也比较灵活。只做笔记是不够的，后期还需要大量的练习才能够熟练使用。下一篇将介绍，ndarray最重要的功能：运算。一起加油。




———未完待续———




