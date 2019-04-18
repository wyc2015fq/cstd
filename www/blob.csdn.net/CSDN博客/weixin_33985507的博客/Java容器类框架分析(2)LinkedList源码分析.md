# Java容器类框架分析(2)LinkedList源码分析 - weixin_33985507的博客 - CSDN博客
2017年11月06日 06:09:43[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
## 概述
在分析LinkedList的源码之前，先看一下ArrayList在数据结构中的位置，常见的数据结构按照逻辑结构跟存储结构可以做如下划分：
数据结构分类
先看看源码的注释：
- Doubly-linked list implementation of the {@code List} and {@code Deque}
interfaces.  Implements all optional list operations, and permits all
elements (including {@code null}).
All of the operations perform as could be expected for a doubly-linked
list.  Operations that index into the list will traverse the list from
the beginning or the end, whichever is closer to the specified index.
- LinkedList是一个实现了List接口跟Deque的双链表。实现了所有的List接口的操作，允许存放任意元素(包括空值).能够进行双链表的所有操作插入操作，LinkedList中的索引操作将会从头到尾遍历整个链表，知道找到具体的索引。
**从注释中可以看出，LinkedList是一个双向非循环链表，并且实现了Deque接口，还是一个双端队列，所以比ArrayList要复杂一些。**
## 正文
### 链表
在分析LinkedList之前我们先复习一下链表这种数据结构
> 
链表是一种物理存储单元上非连续、非顺序的存储结构，数据元素的逻辑顺序是通过链表中的指针链接次序实现的。链表由一系列结点（链表中每一个元素称为结点）组成，结点可以在运行时动态生成。每个结点包括两个部分：一个是存储数据元素的数据域，另一个是存储下一个结点地址的指针域。
链表按照指向可以分为单向链表跟双向链表，也可以按照是否循环氛分为循环链表跟非循环链表。
链表分类
#### 单向链表
单向链表
从head节点开始，next指针只想下一个节点的数据，tail节点的next指针指向null
#### 双向链表
双向链表
每个节点有连个指针，pre跟next，除了head节点pre指针跟tail节点的next指针都指向null之外，其余的相邻节点的指针不管是从头到尾还是反过来，当前节点的两个指针包含了相邻节点的指向。
#### 单向循环链表
单向循环链表
单向循环链表跟单向链表的区别在于，tail节点指向head节点的数据
#### 双向循环链表
双向循环链表
双向循环链表跟单向循环链表可以进行类比，只是把head节点的pre指针跟tail节点的next指针分别指向tail跟head的数据区域而已。
### ArrayList源码分析
先看一下ArrayList的继承关系
LinkedList的继承关系
- 虚线代表实现关系
- 实线代表继承，其中蓝色的代表类之间继承关系，绿色代表接口之间的继承关系
跟ArrayList的区别在于LinkedList实现了Deque这个接口，Deque则继承自Queue这个接口，所以LinkedList能够进行队列操作，其余的实现跟ArrayList基本一样，不再多说，下面开始分析LinkedList的源码。
#### 成员变量
```bash
//序列化
private static final long serialVersionUID = 876323262645176354L;
transient int size = 0;//元素个数
transient Node<E> first;//head结点
transient Node<E> last;//tail节点
//内部类节点
private static class Node<E> {
    E item;存储的数据
    Node<E> next;//next指针，指向下一个数据
    Node<E> prev;//pre指针，指向上一个数据
    Node(Node<E> prev, E element, Node<E> next) {
            this.item = element;
            this.next = next;
            this.prev = prev;
        }复制代码
```
#### 构造函数
- 空的构造函数(Constructs an empty list.)
```bash
public LinkedList() {
    }复制代码
```
当我们通过此构造方法进行初始化LinkedList的时候，实际上什么都没做，此时只有一个Node,data为null，pre指向null，next也指向null。
- 通过Collection来构造(Constructs a list containing the elements of the specified collection, in the order they are returned by the collection's iterator.)
```bash
//调用addAll
  public LinkedList(Collection<? extends E> c) {
        this();
        addAll(c);
    }
//紧接着调用addAll(size, c)
      public boolean addAll(Collection<? extends E> c) {
      return addAll(size, c);
    }
  //这个方法比较关键，因为不管是初始化，还是进行添加，都会调用此方法，下面重点分析一下
    public boolean addAll(int index, Collection<? extends E> c) {
         //检查index是否合法
        checkPositionIndex(index);
        Object[] a = c.toArray();
        int numNew = a.length;
        if (numNew == 0)
            return false;
        //初始化两个Node，保留下一个节点，当集合添加完成之后，需要跟此节点进行连接，构成链表
        Node<E> pred, succ;
          //插入的时候就是分两种，一种是从尾部插入，一种是从中间插入
        if (index == size) {
        //在尾部插入
            succ = null;//null值作为后面连接的一个标志
            pred = last;//将pred指向上一个节点也就是tail节点
        } else {
        //从中间插入
            succ = node(index);
            pred = succ.prev;
        }
        //遍历集合，按照顺序依次插入相应的元素
        for (Object o : a) {
            @SuppressWarnings("unchecked") 
            E e = (E) o;
            //初始化一个节点并进行赋值
            Node<E> newNode = new Node<>(pred, e, null);
            if (pred == null)
            //头结点为空，说明是空链表
                first = newNode;
            else
            //Node个数>0，当前指针指向新的节点
                pred.next = newNode;
            //移到下一个节点
            pred = newNode;
        }
     //链表添加完毕，开始从断开的地方进行连接
        if (succ == null) {
        //尾部插入进行连接，此时last需要重新赋值，即为pred节点
            last = pred;
        } else {
        //中间插入，直接讲集合的最后一个节点跟之前插入点后的节点进行连接就好
            pred.next = succ;将当前Node的next指针指向下一个节点
            succ.prev = pred;//将下一个节点的pre指向pre
        }
        size += numNew;
        modCount++;
        return true;
    }复制代码
```
结合图形来理解一下
双向链表插入元素
稍微总结一下，这个addAll实际上就是先把链表打断，然后从断的左侧进行添加一些元素，添加完成之后再将链表进行连接起来，恩，就是这个样子，归纳一下就是：
- 将链表打断，用两个节点保留插入位置的下一个节点
- 在节点插入完成之后，再进行连接
- 需要注意插入的位置：是在尾部还是中间插入，因为两者最后进行链表重连的方式不一样。
#### add方法
LinkedList的Add方法
通过查看实际上有很多，这里就不一一贴出来了，最终调用的都是这几个方法：
##### 在头部插入(Links e as first element.)
```bash
private void linkFirst(E e) {
         //拿到头结点
        final Node<E> f = first;
        //初始化一个结点，也即是新的头结点
        final Node<E> newNode = new Node<>(null, e, f);
        //将新节点赋值给头结点
        first = newNode;
        //头结点为空
        if (f == null)
        //则相当于此时只有一个节点，尾节点也是头结点
            last = newNode;
        else
        //将原先的头结点的pre指针指向新的头结点
            f.prev = newNode;
        size++;
        modCount++;
    }复制代码
```
##### 在尾部插入(Links e as last element.)
```bash
void linkLast(E e) {
         //拿到尾节点
        final Node<E> l = last;
          //初始化一个Node，也就是新的尾节点
        final Node<E> newNode = new Node<>(l, e, null);
        //将新的尾节点赋值给last
        last = newNode;
        //尾结点为空
        if (l == null)
        //此时只有一个节点，所以当前节点即是头结点也是尾节点
            first = newNode;
        else
        //将原先的尾节点指向现在的新的尾节点
            l.next = newNode;
        size++;
        modCount++;
    }复制代码
```
##### 在某个元素之前插入(Inserts element e before non-null Node succ.)
```bash
void linkBefore(E e, Node<E> succ) {
        // assert succ != null;
        //拿到要插入的元素之前节点
        final Node<E> pred = succ.prev;
        //初始化一个节点
        final Node<E> newNode = new Node<>(pred, e, succ);
        //将下一个节点的头指针指向插入的节点
        succ.prev = newNode;
        if (pred == null)
        //如果此时只有一个节点，那么它既是尾节点也是头结点
            first = newNode;
        else
         //将插入的节点跟前一个节点进行连接
            pred.next = newNode;
        size++;
        modCount++;
    }复制代码
```
#### remove操作
有如下几个方法
Remove操作
跟add操作相对应，也只是改变相应的链表的指向而已，我们选择一个来看看：
```bash
public E remove(int index) {
        checkElementIndex(index);//检查删除的索引值
        return unlink(node(index));//删除节点
    }
       E unlink(Node<E> x) {
        // assert x != null;
        //拿到需要删除的节点
        final E element = x.item;
        //获取删除节点的下一个节点
        final Node<E> next = x.next;
         //获取删除节点的上一个节点
        final Node<E> prev = x.prev;
        if (prev == null) {
        //头结点，删除之后，头结点后移
            first = next;
        } else {
        //将删除节点的前一个节点的next指向后一个节点
            prev.next = next;
            x.prev = null;
        }
        if (next == null) {
        //尾结点，删除之后，尾节点前移
            last = prev;
        } else {
        //将删除节点的后一个节点的pre指向前一个节点
            next.prev = prev;
            x.next = null;
        }
        x.item = null;
        size--;
        modCount++;
        return element;
    }复制代码
```
> 
说到底还是在改变Node节点的指向而已
#### set操作
```bash
public E set(int index, E element) {
        checkElementIndex(index);//检查索引
        Node<E> x = node(index);//拿到需要修改的那个节点
        E oldVal = x.item;//拿到修改的节点的值
        x.item = element;//进行修改
        return oldVal;
    }复制代码
```
#### 查询操作
```bash
public E getFirst() {
        final Node<E> f = first;//拿到head节点
        if (f == null)
            throw new NoSuchElementException();
        return f.item;
    }
    public E getLast() {
        final Node<E> l = last;////拿到tail节点
        if (l == null)
            throw new NoSuchElementException();
        return l.item;
    }
    //获取某一个索引的节点
    public E get(int index) {
        checkElementIndex(index);
        return node(index).item;
    }
        Node<E> node(int index) {
        // assert isElementIndex(index);
        //二分查找思想进行查找
        if (index < (size >> 1)) {
        //遍历
            Node<E> x = first;
            for (int i = 0; i < index; i++)
                x = x.next;
            return x;
        } else {
        //遍历
            Node<E> x = last;
            for (int i = size - 1; i > index; i--)
                x = x.prev;
            return x;
        }
    }复制代码
```
#### contains操作
```bash
public boolean contains(Object o) {
        return indexOf(o) != -1;
    }
        public int indexOf(Object o) {
        int index = 0;
        if (o == null) {
         //遍历查找
            for (Node<E> x = first; x != null; x = x.next) {
                if (x.item == null)
                    return index;
                index++;
            }
        } else {
            for (Node<E> x = first; x != null; x = x.next) {
                if (o.equals(x.item))
                    return index;
                index++;
            }
        }
        return -1;
    }复制代码
```
没有什么好说的，就是遍历查找而已，这里会发现，LinkedList的查找很低效，需要遍历整个集合。
#### 队列操作
#### push
```bash
public void push(E e) {
        addFirst(e);
    }复制代码
```
#### offer
```bash
public boolean offer(E e) {
        return add(e);
    }复制代码
```
#### peek
```bash
public E peek() {
        final Node<E> f = first;
        return (f == null) ? null : f.item;
    }复制代码
```
#### pop
```bash
public E pop() {
        return removeFirst();
    }复制代码
```
#### poll
```bash
public E poll() {
        final Node<E> f = first;
        return (f == null) ? null : unlinkFirst(f);
    }复制代码
```
#### getFirst
```bash
public E getFirst() {
        final Node<E> f = first;
        if (f == null)
            throw new NoSuchElementException();
        return f.item;
    }复制代码
```
#### getLast
```bash
public E getLast() {
        final Node<E> l = last;
        if (l == null)
            throw new NoSuchElementException();
        return l.item;
    }复制代码
```
上面都是关于队列的一些操作，用链表也可以实现，而且操作比较简单，可以看做是队列的一种链表实现方式。
## 总结
- 底层是通过双向链表来实现的，但是并非循环链表。
- 不需要扩容，因为底层是线性存储
- 增删快，但是查找比较慢
- 非线程安全
