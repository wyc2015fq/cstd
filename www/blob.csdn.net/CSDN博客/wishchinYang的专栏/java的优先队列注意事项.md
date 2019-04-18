# java的优先队列注意事项 - wishchinYang的专栏 - CSDN博客
2017年10月16日 10:14:43[wishchin](https://me.csdn.net/wishchin)阅读数：392
在C++语言中，使用优先队列，直接构建一个lambda表达式，使用一个匿名函数指针。java比较函数的返回值不是bool型，只能是整型。
内部对应的C++匿名函数：
// 匿名Comparator实现
```cpp
auto compareMax = []( const Cell &a, const Cell &b ) { return a.max < b.max; };
```
对应的Java函数：
```cpp
import java.util.Queue;
import java.util.Comparator;
import java.util.PriorityQueue;
    // 匿名Comparator实现
   public static Comparator<Cell> compareMax = new Comparator<Cell>() {
        @Override
        public int compare(Cell c1, Cell c2) {
            if (c1.max < c2.max)
                return 1;
            else
                return -1;
        }
    };
```
匿名比较函数实现，java使用int型，返回值为1和-1，C++可以使用boolean型。其实应该写一个lambda表达式的
Java优先队列的使用：
```cpp
Queue<Cell> cellQueue = new PriorityQueue( compareMax );
```
对应C++优先队列的使用：
```cpp
using Queue = std::priority_queue< Cell<T>, std::vector<Cell<T> >, decltype(compareMax)>;
Queue<Cell> cellQueue = new PriorityQueue(compareMax);
```
实现功能，在插入时，可以使用c.max进行排序插入。
参考：[java函数式编程之lambda表达式](http://www.cnblogs.com/mahang/p/3247017.html)
匿名类实现匿名函数：
```cpp
public void testAnonymousClass() {
    Integer[] nums = {2, 5, 1, 6};
    Arrays.sort(nums, new Comparator<Integer>() {
        @Override
        public int compare(Integer o1, Integer o2) {
            if(o1 < o2)
                return -1;
            return 0;
        }
    });
    for (Integer n : nums) {
        System.out.println(n);
    }
}
```
lambda表达式：
```cpp
public void testAnonymousClass() {
    Integer[] nums = {2, 5, 1, 6};
    Arrays.sort(nums, (o1, o2) -> {
        if(o1 < o2)
            return -1;
        return 0;
    });
    for (Integer n : nums) {
        System.out.println(n);
    }
}
```
参考：[java中queue的使用](http://www.cnblogs.com/end/archive/2012/10/25/2738493.html)
    Queue接口与List、Set同一级别，都是继承了Collection接口。LinkedList实现了Queue接 口。Queue接口窄化了对LinkedList的方法的访问权限（即在方法中的参数类型如果是Queue时，就完全只能访问Queue接口所定义的方法 了，而不能直接访问 LinkedList的非Queue的方法），以使得只有恰当的方法才可以使用。BlockingQueue 继承了Queue接口。
**add**增加一个元索如果队列已满，则抛出一个IIIegaISlabEepeplian异常
**remove**移除并返回队列头部的元素如果队列为空，则抛出一个NoSuchElementException异常
**element  **返回队列头部的元素如果队列为空，则抛出一个NoSuchElementException异常
**offer**添加一个元素并返回true       如果队列已满，则返回false
**poll**移除并返问队列头部的元素如果队列为空，则返回null
**peek**返回队列头部的元素如果队列为空，则返回null
**put**添加一个元素如果队列满，则阻塞
**take**移除并返回队列头部的元素如果队列为空，则阻塞
remove、element、offer 、poll、peek 其实是属于Queue接口。
参考：[java8
手把手教你学会写lambda表达式](http://blog.csdn.net/bitcarmanlee/article/details/70195403)
