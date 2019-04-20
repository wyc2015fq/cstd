# 你注意ArrayList扩容原理了吗 - Spark高级玩法 - CSDN博客
2018年12月03日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：126
本文主要是从java 1.6-1.8说一下ArrayList的初始容量大小及扩容的思路，主要是底层是ArrayList在扩容的时候会整个复制导致性能底下，所以在大致知道数组容量大小的时候要给定一个合适的初始大小，最大化减小复制的次数。
**1. java 1.6**
publicArrayList(){
this(10);
 }
带参数的构造函数，初始化了一个长度为初始容量的数组：
publicArrayList(int initialCapacity){
super();
if (initialCapacity < 0)
thrownew IllegalArgumentException("Illegal Capacity: "+
                                               initialCapacity);
this.elementData = new Object[initialCapacity];
    }
(2)add方法：
publicbooleanadd(E e){
  ensureCapacity(size + 1);  // Increments modCount!!
  elementData[size++] = e;
returntrue;
 }
(3)扩容方法：
publicvoidensureCapacity(int minCapacity){
  modCount++;
int oldCapacity = elementData.length;
if (minCapacity > oldCapacity) {
      Object oldData[] = elementData;
int newCapacity = (oldCapacity * 3)/2 + 1;
if (newCapacity < minCapacity)
    newCapacity = minCapacity;
// minCapacity is usually close to size, so this is a win:
            elementData = Arrays.copyOf(elementData, newCapacity);
  }
    }
此方法里，一旦发现容量不足，会自动扩充容量，新的大小是：
int newCapacity = (oldCapacity * 3)/2 + 1
也就是原有容量的1.5倍+1。然后通过底层的复制方法将原有数据复制过来：
elementData = Arrays.copyOf(elementData, newCapacity);
**2.JDK1.7**
(1)默认的构造函数 初试化时的长度为10：
publicArrayList(){
this(10);
 }
带参数的构造函数，初始化了一个长度为初始容量的数组：
publicArrayList(int initialCapacity){
super();
if (initialCapacity < 0)
thrownew IllegalArgumentException("Illegal Capacity: "+initialCapacity);
this.elementData = new Object[initialCapacity];
    }
(2)add方法：
publicbooleanadd(E e){
        ensureCapacityInternal(size + 1);  // Increments modCount!!
        elementData[size++] = e;
returntrue;
 }
(3)扩容方法：
privatevoidensureCapacityInternal(int minCapacity){
        modCount++;
// overflow-conscious code
if (minCapacity - elementData.length > 0)
            grow(minCapacity);
}
这是JDK1.7 ArrayList扩容的关键方法：
privatevoidgrow(int minCapacity){
// overflow-conscious code
int oldCapacity = elementData.length;
int newCapacity = oldCapacity + (oldCapacity >> 1);
if (newCapacity - minCapacity < 0)
            newCapacity = minCapacity;
if (newCapacity - MAX_ARRAY_SIZE > 0)
            newCapacity = hugeCapacity(minCapacity);
// minCapacity is usually close to size, so this is a win:
        elementData = Arrays.copyOf(elementData, newCapacity);
    }
