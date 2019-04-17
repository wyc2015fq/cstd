# [LeetCode] Design Linked List 设计链表 - Grandyang - 博客园







# [[LeetCode] Design Linked List 设计链表](https://www.cnblogs.com/grandyang/p/10005504.html)







Design your implementation of the linked list. You can choose to use the singly linked list or the doubly linked list. A node in a singly linked list should have two attributes: `val` and `next`. `val` is the value of the current node, and `next` is a pointer/reference to the next node. If you want to use the doubly linked list, you will need one more attribute `prev` to indicate the previous node in the linked list. Assume all nodes in the linked list are 0-indexed.

Implement these functions in your linked list class:
- get(index) : Get the value of the `index`-th node in the linked list. If the index is invalid, return `-1`.
- addAtHead(val) : Add a node of value `val` before the first element of the linked list. After the insertion, the new node will be the first node of the linked list.
- addAtTail(val) : Append a node of value `val` to the last element of the linked list.
- addAtIndex(index, val) : Add a node of value `val` before the `index`-th node in the linked list. If `index` equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted.
- deleteAtIndex(index) : Delete the `index`-th node in the linked list, if the index is valid.

Example:
MyLinkedList linkedList = new MyLinkedList();
linkedList.addAtHead(1);
linkedList.addAtTail(3);
linkedList.addAtIndex(1, 2);  // linked list becomes 1->2->3
linkedList.get(1);            // returns 2
linkedList.deleteAtIndex(1);  // now the linked list is 1->3
linkedList.get(1);            // returns 3

Note:
- All values will be in the range of `[1, 1000]`.
- The number of operations will be in the range of `[1, 1000]`.
- Please do not use the built-in LinkedList library.



这道题让我们实现一个链表的数据结构，说我们不能使用现成的链表数据结构，需要自己定义结点，说是可以实现双向或者单向的链表。既然有的选，那肯定选简单的单项链表了。首先就是要先自己定义一个结点的数据结构了，好在LeetCode中有很多很多的链表有关的题目，随便打开一个，并且参考一下结点的定义即可。然后看需要实现的哪些函数，分别是根据坐标取结点，分别在链表开头和末尾加结点，根据坐标位置加结点，根据坐标位置删除结点。既然是结点组成的链表，那么肯定不能向数组那样可以根据坐标直接访问元素。那么我们肯定至少要知道表头的位置。同时，在根据链表取结点函数说明了给定的位置可能是非法的，那么我们也要知道链表中所有元素的个数，这样可以快速的判定给定的位置是否合法。

好，下面来看每个函数如何实现。首先来看根据坐标取结点函数，先判定index是否合法，然后从表头向后移动index个位置，找到要返回的结点即可。对于增加表头函数就比较简单了，新建一个头结点，next连上head，然后head重新指向这个新结点，同时size自增1。同样，对于增加表尾结点函数，首先遍历到表尾，然后在之后连上一个新建的结点，同时size自增1。下面是根据位置来加结点，那么肯定还是先来判定index是否合法，然后再处理一个corner case，就是当index为0的时候，直接调用前面的表头加结点函数即可。然后就是往后遍历index-1个结点，这里为啥要减1呢，因为要加入结点的话，必须要知道加入位置前面一个结点才行，链表加入结点的问题之前的题目中做过很多，这里就不说细节了，最后size还是要自增1。根据位置删除结点也是大同小异，没有太大的难度，参见代码如下：



解法一：

```
class MyLinkedList {
public:
    /** Initialize your data structure here. */
    MyLinkedList() {
        head = NULL;
        size = 0;
    }
    
    /** Get the value of the index-th node in the linked list. If the index is invalid, return -1. */
    int get(int index) {
        if (index < 0 || index >= size) return -1;
        Node *cur = head;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->val;
    }
    
    /** Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list. */
    void addAtHead(int val) {
        Node *t = new Node(val, head);
        head = t;
        ++size;
    }
    
    /** Append a node of value val to the last element of the linked list. */
    void addAtTail(int val) {
        Node *cur = head;
        while (cur->next) cur = cur->next;
        cur->next = new Node(val, NULL);
        ++size;
    }
    
    /** Add a node of value val before the index-th node in the linked list. If index equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted. */
    void addAtIndex(int index, int val) {
        if (index < 0 || index > size) return;
        if (index == 0) {addAtHead(val); return;}
        Node *cur = head;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        Node *t = new Node(val, cur->next);
        cur->next = t;
        ++size;
    }
    
    /** Delete the index-th node in the linked list, if the index is valid. */
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size) return;
        if (index == 0) {
            head = head->next;
            --size;
            return;
        }
        Node *cur = head;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        cur->next = cur->next->next;
        --size;
    }
    
private:
    struct Node {
        int val;
        Node *next;
        Node(int x, Node* n): val(x), next(n) {}
    };
    Node *head, *tail;
    int size;
};
```



