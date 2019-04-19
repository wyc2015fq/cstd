# python求数组中最大元素的下标_用户5706034127_新浪博客
|||
**[python]**[view plain](http://blog.csdn.net/blog_empire/article/details/49403273#)[copy](http://blog.csdn.net/blog_empire/article/details/49403273#)
- 
"""
- 
Create by 2015-10-25
- 
- 
@author: zhouheng
- 
- 
In this function you can get the position of the element
- 
that you want in the matrix. 
- 
"""
- 
- 
importnumpy as np 
- 
- 
defgetPositon(): 
- a = np.mat([[2, 5, 7, 8, 9, 89], [6, 7, 5, 4, 6, 4]]) 
- 
- raw, column = a.shape# get the matrix of a raw and column
- 
- _positon = np.argmax(a)# get the index of max in the a
- print_positon 
- m, n = divmod(_positon, column) 
- print"The raw is ",m 
- print"The column is ", n 
- print"The max of the a is ", a[m , n] 
- 
- 
getPositon() 
a =
np.array([1,2,4,7,3,9,5,9,12,4,15,32]).reshape((3,4))
print a
np.argmax(a,axis=0)
np.argmax(a,axis=1)
