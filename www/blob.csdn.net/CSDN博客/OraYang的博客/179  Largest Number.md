
# 179. Largest Number - OraYang的博客 - CSDN博客

2018年03月02日 09:59:50[OraYang](https://me.csdn.net/u010665216)阅读数：176所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



# 题目
Given a list of non negative integers, arrange them such that they form the largest number.
For example, given`[3, 30, 34, 5, 9]`, the largest formed number is`9534330`.
Note: The result may be very large, so you need to return a string instead of an integer.

# 思路
这道题目是个典型的字符串排序问题。在算法中，关于字符串排序，所有教材都会强调一个词叫做“稳定性”。
> 如果一个排序算法能够保留数组中重复元素的相对位置则可以被称为是稳定的。
在我们学习的排序算法中，有一些算法是稳定的（插入排序和归并排序），但是很多不是（选择排序、希尔排序、快速排序和堆排序）
本题方案实现的思路是：
> 将整数数组转换成字符串数组

> [利用java Arrays.sort(）函数](https://docs.oracle.com/javase/7/docs/api/java/util/Arrays.html#sort%28T%5B%5D,%20java.util.Comparator%29)
> 对字符串排序

> public static <T> void sort(T[] a,Comparator<? super T> c)

> This sort is guaranteed to be stable: equal elements will not be reordered as a result of the sort.

> 这里我们需要自定义比较器

> 两两字符串之间如何定义大小呢？

> 我们发现：

> String
> s1 =
> "9"
> ;
> String
> s2 =
> "31"
> ;
> String
> case1 =  s1 + s2;
> // 931
> String
> case2 = s2 + s1;
> // 319
> s1与s2之间的大小关系是由case1与case2之间的大小关系决定的

> 因此实现方法如下：

> class Solution {
> private
> static
> int
> R =
> 10
> ;
> private
> static
> String[] aux;
> public
> String
> largestNumber
> (
> int
> [] nums) {
> //insertion sort
> int
> N = nums.length;
        String[] a =
> new
> String[N];
        aux =
> new
> String[N];
> //convert int to String
> for
> (
> int
> i=
> 0
> ;i<N;i++)
            a[i] = Integer.toString(nums[i]);
> //override Comparator
> Comparator<String> comp =
> new
> Comparator<String>(){
> @Override
> public
> int
> compare
> (String v,String w)
        {
            String vw = v+w;
            String wv = w+v;
> return
> vw.compareTo(wv);
        }
    };
        Arrays.sort(a,comp);
> if
> (a[N-
> 1
> ].charAt(
> 0
> )==
> '0'
> )
> return
> "0"
> ;
        StringBuilder res=
> new
> StringBuilder();
> for
> (
> int
> i=N-
> 1
> ;i>=
> 0
> ;i--)
            res.append(a[i]);
> return
> res.toString();
    }
}

