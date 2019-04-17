# Leetcode 82 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





有个错误就是member access within null pointer of type 'struct ListNode'  

其实就是判断了指针是否异常了，比如NULL->next之类。要记得用new给节点初始化，而指针不需要初始化



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
    ListNode* deleteDuplicates(ListNode* head) {
        if(head == NULL) return head;
        ListNode* bighead = new ListNode(0);
        ListNode* pre;
        ListNode* move;
        
        bighead->next = head;
        pre = bighead;
        move = head;
        
        while(move != NULL && move->next != NULL){
            if(move->next->val == move->val){
                while((move->next != NULL) && (move->next->val == move->val)){
                    move = move->next;                    
                }
                pre->next = move->next;
                move = move->next;
            }
            else{
                pre = pre->next;
                move = move->next;
            }
        }
        return bighead->next;
    }
};
```













