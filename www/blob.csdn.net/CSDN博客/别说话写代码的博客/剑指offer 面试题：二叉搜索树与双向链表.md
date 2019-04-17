# 剑指offer 面试题：二叉搜索树与双向链表 - 别说话写代码的博客 - CSDN博客





2019年03月04日 13:03:22[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：18
所属专栏：[剑指offer刷题c++/python](https://blog.csdn.net/column/details/29881.html)









题目：输入一棵二叉搜索树，将该二叉搜索树转换成一个排序的双向链表。要求不能创建任何新的结点，只能调整树中结点指针的指向。

思路。题目要求就是将left变为前向指针，right变为后向 指针。那就用递归方法来做，二叉搜索树中序有序，那就 用一个last指针指向根节点左子树最右边 节点，然后将last和根节点连接起来，然后将last 指向根节点，然后遍历右子树。

```cpp
/*
struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
	TreeNode(int x) :
			val(x), left(NULL), right(NULL) {
	}
};*/
class Solution {
public:
    void convert(TreeNode* root,TreeNode **last)
    {
        if(!root) return;
        if(root->left) convert(root->left,last);  //先遍历左子树
        TreeNode *cur=root;
        
        cur->left = *last;            //找到两个相邻节点，互相指向
        if(*last) (*last)->right = cur;
        
        *last = cur;         
        if(cur->right) convert(cur->right,last);   //最后遍历右子树
    }
    TreeNode* Convert(TreeNode* pRootOfTree)
    {
        TreeNode *last = nullptr;   //指向链表最后一个
        convert(pRootOfTree,&last);   //将树转为双向链表
        TreeNode *head = last; 
        while(head && head->left)    //从后往前找头结点
            head = head->left;
        return head;
    }
};
```





