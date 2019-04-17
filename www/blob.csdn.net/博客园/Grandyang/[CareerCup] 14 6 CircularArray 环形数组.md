# [CareerCup] 14.6 CircularArray 环形数组 - Grandyang - 博客园







# [[CareerCup] 14.6 CircularArray 环形数组](https://www.cnblogs.com/grandyang/p/4991289.html)







14.6 Implement a CircularArray class that supports an array-like data structure which can be efficiently rotated.The class should use a generic type, and should support iteration via the standard for (Obj o : CircularArray) notation.



这道题让我们实现一个环形数组类CircularArray，由于环形数组需要调用rotate(int shiftRight)函数，在这里，我们并不会真的去旋转数组，因为这样十分不高效。我们采用另一种实现方法，用一个变量head来记录环形数组的起始位置，那么调用rotate实际上就是改变head的位置而已。请参见如下代码：



```
public static class CircularArray<T> implements Iterable<T> {
    private T[] items;
    private int head = 0;
    
    public CircularArray(int size) {
        items = (T[]) new Object[size];
    }
    
    private int convert(int idx) {
        if (idx < 0) {
            idx += items.length;
        }
        return (head + idx) % items.length;
    }
    
    public void rotate(int shiftRight) {
        head = convert(shiftRight);
    }
    
    public T get(int i) {
        if (i < 0 || i >= items.length) {
            throw new java.lang.IndexOutOfBoundsException("...");
        }
        return items[convert(i)];
    }
    
    public void set(int i, T item) {
        items[convert(i)] = item;
    }
    
    public Iterator<T> iterator() {
        return new CircularArrayIterator<T> (this);
    }
    
    private class CircularArrayIterator<TI> implements Iterator<TI> {
        private int _current = -1;
        private TI[] _items;
        
        public CircularArrayIterator(CircularArray<TI> array) {
            _items = array.items;
        }
        
        @Override
        public boolean hasNext() {
            return _current < items.length - 1;
        }
        
        @Override
        public TI next() {
            ++_current;
            TI item = (TI) _items[convert(_current)];
            return item;
        }
        
        @Override
        public void remove() {
            throw new UnsupportedOperationException("...");
        }
    }
}
```



上述代码中主要有两部分：

1. 实现CircularArray类

实现的过程中容易犯一些错误，比如:

- 我们不能新建一个泛类的数组，我们必须cast数组或者定义类型为List<T>.

- 取余操作符%对于负数运算后会得到负数，这和数学家们定义的取余运算不同，所以我们需要给负数序列加上items.length，时期变为正数再做运算。

- 我们必须一致地保证将原序列转为旋转序列。

2. 实现迭代器Iterator接口

为了使用迭代来遍历数组for (Obj o : CircularArray)，我们必须实现迭代器Iterator接口：

- 修改CircularArray<T>的定义，添加implements Iteratble<T>。这需要我们添加一个iterator()方法给CircularArray<T>。

- 建立CircularArrayIterator<T>类implements Iterator<T>，这需要我们实现CircularArrayIterator的一些方法，如hasNext(), next(), 和 remove()。

一旦我们实现了上面两项，for (Obj o : CircularArray)循环就会神奇的运行了。














