# Java —— Iterator 接口与 ListIterator 接口 - Alex_McAvoy的博客 - CSDN博客





2018年10月19日 19:46:52[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：41
个人分类：[Java学习笔记](https://blog.csdn.net/u011815404/article/category/8863016)

所属专栏：[Java 学习笔记](https://blog.csdn.net/column/details/34317.html)









# 【Iterator 接口】

Iterator 接口定义了 3 个方法分别是 hasNext()、next()、remove()，其用法如下：
- hasNext() 方法用于检查当前序列中是否还有元素
- next() 方法用于获得当前序列中的下一个元素
- remove() 方法用于将上一次返回的元素从迭代器中移除

当使用 Iterator 接口遍历集合时，其基本框架如下

```cpp
Collection<Person> persons = new ArrayList<Person>();
Iterator<Person> iterator = persons.iterator();//获取当前集合的一个iterator实例
while(iterator.hasNext()){//当前集合序列不为空时 
    System.out.println(iterator.next());//输出下一个元素
}
```

# 【ListIterator 接口】

ListIterator 接口继承于 Iterator 接口，其与 Iterator 不同的是，它不仅可以向后迭代，它还可以向前迭代。

ListIterator 接口相对 Iterator 接口增加了如下 3 个方法：
- hasPrevious()：返回该迭代器关联的集合是否还有上一个元素
- previous()：返回该迭代器的上一个元素
- add()：在指定位置插入一个元素

```java
Collection<Person> persons = new ArrayList<Person>();
ListIterator<Person> lit = persons .listIterator();
while (lit.hasNext()){//正向迭代
	System.out.println(lit.next());
}

while(lit.hasPrevious()){//反向迭代
	System.out.println(lit.previous());
}
```

# 【区别】

1. iterator() 方法在 Set 接口和 List 接口中都有定义，但是 ListIterator() 方法仅存在于 List 接口中。

2. ListIterator 接口和 Iterator 接口都有 hasNext()、next() 方法，可以实现顺序向后遍历，但是 ListIterator 接口有 hasPrevious() 方法和 previous() 方法，可以实现逆向（顺序向前）遍历。

3. ListIterator 接口可以通过 nextIndex() 方法和 previousIndex() 方法定位当前的索引位置。

4. 两者均可实现删除对象，但 ListIterator 接口中的 set() 方法可以实现对象的修改。



