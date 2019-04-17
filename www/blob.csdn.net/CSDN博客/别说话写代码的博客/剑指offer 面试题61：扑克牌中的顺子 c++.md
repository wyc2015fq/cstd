# 剑指offer 面试题61：扑克牌中的顺子 c++ - 别说话写代码的博客 - CSDN博客





2018年11月25日 14:44:36[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：125
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：从扑克牌中随机抽5张牌，判断是不是一个顺子，即这5张牌是不是连续的。2~10为数字本身，A为1，J为11，Q为12，K为13，而 大小王可以看成 任意数字

思路：以大小王为0,0可以代替任何数字。先对序列进行排序，然后算数组中0的个数，计算数组空缺 总数，如果空缺总数大于0的个数，不是顺子，否则是顺子。注意如果5张牌发现有 对子，肯定不是顺子

```cpp
class Solution {
public:
    bool IsContinuous( vector<int> numbers ) {
        if(numbers.empty()) return false;
        sort(numbers.begin(),numbers.end());
        int n=numbers.size(),numzero=0,gap=0;
        for(int i=0;i<n && numbers[i]==0 ;++i)  //统计有多少0
            numzero++;
        for(int i=numzero,j=numzero+1;j<n;++i,++j)   //计算他们之间的差值
        {
            if(numbers[i]==numbers[j]) return false;
            gap += numbers[j]-numbers[i]-1;
        
        }
        return numzero>=gap ? true : false;    //如果0的个数大于差值的个数返回true
    }
};
```





