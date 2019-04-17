# python-numpy常见ndarray操作 - BryantLJ学习的地方 - CSDN博客





2017年03月12日 09:32:17[遍地流金](https://me.csdn.net/u012177034)阅读数：13418标签：[numpy](https://so.csdn.net/so/search/s.do?q=numpy&t=blog)
个人分类：[python](https://blog.csdn.net/u012177034/article/category/6538892)









注意numpy里有两种数据类型，ndarray和matrix，一般用ndarray，要用到矩阵的乘除法时再用matrix。ndarray里的数学运算默认为点乘！

# 1.array初始化

a = np.array([[1,2,3],[2,3,4]]) 

a = np.zeros([3,2]) 

a = np.arange(0,10,2) # 产生0到10（不包括10）之间的步长为2的等差数列, 

a = np.linspace(0,10,2) # 产生一个0到10（包括10）之间的总长度为2的等差数列 

注意： 

a = [1,2]初始化的a是list类型，而a =np.array([1,2])是np.ndarray类型，两者不一样，最明显的区别就是[1,2]*2 = [1,2,1,2]而np.array([1,2])*2 = [2,4] 

python自带的range(0,10,2)会产生一个与np.arange(0,10,2)包含同样数据的list
# 2.array的形状及变换

a.shape 

a.reshape([2,3]) 

a.reshape([2,3,0]) 

a.reshape([3,-1]) # -1代表该维大小自动调整
# 3.array的组合、扩展

b = np.ones([3,2]) 

np.vstack(a,b) # 竖直堆放矩阵 

np.hstack(a,b) # 水平对方矩阵 

np.concatenate((a,b),axis = 2) # 沿着2轴堆放矩阵 

a.transpose() # 矩阵转置
# 4.array的运算

idx = a.argmax(axis = 2) # 求第2维的最大值的下标，从0开始，axis=0代表列，axis=1代表行 

res = a.max(axis = 2) # 求第2维的最大值

# 5.元素的赋值与利用

a[:,1]取出第一列的所有行 

a[:-1,1]取出第一列的0到倒数第二行，-1代表从后往前数第二个






