# Leetcode 92 - 村雨sup - 博客园








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
class Solution {
public:
    ListNode* reverseBetween(ListNode* head, int m, int n) {
        ListNode* pre = new ListNode(-1);
        pre->next = head;
        ListNode* move = pre;
        for(int i=1;i < m;i++){
            move = move->next;
        }
        ListNode* connect1 = move;
        ListNode* head1 = move->next;
        
        for(int i=0;i <= n-m;i++){
            move = move->next;
        }
        ListNode* head2 = move->next;
        move->next = NULL;
        
        head1 = func(head1);
        connect1->next = head1;
        while(head1->next != NULL){ head1 = head1->next;}
           
        head1->next = head2;
        
        return pre->next;
    }
    
    ListNode* func(ListNode* head){
        ListNode* pre = new ListNode(-1);
        ListNode* move = head;
        while(move != NULL){
            move = move->next;
            head->next = pre->next;
            pre->next = head;
            head = move;
        }
        return pre->next;
    }
};
```













