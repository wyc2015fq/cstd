# 剑指offer  面试题50：字符流中第一个只出现一次的字符 - 别说话写代码的博客 - CSDN博客





2018年11月15日 18:10:42[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：83
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目描述：请实现一个函数用来找出字符流中第一个只出现一次的字符。例如，当从字符流中只读出前两个字符"go"时，第一个只出现一次的字符是"g"。当从该字符流中读出前六个字符“google"时，第一个只出现一次的字符是"l"。

解法： 使用哈希表occurance[256] ;  初始化occurance为-1，当遇到某个字符并且字符的occurance[ch]==-1时候，将其置为字符出现的位置index，当遇到某个字符并且字符的occurance[ch]>=0时候，说明其出现过，将occurance置为-2.最终只需要找出最小的occurance[i]即可，就是找到的第一个 只出现一次的字符

```cpp
class Solution
{
public:
  //Insert one char from stringstream
    Solution():index(0)
    {
        for(int i=0;i<256;i++)
            occurance[i] = -1;
    }
    void Insert(char ch)
    {
         if(occurance[ch]== -1) occurance[ch] = index;
         else if(occurance[ch] >= 0 ) occurance[ch] =-2;
         index ++;
    }
  //return the first appearence once char in current stringstream
    char FirstAppearingOnce()
    {
        char ch = '#';
        int min = INT_MAX;
        for(int i=0;i<256;++i)
        {
            if(min>occurance[i] && occurance[i] >=0)
            {
                ch = (char)i;
                min = occurance[i];
            }
        }
        return ch;
    }
private:
    int index;
    int occurance[256];
};
```





