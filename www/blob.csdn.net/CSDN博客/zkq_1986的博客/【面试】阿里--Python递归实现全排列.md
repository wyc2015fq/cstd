# 【面试】阿里--Python递归实现全排列 - zkq_1986的博客 - CSDN博客





2018年03月21日 09:33:41[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：417








题1： 对一个字符串如s="abcd"里的元素进行全排列。

```python
def r(s, start,end):
    if start >= end:
        print(s)
    else:
        for i in range(start,end):
            s[start],s[i] = s[i],s[start]
            r(s, start+1, end)
            s[start], s[i] = s[i], s[start]

s="abcd"

r(list(s),0,len(s))
```

输出结果：

['a', 'b', 'c', 'd']

['a', 'b', 'd', 'c']

['a', 'c', 'b', 'd']

['a', 'c', 'd', 'b']

['a', 'd', 'c', 'b']

['a', 'd', 'b', 'c']

['b', 'a', 'c', 'd']

['b', 'a', 'd', 'c']

['b', 'c', 'a', 'd']

['b', 'c', 'd', 'a']

['b', 'd', 'c', 'a']

['b', 'd', 'a', 'c']

['c', 'b', 'a', 'd']

['c', 'b', 'd', 'a']

['c', 'a', 'b', 'd']

['c', 'a', 'd', 'b']

['c', 'd', 'a', 'b']

['c', 'd', 'b', 'a']

['d', 'b', 'c', 'a']

['d', 'b', 'a', 'c']

['d', 'c', 'b', 'a']

['d', 'c', 'a', 'b']

['d', 'a', 'c', 'b']

['d', 'a', 'b', 'c']

全排列结果数：![A_{4}^{4}=4\times 3\times 2=24](https://private.codecogs.com/gif.latex?A_%7B4%7D%5E%7B4%7D%3D4%5Ctimes%203%5Ctimes%202%3D24)



题2：

排列：从n个元素中任取m个元素，并按照一定的顺序进行排列，称为排列；

全排列：当n==m时，称为全排列；



比如：集合{ 1,2,3}的全排列为：

{ 1 2 3} 

{ 1 3 2 }

{ 2 1 3 }

{ 2 3 1 }

{ 3 2 1 }

{ 3 1 2 }



递归思想：

取出数组中第一个元素放到最后，即a[1]与a[n]交换，然后递归求a[n-1]的全排列


1）如果数组只有一个元素n=1，a={1} 则全排列就是{1}

2）如果数组有两个元素n=2，a={1,2} 则全排列是：

{2,1}--a[1]与a[2]交换。交换后求a[2-1]={2}的全排列，归结到1)

{1,2}--a[2]与a[2]交换。交换后求a[2-1]={1}的全排列，归结到1)

3）如果数组有三个元素n=3，a={1,2,3} 则全排列是

{{2,3},1}--a[1]与a[3]交换。后求a[3-1]={2,3}的全排列，归结到2）

{{1,3},2)--a[2]与a[3]交换。后求a[3-1]={1,3}的全排列，归结到2）

{{1,2},3)--a[3]与a[3]交换。后求a[3-1]={1,2}的全排列，归结到2）

...

依此类推。

利用python实现全排列的具体代码perm.py如下：

[python] [view plain](http://blog.csdn.net/zhoufen12345/article/details/53560099#)[copy](http://blog.csdn.net/zhoufen12345/article/details/53560099#)
- COUNT=0  
- **def** perm(n,begin,end):  
- **global** COUNT  
- **if** begin>=end:  
- **print** n  
-         COUNT +=1  
- **else**:  
-         i=begin  
- **for** num **in** range(begin,end):  
-             n[num],n[i]=n[i],n[num]  
-             perm(n,begin+1,end)  
-             n[num],n[i]=n[i],n[num]  
- 
- n=[1,2,3,4]  
- perm(n,0,len(n))  
- **print** COUNT  

最后输出的结果如下：

[plain] [view plain](http://blog.csdn.net/zhoufen12345/article/details/53560099#)[copy](http://blog.csdn.net/zhoufen12345/article/details/53560099#)
- ======================== RESTART: D:/Python27/perm.py ========================  
- [1, 2, 3, 4]  
- [1, 2, 4, 3]  
- [1, 3, 2, 4]  
- [1, 3, 4, 2]  
- [1, 4, 3, 2]  
- [1, 4, 2, 3]  
- [2, 1, 3, 4]  
- [2, 1, 4, 3]  
- [2, 3, 1, 4]  
- [2, 3, 4, 1]  
- [2, 4, 3, 1]  
- [2, 4, 1, 3]  
- [3, 2, 1, 4]  
- [3, 2, 4, 1]  
- [3, 1, 2, 4]  
- [3, 1, 4, 2]  
- [3, 4, 1, 2]  
- [3, 4, 2, 1]  
- [4, 2, 3, 1]  
- [4, 2, 1, 3]  
- [4, 3, 2, 1]  
- [4, 3, 1, 2]  
- [4, 1, 3, 2]  
- [4, 1, 2, 3]  
- 24  
- >>>   

转载自：http://blog.csdn.net/zhoufen12345/article/details/53560099



