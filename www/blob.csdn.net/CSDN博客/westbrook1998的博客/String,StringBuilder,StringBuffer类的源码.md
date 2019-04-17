# String,StringBuilder,StringBuffer类的源码 - westbrook1998的博客 - CSDN博客





2018年03月20日 21:47:28[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：48标签：[java																[源代码																[字符串](https://so.csdn.net/so/search/s.do?q=字符串&t=blog)
个人分类：[java](https://blog.csdn.net/westbrook1998/article/category/7459913)





## StringBuilder类

继承`AbstractStringBuilder`类 

实现`java.io.Serializable, CharSequence`接口

### 构造器

无参构造器默认调用父类构造器，传入参数16为默认容量

```java
public StringBuilder() {
        super(16);
}
```

第一个带参构造调用父类带参构造，手动传入参数确定容量

```java
public StringBuilder(int capacity) {
        super(capacity);
}
```

第二个带参构造是传入字符串类型，调用父类构造器并传入参数使容量比原字符串长度多16，再加入字符串

```
public StringBuilder(String str) {
    super(str.length() + 16);
    append(str);
}
```

第四个带参构造传入的参数类型的是CharSequence接口类型,而实现该接口的主要有CharBuffer,StringBuffer,AbstractStringBuilder,StringBuilder,String(java.lang),Segment(java.swing)等

```
public StringBuilder(CharSequence seq) {
        this(seq.length() + 16);
        append(seq);
}
```

### 重写方法

#### append

```java
@Override
    public StringBuilder append(Object obj) {
        return append(String.valueOf(obj));
    }

    @Override
    public StringBuilder append(String str) {
        super.append(str);
        return this;
    }

    /**
     * Appends the specified {@code StringBuffer} to this sequence.
     * <p>
     * The characters of the {@code StringBuffer} argument are appended,
     * in order, to this sequence, increasing the
     * length of this sequence by the length of the argument.
     * If {@code sb} is {@code null}, then the four characters
     * {@code "null"} are appended to this sequence.
     * <p>
     * Let <i>n</i> be the length of this character sequence just prior to
     * execution of the {@code append} method. Then the character at index
     * <i>k</i> in the new character sequence is equal to the character at
     * index <i>k</i> in the old character sequence, if <i>k</i> is less than
     * <i>n</i>; otherwise, it is equal to the character at index <i>k-n</i>
     * in the argument {@code sb}.
     *
     * @param   sb   the {@code StringBuffer} to append.
     * @return  a reference to this object.
     */
    public StringBuilder append(StringBuffer sb) {
        super.append(sb);
        return this;
    }

    @Override
    public StringBuilder append(CharSequence s) {
        super.append(s);
        return this;
    }

    /**
     * @throws     IndexOutOfBoundsException {@inheritDoc}
     */
    @Override
    public StringBuilder append(CharSequence s, int start, int end) {
        super.append(s, start, end);
        return this;
    }

    @Override
    public StringBuilder append(char[] str) {
        super.append(str);
        return this;
    }

    /**
     * @throws IndexOutOfBoundsException {@inheritDoc}
     */
    @Override
    public StringBuilder append(char[] str, int offset, int len) {
        super.append(str, offset, len);
        return this;
    }

    @Override
    public StringBuilder append(boolean b) {
        super.append(b);
        return this;
    }

    @Override
    public StringBuilder append(char c) {
        super.append(c);
        return this;
    }

    @Override
    public StringBuilder append(int i) {
        super.append(i);
        return this;
    }

    @Override
    public StringBuilder append(long lng) {
        super.append(lng);
        return this;
    }

    @Override
    public StringBuilder append(float f) {
        super.append(f);
        return this;
    }

    @Override
    public StringBuilder append(double d) {
        super.append(d);
        return this;
    }
```

第一二个，接受`Object`对象，再通过`String.valueOf()`，转成String类型，或者直接传入String类型，再调用父类`append`方法 

第三四五个，接受`StringBuffer`和`CharSequence`类型参数，以及可选的表示边界的参数（start，end） 

第六七个，接受`char[]`字符数组类型，以及可选的表示边界的参数（offset表示开始的第一个字符，len表示要传入的长度） 

剩下第八九十十一十二十三个分别是接受六个基本类型`boolean char int long double float`
再看到父类`AbstractStringBuilder`的`append`方法，这个方法是重写了`Appendable`接口的`append`方法

```
public AbstractStringBuilder append(Object obj) {
        return append(String.valueOf(obj));
    }
```

参数为Object类，转成字符串再调用参数为字符串的append

```
public AbstractStringBuilder append(String str) {
        if (str == null)
            return appendNull();
        int len = str.length();
        ensureCapacityInternal(count + len);
        str.getChars(0, len, value, count);
        count += len;
        return this;
    }
```

先判断是否为null，是，则调用`appendNull()`，返回字符串null

```
private AbstractStringBuilder appendNull() {
        int c = count;
        ensureCapacityInternal(c + 4);
        final char[] value = this.value;
        value[c++] = 'n';
        value[c++] = 'u';
        value[c++] = 'l';
        value[c++] = 'l';
        count = c;
        return this;
    }
```

因为是append的方法，所以要保存这个字符串的长度`count`，而`length()`方法返回`count`，再调用`ensureCapacityInternal()`方法扩充字符数组

```
private void ensureCapacityInternal(int minimumCapacity) {
        // overflow-conscious code
        if (minimumCapacity - value.length > 0) {
            value = Arrays.copyOf(value,
                    newCapacity(minimumCapacity));
        }
    }
```

利用`Arrays.copyOf`的方法将`value`字符数组的数据复制到一个新的数组里，仍然赋值给`value`，而构造新数组使用了`newCapacity()`这个方法

```
private int newCapacity(int minCapacity) {
        // overflow-conscious code
        int newCapacity = (value.length << 1) + 2;
        if (newCapacity - minCapacity < 0) {
            newCapacity = minCapacity;
        }
        return (newCapacity <= 0 || MAX_ARRAY_SIZE - newCapacity < 0)
            ? hugeCapacity(minCapacity)
            : newCapacity;
    }
```

第一句`int newCapacity = (value.length << 1) + 2;`的作用是自动扩容，即原数组容量`乘2加2`，这里使用了位运算，然后判断，如果扩容后容量仍然小于传入的参数（即需求容量），则将需求容量作为新容量，最后判断返回，如果新容量`小于等于0`或者`大于MAX_ARRAY_SIZE`这个常量（`private static final int MAX_ARRAY_SIZE = Integer.MAX_VALUE - 8;`），即返回调用`hugeCapacity()`（**传入的参数是需求容量而不是新容量，因为前面的if不一定成立，因此新容量和需求容量不一定相等**）这个方法，否则返回新容量

```
private int hugeCapacity(int minCapacity) {
        if (Integer.MAX_VALUE - minCapacity < 0) { // overflow
            throw new OutOfMemoryError();
        }
        return (minCapacity > MAX_ARRAY_SIZE)
            ? minCapacity : MAX_ARRAY_SIZE;
    }
```

如果传入需求容量大于int最大值，则抛出异常，否则返回需求容量和数组最大容量（`MAX_ARRAY_SIZE`）中的最大值

到达最深处后开始返回，`newCapacity(minimumCapacity)`返回一个新的容量值给`Arrays.copyOf()`，作为第二个参数（复制的长度，如果大于原数组长度，即为空），复制完得到新数组后，`final char[] value = this.value;`构造一个字符数组常量

```
value[c++] = 'n';
        value[c++] = 'u';
        value[c++] = 'l';
        value[c++] = 'l';
        count = c;
```

添加null字符串后将`c`（字符串长度）赋值给`count`，最后`return this`返回调用方法的该对象 

回到`AbstractStringBuilder`的`append`方法

```
public AbstractStringBuilder append(String str) {
        if (str == null)
            return appendNull();
        int len = str.length();
        ensureCapacityInternal(count + len);
        str.getChars(0, len, value, count);
        count += len;
        return this;
    }
```

先获取传入的要添加的字符串长度，调用`ensureCapacityInternal`扩容，再调用`getChars`方法，最后`count`加上添加的字符串长度，返回该对象

```
public void getChars(int srcBegin, int srcEnd, char dst[], int dstBegin) {
        if (srcBegin < 0) {
            throw new StringIndexOutOfBoundsException(srcBegin);
        }
        if (srcEnd > value.length) {
            throw new StringIndexOutOfBoundsException(srcEnd);
        }
        if (srcBegin > srcEnd) {
            throw new StringIndexOutOfBoundsException(srcEnd - srcBegin);
        }
        System.arraycopy(value, srcBegin, dst, dstBegin, srcEnd - srcBegin);
    }
```

字符串的`getChars`方法，先判断一些非法参数抛出异常，然后调用`System.arraycopy`方法，这是一个本地方法，总之就是将参数中的value数组复制到调用该方法的字符串后面 

最后完成这个`append`方法

```java
public AbstractStringBuilder append(StringBuffer sb) {
        if (sb == null)
            return appendNull();
        int len = sb.length();
        ensureCapacityInternal(count + len);
        sb.getChars(0, len, value, count);
        count += len;
        return this;
    }

    /**
     * @since 1.8
     */
    AbstractStringBuilder append(AbstractStringBuilder asb) {
        if (asb == null)
            return appendNull();
        int len = asb.length();
        ensureCapacityInternal(count + len);
        asb.getChars(0, len, value, count);
        count += len;
        return this;
    }
```

接下来的两个`append`基本相同，只是参数类型的不同，分别是`StringBuffer`和`AbstractStringBuilder`

```java
public AbstractStringBuilder append(CharSequence s) {
        if (s == null)
            return appendNull();
        if (s instanceof String)
            return this.append((String)s);
        if (s instanceof AbstractStringBuilder)
            return this.append((AbstractStringBuilder)s);

        return this.append(s, 0, s.length());
    }
```

最后一个是参数类型为`CharSequence`**（`String`类和`AbstractStringBuilder`类都实现了`CharSequence`接口，而`StringBuilder`类继承了`AbstractStringBuilder`类，也实现了`CharSequence`接口）**的方法，方法很简单，判断`null`，判断对象是否为`String`类或者`AbstractStringBuilder`类子类，如果是，转换类型后调用相应参数方法，如果不是，再调用另一个`append`方法

```
public AbstractStringBuilder append(CharSequence s, int start, int end) {
        if (s == null)
            s = "null";
        if ((start < 0) || (start > end) || (end > s.length()))
            throw new IndexOutOfBoundsException(
                "start " + start + ", end " + end + ", s.length() "
                + s.length());
        int len = end - start;
        ensureCapacityInternal(count + len);
        for (int i = start, j = count; i < end; i++, j++)
            value[j] = s.charAt(i);
        count += len;
        return this;
    }
```

这是一个参数为`CharSequence`的`append`方法，容易看出，先判断`null`，再判断范围和合法性，如果都合法，则通过一个for循环将字符串添加上去](https://so.csdn.net/so/search/s.do?q=源代码&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




