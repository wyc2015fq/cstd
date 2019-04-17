# 剑指offer 面试题：二叉搜索树的后序遍历序列 - 别说话写代码的博客 - CSDN博客





2019年03月03日 14:52:52[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：7标签：[二叉搜索树的后序遍历序列																[剑指offer](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)](https://so.csdn.net/so/search/s.do?q=二叉搜索树的后序遍历序列&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)








题目：输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历的结果。如果是则输出Yes,否则输出No。假设输入的数组的任意两个数字都互不相同。

思路：二叉 搜索树的中序遍历序列是有序的。也就是根节点值大于左子节点值，小于右子节点值。那其后序遍历序列是先遍历左子节点，再遍历 右子节点。那就是先拿出来根节点（后序遍历最后一个元素），从前往后找如果元素都比根小，那就是左子树 的节点，如果接下来的都比 根大，就是右子树。 如果不满足以上情况那序列 就不是二叉搜索树的后序遍历序列。

```cpp
class Solution {
public:
    bool bst(vector<int> &vec,int left,int right)
    {
        if(vec.empty() || left>right) return false;
        int i=0,rval=vec[right];
        for(i=left;i<right;++i)
            if(vec[i]>rval) break;
        for(int j=i;j<right;++j)
            if(vec[j]<rval) return false;  //这种情况就说名不满足，可以直接返回
        bool l=true,r=true;
        if(i>left) l=bst(vec,left,i-1);
        if(i<right) r=bst(vec,i,right-1);
        return l&&r;
    }
    bool VerifySquenceOfBST(vector<int> sequence) {
        return bst(sequence,0,sequence.size()-1);
    }
};
```






