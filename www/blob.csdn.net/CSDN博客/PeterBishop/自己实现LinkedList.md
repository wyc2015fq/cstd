# 自己实现LinkedList - PeterBishop - CSDN博客





2018年10月17日 18:17:56[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：6
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









上节课我们自己手动实现了一个ArrayList，底层是用数组实现的。但是，这种实现的缺点显而易见，太浪费空间了。每次扩容直接扩充一倍，浪费的空间太多了，不太好。



这节课我们还是想要实现一个允许动态增删改查元素的容器，只不过这次我们换一种方式。



说到容器，我们可以想一下，我们能不能自己设计一个容器来存放对象而不是使用数组呢？

比如下面这种形式:



class Node{

Object elem;

}



我们的Node实际上就相当于一个容器，里面可以存放对象了。但是有个问题，如何通过这个容器可以访问同类型的其他容器？



很简单，我们再给它加一个成员，类型就是Node



 class Node{

Object elem;

Node next;       //指向下一个Node

}



之后我们再把必要的构造器和修饰符加上，


//类型转换太麻烦了，我们加个泛型

class Node<E>{

    //这里干脆把属性声明成public

    public E elem;

    public Node<E> next;



    public Node(){

//        elem = null;

//        next = null;

        //我们可以直接调用下面那个构造器

        this(null);

    }



    public Node(E elem)

    {

        this.elem = elem;

        next = null;

    }

}


OK,这样我们的存放元素的容器就做好了，接下来我们再来想一下:

Node<Integer> node = new Node<Integer>(1);

Node<Integer> node2 = new Node<Integer>(2);

Node<Integer> node3 = new Node<Integer>(3);



node.next = node2;

node2next = node3;



.



这个看起来是不是很像一条链？



这种结构我们就称之为链表(LinkdeList)



链表必须要有个头结点



Demo:


package helloworld;



//类型转换太麻烦了，我们加个泛型

class Node<E>{

    //这里干脆把属性声明成public

    public E elem;

    public Node<E> next;



    public Node(){

//        elem = null;

//        next = null;

        //我们可以直接调用下面那个构造器

        this(null);

    }



    public Node(E elem)

    {

        this.elem = elem;

        next = null;

    }

}

public class MyLinkedList<E> {



    private Node<E> head;   //头结点

    private Node<E> tail;   //尾结点,这个可以通过代码获取，但为了方便我就直接定义一下了



    int size = 0;



    public MyLinkedList()

    {

        head = null;

        tail = head;

    }



    //核心方法

    public void add(E e)

    {

        //添加结点的步骤:

        //写完就可以发现问题，tail一开始指向的是head，而head一开始是null，所以要先做个判断

        if (this.head == null)

        {

            this.head = new Node<>(e);

            this.tail = head;   //这里必须要让tail指向head

            size++;

            return;

        }



        */**         * 1. **将要添加的对象e封装到一个结点里         *         * 2. 把新结点添加到尾结点的末尾         *         * 3. 让尾结点指向新添加的结点         *         * 4. 别忘了size++         */        *Node<E> node = new Node<>(e);

        this.tail.next = node;

        this.tail = node;

        size++;



    }



    //将结点插入到想要的位置

    public void insert(E e , int index)

    {

        //在ArrayList里也写过这个，首先肯定是对index进行判断

        if (index < 0 || index > size)

        {

            return;

        }

        //如果head为null呢

        //由于head为null，所以可以插入的位置只有0，即调用add方法即可

        if (head == null)

        {

            add(e);

            return;

            //指向完add后直接return就行

        }



        //现在有个问题，怎么插入？

        */**         * 1. **找到想要插入位置的前一个结点         *         * 2. 将e封装成结点         *         * 3. 让e的next指向刚才找到结点的后一个结点         *         * 4. 让刚才找到的结点的next指向e         */        *//因为是找前一个，所以是index - 1

        Node<E> prevNode = getNode(index - 1);



        Node<E> node = new Node<>(e);



        node.next = prevNode.next;

        prevNode.next = node;



        //最后别忘了size++

        size++;



    }



    //删除

    public E delete(int index)

    {

        //和insert反着来就行

        Node<E> prevNode = getNode(index - 1);

        if (prevNode.next == null)

            return null;



        Node<E> node = prevNode.next;

        if (node == tail)

            tail = prevNode;    //让尾结点指向前一个结点

        prevNode.next = node.next;

        node.next = null;   //让node指向的对象彻底不在链表之中

        //不要忘了size--

        size--;



        //但还有个问题，如果删的是尾结点呢



        return node.elem;

    }



    //取数据

    public E get(int index)

    {

        Node<E> node = getNode(index);

        if (node != null)

            return node.elem;

        return null;

    }



    private Node<E> getNode(int index) {

        if (index < 0 || index >= size)

            return null;

        else

        {

            Node node = head;

            for (int i = 0; i < index; i++) {

                node = node.next;

            }

            return node;

        }

    }



    public void set(int index, E e)

    {

        Node<E> node = getNode(index);

        if (node != null)

        {

            node.elem = e;

        }

    }



    public Object[] toArray()

    {

        if (size == 0)

            return new Object[0];



        Object[] arr = new Object[size];

        Node<E> node = head;

        for (int i = 0; i < size; i++) {

            arr[i] = node.elem;

            node = node.next;

        }

        return arr;

    }



    public static void main(String[] args) {

        MyLinkedList<Integer> list = new MyLinkedList<>();

        for (int i = 0; i < 10; i++)

        {

            list.add(i);

        }

        System.*out*.println(list.get(5));

        list.insert(100,5);

        System.*out*.println(list.get(5));

        //可以看到insert函数完成了

        //接下来写删除

        list.delete(5);

        System.*out*.println(list.get(5));

        //可以看到删除函数也是正确的

        list.set(5,30);

        System.*out*.println(list.get(5));

        //修改函数也是正确的

        //OK，增删改查都完成了，最后写个toArray就收工

    }

}


