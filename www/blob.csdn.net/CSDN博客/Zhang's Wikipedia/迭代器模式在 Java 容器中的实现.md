
# 迭代器模式在 Java 容器中的实现 - Zhang's Wikipedia - CSDN博客


2018年02月03日 00:08:18[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：160个人分类：[设计模式](https://blog.csdn.net/lanchunhui/article/category/5936259)[面试](https://blog.csdn.net/lanchunhui/article/category/7702284)[](https://blog.csdn.net/lanchunhui/article/category/5936259)



迭代器接口是迭代器模式实现的精髓：
public interface Iterator<E> {
    boolean hasNext();
        Enext();...}
假设某容器名为 XxCollection（该类本身并不需实现 Iterator 接口），调用其迭代器遍历访问其所有成员时，一般是通过其 iterator() 方法（Java 容器类如 ArrayList/LinkedList 下的 iterator 也是如下所示）
publicclassXxCollection<E> {publicIterator<E>iterator() {returnnewIter();
    }
}而 Iter 类是 XxCollection 类内的私有类：
privateclassItrimplementsIterator<E> {publicbooleanhasNext() {
        ....
    }publicE next() {
        ....
    }
}
## 1. Iterator 与 Iterable
Iterator
package java.util;publicinterfaceIterator<E> {
    boolean hasNext();
    E next();voidremove();
}
Iterable
packagejava.lang;importjava.util.Iterator;publicInterface Iterable<T> {
    Iterator<T> iterator();
}
## 2. 如何在 java 容器中使用迭代器
```python
// 两个不同的 List 子类
```
```python
List
```
```python
<
```
```python
String
```
```python
>
```
```python
list
```
```python
=
```
```python
new
```
```python
ArrayList
```
```python
<
```
```python
String
```
```python
>
```
```python
();
```
```python
List
```
```python
<
```
```python
String
```
```python
>
```
```python
linkList
```
```python
=
```
```python
new
```
```python
LinkedList
```
```python
<
```
```python
String
```
```python
>
```
```python
();
```
```python
void
```
```python
iterate
```
```python
(
```
```python
List
```
```python
<
```
```python
String
```
```python
>
```
```python
list
```
```python
) {
    Iterator
```
```python
<
```
```python
String
```
```python
>
```
```python
iter
```
```python
=
```
```python
list
```
```python
.
```
```python
iterator();
```
```python
while
```
```python
(iter
```
```python
.
```
```python
hasNext()) {
```
```python
...
```
```python
.
```
```python
}
}
```
## 3. Iterator 遍历时不可以删除集合中的元素
在使用Iterator的时候禁止对所遍历的容器进行改变其大小结构的操作。例如: 在使用Iterator进行迭代时，如果对集合进行了add、remove操作就会出现ConcurrentModificationException异常。
因为在你迭代之前，迭代器已经被通过list.itertor()创建出来了，如果在迭代的过程中，又对list进行了改变其容器大小的操作，那么Java就会给出异常。因为此时Iterator对象已经无法主动同步list做出的改变，Java会认为你做出这样的操作是线程不安全的，就会给出善意的提醒（抛出ConcurrentModificationException异常）

