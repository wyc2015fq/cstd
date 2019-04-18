# Cracking the coding interview--Q2.4 - weixin_33985507的博客 - CSDN博客
2013年07月30日 17:02:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# [Cracking the coding interview--Q2.4](http://hawstein.com/posts/2.4.html)
December 16, 2012
 作者：Hawstein
  出处：[http://hawstein.com/posts/2.4.html](http://hawstein.com/posts/2.4.html)
  声明：本文采用以下协议进行授权： [自由转载-非商用-非衍生-保持署名|Creative Commons BY-NC-ND 3.0](http://creativecommons.org/licenses/by-nc-nd/3.0/deed.zh) ，转载请注明作者及出处。
## 题目
原文：
You have two numbers represented by a linked list, where each node
contains a single digit. The digits are stored in reverse order,
such that the 1’s digit is at the head of the list. Write a function
that adds the two numbers and returns the sum as a linked list.
EXAMPLE
Input: (3 –> 1 –> 5), (5 –> 9 –> 2)
Output: 8 –> 0 –> 8
译文：
你有两个由单链表表示的数。每个结点代表其中的一位数字。数字的存储是逆序的，
也就是说个位位于链表的表头。写一函数使这两个数相加并返回结果，结果也由链表表示。
例子：(3 –> 1 –> 5), (5 –> 9 –> 2)
输入：8 –> 0 –> 8
## 解答
这道题目并不难，需要注意的有：1.链表为空。2.有进位。3.链表长度不一样。
代码如下：
```
package cha2;
public class B024 {
    
    static class Node {
        int value;
        Node next;
        public Node() {
            value = -1;
            next = null;
        }
        public Node(int value, Node next) {
            this.value = value;
            this.next = next;
        }
    }
    
    public static Node add(Node r1, Node r2) {
        int carry = 0;
        Node sum = new Node(0,null);
        if (r1 == null)
            return r2;
        if (r2 == null)
            return r1;
        Node tmp = sum;
        Node end = new Node(0, null);
        while (r1!=null && r2!=null) {
            tmp.value = ( r1.value + r2.value + carry ) % 10;
            carry = ( r1.value + r2.value + carry ) / 10;
            Node tn = new Node(0, null);
            tmp.next = tn;
            r1 = r1.next;
            r2 = r2.next;
            end = tmp;
            tmp = tn;
        }
        if (r1==null && r2 == null) {
            if (carry == 0)
                tmp = null;
            else
                tmp.value = 1;
        }
        else {
            end.next = (r1 != null) ? r1 : r2;
            tmp = end.next;
            while (carry == 1) {
                tmp.value = tmp.value + carry;
                carry = tmp.value  / 10;
                tmp.value = tmp.value % 10;
                if (tmp.next != null)
                    tmp = tmp.next;
                else {
                    Node tn = new Node(0, null);
                    tmp.next = tn;
                    tmp = tn;
                }
            }
        }
        return sum;
    }
    
    public static void main(String[] args) {
        Node t2 = new Node(9,null);
        Node t1 = new Node(9,t2);
        Node r1 = new Node(6, t1);
        Node r2 = new Node(7, null);
        Node sum = add(r1, r2);
        while (sum!=null) {
            System.out.print(sum.value);
            sum = sum.next;
        }
    }
}
```
