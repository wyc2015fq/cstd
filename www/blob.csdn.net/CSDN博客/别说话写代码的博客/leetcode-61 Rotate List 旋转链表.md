# leetcode-61. Rotate List 旋转链表 - 别说话写代码的博客 - CSDN博客





2019年01月18日 20:27:43[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：30标签：[旋转链表																[Rotate List																[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=Rotate List&t=blog)



|Given a linked list, rotate the list to the right by *k* places, where *k* is non-negative.**Example 1:****Input:** 1->2->3->4->5->NULL, k = 2**Output:** 4->5->1->2->3->NULL**Explanation:**rotate 1 steps to the right: 5->1->2->3->4->NULLrotate 2 steps to the right: 4->5->1->2->3->NULL**Example 2:****Input:** 0->1->2->NULL, k = 4**Output:**`2->0->1->NULL`**Explanation:**rotate 1 steps to the right: 2->0->1->NULLrotate 2 steps to the right: 1->2->0->NULLrotate 3 steps to the right: `0->1->2->NULL`rotate 4 steps to the right: `2->0->1->NULL`|给定一个链表，旋转链表，将链表每个节点向右移动 *k *个位置，其中 *k *是非负数。**示例 1:****输入:** 1->2->3->4->5->NULL, k = 2**输出:** 4->5->1->2->3->NULL**解释:**向右旋转 1 步: 5->1->2->3->4->NULL向右旋转 2 步: 4->5->1->2->3->NULL**示例 2:****输入:** 0->1->2->NULL, k = 4**输出:**`2->0->1->NULL`**解释:**向右旋转 1 步: 2->0->1->NULL向右旋转 2 步: 1->2->0->NULL向右旋转 3 步: `0->1->2->NULL`向右旋转 4 步: `2->0->1->NULL`|
|----|----|

思路：首先例子中给出的有可能k大于当前链表长度。所以第一步先遍历链表得到链表 表长，然后将链表尾和 链表头连接起来， 然后将链表往后移动len-k%len个， 其next就是新链表的头结点，然后将next指向空

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
    ListNode* rotateRight(ListNode* head, int k) {
        if(!head) return head;
        ListNode *cur=head,*newhead;
        int len=1;
        while(cur->next)
        {
            len++;
            cur=cur->next;
        }
        k=len-k%len;
        cur->next=head;
        for(len=0;len<k;++len)
            cur=cur->next;
        newhead = cur->next;
        cur->next=NULL;
        return newhead;
    }
};
```](https://so.csdn.net/so/search/s.do?q=旋转链表&t=blog)




