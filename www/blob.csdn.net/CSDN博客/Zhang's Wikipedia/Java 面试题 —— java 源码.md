
# Java 面试题 —— java 源码 - Zhang's Wikipedia - CSDN博客


2018年06月05日 23:31:48[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：610



## 1. 静态工厂方法
静态工厂方法不必在每次调用它们的时候都创建一个新的对象；
Boolean.valueOf(boolean)：
publicfinalclassBoolean{publicstaticfinalBoolean TRUE =newBoolean(true);publicstaticfinalBoolean FALSE =newBoolean(false);publicstaticBooleanvalueOf(booleanb) {return(b ? TRUE : FALSE);
    }
}
## 2. ArrayList 长度的动态拓展
构造函数：
// 底层数据结构，定长数组privatetransientObject[] elementData;publicArrayList(intinitialCapacity) {super();if(initialCapacity <0)thrownewIllegalArgumentException("Illegal Capacity: "+initialCapacity);this.elementData =newObject[initialCapacity];//属性指向新建长度为初始容量的临时数组}// 使用初始容量10构造一个空列表publicArrayList() {this(10);
}
/ *构造包含利用collection的迭代器按顺序返回的指定collection元素的列表
 *@paramc 集合，它的元素被用来放入列表t
 *@throwsNullPointerException 如果指定集合为null*/publicArrayList(Collection<? extends E> c) {
    elementData = c.toArray();//用Collection初始化数组elementDatasize = elementData.length;if(elementData.getClass() != Object[].class)
        elementData = Arrays.copyOf(elementData, size, Object[].class);
}
动态拓展 ：

![这里写图片描述](https://img-blog.csdn.net/20180823230733500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)[](https://img-blog.csdn.net/20180823230733500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[](https://img-blog.csdn.net/20180823230733500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)`为了实现这一机制，java 引进了 Capacity 和 size 概念，以区别数组的 length。为了保证用户增加新的列表对象，java设置了最小容量（minCapacity），通常情况上，它大于列表对象的数目，所以 Capactiy 虽然就是底层数组的长度（length），但是对于最终用户来讲，它是无意义的。而size存储着列表对象的数量，才是最终用户所需要的。为了防止用户错误修改，这一属性被设置为privae的，不过可以通过size()获取。
```
private void grow(int minCapacity) {
    // overflow-conscious code
    int oldCapacity = elementData.length;
    int newCapacity = oldCapacity + (oldCapacity >> 1);//新容量扩大到原容量的1.5倍，右移一位相关于原数值除以2。
    if (newCapacity - minCapacity < 0)
        newCapacity = minCapacity;
    if (newCapacity - MAX_ARRAY_SIZE > 0)
        newCapacity = hugeCapacity(minCapacity);
    // minCapacity is usually close to size, so this is a win:
    elementData = Arrays.copyOf(elementData, newCapacity);
}
private static int hugeCapacity(int minCapacity) {
    if (minCapacity < 0) // overflow
        throw new OutOfMemoryError();
    return (minCapacity > MAX_ARRAY_SIZE) ?
        Integer.MAX_VALUE :
        MAX_ARRAY_SIZE;//MAX_ARRAY_SIZE和Integer.MAX_VALUE为常量，详细请参阅下面的注解
}
````[](https://img-blog.csdn.net/20180823230733500?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xhbmNodW5odWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

