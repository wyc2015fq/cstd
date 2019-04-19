# MATLAB中排序函数sort()的用法 - 数据之美的博客 - CSDN博客
2017年03月18日 17:31:31[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：41042
MATLAB中排序函数sort()可以对参数的元素进行升序排序或降序排序。
具体的用法如下：
Y=sort(X)
sort()的参数可以是向量，矩阵，数组等等。当X是向量时，sort(X)对X的元素进行升序排序；当X是矩阵时，sort(X)对X的每一列进行升序排序；
**[plain]**[view
 plain](http://blog.csdn.net/dainiwan/article/details/43378331#)[copy](http://blog.csdn.net/dainiwan/article/details/43378331#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/595239/fork)
- >> A=magic(3)  
- 
- A =  
- 
-      8     1     6  
-      3     5     7  
-      4     9     2  
- 
- >> sort(A)  
- 
- ans =  
- 
-      3     1     2  
-      4     5     6  
-      8     9     7  
Y=sort(X,DIM,MODE)
参数DIM表示对哪一个维数进行排序，例如当X是一个二维矩阵，当DIM=1时表示对X的每一列进行排序，当DIM=2时表示对X的每一行进行排序。
参数MODE表示按哪一种模式进行排序，当MODE=‘ASCEND’的时进行升序排序，当MODE=‘DESCEND’时，进行降序排序。
Y返回已经排好序的X。
**[plain]**[view
 plain](http://blog.csdn.net/dainiwan/article/details/43378331#)[copy](http://blog.csdn.net/dainiwan/article/details/43378331#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/595239/fork)
- >> A=magic(3)  
- 
- A =  
- 
-      8     1     6  
-      3     5     7  
-      4     9     2  
- 
- >> sort(A,2,'descend')  
- 
- ans =  
- 
-      8     6     1  
-      7     5     3  
-      9     4     2  
有时我们还需要知道Y中的元素对应于原始序列X中的哪一个元素。于是我们可以用这个命令：
[Y,I] = sort(X,DIM,MODE)
I返回索引序列，它表示Y中的元素与X中元素的对应。
**[plain]**[view
 plain](http://blog.csdn.net/dainiwan/article/details/43378331#)[copy](http://blog.csdn.net/dainiwan/article/details/43378331#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/595239/fork)
- >> A=magic(3)  
- 
- A =  
- 
-      8     1     6  
-      3     5     7  
-      4     9     2  
- 
- >> [B,I]=sort(A,1,'ascend')  
- 
- B =  
- 
-      3     1     2  
-      4     5     6  
-      8     9     7  
- 
- 
- I =  
- 
-      2     1     3  
-      3     2     1  
-      1     3     2  
- 
