# [LeetCode] Design Circular Deque 设计环形双向队列 - Grandyang - 博客园







# [[LeetCode] Design Circular Deque 设计环形双向队列](https://www.cnblogs.com/grandyang/p/9899490.html)









Design your implementation of the circular double-ended queue (deque).

Your implementation should support following operations:
- `MyCircularDeque(k)`: Constructor, set the size of the deque to be k.
- `insertFront()`: Adds an item at the front of Deque. Return true if the operation is successful.
- `insertLast()`: Adds an item at the rear of Deque. Return true if the operation is successful.
- `deleteFront()`: Deletes an item from the front of Deque. Return true if the operation is successful.
- `deleteLast()`: Deletes an item from the rear of Deque. Return true if the operation is successful.
- `getFront()`: Gets the front item from the Deque. If the deque is empty, return -1.
- `getRear()`: Gets the last item from Deque. If the deque is empty, return -1.
- `isEmpty()`: Checks whether Deque is empty or not. 
- `isFull()`: Checks whether Deque is full or not.



Example:
MyCircularDeque circularDeque = new MycircularDeque(3); // set the size to be 3
circularDeque.insertLast(1);			// return true
circularDeque.insertLast(2);			// return true
circularDeque.insertFront(3);			// return true
circularDeque.insertFront(4);			// return false, the queue is full
circularDeque.getRear();  			// return 2
circularDeque.isFull();				// return true
circularDeque.deleteLast();			// return true
circularDeque.insertFront(4);			// return true
circularDeque.getFront();			// return 4



Note:
- All values will be in the range of [0, 1000].
- The number of operations will be in the range of [1, 1000].
- Please do not use the built-in Deque library.





