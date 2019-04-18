# Leetcode全树类问题 - 無名黑洞 - CSDN博客
2014年02月04日 03:03:24[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：9774
**目录：**
**1、编号87 Scramble String (递归)2、编号95 Unique Binary Search Trees (DP)3、编号96 Unique Binary Search Trees II (递归)4、编号97 Binary Tree Inorder Traversal (递归或迭代)5、编号99 Validate Binary Search Tree (递归)6、编号100 Recover Binary Search Tree7、编号101 Same Tree (递归)8、编号102 Symmetric Tree (递归)9、编号103 Binary Tree Level Order Traversal (BFS)10、编号104 Binary Tree Zigzag Level Order (BFS)11、编号105 Maximum Depth of Binary Tree (递归)12、编号106 Construct Binary Tree from Preorder and Inorder Traversal (递归)13、编号107 Construct Binary Tree from Inorder and Postorder Traversal (递归)14、编号108 Binary Tree Level Order Traversal II (BFS)15、编号109 Convert Sorted Array to Binary Search Tree (递归)16、编号110 Convert Sorted List to Binary Search Tree (递归)17、编号111 Balanced Binary Tree (递归)18、编号112 Minimum Depth of Binary Tree (递归)19、编号113 Path Sum (递归)20、编号114 Path Sum II (递归)21、编号115 Flatten Binary Tree to Linked List (递归)22、编号117 Populating Next Right Pointers in Each Node23、编号120 Populating Next Right Pointers in Each Node II24、编号125 Binary Tree Maximum Path Sum (递归)25、编号130 Sum Root to Leaf Numbers (递归)26、编号145 Binary Tree Preorder Traversal (递归或迭代)27、编号146 Binary Tree Postorder Traversal (递归或迭代)**
算法总的来说就是递归(Stack, DFS)和广度优先(Queue, BFS)两种。下面有关二叉树类linked list的题目，若不加特别说明，都使用如下数据结构：
```cpp
/**
 * Definition for binary tree
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
```
**1、编号87 Scramble String**
Given a string s1, we may represent it as a binary tree by partitioning it to two non-empty substrings recursively.
Below is one possible representation of s1 = "great":
    great
   /    \
  gr    eat
 / \    /  \
g   r  e   at
           / \
          a   t
To scramble the string, we may choose any non-leaf node and swap its two children.
For example, if we choose the node "gr" and swap its two children, it produces a scrambled string "rgeat".
    rgeat
   /    \
  rg    eat
 / \    /  \
r   g  e   at
           / \
          a   t
We say that "rgeat" is a scrambled string of "great".
Similarly, if we continue to swap the children of nodes "eat" and "at", it produces a scrambled string "rgtae".
    rgtae
   /    \
  rg    tae
 / \    /  \
r   g  ta  e
       / \
      t   a
We say that "rgtae" is a scrambled string of "great".
Given two strings s1 and s2 of the same length, determine if s2 is a scrambled string of s1.
很奇怪的一道题，题目要看半天。难度不大，但是因为是树类题目的第一次出现，需要特别注意。方法是递归。观察到要是两个串相等，则必须满足：1含有相同的字母；2当把两个串分别拆成两部分后，第一串的两部分分别跟后一串的两部分相比，只要比得上一次就相等。了解规律之后，容易编写程序如下：
```cpp
/*Recursive*/
    bool isScramble(string s1, string s2) {
        if (s1 == s2) return true; /*Improve performance*/
        
        int A[26] = {0};
        for(int i = 0; i < s1.length(); i++) {
            A[s1[i]-'a']++;
            A[s2[i]-'a']--;
        }
        for (int i = 0; i < 26; i++) 
            if (A[i] != 0) return false;
        
        for (int i = 1; i < s1.length (); ++i) {
            if (isScramble(s1.substr(0, i), s2.substr(0, i)) &&
                isScramble(s1.substr(i), s2.substr(i)))/*i to the end of the string*/
                return true;
            
            if (isScramble(s1.substr(0, i), s2.substr(s1.length() - i)) &&
                isScramble(s1.substr(i), s2.substr(0, s1.length() - i)))
                return true;
        }
        
        return false;
    }
```
**2、编号95 Unique Binary Search Trees**
Given n, how many structurally unique BST's (binary search trees) that store values 1...n?
For example, Given n = 3, there are a total of 5 unique BST's.
   1         3     3      2      1
    \       /     /         / \      \
     3     2     1      1   3      2
    /     /       \                    \
   2     1         2                 3
DP解。[关于DP理论点这里。](http://blog.csdn.net/wangxiaojun911/article/details/4693458)
使用两层循环。i表示当前树的总结点数。j表示树其中一个分支的结点数。则总的数量等于这个分支的数量加上另外一个分支（结点数i-1-j）的数量。
```cpp
int numTrees(int n) {
        if(n == 1) return 1;
        vector<int> dp;
        for(int i = 0; i < n+1; i++) dp.push_back(0); /*safeguard at 0*/
        dp[0] = 1;
        dp[1] = 1;
         
        for(int i = 2; i < n+1; i++)
           for(int j = 0; j < i; j++)
               dp[i] += (dp[j]*dp[i-1-j]);
        
        return dp[n];
    }
```
**3、编号96 Unique Binary Search Trees II**
Given n, generate all structurally unique BST's (binary search trees) that store values 1...n.
For example,
Given n = 3, your program should return all 5 unique BST's shown below.
   1         3     3      2      1
    \       /     /         / \      \
     3     2     1      1   3      2
    /     /       \                   \
   2     1         2                 3
上题的升级版，使用了树的链接表作为数据结构，并且要求得到所有的子树。使用递归法解。绝大部分树类问题都可以用递归解。
```cpp
/*Recursive*/
class Solution {
public:
    vector<TreeNode *> generateTrees(int n) {
        if(n == 0) return generate(1, 0);
        return generate(1, n);
    }
    
    vector<TreeNode*> generate(int start, int end){
        vector<TreeNode*> subTree;
        if(start > end){
            subTree.push_back(NULL);
            return subTree;
        }
        /*Create tree procedure: Create left and right, then new the mid node, then push mid node*/
        for(int i = start; i <= end; i++){/*This two loop garantees the Unique Binary Tree*/
            vector<TreeNode*> leftSubs = generate(start, i-1);
            vector<TreeNode*> rightSubs = generate(i+1, end);
            for(int j = 0; j < leftSubs.size(); j++){
                for(int k = 0; k < rightSubs.size(); k++){
                    TreeNode *node = new TreeNode(i);
                    node->left = leftSubs[j];
                    node->right = rightSubs[k];
                    subTree.push_back(node);
                }
            }
        }
        return subTree;
    }
};
```
**4、编号97 Binary Tree Inorder Traversal**
Given a binary tree, return the inorder traversal of its nodes' values.
For example: Given binary tree {1,#,2,3},
   1
    \
     2
    /
   3
return [1,3,2].
Note: Recursive solution is trivial, could you do it iteratively?
中序遍历是先处理左边，然后中间，最后后边。递归很简单，但题目额外要求用非递归方法，则必须使用额外的栈来做。先列出递归解法如下。
```cpp
class Solution {
public:
    /*Recursive Solution*/
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> result;
        Traverse(result, root);
        return result;
    }
    
    void Traverse(vector<int> &result, TreeNode* root){
        if(root == NULL) return;
        Traverse(result, root->left);
        result.push_back(root->val);
        Traverse(result, root->right);
    }
};
```
非递归解法如下：
```cpp
class Solution {
public:
    /*Iterative*/
    vector<int> inorderTraversal(TreeNode *root) {
        vector<int> result;
        if(root == NULL) return result;
        
        vector<TreeNode*> stack;
        TreeNode *node = root;
        
        while(stack.size() != 0 || node != NULL){
            while(node != NULL){
                stack.push_back(node);
                node = node->left;
            }
            node = stack.back();
            stack.pop_back();
            result.push_back(node->val);
            node = node->right;
        }
    
        return result;
    }
};
```
**5、编号99 Validate Binary Search Tree**
Given a binary tree, determine if it is a valid binary search tree (BST).
Assume a BST is defined as follows:
The left subtree of a node contains only nodes with keys less than the node's key.
The right subtree of a node contains only nodes with keys greater than the node's key.
Both the left and right subtrees must also be binary search trees.
注意，空树被判定为合理的BST树。并且，当存在相同值的时候，BST不满足。然后写一个递归方法判定大小就可以了。
```cpp
class Solution {
public:
    bool isValidBST(TreeNode *root) {
        if(root == NULL) return true; 
        return Recursive(root, -INT_MAX, INT_MAX);
    }
    
    bool Recursive(TreeNode *root, int min, int max){
        if(root->val <= min || root->val >= max) return false;
        
        if(root->left != NULL){
            if(root->left->val >= root->val) return false;
            if(!Recursive(root->left, min, root->val)) return false;
        }
        
        if(root->right != NULL){
            if(root->right->val <= root->val) return false;
            if(!Recursive(root->right, root->val, max)) return false;
        }
        
        return true;
    }
};
```
**6、编号100 Recover Binary Search Tree**
Two elements of a binary search tree (BST) are swapped by mistake.
Recover the tree without changing its structure.
Note:
A solution using O(n) space is pretty straight forward. Could you devise a constant space solution?
如果没有空间要求的话，建立一个数组就可以直接解。因为二叉查找树的中序遍历既可以得到一个排好序的数组，可以很容易观察出错误的元素。但是若不用额外空间（递归也不能用），就只能通过复杂的指针操作来做。下面是网上找到的解答，比较长，但有详细注释。
```cpp
/*O(1) Space solution*/
 /* Function to traverse binary tree without recursion and without stack */
class Solution {
public:
    void recoverTree(TreeNode *root) {
       TreeNode *f1 = NULL, *f2 = NULL;
       TreeNode *current, *pre;
       
       TreeNode *parent = NULL; 
       bool found = false;
       if(root == NULL) return;
       
       current = root;
       while(current != NULL){                
             if(current->left == NULL){
                    if(parent && parent->val > current->val){
                           if(!found) {
                                 f1 = parent;
                                 found = true;
                           }
                           f2 = current;
                    }
                    parent = current;
                    current = current->right;     
             }   
             else {
                    /* Find the inorder predecessor of current */
                    pre = current->left;
                    while(pre->right != NULL && pre->right != current)
                           pre = pre->right;
                    /* Make current as right child of its inorder predecessor */
                    if(pre->right == NULL){
                           pre->right = current;
                           current = current->left;
                    }
                    /* Revert the changes made in if part to restore the original
                    tree i.e., fix the right child of predecssor */  
                    else {
                           pre->right = NULL;
                           if(parent->val > current->val) {
                                 if(!found) {
                                        f1 = parent;      
                                        found = true;
                                 }
                                 f2 = current;
                           }
                           parent = current;
                           current = current->right;     
                    } /* End of if condition pre->right == NULL */
             } /* End of if condition current->left == NULL*/
       } /* End of while */
       if(f1 && f2) swap(f1->val, f2->val);
    }
};
```
**7、编号101 Same Tree**
Given two binary trees, write a function to check if they are equal or not.
Two binary trees are considered equal if they are structurally identical and the nodes have the same value.
比较简单。注意不仅仅要比较树的结构，结点的值也要处理。
```cpp
class Solution {
public:
    bool isSameTree(TreeNode *p, TreeNode *q) {
        return Traverse(p, q);
    }
    
    bool Traverse(TreeNode *p, TreeNode *q){
        if(p == NULL) { if(q != NULL) return false; }
        if(q == NULL) { if(p != NULL) return false; }
        
        if(p == NULL && q == NULL) return true;
        
        if(p->val != q->val) return false;
        
        if(p->left == NULL) { if(q->left != NULL) return false; }
        if(p->right == NULL) { if(q->right != NULL) return false; }
        
        if(!Traverse(p->left, q->left)) return false;
        if(!Traverse(p->right, q->right)) return false;
        
        return true;
    }
};
```
**8、编号102 Symmetric Tree**
Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).
For example, this binary tree is symmetric:
    1
   / \
  2   2
 / \ / \
3  4 4  3
But the following is not:
    1
   / \
  2   2
   \   \
   3    3
使用递归（又是！）。注意22和23行的对称比较。
```cpp
class Solution {
public:
    /*Recursive solution, need iterative*/
    bool isSymmetric(TreeNode *root) {  
      if(root == NULL) return true;  
      return isSym(root->left, root->right);  
    } 
    
    bool isSym(TreeNode *left, TreeNode *right) {  
      if(left == NULL) return right == NULL;  
      if(right == NULL) return left == NULL;  
      if(left->val != right->val) return false;  
      if(!isSym(left->left, right->right)) return false;
      if(!isSym(left->right, right->left)) return false; 
      
      return true;  
    }
};
```
**9、编号103 Binary Tree Level Order Traversal**
Given a binary tree, return the level order traversal of its nodes' values. (ie, from left to right, level by level). For example: Given binary tree {3,9,20,#,#,15,7},
    3
   / \
  9  20
    /  \
   15   7
return its level order traversal as:
[
  [3],
  [9,20],
  [15,7]
]
树的广度优先遍历。使用队列实现。由于输出的关系，这里使用了两个队列，一个用来记录结点，一个记录结点的层级。
```cpp
class Solution {
public:
    vector<vector<int> > levelOrder(TreeNode *root) {
        vector<vector<int>> result;
        if(root == NULL) return result;
        
        queue<TreeNode*> q;
        queue<int> queue_level;
        q.push(root); 
        queue_level.push(0);
        
        vector<int> elem;
        result.push_back(elem);
        
        while(q.size() > 0){
            TreeNode* x = q.front();
            q.pop();
            int l = queue_level.front();
            queue_level.pop();
            
            if(l > (result.size()-1) ){
                vector<int> elem;
                result.push_back(elem);
            }
            result[l].push_back(x->val);
            
            if(x->left != NULL){
                q.push(x->left);
                queue_level.push(l+1);
            }
            
            if(x->right != NULL){
                q.push(x->right);
                queue_level.push(l+1);
            }
            
        }
        
        return result;
    }
};
```
**10、编号104 Binary Tree Zigzag Level Order**
Given a binary tree, return the zigzag level order traversal of its nodes' values. (ie, from left to right, then right to left for the next level and alternate between).
For example:
Given binary tree {3,9,20,#,#,15,7},
    3
   / \
  9  20
    /  \
   15   7
return its zigzag level order traversal as:
[
  [3],
  [20,9],
  [15,7]
]
跟上面那题差不多。区别是要z型输出。定义了新数组tmpArray，当要相反输出的时候就把它倒着放进结果里。在上面题的基础上写的，所以比较长。标记了/*Zigzag*/的是新加入的部分。
```cpp
class Solution {
public:
    vector<vector<int> > zigzagLevelOrder(TreeNode *root) {
        vector<vector<int>> result;
        if(root == NULL) return result;
        
        queue<TreeNode*> q;
        queue<int> queue_level;
        
        vector<int> elem;
        result.push_back(elem);
        
        bool leftToRight = false; /*Zigzag*/
        vector<TreeNode*> tmpArray;/*Zigzag*/
        vector<int> tmpArray_level;
        tmpArray.push_back(root);
        tmpArray_level.push_back(0);
        
        while(tmpArray.size() > 0 || q.size() > 0){
            
            /*Zigzag~*/
            if(q.size() == 0){
                /*!!!!Flip in each step!!!!!!!*/
                for(int i = tmpArray.size()-1; i >= 0 ; i--){
                    q.push(tmpArray[i]);
                    queue_level.push(tmpArray_level[i]);
                }
                leftToRight = !leftToRight;
                while(tmpArray.size() > 0) {
                    tmpArray.pop_back();
                    tmpArray_level.pop_back();
                }
            }
            /*~Zigzag*/
            
            TreeNode *x = q.front();
            q.pop();
            int l = queue_level.front();
            queue_level.pop();
            
            if(l > (result.size() - 1)){
                vector<int> elem;
                result.push_back(elem);
            }
            result[l].push_back(x->val);
            
            /*!!!!Flip in each two steps!!!!!!!*/
            if(leftToRight){
                if(x->left != NULL){
                    tmpArray.push_back(x->left);
                    tmpArray_level.push_back(l+1);
                }
                
                if(x->right != NULL){
                    tmpArray.push_back(x->right);
                    tmpArray_level.push_back(l+1);
                }
            }else{
                if(x->right != NULL){
                    tmpArray.push_back(x->right);
                    tmpArray_level.push_back(l+1);
                }
                
                if(x->left != NULL){
                    tmpArray.push_back(x->left);
                    tmpArray_level.push_back(l+1);
                }
            }
        }
        
        return result;
    }
};
```
**11、编号105 Maximum Depth of Binary Tree**
Given a binary tree, find its maximum depth.
The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.
找到树的最大深度。比较简单，但代表了一类题的解法。就是在递归的函数参数里增加变量，一层一层传递下去，以后还会用到。
```cpp
class Solution {
public:
    int maxDepth(TreeNode *root) {
        int maxDepth = 0;
        if(root == NULL) return maxDepth;
        Recursive(root, 1, maxDepth);
        return maxDepth;
    }
    
    void Recursive(TreeNode* root, int level, int& maxDepth){
        if(level > maxDepth) maxDepth = level;
        if(root->left != NULL) Recursive(root->left, level+1, maxDepth);
        if(root->right != NULL) Recursive(root->right, level+1, maxDepth);
    }
};
```
**12、编号106 Construct Binary Tree from Preorder and Inorder Traversal**
Given preorder and inorder traversal of a tree, construct the binary tree.
Note: You may assume that duplicates do not exist in the tree.
先通过一个简单的例子找规律。
   1
 /   \
2     3
 \    /
  4  5
preorder是：1,2,4,3,5 
inorder是：2,4,1,5,3 
首先preorder的第一个必然是根（1），然后此节点在inorder中的下标是2，那么在inorder中，处于1之前的两个节点2,4是左子树的；反之5，3是右子树的。 针对左子树，2，4就是它的inorder，而在preorder中，除开第一个根，数两个节点的子序列正好是2，4，这是左子树的preorder。这样这个问题就自然变成递归了： 即，其左子树的preorder是（2，4），inorder是（2，4）；类似有右子树preorder（3，5），inorder（5，3）
```cpp
class Solution {
public:
    /*Recursive*/
    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder) {
        if(preorder.size() == 0 || preorder.size() != inorder.size()) return NULL;
        return build(preorder, 0, preorder.size()-1, inorder, 0, inorder.size() - 1);
    }
    TreeNode *build(vector<int> &preorder, int start1, int end1, vector<int> &inorder, 
					   int start2, int end2){
        if(start1 > end1 || start2 > end2) return NULL;
        int val = preorder[start1];
        TreeNode* newNode = new TreeNode(val);
        int k = start2;
        for(; k <= end2; k++) if(inorder[k] == val) break;
        newNode->left =  build(preorder, start1 + 1, start1 + k - start2, inorder, 
					 start2, k - 1);
        newNode->right = build(preorder, start1 + k - start2 + 1, end1, inorder,
					 k + 1, end2);
        return newNode;
    }
};
```
**13、编号107 Construct Binary Tree from Inorder and Postorder Traversal**
Given inorder and postorder traversal of a tree, construct the binary tree.
Note: You may assume that duplicates do not exist in the tree.
上一题的类似过程。在postorder中，最后那一个肯定是整棵树的根，然后在inorder中查找这个根， 找到之后就能确定左子树和右子树的后序遍历和中序遍历，然后递归求解。
```cpp
class Solution {
public:
    TreeNode *buildTree(vector<int> inorder, vector<int> postorder) {
        if(inorder.size() == 0 || inorder.size() != postorder.size()) return NULL;
        return build(inorder, 0, inorder.size() - 1, postorder, 0, postorder.size() - 1);
    }
    TreeNode *build(vector<int> &inorder, int start1, int end1, vector<int> &postorder, 
					  int start2, int end2){
        if(start1 > end1 || start2 > end2) return NULL;
        int val = postorder[end2];
        TreeNode* newNode = new TreeNode(val);
        int k = start1;
        for(; k <= end1; k++) if(val == inorder[k]) break;
        newNode->right = build(inorder, k + 1,  end1,  postorder, 
					end2 - end1 + k, end2 - 1);
        newNode->left =  build(inorder, start1, k - 1, postorder, 
					start2, end2 - end1 + k - 1);
        return newNode;
    }
};
```
**14、编号108 Binary Tree Level Order Traversal II**
Given a binary tree, return the bottom-up level order traversal of its nodes' values. (ie, from left to right, level by level from leaf to root).
For example: Given binary tree {3,9,20,#,#,15,7},
    3
   / \
  9  20
    /  \
   15   7
return its bottom-up level order traversal as:
[
  [15,7]
  [9,20],
  [3],
]
上面出现过的某一题的第二次变种。这次不是从上向下打印，也不是从右往左打印，而是要从下往上输出结果。取巧的结果是，在获得最后结果后，反向交换。。。
```cpp
class Solution {
public:
    vector<vector<int> > levelOrderBottom(TreeNode *root) {
        vector<vector<int>> result;
        if(root == NULL) return result;
        
        queue<TreeNode*> q;
        queue<int> queue_level;
        q.push(root); 
        queue_level.push(0);
        
        vector<int> elem;
        result.push_back(elem);
        
        while(q.size() > 0){
            TreeNode* x = q.front();
            q.pop();
            int l = queue_level.front();
            queue_level.pop();
            
            if(l > (result.size()-1) ){
                vector<int> elem;
                result.push_back(elem);
            }
            result[l].push_back(x->val);
            
            if(x->left != NULL){
                q.push(x->left);
                queue_level.push(l+1);
            }
            
            if(x->right != NULL){
                q.push(x->right);
                queue_level.push(l+1);
            }
        }
        
        /*only difference*/
        for(int i = 0; i < result.size() / 2; i++) Swap(result[i], result[result.size()-1-i]);
        
        return result;
    }
    
    /*only difference*/
    void Swap(vector<int> &x, vector<int> &y){
        vector<int> tmp = x;
        x = y;
        y = tmp;
    }
};
```
**15、编号109 Convert Sorted Array to Binary Search Tree**
Given an array where elements are sorted in ascending order, convert it to a height balanced BST.
平衡二叉查找树的特点是，除了满足BST的基本特点，另外对于任意一个结点，它左子树和右子树的深度最大差1。所以不同用中间结点做每一步的根结点，递归就可以了。
```cpp
class Solution {
public:
    TreeNode *sortedArrayToBST(vector<int> &num) {  
        return BuildTree(num, 0, num.size()-1);  
    }  
    
    TreeNode *BuildTree(vector<int> &num, int start, int end)  
    {  
        if(start > end) return NULL;  
        if(start == end) return new TreeNode(num[start]);  
        int mid = (start + end) / 2;  
        TreeNode *node = new TreeNode(num[mid]);  
        node->left = BuildTree(num, start, mid-1);  
        node->right = BuildTree(num, mid+1, end);  
        return node;  
    }  
};
```
**16、编号110 Convert Sorted List to Binary Search Tree**
Given a singly linked list where elements are sorted in ascending order, convert it to a height balanced BST.
和上题一样递归就可以了。但是linked list不能像数组一样index任意数据，所以不能够自顶向下建立树。下面的方法是一个特别的自底向顶的递归方法。
```cpp
class Solution {
public:
    TreeNode *sortedListToBST(ListNode *head) {  
        int len = 0;  
        ListNode *p = head;  
        while(p){ /*Get the length*/
            len++;  
            p = p->next;  
        }  
        return BuildBST(head, 0, len-1);  
    }  
    
    TreeNode* BuildBST(ListNode* &list, int start, int end) {  
      if (start > end) return NULL;
      
      int mid = (start + end) / 2; 
      
      TreeNode *leftChild = BuildBST(list, start, mid-1);  
      
      TreeNode *parent = new TreeNode(list->val);  
      parent->left = leftChild;  
      list = list->next;  
      
      parent->right = BuildBST(list, mid+1, end);  
      return parent;  
    }  
};
```
**17、编号111 Balanced Binary Tree**
Given a binary tree, determine if it is height-balanced.
For this problem, a height-balanced binary tree is defined as a binary tree in which the depth of the two subtrees of every node never differ by more than 1.
和上面两题一样，注意每个结点都要判断它的左右子树。
```cpp
class Solution {
public:
    bool isBalanced(TreeNode *root) {  
        if(root == NULL) return true;  
      
        int diff = GetDiff(root);  
        if(diff == -1) return false;  /*-1 denotes unbalance*/
        return true;      
    }    
    int GetDiff(TreeNode* node){  
      if(node == NULL) return 0;  
      
      int left = GetDiff(node->left);  
      if(left == -1) return -1;  
      int right = GetDiff(node->right);  
      if(right == -1) return -1; 
      
      if(left-right>1 || right-left>1)  return -1;  
      return left>right? left+1:right+1;  
    }
};
```
**18、编号112 Minimum Depth of Binary Tree**
Given a binary tree, find its minimum depth.
The minimum depth is the number of nodes along the shortest path from the root node down to the nearest leaf node.
跟找最大深度一样，把变量当递归参数调用下去。
```cpp
class Solution {
public:
    int minDepth(TreeNode *root) {
        if(root == NULL) return 0;
        int min = INT_MAX;
        Recursion(root, 1, min);
        return min;
    }
    
    void Recursion(TreeNode *root, int level, int &min){
        if(root->left == NULL && root->right == NULL)
            if(level < min) min = level;
        if(root->left != NULL) Recursion(root->left, level+1, min);
        if(root->right != NULL) Recursion(root->right, level+1, min);
 
    }
};
```
**19、编号113 Path Sum**
Given a binary tree and a sum, determine if the tree has a root-to-leaf path such that adding up all the values along the path equals the given sum.
For example: Given the below binary tree and sum = 22,
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \      \
        7    2      1
return true, as there exist a root-to-leaf path 5->4->11->2 which sum is 22.
跟上题一个思路。。。
```cpp
class Solution {
public:
    bool hasPathSum(TreeNode *root, int sum) {
        if(root == NULL) return false;
        return Recursion(root, 0, sum);
    }                              
    
    bool Recursion(TreeNode *root, int num, int sum){
        int newNum = num + root->val;
        
        if(root->left == NULL && root->right == NULL){
            if(newNum == sum) return true;
            return false;
        }
        
        if(root->left != NULL  && Recursion(root->left,  newNum, sum)) return true;
        if(root->right != NULL && Recursion(root->right, newNum, sum)) return true;
        
        return false;
        
    }
};
```
**20、编号114 Path Sum II**
Given a binary tree and a sum, find all root-to-leaf paths where each path's sum equals the given sum.
For example: Given the below binary tree and sum = 22,
              5
             / \
            4   8
           /   / \
          11  13  4
         /  \    / \
        7    2  5   1
return
[
   [5,4,11,2],
   [5,8,4,5]
]
又跟上题一个思路。。。只不过输出结构不同。
```cpp
class Solution {
public:
    vector<vector<int> > pathSum(TreeNode *root, int sum) {
        vector<vector<int>> result;
        if(root == NULL) return result;
        vector<int> oneResult;
        Recursion(root, 0, sum, oneResult, result);
        return result;
    }
    
    void Recursion(TreeNode *root, int num, int sum, 
                vector<int> oneResult, vector<vector<int>> &result){
        int newNum = num + root->val;
        oneResult.push_back(root->val);
        
        if(root->left == NULL && root->right == NULL)
            if(newNum == sum) result.push_back(oneResult);
        
        if(root->left!=NULL)
            Recursion(root->left, newNum, sum, oneResult, result);
            
        if(root->right!=NULL)
            Recursion(root->right, newNum, sum, oneResult, result);
        
    }
};
```
**21、编号115 Flatten Binary Tree to Linked List**
Given a binary tree, flatten it to a linked list in-place.
For example, Given
         1
        / \
       2   5
      / \   \
     3   4   6
The flattened tree should look like:
   1
    \
     2
      \
       3
        \
         4
          \
           5
            \
             6
click to show hints.
Hints: If you notice carefully in the flattened tree, each node's right child points to the next node of a pre-order traversal.
改变树的结构是有点难的一类题。看不出规律的话就会觉得完全没有思路。解法还是递归。规律是每个结点左边必须是NULL，右边是：若用后续遍历树，则右边指针指向上一个处理的结点。
```cpp
class Solution {
public:
    void flatten(TreeNode *root) {
        TreeNode *lastRoot = NULL;
        Recursion(root, lastRoot);
    }
    
    void Recursion(TreeNode *root, TreeNode *&lastRoot){
        if(root == NULL) return;
        
        Recursion(root->right, lastRoot);
        Recursion(root->left, lastRoot);
        
        root->right = lastRoot;
        lastRoot = root;
        
        root->left = NULL;
    }
};
```
**22、编号117 Populating Next Right Pointers in Each Node**
**23、编号120 Populating Next Right Pointers in Each Node II**
Given a binary tree
    struct TreeLinkNode {
      TreeLinkNode *left;
      TreeLinkNode *right;
      TreeLinkNode *next;
    }
Populate each next pointer to point to its next right node. If there is no next right node, the next pointer should be set to NULL.
Initially, all next pointers are set to NULL.
Note: You may only use constant extra space.
You may assume that it is a perfect binary tree (ie, all leaves are at the same level, and every parent has two children).
For example, Given the following perfect binary tree,
         1
       /  \
      2    3
     / \  / \
    4  5  6  7
After calling your function, the tree should look like:
         1 -> NULL
       /  \
      2 -> 3 -> NULL
     / \  / \
    4->5->6->7 -> NULL
另一道改变树的结构的难题。把编号117和编号120列在一起是因为120除了输入数据不是perfect binary tree外，其它的条件都一样。
用level traversal来做很简单 难的是不能用辅助空间。
首先对于perfect binary tree的情况，有两种方法：
方法1：递归，很简单，但是使用了栈空间
```cpp
void connect(TreeLinkNode *root) {
        if(!root) return;
        
        if(root->left) root->left->next = root->right;
        if(root->right && root->next) root->right->next = root->next->left;
        connect(root->left);
        connect(root->right);
    }
```
方法2: 迭代，需要利用perfect binary tree这个条件.
使用两层循环：第一层循环用一个指针总是存最左边的端点；
第二层循环用另一个指针循环本层，每循环到一个点，把该节点底下左和右节点连上
```cpp
void connect(TreeLinkNode *root) {
        TreeLinkNode *leftEnd = root;
        while(leftEnd){
            TreeLinkNode *iter = leftEnd;
            do{
                if(iter->left) iter->left->next = iter->right;
                if(iter->right && iter->next) iter->right->next = iter->next->left;
                iter = iter->next;
            }while(iter);
            leftEnd = leftEnd->left;
        }
    }
```
对于不是perfect binary tree的情况，稍微复杂一点。首先需要对每个节点左右子树的情况分类讨论；第二是要一个循环来找下一个新的最左侧节点。
顺便说一句，120的解法也是肯定可以用来解117的。
```cpp
void connect(TreeLinkNode *root) {
        TreeLinkNode *leftEnd = root;
        while(leftEnd){
            TreeLinkNode *iter = leftEnd;//iter是遍历本层的迭代器
            //找到下一个可以作为左侧链接的节点
            TreeLinkNode *nextAvailableLeftNode = NULL; 
            do{
                TreeLinkNode *lc = iter->left;
                TreeLinkNode *rc = iter->right;
                
                if(lc && rc){ //按左右子树是否存在作三类别分类讨论
                    if(nextAvailableLeftNode) nextAvailableLeftNode->next = lc;
                    lc->next = rc;
                    nextAvailableLeftNode = rc;
                }else if(lc && !rc){
                    if(nextAvailableLeftNode) nextAvailableLeftNode->next = lc;
                    nextAvailableLeftNode = lc;
                }else if(!lc && rc){
                    if(nextAvailableLeftNode) nextAvailableLeftNode->next = rc;
                    nextAvailableLeftNode = rc;
                }
                
                iter = iter->next;
            }while(iter);
            
            
            //找到下一层的最左端的节点
            if(leftEnd && !leftEnd->left && !leftEnd->right) leftEnd = leftEnd->next;
            if(!leftEnd) break;
            if(leftEnd->left) leftEnd = leftEnd->left;
            else if(leftEnd->right) leftEnd = leftEnd->right;
        }
    }
```
**24、编号125 Binary Tree Maximum Path Sum**
Given a binary tree, find the maximum path sum.
The path may start and end at any node in the tree.
For example: Given the below binary tree,
       1
      / \
     2   3
Return 6
跟以前一样，把答案放参数里面递归就可以了。唯一小心的是这个path可以以任何点为起点和终点（不只是根结点和叶子）
```cpp
class Solution {
public:
    /*Recursive*/
    int maxPathSum(TreeNode *root) {
        if(root == NULL) return 0;
        
        int maxValue = -INT_MAX; /*Do not use maxValue here*/
        
        int v = root->val;
        int l = Recursion(root->left, maxValue);
        int r = Recursion(root->right, maxValue);
            
        //return:v, (l, r,) l+v, v+r, l+v+r, maxValue
        int returnValue = -INT_MAX;
        returnValue = max(returnValue, v);
        returnValue = max(returnValue, l+v);
        returnValue = max(returnValue, v+r);
        returnValue = max(returnValue, l+v+r);
        returnValue = max(returnValue, maxValue);
        return returnValue;
       
    }
    
    int Recursion(TreeNode *root, int &maxValue){
        if(root == NULL) return 0;
        
        int v = root->val;
        int l = Recursion(root->left, maxValue);
        int r = Recursion(root->right, maxValue);
 
        //maxValue:v, (l, r,) l+v, v+r, l+v+r
        maxValue = max(maxValue, v);
        maxValue = max(maxValue, l+v);
        maxValue = max(maxValue, v+r);
        maxValue = max(maxValue, l+v+r);
        
        //return: v, l+v, v+r
        return max(max(l+v, v+r), v);
    }
};
```
**25、编号130 Sum Root to Leaf Numbers**
Given a binary tree containing digits from 0-9 only, each root-to-leaf path could represent a number.
An example is the root-to-leaf path 1->2->3 which represents the number 123.
Find the total sum of all root-to-leaf numbers.
For example,
    1
   / \
  2   3
The root-to-leaf path 1->2 represents the number 12.
The root-to-leaf path 1->3 represents the number 13.
Return the sum = 12 + 13 = 25.
又是自顶向下递归（递归得要吐了有木有！）
```cpp
class Solution {
public:
    int sumNumbers(TreeNode *root) {
        if(root == NULL) return 0;
        
        int result = 0;
        
        if(root->left == NULL && root->right == NULL)  
            return root->val;
        
        GetSum(root->left, root->val, result);
        GetSum(root->right, root->val, result);
        
        return result;
    }
    
    void GetSum(TreeNode *root, int num, int &result){
        if(root == NULL) return;
        
        if(root->left == NULL && root->right == NULL){
            result += num*10 + root->val;
            return;
        }
        
        GetSum(root->left, num*10 + root->val, result);
        GetSum(root->right, num*10 + root->val, result);
    }
};
```
**26、编号145 Binary Tree Preorder Traversal**
Given a binary tree, return the preorder traversal of its nodes' values.
For example: Given binary tree {1,#,2,3},
   1
    \
     2
    /
   3
return [1,2,3].
Note: Recursive solution is trivial, could you do it iteratively?
跟编号97一个类型的题。97是中序，这个是前序。先给个简单的递归吧
```cpp
vector<int> preorderTraversal(TreeNode *root) {
        vector<int> result;
        Recursion(root, result);
        return result;
    }
    
    void Recursion(TreeNode *root, vector<int> &result){
        if(!root) return;
        result.push_back(root->val);
        Recursion(root->left, result);
        Recursion(root->right, result);
    }
```
下面是非递归。
```cpp
class Solution {
public:
    /*Iterative*/
    vector<int> preorderTraversal(TreeNode *root) {
        vector<int> result;
        if(!root) return result;
        
        stack<TreeNode*> s;
        s.push(root);
        
        while(!s.empty()){
            TreeNode* n = s.top();
            result.push_back(n->val);
            s.pop();
            
            if(n->right) s.push(n->right);
            if(n->left) s.push(n->left);
        }
        
        
        return result;
    }
};
```
**27、编号146 Binary Tree Postorder Traversal**
Given a binary tree, return the postorder traversal of its nodes' values.
For example: Given binary tree {1,#,2,3},
   1
    \
     2
    /
   3
return [3,2,1].
Note: Recursive solution is trivial, could you do it iteratively?
有中序和前序了，这个就是后序法。后续是 左-右-中，倒过来是 中-右-左。先处理中结点比较容易。最后再反过来就好了。
```cpp
class Solution {
public:
    vector<int> postorderTraversal(TreeNode *root) {
        vector<int> result;
        if(root == NULL) return result;
        
        stack<TreeNode *> s;
        s.push(root);
        
        while(!s.empty()){
            TreeNode *n = s.top();
            result.push_back(n->val);
            s.pop();
            
            if(n->left != NULL)  s.push(n->left);
            if(n->right != NULL) s.push(n->right);
        }
        
        reverse(result.begin(), result.end());
        
        return result;
    }
};
```
**参考文献：**
http://oj.leetcode.com/problems/
