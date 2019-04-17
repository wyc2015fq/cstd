# Leetcode 61 - 村雨sup - 博客园








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
//注意取模，和空集

class Solution {
public:
    ListNode* rotateRight(ListNode* head, int k) {
        int num = cnt(head);
        if(num == 0) return head;
        k = k%num;
        if(k == 0) return head;
        int m = num-k;
        
        ListNode* move = head;
        for(int i=0;i < m-1;i++){
            move = move->next;
        }
        ListNode* reshead = move->next;
        ListNode* node1 = reshead;
        while(node1->next != NULL){node1 = node1->next;}
        move->next = NULL;
        node1->next = head;
        return reshead;
    }
    
    int cnt(ListNode* head){
        int res = 0;
        ListNode* temp = head;
        while(temp != NULL){
            res++;
            temp = temp->next;
        }
        return res;
    }
};
```