这道题让我们设计一个环形双向队列，由于之前刚做过一道[Design Circular Queue](https://www.cnblogs.com/grandyang/p/9899034.html)，那道设计一个环形队列，其实跟这道题非常的类似，环形双向队列在环形队列的基础上多了几个函数而已，其实本质并没有啥区别，那么之前那道题的解法一改吧改吧也能用在这道题上，参见代码如下：



解法一：

```
class MyCircularDeque {
public:
    /** Initialize your data structure here. Set the size of the deque to be k. */
    MyCircularDeque(int k) {
        size = k;   
    }
    
    /** Adds an item at the front of Deque. Return true if the operation is successful. */
    bool insertFront(int value) {
        if (isFull()) return false;
        data.insert(data.begin(), value);
        return true;
    }
    
    /** Adds an item at the rear of Deque. Return true if the operation is successful. */
    bool insertLast(int value) {
        if (isFull()) return false;
        data.push_back(value);
        return true;
    }
    
    /** Deletes an item from the front of Deque. Return true if the operation is successful. */
    bool deleteFront() {
        if (isEmpty()) return false;
        data.erase(data.begin());
        return true;
    }
    
    /** Deletes an item from the rear of Deque. Return true if the operation is successful. */
    bool deleteLast() {
        if (isEmpty()) return false;
        data.pop_back();
        return true;
    }
    
    /** Get the front item from the deque. */
    int getFront() {
        if (isEmpty()) return -1;
        return data.front();
    }
    
    /** Get the last item from the deque. */
    int getRear() {
        if (isEmpty()) return -1;
        return data.back();
    }
    
    /** Checks whether the circular deque is empty or not. */
    bool isEmpty() {
        return data.empty();
    }
    
    /** Checks whether the circular deque is full or not. */
    bool isFull() {
        return data.size() >= size;
    }

private:
    vector<int> data;
    int size;
};
```



就像前一道题中的分析的一样，上面的解法并不是本题真正想要考察的内容，我们要用上环形Circular的性质，我们除了使用size来记录环形队列的最大长度之外，还要使用三个变量，head，tail，cnt，分别来记录队首位置，队尾位置，和当前队列中数字的个数，这里我们将head初始化为k-1，tail初始化为0。还是从简单的做起，判空就看当前个数cnt是否为0，判满就看当前个数cnt是否等于size。接下来取首尾元素，先进行判空，然后根据head和tail分别向后和向前移动一位取即可，记得使用上循环数组的性质，要对size取余。再来看删除末尾函数，先进行判空，然后tail向前移动一位，使用循环数组的操作，然后cnt自减1。同理，删除开头函数，先进行判空，队首位置head要向后移动一位，同样进行加1之后对长度取余的操作，然后cnt自减1。再来看插入末尾函数，先进行判满，然后将新的数字加到当前的tail位置，tail移动到下一位，为了避免越界，我们使用环形数组的经典操作，加1之后对长度取余，然后cnt自增1即可。同样，插入开头函数，先进行判满，然后将新的数字加到当前的head位置，head移动到前一位，然后cnt自增1，参见代码如下：



解法二：

```
class MyCircularDeque {
public:
    /** Initialize your data structure here. Set the size of the deque to be k. */
    MyCircularDeque(int k) {
        size = k; head = k - 1; tail = 0, cnt = 0;
        data.resize(k);
    }
    
    /** Adds an item at the front of Deque. Return true if the operation is successful. */
    bool insertFront(int value) {
        if (isFull()) return false;
        data[head] = value;
        head = (head - 1 + size) % size;
        ++cnt;
        return true;
    }
    
    /** Adds an item at the rear of Deque. Return true if the operation is successful. */
    bool insertLast(int value) {
        if (isFull()) return false;
        data[tail] = value;
        tail = (tail + 1)  % size;
        ++cnt;
        return true;
    }
    
    /** Deletes an item from the front of Deque. Return true if the operation is successful. */
    bool deleteFront() {
        if (isEmpty()) return false;
        head = (head + 1) % size;
        --cnt;
        return true;
    }
    
    /** Deletes an item from the rear of Deque. Return true if the operation is successful. */
    bool deleteLast() {
        if (isEmpty()) return false;
        tail = (tail - 1 + size) % size;
        --cnt;
        return true;
    }
    
    /** Get the front item from the deque. */
    int getFront() {
        return isEmpty() ? -1 : data[(head + 1) % size];
    }
    
    /** Get the last item from the deque. */
    int getRear() {
        return isEmpty() ? -1 : data[(tail - 1 + size) % size];
    }
    
    /** Checks whether the circular deque is empty or not. */
    bool isEmpty() {
        return cnt == 0;
    }
    
    /** Checks whether the circular deque is full or not. */
    bool isFull() {
        return cnt == size;
    }

private:
    vector<int> data;
    int size, head, tail, cnt;
};
```



论坛上还见到了使用链表来做的解法，由于博主比较抵触在解法中新建class，所以这里就不贴了，可以[参见这个帖子](https://leetcode.com/problems/design-circular-deque/discuss/149371/Java-doubly-LinkedList-solution-very-straightforward)。



类似题目：

[Design Circular Queue](https://www.cnblogs.com/grandyang/p/9899034.html)



参考资料：

[https://leetcode.com/problems/design-circular-deque/](https://leetcode.com/problems/design-circular-deque/)

[https://leetcode.com/problems/design-circular-deque/discuss/149371/Java-doubly-LinkedList-solution-very-straightforward](https://leetcode.com/problems/design-circular-deque/discuss/149371/Java-doubly-LinkedList-solution-very-straightforward)

[https://leetcode.com/problems/design-circular-deque/discuss/155209/c%2B%2B-99-ring-buffer-no-edge-cases.-fb-interviewer-really-loves-it.-easy-to-impl-in-4mins.-cheers!](https://leetcode.com/problems/design-circular-deque/discuss/155209/c%2B%2B-99-ring-buffer-no-edge-cases.-fb-interviewer-really-loves-it.-easy-to-impl-in-4mins.-cheers!)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












