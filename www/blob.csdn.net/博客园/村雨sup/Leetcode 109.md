# Leetcode 109 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
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
    TreeNode* sortedListToBST(ListNode* head) {
        vector<int>vec;
        while(head != NULL){
            vec.push_back(head->val);
            head = head->next;
        }
        return DFS(vec,0,vec.size()-1);
        
    }
    TreeNode* DFS(vector<int> nums,int left,int right){
        if(left > right) return NULL;
        int mid = (left+right)/2;
        TreeNode* cur = new TreeNode(nums[mid]);
        cur->left = DFS(nums,left,mid-1);
        cur->right = DFS(nums,mid+1,right);
        return cur;
    }
};
```













