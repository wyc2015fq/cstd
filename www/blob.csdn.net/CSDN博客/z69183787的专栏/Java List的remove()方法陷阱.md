# Java List的remove()方法陷阱 - z69183787的专栏 - CSDN博客
2019年03月04日 10:25:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：65
[https://blog.csdn.net/pelifymeng2/article/details/78085836](https://blog.csdn.net/pelifymeng2/article/details/78085836)
Java的List在删除元素时，一般会用list.remove(o)/remove(i)方法。在使用时，容易触碰陷阱，得到意想不到的结果。总结以往经验，记录下来与大家分享。
  首先初始化List，代码如下：
package com.cicc.am.test;
import java.util.ArrayList;
import java.util.List;
public class ListTest {
    public static void main(String[] args) {
        List<Integer> list=new ArrayList<Integer>();
        list.add(1);
        list.add(2);
        list.add(3);
        list.add(3);
        list.add(4);
        System.out.println(list);
    }
}
输出结果为[1, 2, 3, 3, 4]
1、普通for循环遍历List删除指定元素--错误！！！
for(int i=0;i<list.size();i++){
   if(list.get(i)==3) list.remove(i);
}
System.out.println(list);
输出结果：[1, 2, 3, 4]
   为什么元素3只删除了一个？本以为这代码再简单不过，可还是掉入了陷阱里，上面的代码这样写的话，元素3是过滤不完的。只要list中有相邻2个相同的元素，就过滤不完。List调用remove(index)方法后，会移除index位置上的元素，index之后的元素就全部依次左移，即索引依次-1要保证能操作所有的数据，需要把index-1，否则原来索引为index+1的元素就无法遍历到(因为原来索引为index+1的数据，在执行移除操作后，索引变成index了，如果没有index-1的操作，就不会遍历到该元素，而是遍历该元素的下一个元素)。
  如果这样，删除元素后同步调整索引或者倒序遍历删除元素，是否可行呢？
2、for循环遍历List删除元素时，让索引同步调整--正确！
for(int i=0;i<list.size();i++){
   if(list.get(i)==3) list.remove(i--);
}
System.out.println(list);
输出结果：[1, 2, 4]
3、倒序遍历List删除元素--正确！
for(int i=list.size()-1;i>=0;i--){
    if(list.get(i)==3){
        list.remove(i);
    }
}
System.out.println(list);
输出结果：[1, 2, 4]
4、foreach遍历List删除元素--错误！！！
for(Integer i:list){
    if(i==3) list.remove(i);
}
System.out.println(list);
   抛出异常：java.util.ConcurrentModificationException
   foreach 写法实际上是对的 Iterable、hasNext、next方法的简写。因此从List.iterator()源码着手分析，跟踪iterator()方法，该方法返回了 Itr 迭代器对象。
  public Iterator<E> iterator() {
        return new Itr();
    }
Itr 类定义如下：
private class Itr implements Iterator<E> {
        int cursor;       // index of next element to return
        int lastRet = -1; // index of last element returned; -1 if no such
        int expectedModCount = modCount;
        public boolean hasNext() {
            return cursor != size;
        }
        @SuppressWarnings("unchecked")
        public E next() {
            checkForComodification();
            int i = cursor;
            if (i >= size)
                throw new NoSuchElementException();
            Object[] elementData = ArrayList.this.elementData;
            if (i >= elementData.length)
                throw new ConcurrentModificationException();
            cursor = i + 1;
            return (E) elementData[lastRet = i];
        }
        public void remove() {
            if (lastRet < 0)
                throw new IllegalStateException();
            checkForComodification();
            try {
                ArrayList.this.remove(lastRet);
                cursor = lastRet;
                lastRet = -1;
                expectedModCount = modCount;
            } catch (IndexOutOfBoundsException ex) {
                throw new ConcurrentModificationException();
            }
        }
        final void checkForComodification() {
            if (modCount != expectedModCount)
                throw new ConcurrentModificationException();
        }
    }
       通过代码我们发现 Itr 是 ArrayList 中定义的一个私有内部类，在 next、remove方法中都会调用checkForComodification 方法，该方法的作用是判断 modCount != expectedModCount是否相等，如果不相等则抛出ConcurrentModificationException异常。每次正常执行remove 方法后，都会对执行expectedModCount = modCount赋值，保证两个值相等，那么问题基本上已经清晰了，在 foreach 循环中
执行 list.remove(item);，对 list 对象的 modCount 值进行了修改，而 list 对象的迭代器的 expectedModCount 值未进行修改，因此抛出了ConcurrentModificationException异常。
5、迭代删除List元素--正确！
java中所有的集合对象类型都实现了Iterator接口，遍历时都可以进行迭代：
Iterator<Integer> it=list.iterator();
    while(it.hasNext()){
        if(it.next()==3){
            it.remove();
        }
        }
System.out.println(list);
输出结果：[1, 2, 4]
 Iterator.remove() 方法会在删除当前迭代对象的同时，会保留原来元素的索引。所以用迭代删除元素是最保险的方法，建议大家使用List过程
中需要删除元素时，使用这种方式。
6、迭代遍历,用list.remove(i)方法删除元素--错误！！！
Iterator<Integer> it=list.iterator();
    while(it.hasNext()){
        Integer value=it.next();
         if(value==3){
            list.remove(value);
        }
    }
System.out.println(list);
抛出异常：java.util.ConcurrentModificationException，原理同上述方法4.
7、List删除元素时，注意Integer类型和int类型的区别.
上述Integer的list,直接删除元素2，代码如下：
list.remove(2);
System.out.println(list);
输出结果：[1, 2, 3, 4]
可以看出,List删除元素时传入数字时，默认按索引删除。如果需要删除Integer对象，调用remove(object)方法，需要传入Integer类型，代码如下：
list.remove(new Integer(2));
System.out.println(list);
输出结果：[1, 3, 3, 4]
总结：
   1、用for循环遍历List删除元素时，需要注意索引会左移的问题。
   2、List删除元素时，为避免陷阱，建议使用迭代器iterator的remove方式。
   3、List删除元素时，默认按索引删除，而不是对象删除。
