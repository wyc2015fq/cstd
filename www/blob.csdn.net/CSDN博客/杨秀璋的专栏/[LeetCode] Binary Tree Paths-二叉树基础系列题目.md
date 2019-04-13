
# [LeetCode] Binary Tree Paths - 二叉树基础系列题目 - 杨秀璋的专栏 - CSDN博客

2015年09月09日 00:58:21[Eastmount](https://me.csdn.net/Eastmount)阅读数：2112



目录：
1.Binary Tree Paths - 求二叉树路径
2.Same Tree - 判断二叉树相等
3.Symmetric Tree - 判断二叉树对称镜像


> Binary Tree Paths

**题目概述：**
Given a binary tree, return all root-to-leaf paths.
For example, given the following binary tree:
1
 /   \
2     3
 \
  5
All root-to-leaf paths are:
["1->2->5", "1->3"]
**题目解析：**
本题主要考察二叉树遍历操作，输出二叉树的所有路径，通常采用递归方法能很好的解决。但是如果采用C语言编写，返回二维字符串数组如何添加二叉树路径是个难点？
char** binaryTreePaths(struct TreeNode* root, int* returnSize) {}
最终采用C++完成，当遍历至叶子节点时，通过容器push_back添加一条路径。
![](https://img-blog.csdn.net/20150909005152653)
**我的代码：**
```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    //创建空容器 对象类型为string类
    vector<string> result;
    void getPaths(TreeNode* node,string path) {
        if(node->left==NULL && node->right==NULL) { //左右子树为空 路径寻找完成 增加至数组中
            result.push_back(path);
        }
        if(node->left!=NULL) { //递归遍历左子树 当前路径添加左孩子结点
            getPaths(node->left,path+"->"+to_string(node->left->val));
        }
        if(node->right!=NULL) { //递归遍历右子树
            getPaths(node->right,path+"->"+to_string(node->right->val));
        }
    }
    
    //获取二叉树路径
    vector<string> binaryTreePaths(TreeNode* root) {
        if(root==NULL)
            return result;
        getPaths(root, to_string(root->val)); //to_string整数转换为字符串
        return result;
    }
};
```
**推荐代码：**
Java代码 地址：[http://segmentfault.com/a/1190000003465753](http://segmentfault.com/a/1190000003465753)
```python
public class Solution {
    
    List<String> res = new ArrayList<String>();
    
    public List<String> binaryTreePaths(TreeNode root) {
        if(root != null) findPaths(root,String.valueOf(root.val));
        return res;
    }
    
    private void findPaths(TreeNode n, String path){
        if(n.left == null && n.right == null) res.add(path);
        if(n.left != null) findPaths(n.left, path+"->"+n.left.val);
        if(n.right != null) findPaths(n.right, path+"->"+n.right.val);
    }
}
```

> Same Tree

判断两颗二叉树是否相等，非递归方法通过isSameNode依次遍历结点
```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
//递归方法
bool isSameTree(struct TreeNode* p, struct TreeNode* q) {
    if(p==NULL&&q==NULL)
        return true;
    else if( (p!=NULL&&q==NULL) || (p==NULL&&q!=NULL) )
        return false;
    else
    {
        if(p->val != q->val)
            return false;
        else
            return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);
    }
}
```


> Symmetric Tree

**题目概述：**
Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).
For example, this binary tree is symmetric:
1
   / \
  2   2
 / \ / \
3  4 4  3But the following is not:
1
   / \
  2   2
   \   \
   3    3Note:
Bonus points if you could solve it both recursively and iteratively.

**题目解析：**
判断二叉树是否为镜像对称二叉树，当时错误理解为判断完全二叉树。解题思路是通过比较左右结点，左结点->left和右结点->right比较、左结点->right和右结点->left比较。
非递归算法可以采用层次遍历，每次比较同一层的数是否镜像即可。
**我的代码：**
```python
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
 //比较左右结点
 bool isSameNode(struct TreeNode* L, struct TreeNode* R) {
     if(L==NULL&&R==NULL) {
         return true;
     }
     else if((L!=NULL&&R==NULL) || (L==NULL&&R!=NULL)) { //其中一个为空
         return false;
     }
     else if(L->val!=R->val) {
         return false;
     }
     else {
         return isSameNode(L->left,R->right) && isSameNode(L->right,R->left);
     }
 }
 //判断二叉树是否为镜像对称
bool isSymmetric(struct TreeNode* root) {
    if(!root)
        return true;
    else {
        return isSameNode(root->left,root->right);
    }
}
```
**非递归代码：**
来源地址：[http://blog.csdn.net/lc_910927/article/details/36180075](http://blog.csdn.net/lc_910927/article/details/36180075)
```python
class Solution {  
public:  
    bool isSymmetric (TreeNode* root) {  
        if (!root) return true;  
        stack<TreeNode*> s;  
        s.push(root->left);  
        s.push(root->right);  
        while (!s.empty ()) {  
            auto p = s.top (); s.pop();  
            auto q = s.top (); s.pop();  
            if (!p && !q) continue;  
            if (!p || !q) return false;  
            if (p->val != q->val) return false;  
            s.push(p->left);  
            s.push(q->right);  
            s.push(p->right);  
            s.push(q->left);  
        }  
        return true;  
    }  
};
```


PS：二叉树是面试中经常考察的题目，包括建立二叉树、遍历二叉树、二叉树交换、二叉树求和等。希望文章对你有所帮助，同时Java、C\#、C++、C学杂了容易混乱，再次验证了学精的重要性。
(By:Eastmount 2015-9-9 凌晨1点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))


