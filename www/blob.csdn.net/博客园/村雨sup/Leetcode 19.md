# Leetcode 19 - 村雨sup - 博客园








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
//一开始发现头结点不好删啊，然后加了一个前置节点就ok了

class Solution {
public:
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        ListNode *add = new ListNode(0);
        add->next = head;
    
        ListNode *end = head;
        ListNode *start = add;
        for(int i=1;i < n;i++){
            end = end->next;
        }
        while(end->next != NULL){
            end = end->next;
            start = start->next;
        }
        start->next = start->next->next;
        return add->next;
    }
};
```













