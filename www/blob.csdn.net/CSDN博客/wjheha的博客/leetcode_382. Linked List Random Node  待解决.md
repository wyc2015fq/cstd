# leetcode_382. Linked List Random Node ? 待解决 - wjheha的博客 - CSDN博客
2017年10月20日 18:36:17[wjheha](https://me.csdn.net/wjheha)阅读数：63标签：[leetcode](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)
个人分类：[数据结构和算法](https://blog.csdn.net/wjheha/article/category/6854909)
Given a singly linked list, return a random node’s value from the linked list. Each node must have the same probability of being chosen.
Follow up: 
What if the linked list is extremely large and its length is unknown to you? Could you solve this efficiently without using extra space?
Example:
// Init a singly linked list [1,2,3]. 
ListNode head = new ListNode(1); 
head.next = new ListNode(2); 
head.next.next = new ListNode(3); 
Solution solution = new Solution(head);
// getRandom() should return either 1, 2, or 3 randomly. Each element should have equal probability of returning. 
solution.getRandom();
提示：提交代码后，需要用简洁的语言解释一下代码思路~ 谢谢
历史题目和总结见公众号「每日一道算法题」
[https://leetcode.com/problems/linked-list-random-node/description/](https://leetcode.com/problems/linked-list-random-node/description/)
***思路***：
