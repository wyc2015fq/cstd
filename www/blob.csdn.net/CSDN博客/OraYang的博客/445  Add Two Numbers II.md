
# 445. Add Two Numbers II - OraYang的博客 - CSDN博客

2017年12月05日 11:52:11[OraYang](https://me.csdn.net/u010665216)阅读数：267标签：[linked																](https://so.csdn.net/so/search/s.do?q=linked&t=blog)[list																](https://so.csdn.net/so/search/s.do?q=list&t=blog)[add																](https://so.csdn.net/so/search/s.do?q=add&t=blog)[sum																](https://so.csdn.net/so/search/s.do?q=sum&t=blog)[stack																](https://so.csdn.net/so/search/s.do?q=stack&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=sum&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=add&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
[
																					](https://so.csdn.net/so/search/s.do?q=add&t=blog)所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)[
							](https://so.csdn.net/so/search/s.do?q=add&t=blog)
[
																	](https://so.csdn.net/so/search/s.do?q=list&t=blog)
[
				](https://so.csdn.net/so/search/s.do?q=linked&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=linked&t=blog)


## 题目
You are given two non-empty linked lists representing two non-negative integers. The most significant digit comes first and each of their nodes contain a single digit. Add the two numbers and return it as a linked list.
You may assume the two numbers do not contain any leading zero, except the number 0 itself.
Follow up:
What if you cannot modify the input lists? In other words, reversing the lists is not allowed.
Example:
> Input: (7 -> 2 -> 4 -> 3) + (5 -> 6 -> 4)

> Output: 7 -> 8 -> 0 -> 7

> 思路

> 本题让你做链表加法，因为个位在链表末尾，因为我们可以将链表反转，求和，最后再反转回来。还有一种方法就是利用栈的思想，先将链表元素压栈，然后求和，并新建链表。

> 代码

> /**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */
> class Solution {
> public
> :
    ListNode
> *
> addTwoNumbers(ListNode
> *
> l1, ListNode
> *
> l2) {
> //initialize stack
> stack
> <
> int
> >
> s1,s2;
> // push
> while
> (l1){s1
> .
> push(l1
> ->
> val);l1
> =
> l1
> ->
> next;}
> while
> (l2){s2
> .
> push(l2
> ->
> val);l2
> =
> l2
> ->
> next;}
> //sum
> int
> sum
> =
> 0
> ;
        ListNode
> *
> head
> =
> new
> ListNode(
> 0
> );
> while
> (
> !
> s1
> .
> empty()
> ||!
> s2
> .
> empty())
        {
> if
> (
> !
> s1
> .
> empty())
            {
> sum
> +=
> s1
> .
> top();
                s1
> .
> pop();
            }
> if
> (
> !
> s2
> .
> empty())
            {
> sum
> +=
> s2
> .
> top();
                s2
> .
> pop();
            }
            head
> ->
> val
> =
> sum
> %
> 10
> ;
            ListNode
> *
> tmp
> =
> new
> ListNode(
> sum
> /
> 10
> );
            tmp
> ->
> next
> =
> head;
            head
> =
> tmp;
> sum
> /=
> 10
> ;
        }
> //判断首位是否为0；
> return
> head
> ->
> val
> ?
> head:head
> ->
> next;
    }
};

