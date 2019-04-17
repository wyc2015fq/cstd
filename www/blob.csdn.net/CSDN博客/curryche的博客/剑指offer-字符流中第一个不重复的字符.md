# 剑指offer - 字符流中第一个不重复的字符 - curryche的博客 - CSDN博客





2018年09月04日 22:07:49[curryche](https://me.csdn.net/whwan11)阅读数：37








## 题目

字符流中第一个不重复的字符 

时间限制：1秒 空间限制：32768K 热度指数：83662 

本题知识点： 字符串
题目描述 

请实现一个函数用来找出字符流中第一个只出现一次的字符。例如，当从字符流中只读出前两个字符”go”时，第一个只出现一次的字符是”g”。当从该字符流中读出前六个字符“google”时，第一个只出现一次的字符是”l”。 

输出描述: 

如果当前字符流没有存在出现一次的字符，返回#字符。
## 解法

代码

```
class Solution
{
public:
    Solution():label(0)
    {
        for(int i=0;i<256;i++)
            first_pos[i]=-1;
    }
  //Insert one char from stringstream
    void Insert(char ch)
    {
        if(first_pos[ch]==-1)
            first_pos[ch]=label;
        else if(first_pos[ch]>-1)
            first_pos[ch]=-2;

        label++;

    }
  //return the first appearence once char in current stringstream
    char FirstAppearingOnce()
    {
        int min_pos=label+1;
        char target_char='#';
        for(int i=0;i<256;i++)
        {
            //cout<<char(i)<<" "<<first_pos[i]<<endl;
            if(first_pos[i]>=0&&first_pos[i]<min_pos)
            {
                min_pos=first_pos[i];
                target_char=i;
            }
        }
        return target_char;
    }

private:
    int first_pos[256];
    int label;

};
```





