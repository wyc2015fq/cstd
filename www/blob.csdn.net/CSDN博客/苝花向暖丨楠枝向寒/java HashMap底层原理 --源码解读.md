# java HashMap底层原理 --源码解读 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年08月12日 20:03:55[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：57


##  jdk1.7中

 HashMap底层存数据的 实际上是entry对象数组，每一个entry对象中有4个属性，分别是

key、value、hash值、和下一个entry对象(链表)，所以说是 数组与链表的结合

如下图所示

![](https://img-blog.csdn.net/20180812142624549?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 如果我们知道大概需要向一个hashmap中存入多少条数据，那么就可以在创建一个hashMap对象时通过构造方法，指定容量以及负载因子，容量为存入数据条数的75%-150%之间比较合理，加载因子为0.75比较合理，这样设定能够提高效率。因为，给定的默认容量为 16，负载因子为 0.75。Map 在使用过程中不断的往里面存放数据，当数量达到了 `16 * 0.75 = 12` 就需要将当前 16 的容量进行扩容，而扩容这个过程涉及到 rehash、复制数据等操作，所以非常消耗性能。

当我们通过put方法向hashmap中存入数据的时候，它会先计算key的hash值，用hash值与entry数组的长度相除取余数，余数是多少即放入第几个entry中。然后再遍历这个entry链表，看看是否有entry中存入的key与我们新放入的key相同，有相同的就用新value替换掉之前的value，如果不存在相同的key，就会创建新的entry，创建新entry会判断是否需要扩容，需要扩容就将容量大小扩大二倍，并重新计算我们要存入的 key-value键值对要放入哪个桶（entry数组中的每一个entry被称为一个桶）当中，不需要扩容就不需要重新计算放入哪个桶。

![](https://img-blog.csdn.net/2018081214521784?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

注意，创建新的entry并不是将entry放在一个entry列表的最后面，而是放在最前面

假如我们计算以后需要放在第四个桶中，那么添加完entry以后如下图所指示:

![](https://img-blog.csdn.net/20180812144333290?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

看相关实现代码

 先看一下entry中的变量

![](https://img-blog.csdn.net/20180812144631330?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180812144903985?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

最后看一下get方法

![](https://img-blog.csdn.net/20180812145631222?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

## jkd1.8中的hashmap

对hashmap进行了优化，1.7中，会存在hash冲撞，说白了就是你put进去的好几个key，算出来的hash值都一样，那么他们就被会放入同一个桶中，这样无论是在put或是get的时候都非常影响效率。

1.8的解决方式就是当桶中的链表到达一定长度时会将这个链表转换为红黑树，从而提高了效率

![](https://img-blog.csdn.net/20180812171736224?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面是put方法的源码

```java
final V putVal(int hash, K key, V value, boolean onlyIfAbsent,
                   boolean evict) {
        Node<K,V>[] tab; Node<K,V> p; int n, i;
        if ((tab = table) == null || (n = tab.length) == 0)  //table是桶数组
            n = (tab = resize()).length;
        if ((p = tab[i = (n - 1) & hash]) == null) // (n - 1) & hash 计算放入哪个桶 
            tab[i] = newNode(hash, key, value, null);
        else {
            Node<K,V> e; K k;
            if (p.hash == hash &&  //判断桶中存的key和put的key是否一致，这里的桶代表链表中的第一个节点
                ((k = p.key) == key || (key != null && key.equals(k))))
                e = p;    //如果第一个节点存的key就与我们put的key重复了，就不用再循环遍历链表了，将该节点赋值给e，呆会要更改该节点的value值
            else if (p instanceof TreeNode)  //如果第一个节点的key与我们put的key不同，则需要判断该桶下的是链表还是红黑树
                e = ((TreeNode<K,V>)p).putTreeVal(this, tab, hash, key, value); //红黑树就调用特有的put方法
            else {   //如果桶下是链表结构
                for (int binCount = 0; ; ++binCount) { 遍历链表
                    if ((e = p.next) == null) { 如果遍历到最后一个节点了，还是没有与put的key相同的值
                        p.next = newNode(hash, key, value, null);  //在链表最后添加一个新的节点
                        if (binCount >= TREEIFY_THRESHOLD - 1) // 根据TREEIFY_THRESHOLD-1 来判断是否需要将链表转换为红黑树，TREEIFY_THRESHOLD 值为 8
                            treeifyBin(tab, hash);
                        break;
                    }
                    if (e.hash == hash &&  //如果找到了key相等的节点，就退出循环
                        ((k = e.key) == key || (key != null && key.equals(k))))
                        break;
                    p = e; //每一次循都将p的下一个节点赋值给p，以便继续遍历
                }
            }
            if (e != null) { // 这里的e代表与put的key重复的节点
                V oldValue = e.value;  
                if (!onlyIfAbsent || oldValue == null)
                    e.value = value; //覆盖 value值
                afterNodeAccess(e);
                return oldValue;
            }
        }
        ++modCount;
        if (++size > threshold)   //判断是否需要扩容
            resize();
        afterNodeInsertion(evict);
        return null;
    }
```

 get方法

```java
final Node<K,V> getNode(int hash, Object key) {
        Node<K,V>[] tab; Node<K,V> first, e; int n; K k;
        if ((tab = table) != null && (n = tab.length) > 0 &&
            (first = tab[(n - 1) & hash]) != null) {  //找到桶
            if (first.hash == hash && // 判断第一个节点是否的key是否是我们要get的
                ((k = first.key) == key || (key != null && key.equals(k))))
                return first;
            if ((e = first.next) != null) { 
                if (first instanceof TreeNode) //判断是否是红黑树类型的
                    return ((TreeNode<K,V>)first).getTreeNode(hash, key);
                do {  //不是红黑树就是链表，遍历链表 找
                    if (e.hash == hash &&    
                        ((k = e.key) == key || (key != null && key.equals(k))))
                        return e;
                } while ((e = e.next) != null);
            }
        }
        return null;
    }
```

