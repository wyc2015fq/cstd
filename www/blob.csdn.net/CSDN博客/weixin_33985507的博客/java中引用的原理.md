# java中引用的原理 - weixin_33985507的博客 - CSDN博客
2014年10月11日 17:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
转自:http://blog.163.com/xubin_3@126/blog/static/112987702200962211145825/
在Java中的引用类型,是指除了基本的变量类型之外的所有类型,所有的类型在内存中都会分配一定的存储空间(形参在使用的时候也会分配存储空间,方法调用完成之后,这块存储空间自动消失), 基本的变量类型只有一块存储空间(分配在stack中), 而引用类型有两块存储空间(一块在stack中,一块在heap中), **方法形参的值传递(引用)是指形参和传进来的参数指向同一个值的内存(heap)中**;
java是[传值](http://www.hudong.com/wiki/%E4%BC%A0%E5%80%BC)还是传[引用](http://www.hudong.com/wiki/%E5%BC%95%E7%94%A8)，这个估计很多人至今都很糊涂，这里有篇文章写的还是可以的，大家可以看看。。 这个写的还是比较清楚，只是不够深入。
**1. 简单类型是按值传递的**
[Java](http://www.hudong.com/wiki/Java) 方法的参数是[简单类型](http://www.hudong.com/wiki/%E7%AE%80%E5%8D%95%E7%B1%BB%E5%9E%8B)的时候，是按值传递的 (pass by value)。这一点我们可以通过一个简单的例子来说明：
```
1 /* 例 1 */
 2 
 3 /**
 4 
 5 * @(#) Test.java
 6 
 7 * @author fancy
 8 
 9 */
10 
11 public class Test {
12 
13 public static void test(boolean test) {
14 
15 　　test = ! test;
16 
17 　　System.out.println("In test(boolean) : test = " + test);
18 
19 }
20 
21 public static void main(String[] args) {
22 
23 　　boolean test = true;
24 
25 　　System.out.println("Before test(boolean) : test = " + test);
26 
27 　　test(test);
28 
29 　　System.out.println("After test(boolean) : test = " + test);
30 
31 }
32 
33 }
```
运行结果：
Before test(boolean) : test = true
In test(boolean) : test = false
After test(boolean) : test = true
不难看出，虽然在 test(boolean) 方法中改变了传进来的参数的值，但对这个参数源变量本身并没有影响，即对 main(String[]) 方法里的 test 变量没有影响。那说明，参数类型是简单类型的时候，是按值传递的。以参数形式传递简单类型的变量时，实际上是将参数的值作了一个拷贝传进方法函数的，那么在方法函数里再怎么改变其值，其结果都是只改变了拷贝的值，而不是源值。
**2. 什么是引用**
Java 是传值还是传引用，问题主要出在对象的传递上，因为 Java 中简单类型没有引用。既然争论中提到了引用这个东西，为了搞清楚这个问题，我们必须要知道引用是什么。
简单的说，引用其实就像是一个对象的名字或者别名 (alias)，一个对象在内存中会请求一块空间来保存数据，根据对象的大小，它可能需要占用的空间大小也不等。访问对象的时候，我们不会直接是访问对象在内存中的数据，而是通过引用去访问。引用也是一种数据类型，我们可以把它想象为类似 C 语言中指针的东西，它指示了对象在内存中的地址——只不过我们不能够观察到这个地址究竟是什么。
如果我们定义了不止一个引用指向同一个对象，那么这些引用是不相同的，因为引用也是一种数据类型，需要一定的内存空间来保存。但是它们的值是相同的，都指示同一个对象在内存的中位置。比如
String a = "Hello";
String b = a;
这里，a 和 b 是不同的两个引用，我们使用了两个定义语句来定义它们。但它们的值是一样的，都指向同一个对象 "Hello"。也许你还觉得不够直观，因为 String 对象的值本身是不可更改的 (像 b = "World"; b = a; 这种情况不是改变了 "World" 这一对象的值，而是改变了它的引用 b 的值使之指向了另一个 String 对象 a)。那么我们用 StringBuffer 来举一个例子：
```
1 /* 例 2 */
 2 
 3 /**
 4 
 5 * @(#) Test.java
 6 
 7 * @author fancy
 8 
 9 */
10 
11 public class Test {
12 
13 public static void main(String[] args) {
14 
15 　　StringBuffer a = new StringBuffer("Hello");
16 
17 　　StringBuffer b = a;
18 
19 　　b.append(", World");
20 
21 　　System.out.println("a is " + a);
22 
23 }
24 
25 }
```
运行结果：
a is Hello, World
这个例子中 a 和 b 都是引用，当改变了 b 指示的对象的值的时候，从输出结果来看，a 所指示的对象的值也改变了。所以，a 和 b 都指向同一个对象即包含 "Hello" 的一个 StringBuffer 对象。
这里我描述了两个要点：
1. 引用是一种数据类型，保存了对象在内存中的地址，这种类型即不是我们平时所说的简单数据类型也不是类实例(对象)；
2. 不同的引用可能指向同一个对象，换句话说，一个对象可以有多个引用，即该类类型的变量。
**3. 对象是如何传递的呢**
关于对象的传递，有两种说法，即“它是按值传递的”和“它是按引用传递的”。这两种说法各有各的道理，但是它们都没有从本质上去分析，即致于产生了争论。 既然现在我们已经知道了引用是什么东西，那么现在不妨来分析一下对象作是参数是如何传递的。还是先以一个程序为例：
```
1 /* 例 3 */
 2 
 3 /**
 4 
 5 * @(#) Test.java
 6 
 7 * @author fancy
 8 
 9 */
10 
11 public class Test {
12 
13 　　public static void test(StringBuffer str) {
14 
15 　　str.append(", World!");
16 
17 }
18 
19 public static void main(String[] args) {
20 
21 　　StringBuffer string = new StringBuffer("Hello");
22 
23 　　test(string);
24 
25 　　System.out.println(string);
26 
27 }
28 
29 }
```
运行结果：
Hello, World!
test(string) 调用了 test(StringBuffer) 方法，并将 string 作为参数传递了进去。这里 string 是一个引用，这一点是勿庸置疑的。前面提到，引用是一种数据类型，而且不是对象，所以它不可能按引用传递，所以它是按值传递的，它么它的值究竟是什么呢？是对象的地址。
由此可见，对象作为参数的时候是按值传递的，对吗？错！为什么错，让我们看另一个例子：
```
1 /* 例 4 */
 2 
 3 /**
 4 
 5 * @(#) Test.java
 6 
 7 * @author fancy
 8 
 9 */
10 
11 public class Test {
12 
13 public static void test(String str) {
14 
15 　　str = "World";
　　　　//神奇之处在于这里：“World”有自己一片内存，这句话是把"World"的内存地址给了str，使原来“Hello”的内存地址变成了"World"的内存地址。
16 
17 }
18 
19 public static void main(String[] args) {
20 
21 　　String string = "Hello";
22 
23 　　test(string);
24 
25 　　System.out.println(string);
26 
27 }
28 
29 }
```
运行结果：
Hello
为什么会这样呢？因为参数 str 是一个引用，而且它与 string 是不同的引用，虽然它们都是同一个对象的引用。str = "World" 则改变了 str 的值，使之指向了另一个对象，然而 str 指向的对象改变了，但它并没有对 "Hello" 造成任何影响，而且由于 string 和 str 是不同的引用，str 的改变也没有对 string 造成任何影响，结果就如例中所示。
其结果是推翻了参数按值传递的说法。那么，对象作为参数的时候是按引用传递的了？也错！因为上一个例子的确能够说明它是按值传递的。
结果，就像光到底是波还是?问题，其答案就只能是：即是按值传递也是按引用传递，只是参照物不同，结果也就不同。
**4. 正确看待传值还是传引用的问题**
要正确的看待这个问题必须要搞清楚为什么会有这样一个问题。
实际上，问题来源于 C，而不是 Java。
C 语言中有一种数据类型叫做指针，于是将一个数据作为参数传递给某个函数的时候，就有两种方式：传值，或是传指针，它们的区别，可以用一个简单的例子说明： /* 例 5 */
```
1 /**
 2 
 3 * @(#) test.c
 4 
 5 * @author fancy
 6 
 7 */
 8 
 9 void SwapValue(int a, int b) {
10 
11 int t = a;
12 
13 a = b;
14 
15 b = t;
16 
17 }
18 
19 void SwapPointer(int * a, int * b) {
20 
21 int t = * a;
22 
23 * a = * b;
24 
25 * b = t;
26 
27 }
28 
29 void main() {
30 
31 int a = 0, b = 1;
32 
33 printf("1 : a = %d, b = %d\n", a, b);
34 
35 SwapValue(a, b);
36 
37 printf("2 : a = %d, b = %d\n", a, b);
38 
39 SwapPointer(&a, &b);
40 
41 printf("3 : a = %d, b = %d\n", a, b);
42 
43 }
```
运行结果：
1 : a = 0, b = 1
2 : a = 0, b = 1
3 : a = 1, b = 0
大家可以明显的看到，按指针传递参数可以方便的修改通过参数传递进来的值，而按值传递就不行。
当 Java 成长起来的时候，许多的 C 程序员开始转向学习 Java，他们发现，使用类似 SwapValue 的方法仍然不能改变通过参数传递进来的简单数据类型的值，但是如果是一个对象，则可能将其成员随意更改。于是他们觉得这很像是 C 语言中传值/传指针的问题。但是 Java 中没有指针，那么这个问题就演变成了传值/传引用的问题。可惜将这个问题放在 Java 中进行讨论并不恰当。
讨论这样一个问题的最终目的只是为了搞清楚何种情况才能在方法函数中方便的更改参数的值并使之长期有效。 Java 中，改变参数的值有两种情况，第一种，使用赋值号“=”直接进行赋值使其改变，如例 1 和例 4；第二种，对于某些对象的引用，通过一定途径对其成员数据进行改变，如例 3。对于第一种情况，其改变不会影响到方法该方法以外的数据，或者直接说源数据。而第二种方法，则相反，会影响到源数据——因为引用指示的对象没有变，对其成员数据进行改变则实质上是改变的该对象。
**5. 如何实现类似 swap 的方法**
传值还是传引用的问题，到此已经算是解决了，但是我们仍然不能解决这样一个问题：如果我有两个 int 型的变量 a 和 b，我想写一个方法来交换它们的值，应该怎么办？
结论很让人失望——没有办法！因此，我们只能具体情况具体讨论，以经常使用交换方法的排序为例：
```
/** 例 6 */
/**
* @(#) Test.java
* @author fancy
*/
public class Test {
public static void swap(int[] data, int a, int b) {
　　int t = data[a];
　　data[a] = data[b];
　　data[b] = t;
}
public static void main(String[] args) {
　　int[] data = new int[10];
　　for (int i = 0; i < 10; i++) {
　　　　data[i] = (int) (Math.random() * 100);
　　　　System.out.print(" " + data[i]);
　　}
　　System.out.println();
　　for (int i = 0; i < 9; i++) {
　　　　for (int j = i; j < 10; j++) {
　　　　　　if (data[i] > data[j]) {
　　　　　　swap(data, i, j);
　　　　}
　　}
}
for (int i = 0; i < 10; i++) {
　　System.out.print(" " + data[i]);
}
　System.out.println();
}
}
```
运行结果(情况之一)：
78 69 94 38 95 31 50 97 84 1
1 31 38 50 69 78 84 94 95 97
swap(int[] data, int a, int b) 方法在内部实际上是改变了 data 所指示的对象的成员数据，即上述讨论的第二种改变参数值的方法。希望大家能够举一反三，使用类似的方法来解决相关问题。
