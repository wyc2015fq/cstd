# 链表的相关操作（2）_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月14日 17:52:57[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：31
个人分类：[链表																[数据结构和算法](https://blog.csdn.net/zhuzhuxia183/article/category/8608092)](https://blog.csdn.net/zhuzhuxia183/article/category/8609128)

所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)








# 再来一波关于单链表的骚操作，嘻嘻嘻

继续来复习链表的操作～

上一篇的链接 [https://blog.csdn.net/zhuzhuxia183/article/details/86439006](https://blog.csdn.net/zhuzhuxia183/article/details/86439006)
- 8、复制带随机指针的链表 [https://leetcode-cn.com/problems/copy-list-with-random-pointer/](https://leetcode-cn.com/problems/copy-list-with-random-pointer/)

给定一个链表，每个节点包含一个额外增加的随机指针，该指针可以指向链表中的任何节点或空节点。

要求返回这个链表的深度拷贝。
这个题目，中规中矩的实现方法，其实也是不难的，上代码：

```
public RandomListNode copyRandomList1(RandomListNode head) {
        if (head==null)
            return null;
        RandomListNode root  = new RandomListNode(head.label);
        ArrayList<RandomListNode> list = new ArrayList<>();
        list.add(head);

        ArrayList<RandomListNode> new_list = new ArrayList<>();
        new_list.add(root);

        RandomListNode temp = head;
        RandomListNode temp_new = root;
        while (head.next!=null){
            root.next  = new RandomListNode(head.next.label);
            new_list.add(root.next);
            list.add(head.next);
            head = head.next;
            root = root.next;
        }
        root = temp_new;
        while (temp!=null){
            if (temp.random!=null)
                temp_new.random = new_list.get(list.indexOf(temp.random));
            temp = temp.next;
            temp_new = temp_new.next;

        }

        return root;
    }
```

上面的代码可以说还是十分直接浅显的，主要是利用了一个List结构来存储所有的node，然后在给random赋值时可以通过两个两个list中的元素对应位置一直来获取的～

但是还有一种更好的解决办法，可以更好解决如何复制 random节点，即利用先在原链表上给每个节点进行复制添加，之后在复制random节点时便可以比较轻松的定位，最后再把链表切割成两个部分即可。上代码：

```
public RandomListNode copyRandomList(RandomListNode head) {
       if(head==null)
            return null;

        RandomListNode source_head = head;
        RandomListNode new_head=null;

        //next添加
        while (source_head!=null){
            new_head = new RandomListNode(source_head.label);
            new_head.next = source_head.next;
            source_head.next = new_head;
            source_head = new_head.next;
        }
        
		//random节点赋值
        source_head = head;
        while (source_head!=null){
            if (source_head.random!=null){
                source_head.next.random = source_head.random.next;
            }
            source_head = source_head.next.next;
        }

        //进行切断
        source_head = head;
        new_head = head.next;
        RandomListNode new_head_node = head.next;
        while (source_head!=null){
            new_head_node = source_head.next;
            source_head.next = source_head.next.next;
            if (new_head_node.next!=null){
                new_head_node.next = new_head_node.next.next;
            }
            source_head = source_head.next;
        }
        return new_head;
    }
```
- 9、环形链表 [https://leetcode-cn.com/problems/linked-list-cycle/](https://leetcode-cn.com/problems/linked-list-cycle/)

给定一个链表，判断链表中是否有环。

为了表示给定链表中的环，我们使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。 如果 pos 是 -1，则在该链表中没有环。
这个问题，对于有没有环这个问题，我们可以利用一块一慢两个指针是否会相遇来进行判断，当然在快指针不会为null的情况下，代码如下：

```
public boolean hasCycle(ListNode head) {
		if(head==null||head.next==null)
            return false;

        ListNode node_slow = head;
        ListNode node_fast = head.next;
        while (node_slow!=node_fast){
            if (node_fast==null||node_fast.next==null)
                return false;
            node_slow = node_slow.next;
            node_fast = node_fast.next.next;
        }
        return true;
   }
```
- 10、环形链表 II [https://leetcode-cn.com/problems/linked-list-cycle-ii/](https://leetcode-cn.com/problems/linked-list-cycle-ii/)

给定一个链表，返回链表开始入环的第一个节点。 如果链表无环，则返回 null。

为了表示给定链表中的环，我们使用整数 pos 来表示链表尾连接到链表中的位置（索引从 0 开始）。 如果 pos 是 -1，则在该链表中没有环。

说明：不允许修改给定的链表。
这个题目，相对之前的题目，可以说，是加大了难度了，对于如何求出那个环所在的位置，我们可以这么想，对于慢指针和快指针来说，他们相遇之时，快指针一定是比慢指针多走了n个环，这个肯定是对的，而且他们相遇的位置，一定在如环口后面，然后我们有从开始到环所在的位置，肯定是等于 n * 环的长度 - (相遇点-入环口)，如果从相遇点出发，对应这么长度的点，恰好是入环口，emmmm，虽然有些绕，也是可以用公式来证明的，但是我本人觉得这种题目如果到了非要用公式来证明，其实思路可以说是不直接，但是按照我这种来思考，其实很快就可以相同的，相信我嘛·～

所以代码就出来啦：

```
public ListNode detectCycle(ListNode head) {
        if(head==null||head.next==null)
            return null;
        ListNode node_slow = head.next;
        ListNode node_fast = head.next.next;
        while (node_slow!=node_fast){
            if (node_fast==null||node_fast.next==null){
                return null;
            }
            node_slow = node_slow.next;
            node_fast = node_fast.next.next;
        }

        //这个点就是相遇点
        ListNode node = head;

        while (node!=node_slow){
            node = node.next;
            node_slow = node_slow.next;
        }
        return node;
    }
```
- 11、重排链表 [https://leetcode-cn.com/problems/reorder-list/](https://leetcode-cn.com/problems/reorder-list/)

给定一个单链表 L：L0→L1→…→Ln-1→Ln ，

将其重新排列后变为： L0→Ln→L1→Ln-1→L2→Ln-2→…

你不能只是单纯的改变节点内部的值，而是需要实际的进行节点交换。

示例 1:

给定链表 1->2->3->4, 重新排列为 1->4->2->3.

示例 2:

给定链表 1->2->3->4->5, 重新排列为 1->5->2->4->3.
这个题目，如果用一些队列的数据结构做，emmm，感觉分分钟的事，但是如果不用呢，给出的思路是：首先把链表分成两个部分，用快慢指针，弄成前一半和后一半，然后把后一半反转，然后两个列表进行合并即可～jio得还行

```
public void reorderList(ListNode head) {
		if (head==null||head.next==null)
            return;

        ListNode slow = head,fast = head;
        while (fast.next!=null && fast.next.next!=null){
            slow = slow.next;
            fast = fast.next.next;
        }

        ListNode end =new ListNode(-1);

        if (fast.next==null){
            fast.next = end;
        }else {
            fast.next.next = end;
        }

        ListNode pre = slow;
        ListNode first = slow.next;
        ListNode cur = first.next;
        while (cur!=end){
            first.next = cur.next;
            cur.next = pre.next;

            pre.next = cur;
            cur = first.next;
        }


        first.next = null;

        ListNode node = pre.next;
        pre.next = null;


        ListNode temp = null;

        while (node!=null){
            temp = node.next;
            node.next = head.next;
            head.next = node;
            head = node.next;
            node = temp;
        }
   }
```

很容易写错，呜～，大家也要注意，理清楚思路，一步一步来吧

剩下的继续舔舔补补吧～