我们可以对上面的解法做一丢丢的优化，主要在末尾添加结点函数addAtTail()上，如果要大量调用这个方法的话，每次都要遍历到链表末尾，很不高效。于是我们可以同时记录表头和表尾的位置，这样我们就可以直接访问末尾结点了，缺点是在其他一些地方如果末尾元素改变了，要更新tail指针，否则就会出错，参见代码如下：



解法二：

```
class MyLinkedList {
public:
    /** Initialize your data structure here. */
    MyLinkedList() {
        head = NULL; tail = NULL;
        size = 0;
    }
    
    /** Get the value of the index-th node in the linked list. If the index is invalid, return -1. */
    int get(int index) {
        if (index < 0 || index >= size) return -1;
        Node *cur = head;
        for (int i = 0; i < index; ++i) cur = cur->next;
        return cur->val;
    }
    
    /** Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list. */
    void addAtHead(int val) {
        Node *t = new Node(val, head);
        head = t;
        if (size == 0) tail = t;
        ++size;
    }
    
    /** Append a node of value val to the last element of the linked list. */
    void addAtTail(int val) {
        Node *t = new Node(val, NULL);
        if (size == 0) {tail = t; head = t;}
        tail->next = t;
        tail = t;
        ++size;
    }
    
    /** Add a node of value val before the index-th node in the linked list. If index equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted. */
    void addAtIndex(int index, int val) {
        if (index < 0 || index > size) return;
        if (index == 0) {addAtHead(val); return;}
        if (index == size) {addAtTail(val); return;}
        Node *cur = head;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        Node *t = new Node(val, cur->next);
        cur->next = t;
        ++size;
    }
    
    /** Delete the index-th node in the linked list, if the index is valid. */
    void deleteAtIndex(int index) {
        if (index < 0 || index >= size) return;
        if (index == 0) {
            head = head->next;
            --size;
            return;
        }
        Node *cur = head;
        for (int i = 0; i < index - 1; ++i) cur = cur->next;
        cur->next = cur->next->next;
        if (index == size - 1) tail = cur;
        --size;
    }
    
private:
    struct Node {
        int val;
        Node *next;
        Node(int x, Node* n): val(x), next(n) {}
    };
    Node *head, *tail;
    int size;
};
```



最后来看一种很简洁的方法，用到了内置的双向队列deque这个数据结构，很难说这样算不算cheating，但是巨简洁，大爱之～ 参见代码如下：



解法三：

```
class MyLinkedList {
public:
    /** Initialize your data structure here. */
    MyLinkedList() {}
    
    /** Get the value of the index-th node in the linked list. If the index is invalid, return -1. */
    int get(int index) {
        return (index >= 0 && index < data.size()) ? data[index] : -1;
    }
    
    /** Add a node of value val before the first element of the linked list. After the insertion, the new node will be the first node of the linked list. */
    void addAtHead(int val) {
        data.push_front(val);
    }
    
    /** Append a node of value val to the last element of the linked list. */
    void addAtTail(int val) {
        data.push_back(val);
    }
    
    /** Add a node of value val before the index-th node in the linked list. If index equals to the length of linked list, the node will be appended to the end of linked list. If index is greater than the length, the node will not be inserted. */
    void addAtIndex(int index, int val) {
        if (index < 0 || index > data.size()) return;
        data.insert(data.begin() + index, val);
    }
    
    /** Delete the index-th node in the linked list, if the index is valid. */
    void deleteAtIndex(int index) {
        if (index < 0 || index >= data.size()) return;
        data.erase(data.begin() + index);
    }
    
private:
    deque<int> data;
};
```



参考资料：

[https://leetcode.com/problems/design-linked-list/](https://leetcode.com/problems/design-linked-list/)

[https://leetcode.com/problems/design-linked-list/discuss/154116/C%2B%2B-deque](https://leetcode.com/problems/design-linked-list/discuss/154116/C%2B%2B-deque)

[https://leetcode.com/problems/design-linked-list/discuss/145380/C%2B%2B-SOLUTION-24ms](https://leetcode.com/problems/design-linked-list/discuss/145380/C%2B%2B-SOLUTION-24ms)

[https://leetcode.com/problems/design-linked-list/discuss/150999/C%2B%2B-simple-solution-beats-97.27!](https://leetcode.com/problems/design-linked-list/discuss/150999/C%2B%2B-simple-solution-beats-97.27!)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












