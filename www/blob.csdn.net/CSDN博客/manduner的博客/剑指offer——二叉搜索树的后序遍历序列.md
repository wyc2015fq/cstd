
# 剑指offer——二叉搜索树的后序遍历序列 - manduner的博客 - CSDN博客


2019年03月29日 21:38:10[Manduner_TJU](https://me.csdn.net/manduner)阅读数：16标签：[java																](https://so.csdn.net/so/search/s.do?q=java&t=blog)[剑指offer																](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=java&t=blog)个人分类：[Java学习笔记																](https://blog.csdn.net/manduner/article/category/7486695)



# 1，问题的描述
输入一个整数数组，判断该数组是不是某二叉搜索树的后序遍历的结果。如果是则输出Yes,否则输出No。假设输入的数组的任意两个数字都互不相同。
# 2，解题思路
二叉树后序遍历序列的最后一个值是根节点的值，**二叉搜索树后序遍历序列的合法性条件是**：先根据根节点的值将后序遍历序列分为左右两个序列，左边序列所有元素应该都小于根节点的值，右边序列所有元素应该都大于根节点的值。左右两个序列同样应该是二叉搜索树的合法后序遍历序列，那么就用递归的方法去判断就行了。注：一定要注意递归的出口，在源码中有注释。
# 3，源码
```python
public class Solution {
    public boolean VerifySquenceOfBST(int [] sequence) {
        if(sequence.length==0) return false;//正常情况下空树是二叉搜索树，但是此题认定空树不是二叉搜索树
        return judge(sequence,0,sequence.length-1);
    }
    public boolean judge(int[] sequence, int l, int r) {
        if(l>=r) return true;//l>r表示空树，l==r表示叶子节点
        int i=r;
        while(i>l && sequence[i-1]>sequence[r]) i--;//i的最终值是右序列的起始位置
        //for循环判断左序列是否包含大于根节点值的节点，若有，说明不满足二叉查找树的合法性。
        for(int j=l;j<i-1;j++) {
            if(sequence[j]>sequence[r]) return false;
        }
        return judge(sequence,l,i-1)&&judge(sequence,i,r-1);
    }
}
```


