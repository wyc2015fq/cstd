# [算法] - 二叉树（C++） - Snoopy_Dream - CSDN博客





2019年03月22日 10:55:46[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：23








自己动手写代码，记录中间出现的错误。

**目录**

[[98] 验证二叉搜索树](#main-toc)

[二叉树的层高](#%E4%BA%8C%E5%8F%89%E6%A0%91%E7%9A%84%E5%B1%82%E9%AB%98)

[二叉树的左右边界打印](#%E4%BA%8C%E5%8F%89%E6%A0%91%E7%9A%84%E5%B7%A6%E5%8F%B3%E8%BE%B9%E7%95%8C%E6%89%93%E5%8D%B0)

[[94] 二叉树的中序遍历](#*%20%5B94%5D%20%E4%BA%8C%E5%8F%89%E6%A0%91%E7%9A%84%E4%B8%AD%E5%BA%8F%E9%81%8D%E5%8E%86)

[[96] 不同的二叉搜索树](#%5B96%5D%20%E4%B8%8D%E5%90%8C%E7%9A%84%E4%BA%8C%E5%8F%89%E6%90%9C%E7%B4%A2%E6%A0%91)

[[112] 路径总和](#%5B112%5D%20%E8%B7%AF%E5%BE%84%E6%80%BB%E5%92%8C)

### [98] 验证二叉搜索树



```cpp
/*
 * 
 * 示例 1:
 * 
 * 输入:
 * ⁠   2
 * ⁠  / \
 * ⁠ 1   3
 * 输出: true

 */
class Solution {
    double last = LONG_MIN;//temp
public:
    bool isValidBST(TreeNode* root) {
      if (root == nullptr) {
            return true;
        }
        if (isValidBST(root->left)) {//zuo
            if (last < root->val) {//如果左节点满足，
                last = root->val;
                return isValidBST(root->right);
            }
        }
        return false;
    }
};

class Solution {
public:
    bool isValidBST(TreeNode* root) {
        // 注意 用long的最大最小值
        return check(root, LONG_MIN, LONG_MAX);
    }
    bool check(TreeNode* root, long min, long max){
        if (root == NULL)
            return true;
        if (root->val <= min || root->val >= max)
            return false;
        return check(root->left, min, root->val) && check(root->right, root->val, max);
    }

}
```

### 二叉树的层高



```cpp
站到头结点，和左树右树，要信息
int getHeight(treeNode * head,int level)
{
    //传入头结点，level为0，返回的level，就是层高
    if (head == nullptr) return level;
    left_height =  getHeight(head->left,level+1);//递归的状态，只应用于本次循环，所以每一对leftright都是一对儿。
    right_height=  getHeight(head->right,level+1);//右树高度
    return max(left_height,right_height);

}

int getHeight(treeNode * head)
{
    if (head == nullptr) return 0;
    left_height =  getHeight(head->left);//
    right_height=  getHeight(head->right);
    return max(left_height,right_height)+1;

}

// 非递归，bfs通过队列size得到本层的节点数目
int TreeDepth(TreeNode* pRoot)
    {
     queue<TreeNode*> q;
        if(!pRoot) return 0;
        q.push(pRoot);
        int level=0;
        while(!q.empty()){
            int len=q.size();
            level++;
            while(len--){
                TreeNode* tem=q.front();
                q.pop();
                if(tem->left) q.push(tem->left);
                if(tem->right) q.push(tem->right);
            }
        }
        return level;
    }
```

### 二叉树的左右边界打印

辅助数据：一个二维数组

**内部逻辑：**中序遍历，basecase考虑某一节点**，if 什么时候是左边界，if 什么时候是右边界，然后遍历左右**
- 如果是第一次遍历到这一层的edgemap[l][0]==nullptr;那么它就是左边界，edgemap[l][0]=cur
- 如果edgemap[l][1]，把最后一次遍历的cur给它就是右边界，所以不用加判断，一直edgemap[l][1]=cur，就好，循环到最后就是

凡是和层高有关系的，递归函数的形参里面都有level
-     setEdgeMap(h->left, l + 1);
-     setEdgeMap(h->right, l + 1);


- int getHeight(treeNode * head,int level)
- {
-     //传入头结点，level为0，返回的level，就是层高
-     if (head == nullptr) return level;
-     left_height =  getHeight(head->left,level+1);//递归的状态，只应用于本次循环，所以每一对leftright都是一对儿。
-     right_height=  getHeight(head->right,level+1);//右树高度
-     return max(left_height,right_height);
- }

```cpp
#include "stdafx.h"
#include"iostream"
using namespace std;
struct Node {
	Node* left;
	Node* right;
	int data;
	Node(int value):data(value), left(NULL), right(NULL){}
};
Node* T;
Node* edgeMap[100 + 5][2];

void setEdgeMap(Node* h, int l)
{
	// 根左右，中序遍历，先basecase再判断，根节点的此处的逻辑，然后左右遍历
	if (h == NULL) return;
    //左边界 如果是第一次遇到就令它等于左边界
	if (edgeMap[l][0] == NULL) 
		edgeMap[l][0] = h;
    //右边界 是这一层最后一个遍历到的，所以一直赋予就可以了，到最后就是最右边界
	edgeMap[l][1] = h;
	setEdgeMap(h->left, l + 1);
	setEdgeMap(h->right, l + 1);
	
}

int main()
{
	Node* node1 = new Node(1);
	Node*node2 = new Node(2);
	Node*node3 = new Node(3);
	Node*node4 = new Node(4);
	Node*node5 = new Node(5);
	Node*node6 = new Node(6);
	Node*node7 = new Node(7);
	Node*node8 = new Node(8);
	Node*node9 = new Node(9);
	Node*node10 = new Node(10);
	Node*node11 = new Node(11);
	Node*node12 = new Node(12);
	Node*node13 = new Node(13);
	Node*node14 = new Node(14);
	Node*node15 = new Node(15);
	Node*node16 = new Node(16);
	node1->left = node2;
	node1->right = node3;
	node2->right = node4;
	node3->left = node5;
	node3->right = node6;
	node4->left = node7;
	node4->right = node8;
	node5->left = node9;
	node5->right = node10;
	node8->right = node11;
	node9->left = node12;
	node11->left = node13;
	node11->right = node14;
	node12->left = node15;
	node12->right = node16;
	setEdgeMap(node1,0);
	for (int i = 0; i < 6; i++)
	{
		cout << edgeMap[i][0]->data<<" ";

	}
	cout << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << edgeMap[i][1]->data << " ";

	}
	cout << endl;

    return 0;
}
```







### [94] 二叉树的中序遍历

```cpp
class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) {     
        vector<int> res;
        inorder(root,res);
        return res;
    }
    void inorder(TreeNode * Node,vector<int> &res){
        if (Node==nullptr) return;
        inorder(Node->left,res);
        res.push_back(Node->val);
        inorder(Node->right,res);
    }
};

class Solution {
public:
    vector<int> inorderTraversal(TreeNode* root) { 
        TreeNode* cur = root;
        vector<int> res;
        stack<TreeNode *> helpStack;
        while(!helpStack.empty()||cur!=nullptr){//记住这里的条件
            if(cur!=nullptr){// while里面if就行
                helpStack.push(cur);
                cur = cur->left;
            }
            else{
                cur = helpStack.top();
                helpStack.pop();
                res.push_back(cur->val);//注意是->val
                cur = cur->right;// 弹出节点的右边！！
            }
        }
        return res;
    }
};
```

**Tricks：**
- 递归遍历是非常重要的，对于二叉树的题目来说，虽然单独让你写中序遍历的时候最好不要用递归，但是其他题目中有用到很多。inorder(Node->left,res);一直递归到最左边的null了，返回，此时压入最左边节点res.push_back(Node->val);然后看看有没有右边的节点，有的话带进去， inorder(Node->right,res);然后又回到了找最左的过程，以此循环。有一个回溯的过程。
- 递归遍历的过程是void函数，注意vector<int>**& res**,这里的&符号。
- 注意Node->left的时候，要想**if(node->left),这里虽然不用加，但也要有思想的过程。**



### **[96] 不同的二叉搜索树**

```cpp
/* 给定 n = 3, 一共有 5 种不同结构的二叉搜索树:
 * 
 * ⁠  1         3     3      2      1
 * ⁠   \       /     /      / \      \
 * ⁠    3     2     1      1   3      2
 * ⁠   /     /       \                 \
 * ⁠  2     1         2                 3 */
class Solution {// 总结规律，从遍历的过程。二叉搜索树的中序遍历是递增的。
public:
    int numTrees(int n) {     
        int f[n+1]={0};//注意一定要初始化！！！
        f[0]=1;
        for(int i=1;i<n+1;i++){
            for(int j=1;j<i+1;j++){
                f[i]=f[i]+f[j-1]*f[i-j];//这个就是下面那个的公式
            }
        }
        return f[n];
    }      
};
```

tricks:  
- **不写累加符号什么的，直接dp(n)=dp(0)*dp(n-1)+dp(1)*dp(n-2)+dp(2)*dp(n-3)+...+dp(n-1)*dp(0) ，好看。注意dp[0]=1.**
- **注意dp(n)=dp(0)*dp(n-1)+dp(1)*dp(n-2)+dp(2)*dp(n-3)+...+dp(n-1)*dp(0) 到dp[n] + = dp[j-1]*dp[n-j] 内遍历，j从1到n。**然后到**dp[i] + = dp[j-1]*dp[i-j]，外遍历**
- 注意**数组一定要初始化! **动态规划的题目，有几个变量就有几个for，里面一般就是if什么的。

### [112] 路径总和

```cpp
/*
 * 给定一个二叉树和一个目标和，判断该树中是否存在根节点到叶子节点的路径，这条路径上所有节点值相加等于目标和。

 * 
 * ⁠             5
 * ⁠            / \
 * ⁠           4   8
 * ⁠          /   / \
 * ⁠         11  13  4
 * ⁠        /  \      \
 * ⁠       7    2      1
 * 
 * 
 * 返回 true, 因为存在目标和为 22 的根节点到叶子节点的路径 5->4->11->2。
 * 
 */
class Solution {
public:
    bool hasPathSum(TreeNode* root, int sum) {
        if (root == nullptr){
            return false;
        }
        if(root->left==nullptr&&root->right==nullptr&&root->val==sum)
            return true;
        if (hasPathSum(root->left, sum-root->val))
            return true;
        if (hasPathSum(root->right, sum-root->val))
            return true;
        return false;
    }
};
```





