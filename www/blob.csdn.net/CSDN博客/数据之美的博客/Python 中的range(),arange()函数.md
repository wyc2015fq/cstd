# Python 中的range(),arange()函数 - 数据之美的博客 - CSDN博客
2017年03月03日 15:27:41[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：2169
个人分类：[python](https://blog.csdn.net/oppo62258801/article/category/6738404)
[Python](http://lib.csdn.net/base/python) 中的range,以及numpy包中的arange函数
**range()函数**
- 函数说明： range(start, stop[, step]) -> range object，根据start与stop指定的范围以及step设定的步长，生成一个序列。
参数含义：start:计数从start开始。默认是从0开始。例如range（5）等价于range（0，
 5）;
              end:技术到end结束，但不包括end.例如：range（0，
 5） 是[0, 1, 2, 3, 4]没有5
              scan：每次跳跃的间距，默认为1。例如：range（0， 5） 等价于 range(0, 5, 1)
函数返回的是一个range object
例子：
- 
**[python]**[view
 plain](http://blog.csdn.net/qianwenhong/article/details/41414809#)[copy](http://blog.csdn.net/qianwenhong/article/details/41414809#)
- >>> range(0,5)                 #生成一个range object,而不是[0,1,2,3,4] 
- range(0, 5)     
- >>> c = [i for i in range(0,5)]     #从0 开始到4，不包括5，默认的间隔为1
- >>> c  
- [0, 1, 2, 3, 4]  
- >>> c = [i for i in range(0,5,2)]   #间隔设为2
- >>> c  
- [0, 2, 4]  
- 若需要生成[ 0.   0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9]
**[python]**[view
 plain](http://blog.csdn.net/qianwenhong/article/details/41414809#)[copy](http://blog.csdn.net/qianwenhong/article/details/41414809#)
- >>> range(0,1,0.1)    #range中的setp 不能使float
- Traceback (most recent call last):  
-   File "<pyshell#5>", line 1, in <module>  
-     range(0,1,0.1)  
- TypeError: 'float' object cannot be interpreted as an integer  
arrange()函数
- 函数说明：arange([start,] stop[, step,], dtype=None)根据start与stop指定的范围以及step设定的步长，生成一个 ndarray。 dtype
 : dtype
        The type of the output array.  If `dtype` is not given, infer the data
        type from the other input arguments.
**[python]**[view
 plain](http://blog.csdn.net/qianwenhong/article/details/41414809#)[copy](http://blog.csdn.net/qianwenhong/article/details/41414809#)
- >>> np.arange(3)  
-   array([0, 1, 2])  
-   >>> np.arange(3.0)  
-   array([ 0.,  1.,  2.])  
-   >>> np.arange(3,7)  
-   array([3, 4, 5, 6])  
-   >>> np.arange(3,7,2)  
-   array([3, 5])  
![](http://static.blog.csdn.net/images/save_snippets.png)
- 
**[python]**[view
 plain](http://blog.csdn.net/qianwenhong/article/details/41414809#)[copy](http://blog.csdn.net/qianwenhong/article/details/41414809#)
- >>> arange(0,1,0.1)  
- array([ 0. ,  0.1,  0.2,  0.3,  0.4,  0.5,  0.6,  0.7,  0.8,  0.9])  
- 
