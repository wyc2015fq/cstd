# leetcode-23.Merge k Sorted Lists 合并 k 个有序链表 - 别说话写代码的博客 - CSDN博客





2018年12月21日 11:53:15[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：27标签：[leetcode																[Merge k Sorted Lists																[合并k个有序链表](https://so.csdn.net/so/search/s.do?q=合并k个有序链表&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Merge k Sorted Lists&t=blog)




题目：
|Merge *k* sorted linked lists and return it as one sorted list. Analyze and describe its complexity.**Example:****Input:**[  1->4->5,  1->3->4,  2->6]**Output:** 1->1->2->3->4->4->5->6|合并 *k *个排序链表，返回合并后的排序链表。请分析和描述算法的复杂度。**示例:****输入:**[  1->4->5,  1->3->4,  2->6]**输出:** 1->1->2->3->4->4->5->6|
|----|----|

思路：

（1）可以借鉴[合并两个有序链表](https://blog.csdn.net/qq_21997625/article/details/85158124)的思路，比如1,2,3,4,5链表，可以1,2合并，然后将结果与3合并。这样很慢。我们 可以对其二分，再两两合并，然后再结合。比如1,2,3,4,5链表，我们分别合并（1,2）（3,4）（5）或者我们底下程序采用的这种（1,4）（2,5）（3），这就类似于归并排序，采用分治法。这样快很多。 底下的程序k=（n+1）/2表示合并之后总共有几个表，比如1,2,3,4,5合并一次后有3个表。

```cpp
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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        int n=lists.size(),k;
        if(n==0) return NULL;
        while(n>1)
        {
            k = (n+1)/2;
            for(int i=0;i<n/2;++i)
                lists[i] = mergelist(lists[i],lists[i+k]);
            n=k;
        }
        return lists[0];
    }
    ListNode* mergelist(ListNode* l1,ListNode *l2)//合并俩链表
    {
        ListNode* head=new ListNode(-1);
        ListNode* cur = head;
        while(l1 && l2)
        {
            if(l1->val < l2->val)
            {
                cur->next=l1;
                l1=l1->next;
            }else{
                cur->next = l2;
                l2 = l2->next;
            }cur = cur->next;
        }
        if(l1) cur->next = l1;  //记得不要忘了还要连接另外一个链表
        if(l2) cur->next = l2;
        return head->next;
    }
};
```

（2）用最小堆，首先把k个链表的首元素都加入最小堆中，它们会自动排好序。然后我们每次取出最小的那个元素加入我们最终结果的链表中，然后把取出元素的下一个元素再加入堆中，下次仍从堆中取出最小的元素做相同的操作，以此类推，直到堆中没有元素了，此时k个链表也合并为了一个链表，返回首节点即可

```cpp
struct cmp {
    bool operator () (ListNode *a, ListNode *b) {
        return a->val > b->val;
    }
};
class Solution {  
public:  
    ListNode *mergeKLists(vector<ListNode *> &lists) {  
        priority_queue<ListNode*, vector<ListNode*>, cmp> q;
        for (int i = 0; i < lists.size(); ++i) {
            if (lists[i]) q.push(lists[i]);
        }
        ListNode *head = NULL, *pre = NULL, *tmp = NULL;
        while (!q.empty()) {
            tmp = q.top();
            q.pop();
            if (!pre) head = tmp;
            else pre->next = tmp;
            pre = tmp;
            if (tmp->next) q.push(tmp->next);
        }
        return head;
    }  
};
```

参考：[http://www.cnblogs.com/grandyang/p/4606710.html](http://www.cnblogs.com/grandyang/p/4606710.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)




