
# java队列(Queue)用法总结 - bitcarmanlee的博客 - CSDN博客


2018年11月30日 19:05:56[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1083



## 1.队列的特点
队列是一种比较特殊的线性结构。它只允许在表的前端（front）进行删除操作，而在表的后端（rear）进行插入操作。进行插入操作的端称为队尾，进行删除操作的端称为队头。
队列中最先插入的元素也将最先被删除，对应的最后插入的元素将最后被删除。因此队列又称为“先进先出”（FIFO—first in first out）的线性表，与栈(FILO-first in last out)刚好相反。
## 2.java中的队列
java中的Queue接口就实现了队列的功能。
`public interface Queue<E> extends Collection<E> {
    /**
     * Inserts the specified element into this queue if it is possible to do so
     * immediately without violating capacity restrictions, returning
     * {@code true} upon success and throwing an {@code IllegalStateException}
     * if no space is currently available.
     *
     * @param e the element to add
     * @return {@code true} (as specified by {@link Collection#add})
     * @throws IllegalStateException if the element cannot be added at this
     *         time due to capacity restrictions
     * @throws ClassCastException if the class of the specified element
     *         prevents it from being added to this queue
     * @throws NullPointerException if the specified element is null and
     *         this queue does not permit null elements
     * @throws IllegalArgumentException if some property of this element
     *         prevents it from being added to this queue
     */
    boolean add(E e);
    /**
     * Inserts the specified element into this queue if it is possible to do
     * so immediately without violating capacity restrictions.
     * When using a capacity-restricted queue, this method is generally
     * preferable to {@link #add}, which can fail to insert an element only
     * by throwing an exception.
     *
     * @param e the element to add
     * @return {@code true} if the element was added to this queue, else
     *         {@code false}
     * @throws ClassCastException if the class of the specified element
     *         prevents it from being added to this queue
     * @throws NullPointerException if the specified element is null and
     *         this queue does not permit null elements
     * @throws IllegalArgumentException if some property of this element
     *         prevents it from being added to this queue
     */
    boolean offer(E e);
    /**
     * Retrieves and removes the head of this queue.  This method differs
     * from {@link #poll poll} only in that it throws an exception if this
     * queue is empty.
     *
     * @return the head of this queue
     * @throws NoSuchElementException if this queue is empty
     */
    E remove();
    /**
     * Retrieves and removes the head of this queue,
     * or returns {@code null} if this queue is empty.
     *
     * @return the head of this queue, or {@code null} if this queue is empty
     */
    E poll();
    /**
     * Retrieves, but does not remove, the head of this queue.  This method
     * differs from {@link #peek peek} only in that it throws an exception
     * if this queue is empty.
     *
     * @return the head of this queue
     * @throws NoSuchElementException if this queue is empty
     */
    E element();
    /**
     * Retrieves, but does not remove, the head of this queue,
     * or returns {@code null} if this queue is empty.
     *
     * @return the head of this queue, or {@code null} if this queue is empty
     */
    E peek();
}`不得不说，JDK里的代码以及注释看着就是很舒服。各位稍微花点时间看看JDK里的源码以及注释，相信会有很多收获。
## 3.测试Queue接口
JDK中,LinkedList类实现了Queue接口，可以当Queue使用。
`public class QueueTest {
    @Test
    public void test() {
        Queue<Integer> queue = new LinkedList<>();
        queue.offer(1);
        queue.offer(2);
        queue.offer(3);
        queue.offer(4);
        for(int e : queue) {
            System.out.println(e);
        }
        System.out.println("----------");
        System.out.println("poll : " + queue.poll());
        System.out.println("----------");
        for(int e : queue) {
            System.out.println(e);
        }
        System.out.println("ele is: " + queue.element());
        System.out.println("----------");
        for(int e : queue) {
            System.out.println(e);
        }
        System.out.println("peek : " + queue.peek());
        System.out.println("----------");
        for(int e : queue) {
            System.out.println(e);
        }
    }
}`最终代码运行的结果：
`1
2
3
4
----------
poll : 1
----------
2
3
4
ele is: 2
----------
2
3
4
peek : 2
----------
2
3
4`结合第二部分内容，可以有以下结论:
1.尽量使用offer()方法添加元素，使用poll()方法移除元素。dd()和remove()方法在失败的时候会抛出异常。
2.peek方法不会删除元素， Retrieves, but does not remove, the head of this queue,

