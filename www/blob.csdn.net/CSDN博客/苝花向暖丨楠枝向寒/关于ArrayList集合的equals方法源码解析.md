# 关于ArrayList集合的equals方法源码解析 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年03月15日 19:39:42[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：193


```java

```

```java
public boolean equals(Object o) {
        if (o == this)
            return true;
        if (!(o instanceof List))
            return false;

        ListIterator<E> e1 = listIterator();
        ListIterator<?> e2 = ((List<?>) o).listIterator();
        while (e1.hasNext() && e2.hasNext()) {
            E o1 = e1.next();
            Object o2 = e2.next();
            if (!(o1==null ? o2==null : o1.equals(o2)))
                return false;
        }
        return !(e1.hasNext() || e2.hasNext());
    }
```

首先判断 两个对象地址是否一样，如果一样说明是同一个对象直接返回true

如果为false，继续判断传入的参数是否是list的子类，如果不是返回false。

如果为false，分别迭代两个对象，判断调用equals方法的那个集合中的元素是否为空，如果为空，再判断另一个集合对应的元素是否为空，为空则返回true，然后前面加了个！就变成false了，然后就继续循环判断。直到找到不同元素。

能到这步肯定就是false了，要不然也跳不出while循环。

如果调用equals方法的集合不为空，那么就通过其重写object中的equals方法来判断值是否相等。

例如：集合A和集合B 中存放的 第一个元素都是String 类型的，那么就调用String的equals方法来判断，如果是基本类型的，会自动转型为包装类，然后通过包装类里面的equals方法来判断是否相等。

还有一个情况就是其中一个集合中没有其他元素了，那么就

```java
return !(e1.hasNext() || e2.hasNext());
```





