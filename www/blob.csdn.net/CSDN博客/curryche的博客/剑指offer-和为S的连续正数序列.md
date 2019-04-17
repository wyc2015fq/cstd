# 剑指offer - 和为S的连续正数序列 - curryche的博客 - CSDN博客





2018年09月10日 21:29:21[curryche](https://me.csdn.net/whwan11)阅读数：32标签：[剑指offer																[c++](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

和为S的连续正数序列 

时间限制：1秒 空间限制：32768K 热度指数：155271

题目描述 

小明很喜欢数学,有一天他在做数学作业时,要求计算出9~16的和,他马上就写出了正确答案是100。但是他并不满足于此,他在想究竟有多少种连续的正数序列的和为100(至少包括两个数)。没多久,他就得到另一组连续正数和为100的序列:18,19,20,21,22。现在把问题交给你,你能不能也很快的找出所有和为S的连续正数序列? Good Luck! 

输出描述: 

输出所有和为S的连续正数序列。序列内按照从小至大的顺序，序列间按照开始数字从小到大的顺序
## 解法

代码

```cpp
class Solution {
public:
    vector<vector<int> > FindContinuousSequence(int sum) {
        vector<vector<int>> result;
        if(sum<3)
            return result;

        vector<int> current_list={0};
        int start,current_sum;
        start=1;
        current_sum=1;
        for(int i=2;i<=(sum/2+1);i++)
        {
            current_sum+=i;
            if(current_sum==sum)
            {
                current_list.clear();
                for(int j=start;j<=i;j++)
                    current_list.push_back(j);
                result.push_back(current_list);
            }
            if(current_sum>sum)
            {
                while(current_sum>sum&&start<i)
                {
                    current_sum-=start;
                    start++;
                }
                if(current_sum==sum)
                {
                    current_list.clear();
                    for(int j=start;j<=i;j++)
                        current_list.push_back(j);
                    result.push_back(current_list);
                }
            }


        }

        return result;
    }
};
```





