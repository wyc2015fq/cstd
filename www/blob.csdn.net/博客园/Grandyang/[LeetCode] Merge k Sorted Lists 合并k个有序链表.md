# [LeetCode] Merge k Sorted Lists 合并k个有序链表 - Grandyang - 博客园







# [[LeetCode] Merge k Sorted Lists 合并k个有序链表](https://www.cnblogs.com/grandyang/p/4606710.html)







Merge *k* sorted linked lists and return it as one sorted list. Analyze and describe its complexity.

Example:
Input:
[
  1->4->5,
  1->3->4,
  2->6
]
Output: 1->1->2->3->4->4->5->6


这道题让我们合并k个有序链表，最终合并出来的结果也必须是有序的，之前我们做过一道 [Merge Two Sorted Lists](http://www.cnblogs.com/grandyang/p/4086297.html)，是混合插入两个有序链表。这道题增加了难度，变成合并k个有序链表了，但是不管合并几个，基本还是要两两合并。那么我们首先考虑的方法是能不能利用之前那道题的解法来解答此题。答案是肯定的，但是需要修改，怎么修改呢，最先想到的就是两两合并，就是前两个先合并，合并好了再跟第三个，然后第四个直到第k个。这样的思路是对的，但是效率不高，没法通过OJ，所以我们只能换一种思路，这里就需要用到分治法 Divide and Conquer Approach。简单来说就是不停的对半划分，比如k个链表先划分为合并两个k/2个链表的任务，再不停的往下划分，直到划分成只有一个或两个链表的任务，开始合并。举个例子来说比如合并6个链表，那么按照分治法，我们首先分别合并0和3，1和4，2和5。这样下一次只需合并3个链表，我们再合并1和3，最后和2合并就可以了。代码中的k是通过 (n+1)/2 计算的，这里为啥要加1呢，这是为了当n为奇数的时候，k能始终从后半段开始，比如当n=5时，那么此时k=3，则0和3合并，1和4合并，最中间的2空出来。当n是偶数的时候，加1也不会有影响，比如当n=4时，此时k=2，那么0和2合并，1和3合并，完美解决问题，参见代码如下：



解法一：

```
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        if (lists.empty()) return NULL;
        int n = lists.size();
        while (n > 1) {
            int k = (n + 1) / 2;
            for (int i = 0; i < n / 2; ++i) {
                lists[i] = mergeTwoLists(lists[i], lists[i + k]);
            }
            n = k;
        }
        return lists[0];
    }
    ListNode* mergeTwoLists(ListNode* l1, ListNode* l2) {
        ListNode *dummy = new ListNode(-1), *cur = dummy;
        while (l1 && l2) {
            if (l1->val < l2->val) {
                cur->next = l1;
                l1 = l1->next;
            } else {
                cur->next = l2;
                l2 = l2->next;
            }
            cur = cur->next;
        }
        if (l1) cur->next = l1;
        if (l2) cur->next = l2;
        return dummy->next;
    }
};
```



我们再来看另一种解法，这种解法利用了[最小堆](https://zh.wikipedia.org/wiki/%E6%9C%80%E5%A4%A7%E2%80%94%E6%9C%80%E5%B0%8F%E5%A0%86)这种数据结构，我们首先把k个链表的首元素都加入最小堆中，它们会自动排好序。然后我们每次取出最小的那个元素加入我们最终结果的链表中，然后把取出元素的下一个元素再加入堆中，下次仍从堆中取出最小的元素做相同的操作，以此类推，直到堆中没有元素了，此时k个链表也合并为了一个链表，返回首节点即可，代码如下：



解法二：

```
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        auto cmp = [](ListNode*& a, ListNode*& b) {
            return a->val > b->val;
        };
        priority_queue<ListNode*, vector<ListNode*>, decltype(cmp) > q(cmp);
        for (auto node : lists) {
            if (node) q.push(node);
        }
        ListNode *dummy = new ListNode(-1), *cur = dummy;
        while (!q.empty()) {
            auto t = q.top(); q.pop();
            cur->next = t;
            cur = cur->next;
            if (cur->next) q.push(cur->next);
        }
        return dummy->next;
    }
};
```



下面这种解法利用到了混合排序的思想，也属于分治法的一种，做法是将原链表分成两段，然后对每段调用递归函数，suppose返回的left和right已经合并好了，然后再对left和right进行合并，合并的方法就使用之前那道 [Merge Two Sorted Lists](http://www.cnblogs.com/grandyang/p/4086297.html) 中的任意一个解法即可，这里我们使用了递归的写法，而本题解法一中用的是迭代的写法，参见代码如下：



解法三：

```
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        return helper(lists, 0, (int)lists.size() - 1);
    }
    ListNode* helper(vector<ListNode*>& lists, int start, int end) {
        if (start > end) return NULL;
        if (start == end) return lists[start];
        int mid = start + (end - start) / 2;
        ListNode *left = helper(lists, start, mid);
        ListNode *right = helper(lists, mid + 1, end);
        return mergeTwoLists(left, right);
    }
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



下面这种解法利用到了计数排序的思想，由留言区二楼热心网友闽A2436提供，思路是将所有的结点值出现的最大值和最小值都记录下来，然后记录每个结点值出现的次数，这样我们从最小值遍历到最大值的时候，就会按顺序经过所有的结点值，根据其出现的次数，建立相对应个数的结点。但是这种解法有个特别需要注意的地方，那就是合并后的链表结点都是重新建立的，若在某些情况下，我们不能新建结点，而只能交换或者重新链接结点的话，那么此解法就不能使用，但好在本题并没有这种限制，可以完美过OJ，参见代码如下：



解法四：

```
class Solution {
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        ListNode *dummy = new ListNode(-1), *cur = dummy;
        unordered_map<int, int> m;
        int mx = INT_MIN, mn = INT_MAX;
        for (auto node : lists) {
            ListNode *t = node;
            while (t) {
                mx = max(mx, t->val);
                mn = min(mn, t->val);
                ++m[t->val];
                t = t->next;
            }
        }
        for (int i = mn; i <= mx; ++i) {
            if (!m.count(i)) continue;
            for (int j = 0; j < m[i]; ++j) {
                cur->next = new ListNode(i);
                cur = cur->next;
            }
        }
        return dummy->next;
    }
};
```



类似题目：

[Merge Two Sorted Lists](http://www.cnblogs.com/grandyang/p/4086297.html)

[Ugly Number II](http://www.cnblogs.com/grandyang/p/4743837.html)



参考资料：

[https://leetcode.com/problems/merge-k-sorted-lists/](https://leetcode.com/problems/merge-k-sorted-lists/)

[https://leetcode.com/problems/merge-k-sorted-lists/discuss/10640/Simple-Java-Merge-Sort](https://leetcode.com/problems/merge-k-sorted-lists/discuss/10640/Simple-Java-Merge-Sort)

[https://leetcode.com/problems/merge-k-sorted-lists/discuss/10528/A-java-solution-based-on-Priority-Queue](https://leetcode.com/problems/merge-k-sorted-lists/discuss/10528/A-java-solution-based-on-Priority-Queue)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












