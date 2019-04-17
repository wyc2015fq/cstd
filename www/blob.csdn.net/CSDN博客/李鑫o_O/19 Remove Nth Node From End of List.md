# 19. Remove Nth Node From End of List - 李鑫o_O - CSDN博客





2016年03月19日 21:25:08[hustlx](https://me.csdn.net/HUSTLX)阅读数：180








```cpp
ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode* first = head;
    ListNode* second = head;
    int i = 0;
    while (second) {
        i++;
        second = second->next;
    }
    int j= 0;
    if(i==n) return head->next;
    while (j < i-n -1) {
        first = first->next;
        j++;
    }
    first->next = first->next->next;
    return head;
}
```




