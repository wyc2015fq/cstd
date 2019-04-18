# 链表的相关操作（1）_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月14日 14:16:59[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：27
个人分类：[数据结构和算法																[链表](https://blog.csdn.net/zhuzhuxia183/article/category/8609128)](https://blog.csdn.net/zhuzhuxia183/article/category/8608092)

所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)








## 来一波关于单链表的骚操作，嘻嘻嘻

关于链表这种结构，上数据结构的时候大家其实都是有了解过的啦，其实接下来总结的这些题目呢，大多都是基础题目，但是个人感觉要想比较高效、优美的实现，也是要注意一些小trick的啦。
- 1、 删除链表的倒数第N个节点 [https://leetcode-cn.com/problems/remove-nth-node-from-end-of-list/](https://leetcode-cn.com/problems/remove-nth-node-from-end-of-list/)

给定一个链表，删除链表的倒数第 n 个节点，并且返回链表的头结点。

示例：

给定一个链表: 1->2->3->4->5, 和 n = 2.

当删除了倒数第二个节点后，链表变为 1->2->3->5.

说明：

给定的 n 保证是有效的。

进阶：

你能尝试使用一趟扫描实现吗？
当然，这个题目思考的点就在于如何遍历一次就可以找到呢？

这里是通过两个指针，首先，一个指向头，另外一个指向距离他n+1的位置的话，然后同时往后遍历就可以啦

```
public ListNode removeNthFromEnd(ListNode head, int n) {
        ListNode new_head = new ListNode(-1);
        new_head.next = head;
        ListNode first = new_head,second = new_head;
        for (int i = 0; i < n+1; i++) {
            first = first.next;
        }
        while(first!=null){
            first = first.next;
            second = second.next;
        }

        second.next = second.next.next;
        return new_head.next;
        
    }
```

我jio得 这种显得逼格很高，嘻嘻嘻，骚
- 2、 合并两个有序链表 [https://leetcode-cn.com/problems/merge-two-sorted-lists/](https://leetcode-cn.com/problems/merge-two-sorted-lists/)

将两个有序链表合并为一个新的有序链表并返回。新链表是通过拼接给定的两个链表的所有节点组成的。

示例：

输入：1->2->4, 1->3->4

输出：1->1->2->3->4->4
差不多算是很规规矩矩的题目啦，不过呢，提醒一下，leetcode上面的链表是没有标志头的，所以包括第一题，都是自己强行补一个上来，至于这样做的好处，课本上应该都会有介绍，是为了保证无论链表是不是空都保持结构的一致性，废话不多说，上代码：

```
public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        ListNode head = new ListNode(-1);
        ListNode node = head;
        while ((l1!=null)&&(l2!=null)){
            int value1 = l1.val;
            int value2 = l2.val;
            if (value1<value2){
                node.next = l1;
                node = node.next;
                l1 = l1.next;
            }else{
                node.next = l2;
                node = node.next;
                l2 = l2.next;
            }
        }
        if (l1!=null){
            node.next = l1;
        }
        if(l2!=null){
            node.next = l2;
        }
        return head.next;
    }
```
- 3、 合并K个排序链表 [https://leetcode-cn.com/problems/merge-k-sorted-lists/](https://leetcode-cn.com/problems/merge-k-sorted-lists/)

合并 k 个排序链表，返回合并后的排序链表。请分析和描述算法的复杂度。

示例:

输入:

[

1->4->5,

1->3->4,

2->6

]

输出: 1->1->2->3->4->4->5->6
这个题目，可以说就是上面题目的加强版，然后如果说站在上面的基础上来看这个题目，可以给出一个很基础的回答，从头开始，两两一合并，操作的时候，我们按照0+1生成的覆盖1的那个元素，再1+2，覆盖2的位置，一直到length-1。ok,上代码：

```
public ListNode mergeKLists(ListNode[] lists) {
         int length = lists.length;
         if (length==0)
             return null;
         int n= length;
         for (int i = 1; i <length; i++) {
             lists[i] = mergeTwoLists(lists[i-1],lists[i]);
       	 }
        return lists[length-1];
    }
    
    
    public ListNode mergeTwoLists(ListNode l1, ListNode l2) {
        ListNode head=new ListNode(0); 
        ListNode node = head;
        while(l1!=null&&l2!=null){
            node.next = l1.val<l2.val?l1:l2;
            node = node.next;
            if(l1.val<l2.val){
                l1 =l1.next;
            }else{
                l2 = l2.next;
            }
        }
        if(l1!=null){
            node.next = l1;
        }
        if(l2!=null){
            node.next = l2;
        }
        
        return head.next;
    }
```

emmmm,但是不知道为什么，这种的效果其实不是很好，然后发现两外一种合并的思路最后的效率高很多，代码如下

```
public ListNode mergeKLists(ListNode[] lists) {
		int length = lists.length;
        if (length==0)
            return null;
        int n=length;
        while (n>1){
            int i=0,j=0;
            for (;i<n-1;i+=2) {
                ListNode node = mergeTwoLists(lists[i],lists[i+1]);
                lists[j++] = node;
            }
            if (i==n-1) lists[j++] = lists[i];
            n = j;
        }
        return lists[0];
   }
```

这种思路就是利用两两一合并，这种的就是尽可能的使得避免重复元素的合并，嗯，蛮好的一个点～
- 4、两两交换链表中的节点 [https://leetcode-cn.com/problems/swap-nodes-in-pairs/submissions/](https://leetcode-cn.com/problems/swap-nodes-in-pairs/submissions/)

给定一个链表，两两交换其中相邻的节点，并返回交换后的链表。

示例:

给定 1->2->3->4, 你应该返回 2->1->4->3.
这种题目，其实可以在遍历过程中进行修改，但是我们需要三个变量，分别是pre,first,next三个变量，让next->first，然后pre->next，first->next.next,代码如下：

```
public ListNode swapPairs(ListNode head) {
        ListNode new_head = new ListNode(0);
        new_head.next = head;
        ListNode first ,second,third;
        first = new_head;
    
        while (first.next!=null&&first.next.next!=null){
            second = first.next;
            third = second.next;
            
            
            second.next = third.next;
            third.next = second;
            first.next = third;
            
            first = second;
        }
        return new_head.next;
    }
```
- 5、k个一组翻转链表  [https://leetcode-cn.com/problems/reverse-nodes-in-k-group/](https://leetcode-cn.com/problems/reverse-nodes-in-k-group/)

给出一个链表，每 k 个节点一组进行翻转，并返回翻转后的链表。

k 是一个正整数，它的值小于或等于链表的长度。如果节点总数不是 k 的整数倍，那么将最后剩余节点保持原有顺序。

示例 :

给定这个链表：1->2->3->4->5

当 k = 2 时，应当返回: 2->1->4->3->5

当 k = 3 时，应当返回: 3->2->1->4->5

说明 :

你的算法只能使用常数的额外空间。

你不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换
是不是瞬间觉得这个是上面一个题目的普遍情况，上面是k=2的特例而已嘛，这个咋搞呢，先上代码：

```
public ListNode reverseKGroup(ListNode head, int k) {
        ListNode new_Head= new ListNode(-1);
        new_Head.next = head;
        ListNode pre,cur;
        pre = new_Head;
        cur = pre.next;
        int index = 1;
        while (cur!=null){
            if (index%k==0){
                pre = getReverseSubGroup(pre,cur.next);
                cur = pre.next;
            }else{
                cur = cur.next;
            }
            index++;
        }
        return new_Head.next;
    }

    private ListNode getReverseSubGroup(ListNode pre, ListNode last) {
        ListNode first = pre.next;
        ListNode cur = first.next;
        while (cur!=last){
            first.next = cur.next;
            cur.next = pre.next;
            pre.next = cur;
            cur = first.next;
        }
        return first;
    }
```

上面的代码主要思想是：从头遍历到尾，遍历了k遍的时候就会进行一次逆序，然后返回逆序的最后一个元素，令pre 记录，不断的重复进行。重点是这个逆序函数，我们其实是不断的用first来记录最开始的位置，他一直指向cur.next的对象，然后令pre.next指向当前的要逆序的两个元素的首个，然后second来表示下一个。最终的状态时first.next为下一个逆序的开头对象，pre.next为该逆序序列的所在原始序列的左后一个，也就是逆序之后的开头

嗯，其实有了下面的，上面我们就可以直接套下面的函数，调用时K=1即可～
- 6、删除排序链表中的重复元素 [https://leetcode-cn.com/problems/remove-duplicates-from-sorted-list/](https://leetcode-cn.com/problems/remove-duplicates-from-sorted-list/)

给定一个排序链表，删除所有重复的元素，使得每个元素只出现一次。

示例 1:

输入: 1->1->2

输出: 1->2

示例 2:

输入: 1->1->2->3->3

输出: 1->2->3
这道题目十分基础，可以直接来，代码如下：

```
public ListNode deleteDuplicates(ListNode head) {
        if (head==null|| head.next==null)
            return head;

        ListNode pre = head,cur = head.next;
        while (cur!=null){
            if (cur.val==pre.val){
                pre.next = cur.next;
                cur = cur.next;
            }else {
                pre = cur;
                cur = cur.next;
            }

        }
        return head;

    }
```
- 7、删除排序链表中的重复元素 II [https://leetcode-cn.com/problems/remove-duplicates-from-sorted-list-ii/](https://leetcode-cn.com/problems/remove-duplicates-from-sorted-list-ii/)

给定一个排序链表，删除所有含有重复数字的节点，只保留原始链表中 没有重复出现 的数字。

示例 1:

输入: 1->2->3->3->4->4->5

输出: 1->2->5

示例 2:

输入: 1->1->1->2->3

输出: 2->3
这里和上一题不一样的地方在于，这里要求把出现了重复数字的节点都去掉，这里我们为了方便，用两个变量一个用来记录之前重复的数值，一个用来判断之前是不是有重复了值，避免 same_value的默认值与node节点值相混淆

```
int  same_value = -1;
        boolean pre_same = false;
```

代码如下；

```
if (head==null||head.next==null){
            return head;
        }
        ListNode new_head = new ListNode(-1);
        new_head.next = head;

        ListNode pre = new_head;
        ListNode cur = new_head.next;
        int  same_value = -100;
        boolean pre_same = false;
        while (cur!=null){
            if ((cur.val==same_value&&pre_same)||(cur.next!=null&& cur.val==cur.next.val)){
                same_value = cur.val;
                pre_same = true;
                cur = cur.next;
                pre.next = cur;
            }else {
                pre_same = false;
                pre = cur;
                cur = cur.next;
            }
        }


        return new_head.next;
```

还有一种在leetcode里面表现更好的做法，如下：

```
public ListNode deleteDuplicates(ListNode head) {
        if (head==null||head.next==null)
            return head;
        ListNode new_head = new ListNode(-1);
        ListNode start = new_head;
        ListNode pre = new_head,cur = head,next_node = cur.next;
        while (next_node!=null){
            if ((pre==new_head||pre.val!=cur.val)&&cur.val!=next_node.val){
                start.next = cur;
                start = start.next;
                start.next = null;
            }
            pre = cur;
            cur = next_node;
            next_node = next_node.next;
        }
        if (pre==new_head||pre.val!=cur.val){
            start.next = cur;
        }
        return new_head.next;
    }
```

这种做法呢，利用三个变量记录前一个位置，当前位置，后一个位置，即，pre、cur、next_node三个变量，记得在一开始的时候new_head.next 为null的，然后每次在发现前后不一致的时候，即不会重复的元素时，进行连接，更新start当前位置后，并且把next的对象置为空。十分巧妙～











