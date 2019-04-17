# 23.Merge k Sorted Lists - 李鑫o_O - CSDN博客





2016年03月21日 15:53:56[hustlx](https://me.csdn.net/HUSTLX)阅读数：280








```cpp
ListNode* mergeKLists(vector<ListNode*>& lists) {
    ListNode* temp = new ListNode(0);
    ListNode** res = &(temp->next);
    vector<int> v;
    for (int i = 0; i < lists.size(); i++) {
        while (lists[i]) {
            v.push_back(lists[i]->val);
            lists[i]=lists[i]->next;
        }
    }
    sort(v.begin(), v.end());
    for (auto i : v) {
        temp->next = new ListNode(i);
        temp = temp->next;
    }
    return *res;
}
```




