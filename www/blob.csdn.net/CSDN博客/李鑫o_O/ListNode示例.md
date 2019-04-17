# ListNode示例 - 李鑫o_O - CSDN博客





2016年05月17日 22:03:57[hustlx](https://me.csdn.net/HUSTLX)阅读数：928
















```cpp
ListNode* partition(ListNode* head, int x) {
    ListNode res(0), mid(0);
    if (!head) return NULL;
    ListNode* small=&res;
    ListNode* big=∣
    
    while (head) {
        if (head->val < x) {
            small->next = head;
            small = small->next;
        }
        else {
            big->next = head;
            big = big->next;
        }
        head = head->next;
    }
    big->next = NULL;
    small->next = mid.next;
    return res.next;
}
```





