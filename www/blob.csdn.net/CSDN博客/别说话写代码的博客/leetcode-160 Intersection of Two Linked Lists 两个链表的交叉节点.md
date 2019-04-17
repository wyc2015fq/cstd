# leetcode-160. Intersection of Two Linked Lists 两个链表的交叉节点 - 别说话写代码的博客 - CSDN博客





2019年01月18日 22:36:10[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：38标签：[两个链表的交叉节点																[Intersection of Two Linked Lis																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Intersection of Two Linked Lis&t=blog)



|Write a program to find the node at which the intersection of two singly linked lists begins.For example, the following two linked lists:![](https://assets.leetcode.com/uploads/2018/12/13/160_statement.png)begin to intersect at node c1.**Example 1:**![](https://assets.leetcode.com/uploads/2018/12/13/160_example_1.png)**Input: **intersectVal = 8, listA = [4,1,8,4,5], listB = [5,0,1,8,4,5], skipA = 2, skipB = 3**Output:** Reference of the node with value = 8**Input Explanation:** The intersected node's value is 8 (note that this must not be 0 if the two lists intersect). From the head of A, it reads as [4,1,8,4,5]. From the head of B, it reads as [5,0,1,8,4,5]. There are 2 nodes before the intersected node in A; There are 3 nodes before the intersected node in B.**Example 2:**![](https://assets.leetcode.com/uploads/2018/12/13/160_example_2.png)**Input: **intersectVal = 2, listA = [0,9,1,2,4], listB = [3,2,4], skipA = 3, skipB = 1**Output:** Reference of the node with value = 2**Input Explanation:** The intersected node's value is 2 (note that this must not be 0 if the two lists intersect). From the head of A, it reads as [0,9,1,2,4]. From the head of B, it reads as [3,2,4]. There are 3 nodes before the intersected node in A; There are 1 node before the intersected node in B.**Example 3:**![](https://assets.leetcode.com/uploads/2018/12/13/160_example_3.png)**Input: **intersectVal = 0, listA = [2,6,4], listB = [1,5], skipA = 3, skipB = 2**Output:** null**Input Explanation:** From the head of A, it reads as [2,6,4]. From the head of B, it reads as [1,5]. Since the two lists do not intersect, intersectVal must be 0, while skipA and skipB can be arbitrary values.**Explanation:** The two lists do not intersect, so return null.**Notes:**- If the two linked lists have no intersection at all, return `null`.- The linked lists must retain their original structure after the function returns.- You may assume there are no cycles anywhere in the entire linked structure.- Your code should preferably run in O(n) time and use only O(1) memory.|编写一个程序，找到两个单链表相交的起始节点。如下面的两个链表**：**![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/14/160_statement.png)在节点 c1 开始相交。**示例 1：**![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/14/160_example_1.png)**输入：**intersectVal = 8, listA = [4,1,8,4,5], listB = [5,0,1,8,4,5], skipA = 2, skipB = 3**输出：**Reference of the node with value = 8**输入解释：**相交节点的值为 8 （注意，如果两个列表相交则不能为 0）。从各自的表头开始算起，链表 A 为 [4,1,8,4,5]，链表 B 为 [5,0,1,8,4,5]。在 A 中，相交节点前有 2 个节点；在 B 中，相交节点前有 3 个节点。**示例 2：**![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/14/160_example_2.png)**输入：**intersectVal = 2, listA = [0,9,1,2,4], listB = [3,2,4], skipA = 3, skipB = 1**输出：**Reference of the node with value = 2**输入解释：**相交节点的值为 2 （注意，如果两个列表相交则不能为 0）。从各自的表头开始算起，链表 A 为 [0,9,1,2,4]，链表 B 为 [3,2,4]。在 A 中，相交节点前有 3 个节点；在 B 中，相交节点前有 1 个节点。**示例 3：**![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/14/160_example_3.png)**输入：**intersectVal = 0, listA = [2,6,4], listB = [1,5], skipA = 3, skipB = 2**输出：**null**输入解释：**从各自的表头开始算起，链表 A 为 [2,6,4]，链表 B 为 [1,5]。由于这两个链表不相交，所以 intersectVal 必须为 0，而 skipA 和 skipB 可以是任意值。**解释：**这两个链表不相交，因此返回 null。**注意：**- 如果两个链表没有交点，返回 `null`.- 在返回结果后，两个链表仍须保持原有的结构。- 可假定整个链表结构中没有循环。- 程序尽量满足 O(*n*) 时间复杂度，且仅用 O(*1*) 内存。|
|----|----|

思路：

**第一种**：先得到两个链表的各自长度，然后看哪个链表长，哪个链表先走和另一个链表长度的差值，然后相当于现在两个链表一样长了，因为链表交点后面的 元素肯定是相同的，所以两个 链表从当前一起往后，看是否是一个节点，若都 空则没交点

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
    int lenOfList(ListNode * li)
    {
        int len=0;
        while(li)
        {
            len++;
            li=li->next;
        }return len;
    }
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if(!headA || !headB) return NULL;
        int len1=lenOfList(headA),len2=lenOfList(headB);
        if(len1<len2)
            for(int i=0;i<len2-len1;++i) headB=headB->next;
        else
            for(int i=0;i<len1-len2;++i) headA=headA->next;
        while(headA && headB && headA!=headB)
        {
            headA=headA->next;
            headB=headB->next;
        }
        return (headA && headB)? headA : NULL;
    }
};
```

**第二种**：很神奇的思路，链表A和B同时遍历，分别遍历到尾的时候从下一个节点开始，如果 两个链表相等，则就是交叉节点，若都为NULL，说明两个遍历指针将两个链表走完了。举个栗子：

![](https://assets.leetcode.com/uploads/2018/12/13/160_example_1.png)

A遍历到5的时候，下一个从B的头开始 遍历，B遍历到尾5的时候从A的头开始，等两个 遍历节点相交也就是同时在8的地方，结束。这是为什么第一个链表长度为5， 第二个为6，第一个遍历完又遍历了第二 个的前3个， 相当于走了5+3=8个，第二个 遍历完又遍历了第1个的前2个，就是6+2=8，也就是说他俩相遇就是交点，要么都为空（两个指针分别将两个链表遍历完了）

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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        if(!headA ||!headB) return NULL;
        ListNode *a=headA,*b=headB;
        while(a!=b)
        {
            a=a ? a->next :headB;
            b=b ? b->next :headA;
        }
        return a;
    }
};
```](https://so.csdn.net/so/search/s.do?q=两个链表的交叉节点&t=blog)




