# Leetcode——338. Bit位计数 - westbrook1998的博客 - CSDN博客





2018年04月30日 22:32:01[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：667








很不错的一道题，要求O(n)即一次遍历得到0-num这些数的二进制1个数 

代码

```cpp
class Solution {
public:
    vector<int> countBits(int num) {
        vector<int> res(num+1,0);
        for(int i=1;i<=num;i++){
            res[i]=res[i&(i-1)]+1;
        }
        return res;
    }
};
```

分析： 

首先是一个数减1，对应二进制的变化就是最右的一个1变为0，而这个1右边的所有0变为1，**即相当于包括最后一个1在内的右边所有位取反，**例如12（1100）减1，得到11（1011），然后再与变化前的数12（1100）进行与`&`运算，得到8（1000），可以看出经过这样一个运算之后这个数的1的个数减少了一个，所以可以利用这个原理，得到`res[i]=res[i&(i-1)]+1`



