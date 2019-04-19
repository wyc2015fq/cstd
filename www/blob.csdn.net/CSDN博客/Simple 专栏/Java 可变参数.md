# Java 可变参数 - Simple 专栏 - CSDN博客
2012年01月14日 15:05:24[Simple_Zz](https://me.csdn.net/love284969214)阅读数：1077标签：[Java面试																[Java基础																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=Java基础&t=blog)](https://so.csdn.net/so/search/s.do?q=Java面试&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
Java在1.5之后允许方法使用可变参数，可变参数的好处在于：它允许传递0个或者多个参数。
比如原来有一段代码如下：
```java
public class Test {
    public static void main(String[] args) {
        test();
        test(new String[]{"a"});
        test(new String[]{"a", "b"});
    }
    private static void test() {
        System.out.println(“[]”);
    }
    private static void test(String[] args) {
        System.out.println(Arrays.toString(args));
    }
}
```
使用可变参数之后的代码如下：
```java
public class Test {
    
    public static void main(String[] args) {
        test();
        test(new String[]{"a"});
        test(new String[]{"a", "b"});
    }
    private static void test(String... args) {
        System.out.println(Arrays.toString(args));
    }
}
```
可见，可变参数的功能更加强大，语意包含的范围更广。
我们知道，在Java中，除了8种基本类型之外一切都是类。那么可变参数到底是个什么类呢？来，用代码验证下！
```java
public class Test {
    public static void main(String[] args) {
        whatClass();
        whatClass1();
    }
    private static void whatClass(String... args) {
        System.out.println(args.getClass());
        System.out.println(new String[]{}.getClass());
    }
    private static void whatClass1(int... args) {
        System.out.println(args.getClass());
        System.out.println(new int[]{}.getClass());
    }
}
/* 这段代码运行的结果是
 * class [Ljava.lang.String;
 * class [Ljava.lang.String;
 * class [I
 * class [I
 */
```
可见，可变参数在运行时还是会被JVM转变成一个数组，所以当我们使用可变参数的时候，实际上又一个隐含的过程，就是创建一个数组并且赋初值。当我们对性能敏感时，这一特性可能会坑了我们，为何这样说？看下面的代码：
```java
public class Test {
    public static void main(String[] args) {
        testRunTime();
    }
    private static void testRunTime() {
        long start = System.currentTimeMillis();
        for (int a = 0; a < Integer.MAX_VALUE; a++) {
            for (int b = 0; b < Integer.MAX_VALUE; b++) {
                test1(1);
            }
        }
        long end = System.currentTimeMillis();
        System.out.println("test1-->" + (end - start));
		
        start = System.currentTimeMillis();
        for (int a = 0; a < Integer.MAX_VALUE; a++) {
            for (int b = 0; b < Integer.MAX_VALUE; b++) {
                test2(1);
            }
        }
        end = System.currentTimeMillis();
        System.out.println("test2-->" + (end - start));
    }
    private static int test1(int first) {
        return first;
    }
    private static int[] test2(int... args) {
        return args;
    }
}
```
test2的时间大于test1的时间，这种差别随着循环次数的增大越来越明显。这种性能的参数就是可变参数的“功劳”。因此，我们总见到这样的方法重载：
```java
public void fun(int val1){}
public void fun(int val1,int val2){}
public void fun(int val1,int val2,int val3){}
public void fun(int val1,int val2,int val3,int... args){}
```
当fun方法的大多数调用都是有1-3个参数值的时候，使用上面的方法比仅有一个可变参数的方法性能要好的多，因为很少能用到可变参数的方法。这种例子最常见的就是JDK中的EnumSet类型的of()方法。看过的小伙伴可能对of(E first, E... rest)方法有些疑问，这个方法和of(E first,E second)等方法是“冲突”的啊？
实际上，Java是很聪明的。Java语法规定，可变参数必须在所有参数的最后声明，而且Java会首先寻找满足条件的不包含可变参数的方法，如果没找到，再使用包含可变参数的方法，看下面的例子：
```java
public class Test {
    public static void main(String[] args) {
        fun(1);
        fun(1, 2);
        fun(1, 2, 3);
        fun(1, 2, 3, 4);
        fun(1, 2, 3, 4, 5);
    }
    private static void fun(int val1) {
        System.out.println("fun(int val1)");
    }
    private static void fun(int val1, int val2) {
        System.out.println("fun(int val1, int val2)");
    }
    private static void fun(int val1, int val2, int val3) {
        System.out.println("fun(int val1,int val2,int val3)");
    }
    private static void fun(int val1, int... args) {
        System.out.println("fun(int val1, int val2, int val3, int... args)");
    }
}
/** 输出结果为
 * fun(int val1)
 * fun(int val1, int val2)
 * fun(int val1,int val2,int val3)
 * fun(int val1, int val2, int val3, int... args)
 * fun(int val1, int val2, int val3, int... args)
 */
```
