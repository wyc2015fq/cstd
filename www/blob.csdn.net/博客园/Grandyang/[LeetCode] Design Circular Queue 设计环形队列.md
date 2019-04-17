# [LeetCode] Design Circular Queue 设计环形队列 - Grandyang - 博客园







# [[LeetCode] Design Circular Queue 设计环形队列](https://www.cnblogs.com/grandyang/p/9899034.html)







Design your implementation of the circular queue. The circular queue is a linear data structure in which the operations are performed based on FIFO (First In First Out) principle and the last position is connected back to the first position to make a circle. It is also called "Ring Buffer".

One of the benefits of the circular queue is that we can make use of the spaces in front of the queue. In a normal queue, once the queue becomes full, we cannot insert the next element even if there is a space in front of the queue. But using the circular queue, we can use the space to store new values.

Your implementation should support following operations:
- `MyCircularQueue(k)`: Constructor, set the size of the queue to be k.
- `Front`: Get the front item from the queue. If the queue is empty, return -1.
- `Rear`: Get the last item from the queue. If the queue is empty, return -1.
- `enQueue(value)`: Insert an element into the circular queue. Return true if the operation is successful.
- `deQueue()`: Delete an element from the circular queue. Return true if the operation is successful.
- `isEmpty()`: Checks whether the circular queue is empty or not.
- `isFull()`: Checks whether the circular queue is full or not.



Example:
MyCircularQueue circularQueue = new MyCircularQueue(3); // set the size to be 3
circularQueue.enQueue(1);  // return true
circularQueue.enQueue(2);  // return true
circularQueue.enQueue(3);  // return true
circularQueue.enQueue(4);  // return false, the queue is full
circularQueue.Rear();  // return 3
circularQueue.isFull();  // return true
circularQueue.deQueue();  // return true
circularQueue.enQueue(4);  // return true
circularQueue.Rear();  // return 4



Note:
- All values will be in the range of [0, 1000].
- The number of operations will be in the range of [1, 1000].
- Please do not use the built-in Queue library.



这道题让我们设计一个环形的队列，说是不能使用内置的queue类，并且让我们实现一系列的成员函数，如进队，出队，取首尾元素，以及判空，判满等等。那么博主最先想到的就是用一个数组data来实现，并且用一个变量size来保存我们的环形队列的大小。先来实现最简单的判空和判满函数吧，判空就是判断data数组是否为空，判满就是看data数组的大小是否等于size。然后是取首尾元素，需要先对数组判空，然后取首尾元素即可。进队列函数先要判满，然后加入data数组，出队列函数，先要判空，然后去掉数组的首元素即可，参见代码如下：



解法一：

```
class MyCircularQueue {
public:
    MyCircularQueue(int k) {
        size = k;
    }
    bool enQueue(int value) {
        if (isFull()) return false;
        data.push_back(value);
        return true;
    }
    bool deQueue() {
        if (isEmpty()) return false;
        data.erase(data.begin());
        return true;
    }
    int Front() {
        if (isEmpty()) return -1;
        return data.front();
    }
    int Rear() {
        if (isEmpty()) return -1;
        return data.back();
    }
    bool isEmpty() {
        return data.empty();
    }
    bool isFull() {
        return data.size() >= size;
    }
    
private:
    vector<int> data;
    int size;
};
```



做完上面的方法有没有一种感觉，这跟环形Circular有毛线关系，还有题目要求中的第二段话里的“我们可以使用队列前面的空间”，完全没有用到啊。其实上面的解法并不是本题真正想要考察的内容，我们要用上环形Circular的性质，之前我们貌似应该做过环形数组的题目吧，提到环形数组，博主最先想到的就是坐标加1，再对数组长度取余。这是数组能够环形的关键，那么这里也一样，我们除了使用size来记录环形队列的最大长度之外，还要使用三个变量，head，tail，cnt，分别来记录队首位置，队尾位置，和当前队列中数字的个数，这里我们将head初始化为 k-1，tail初始化为0，这样初始化我们可以看出什么端倪么，实际上head是指向数组范围内的起始位置的前一个数字，之所以初始化为 k-1，是因为起始时我们假设数组的范围是 [0, k-1]，所以 0 的前一个不是 -1，而是 k-1，因为是环形数组，同理，tail指向数组范围内的结束位置的下一个数字，k-1 的下一个位置不是k，而是0，因为是环形数组。那么在 Front() 函数，由于我们要返回起始位置的数字，而head指向其前一个位置，所以我们要加1，而为了不越界，进行环形走位，还要对 size 取余，于是就变成了 (head+1) % size，同理，对于 Rear() 函数，我们要返回结束位置的数字，而tail指向其下一个位置，所以我们要减1，为了不越界，并且环形走位，要先加上size，再对 size 取余，于是就变成了 (tail-1+size) % size。

还是从简单的做起，判空就看当前个数cnt是否为0，判满就看当前个数cnt是否等于size。接下来取首尾元素，先进行判空，然后根据head和tail分别向后和向前移动一位取即可，记得使用上循环数组的性质，要对size取余。再来看进队列函数，先进行判满，然后将新的数字加到当前的tail位置，tail移动到下一位，为了避免越界，我们使用环形数组的经典操作，加1之后对长度取余，然后cnt自增1即可。同样，出队列函数先进行判空，队首位置head要向后移动一位，同样进行加1之后对长度取余的操作，到这里就可以了，不用真正的去删除数字，因为head和tail限定了我们的当前队列的范围，然后cnt自减1，参见代码如下：



解法二：

```
class MyCircularQueue {
public:
    MyCircularQueue(int k) {
        size = k; head = k - 1; tail = 0; cnt = 0;
        data.resize(k);
    }
    bool enQueue(int value) {
        if (isFull()) return false;
        data[tail] = value;
        tail = (tail + 1) % size;
        ++cnt;
        return true;
    }
    bool deQueue() {
        if (isEmpty()) return false;
        head = (head + 1) % size;
        --cnt;
        return true;
    }
    int Front() {
        return isEmpty() ? -1 : data[(head + 1) % size];
    }
    int Rear() {
        return isEmpty() ? -1 : data[(tail - 1 + size) % size];
    }
    bool isEmpty() {
        return cnt == 0;
    }
    bool isFull() {
        return cnt == size;
    }
    
private:
    vector<int> data;
    int size, cnt, head, tail;
};
```



论坛上还见到了使用链表来做的解法，由于博主比较抵触在解法中新建class，所以这里就不贴了，可以[参见这个帖子](https://leetcode.com/problems/design-circular-queue/discuss/162759/JAVA-Pass-All-Test-Cases-100-O(1))。



类似题目：

[Design Circular Deque](https://www.cnblogs.com/grandyang/p/9899490.html)



参考资料：

[https://leetcode.com/problems/design-circular-queue/](https://leetcode.com/problems/design-circular-queue/)

[https://leetcode.com/problems/design-circular-queue/discuss/149420/Concise-Java-using-array](https://leetcode.com/problems/design-circular-queue/discuss/149420/Concise-Java-using-array)

[https://leetcode.com/problems/design-circular-queue/discuss/162759/JAVA-Pass-All-Test-Cases-100-O(1)](https://leetcode.com/problems/design-circular-queue/discuss/162759/JAVA-Pass-All-Test-Cases-100-O(1))



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












