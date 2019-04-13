
# 60. Permutation Sequence - OraYang的博客 - CSDN博客

2017年09月21日 11:23:24[OraYang](https://me.csdn.net/u010665216)阅读数：387所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



**重点内容**\#\#题目
The set [1,2,3,…,n] contains a total of n! unique permutations.
By listing and labeling all of the permutations in order,
We get the following sequence (ie, for n = 3):
“123”
“132”
“213”
“231”
“312”
“321”
Given n and k, return the kth permutation sequence.
Note: Given n will be between 1 and 9 inclusive.
## 思路
这道题目，花了我不少时间，倒不是因为题目很难，主要是因为这道题目与前面两篇求全排列的题目十分类似：[Permutations](http://blog.csdn.net/u010665216/article/details/78016334)、[Permutations II](http://blog.csdn.net/u010665216/article/details/78016461)；于是乎，我直接把上面两个博客中的dfs方法，迁移过来，最后再排序，求第k个序列。代码如下：
本来以为很简单直接就完成了，谁知道，竟然**time limit**了，后来我仔细观察了题目及数据结构；发现首先n个数默认有序，其次要求第k个序列，序列数总和是n!，那么我们是不是可以这样想，为方便说明，举例如下：
> n=3,k=3

> 序列总数为3*2*1=6，维护string s =”” 初始序列1，2，3，，，n

> step 1：首先将序列分成3组，以1，2，3开头的序列，各两个

> step 2：判断k落在第几组：落在第二组，s添加字符‘2’

> step 3：初始序列删除2，还剩余1，3；此时k在第二组的第一位，k=1，n=2;

> step 4：1，3序列总数为2*1=2；将序列分成两组，以1，3开头的序列，各一个

> step 5：判断k落在第几组

> …

> step n 不断递归判断 n==0就返回

> “123”

> “132”

> “213”

> “231”

> “312”

> “321”

> 感觉解释起来还是有些说不清楚，直接上代码！

> 代码

> class
> Solution {
> public
> :
> string
> getPermutation(
> int
> n,
> int
> k) {
> vector
> <
> int
> >
> num;
> string
> s =
> ""
> ;
> //维护字符串变量
> for
> (
> int
> i =
> 1
> ;i<=n;i++)
            num.push_back(i);
> //建立初始序列
> find_str(n,k,s,num);
> //递归
> return
> s;
    }
> private
> :
> void
> find_str(
> int
> n,
> int
> k,
> string
> & s,
> vector
> <
> int
> >
> &num)
    {
> if
> (n==
> 0
> )
> //如果n==0，递归临界条件
> return
> ;
> int
> count =
> 1
> ;
> for
> (
> int
> i=n;i>=
> 1
> ;i--)
            count*=i; 
        count /= n;
> int
> begin = k /count;
> int
> reminder = k%count;
> if
> (reminder==
> 0
> )
            ;
> else
> begin++;
> //判断k落在第几组
> k = k-count*(begin-
> 1
> );
        s+=to_string(num[begin-
> 1
> ]);
> //添加k落在前面对应小组的开头字符x
> num.erase(num.begin()+begin-
> 1
> );
> //删除字符x
> find_str(n-
> 1
> ,k,s,num);
> //递归
> }
};