此方法里，一旦发现容量不足，会自动扩充容量，新的大小是：
int newCapacity = oldCapacity + (oldCapacity >> 1);
也就是原有容量加上自己除以2的值。然后通过底层的复制方法将原有数据复制过来：
elementData = Arrays.copyOf(elementData, newCapacity);
综述所述，JDK1.6和JDK1.7的ArrayList在扩容方面是不同的，JDK1.7通过位移的方式，效率更高些。
**3. JDK1.8**
在1.8 arraylist这个类中，扩容调用的是grow()方法，通过grow()方法中调用的Arrays.copyof()方法进行对原数组的复制，在通过调用System.arraycopy()方法进行复制，达到扩容的目的。
(1). 几个重要的初始化值
存储数组元素的缓冲区
// non-private to simplify nested class access
transient Object[] elementData; 
默认空数组元素
privatestaticfinal Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};
默认初始化容量
privatestaticfinalint DEFAULT_CAPACITY = 10;
数组的大小
privateint size;
记录被修改的次数
protectedtransientint modCount = 0;
数组的最大值
privatestaticfinalint MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8
(2). 几个重要的方法
一个空的构造方法
默认数组的长度为10
publicArrayList(){
this.elementData  = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
}
(3). add方法
publicbooleanadd(E e){
扩充长度，在原来的大小上面加1
// Increments modCount!!
    ensureCapacityInternal(size + 1);  
添加元素
    elementData[size++] = e;
returntrue;
}
(4). 确认内部容量
privatevoidensureCapacityInternal(int minCapacity){
    ensureExplicitCapacity(
calculateCapacity(elementData, minCapacity));
}
(5). 计算容量
privatestaticintcalculateCapacity(Object[] elementData, int minCapacity){
如果这个数组等于空，返回最大的容量，否则，还是原来的容量
if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
return Math.max(DEFAULT_CAPACITY, minCapacity);
    }
return minCapacity;
    }
(6). 确认扩展容量
privatevoidensureExplicitCapacity(int minCapacity){
修改次数加1
    modCount++;
如果容量不够，调用增加容量方法
// overflow-conscious code
if (minCapacity - elementData.length > 0)
        grow(minCapacity);
}
(7). 扩展方法
privatevoidgrow(int minCapacity){
// overflow-conscious code
获取原来数组容量的长度
int oldCapacity = elementData.length;
新增加的容量长度为原来容量的1.5倍
int newCapacity = oldCapacity + (oldCapacity >> 1);
新容量比老容量小，那么新的容量就是老的容量
if (newCapacity - minCapacity < 0)
        newCapacity = minCapacity;
新创建的容量超过数组的最大值。抛出异常
if (newCapacity - MAX_ARRAY_SIZE > 0)
        newCapacity = hugeCapacity(minCapacity);
// minCapacity is usually close to size, so this is a win:
调用复制方法，在原来元素上增加容量，这就是传说中的可变集合。用新长度复制原数组。
    elementData = Arrays.copyOf(elementData, newCapacity);
}
(8). 数组工具类的复制方法
publicstatic <T> T[] copyOf (T[] original, int newLength) {
return (T[]) copyOf(original, newLength, original.getClass());
}
(9). 具体的复制方法
publicstatic <T,U> T[] copyOf(U[] original, int newLength, 
Class<? extends T[]> newType) {
@SuppressWarnings("unchecked")
获得者数组对象
    T[] copy = ((Object)newType == (Object)Object[].class)
        ? (T[]) newObject[newLength]
        : (T[]) Array.newInstance(newType.getComponentType(), newLength);
调用系统的方法增加数组容量
    System.arraycopy(original, 0, copy, 0,Math.min(original.length, newLength));
return copy;
}
**推荐阅读：**
[负责任的说，Java仍然免费](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485999&idx=2&sn=1a0980cb4842ea03c05e36f2690fd90a&chksm=9f38e907a84f601124ee4a8a474aeb8a10232c78dd656204c2d3af35c6e0faefc453cf69e923&scene=21#wechat_redirect)
[Spark SQL用UDF实现按列特征重分区](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485842&idx=1&sn=cf7331c3a082a141366e5771640c5e52&chksm=9f38eabaa84f63acec983e9e15691b01239349f568bd8cb9725fbc42a01aa721e97145f8622c&scene=21#wechat_redirect)
[Java 程序员必须了解的 7 个性能指标](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486012&idx=1&sn=9e333e60e4243f6d1e376c6d3d46244e&chksm=9f38e914a84f6002f4ce43b87466f03603175cdb7afe07326718670e532f39c1b7ba2add79bf&scene=21#wechat_redirect)
[Apache Kafka：优化部署的 10 种最佳实践](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485967&idx=1&sn=0d59c15d389812b7df8ef80b6dca9462&chksm=9f38e927a84f603113b87cd1ffd42e8afe9fd27f8da1092fe841d0855e998dec24ae9f8bc310&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
欢迎转发～
