# Leetcode 86 - 村雨sup - 博客园








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
    ListNode* partition(ListNode* head, int x) {
        if(head == NULL) return head;
        ListNode* pres = new ListNode(-1);
        pres->next = head;
        ListNode* move = pres;
        
        ListNode* preb = new ListNode(-1);
        ListNode* move1 = preb;
        while(move->next != NULL){
            if(move->next->val < x){
                move = move->next;
            }
            else if(move->next->val >= x){
                move1->next = move->next;
                move->next = move->next->next;
                move1->next->next = NULL;
                move1 = move1->next;
            }
        }
        move->next = preb->next;
        return pres->next;
    }
};
```













