# leetcode(7): easy2 - 刘炫320的博客 - CSDN博客
2018年03月15日 10:26:06[刘炫320](https://me.csdn.net/qq_35082030)阅读数：127
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
今天我们接着讲简单题
# 1.leetcode#136. Single Number
## 1.1题目描述
> 
iven an array of integers, every element appears twice except for one. Find that single one. 
  Note: 
  Your algorithm should have a linear runtime complexity. Could you implement it without using extra memory?
## 1.2思路
简单的想法就是直接建一个hash表，然后记录下见过的词，如果是新词，就求和，如果是见过的，那么就减掉。
```java
public int singleNumber(int[] nums) {
        HashSet<Integer> hashMap=new HashSet<>();
        int sum=0;
        for(int i=0;i<nums.length;i++){
            if(hashMap.contains(nums[i])){
                sum-=nums[i];
            }
            else{
                hashMap.add(nums[i]);
                sum+=nums[i];
            }
        }
        return sum;
    }
```
## 1.3更进一步
但是事实上，人家只允许使用额外空间为O（1），因此上述方法行不通了，这时候，我们就要巧用位或运算符的性质，即一个数和0异或还是自己，一个数和自己异或是0。
## 1.4代码
```java
public int singleNumber2(int[] nums) {
        int a = 0 ;
        for(int i : nums){
            a ^= i;
        }
        return a;
    }
```
# 2.leetcode#141. Linked List Cycle
## 2.1问题描述
> 
Given a linked list, determine if it has a cycle in it. 
  Follow up: 
  Can you solve it without using extra space?
## 2.2思路
这道题考察的是新的一个思路，即快慢指针的使用。这样可以发现如果出现环了，那么快指针一定会追上慢指针。
## 2.3代码
```java
public boolean hasCycle(ListNode head) {
        if(head==null||head.next==null) return false;
        ListNode step1=head;
        ListNode step2=head;
        while(step1!=null&&step2!=null){
                //慢指针
                step1=step1.next;
                if(step2.next!=null){
                //快指针
                    step2=step2.next.next;
                }
                else{
                    return false;
                }
                if(step1.equals(step2)){
                    return true;
                }
        }
        return false;
    }
```
# 3.leetcode#155. Min Stack
## 3.1问题描述
> 
Design a stack that supports push, pop, top, and retrieving the minimum element in constant time. 
  push(x) – Push element x onto stack. 
  pop() – Removes the element on top of the stack. 
  top() – Get the top element. 
  getMin() – Retrieve the minimum element in the stack.
Example:
> 
MinStack minStack = new MinStack(); 
  minStack.push(-2); 
  minStack.push(0); 
  minStack.push(-3); 
  minStack.getMin();   –> Returns -3. 
  minStack.pop(); 
  minStack.top();      –> Returns 0. 
  minStack.getMin();   –> Returns -2.
## 3.2思路
这道题是比较清楚的，即让你设计一个最小的栈，它和普通的栈的唯一区别就是，它可以一直找到那个最小的。非常巧妙的一个方法就是只记录当前数与当前最小数的差值即可。
## 3.3代码
```java
class MinStack {
    long min;
        Stack<Long> stack;
        public MinStack(){
            stack=new Stack<>();
        }
        //压人
        public void push(int x) {
            //如果是空的，就压入0
            if (stack.isEmpty()){
                stack.push(0L);
                //设置最小值为X；
                min=x;
            }
            //如果不是空的
            else{
                //就需要压入差值
                stack.push(x-min);
                //如果是小于最小值的话
                if (x<min) min=x;
            }
        }
        //弹出
        public void pop() {
            //如果是空的，就返回为空
            if (stack.isEmpty()) return;
            long pop=stack.pop();
            //如果pop小于0，那么min就需要加上那么多
            if (pop<0)  min=min-pop;
        }
        //顶层
        public int top() {
            long top=stack.peek();
            //如果顶层是大于0的，说明不是最小值
            if (top>0){
                return (int)(top+min);
            }else{
               return (int)(min);
            }
        }
        //获取到最小值
        public int getMin() {
            return (int)min;
        }
}
```
# 4.leetcode#160. Intersection of Two Linked Lists
## 4.1问题描述
> 
Write a program to find the node at which the intersection of two singly linked lists begins.
Example
> 
For example, the following two linked lists: 
  A:          a1 → a2 
                     ↘ 
                       c1 → c2 → c3 
                     ↗ 
  B:     b1 → b2 → b3 
  begin to intersect at node c1.
Notes:
If the two linked lists have no intersection at all, return null. 
The linked lists must retain their original structure after the function returns. 
You may assume there are no cycles anywhere in the entire linked structure. 
Your code should preferably run in O(n) time and use only O(1) memory.
## 4.2思路
这道题最朴素的想法就是，我先遍历一遍，然后看谁长，谁长的话，我先遍历谁，然后到达两个一样长后，同时遍历。
## 4.3 代码
```java
public ListNode getIntersectionNode(ListNode headA, ListNode headB) {
        if(headA==null||headB==null){
            return null;
        }
        int countA=0;
        int countB=0;
        ListNode a = headA;
        ListNode b = headB;
        //统计个数
        while(a!=null){
            countA++;
            a=a.next;
        }
        while(b!=null){
            countB++;
            b=b.next;
        }
        //长的先走
        a=headA;
        b=headB;
        while(countA>countB){
            a=a.next;
            countA--;
        }
        while(countB>countA){
            b=b.next;
            countB--;
        }
        //判断相同的第一个
        while(a!=b){
            a=a.next;
            b=b.next;
        }
        return a;
    }
```
## 4.4更进一步
但是，其实还有更加巧妙的方法，这个方法不一定非常快，但是写代码一定非常快。那就是让指针A和指针B分别指向a和b链表，当指到头的时候，再换另一个链表头开始指，那么他们一定可以同时到达相同的节点，要么这个节点存在，要么同时到达NULL。 
证明：假设a长度为a+c,假设b长度为b+c,那么如果有重叠的就是a+c+b=b+c+a，如果没重叠，则是a+c+b+d=b+d+a+c；
## 4.5代码
```java
public ListNode getIntersectionNode(ListNode headA, ListNode headB) {
        if(headA==null||headB==null){
            return null;
        }
        ListNode a = headA;
        ListNode b = headB;
        while( a != b){
            //如果A到头了，那么就移动到B上面
            a = a == null? headB : a.next;
            //如果B到头了，那么就移动到A上面
            b = b == null? headA : b.next;    
        }
        //要么a为相同的，要么a为null
        return a;
    }
```
# 5.小结
这一次，我们学习了几个小技巧，第一个是使用异或运算，第二个是使用快慢指针，第三个是使用存储差值，第四个是使用两个链表链接，都是非常神奇的操作。
