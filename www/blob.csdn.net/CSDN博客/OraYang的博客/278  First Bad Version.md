
# 278. First Bad Version - OraYang的博客 - CSDN博客

2017年08月04日 10:22:30[OraYang](https://me.csdn.net/u010665216)阅读数：141所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
You are a product manager and currently leading a team to develop a new product. Unfortunately, the latest version of your product fails the quality check. Since each version is developed based on the previous version, all the versions after a bad version are
 also bad.
Suppose you have`n`versions`[1,
 2, ..., n]`and you want to find out the first bad one, which causes all the following ones to be bad.
You are given an API`bool isBadVersion(version)`which will return whether`version`is
 bad. Implement a function to find the first bad version. You should minimize the number of calls to the API.
思路：
本题使用二分法查找，二分法详情请戳：[binary search](https://en.wikipedia.org/wiki/Binary_search_algorithm)
代码：

```python
// Forward declaration of isBadVersion API.
bool isBadVersion(int version);
class Solution {
public:
    int firstBadVersion(int n) {
        int lower = 1,upper = n,mid;
        while(lower<upper){
            mid = lower + (upper - lower) /2;
            if(!isBadVersion(mid))
                lower = mid + 1;
            else
                upper = mid;
        }
            return lower;
    }
};
```


