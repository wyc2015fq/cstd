# Java习惯用法总结 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年04月09日 13:38:19[boonya](https://me.csdn.net/boonya)阅读数：573标签：[Java																[stringbuilder																[equals																[thread																[hashcode](https://so.csdn.net/so/search/s.do?q=hashcode&t=blog)
个人分类：[Java Practice](https://blog.csdn.net/boonya/article/category/3089055)






在Java编程中，有些知识 并不能仅通过语言规范或者标准API文档就能学到的。在本文中，我会尽量收集一些最常用的习惯用法，特别是很难猜到的用法。（Joshua Bloch的《[Effective
 Java](http://www.amazon.com/gp/product/B000WJOUPA/ref=as_li_qf_sp_asin_il_tl?ie=UTF8&camp=1789&creative=9325&creativeASIN=B000WJOUPA&linkCode=as2&tag=job0ae-20)》对这个话题给出了更详尽的论述，可以从这本书里学习更多的用法。）


我把本文的所有代码都放在公共场所里。你可以根据自己的喜好去复制和修改任意的代码片段，不需要任何的凭证。

#### 目录
- 实现：
- [equals()](http://www.importnew.com/15605.html#impl_equals)
- [hashCode()](http://www.importnew.com/15605.html#impl_hashcode)
- [compareTo()](http://www.importnew.com/15605.html#impl_compareto)
- [clone()](http://www.importnew.com/15605.html#impl_clone)

- 应用：
- [StringBuilder/StringBuffer](http://www.importnew.com/15605.html#stringbuilder_stringbuffer)
- [Random.nextInt(int)](http://www.importnew.com/15605.html#random)
- [Iterator.remove()](http://www.importnew.com/15605.html#iterator)
- [StringBuilder.reverse()](http://www.importnew.com/15605.html#stringbuilder_reverse)
- [Thread/Runnable](http://www.importnew.com/15605.html#thread)
- [try-finally](http://www.importnew.com/15605.html#try)

- 输入/输出：
- [从输入流里读取字节数据](http://www.importnew.com/15605.html#reading-bytewise-inputstream)
- [从输入流里读取块数据](http://www.importnew.com/15605.html#reading-blockwise-inputstream)
- [从文件里读取文本](http://www.importnew.com/15605.html#reading-text-file)
- [向文件里写文本](http://www.importnew.com/15605.html#writing-text-file)

- 预防性检测：
- [数值](http://www.importnew.com/15605.html#defensive-checking-values)
- [对象](http://www.importnew.com/15605.html#defensive-checking-object)
- [数组索引](http://www.importnew.com/15605.html#defensive-checking-array-indices)
- [数组区间](http://www.importnew.com/15605.html#defensive-checking-array-ranges)

- 数组：
- [填充元素](http://www.importnew.com/15605.html#filling-array-elements)
- [复制一个范围内的数组元素](http://www.importnew.com/15605.html#copying-a-range-of-array-elements)
- [调整数组大小](http://www.importnew.com/15605.html#resizing-an-array)

- 包装
- [个字节包装成一个int](http://www.importnew.com/15605.html#packing-4bytes-int4)
- [分解成4个字节](http://www.importnew.com/15605.html#packing-int-4bytesint)




### 实现equals()



```
```java
class
```

```java
Person {
```



```java

```

```java
String
 name;
```



```java

```

```java
int
```

```java
birthYear;
```



```java

```

```java
byte
```

```java
[]
 raw;
```





```java

```

```java
public
```

```java
boolean
```

```java
equals(Object obj) {
```



```java

```

```java
if
```

```java
(!obj
```

```java
instanceof
```

```java
Person)
```



```java

```

```java
return
```

```java
false
```

```java
;
```





```java

```

```java
Person
 other = (Person)obj;
```



```java

```

```java
return
```

```java
name.equals(other.name)
```



```java

```

```java
&&
 birthYear == other.birthYear
```



```java

```

```java
&&
 Arrays.equals(raw, other.raw);
```



```java

```

```java
}
```





```java

```

```java
public
```

```java
int
```

```java
hashCode() { ... }
```



```java
}
```
```


- 参数必须是Object类型，不能是外围类。
- foo.equals(null) 必须返回false，不能抛NullPointerException。（注意，null instanceof 任意类 总是返回false，因此上面的代码可以运行。）
- 基本类型域（比如，int）的比较使用 == ，基本类型数组域的比较使用Arrays.equals()。
- 覆盖equals()时，记得要相应地覆盖 hashCode()，与 equals() 保持一致。
- 参考：[java.lang.Object.equals(Object)](http://docs.oracle.com/javase/6/docs/api/java/lang/Object.html#equals(java.lang.Object)。



### 实现hashCode()



```
```java
class
```

```java
Person {
```



```java

```

```java
String
 a;
```



```java

```

```java
Object
 b;
```



```java

```

```java
byte
```

```java
c;
```



```java

```

```java
int
```

```java
[]
 d;
```





```java

```

```java
public
```

```java
int
```

```java
hashCode() {
```



```java

```

```java
return
```

```java
a.hashCode() + b.hashCode() + c + Arrays.hashCode(d);
```



```java

```

```java
}
```





```java

```

```java
public
```

```java
boolean
```

```java
equals(Object o) { ... }
```



```java
}
```
```


- 当x和y两个对象具有x.equals(y) == true ，你必须要确保x.hashCode() == y.hashCode()。
- 根据逆反命题，如果x.hashCode() != y.hashCode()，那么x.equals(y) == false 必定成立。
- 你不需要保证，当x.equals(y) == false时，x.hashCode() != y.hashCode()。但是，如果你可以尽可能地使它成立的话，这会提高哈希表的性能。
- hashCode()最简单的合法实现就是简单地return 0；虽然这个实现是正确的，但是这会导致HashMap这些数据结构运行得很慢。
- 参考：[java.lang.Object.hashCode()](http://docs.oracle.com/javase/6/docs/api/java/lang/Object.html#hashCode()。



### 实现compareTo()



```
```java
class
```

```java
Person
```

```java
implements
```

```java
Comparable<Person> {
```



```java

```

```java
String
 firstName;
```



```java

```

```java
String
 lastName;
```



```java

```

```java
int
```

```java
birthdate;
```





```java

```

```java
//
 Compare by firstName, break ties by lastName, finally break ties by birthdate
```



```java

```

```java
public
```

```java
int
```

```java
compareTo(Person other) {
```



```java

```

```java
if
```

```java
(firstName.compareTo(other.firstName) !=
```

```java
0
```

```java
)
```



```java

```

```java
return
```

```java
firstName.compareTo(other.firstName);
```



```java

```

```java
else
```

```java
if
```

```java
(lastName.compareTo(other.lastName) !=
```

```java
0
```

```java
)
```



```java

```

```java
return
```

```java
lastName.compareTo(other.lastName);
```



```java

```

```java
else
```

```java
if
```

```java
(birthdate < other.birthdate)
```



```java

```

```java
return
```

```java
-
```

```java
1
```

```java
;
```



```java

```

```java
else
```

```java
if
```

```java
(birthdate > other.birthdate)
```



```java

```

```java
return
```

```java
1
```

```java
;
```



```java

```

```java
else
```



```java

```

```java
return
```

```java
0
```

```java
;
```



```java

```

```java
}
```



```java
}
```
```


- 总是实现泛型版本 Comparable 而不是实现原始类型 Comparable 。因为这样可以节省代码量和减少不必要的麻烦。
- 只关心返回结果的正负号（负/零/正），它们的大小不重要。
- [Comparator.compare()](http://docs.oracle.com/javase/6/docs/api/java/util/Comparator.html#compare(T, T)的实现与这个类似。
- 参考：[java.lang.Comparable](http://docs.oracle.com/javase/6/docs/api/java/lang/Comparable.html)。



### 实现clone()



```
```java
class
```

```java
Values
```

```java
implements
```

```java
Cloneable {
```



```java

```

```java
String
 abc;
```



```java

```

```java
double
```

```java
foo;
```



```java

```

```java
int
```

```java
[]
 bars;
```



```java

```

```java
Date
 hired;
```





```java

```

```java
public
```

```java
Values clone() {
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
Values
 result = (Values)
```

```java
super
```

```java
.clone();
```



```java

```

```java
result.bars
 = result.bars.clone();
```



```java

```

```java
result.hired
 = result.hired.clone();
```



```java

```

```java
return
```

```java
result;
```



```java

```

```java
}
```

```java
catch
```

```java
(CloneNotSupportedException e) {
```

```java
//
 Impossible
```



```java

```

```java
throw
```

```java
new
```

```java
AssertionError(e);
```



```java

```

```java
}
```



```java

```

```java
}
```



```java
}
```
```


- 使用 super.clone() 让Object类负责创建新的对象。
- 基本类型域都已经被正确地复制了。同样，我们不需要去克隆String和BigInteger等不可变类型。
- 手动对所有的非基本类型域（对象和数组）进行深度复制（deep copy）。
- 实现了Cloneable的类，clone()方法永远不要抛CloneNotSupportedException。因此，需要捕获这个异常并忽略它，或者使用不受检异常（unchecked exception）包装它。
- 不使用Object.clone()方法而是手动地实现clone()方法是可以的也是合法的。
- 参考：[java.lang.Object.clone()](http://docs.oracle.com/javase/6/docs/api/java/lang/Object.html#clone()、[java.lang.Cloneable()](http://docs.oracle.com/javase/6/docs/api/java/lang/Cloneable.html)。



### 使用StringBuilder或StringBuffer



```
```java
//
 join(["a", "b", "c"]) -> "a and b and c"
```



```java
String
 join(List<String> strs) {
```



```java

```

```java
StringBuilder
 sb =
```

```java
new
```

```java
StringBuilder();
```



```java

```

```java
boolean
```

```java
first =
```

```java
true
```

```java
;
```



```java

```

```java
for
```

```java
(String s : strs) {
```



```java

```

```java
if
```

```java
(first) first =
```

```java
false
```

```java
;
```



```java

```

```java
else
```

```java
sb.append(
```

```java
"
 and "
```

```java
);
```



```java

```

```java
sb.append(s);
```



```java

```

```java
}
```



```java

```

```java
return
```

```java
sb.toString();
```



```java
}
```
```


- 不要像这样使用重复的字符串连接：s += item ，因为它的时间效率是O(n^2)。
- 使用StringBuilder或者StringBuffer时，可以使用append()方法添加文本和使用toString()方法去获取连接起来的整个文本。
- 优先使用StringBuilder，因为它更快。StringBuffer的所有方法都是同步的，而你通常不需要同步的方法。
- 参考[java.lang.StringBuilder](http://docs.oracle.com/javase/6/docs/api/java/lang/StringBuilder.html)、[java.lang.StringBuffer](http://docs.oracle.com/javase/6/docs/api/java/lang/StringBuffer.html)。



### 生成一个范围内的随机整数



```
```java
Random
 rand =
```

```java
new
```

```java
Random();
```





```java
//
 Between 1 and 6, inclusive
```



```java
int
```

```java
diceRoll() {
```



```java

```

```java
return
```

```java
rand.nextInt(
```

```java
6
```

```java
)
 +
```

```java
1
```

```java
;
```



```java
}
```
```


- 总是使用Java API方法去生成一个整数范围内的随机数。
- 不要试图去使用 Math.abs(rand.nextInt()) % n 这些不确定的用法，因为它的结果是有偏差的。此外，它的结果值有可能是负数，比如当rand.nextInt() == Integer.MIN_VALUE时就会如此。
- 参考：[java.util.Random.nextInt(int)](http://docs.oracle.com/javase/6/docs/api/java/util/Random.html#nextInt(int)。



### 使用Iterator.remove()



```
```java
void
```

```java
filter(List<String> list) {
```



```java

```

```java
for
```

```java
(Iterator<String> iter = list.iterator(); iter.hasNext(); ) {
```



```java

```

```java
String
 item = iter.next();
```



```java

```

```java
if
```

```java
(...)
```



```java

```

```java
iter.remove();
```



```java

```

```java
}
```



```java
}
```
```


- remove()方法作用在next()方法最近返回的条目上。每个条目只能使用一次remove()方法。
- 参考：[java.util.Iterator.remove()](http://docs.oracle.com/javase/6/docs/api/java/util/Iterator.html#remove()。



### 返转字符串



```
```java
String
 reverse(String s) {
```



```java

```

```java
return
```

```java
new
```

```java
StringBuilder(s).reverse().toString();
```



```java
}
```
```


- 这个方法可能应该加入Java标准库。
- 参考：[java.lang.StringBuilder.reverse()](http://docs.oracle.com/javase/6/docs/api/java/lang/StringBuilder.html#reverse()。



### 启动一条线程


下面的三个例子使用了不同的方式完成了同样的事情。


实现Runnnable的方式：



```
```java
void
```

```java
startAThread0() {
```



```java

```

```java
new
```

```java
Thread(
```

```java
new
```

```java
MyRunnable()).start();
```



```java
}
```





```java
class
```

```java
MyRunnable
```

```java
implements
```

```java
Runnable {
```



```java

```

```java
public
```

```java
void
```

```java
run() {
```



```java

```

```java
...
```



```java

```

```java
}
```



```java
}
```
```




继承Thread的方式：



```
```java
void
```

```java
startAThread1() {
```



```java

```

```java
new
```

```java
MyThread().start();
```



```java
}
```





```java
class
```

```java
MyThread
```

```java
extends
```

```java
Thread {
```



```java

```

```java
public
```

```java
void
```

```java
run() {
```



```java

```

```java
...
```



```java

```

```java
}
```



```java
}
```
```




匿名继承Thread的方式：



```
```java
void
```

```java
startAThread2() {
```



```java

```

```java
new
```

```java
Thread() {
```



```java

```

```java
public
```

```java
void
```

```java
run() {
```



```java

```

```java
...
```



```java

```

```java
}
```



```java

```

```java
}.start();
```



```java
}
```
```


- 不要直接调用run()方法。总是调用Thread.start()方法，这个方法会创建一条新的线程并使新建的线程调用run()。
- 参考：[java.lang.Thread, java.lang.Runnable](http://docs.oracle.com/javase/6/docs/api/java/lang/Thread.html)。



### 使用try-finally


I/O流例子：



```
```java
void
```

```java
writeStuff()
```

```java
throws
```

```java
IOException {
```



```java

```

```java
OutputStream
 out =
```

```java
new
```

```java
FileOutputStream(...);
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
out.write(...);
```



```java

```

```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
out.close();
```



```java

```

```java
}
```



```java
}
```
```




锁例子：



```
```java
void
```

```java
doWithLock(Lock lock) {
```



```java

```

```java
lock.acquire();
```



```java

```

```java
try
```

```java
{
```



```java

```

```java
...
```



```java

```

```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
lock.release();
```



```java

```

```java
}
```



```java
}
```
```


- 如果try之前的语句运行失败并且抛出异常，那么finally语句块就不会执行。但无论怎样，在这个例子里不用担心资源的释放。
- 如果try语句块里面的语句抛出异常，那么程序的运行就会跳到finally语句块里执行尽可能多的语句，然后跳出这个方法（除非这个方法还有另一个外围的finally语句块）。



### 从输入流里读取字节数据



```
```java
InputStream
 in = (...);
```



```java
try
```

```java
{
```



```java

```

```java
while
```

```java
(
```

```java
true
```

```java
)
 {
```



```java

```

```java
int
```

```java
b = in.read();
```



```java

```

```java
if
```

```java
(b == -
```

```java
1
```

```java
)
```



```java

```

```java
break
```

```java
;
```



```java

```

```java
(...
 process b ...)
```



```java

```

```java
}
```



```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
in.close();
```



```java
}
```
```


- read()方法要么返回下一次从流里读取的字节数（0到255，包括0和255），要么在达到流的末端时返回-1。
- 参考：[java.io.InputStream.read()](http://docs.oracle.com/javase/6/docs/api/java/io/InputStream.html#read()。



### 从输入流里读取块数据



```
```java
InputStream
 in = (...);
```



```java
try
```

```java
{
```



```java

```

```java
byte
```

```java
[]
 buf =
```

```java
new
```

```java
byte
```

```java
[
```

```java
100
```

```java
];
```



```java

```

```java
while
```

```java
(
```

```java
true
```

```java
)
 {
```



```java

```

```java
int
```

```java
n = in.read(buf);
```



```java

```

```java
if
```

```java
(n == -
```

```java
1
```

```java
)
```



```java

```

```java
break
```

```java
;
```



```java

```

```java
(...
 process buf with offset=
```

```java
0
```

```java
and length=n ...)
```



```java

```

```java
}
```



```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
in.close();
```



```java
}
```
```


- 要记住的是，read()方法不一定会填满整个buf，所以你必须在处理逻辑中考虑返回的长度。
- 参考：[java.io.InputStream.read(byte[])](http://docs.oracle.com/javase/6/docs/api/java/io/InputStream.html#read(byte[])、[java.io.InputStream.read(byte[],
 int, int)](http://docs.oracle.com/javase/6/docs/api/java/io/InputStream.html#read(byte[], int, int)。



### 从文件里读取文本



```
```java
BufferedReader
 in =
```

```java
new
```

```java
BufferedReader(
```



```java

```

```java
new
```

```java
InputStreamReader(
```

```java
new
```

```java
FileInputStream(...),
```

```java
"UTF-8"
```

```java
));
```



```java
try
```

```java
{
```



```java

```

```java
while
```

```java
(
```

```java
true
```

```java
)
 {
```



```java

```

```java
String
 line = in.readLine();
```



```java

```

```java
if
```

```java
(line ==
```

```java
null
```

```java
)
```



```java

```

```java
break
```

```java
;
```



```java

```

```java
(...
 process line ...)
```



```java

```

```java
}
```



```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
in.close();
```



```java
}
```
```


- BufferedReader对象的创建显得很冗长。这是因为Java把字节和字符当成两个不同的概念来看待（这与C语言不同）。
- 你可以使用任何类型的InputStream来代替FileInputStream，比如socket。
- 当达到流的末端时，[BufferedReader.readLine()](http://docs.oracle.com/javase/6/docs/api/java/io/BufferedReader.html#readLine()会返回null。
- 要一次读取一个字符，使用[Reader.read()](http://docs.oracle.com/javase/6/docs/api/java/io/Reader.html#read()方法。
- 你可以使用其他的字符编码而不使用UTF-8，但最好不要这样做。
- 参考：[java.io.BufferedReader](http://docs.oracle.com/javase/6/docs/api/java/io/BufferedReader.html)、[java.io.InputStreamReader](http://docs.oracle.com/javase/6/docs/api/java/io/InputStreamReader.html)。



### 向文件里写文本



```
```java
PrintWriter
 out =
```

```java
new
```

```java
PrintWriter(
```



```java

```

```java
new
```

```java
OutputStreamWriter(
```

```java
new
```

```java
FileOutputStream(...),
```

```java
"UTF-8"
```

```java
));
```



```java
try
```

```java
{
```



```java

```

```java
out.print(
```

```java
"Hello
 "
```

```java
);
```



```java

```

```java
out.print(
```

```java
42
```

```java
);
```



```java

```

```java
out.println(
```

```java
"
 world!"
```

```java
);
```



```java
}
```

```java
finally
```

```java
{
```



```java

```

```java
out.close();
```



```java
}
```
```


- Printwriter对象的创建显得很冗长。这是因为Java把字节和字符当成两个不同的概念来看待（这与C语言不同）。
- 就像System.out，你可以使用print()和println()打印多种类型的值。
- 你可以使用其他的字符编码而不使用UTF-8，但最好不要这样做。
- 参考：[java.io.PrintWriter](http://docs.oracle.com/javase/6/docs/api/java/io/PrintWriter.html)、[java.io.OutputStreamWriter](http://docs.oracle.com/javase/6/docs/api/java/io/OutputStreamWriter.html)。



### 预防性检测（Defensive checking）数值



```
```java
int
```

```java
factorial(
```

```java
int
```

```java
n) {
```



```java

```

```java
if
```

```java
(n <
```

```java
0
```

```java
)
```



```java

```

```java
throw
```

```java
new
```

```java
IllegalArgumentException(
```

```java
"Undefined"
```

```java
);
```



```java

```

```java
else
```

```java
if
```

```java
(n >=
```

```java
13
```

```java
)
```



```java

```

```java
throw
```

```java
new
```

```java
ArithmeticException(
```

```java
"Result
 overflow"
```

```java
);
```



```java

```

```java
else
```

```java
if
```

```java
(n ==
```

```java
0
```

```java
)
```



```java

```

```java
return
```

```java
1
```

```java
;
```



```java

```

```java
else
```



```java

```

```java
return
```

```java
n * factorial(n -
```

```java
1
```

```java
);
```



```java
}
```
```


- 不要认为输入的数值都是正数、足够小的数等等。要显式地检测这些条件。
- 一个设计良好的函数应该对所有可能性的输入值都能够正确地执行。要确保所有的情况都考虑到了并且不会产生错误的输出（比如溢出）。



### 预防性检测对象



```
```java
int
```

```java
findIndex(List<String> list, String target) {
```



```java

```

```java
if
```

```java
(list ==
```

```java
null
```

```java
|| target ==
```

```java
null
```

```java
)
```



```java

```

```java
throw
```

```java
new
```

```java
NullPointerException();
```



```java

```

```java
...
```



```java
}
```
```


- 不要认为对象参数不会为空（null）。要显式地检测这个条件。



### 预防性检测数组索引



```
```java
void
```

```java
frob(
```

```java
byte
```

```java
[]
 b,
```

```java
int
```

```java
index) {
```



```java

```

```java
if
```

```java
(b ==
```

```java
null
```

```java
)
```



```java

```

```java
throw
```

```java
new
```

```java
NullPointerException();
```



```java

```

```java
if
```

```java
(index <
```

```java
0
```

```java
|| index >= b.length)
```



```java

```

```java
throw
```

```java
new
```

```java
IndexOutOfBoundsException();
```



```java

```

```java
...
```



```java
}
```
```


- 不要认为所以给的数组索引不会越界。要显式地检测它。



### 预防性检测数组区间



```
```java
void
```

```java
frob(
```

```java
byte
```

```java
[]
 b,
```

```java
int
```

```java
off,
```

```java
int
```

```java
len) {
```



```java

```

```java
if
```

```java
(b ==
```

```java
null
```

```java
)
```



```java

```

```java
throw
```

```java
new
```

```java
NullPointerException();
```



```java

```

```java
if
```

```java
(off <
```

```java
0
```

```java
|| off > b.length
```



```java

```

```java
||
 len <
```

```java
0
```

```java
|| b.length - off < len)
```



```java

```

```java
throw
```

```java
new
```

```java
IndexOutOfBoundsException();
```



```java

```

```java
...
```



```java
}
```
```


- 不要认为所给的数组区间（比如，从off开始，读取len个元素）是不会越界。要显式地检测它。



### 填充数组元素


使用循环：



```
```java
//
 Fill each element of array 'a' with 123
```



```java
byte
```

```java
[]
 a = (...);
```



```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
;
 i < a.length; i++)
```



```java

```

```java
a[i]
 =
```

```java
123
```

```java
;
```
```




（优先）使用标准库的方法：
Arrays.fill(a, (byte)123);- 参考：[java.util.Arrays.fill(T[],
 T)](http://docs.oracle.com/javase/6/docs/api/java/util/Arrays.html#fill(byte[], byte)。
- 参考：[java.util.Arrays.fill(T[],
 int, int, T)](http://docs.oracle.com/javase/6/docs/api/java/util/Arrays.html#fill(byte[], int, int, byte)。



### 复制一个范围内的数组元素


使用循环：



```
```java
//
 Copy 8 elements from array 'a' starting at offset 3
```



```java
//
 to array 'b' starting at offset 6,
```



```java
//
 assuming 'a' and 'b' are distinct arrays
```



```java
byte
```

```java
[]
 a = (...);
```



```java
byte
```

```java
[]
 b = (...);
```



```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
;
 i <
```

```java
8
```

```java
;
 i++)
```



```java

```

```java
b[
```

```java
6
```

```java
+ i] = a[
```

```java
3
```

```java
+ i];
```
```




（优先）使用标准库的方法：
System.arraycopy(a, 3, b, 6, 8);- 参考：[java.lang.System.arraycopy(Object,
 int, Object, int, int)](http://docs.oracle.com/javase/6/docs/api/java/lang/System.html#arraycopy(java.lang.Object, int, java.lang.Object, int, int)。



### 调整数组大小


使用循环（扩大规模）：



```
```java
//
 Make array 'a' larger to newLen
```



```java
byte
```

```java
[]
 a = (...);
```



```java
byte
```

```java
[]
 b =
```

```java
new
```

```java
byte
```

```java
[newLen];
```



```java
for
```

```java
(
```

```java
int
```

```java
i =
```

```java
0
```

```java
;
 i < a.length; i++)
```

```java
//
 Goes up to length of A
```



```java

```

```java
b[i]
 = a[i];
```



```java
a
 = b;
```
```




使用循环（减小规模）：
// Make array 'a' smaller to newLen
byte[] a = (...);
byte[] b = new byte[newLen];
for (int i = 0; i < b.length; i++)  // Goes up to length of B
  b[i] = a[i];
a = b;

（优先）使用标准库的方法：



```
```java
a
 = Arrays.copyOf(a, newLen);
```
```


- 参考：[java.util.Arrays.copyOf(T[],
 int)](http://docs.oracle.com/javase/6/docs/api/java/util/Arrays.html#copyOf(byte[], int)。
- 参考：[java.util.Arrays.copyOfRange(T[],
 int, int)](http://docs.oracle.com/javase/6/docs/api/java/util/Arrays.html#copyOfRange(byte[], int, int)。



### 把4个字节包装（packing）成一个int



```
```java
int
```

```java
packBigEndian(
```

```java
byte
```

```java
[]
 b) {
```



```java

```

```java
return
```

```java
(b[
```

```java
0
```

```java
]
 &
```

```java
0xFF
```

```java
)
 <<
```

```java
24
```



```java

```

```java
|
 (b[
```

```java
1
```

```java
]
 &
```

```java
0xFF
```

```java
)
 <<
```

```java
16
```



```java

```

```java
|
 (b[
```

```java
2
```

```java
]
 &
```

```java
0xFF
```

```java
)
 <<
```

```java
8
```



```java

```

```java
|
 (b[
```

```java
3
```

```java
]
 &
```

```java
0xFF
```

```java
)
 <<
```

```java
0
```

```java
;
```



```java
}
```





```java
int
```

```java
packLittleEndian(
```

```java
byte
```

```java
[]
 b) {
```



```java

```

```java
return
```

```java
(b[
```

```java
0
```

```java
]
 &
```

```java
0xFF
```

```java
)
 <<
```

```java
0
```



```java

```

```java
|
 (b[
```

```java
1
```

```java
]
 &
```

```java
0xFF
```

```java
)
 <<
```

```java
8
```



```java

```

```java
|
 (b[
```

```java
2
```

```java
]
 &
```

```java
0xFF
```

```java
)
 <<
```

```java
16
```



```java

```

```java
|
 (b[
```

```java
3
```

```java
]
 &
```

```java
0xFF
```

```java
)
 <<
```

```java
24
```

```java
;
```



```java
}
```
```





### 把int分解（Unpacking）成4个字节



```
```java
byte
```

```java
[]
 unpackBigEndian(
```

```java
int
```

```java
x) {
```



```java

```

```java
return
```

```java
new
```

```java
byte
```

```java
[]
 {
```



```java

```

```java
(
```

```java
byte
```

```java
)(x
 >>>
```

```java
24
```

```java
),
```



```java

```

```java
(
```

```java
byte
```

```java
)(x
 >>>
```

```java
16
```

```java
),
```



```java

```

```java
(
```

```java
byte
```

```java
)(x
 >>>
```

```java
8
```

```java
),
```



```java

```

```java
(
```

```java
byte
```

```java
)(x
 >>>
```

```java
0
```

```java
)
```



```java

```

```java
};
```



```java
}
```





```java
byte
```

```java
[]
 unpackLittleEndian(
```

```java
int
```

```java
x) {
```



```java

```

```java
return
```

```java
new
```

```java
byte
```

```java
[]
 {
```



```java

```

```java
(
```

```java
byte
```

```java
)(x
 >>>
```

```java
0
```

```java
),
```



```java

```

```java
(
```

```java
byte
```

```java
)(x
 >>>
```

```java
8
```

```java
),
```



```java

```

```java
(
```

```java
byte
```

```java
)(x
 >>>
```

```java
16
```

```java
),
```



```java

```

```java
(
```

```java
byte
```

```java
)(x
 >>>
```

```java
24
```

```java
)
```



```java

```

```java
};
```



```java
}
```
```


- 总是使用无符号右移操作符（>>>）对位进行包装（packing），不要使用算术右移操作符（>>）。
原文链接： [nayuki](http://www.nayuki.io/page/good-java-idioms) 翻译： [ImportNew.com](http://www.importnew.com/)- [进林](http://www.importnew.com/author/8zjl8)
译文链接： [http://www.importnew.com/15605.html](http://www.importnew.com/15605.html)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=equals&t=blog)](https://so.csdn.net/so/search/s.do?q=stringbuilder&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)




