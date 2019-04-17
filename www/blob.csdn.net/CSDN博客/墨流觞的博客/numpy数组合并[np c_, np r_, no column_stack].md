# numpy数组合并[np.c_, np.r_, no.column_stack] - 墨流觞的博客 - CSDN博客





2018年09月23日 20:20:30[墨氲](https://me.csdn.net/dss_dssssd)阅读数：116








> 
《惊蛰》

谁提了剑嫁衣勾破

披身贫贱夜色 把杀机淹没
## np.r_, np.c_

在实际数据处理中，操作的都是二维数据，因此此处讲的都是最简单的形式。

#### np.c_

等同于np.r_[’-1’, array, array],相当于在最后一个轴上进行数据合并, shape的最后一维.
**对于二维来说最后一个轴就是列， 第一个轴就是行**

```python
```python
a = np.array([[0, 1, 2], [3, 4, 5]])
    np.c_[a, a]
    
    # out: 
    '''
    array([[0, 1, 2, 0, 1, 2],
       [3, 4, 5, 3, 4, 5]])
       '''
       
    np.r_['-1', a,a]

   # out:
      '''
   rray([[0, 1, 2, 0, 1, 2],
       [3, 4, 5, 3, 4, 5]])
   '''
# 再看一个例子：
np.c_[np.array([[1,2,3]]), 0, 0, np.array([[4,5,6]])]

# 注意输出：
'''
array([[1, 2, 3, 0, 0, 4, 5, 6]])
'''
```
```

#### np.r_[‘param’, array[,array,…]]

param:
- -1  按最后一个轴合并， <=>`np.c_[array[,array,...]]`
- 0  按第一个轴合并

```python
```python
a = np.array([[1,2,3]])
    
    a
   ''' array([[1, 2, 3]])'''
    
    np.r_['-1', a, a]
    '''
    array([[1, 2, 3, 1, 2, 3]])
    '''
   np.r_['0', a, a]
   
   '''
   array([[1, 2, 3],
       [1, 2, 3]])
   '''
```
```

# **重点：**

好吧，这个得好好理解，但是项目最常用的就是想一个二维数组中添加行，或者列

1、将一个一维数组合并为一个二维数组的列， 向一个二维数组中添加一个新的列，比如添加一个新的属性，或特征值

```python
```python
a = np.array([[1, 2, 3], [4, 5, 6]])
   # 2 * 3的二维数组
   a
```
```

out:

> 
array([[1, 2, 3],

[4, 5, 6]])


```python
```python
# 添加新的列
	add_columns = [7, 8]
	# option 1:
	np.c_[a, add_columns]
	# option 2
	np.column_stack((a, add_columns))
```
```

out:

> 
array([[1, 2, 3, 7],

[4, 5, 6, 8]])

- 向二维数组添加行，比如添加一个新的样本

这个比较烦，

如果添加的数组不是二维数组，得先转化为二维数组
```python
```python
add_rows = [7, 8, 9]
 add_rows = np.array(add_rows).reshape(1, -1)
 
 add_rows
 # out:  
 '''array([[7, 8, 9]])'''
 
 np.concatenate((a, add_rows), axis=0)
#out:
'''array([[1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]])
'''  
 
 np.r_['0', a , add_rows]
 #out:
 '''array([[1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]])
 '''
```
```






