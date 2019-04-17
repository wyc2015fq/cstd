# 数据结构与算法（一）:线性表、栈、树(二叉树，AVL树)、图 - Machine Learning with Peppa - CSDN博客





2018年01月08日 21:55:03[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：454
所属专栏：[C语言入门](https://blog.csdn.net/column/details/18792.html)[有趣的算法](https://blog.csdn.net/column/details/19022.html)









数据结构是以某种形式将数据组织在一起的集合，它不仅存储数据，还支持访问和处理数据的操作。算法是为求解一个问题需要遵循的、被清楚指定的简单指令的集合。下面是自己整理的常用数据结构与算法相关内容，如有错误，欢迎指出。

为了便于描述，文中涉及到的代码部分都是用Java语言编写的，其实Java本身对常见的几种数据结构，线性表、栈、队列等都提供了较好的实现，就是我们经常用到的Java集合框架，有需要的可以阅读这篇文章。Java - 集合框架完全解析

一、线性表 

  1.数组实现 

  2.链表 

二、栈与队列 

三、树与二叉树 

  1.树 

  2.二叉树基本概念 

  3.二叉查找树 

  4.平衡二叉树 

  5.红黑树 

四、图 

五、总结 

一、线性表 

线性表是最常用且最简单的一种数据结构，它是n个数据元素的有限序列。
实现线性表的方式一般有两种，一种是使用数组存储线性表的元素，即用一组连续的存储单元依次存储线性表的数据元素。另一种是使用链表存储线性表的元素，即用一组任意的存储单元存储线性表的数据元素（存储单元可以是连续的，也可以是不连续的）。

数组实现 

数组是一种大小固定的数据结构，对线性表的所有操作都可以通过数组来实现。虽然数组一旦创建之后，它的大小就无法改变了，但是当数组不能再存储线性表中的新元素时，我们可以创建一个新的大的数组来替换当前数组。这样就可以使用数组实现动态的数据结构。

代码1 创建一个更大的数组来替换当前数组 

int[] oldArray = new int[10];

int[] newArray = new int[20];

for (int i = 0; i < oldArray.length; i++) { 

    newArray[i] = oldArray[i]; 

}
// 也可以使用System.arraycopy方法来实现数组间的复制 

// System.arraycopy(oldArray, 0, newArray, 0, oldArray.length);

oldArray = newArray; 

代码2 在数组位置index上添加元素e 

//oldArray 表示当前存储元素的数组 

//size 表示当前元素个数 

public void add(int index, int e) {
```
if (index > size || index < 0) {
    System.out.println("位置不合法...");
}

//如果数组已经满了 就扩容
if (size >= oldArray.length) {
    // 扩容函数可参考代码1
}

for (int i = size - 1; i >= index; i--) {
    oldArray[i + 1] = oldArray[i];
}

//将数组elementData从位置index的所有元素往后移一位
// System.arraycopy(oldArray, index, oldArray, index + 1,size - index);

oldArray[index] = e;

size++;
```

} 

上面简单写出了数组实现线性表的两个典型函数，具体我们可以参考Java里面的ArrayList集合类的源码。数组实现的线性表优点在于可以通过下标来访问或者修改元素，比较高效，主要缺点在于插入和删除的花费开销较大，比如当在第一个位置前插入一个元素，那么首先要把所有的元素往后移动一个位置。为了提高在任意位置添加或者删除元素的效率，可以采用链式结构来实现线性表。

链表 

链表是一种物理存储单元上非连续、非顺序的存储结构，数据元素的逻辑顺序是通过链表中的指针链接次序实现的。链表由一系列节点组成，这些节点不必在内存中相连。每个节点由数据部分Data和链部分Next，Next指向下一个节点，这样当添加或者删除时，只需要改变相关节点的Next的指向，效率很高。

单链表的结构 

下面主要用代码来展示链表的一些基本操作，需要注意的是，这里主要是以单链表为例，暂时不考虑双链表和循环链表。

代码3 链表的节点 

class Node {

```
E item;
Node<E> next;

//构造函数
Node(E element) {
   this.item = element;
   this.next = null;
```

} 

} 

代码4 定义好节点后，使用前一般是对头节点和尾节点进行初始化 

//头节点和尾节点都为空 链表为空 

Node head = null; 

Node tail = null; 

代码5 空链表创建一个新节点 

//创建一个新的节点 并让head指向此节点 

head = new Node(“nodedata1”);
//让尾节点也指向此节点 

tail = head; 

代码6 链表追加一个节点 

//创建新节点 同时和最后一个节点连接起来 

tail.next = new Node(“node1data2”);
//尾节点指向新的节点 

tail = tail.next; 

代码7 顺序遍历链表 

Node current = head; 

while (current != null) { 

    System.out.println(current.item); 

    current = current.next; 

} 

代码8 倒序遍历链表 

static void printListRev(Node head) { 

//倒序遍历链表主要用了递归的思想 

    if (head != null) { 

        printListRev(head.next); 

        System.out.println(head.item); 

    } 

} 

代码 单链表反转 

//单链表反转 主要是逐一改变两个节点间的链接关系来完成 

static Node revList(Node head) {
```
if (head == null) {
    return null;
}

Node<String> nodeResult = null;

Node<String> nodePre = null;
Node<String> current = head;

while (current != null) {

    Node<String> nodeNext = current.next;

    if (nodeNext == null) {
        nodeResult = current;
    }

    current.next = nodePre;
    nodePre = current;
    current = nodeNext;
}

return nodeResult;
```

} 

上面的几段代码主要展示了链表的几个基本操作，还有很多像获取指定元素，移除元素等操作大家可以自己完成，写这些代码的时候一定要理清节点之间关系，这样才不容易出错。

链表的实现还有其它的方式，常见的有循环单链表，双向链表，循环双向链表。 循环单链表 主要是链表的最后一个节点指向第一个节点，整体构成一个链环。 双向链表 主要是节点中包含两个指针部分，一个指向前驱元，一个指向后继元，JDK中LinkedList集合类的实现就是双向链表。** 循环双向链表** 是最后一个节点指向第一个节点。

二、栈与队列 

栈和队列也是比较常见的数据结构，它们是比较特殊的线性表，因为对于栈来说，访问、插入和删除元素只能在栈顶进行，对于队列来说，元素只能从队列尾插入，从队列头访问和删除。

栈

栈是限制插入和删除只能在一个位置上进行的表，该位置是表的末端，叫作栈顶，对栈的基本操作有push(进栈)和pop(出栈)，前者相当于插入，后者相当于删除最后一个元素。栈有时又叫作LIFO(Last In First Out)表，即后进先出。

栈的模型 

下面我们看一道经典题目，加深对栈的理解。

关于栈的一道经典题目 

上图中的答案是C，其中的原理可以好好想一想。

因为栈也是一个表，所以任何实现表的方法都能实现栈。我们打开JDK中的类Stack的源码，可以看到它就是继承类Vector的。当然，Stack是Java2前的容器类，现在我们可以使用LinkedList来进行栈的所有操作。

队列

队列是一种特殊的线性表，特殊之处在于它只允许在表的前端（front）进行删除操作，而在表的后端（rear）进行插入操作，和栈一样，队列是一种操作受限制的线性表。进行插入操作的端称为队尾，进行删除操作的端称为队头。

队列示意图 

我们可以使用链表来实现队列，下面代码简单展示了利用LinkedList来实现队列类。

代码9 简单实现队列类 

public class MyQueue {

```
private LinkedList<E> list = new LinkedList<>();

// 入队
public void enqueue(E e) {
    list.addLast(e);
}

// 出队
public E dequeue() {
    return list.removeFirst();
}
```

} 

普通的队列是一种先进先出的数据结构，而优先队列中，元素都被赋予优先级。当访问元素的时候，具有最高优先级的元素最先被删除。优先队列在生活中的应用还是比较多的，比如医院的急症室为病人赋予优先级，具有最高优先级的病人最先得到治疗。在Java集合框架中，类PriorityQueue就是优先队列的实现类，具体大家可以去阅读源码。









