# kSum问题的总结 - bigfacesafdasgfewgf - CSDN博客





2015年05月07日 11:11:07[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1418











    kSum问题是一类题型，常见的有2Sum，3Sum，4Sum等。这篇博文就来总结一些kSum问题的解法，以及对应的时间复杂度。




#     1. 2Sum

    在一个给定的数组nums中，寻找和为定值target的两个数。

    【解法1】：把数组排序，然后使用two pointers的方法来求解。时间复杂度分析：排序O(nlogn)，两指针遍历O(n)，总体O(nlogn).

    【解法2】：先遍历一遍，把整个数组存入到hash_map中，key是每个数，value是出现的次数。然后再遍历一次，每次取查找hasp_map中是否有target-nums[i]。建立hash_map的时间复杂度是O(n)，第二次遍历时 每个查找为O(1)，所以查找全部也是O（n），总体为O(n)。注意：如果数组中有重复的数，比如{1,
 5, 3, 3}， target=6，那么我们在查找的时候就要考虑3的情况，正是因为数组中有两个3，我们才能返回，此时key=3是的value=2就起到了作用。这就是为什么我们前面要记录value的原因。




    总的来说，2Sum的最好时间复杂度是O(n)。



    代码：http://blog.csdn.net/puqutogether/article/details/41775343




#     2. 3Sum

    在一个给定的数组nums中，寻找和为定值target的三个数。

    【解法1】：固定其中的一个数a，问题便转换为寻找2Sum，新的target为target-a。时间复杂度为O(nlogn + n^2)=O(n^2)。

    【解法2】：依然是固定其中一个数，之后使用hash_map的做法，时间复杂度还是O(n*n)=O(n^2)。



    总体的最好的时间复杂度是O(n^2)。




#     3. 4Sum

在一个给定的数组nums中，寻找和为定值target的三个数。

    【解法1】：使用不断固定的思想，最后转化成2Sum问题，时间复杂度为O(n^3).

    【解法2】：既然前面的hash_map的思路展示出优势，那么我们就直接使用hash_map来解决。

                      首先，遍历所有可能的pair，把所有的pair存在hash_map中，key=a+b,
 value是一个list，每个元素为(a, b)。时间复杂度为O(n^2)，为啥是pair呢？因为后面要查找的是pair。

                      然后，两个循环for遍历所有的可能值c和d，O(n^2)，每种情况下都需要查找target-c-d，此时，这个差值就是我们hash_map中的key，如果发现有pair满足要求即返回。

                      时间复杂度为O(n^2*logn)，因为list的查找至少要O(logn)。所以，解法2的总体时间复杂度为O(n^2logn)。




#     4. kSum

    关于kSum问题，我们一样可以使用上述解法1的思路，这样的时间复杂度为O(n^(k-1))。也可以使用hash_map的思路，这样子会更快。具体的时间复杂度分析较复杂，参考如下：





-SUM
 can be solved more quickly as follows.
- 
For even :Compute
 a sorted list  of
 all sums of  input
 elements. Check whether contains
 both some number  and
 its negation .
 The algorithm runs in  time.

- 
For odd : Compute
 the sorted list  of
 all sums of  input
 elements. For each input element ,
 check whether  contains
 both  and ,
 for some number .
 (The second step is essentially the -time
 algorithm for 3SUM.) The algorithm runs in  time.



Both algorithms are optimal (except possibly for the log factor when  is
 even and bigger than )
 for any constant  in
 a certain weak but natural restriction of the linear decision tree model of computation. For more details, see:
- 

Nir Ailon and Bernard Chazelle. [Lower bounds for linear degeneracy testing](http://www.cs.princeton.edu/~chazelle/pubs/lindegen-jacm.pdf). JACM 2005.

- 

Jeff Erickson. [Lower bounds for linear satisfiability problems](http://compgeom.cs.uiuc.edu/~jeffe/pubs/linsat.html). CJTCS 1999.




参考链接：

http://cs.stackexchange.com/questions/2973/generalised-3sum-k-sum-problem.


http://blog.csdn.net/doc_sgl/article/details/12462151










