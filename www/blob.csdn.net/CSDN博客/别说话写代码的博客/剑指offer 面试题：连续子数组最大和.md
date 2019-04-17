# 剑指offer 面试题：连续子数组最大和 - 别说话写代码的博客 - CSDN博客





2019年03月09日 11:03:29[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：17标签：[剑指offer																[连续子数组最大和](https://so.csdn.net/so/search/s.do?q=连续子数组最大和&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：HZ偶尔会拿些专业问题来忽悠那些非计算机专业的同学。今天测试组开完会后,他又发话了:在古老的一维模式识别中,常常需要计算连续子向量的最大和,当向量全为正数的时候,问题很好解决。但是,如果向量中包含负数,是否应该包含某个负数,并期望旁边的正数会弥补它呢？例如:{6,-3,-2,7,-15,1,2,2},连续子向量的最大和为8(从第0个开始,到第3个为止)。给一个数组，返回它的最大连续子序列的和，你会不会被他忽悠住？(子向量的长度至少是1)

思路：遍历一遍，思路是dp。

```cpp
class Solution {
public:
    int FindGreatestSumOfSubArray(vector<int> array) {
        if(array.empty()) return 0;
        int res =INT_MIN,cur=0;
        for(int i=0;i<array.size();++i)
        {
            if(cur>=0) cur+=array[i];
            if(cur<0) cur=array[i];
            if(cur>res) res=cur;
        }return res;
    }
};
```

```cpp
class Solution {
public:
    int FindGreatestSumOfSubArray(vector<int> array) {
        if(array.empty()) return 0;
        int res=array[0],mx=array[0];
        for(int i=1;i<array.size();++i)
        {
            mx = max(array[i],mx+array[i]);
            res = max(res,mx);
        }return res;
    }
};
```






