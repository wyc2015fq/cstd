# 剑指offer 面试题43：整数中1出现的次数（1~n整数中1出现的次数） - 别说话写代码的博客 - CSDN博客





2018年11月23日 14:32:08[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：41
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：求出1~13的整数中1出现的次数,并算出100~1300的整数中1出现的次数？为此他特别数了一下1~13中包含1的数字有1、10、11、12、13因此共出现6次,但是对于后面问题他就没辙了。ACMer希望你们帮帮他,并把问题更加普遍化,可以很快的求出任意非负整数区间中1出现的次数（从1 到 n 中1出现的次数）。

思路：参考[https://blog.csdn.net/yi_afly/article/details/52012593](https://blog.csdn.net/yi_afly/article/details/52012593)

```cpp
class Solution {
public:
    int NumberOf1Between1AndN_Solution(int n)
    {
        if(n<1) return 0;
        int count=0,base=1,round=n,weight;
        while(round)
        {
            weight = round%10;
            round/=10;
            count += round*base;
            if(weight == 1) count+=(n%base)+1;
            else if(weight >1) count += base;
            base *=10;
        }return count;
    }
};
```





