# ArrayList学习笔记 - westbrook1998的博客 - CSDN博客





2018年03月23日 16:26:58[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：66标签：[集合类																[java																[ArrayList](https://so.csdn.net/so/search/s.do?q=ArrayList&t=blog)
个人分类：[java](https://blog.csdn.net/westbrook1998/article/category/7459913)





### 概述：

![这里写图片描述](https://img-blog.csdn.net/20180323142204660?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlc3Ricm9vazE5OTg=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

`ArrayList`是一个基于数组实现的列表集合类，父类是`AbstractList`和`AbstractCollection`，直接实现了`List<E>`，`RandomAccess`，`Cloneable`，`java.io.Serializable`接口，同时`List<E>`接口也继承了`Collection<E>`接口，`Collection<E>`继承了`Iterable<E>`接口

### 基础操作：

```
package com.zxc.collection;

import java.util.*;

public class ArrayListTest {
    public static void main(String[] args) {
        //默认构造空列表
        //使用List类型引用的话，会有一些ArrayList的特有方法无法使用
        //List<String> stringList = new ArrayList<>();
        ArrayList<String> stringList = new ArrayList<>();
        //add方法添加元素，返回是否添加成功
        stringList.add("hello");
        stringList.add("java");
        stringList.add("hello");
        //带索引的add方法表示从索引处后添加元素
        stringList.add(2,"world");
        List<String> anotherList=new ArrayList<>();
        anotherList.add("html");
        anotherList.add("css");
        //addAll方法添加另一个列表，同样有带索引参数和不带索引参数的两个方法
        stringList.addAll(anotherList);
        stringList.addAll(3,anotherList);
        //isEmpty方法判断是否为空
        if(!stringList.isEmpty()){
            //size方法获取列表长度
            System.out.println(stringList.size());
            //get方法通过索引获取元素
            System.out.println(stringList.get(2));
        }
        //直接打印整个列表
        System.out.println(stringList);
        //set方法覆盖索引处元素
        System.out.println(stringList.set(3,"c++"));
        //参数为元素对象的remove方法返回删除成功与否，而且只删除第一个匹配元素
        System.out.println(stringList.remove("hello"));
        //参数为索引的remove方法返回被删除的对象
        System.out.println(stringList.remove(2));
        System.out.println(stringList);
        //indexOf方法返回第一个匹配元素的索引
        System.out.println(stringList.indexOf("css"));
        //lastIndexOf方法返回最后一个匹配元素的索引
        System.out.println(stringList.lastIndexOf("css"));
        //如果没有匹配元素则返回-1
        System.out.println(stringList.indexOf("c"));
        //contains和containsAll分别判断另一元素或者另一列表是否存在于列表中
        System.out.println(stringList.contains("java"));
        System.out.println(stringList.containsAll(anotherList));
        System.out.println(stringList.removeAll(anotherList));
        //removeALl方法删除参数列表所匹配的所有元素，返回是否删除成功
        System.out.println(stringList);
        System.out.println(anotherList);
        //retainAll方法返回值true或者false是看列表是否发生了改变！而不是两个列表是否相同
        //而得到的两个集合的交集是存储在该调用对象，即原列表中
        //所以并不是通过返回值判断是否有交集！！可以通过判断stringList.size()的大小来判断
        System.out.println(anotherList.retainAll(stringList));
        System.out.println(anotherList);
        //replaceAll方法的作用是将操作符作用于元素，并使用其结果代替每个元素
        //参数为一个实现UnaryOperator接口的操作对象，并重写应用于每个元素的apply方法
        stringList.replaceAll(new UnaryOperator<String>() {
            @Override
            public String apply(String s) {
                return s+"测试匿名内部类";
            }
        });
        //增强型for循环输出
        for(String s: stringList){
            System.out.print(s+" ");
        }
        System.out.println();
        //迭代器输出，通过iterator方法构造迭代器实例
        Iterator<String> iterator=stringList.iterator();
        while(iterator.hasNext()){
            System.out.print(iterator.next()+" ");
        }
        System.out.println();
        //通过lambda表达式输出
        stringList.forEach((string) -> System.out.print(string+"lambda "));
        System.out.println();
        //toArray方法返回一个含该集合元素的数组
        System.out.println(stringList.toArray()[0]);
        String[] str=new String[10];
        //如果toArray方法传入一个数组参数，则返回这一个数组
        System.out.println(stringList.toArray(str)[0]);
        System.out.println(str[0]);
        //subList方法获取子列表，第一索引包含，第二个不包含，即长度为第二个参数-第一个参数
        System.out.println(stringList.subList(0,2));
        //equals方法判断两个列表是否相同
        System.out.println(stringList.equals(anotherList));
        System.out.println(stringList.equals(stringList));
        System.out.println(stringList);
        //一个实现Comparator接口的匿名内部类作为sort函数的参数
        stringList.sort(new Comparator<String>() {
            //重写compare方法
            @Override
            public int compare(String o1, String o2) {
                //重写排序逻辑,通过字符串的compareTo函数判断
                return o1.compareTo(o2);
            }
        });
        System.out.println(stringList);
        //hashCode方法获取列表哈希码
        System.out.println(stringList.hashCode());
        //用lambda表达式代替匿名内部类
        stringList.sort((String a,String b)->{
            return a.compareTo(b)*(-1);
        });
        System.out.println(stringList);
        //使用listIterator方法获取ListIterator类型迭代器
        ListIterator<String> listIterator=stringList.listIterator();
        while(listIterator.hasNext()){
            //可以获取下一个元素和索引，索引从1开始
            System.out.println(listIterator.next());
            System.out.println(listIterator.nextIndex());
            //ListIterator迭代器可以实现迭代过程中的add和set,Iterator迭代器无法做到
            //listIterator.add("add");
            listIterator.set("HH");
        }
        //带索引参数的listIterator方法返回一个从索引位置开始的迭代器，例如2 则迭代器元素从第三个开始
        ListIterator<String> listIterator2=stringList.listIterator(2);
        System.out.println(listIterator2.next());
        while(listIterator2.hasPrevious()){
            //此时虽然迭代器第一个元素是原列表中的第三个，但是在迭代器列表中是第一个，所以索引为1，所以previousIndex返回0 -1
            System.out.println(listIterator2.previous());
            System.out.println(listIterator2.previousIndex());
        }
        System.out.println(stringList.clone());
        //spliterator方法是构造一个并行迭代器,具体用法暂无
        Spliterator<String> stringSpliterator=stringList.spliterator();
        //clear方法清除列表所有元素
        stringList.clear();
        System.out.println(stringList.isEmpty());
        ArrayList<String> arrayList=new ArrayList<>(3);
        for(int i=0;i<3;i++){
            arrayList.add("i"+i);
            System.out.println(arrayList);
            //trimToSize函数使列表中数组长度和列表实际长度相等
            arrayList.trimToSize();
        }
        //ensureCapacity方法手动对底层数组进行扩容
        //手动扩容的好处在于可以避免add方法时集合的自动扩容（通过列表的复制实现）的较低效率
        arrayList.ensureCapacity(5);
        System.out.println(arrayList.size());
        //trimToSize方法可以去除列表扩增时申请的预留数组空间
        arrayList.trimToSize();
        System.out.println(arrayList.size());
Iterator<String> iterator1=arrayList.iterator();
        //使用迭代器迭代删除
        while(iterator1.hasNext()){
            System.out.println(iterator1.next());
            //使用的是迭代器的remove方法，使用该方法之前一定要调用next方法迭代出该元素
            iterator1.remove();
            System.out.println(arrayList);
        }
        System.out.println(arrayList);
    }
}
```

*所有基本api操作和注意事项都在代码注释中*

### ArrayList源代码分析：

```java
private static final long serialVersionUID = 8683452581122892189L;
private static final int DEFAULT_CAPACITY = 10;    
private static final Object[] EMPTY_ELEMENTDATA = {};
private static final Object[] DEFAULTCAPACITY_EMPTY_ELEMENTDATA = {};
transient Object[] elementData; // non-private to simplify nested class access
private int size;
```

基本的成员变量有`elementData`，最主要的数据对象数组，`size`列表长度，三个final常量`DEFAULT_CAPACITY`，默认容量，`EMPTY_ELEMENTDATA`，空数组，`DEFAULTCAPACITY_EMPTY_ELEMENTDATA`，默认容量的空数组

```
public ArrayList(int initialCapacity) {
        if (initialCapacity > 0) {
            this.elementData = new Object[initialCapacity];
        } else if (initialCapacity == 0) {
            this.elementData = EMPTY_ELEMENTDATA;
        } else {
            throw new IllegalArgumentException("Illegal Capacity: "+
                                               initialCapacity);
        }
    }
public ArrayList() {
        this.elementData = DEFAULTCAPACITY_EMPTY_ELEMENTDATA;
    }
public ArrayList(Collection<? extends E> c) {
        elementData = c.toArray();
        if ((size = elementData.length) != 0) {
            // c.toArray might (incorrectly) not return Object[] (see 6260652)
            if (elementData.getClass() != Object[].class)
                elementData = Arrays.copyOf(elementData, size, Object[].class);
        } else {
            // replace with empty array.
            this.elementData = EMPTY_ELEMENTDATA;
        }
    }
```

首先是三个构造方法，`无参构造`，直接赋值一个默认空数组常量，`带初始化容量参数的构造，`通过先判断参数，大于0则构造一个新的对象数组，等于0，直接赋值空数组常量，小于0则抛出异常。`参数为另一个集合类实例的构造`，将集合通过`toArray`转成数组赋值给elementData，再判断数组长度是否为0，如果是，赋值一个空数组常量，**如果不是，还需再判断是否为`Object[]`对象，如果不是，还需要转成`Object[]`对象**，（Mark一下，还待加深理解）

```
public boolean add(E e) {
        //调用ensureCapacityInternal方法对数组容量进行扩充
        ensureCapacityInternal(size + 1);  // Increments modCount!!
        //将对象添加到数组中
        elementData[size++] = e;
        //返回成功
        return true;
    }
    public void add(int index, E element) {
        //rangeCheckForAdd方法检测索引参数是否合法，否则抛出异常
        rangeCheckForAdd(index);
        //扩容数组
        ensureCapacityInternal(size + 1);  // Increments modCount!!
        //调用System的本地方法arraycopy通过数组的复制，将index（包括index）之后的内容复制到原数组里，参数index和index+1 即相当于腾出数组的index位置
        System.arraycopy(elementData, index, elementData, index + 1,
                         size - index);
        //添加元素在index位置上
        elementData[index] = element;
        //长度加一
        size++;
    }
```

add方法分为带索引和不带索引两个，`不带索引的`就直接扩容数组，添加数组元素，`带索引的`先判断索引合法性，再扩容数组，通过数组复制将索引之后的元素后移一位，添加数组元素到索引处

```
private void rangeCheckForAdd(int index) {
        if (index > size || index < 0)
            throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
    }
```

检查索引是否超过列表长度或者小于0

```java
private static int calculateCapacity(Object[] elementData, int minCapacity) {
        //如果数组是默认容量的空数组（空数组不一定是默认容量空数组，不同对象），则返回默认容量（10）和传入最小容量中的大值,而如果是默认容量空数组，此时传入的minCapacity显然是1，所以默认空数组在这里会返回10
        if (elementData == DEFAULTCAPACITY_EMPTY_ELEMENTDATA) {
            return Math.max(DEFAULT_CAPACITY, minCapacity);
        }
        //否则返回传入的最小容量
        return minCapacity;

        }

private void ensureCapacityInternal(int minCapacity) {
        //参数minCapacity被调用所传入的实参是size+1
        ensureExplicitCapacity(calculateCapacity(elementData, minCapacity));
    }

//默认容量空数组传入10，其他传入实际size+1
private void ensureExplicitCapacity(int minCapacity) {
        modCount++;

        // overflow-conscious code
        //如果minCapacity大于elementData数组的长度，调用grow方法进行扩容
        if (minCapacity - elementData.length > 0)
            grow(minCapacity);
    }

private static final int MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8;

private void grow(int minCapacity) {
        // overflow-conscious code
        //获取原数组长度
        int oldCapacity = elementData.length;
        //将原数组长度加上自身1/2赋值给新的数组长度变量，即将数组扩容为原先的1.5倍
        int newCapacity = oldCapacity + (oldCapacity >> 1);
        //如果还小于参数（最小容量），则将参数作为新数组长度
        if (newCapacity - minCapacity < 0)
            newCapacity = minCapacity;
        //如果大于数组最大长度，则调用hugeCapacity方法
        if (newCapacity - MAX_ARRAY_SIZE > 0)
            newCapacity = hugeCapacity(minCapacity);
        // minCapacity is usually close to size, so this is a win:
        //通过copyOf方法，将数组复制到一个新的长度的数组并赋值给原数组
        elementData = Arrays.copyOf(elementData, newCapacity);
    }

private static int hugeCapacity(int minCapacity) {
        //如果参数，最小容量小于0，则抛出异常
        if (minCapacity < 0) // overflow
            throw new OutOfMemoryError();
        //根据最小容量是否大于数组最大容量，返回int类型最大值或者最大数组容量
        return (minCapacity > MAX_ARRAY_SIZE) ?
            Integer.MAX_VALUE :
            MAX_ARRAY_SIZE;
    }
```

add方法中的扩容，很明显，默认的空列表在**第一次**`add`时，会将数组容量扩容到10，而第二次以及之后每次`add`就只会将数组扩容大约1.5倍 

可以看出，这样的扩容经过一定时间后会导致数组大小大于实际列表长度，即浪费了存储空间，所以有了下面的方法`trimToSize`

```
public void trimToSize() {
        modCount++;
        //如果列表长度小于数组长度（即容量）
        if (size < elementData.length) {
        //赋值运算符优先级很低
        //这里先判断列表长度是否为0，分别给elementData赋值空数组或者是将列表实际长度的数组赋值给数组，即将数组中空的元素去除
            elementData = (size == 0)
              ? EMPTY_ELEMENTDATA
              : Arrays.copyOf(elementData, size);
        }
    }
```

```
public void ensureCapacity(int minCapacity) {
        //根据elementData是否为默认空数组给minExpand赋值0或者默认容量（10）
        int minExpand = (elementData != DEFAULTCAPACITY_EMPTY_ELEMENTDATA)
            // any size if not default element table
            ? 0
            // larger than default for default empty table. It's already
            // supposed to be at default size.
            : DEFAULT_CAPACITY;
        //如果minCapacity大于minExpand，调用ensureExplicitCapacity方法
        if (minCapacity > minExpand) {
            ensureExplicitCapacity(minCapacity);
        }
    }
```

`ensureCapacity`方法的目的就是保证列表数组有足够的容量 

这两个方法都是`public`，ArrayList类型的引用可以调用（List接口引用不可以）

```
public boolean addAll(Collection<? extends E> c) {
        //用一个对象数组保存要添加的列表
        Object[] a = c.toArray();
        //获取数组长度
        int numNew = a.length;
        //扩容数组，参数为当前列表长度加将要添加的数组长度，实际扩容后的数组大小比这个参数大
        ensureCapacityInternal(size + numNew);  // Increments modCount
        //复制数组到elementData的后面
        System.arraycopy(a, 0, elementData, size, numNew);
        //更新列表长度
        size += numNew;
        //返回是否成功添加
        return numNew != 0;
    }
public boolean addAll(int index, Collection<? extends E> c) {
        //带索引的首先检查索引的合法性
        rangeCheckForAdd(index);
        //相同的操作
        Object[] a = c.toArray();
        int numNew = a.length;
        ensureCapacityInternal(size + numNew);  // Increments modCount
        //计算需要向后移动的元素数量
        int numMoved = size - index;
        if (numMoved > 0)
        //复制数组，将原数组索引之后的元素移动numNew个单位
            System.arraycopy(elementData, index, elementData, index + numNew,
                             numMoved);
        //将要添加的数组复制到原数组里
        System.arraycopy(a, 0, elementData, index, numNew);
        //同样的操作
        size += numNew;
        return numNew != 0;
    }
```

带索引和不带索引的两个`addALl`方法

```java
public boolean isEmpty() {
        return size == 0;
    }
```

`isEmpty`方法判断是否为空

```
public int size() {
        return size;
    }
```

`size`方法返回列表长度

```
public E get(int index) {
        rangeCheck(index);

        return elementData(index);
    }
private void rangeCheck(int index) {
        if (index >= size)
            throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
    }
```

`get`方法先通过`rangeCheck`方法检查索引合法性，再返回数组的对应元素

```
List.java

public static String toString(long[] a) {
        if (a == null)
            return "null";
        int iMax = a.length - 1;
        if (iMax == -1)
            return "[]";

        StringBuilder b = new StringBuilder();
        b.append('[');
        for (int i = 0; ; i++) {
            b.append(a[i]);
            if (i == iMax)
                return b.append(']').toString();
            b.append(", ");
        }
    }
```

在**`List`**类中重写了`toString`方法，所以可以直接打印出列表的所有元素

```
public E set(int index, E element) {
        //检查索引合法性
        rangeCheck(index);

        //通过elementData方法获取原元素
        E oldValue = elementData(index);
        //更新数组元素
        elementData[index] = element;
        //返回原元素
        return oldValue;
    }
E elementData(int index) {
        return (E) elementData[index];
    }
```

`set`方法会**返回原元素**，在源代码中使用了`elementData`方法来获取原元素而不是直接用数组，体现了一种封装性

```
//通过索引删除
public E remove(int index) {
        //检查索引合法性
        rangeCheck(index);
        //记录修改次数 便于迭代器进行线程安全检查
        modCount++;
        //保存要删除的元素
        E oldValue = elementData(index);

        //获取数组应该（往前）移动的元素个数
        int numMoved = size - index - 1;
        if (numMoved > 0)
            //将数组在索引后的元素前移一位
            System.arraycopy(elementData, index+1, elementData, index,
                             numMoved);
        //数组最后一位元素赋值为nul，由gc回收
        elementData[--size] = null; // clear to let GC do its work
        //返回删除的元素
        return oldValue;
    }
//通过元素删除
public boolean remove(Object o) {
        //如果参数为null
        if (o == null) {
            //循环查找到第一个null，通过fastRemove删除
            for (int index = 0; index < size; index++)
                if (elementData[index] == null) {
                    fastRemove(index);
                    return true;
                }
        } else {
            //否则循环找到与参数元素相等的元素，删除
            for (int index = 0; index < size; index++)
                if (o.equals(elementData[index])) {
                    fastRemove(index);
                    return true;
                }
        }
        //找不到元素则返回false
        return false;
    }
//将通过索引删除的方法进行简化封装，减去了检查索引合法性和返回删除元素两部分
private void fastRemove(int index) {
        modCount++;
        int numMoved = size - index - 1;
        if (numMoved > 0)
            System.arraycopy(elementData, index+1, elementData, index,
                             numMoved);
        elementData[--size] = null; // clear to let GC do its work
    }
```

`remove`方法通过索引或者通过元素进行删除，而传入元素其实最终也是转化为通过索引寻找并删除 

删除操作时会看到`modCount++;`这个变量的自增，这里他的作用是：在ArrayList迭代器迭代时进行线程安全检查的，因为迭代过程中会检查`modCount`这个值的变化，如果发生了变化，则停止迭代并抛出异常，因为迭代器继续迭代可能会出现不可预知的情况

```
public int indexOf(Object o) {
        //循环找到null元素的索引
        if (o == null) {
            for (int i = 0; i < size; i++)
                if (elementData[i]==null)
                    return i;
        } else {
            //找到非null元素索引
            for (int i = 0; i < size; i++)
                if (o.equals(elementData[i]))
                    return i;
        }
        //找不到返回-1
        return -1;
    }

//基本一样的操作，只不过循环是从后往前
public int lastIndexOf(Object o) {
        if (o == null) {
            for (int i = size-1; i >= 0; i--)
                if (elementData[i]==null)
                    return i;
        } else {
            for (int i = size-1; i >= 0; i--)
                if (o.equals(elementData[i]))
                    return i;
        }
        return -1;
    }
```

`indexOf`和`lastIndexOf`都是通过简单的for循环找到索引，只不过循环的方向不同

```java
public boolean contains(Object o) {
        return indexOf(o) >= 0;
    }
public boolean containsAll(Collection<?> c) {
        for (Object e : c)
            if (!contains(e))
                return false;
        return true;
    }
```

`contains`方法通过调用`indexOf`方法来获取索引，如果索引大于等于0，即表示包含，`containsAll`方法通过foreach循环调用`contains`方法，来判断列表是否包含

```java
public boolean removeAll(Collection<?> c) {
        //检查对象是否为空
        Objects.requireNonNull(c);
        //传入false 删除相同元素
        return batchRemove(c, false);
    }
public boolean retainAll(Collection<?> c) {
        Objects.requireNonNull(c);
        //传入true 保留相同元素
        return batchRemove(c, true);
    }
public static <T> T requireNonNull(T obj) {
        if (obj == null)
            throw new NullPointerException();
        return obj;
    }
private boolean batchRemove(Collection<?> c, boolean complement) {
        final Object[] elementData = this.elementData;
        int r = 0, w = 0;
        boolean modified = false;
        try {
            //遍历原列表元素，removeAll方法将不存在于欲删除列表中的元素保存到原列表中
            //retainsAll方法将存在欲删除列表中的元素保存到原列表中
            for (; r < size; r++)
                if (c.contains(elementData[r]) == complement)
                    elementData[w++] = elementData[r];
        } finally {
            // Preserve behavioral compatibility with AbstractCollection,
            // even if c.contains() throws.
            if (r != size) {
                System.arraycopy(elementData, r,
                                 elementData, w,
                                 size - r);
                w += size - r;
            }
            if (w != size) {
                // clear to let GC do its work
                //通过循环将原列表除了前面保留的元素之外的后面所有元素赋值为null，又gc回收
                for (int i = w; i < size; i++)
                    elementData[i] = null;
                //更新修改元素次数
                modCount += size - w;
                //更新列表长度
                size = w;
                //列表修改标志
                modified = true;
            }
        }
        return modified;
    }
```

`removeALl`方法先调用`requireNonNull`方法，如果列表为空则抛出异常，再调用`batchRemove`，传入的参数是要删除的列表和`false`，类似的`retainsAll`方法判断是否为空后，也是调用`batchRemove`方法，但是传递的参数是要取并集的列表和`true`

```java
@Override
    @SuppressWarnings("unchecked")
    public void replaceAll(UnaryOperator<E> operator) {
        //判断对象是否为空
        Objects.requireNonNull(operator);
        final int expectedModCount = modCount;
        final int size = this.size;
        //通过循环对整理列表元素进行操作
        for (int i=0; modCount == expectedModCount && i < size; i++) {
            elementData[i] = operator.apply((E) elementData[i]);
        }
        if (modCount != expectedModCount) {
            throw new ConcurrentModificationException();
        }
        //更新修改次数
        modCount++;
    }
```

`replaceAll`方法是通过传入操作参数对整个列表元素进行操作

```
public Iterator<E> iterator() {
        return new Itr();
    }
```

`iterator`方法返回一个迭代器

```java
@Override
    public void forEach(Consumer<? super E> action) {
        //判断对象是否为空
        Objects.requireNonNull(action);
        final int expectedModCount = modCount;
        @SuppressWarnings("unchecked")
        final E[] elementData = (E[]) this.elementData;
        final int size = this.size;
        //循环对元素进行操作
        for (int i=0; modCount == expectedModCount && i < size; i++) {
            action.accept(elementData[i]);
        }
        if (modCount != expectedModCount) {
            throw new ConcurrentModificationException();
        }
        //这里不用更新修改次数
    }
```

`forEach`方法可以通过使用`lambda表达式`对列表进行遍历，代码与`replaceAll`类似，内部都是通过一个循环再利用`operator.apply`或`action.accept`对元素进行操作

```
public Object[] toArray() {
        return Arrays.copyOf(elementData, size);
    }
public <T> T[] toArray(T[] a) {
        //如果参数数组长度比列表长度小
        if (a.length < size)
            // Make a new array of a's runtime type, but my contents:
            //复制数组
            return (T[]) Arrays.copyOf(elementData, size, a.getClass());
        //复制数组
        System.arraycopy(elementData, 0, a, 0, size);
        //如果参数数组长度大于列表长度
        if (a.length > size)
            //将多余的空间设为null
            a[size] = null;
        return a;
    }
```

无参的`toArray`方法直接通过`Arrays.copyOf`将列表（实际上也是数组）复制到数组并返回，带数组对象参数的`toArray`方法先判断长度，最终通过`System.arraycopy`方法复制数组

```
public List<E> subList(int fromIndex, int toIndex) {
        subListRangeCheck(fromIndex, toIndex, size);
        return new SubList(this, 0, fromIndex, toIndex);
    }
static void subListRangeCheck(int fromIndex, int toIndex, int size) {
        if (fromIndex < 0)
            throw new IndexOutOfBoundsException("fromIndex = " + fromIndex);
        if (toIndex > size)
            throw new IndexOutOfBoundsException("toIndex = " + toIndex);
        if (fromIndex > toIndex)
            throw new IllegalArgumentException("fromIndex(" + fromIndex +
                                               ") > toIndex(" + toIndex + ")");
    }

    private class SubList extends AbstractList<E> implements RandomAccess {
        private final AbstractList<E> parent;
        private final int parentOffset;
        private final int offset;
        int size;

        SubList(AbstractList<E> parent,
                int offset, int fromIndex, int toIndex) {
            this.parent = parent;
            this.parentOffset = fromIndex;
            this.offset = offset + fromIndex;
            this.size = toIndex - fromIndex;
            this.modCount = ArrayList.this.modCount;
        }

        public E set(int index, E e) {
            rangeCheck(index);
            checkForComodification();
            E oldValue = ArrayList.this.elementData(offset + index);
            ArrayList.this.elementData[offset + index] = e;
            return oldValue;
        }

        public E get(int index) {
            rangeCheck(index);
            checkForComodification();
            return ArrayList.this.elementData(offset + index);
        }

        public int size() {
            checkForComodification();
            return this.size;
        }

        public void add(int index, E e) {
            rangeCheckForAdd(index);
            checkForComodification();
            parent.add(parentOffset + index, e);
            this.modCount = parent.modCount;
            this.size++;
        }

        public E remove(int index) {
            rangeCheck(index);
            checkForComodification();
            E result = parent.remove(parentOffset + index);
            this.modCount = parent.modCount;
            this.size--;
            return result;
        }

        protected void removeRange(int fromIndex, int toIndex) {
            checkForComodification();
            parent.removeRange(parentOffset + fromIndex,
                               parentOffset + toIndex);
            this.modCount = parent.modCount;
            this.size -= toIndex - fromIndex;
        }

        public boolean addAll(Collection<? extends E> c) {
            return addAll(this.size, c);
        }

        public boolean addAll(int index, Collection<? extends E> c) {
            rangeCheckForAdd(index);
            int cSize = c.size();
            if (cSize==0)
                return false;

            checkForComodification();
            parent.addAll(parentOffset + index, c);
            this.modCount = parent.modCount;
            this.size += cSize;
            return true;
        }

        public Iterator<E> iterator() {
            return listIterator();
        }

        public ListIterator<E> listIterator(final int index) {
            checkForComodification();
            rangeCheckForAdd(index);
            final int offset = this.offset;

            return new ListIterator<E>() {
                int cursor = index;
                int lastRet = -1;
                int expectedModCount = ArrayList.this.modCount;

                public boolean hasNext() {
                    return cursor != SubList.this.size;
                }

                @SuppressWarnings("unchecked")
                public E next() {
                    checkForComodification();
                    int i = cursor;
                    if (i >= SubList.this.size)
                        throw new NoSuchElementException();
                    Object[] elementData = ArrayList.this.elementData;
                    if (offset + i >= elementData.length)
                        throw new ConcurrentModificationException();
                    cursor = i + 1;
                    return (E) elementData[offset + (lastRet = i)];
                }

                public boolean hasPrevious() {
                    return cursor != 0;
                }

                @SuppressWarnings("unchecked")
                public E previous() {
                    checkForComodification();
                    int i = cursor - 1;
                    if (i < 0)
                        throw new NoSuchElementException();
                    Object[] elementData = ArrayList.this.elementData;
                    if (offset + i >= elementData.length)
                        throw new ConcurrentModificationException();
                    cursor = i;
                    return (E) elementData[offset + (lastRet = i)];
                }

                @SuppressWarnings("unchecked")
                public void forEachRemaining(Consumer<? super E> consumer) {
                    Objects.requireNonNull(consumer);
                    final int size = SubList.this.size;
                    int i = cursor;
                    if (i >= size) {
                        return;
                    }
                    final Object[] elementData = ArrayList.this.elementData;
                    if (offset + i >= elementData.length) {
                        throw new ConcurrentModificationException();
                    }
                    while (i != size && modCount == expectedModCount) {
                        consumer.accept((E) elementData[offset + (i++)]);
                    }
                    // update once at end of iteration to reduce heap write traffic
                    lastRet = cursor = i;
                    checkForComodification();
                }

                public int nextIndex() {
                    return cursor;
                }

                public int previousIndex() {
                    return cursor - 1;
                }

                public void remove() {
                    if (lastRet < 0)
                        throw new IllegalStateException();
                    checkForComodification();

                    try {
                        SubList.this.remove(lastRet);
                        cursor = lastRet;
                        lastRet = -1;
                        expectedModCount = ArrayList.this.modCount;
                    } catch (IndexOutOfBoundsException ex) {
                        throw new ConcurrentModificationException();
                    }
                }

                public void set(E e) {
                    if (lastRet < 0)
                        throw new IllegalStateException();
                    checkForComodification();

                    try {
                        ArrayList.this.set(offset + lastRet, e);
                    } catch (IndexOutOfBoundsException ex) {
                        throw new ConcurrentModificationException();
                    }
                }

                public void add(E e) {
                    checkForComodification();

                    try {
                        int i = cursor;
                        SubList.this.add(i, e);
                        cursor = i + 1;
                        lastRet = -1;
                        expectedModCount = ArrayList.this.modCount;
                    } catch (IndexOutOfBoundsException ex) {
                        throw new ConcurrentModificationException();
                    }
                }

                final void checkForComodification() {
                    if (expectedModCount != ArrayList.this.modCount)
                        throw new ConcurrentModificationException();
                }
            };
        }

        public List<E> subList(int fromIndex, int toIndex) {
            subListRangeCheck(fromIndex, toIndex, size);
            return new SubList(this, offset, fromIndex, toIndex);
        }

        private void rangeCheck(int index) {
            if (index < 0 || index >= this.size)
                throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
        }

        private void rangeCheckForAdd(int index) {
            if (index < 0 || index > this.size)
                throw new IndexOutOfBoundsException(outOfBoundsMsg(index));
        }

        private String outOfBoundsMsg(int index) {
            return "Index: "+index+", Size: "+this.size;
        }

        private void checkForComodification() {
            if (ArrayList.this.modCount != this.modCount)
                throw new ConcurrentModificationException();
        }

        public Spliterator<E> spliterator() {
            checkForComodification();
            return new ArrayListSpliterator<E>(ArrayList.this, offset,
                                               offset + this.size, this.modCount);
        }
    }
```

`subList`方法先通过`subListRangeCheck`检查索引合法性，然后返回一个`SubList`内部类实例 
`SubList`内部类内的api基本与外部类相同，比外部类多了外部类的列表引用，以及索引偏移量`private final int offset;`，这里就不细看了

```java
public boolean equals(Object o) {
        //同一个对象返回true
        if (o == this)
            return true;
        //如果对象不是List的实例或者子类实例，返回false
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

**`AbstractList`**重写了`equals`方法

```java
public void sort(Comparator<? super E> c) {
        final int expectedModCount = modCount;
        Arrays.sort((E[]) elementData, 0, size, c);
        if (modCount != expectedModCount) {
            throw new ConcurrentModificationException();
        }
        modCount++;
    }
```

`sort`方法，参数是实现`Comparator`接口的类实例，再调用`Arrays.sort`方法

```
public int hashCode() {
        int hashCode = 1;
        for (E e : this)
            hashCode = 31*hashCode + (e==null ? 0 : e.hashCode());
        return hashCode;
    }
```

**`AbstractList`**重写了`hashCode`方法返回列表的哈希值](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=集合类&t=blog)




