# 剑指offer - 扑克牌顺子 - curryche的博客 - CSDN博客





2018年09月11日 17:32:15[curryche](https://me.csdn.net/whwan11)阅读数：42标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

扑克牌顺子 

时间限制：1秒 空间限制：32768K 热度指数：124824 

本题知识点： 字符串
题目描述 

LL今天心情特别好,因为他去买了一副扑克牌,发现里面居然有2个大王,2个小王(一副牌原本是54张^_^)…他随机从中抽出了5张牌,想测测自己的手气,看看能不能抽到顺子,如果抽到的话,他决定去买体育彩票,嘿嘿！！“红心A,黑桃3,小王,大王,方片5”,“Oh My God!”不是顺子…..LL不高兴了,他想了想,决定大\小 王可以看成任何数字,并且A看作1,J为11,Q为12,K为13。上面的5张牌就可以变成“1,2,3,4,5”(大小王分别看作2和4),“So Lucky!”。LL决定去买体育彩票啦。 现在,要求你使用这幅牌模拟上面的过程,然后告诉我们LL的运气如何， 如果牌能组成顺子就输出true，否则就输出false。为了方便起见,你可以认为大小王是0。

## 解法

代码

```cpp
class Solution {
public:
    bool IsContinuous(vector<int> numbers ) {
        if(numbers.size()==0)
            return false;


        quick_sort(&numbers,0,numbers.size()-1);

        int count0,count_d;
        count0=0;
        count_d=0;
        for(int i=0;i<numbers.size();i++)
        {
            if(numbers[i]==0)
            {
                count0++;
            }
            else
            {
                if(i>0&&numbers[i]==numbers[i-1])
                    return false;
                if(i>0&&numbers[i-1]!=0&&((numbers[i]-numbers[i-1])>1))
                    count_d+=(numbers[i]-numbers[i-1]-1);
            }
        }

        if(count_d>count0)
            return false;
        else
            return true;

    }

    void quick_sort(vector<int>* num,int start,int end)
    {
        if(start>=end)
            return;
        int i,j,pivot;
        pivot=(*num)[start];
        i=start;
        j=end;
        while(i<j)
        {
            while(i<j&&(*num)[j]>=pivot)
                j--;
            if(i<j)
                (*num)[i++]=(*num)[j];

            while(i<j&&(*num)[i]<=pivot)
                i++;
            if(i<j)
                (*num)[j--]=(*num)[i];
        }
        (*num)[i]=pivot;
        quick_sort(num,start,i-1);
        quick_sort(num,i+1,end);
    }
};
```





