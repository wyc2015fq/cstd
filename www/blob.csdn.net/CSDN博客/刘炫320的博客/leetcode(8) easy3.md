# leetcode(8):easy3 - 刘炫320的博客 - CSDN博客
2018年03月17日 09:03:51[刘炫320](https://me.csdn.net/qq_35082030)阅读数：118标签：[单链表反转																[单链表回文																[动态规划																[数组遍历](https://so.csdn.net/so/search/s.do?q=数组遍历&t=blog)](https://so.csdn.net/so/search/s.do?q=动态规划&t=blog)](https://so.csdn.net/so/search/s.do?q=单链表回文&t=blog)](https://so.csdn.net/so/search/s.do?q=单链表反转&t=blog)
个人分类：[leetcode](https://blog.csdn.net/qq_35082030/article/category/7480326)
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
渐渐的发现，即使是简单题，也并不简单。每当发现一些奇妙的解法时，总是能够新奇不已。
# 1.leetcode#169. Majority Element
## 1.1问题描述
> 
Given an array of size n, find the majority element. The majority element is the element that appears more than ⌊ n/2 ⌋ times. 
  You may assume that the array is non-empty and the majority element always exist in the array.
## 1.2思路
数字一一对对碰，碰没了就剩下最大的数了。这道题我们见过的。
## 1.3代码
```java
public int majorityElement(int[] nums) {
        int majorityElement=nums[0];
        int count=1;
        for(int i=1;i<nums.length;i++){
            if(majorityElement!=nums[i]){
                count--;
                if(count==0){
                    majorityElement=nums[i];
                    count=1;
                }
            }
            else{
                count++;
            }
        }
        return majorityElement;
    }
```
# 2.leetcode#198. House Robber
## 2.1问题描述
> 
You are a professional robber planning to rob houses along a street. Each house has a certain amount of money stashed, the only constraint stopping you from robbing each of them is that adjacent houses have security system connected and it will automatically contact the police if two adjacent houses were broken into on the same night. 
  Given a list of non-negative integers representing the amount of money of each house, determine the maximum amount of money you can rob tonight without alerting the police.
## 2.2思路
这也是动态规划，不过它的约束条件比较苛刻，那就是上一个取过了，这一个就不能取了。使用0/1背包问题的网格解法，就可以解决这个问题，甚至只用3个变量也可以解决。
## 2.3 代码
```java
public int rob(int[] nums) {
     int[][] dp = new int[nums.length + 1][2];
        for (int i = 1; i <= nums.length; i++) {
            dp[i][0] = Math.max(dp[i - 1][0], dp[i - 1][1]);
            //如果不带约束的话
            //dp[i][1]=Math.max(nums[i - 1] + dp[i - 1][0], nums[i - 1] + dp[i - 1][1]);
            //带了约束条件了
            dp[i][1] = nums[i - 1] + dp[i - 1][0];
        }
        return Math.max(dp[nums.length][0], dp[nums.length][1]);
    }
```
# 3.leetcode#206. Reverse Linked List
## 3.1问题描述
> 
Reverse a singly linked list.
## 3.2思路
这道题也是见过的，如果使用循环的话，就是把链表划分为2部分，一部分是已逆序的，一部分是未逆序的。逆序步骤为，先从未逆序的链表中，拆下头结点，然后作为已逆序的头结点，最后再把未逆序的头结点和已逆序的头结点都分别更新一下即可。
## 3.3 代码
```java
public ListNode reverseList(ListNode head) {
        if(head==null){
            return null;
        }
        else{
            ListNode newhead=null;
            //划分点之前为逆序的，划分点之后为正序
            while(head!=null){
                //记录正序的下一个节点
                ListNode temphead=head.next;
                //把正序的尾部连接到新节点上
                head.next=newhead;
                //把新节点头换成新的
                newhead=head;
                //把正序往后挪一个
                head=temphead;
            }
            return newhead;
        }
    }
```
# 4.leetcode#234. Palindrome Linked List
## 4.1问题描述
> 
Given a singly linked list, determine if it is a palindrome. 
  Follow up: 
  Could you do it in O(n) time and O(1) space?
## 4.2思路
如果是可以用额外空间的话，完全可以依靠位置相加之和为固定值来解决。但是现在只能使用O（1）的额外空间，这样可能就需要逆转链表的操作了，即让链表的后半部分逆转。但是这样就破坏了链表的结构了，那该怎么办？没事，既然能逆转1遍，就能逆转2遍，然后负负得正即可。网上代码基本上没有恢复这一操作。
## 4.3代码
```java
if(head==null||head.next==null) 
            return true;
        //使用快慢指针找到中间值和最后值
        ListNode fastnode=head;
        ListNode lownode=head;
        while(fastnode!=null&&fastnode.next!=null){
            fastnode=fastnode.next.next;
            lownode=lownode.next;
        }
        ListNode lasthead=lownode;
        //如果是奇数
        if(fastnode!=null){
            lasthead=lasthead.next;
        }
        ListNode newhead=null;
        //从中间值开始反转链表到最后
        while (lasthead!=null) {
            ListNode temp=lasthead.next;
            lasthead.next=newhead;
            newhead=lasthead;
            lasthead=temp;
        }
        //从最后开始比较两个数，记录是否不一致，并且恢复指针
        ListNode newhead2=null;
        Boolean flag=true;
        while(newhead!=null){
            if(newhead.val!=head.val){
                flag=false;
            }
            ListNode temp2=newhead.next;
            newhead.next=newhead2;
            newhead2=newhead;
            newhead=temp2;
            //一定不要忘记头还是要动的
            head=head.next;
        }
        lownode.next=newhead2;
        //返回是否一致
        return flag;
```
# 5.leetcode#283. Move Zeroes
## 5.1问题描述
> 
Given an array nums, write a function to move all 0’s to the end of it while maintaining the relative order of the non-zero elements.
Example
> 
For example, given nums = [0, 1, 0, 3, 12], after calling your function, nums should be [1, 3, 12, 0, 0]. 
  Note: 
  You must do this in-place without making a copy of the array. 
  Minimize the total number of operations
## 5.2思路
这道题是只允许使用O（1）的额外空间。这里我们使用人类常用的方法，那就是只要把0全部拿出即可，在最后都补上0. 
设置一个指针找到第一个是0的位置。然后对此后的数据开始搬迁，如果遇到0，就不搬迁，这样在最后全补上0即可。这样有一个好处，就是不用进行交换，因为交换是需要额外一个空间进行三步交换。但是我们这方法只需要赋值，即可。
## 5.3代码
```java
public void moveZeroes(int[] nums) {
         if(nums.length!=0&&nums!=null){
            int i=0;
            int insertindex=-1;
            //找到第一个是0的位置
            while(i<nums.length&&nums[i]!=0){
                i++;
            }
            //说明存在0
            if(i!=nums.length){
                insertindex=i++;
                while(i<nums.length){
                    if(nums[i]!=0){
                        nums[insertindex++]=nums[i];
                    }
                    i++;
                }
                while(insertindex<nums.length){
                    nums[insertindex++]=0;
                }
            }
        }
    }
```
# 6.小结
在本章中，我们主要学习的是单链表反转、单链表的回文和数组的遍历及简单的动态规划的问题。
