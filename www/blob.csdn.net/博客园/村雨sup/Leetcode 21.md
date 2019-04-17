# Leetcode 21 - 村雨sup - 博客园








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
//还可以简单的一遍AC

class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode *head = new ListNode(0);
        ListNode *move = head;
        while(l1 || l2){
            int data = 0;
            if(l1 == NULL){data = l2->val;l2 = l2->next;}
            else if(l2 == NULL){data = l1->val;l1 = l1->next;}
            else{
                if(l1->val < l2->val){data = l1->val;l1 = l1->next;}
                else{data = l2->val;l2 = l2->next;}
            }
            
            ListNode *add = new ListNode(data);
            move->next = add;
            move = move->next;
        }
        return head->next;
    }
};
```

```
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1) return l2;
        if (!l2) return l1;
        if (l1->val < l2->val) {
            l1->next = mergeTwoLists(l1->next, l2);
            return l1;
        } else {
            l2->next = mergeTwoLists(l1, l2->next);
            return l2;
        }
    }
};
```

```
class Solution {
public:
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        if (!l1 || (l2 && l1->val > l2->val)) swap(l1, l2);
        if (l1) l1->next = mergeTwoLists(l1->next, l2);
        return l1;
    }
};
```













