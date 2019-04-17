# 21. Merge Two Sorted Lists - 李鑫o_O - CSDN博客





2016年03月20日 11:54:44[hustlx](https://me.csdn.net/HUSTLX)阅读数：171








```cpp
ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
    ListNode* temp= new ListNode(0);
    ListNode** res = &(temp->next);
    while (l1&&l2) {
        if (l1->val <= l2->val) {
            temp->next = l1;
            l1 = l1->next;
        }
        else if (l2->val < l1->val) {
            temp->next = l2;
            l2 = l2->next;
        }
        temp = temp->next;
    }
    temp->next = l1 ? l1 : l2;
    return *res;
}
```




