
# Python实现螺旋矩阵的填充算法示例 - jiahaowanhao的博客 - CSDN博客


2018年04月11日 11:02:21[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：229


[Python实现螺旋矩阵的填充算法示例](http://cda.pinggu.org/view/25236.html)
本文实例讲述了Python实现螺旋矩阵的填充算法。分享给大家供大家参考，具体如下：
afanty的分析：
关于矩阵（二维数组）填充问题自己动手推推，分析下两个下表的移动规律就很容易咯。
对于螺旋矩阵，不管它是什么鬼，反正就是依次向右、向下、向右、向上移动。
向右移动：横坐标不变，纵坐标加1
向下移动：纵坐标不变，横坐标加1
向右移动：横坐标不变，纵坐标减1
向上移动：纵坐标不变，横坐标减1
代码实现：
\#coding=utf-8
importnumpy
'''''
Author: afanty
Date:  2016/6/23
'''
defhelixMatrix(n):
'''''实现n维螺旋矩阵的填充
:param n:维数
:return:螺旋矩阵
'''
ifnotisinstance(n,int)orn <=0:
raiseValueError('请输入合适的维数')
matrix=numpy.zeros((n, n))
left_top=0
right_buttom=n-1
number=1
whileleft_top < right_buttom:
\# 向右移动，横坐标不变，纵坐标+1，number+1
i=left_top
whilei < right_buttom:
matrix[left_top][i]=number
i+=1
number+=1
\# while
\# 向下移动，纵坐标不变，横坐标+1，number+1
i=left_top
whilei < right_buttom:
matrix[i][right_buttom]=number
i+=1
number+=1
\#while
\# 向左移动，横坐标不变，纵坐标-1，number+1
i=right_buttom
whilei > left_top:
matrix[right_buttom][i]=number
i-=1
number+=1
\# while
\# 向上移动，纵坐标不变，横坐标-1，number+1
i=right_buttom
whilei > left_top:
matrix[i][left_top]=number
i-=1
number+=1
\# while
left_top+=1
right_buttom-=1
\# while
ifn%2!=0:
matrix[n/2][n/2]=n*n
returnmatrix
\# end
print("脚本之家测试结果：")
printhelixMatrix(5)
运行结果：
![](http://files.jb51.net/file_images/article/201712/2017122891355384.png?2017112891416)

