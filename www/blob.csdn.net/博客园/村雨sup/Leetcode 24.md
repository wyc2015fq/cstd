# Leetcode 24 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
//需要注意的就是只用一个pre就能完成交换，一次AC，happy，已经忘记了第一次写链表时候的痛苦，所以说算法没难的，只不过练的欠火候。

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
    ListNode* swapPairs(ListNode* head) {
        if(head == NULL)return NULL;
        ListNode *res = new ListNode(0);
        res->next = head;
        ListNode *pre = res;
        while(pre->next && pre->next->next){
            ListNode *temp = pre->next->next->next;
            pre->next->next->next = pre->next;
            pre->next = pre->next->next;
            pre->next->next->next = temp;
            pre = pre->next->next;
        }
        return res->next;
    }
};
```













