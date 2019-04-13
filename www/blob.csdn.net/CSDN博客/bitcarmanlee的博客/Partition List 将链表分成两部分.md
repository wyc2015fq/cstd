
# Partition List 将链表分成两部分 - bitcarmanlee的博客 - CSDN博客


2019年03月10日 17:44:33[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：54标签：[PartitionList																](https://so.csdn.net/so/search/s.do?q=PartitionList&t=blog)[链表分为两部分																](https://so.csdn.net/so/search/s.do?q=链表分为两部分&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=PartitionList&t=blog)个人分类：[interview																](https://blog.csdn.net/bitcarmanlee/article/category/6156039)



## 1.问题
给定一个链表和一个特定值 x，对链表进行分隔，使得所有小于 x 的节点都在大于或等于 x 的节点之前。
你应当保留两个分区中每个节点的初始相对位置。
示例:
输入: head = 1->4->3->2->5->2, x = 3
输出: 1->2->2->4->3->5
## 2.解法
将所有小于给定值的节点组成一个新的链表A，将所有大于等于给定值的节点组成一个链表B，然后将B链表加入A链表后面即可。
`public class PartitionList {
    public static ListNode initList() {
        ListNode dummy = new ListNode(-1);
        ListNode current = dummy;
        int[] nums = {1, 4, 3, 2, 5, 2};
        for(int num: nums) {
            ListNode tmp = new ListNode(num);
            current.next = tmp;
            current = current.next;
        }
        return dummy;
    }
    public static ListNode partition(ListNode<Integer> dummy, int k) {
        ListNode<Integer> dummy1 = new ListNode(-1);
        ListNode<Integer> dummy2 = new ListNode(-1);
        ListNode current1 = dummy1;
        ListNode current2 = dummy2;
        ListNode<Integer> current = dummy.next;
        while (current != null) {
            if (current.data < k) {
                current1.next = current;
                current1 = current1.next;
            } else {
                current2.next = current;
                current2 = current2.next;
            }
            current = current.next;
        }
        // 防止多出来最后一个节点
        current1.next = null;
        current2.next = null;
        ListNode tmp = dummy2.next;
        while(tmp != null) {
            current1.next = tmp;
            current1 = current1.next;
            tmp = tmp.next;
        }
        return dummy1;
    }

    public static void main(String[] args) {
        ListNode dummy = initList();
        ListNode dummy1 = partition(dummy, 3);
        ListNode result = dummy1.next;
        ListNode.printListNode(result);
    }
}``public class ListNode<T> {
    public T data;
    public ListNode<T> next;
    public ListNode(T data) {
        this.data = data;
        this.next = null;
    }
    public static void printListNode(ListNode head) {
        while (head != null) {
            System.out.print(head.data + " ");
            head = head.next;
        }
    }
    public static ListNode initLinkList() {
        ListNode head = new ListNode(1);
        ListNode current = head;
        for(int i=2; i<=10; i++) {
            ListNode tmp = new ListNode(i);
            current.next = tmp;
            current = current.next;
        }
        return head;
    }
}`

