# System类的用法 - 博客堂 - CSDN博客





2017年02月28日 09:38:02[最小森林](https://me.csdn.net/u012052268)阅读数：297








# System类的一般用法



- [System类的一般用法](#system类的一般用法)- [概述](#概述)
- [常用方法](#常用方法)- [gc](#1-gc)
- [exit 退出虚拟机](#2-exit-退出虚拟机)
- [currentTimeMillis](#3-currenttimemillis)
- [arraycopy](#4-arraycopy)






## 概述

System类包含一些有用的字段 和 方法，它不能被实例化

```
public static void gc()：运行垃圾回收器。 

public static void exit(int status)

public static long currentTimeMillis()

public static void arraycopy(Object src,int srcPos,Object dest,int destPos,int length)
```

## 常用方法：

### 1. gc()

System.gc()可用于垃圾回收。

当使用System.gc()回收某个对象所占用的内存之前，通过要求程序调用适当的方法来清理资源。在没有明确指定资源清理的情况下，Java提高了默认机制来清理该对象的资源，就是调用Object类的finalize()方法。

finalize()方法的作用是释放一个对象占用的内存空间时，会被JVM调用。而子类重写该方法，就可以清理对象占用的资源，该方法有没有链式调用，所以必须手动实现。

从程序的运行结果可以发现，执行System.gc()前，系统会自动调用finalize()方法清除对象占有的资源，通过super.finalize()方式可以实现从下到上的finalize()方法的调用，即先释放自己的资源，再去释放父类的资源。

但是，不要在程序中频繁的调用垃圾回收，因为每一次执行垃圾回收，jvm都会强制启动垃圾回收器运行，这会耗费更多的系统资源，会与正常的Java程序运行争抢资源，只有在执行大量的对象的释放，才调用垃圾回收最好

```
public class SystemDemo {
    public static void main(String[] args) {
        Person p = new Person("赵雅芝", 60);
        System.out.println(p);

        p = null; // 让p不再指定堆内存
        System.gc(); // 会先调用Person类的finalize()方法。再gc()
    }
}
```

其中 Person类的重写方法为：

```java
public class Person {
    private String name;
    private int age;


    @Override
    protected void finalize() throws Throwable {
        System.out.println("当前的对象被回收了" + this);
        super.finalize();
    }
}
```

### 2. exit() 退出虚拟机

public static void exit(int status):

终止当前正在运行的 Java 虚拟机。参数用作状态码；根据惯例，非 0 的状态码表示异常终止。 

```
System.out.println("我们喜欢林青霞(东方不败)");
System.exit(0);
System.out.println("我们也喜欢赵雅芝(白娘子)"); // 这一句是不会执行的
```

### 3. currentTimeMillis()

public static long currentTimeMillis():

返回以毫秒为单位的当前时间

应用：

```
public class SystemDemo {
    public static void main(String[] args) {
        // 单独得到这样的实际目前对我们来说意义不大
        // 那么，它到底有什么作用呢?
        // 要求：请大家给我统计这段 程序的运行时间
        long start = System.currentTimeMillis();

        for (int x = 0; x < 100000; x++) {
            System.out.println("hello" + x);
        }

        long end = System.currentTimeMillis();
        System.out.println("共耗时：" + (end - start) + "毫秒");
    }
}
```

### 4. arraycopy()

public static void arraycopy(Object src,int srcPos,Object dest,int destPos,int length)

从指定源数组中复制一个数组，复制从指定的位置开始，到目标数组的指定位置结束。

```
public class SystemDemo {
    public static void main(String[] args) {
        // 定义数组
        int[] arr = { 11, 22, 33, 44, 55 };
        int[] arr2 = { 6, 7, 8, 9, 10 };

        // 请大家看这个代码的意思
        System.arraycopy(arr, 1, arr2, 2, 2);

        System.out.println(Arrays.toString(arr));   //  结果不变
        System.out.println(Arrays.toString(arr2));  //  结果：6 7 22 33 10
    }
}
```



