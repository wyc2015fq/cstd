# leetcode(6): easy - 刘炫320的博客 - CSDN博客
2018年03月14日 23:21:36[刘炫320](https://me.csdn.net/qq_35082030)阅读数：97标签：[镜像树																[合并链表																[二叉树深度](https://so.csdn.net/so/search/s.do?q=二叉树深度&t=blog)](https://so.csdn.net/so/search/s.do?q=合并链表&t=blog)](https://so.csdn.net/so/search/s.do?q=镜像树&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
今天，我们主要讲几个简单的题目供大家休息。
# 1.leetcode#20. Valid Parentheses
## 1.1问题描述
> 
Given a string containing just the characters ‘(‘, ‘)’, ‘{‘, ‘}’, ‘[’ and ‘]’, determine if the input string is valid. 
  The brackets must close in the correct order, “()” and “()[]{}” are all valid but “(]” and “([)]” are not.
## 1.2思路
其实这道题由来已久，主要使用栈进行解决即可。当我们进入一个括号时，必须有对应相匹配的括号在才行，不能够相互交叉。但是关键一点在于，如何使用这个栈内的空间，也就是到底存什么。
## 1.3代码
```java
public boolean isValid(String s) {
       Stack<Character> stack = new Stack<Character>();
    for (char c : s.toCharArray()) {
        if (c == '(')
            stack.push(')');
        else if (c == '{')
            stack.push('}');
        else if (c == '[')
            stack.push(']');
        else if (stack.isEmpty() || stack.pop() != c)
            return false;
    }
    return stack.isEmpty();
    }
```
# 2.leetcode#21. Merge Two Sorted Lists
## 2.1问题描述
> 
Merge two sorted linked lists and return it as a new list. The new list should be made by splicing together the nodes of the first two lists.
Example:
> 
Input: 1->2->4, 1->3->4 
  Output: 1->1->2->3->4->4
## 2.2思路
这道题其实如果使用额外空间为O（n）的，应该是比较容易的，但是如果使用的是额外空间为O（1）的就很难了。但是这道题我们之前见过，可以使用递归的思想来解决。
## 2.3代码
```java
public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        if(l1 == null) return l2;
        if(l2 == null) return l1;
        if(l1.val < l2.val){
            l1.next = mergeTwoLists(l1.next, l2);
            return l1;
        } else{
            l2.next = mergeTwoLists(l1, l2.next);
            return l2;
        }
    }
```
# 3.leetcode#101. Symmetric Tree
## 3.1问题描述
> 
Given a binary tree, check whether it is a mirror of itself (ie, symmetric around its center).
Example
> 
For example, this binary tree [1,2,2,3,4,4,3] is symmetric: 
      1 
     / \ 
    2   2 
   / \ / \ 
  3  4 4  3 
  But the following [1,2,2,null,3,null,3] is not: 
      1 
     / \ 
    2   2 
     \   \ 
     3    3
## 3.2思路
还记得我之前一直说的么？遇到树的话，一定会有2个递归的方法才行，一个是树的路径，一个是约束条件。
## 3.3代码
```java
public boolean isSymmetric(TreeNode root) {
        if(root==null){
            return true;
        }
        return CompareSymmetriccnode(root.left, root.right);
    }
    public boolean CompareSymmetriccnode(TreeNode left,TreeNode right){
        if(left==null&&right==null){
            return true;
        }
        //如果都不是空的，并且也相等，那就要看他们自己是不是对称了。
        else if(left!=null&&right!=null&&left.val==right.val){
            if(CompareSymmetriccnode(left.left, right.right)&&CompareSymmetriccnode(left.right, right.left)){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
```
# 4.leetcode#104. Maximum Depth of Binary Tree
## 4.1问题描述
> 
Given a binary tree, find its maximum depth.
The maximum depth is the number of nodes along the longest path from the root node down to the farthest leaf node.
For example:
> 
Given binary tree [3,9,20,null,null,15,7], 
      3 
     / \ 
    9  20 
      /  \ 
     15   7 
  return its depth = 3.
## 4.2思路
这道题也有点太简单了。不讲了。
## 4.3代码
```java
public int maxDepth(TreeNode root) {
        if(root==null){
            return 0;
        }
        else{
            return Math.max(maxDepth(root.left), maxDepth(root.right))+1;
        }
    }
```
# 5.leetcode#121. Best Time to Buy and Sell Stock
## 5.1问题描述
> 
Say you have an array for which the ith element is the price of a given stock on day i. 
  If you were only permitted to complete at most one transaction (ie, buy one and sell one share of the stock), design an algorithm to find the maximum profit.
Example
> 
Example 1: 
  Input: [7, 1, 5, 3, 6, 4] 
  Output: 5 
  max. difference = 6-1 = 5 (not 7-1 = 6, as selling price needs to be larger than buying price) 
  Example 2: 
  Input: [7, 6, 4, 3, 1] 
  Output: 0 
  In this case, no transaction is done, i.e. max profit = 0.
## 5.2思路
这道题也是老朋友了，它的思想是使用O（n）的时间复杂度，从后往前遍历，然后获取最高价位与最高利润，即可。
## 5.3代码
```java
public int maxProfit(int[] prices) {
        if(prices==null||prices.length==0)return 0;
        int highprice=prices[prices.length-1];
        int maxProfit=0;
        for(int i=prices.length-1;i>=0;i--){
            if(prices[i]>highprice){
                highprice=prices[i];
                continue;
            }
            int profit=highprice-prices[i];
            if(profit>maxProfit){
                maxProfit=profit;
            }
        }
        return maxProfit;
    }
```
# 6.小结
像往常一样，我们需要小结一下本次内容，主要有镜像树、合并链表、求二叉树的深度等，相对比较简单。
