# 【Numpy】transpose() - YZXnuaa的博客 - CSDN博客
2018年03月01日 14:43:44[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：216
numpy.transpose()是对矩阵按照所需的要求的转置，比较难理解，现以例子来说明：
**[python]**[view plain](http://blog.csdn.net/Hearthougan/article/details/72626643?locationNum=7&fps=1#)[copy](http://blog.csdn.net/Hearthougan/article/details/72626643?locationNum=7&fps=1#)
- <span style="font-family:'KaiTi_GB2312';font-size:18px;">import numpy as np  
- 
- a = np.array(range(30)).reshape(2, 3, 5)  
- 
- print ("a = ")  
- print (a)  
- 
- print"\n=====================\n"
- 
- print ("a.transpose() = ")  
- print (a.transpose(1, 0, 2))</span>  
输出：
**[python]**[view plain](http://blog.csdn.net/Hearthougan/article/details/72626643?locationNum=7&fps=1#)[copy](http://blog.csdn.net/Hearthougan/article/details/72626643?locationNum=7&fps=1#)
- <span style="font-family:'KaiTi_GB2312';font-size:18px;">a =   
- [[[ 01234]  
-   [ 56789]  
-   [1011121314]]  
- 
-  [[1516171819]  
-   [2021222324]  
-   [2526272829]]]  
- #一维数组中包含2个元素，每个元素是一个包含3个元素的数组，这三个元素每个元素同样又是一个包含5个元素的数组，这就是这个三维数组的解释。
- =====================  
- 
- a.transpose() =   
- [[[ 01234]  
-   [1516171819]]  
- 
-  [[ 56789]  
-   [2021222324]]  
- 
-  [[1011121314]  
-   [2526272829]]]</span>  
       刚开始看这些数据，根本没有头绪，这就要理解transpose()中的参数的意义,因为数组a的shape为（2,3,5），是一个三维数组，那么这个元组对应的索引为：（0,1,2），也就是a.shape的下标：(2[0], 3[1], 5[2])， []中对应的是shape元组的索引。那么，现在，通过b = a.transpose(1, 0, 2)，那么b.shape就变成(3， 2， 5)，这就是说transpose就是改变高维数组的形状，形状改变了，那么里面的元素自然也要重新排列，比如：
      元素11在a中的位置是a[0][2][1]，经过b = a.transpose(1, 0, 2)之后，11在b中的位置就变成b[2][0][1]。再比如元素28，在a中的位置a[1][2][3]，在b中为：a[2][1][3].
