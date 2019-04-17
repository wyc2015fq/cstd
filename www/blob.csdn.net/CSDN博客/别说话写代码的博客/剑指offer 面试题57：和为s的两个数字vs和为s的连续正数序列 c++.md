# 剑指offer 面试题57：和为s的两个数字vs和为s的连续正数序列 c++ - 别说话写代码的博客 - CSDN博客





2018年11月21日 10:12:43[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：36
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：输入一个递增排序的数组和一个数字S，在数组中查找两个数，使得他们的和正好是S，如果有多对数字的和等于S，输出两个数的乘积最小的。

思路：两个 指针，start从前往后遍历，end从后往前遍历，如果当前前后之和大于s,end--,如果当前前后之和小于s，start++，这样能找出乘积最小的和为s的两个数

```cpp
class Solution {
public:
    vector<int> FindNumbersWithSum(vector<int> array,int sum) {
        if(array.size()==0) return array;
        int start=0;
        int end=array.size()-1;
        int allsum = 0;
        vector<int> ret;
        while(start <= end)
        {
            allsum=array[start]+array[end];
            if(allsum==sum)
            {
                ret.push_back(array[start]);
                ret.push_back(array[end]);
                break;
            }else if(allsum>sum) end--;
            else start++;
        }return ret;
    }
};
```

题目：小明很喜欢数学,有一天他在做数学作业时,要求计算出9~16的和,他马上就写出了正确答案是100。但是他并不满足于此,他在想究竟有多少种连续的正数序列的和为100(至少包括两个数)。没多久,他就得到另一组连续正数和为100的序列:18,19,20,21,22。现在把问题交给你,你能不能也很快的找出所有和为S的连续正数序列? Good Luck!

思路：两个指针，start和end，因为是连续子序列，我们可以用等差数列求和公式(start+end)*(end-start+1)/2来算出当前子序列和。若等于s，则将其序列拿出来，若大于s，说明当前序列多了，start++，若小于s，说明当前序列不够，end++

```cpp
class Solution {
public:
    vector<vector<int> > FindContinuousSequence(int sum) {
        vector<vector<int>> ret;
        int start = 1,end=2;
        while(start<end && end<sum)
        {
            int cur = (start+end)*(end-start+1)/2; //使用等差数列公式计算连续串和
            if(cur < sum) end++;
            else if(cur > sum) start++;
            else if(cur==sum)
            {
                vector<int> oneseq;
                for(int i=start;i<=end;++i)
                    oneseq.push_back(i);
                ret.push_back(oneseq);
                start++;
            }
        }return ret;
    }
};
```





