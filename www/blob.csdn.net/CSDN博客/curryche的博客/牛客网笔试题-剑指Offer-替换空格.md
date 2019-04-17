# 牛客网笔试题 - 剑指Offer - 替换空格 - curryche的博客 - CSDN博客





2018年07月29日 21:45:28[curryche](https://me.csdn.net/whwan11)阅读数：66








## 题目

时间限制：1秒 空间限制：32768K 热度指数：661351 

本题知识点： 字符串

题目描述 

请实现一个函数，将一个字符串中的每个空格替换成“%20”。例如，当字符串为We Are Happy.则经过替换之后的字符串为We%20Are%20Happy。

## 解法

思路： 

1. 先遍历字符串，找到空格字符，并替换为%，统计空格字符个数count。 

2. 从现有字符串最后一个有效字符倒序处理，用end_pos标记未处理字符串的最后一个位置，碰到%字符时，将当前%与end_pos之间的字符串向后整体搬移2*count个位置，然后在前面添加“20”，再将%搬移到“20”的前面。完成操作之后count要减1。
代码

```
class Solution {
public:

    void replaceSpace(char *str,int length) {
        int i,j,count,end_pos;
        count=0;
        for(i=0;i<length;i++)
        {
            if(str[i]==' ')
            {
                str[i]='%';
                count++;
            }
        }

        i=length-1;
        end_pos=length-1;
        while(i>=0)
        {
            if(str[i]=='%')
            {
                for(j=end_pos;j>i;j--)
                {
                    str[j+count*2]=str[j];
                }
                str[j+count*2]='0';
                j--;
                str[j+count*2]='2';
                j--;
                str[j+count*2]='%';

                count--;
                i--;
                end_pos=i;
            }
            else
            {
                i--;
            }
        }
    }
};
```




