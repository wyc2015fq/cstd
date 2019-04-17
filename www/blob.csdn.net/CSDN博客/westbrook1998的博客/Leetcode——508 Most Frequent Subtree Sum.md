# Leetcode——508. Most Frequent Subtree Sum - westbrook1998的博客 - CSDN博客





2018年05月02日 22:47:25[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：60








> 
Given the root of a tree, you are asked to find the most frequent subtree sum. The subtree sum of a node is defined as the sum of all the node values formed by the subtree rooted at that node (including the node itself). So what is the most frequent subtree sum value? If there is a tie, return all the values with the highest frequency in any order.


> 
Examples 1 

  Input: 

    5 

   /  \ 

  2   -3 

  return [2, -3, 4], since all the values happen only once, return all of them in any order. 

  Examples 2 

  Input: 

    5 

   /  \ 

  2   -5 

  return [2], since 2 happens twice, however -5 only occur once. 

  Note: You may assume the sum of values in any subtree is in the range of 32-bit signed integer.
题意就是说找出每一个节点作为根的子树的和中出现频率最大的一个，如果频率一样就全部输出 

使用unorder_map和左右子树的递归 

代码1：
```cpp
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
    vector<int> findFrequentTreeSum(TreeNode* root) {
        vector<int> res;
        //记录每个子树和以及频率的映射表
        unordered_map<int,int> m;
        //记录最大频率
        int cnt=0;
        postorder(root,m,cnt,res);
        return res;
    }
    int postorder(TreeNode* node, unordered_map<int,int>& m,int& cnt,vector<int>& res){
        if(node==NULL){
            return 0;
        }
        int left=postorder(node->left,m,cnt,res);
        int right=postorder(node->right,m,cnt,res);
        int sum=left+right+node->val;
        //更新该子树和的频率
        m[sum]++;
        if(m[sum]>=cnt){
            //如果只有一个最大频率，则清空结果集
            if(m[sum]>cnt){
                res.clear();
            }
            res.push_back(sum);
            //cnt更新为当前最大频率
            cnt=m[sum];
        }
        return sum;
    }
};
```

代码2：（把更新res放在递归外）

```cpp
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
    vector<int> findFrequentTreeSum(TreeNode* root) {
        vector<int> res;
        //记录每个子树和以及频率的映射表
        unordered_map<int,int> m;
        //记录最大频率
        int cnt=0;
        postorder(root,m,cnt,res);
        //只要和最大频率一样就放入结果集中，无需考虑是否有多个
        for(auto a:m){
            if(a.second==cnt){
                res.push_back(a.first);
            }
        }
        return res;
    }
    int postorder(TreeNode* node, unordered_map<int,int>& m,int& cnt,vector<int>& res){
        if(node==NULL){
            return 0;
        }
        int left=postorder(node->left,m,cnt,res);
        int right=postorder(node->right,m,cnt,res);
        int sum=left+right+node->val;
        //更新该子树和的频率
        m[sum]++;
        //只要更新最大频率就行，res的更新放在递归外
        cnt=max(cnt,m[sum]);
        return sum;
    }
};
```

[参考博客](https://www.cnblogs.com/grandyang/p/6481682.html)

还是没办法独立完全做出来






