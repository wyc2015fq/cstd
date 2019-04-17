# 字符串String源码分析 - PeterBishop - CSDN博客





2018年10月01日 09:15:20[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：28








### 上节课我们讲了String是什么，怎么使用，这节课我们就来分析分析String的底层源码。

### public final class String

### implements java.io.Serializable, Comparable<String>, CharSequence {

### 可以看到String类实现了三个接口，前面两个我们已经见过了，就是序列化和可比较，最后一个CharSequence是字符队列，接下来看看CharSequence的代码。

### public interface CharSequence {

### int length();

### char charAt(int index);

### CharSequence subSequence(int start, int end);

### public String toString();

### public default IntStream chars() {

### class CharIterator implements PrimitiveIterator.OfInt {

### int cur = 0;

###             public boolean hasNext() {

### return cur < length();

### }

### public int nextInt() {

### if (hasNext()) {

### return charAt(cur++);

### } else {

### throw new NoSuchElementException();

### }

###             }

### @Override

### public void forEachRemaining(IntConsumer block) {

### for (; cur < length(); cur++) {

###                     block.accept(charAt(cur));

### }

###             }

###         }

### return StreamSupport.*intStream*(() ->

###                 Spliterators.*spliterator*(

### new CharIterator(),

### length(),

### Spliterator.*ORDERED*),

### Spliterator.*SUBSIZED *| Spliterator.*SIZED *| Spliterator.*ORDERED*,

###                 false);

### }

### public default IntStream codePoints() {

### class CodePointIterator implements PrimitiveIterator.OfInt {

### int cur = 0;

### @Override

### public void forEachRemaining(IntConsumer block) {

### final int length = length();

###                 int i = cur;

###                 try {

### while (i < length) {

### char c1 = charAt(i++);

###                         if (!Character.*isHighSurrogate*(c1) || i >= length) {

###                             block.accept(c1);

### } else {

### char c2 = charAt(i);

###                             if (Character.*isLowSurrogate*(c2)) {

###                                 i++;

### block.accept(Character.*toCodePoint*(c1, c2));

### } else {

###                                 block.accept(c1);

### }

###                         }

###                     }

###                 } finally {

### cur = i;

### }

###             }

### public boolean hasNext() {

### return cur < length();

### }

### public int nextInt() {

### final int length = length();

###                 if (cur >= length) {

### throw new NoSuchElementException();

### }

### char c1 = charAt(cur++);

###                 if (Character.*isHighSurrogate*(c1) && cur < length) {

### char c2 = charAt(cur);

###                     if (Character.*isLowSurrogate*(c2)) {

### cur++;

###                         return Character.*toCodePoint*(c1, c2);

### }

###                 }

### return c1;

### }

###         }

### return StreamSupport.*intStream*(() ->

###                 Spliterators.*spliteratorUnknownSize*(

### new CodePointIterator(),

### Spliterator.*ORDERED*),

### Spliterator.*ORDERED*,

###                 false);

### }

### }

### 解释一下为什么接口里的一些方法有具体实现。

### 可以看到这些被实现了的方法在声明的时候都加了default关键字，这个是jdk1.8的新特性，表示默认方法，加了这个关键字方法就可以在接口里实现

### 我们主要看那些未被实现的方法。

### int length();      //返回字符队列的长度    char charAt(int index);    //返回index位置的字符    CharSequence subSequence(int start, int end);     //提取字串，返回值类型为CharSequence    public String toString();   //将本对象转换成字符串

### 接下来看看String类

### */** The value is used for character storage. */*

### private final char value[];

### */** Cache the hash code for the string */*

### private int hash; // Default to 0

### */** use serialVersionUID from JDK 1.0.2 for interoperability */*

### private static final long *serialVersionUID *= -6849794470754667710L;

### 这些是String类的属性

### 主要看value

### value属性是干什么的？

### value是一个字符数组char[]，用于储存字符串的值，一切对字符串的操作本质是对这个字符操作。

### 接下来看几个构造器

### public String() {

### this.value = "".value;

### }

### public String(String original) {

### this.value = original.value;

###     this.hash = original.hash;

### }

### public String(char value[]) {

### this.value = Arrays.*copyOf*(value, value.length);

### }

### 说明一下，Arrays.copyOf的功能是构造一个新的数组，然后将原数组里面的元素复制一份放到新数组里，具体源码太复杂了，就不看了。

### 我们主要用到的就这三个构造器，其他的用的不多。

### 接下来看一些方法的实现。

### public int length() {

### return value.length;

### }

### 可以看到length方法就是直接返回数组的长度，这也说明了value就是保存字符串内容的对象，而对字符串的操作本质上是对value的操作

### public boolean isEmpty() {

### return value.length == 0;

### }

### isEmpty方法用于判断字符串是否是空串(“”)，如果为空则返回true，否则返回false

### public char charAt(int index) {

### if ((index < 0) || (index >= value.length)) {

### throw new StringIndexOutOfBoundsException(index);

### }

### return value[index];

### }

### throw是抛异常，这个以后说，可以看到返回值就是value[index]

### void getChars(char dst[], int dstBegin) {

###     System.*arraycopy*(value, 0, dst, dstBegin, value.length);

### }

### getChars()方法用于提取字串到dst，具体作用是复制value到dst的dstBegin位置

### public void getChars(int srcBegin, int srcEnd, char dst[], int dstBegin) {

### if (srcBegin < 0) {

### throw new StringIndexOutOfBoundsException(srcBegin);

### }

### if (srcEnd > value.length) {

### throw new StringIndexOutOfBoundsException(srcEnd);

### }

### if (srcBegin > srcEnd) {

### throw new StringIndexOutOfBoundsException(srcEnd - srcBegin);

### }

###     System.*arraycopy*(value, srcBegin, dst, dstBegin, srcEnd - srcBegin);

### }

### 这个很显然是提取子串复制到dst的dstBegin位置

### public boolean equals(Object anObject) {

### if (this == anObject) {

### return true;

### }

### if (anObject instanceof String) {

###         String anotherString = (String)anObject;

###         int n = value.length;

###         if (n == anotherString.value.length) {

### char v1[] = value;

###             char v2[] = anotherString.value;

###             int i = 0;

###             while (n-- != 0) {

### if (v1[i] != v2[i])

### return false;

### i++;

### }

### return true;

### }

###     }

### return false;

### }

### 判断字符串和anObject对象的值是否相等，具体表现为

### 首先看本引用变量和anObject是否引用同一对象，如果是，则返回true，如果不是

### 看anObject是不是String类型，如果不是直接返回false，否则

### 强转一下类型，然后比较本对象和anotherString的value是否一样，不一样就         返回false，否则返回true

### public boolean startsWith(String prefix, int toffset) {

### char ta[] = value;

###     int to = toffset;

###     char pa[] = prefix.value;

###     int po = 0;

###     int pc = prefix.value.length;

### // Note: toffset might be near -1>>>1.

### if ((toffset < 0) || (toffset > value.length - pc)) {

### return false;

### }

### while (--pc >= 0) {

### if (ta[to++] != pa[po++]) {

### return false;

### }

###     }

### return true;

### }

### public boolean startsWith(String prefix) {

### return startsWith(prefix, 0);

### }

### startwith，判断字符串是否以某个字符串开头

### 还有个endWith是判断字符串是否以某个字符串结尾。

### public int indexOf(String str)

### 返回str第一次在本字符串对象中出现的位置，如果不存在返回-1

### 还有个lastIndexOf，返回str最后一次在本字符串对象中出现的位置，如果不存在返回-1

### public String substring(int beginIndex, int endIndex) {

### if (beginIndex < 0) {

### throw new StringIndexOutOfBoundsException(beginIndex);

### }

### if (endIndex > value.length) {

### throw new StringIndexOutOfBoundsException(endIndex);

### }

### int subLen = endIndex - beginIndex;

###     if (subLen < 0) {

### throw new StringIndexOutOfBoundsException(subLen);

### }

### return ((beginIndex == 0) && (endIndex == value.length)) ? this

### : new String(value, beginIndex, subLen);

### }

### substring，提取字串，可以看到返回的是否new了一个新的字串

### public String substring(int beginIndex) {

### if (beginIndex < 0) {

### throw new StringIndexOutOfBoundsException(beginIndex);

### }

### int subLen = value.length - beginIndex;

###     if (subLen < 0) {

### throw new StringIndexOutOfBoundsException(subLen);

### }

### return (beginIndex == 0) ? this : new String(value, beginIndex, subLen);

### }

### substring如果只传一个参数就提取beinIndex直到字符串结尾的字串，返回的时候还是new了一个新的String

### public CharSequence subSequence(int beginIndex, int endIndex) {

### return this.substring(beginIndex, endIndex);

### }

### 这个不用我说都看得懂

### public String concat(String str) {

### int otherLen = str.length();

###     if (otherLen == 0) {

### return this;

### }

### int len = value.length;

###     char buf[] = Arrays.*copyOf*(value, len + otherLen);

### str.getChars(buf, len);

###     return new String(buf, true);

### }

### 这个就是连接字符串

### public String replace(CharSequence target, CharSequence replacement)

### 由于String实现了CharSequence接口，可以直接传String对象，作用是用replacement替换target，然后返回被替换后的String对象，注意，replace不会对原对象造成影响，而是new一个新的String对象来进行replace操作

### 之后还有replaceFirst和replaceAll，作用分别是替换第一次和替换所有

### public String[] split(String regex)

### 将字符串对象以regex分割成字符串数组

### 比如:

### public static void main(String[] args) {    String s = "com.helloworld.teach";    String[] split = s.split("\\.");    //这个\\.是转义字符，regex是一个正则表达式，这个以后再讲    for (String s1 : split) {        System.out.println(s1);    }    //再举一个例子    String str = "aaaa/b/ccccd/b/aaaccc";    String[] bs = str.split("b");    for (String b : bs) {        System.out.println(b);    }}

### 运行结果:

### com

### helloworld

### teach

### aaaa/

### /ccccd/

### /aaaccc

### 最后说一点:

### 字符串可以用 + 来连接

### 字符串和数字做+运算会先将数字变成字符串然后进行字符串连接操作



