# Numpy之arrange - qq_37385726的博客 - CSDN博客





2018年08月14日 13:51:54[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：473
所属专栏：[Numpy](https://blog.csdn.net/column/details/26170.html)









[Python](http://lib.csdn.net/base/python) 中的range,以及numpy包中的arange函数



# **range()函数**


- 
函数说明： range(start, stop[, step]) -> range object，根据start与stop指定的范围以及step设定的步长，***生成一个序列。***

	参数含义：start:计数从start开始。默认是从0开始。例如range（5）等价于range（0， 5）;

	              end:技术到end结束，但不包括end.例如：range（0， 5） 是[0, 1, 2, 3, 4]没有5

	              scan：每次跳跃的间距，默认为1。例如：range（0， 5） 等价于 range(0, 5, 1)

	函数返回的是一个range object

	例子：

- **[python]**[view plain](https://blog.csdn.net/weixin_37226516/article/details/60881914#)[copy](https://blog.csdn.net/weixin_37226516/article/details/60881914#)- >>> range(0,5)                 #生成一个range object,而不是[0,1,2,3,4]   
- range(0, 5)     
- >>> c = [i for i in range(0,5)]     #从0 开始到4，不包括5，默认的间隔为1  
- >>> c  
- [0, 1, 2, 3, 4]  
- >>> c = [i for i in range(0,5,2)]   #间隔设为2  
- >>> c  
- [0, 2, 4]  

```
>>> range(0,5)               #生成一个range object,而不是[0,1,2,3,4] 
range(0, 5)   
>>> c = [i for i in range(0,5)]     #从0 开始到4，不包括5，默认的间隔为1
>>> c
[0, 1, 2, 3, 4]
>>> c = [i for i in range(0,5,2)]   #间隔设为2
>>> c
[0, 2, 4]
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8

- 若需要生成[ 0.   0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9]
**[python]**[view plain](https://blog.csdn.net/weixin_37226516/article/details/60881914#)[copy](https://blog.csdn.net/weixin_37226516/article/details/60881914#)- >>> range(0,1,0.1)    #range中的setp 不能使float  
- Traceback (most recent call last):  
-   File ”<pyshell#5>”, line 1, in <module>  
-     range(0,1,0.1)  
- TypeError: ’float’ object cannot be interpreted as an integer  

```
>>> range(0,1,0.1)    #range中的setp 不能使float
Traceback (most recent call last):
  File "<pyshell#5>", line 1, in <module>
    range(0,1,0.1)
TypeError: 'float' object cannot be interpreted as an integer
```
- 1
- 2
- 3
- 4
- 5




# arrange()函数


- 函数说明：arange([start,] stop[, step,], dtype=None)根据start与stop指定的范围以及step设定的步长，***生成一个 ndarray***。 dtype : dtype

	        The type of the output array.  If `dtype` is not given, infer the data

	        type from the other input arguments.**[python]**[view plain](https://blog.csdn.net/weixin_37226516/article/details/60881914#)[copy](https://blog.csdn.net/weixin_37226516/article/details/60881914#)- >>> np.arange(3)  
-   array([0, 1, 2])  
-   >>> np.arange(3.0)  
-   array([ 0.,  1.,  2.])  
-   >>> np.arange(3,7)  
-   array([3, 4, 5, 6])  
-   >>> np.arange(3,7,2)  
-   array([3, 5])  

```
>>> np.arange(3)
    array([0, 1, 2])
    >>> np.arange(3.0)
    array([ 0.,  1.,  2.])
    >>> np.arange(3,7)
    array([3, 4, 5, 6])
    >>> np.arange(3,7,2)
    array([3, 5])
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8




```python
import numpy as np


#arange函数返回值为ndarray对象，数组

#仅一个参数就是从[0,参数)步长为1
a=np.arange(5)
print(a)

#start,end,step,dtype
b=np.arange(1,6,2,dtype=float)
print(b)

#返回值为数组，则通过修改shape可以变成矩阵
#b.shape=(1,b.shape) 报错因为b.shape返回为元组(3,)
b.shape=(1,b.size)
print(b)
```






- **[python]**[view plain](https://blog.csdn.net/weixin_37226516/article/details/60881914#)[copy](https://blog.csdn.net/weixin_37226516/article/details/60881914#)- >>> arange(0,1,0.1)  
- array([ 0. ,  0.1,  0.2,  0.3,  0.4,  0.5,  0.6,  0.7,  0.8,  0.9])  

```
>>> arange(0,1,0.1)
array([ 0. ,  0.1,  0.2,  0.3,  0.4,  0.5,  0.6,  0.7,  0.8,  0.9])
```
- 1
- 2
- 3








