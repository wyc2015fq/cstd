# C#语法小知识（二十二）集合Collection - 凯奥斯的注释 - CSDN博客





2016年11月27日 23:41:42[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：797
所属专栏：[C#编程小知识](https://blog.csdn.net/column/details/12898.html)









# System.Collections命名空间里面包含了几种常用的集合类型。

它们是：


|ArrayList|动态数组可以对它进行添加删除排除和查找。||
|----|----|----|
|BitArray|二进制数组可以对数组进行And,Or,Xor和Not操作。||
|Hashtable|哈希表用键值来访问元素。||
|Queue|队列先进先出。||
|SortedList|有序数组这是ArrayList和Hashtable的组合，可以用键值来访问元素，也可以用下标来访问。||
|Stack|栈后进先出。||



# System.Collections.Generic命名空间里面包含了几种常用的泛型集合类型。




|Dictionary<TKey,TValue>|泛型字典，TKey代表键值的类型，TValue代表元素的类型。与哈希表相似，使用键值来访问元素。||
|----|----|----|
|HashSet<T>|泛型哈希集，T代表元素的类型。集合中每个元素只有一个，不能使用下标来访问元素。||
|LinkedList<T>|泛型链表，T代表元素的类型。使用LinkedListNode<T>作为链表的节点。||
|List<T>|泛型列表，T代表元素的类型。与动态数组相似，使用下标来访问元素。||
|Queue<T>|泛型队列，T代表元素类型。先进先出。||
|SortedDictionary<TKey,TValue>|泛型有序字典，TKey代表键值的类型，TValue代表元素的类型。遍历时，根据键值排序。||
|SortedList<TKey,TValue>|泛型有序数组，TKey代表键值的类型，TValue代表元素的类型。遍历时，根据键值排序。（与SortedDictionary<TKey,TValue>功能基本一致）||
|SortedSet<T>|泛型有序哈希集，T代表元素的类型。集合中每个元素只有一个，不能使用下标来访问元素。||
|Stack<T>|泛型栈，T代表元素的类型。后进先出。||







