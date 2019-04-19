# Maximum Product Subarray -- LeetCode - 数据之美的博客 - CSDN博客
2017年08月23日 22:15:45[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：125
个人分类：[数据结构																[算法](https://blog.csdn.net/oppo62258801/article/category/6551325)](https://blog.csdn.net/oppo62258801/article/category/6551324)

版权声明：本文为博主原创文章，未经博主允许不得转载。
原题链接: [https://oj.leetcode.com/problems/maximum-product-subarray/](https://leetcode.com/problems/maximum-product-subarray/)
这道题跟[Maximum
 Subarray](http://blog.csdn.net/linhuanmars/article/details/21314059)模型上和思路上都比较类似，还是用一维动态规划中的“局部最优和全局最优法”。这里的区别是维护一个局部最优不足以求得后面的全局最优，这是由于乘法的性质不像加法那样，累加结果只要是正的一定是递增，乘法中有可能现在看起来小的一个负数，后面跟另一个负数相乘就会得到最大的乘积。不过事实上也没有麻烦很多，我们只需要在维护一个局部最大的同时，在维护一个局部最小，这样如果下一个元素遇到负数时，就有可能与这个最小相乘得到当前最大的乘积和，这也是利用乘法的性质得到的。代码如下： 
**[java]**[view
 plain](http://blog.csdn.net/linhuanmars/article/details/39537283#)[copy](http://blog.csdn.net/linhuanmars/article/details/39537283#)
- publicint maxProduct(int[] A) {  
- if(A==null || A.length==0)  
- return0;  
- if(A.length == 1)  
- return A[0];  
- int max_local = A[0];  
- int min_local = A[0];  
- int global = A[0];  
- for(int i=1;i<A.length;i++)  
-     {  
- int max_copy = max_local;  
-         max_local = Math.max(Math.max(A[i]*max_local, A[i]), A[i]*min_local);  
-         min_local = Math.min(Math.min(A[i]*max_copy, A[i]), A[i]*min_local);  
-         global = Math.max(global, max_local);  
-     }  
- return global;  
- }  
这道题是一道很不错的面试题目，因为[Maximum
 Subarray](http://blog.csdn.net/linhuanmars/article/details/21314059)这道题过于常见了，所以可能大部分人都做过，这道题模型类似，但是又有一些新的考点，而且总体还是比较简单，无论是思路上还是实现上，又能考察动态规划，个人还是比较喜欢的哈。
