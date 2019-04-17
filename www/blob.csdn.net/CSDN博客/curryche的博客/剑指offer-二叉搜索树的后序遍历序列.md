# 剑指offer - 二叉搜索树的后序遍历序列 - curryche的博客 - CSDN博客





2018年08月23日 11:22:32[curryche](https://me.csdn.net/whwan11)阅读数：46








## 题目

二叉搜索树的后序遍历序列 

时间限制：1秒 空间限制：32768K 热度指数：259046

题目描述 

输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历的结果。如果是则输出Yes,否则输出No。假设输入的数组的任意两个数字都互不相同。

## 解法

代码

```cpp
class Solution {
public:
    bool VerifySquenceOfBST(vector<int> sequence) {
        if(sequence.size()==0)
            return false;

        if(sequence.size()==1)
            return true;

        int i,j;
        i=0;
        while(i<sequence.size()-1)
        {
            if(sequence[i]<sequence.back())
                i++;
            else
                break;
        }

        for(j=i;j<sequence.size()-1;j++)
        {
            if(sequence[j]<sequence.back())
                return false;
        }

        if(i==0||i==sequence.size()-1)
        { 
            vector<int> single_side;
            single_side.assign(sequence.begin(), sequence.begin()+sequence.size()-1);
            return VerifySquenceOfBST(single_side);
        }
        else
        {
            vector<int> left,right;
            left.assign(sequence.begin(), sequence.begin()+i);
            right.assign(sequence.begin()+i, sequence.begin()+sequence.size()-1);
            return VerifySquenceOfBST(left)&&VerifySquenceOfBST(right);
        }

    }
};
```



