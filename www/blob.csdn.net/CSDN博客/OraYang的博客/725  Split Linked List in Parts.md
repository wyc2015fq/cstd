
# 725. Split Linked List in Parts - OraYang的博客 - CSDN博客

2017年12月05日 11:06:53[OraYang](https://me.csdn.net/u010665216)阅读数：301所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



## 题目
Given a (singly) linked list with head node root, write a function to split the linked list into k consecutive linked list “parts”.
The length of each part should be as equal as possible: no two parts should have a size differing by more than 1. This may lead to some parts being null.
The parts should be in order of occurrence in the input list, and parts occurring earlier should always have a size greater than or equal parts occurring later.
Return a List of ListNode’s representing the linked list parts that are formed.
Examples 1->2->3->4, k = 5 // 5 equal parts [ [1], [2], [3], [4], null ]
**Example 1:**
> Input:

> root = [1, 2, 3], k = 5

> Output: [[1],[2],[3],[],[]]

> Explanation:

> The input and each element of the output are ListNodes, not arrays.

> For example, the input root has root.val = 1, root.next.val = 2, \root.next.next.val = 3, and root.next.next.next = null.

> The first element output[0] has output[0].val = 1, output[0].next = null.

> The last element output[4] is null, but it’s string representation as a ListNode is [].

> Example 2:

> Input:

> root = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10], k = 3

> Output: [[1, 2, 3, 4], [5, 6, 7], [8, 9, 10]]

> Explanation:

> The input has been split into consecutive parts with size difference at most 1, and earlier parts are a larger size than the later parts.

> Note:

> The length of root will be in the range [0, 1000].

> Each value of a node in the input will be an integer in the range [0, 999].

> k will be an integer in the range [1, 50].

> 思路

> 这道题目是希望把链表分成k组，其中有些限制条件，任意小组之间的长度不能超过1，而且小组链表长度是降序的。看懂题意这道题目的解题思路就很明确了：

> step1：初始化一个大小为k的链表数组，默认值为NULL

> step2：遍历链表并计算链表长度

> step3：计算划分k组后每个小组的链表长度，并开始进行划分

> 代码

> /**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
> class
> Solution {
> public
> :
> vector
> <ListNode*>
> splitListToParts(ListNode* root,
> int
> k) {
> //step1 initialize the vector
> vector
> <ListNode*>
> res(k,NULL);
> //step2 calculate the length of ListNode
> ListNode* node = root, *pre=NULL;
> int
> len =
> 0
> ;
> while
> (node)
        {
            len++;
            node = node->next;
        }
> //step3 split
> int
> n = len/k , r = len%k;
> for
> (
> int
> i=
> 0
> ;i<k;i++,r--)
        {
            res[i] = root;
> for
> (
> int
> j=
> 0
> ;j<n+(r>
> 0
> );j++)
            {
                pre = root;
                root = root->next;
            }
> if
> (pre)
                pre->next=NULL;
        }
> return
> res;        
    }
};

