# matlab矩阵元素的累加和累积 - qq_32515081的博客 - CSDN博客
2018年07月15日 16:11:38[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：1886

假设有向量X和矩阵A，那么和求求矩阵元素的累加、累积相关的函数matlab函数用法如下
- 
求向量累加和向量：cumsum(X)
- 
求向量累乘积向量：cumprod(X)
- 
求矩阵每一列的累加和：cumsum(A)、cumsum(A,1)
- 
求矩阵每一列的累积积：cumprod(A)、cumprod(A,1)
- 
求矩阵每一行的累加和：cumsum(A,2)
- 
求矩阵每一行的累积积：cumprod(A,2)
1、编写、建立一个M文件，文件内容如下
- 
%求向量X=(1!,2!,3!)
- 
cumsum(1:10)
- 
cumprod(1:3)
- 
cumsum(1:10,2)
- 
cumprod(1:10,2)
2、程序运行结果![](https://imgsa.baidu.com/exp/pic/item/27d647ee3d6d55fb5b9d68696e224f4a21a4dde3.jpg)
