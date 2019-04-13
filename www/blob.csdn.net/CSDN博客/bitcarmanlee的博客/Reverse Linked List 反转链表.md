
# Reverse Linked List 反转链表 - bitcarmanlee的博客 - CSDN博客


2019年03月11日 22:50:12[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：38



## 1.题目
反转一个单链表。
示例:
输入: 1->2->3->4->5->NULL
输出: 5->4->3->2->1->NULL
## 2.解法
1.将当前节点cur的下一个节点cur.next取出来保存为tmp。
2.更改当前节点cur的下一个节点为pre，也即反转当前结点的next指针指向前。
3.将pre指针前移，更改为cur，这样下一个节点的next将指向cur。
4.将当前指针cur指向tmp。
`public class ReverseLinkedList {
    public static ListNode reverse(ListNode head) {
        ListNode pre = null, cur = head;
        while (cur != null) {
            ListNode tmp = cur.next;
            cur.next = pre; // 当前结点指向前一个节点
            pre = cur; // pre指针前移
            cur = tmp; // 将cur指针指向tmp，即下一个
        }
        head.next = null;
        return pre;
    }
    public static void main(String[] args) {
        ListNode head = ListNode.initLinkList();
        ListNode newHead = reverse(head);
        ListNode.printListNode(newHead);
    }
}
public class ListNode<T> {
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

