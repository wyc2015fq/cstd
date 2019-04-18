# List的两种实现对比 - weixin_33985507的博客 - CSDN博客
2016年04月03日 00:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
## 前言
> 
在日常的java开发中，我们经常用到各种集合类，而List是其中最常见的一种；以前我们在使用数组的时候，无论是c++或者java，都要指定它的大小；而List居然不用指定大小，太神奇了，接下来让我们仔细分析一下它们的实现
## LinkedList：
- 主要成员变量：
```
transient int size = 0; //List长度
transient Node<E> first;//头指针
transient Node<E> last;//尾指针
```
- add方法(将元素放入List尾部)：
```
public boolean add(E e) {
    linkLast(e);
    return true;
}
```
再看linkLast方法：
```
void linkLast(E e) {
    final Node<E> l = last;
    final Node<E> newNode = new Node<>(l, e, null);
    last = newNode;
    if (l == null)
        first = newNode;
    else
        l.next = newNode;
    size++;
    modCount++;
}
```
> 
也就是说，每次添加一个节点到末尾时，都是新申请一个链表节点，然后再缝缝补补，把相关的链表指针接在一起；复杂度为O(1);
再看看另外一个add方法：
- add(int index, E element)(将元素插入到某个下标位置):
```
public void add(int index, E element) {
    checkPositionIndex(index);
    if (index == size)
        linkLast(element);
    else
        linkBefore(element, node(index));
}
```
<p>
再看node()方法(找到下标对应的链表节点):
```
Node<E> node(int index) {
    // assert isElementIndex(index);
    if (index < (size >> 1)) {
        Node<E> x = first;
        for (int i = 0; i < index; i++)
            x = x.next;
        return x;
    } else {
        Node<E> x = last;
        for (int i = size - 1; i > index; i--)
            x = x.prev;
        return x;
    }
}
```
其寻址复杂度是O(n)的,唯一的优化点就在于，看下标离头指针近还是离尾指针近，复杂度做了常数上的优化，从n下降到n/2, 但还是O(n)；
剩下的删查都涉及到寻址的时间损耗，复杂度也都是O(n)，它们的实现都是类似的，就不赘诉了。
> 
小优化：其实如果我们自己实现一个LinkedList，完全可以把指针信息加在节点上，这样就可以避免一些寻址上的时间损耗了。
## ArrayList（与c++里的vector类似）
- 成员变量：
```
transient Object[] elementData; // 存储元素的载体是数组
private int size;//List当前长度
```
- 主构造方法：
```
public ArrayList(int initialCapacity) {
    if (initialCapacity > 0) {
        this.elementData = new Object[initialCapacity];
    } else if (initialCapacity == 0) {
        this.elementData = EMPTY_ELEMENTDATA;
    } else {
        throw new IllegalArgumentException("Illegal Capacity: "+
                                           initialCapacity);
    }
}
没有什么特别的，就是申请一个定长的数组
```
- add(E e)方法:
```
public boolean add(E e) {
    ensureCapacityInternal(size + 1);  // Increments modCount!!
    elementData[size++] = e;
    return true;
}
```
第一行是确保数组载体容量足够，后边就是把下标为size+1的元素赋值成e；
它调用了ensureCapacityInternal()方法：
```
private void ensureCapacityInternal(int minCapacity) {
    if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
        minCapacity = Math.max(DEFAULT_CAPACITY, minCapacity);
    }
    ensureExplicitCapacity(minCapacity);
}
```
又调用了ensureExplicitCapacity()方法：
```
private void ensureExplicitCapacity(int minCapacity) {
    modCount++;
    // overflow-conscious code
    if (minCapacity - elementData.length > 0)
        grow(minCapacity);
}
```
再看grow()方法：
```
private void grow(int minCapacity) {
    // overflow-conscious code
    int oldCapacity = elementData.length;
    int newCapacity = oldCapacity + (oldCapacity >> 1);
    if (newCapacity - minCapacity < 0)
        newCapacity = minCapacity;
    if (newCapacity - MAX_ARRAY_SIZE > 0)
        newCapacity = hugeCapacity(minCapacity);
    // minCapacity is usually close to size, so this is a win:
    elementData = Arrays.copyOf(elementData, newCapacity);
}
```
里边关键的一句代码就是：
```
int newCapacity = oldCapacity + (oldCapacity >> 1);
```
> 
意思就是，新的数组容量变成原数组的大约1.5倍（看不懂的可以去了解一下位运算），新的数组申请内存之后，再把老的数组里的元素全copy到新数组就完成了；
那么为什么每次容量不足的时候，要扩充到1.5倍呢？通过分析我们可以发现，每次扩充1.5倍容量，那么假设最后容量扩充为n，那么总体上的申请空间的数量近似于：f(n) = n + n * 2/3 + n * (2/3)^2 + n * (2/3)^3 + ... ，根据等比数列公式可以知道，这个表达式的值为： f(n) = 3 * n = O(n)
采用倍增的方法扩容，优点在于总体的复杂度数量级是线性的，但是也不可避免的可能会有空间浪费；在最极端的情况下，会有接近1/3的空间是没有利用上的；因此，在List会很大，且能预先大致估算出List会有多大的前提下，为了减少系统的内存消耗和频繁GC，应尽量使用以下构造方法来申请List：
```
public ArrayList(int initialCapacity);
```
- add(int index, E element)方法（将元素插入到指定下标）:
```
public void add(int index, E element) {
    rangeCheckForAdd(index);
    ensureCapacityInternal(size + 1);  // Increments modCount!!
    System.arraycopy(elementData, index, elementData, index + 1,
                     size - index);
    elementData[index] = element;
    size++;
}
```
跟add(E element)方法的最大的差别就在于，在赋值之前，要先把该下标以及后面的元素全都向后移一位，其复杂度为O(n);
- 接下来看get(int index)方法：
```
public E get(int index) {
    rangeCheck(index);
    return elementData(index);
}
```
检查下标合法性，再调用elementData():
```
E elementData(int index) {
    return (E) elementData[index];
}
```
直接是数组的根据下标随机访问的操作，复杂度是O(1)，再强转成E类型就返回了;
- remove(int index)方法：
```
public E remove(int index) {
    rangeCheck(index);
    modCount++;
    E oldValue = elementData(index);
    int numMoved = size - index - 1;
    if (numMoved > 0)
        System.arraycopy(elementData, index+1, elementData, index,
                         numMoved);
    elementData[--size] = null; // clear to let GC do its work
    return oldValue;
}
```
在删除指定下标的元素时，如果这个元素不是最后一个，那么要将后面的元素全部向前移一位，复杂度也是O(n), 另外还有一点就是，删除了这个元素之后，要把最大下标的那个元素赋值成null，方便系统进行GC（系统GC时，会根据对象是否被引用来判断对象是否可以回收）
- remove(Object o)方法：
```
public boolean remove(Object o) {
    if (o == null) {
        for (int index = 0; index < size; index++)
            if (elementData[index] == null) {
                fastRemove(index);
                return true;
            }
    } else {
        for (int index = 0; index < size; index++)
            if (o.equals(elementData[index])) {
                fastRemove(index);
                return true;
            }
    }
    return false;
}
```
定位到元素位置复杂度为O(n), fastRemove(index)的复杂度也是O(n);
另外，初看这段代码，感觉写得有点“啰嗦”，但实际上确实是有必要的：List元素允许为空，所以要特判该元素是否为空，为空时直接用==null来判断，而不为空时，才能调用equals方法进行比较，否则会有空指针异常；所以这段代码也是JDK严谨性的一种体现；
- 以下表格列出了两者在各种操作下的复杂度：
> 
| 操作\List实现类        | LinkedList           | ArrayList   |
| ------------- |:-------------:| -----:|
|   add(E e)    | O(1) | O(1) |
|   add(int index, E element)   | O(n)      |   O(n) |
|  get(int index) | O(n) | O(1) |
| remove(int index) | O(n) | O(n) |
| remove(Object o) | O(n) | O(n) |
从上表看，LinkedList几乎没有比ArrayList优越的地方；另外，LinkedList相比ArrayList没有0.5倍的空间浪费，但是其每个节点都有前后指针的内存占用，且每次新增一个元素时都要新申请一个Node，而ArrayList则是一次性批量申请；所以，当List长度比较大的时候，肯定是ArrayList效率比较高。
如果说LinkedList有优点的话，可能就是它不需要申请连续的内存，所以建议大家除了极端情况，大部分时候都使用ArrayList。
