# leetcode-142. Linked List Cycle II 环形链表 II - 别说话写代码的博客 - CSDN博客





2019年01月18日 21:46:51[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：33标签：[Linked List Cycle II 																[leetcode																[环形链表 II](https://so.csdn.net/so/search/s.do?q=环形链表 II&t=blog)
个人分类：[数据结构/算法/刷题](https://blog.csdn.net/qq_21997625/article/category/6193466)

所属专栏：[leetcode刷题c++/python](https://blog.csdn.net/column/details/29882.html)](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)



|Given a linked list, return the node where the cycle begins. If there is no cycle, return `null`.To represent a cycle in the given linked list, we use an integer `pos` which represents the position (0-indexed) in the linked list where tail connects to. If `pos` is `-1`, then there is no cycle in the linked list.**Note:** Do not modify the linked list.**Example 1:****Input: **head = [3,2,0,-4], pos = 1**Output: **tail connects to node index 1**Explanation:** There is a cycle in the linked list, where tail connects to the second node.![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist.png)**Example 2:****Input: **head = [1,2], pos = 0**Output: **tail connects to node index 0**Explanation:** There is a cycle in the linked list, where tail connects to the first node.![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist_test2.png)**Example 3:****Input: **head = [1], pos = -1**Output: **no cycle**Explanation:** There is no cycle in the linked list.![](https://assets.leetcode.com/uploads/2018/12/07/circularlinkedlist_test3.png)**Follow up**:			Can you solve it without using extra space?|给定一个链表，返回链表开始入环的第一个节点。 如果链表无环，则返回 `null`。为了表示给定链表中的环，我们使用整数 `pos` 来表示链表尾连接到链表中的位置（索引从 0 开始）。 如果 `pos` 是 `-1`，则在该链表中没有环。**说明：**不允许修改给定的链表。**示例 1：****输入：**head = [3,2,0,-4], pos = 1**输出：**tail connects to node index 1**解释：**链表中有一个环，其尾部连接到第二个节点。![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/07/circularlinkedlist.png)**示例 2：****输入：**head = [1,2], pos = 0**输出：**tail connects to node index 0**解释：**链表中有一个环，其尾部连接到第一个节点。![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/07/circularlinkedlist_test2.png)**示例 3：****输入：**head = [1], pos = -1**输出：**no cycle**解释：**链表中没有环。![](https://assets.leetcode-cn.com/aliyun-lc-upload/uploads/2018/12/07/circularlinkedlist_test3.png)**进阶：**			你是否可以不用额外空间解决此题？|
|----|----|

思路：和[上一题](https://blog.csdn.net/qq_21997625/article/details/86546161)一样

**第一种**：哈希表，所有元素放入哈希表，最后一个元素下一个元素在哈希表中，就返回 哈希表元素，缺点是空间 大

**第二种**：快慢指针，slow走一步,quick 走两步，若quick或者quick->next为空则不存在环，如果碰头，就是有环，然后 一个指针从环这里，另一个指针从head开始，每个 走一步，知道最终 两个相遇就是要的交点

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
    ListNode *detectCycle(ListNode *head) {
        ListNode *quick=head,*slow=head;
        while(quick && quick->next)
        {
            slow=slow->next;
            quick=quick->next->next;
            if(slow == quick) break;
        }
        if(!quick || !quick->next) return NULL;
        slow = head;
        while(slow!=quick)
        {
            slow=slow->next;
            quick=quick->next;
        }return quick;
    }
};
```](https://so.csdn.net/so/search/s.do?q=Linked List Cycle II &t=blog)




