# Java 程序执行顺序

 

# Java程序执行顺序

Java程序的执行顺序大概可以分成三个部分。

> 1.静态属性：静态方法声明，静态块。 
>    2.动态属性：普通方法声明，构造块。 
>    3.构造方法。

而如果类与类之间存在继承关系的话，那么会先执行父类的静态属性，接着执行子类的静态属性；然后是父类的动态属性，父类的构造方法；接着执行子类的动态属性；子类的构造方法。至此，Java程序便执行完成。借用[想知道Java代码执行顺序么，Let’s go](https://www.jianshu.com/p/55c86e6c5c60)这篇博文的图，如下所示。

![java程序执行顺序](https://img-blog.csdn.net/20180122212730403?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMzU5NTQxOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# Java程序执行例子

```Java
public class Text {
    public static int k = 0;
    public static Text t1 = new Text("t1");
    public static Text t2 = new Text("t2");
    public static int i = print("i");
    public static int n = 99;
    public int j = print("j");

    {
        print("构造块");
    }

    static {
        print("静态块");
    }

    public Text(String str) {
        System.out.println((++k) + ":" + str + "   i=" + i + "    n=" + n);
        ++i;
        ++n;
    }

    public static int print(String str) {
        System.out.println((++k) + ":" + str + "   i=" + i + "    n=" + n);
        ++n;
        return ++i;
    }

    public static void main(String args[]) {
        Text t = new Text("init");
    }
}
```

在本程序一开始，因为使用自身的构造方法创建了新的对象t1, t2。这里在内存中便出现了`内存压栈`的情形。

当执行完

```Java
public static Text t1 = new Text("t1");
```

这句时： 
 最开始的`Text`类便被入栈，那么在此时，新的`Text`类会变成类似于下述的样子。 
 注意，因为

```Java
public static int k = 0;
```

已经被最开始的`Text`类执行，因此新的`Text`类并不会再次执行上述语句。

```Java
public class Text {
    // public static Text t1 = new Text("t1");
    public int j = print("j");

    {
        print("构造块");
    }

    public static int print(String str) {
        System.out.println((++k) + ":" + str + "   i=" + i + "    n=" + n);
        ++n;
        return ++i;
    }

    public Text(String str) {
        System.out.println((++k) + ":" + str + "   i=" + i + "    n=" + n);
        ++i;
        ++n;
    }
}
```

那么按照**Java程序执行顺序**一节的介绍，执行完成后会打印

```
1:j       i=0    n=0
2:构造块   i=1    n=1
3:t1      i=2    n=2
```

此时，该`Text`类成功创建了对象t1。同理创建对象t2，打印出

```
4:j       i=3    n=3
5:构造块   i=4    n=4
6:t2      i=5    n=5123
```

此时程序已经执行到（但暂未执行）。这里应该注意到，上述新的`Text`类已经完全执行结束。这里执行的`Text`类则是最开始的`Text`类

```Java
public static int i = print("i");
```

执行上述语句后打印出

```
7:i       i=6    n=6
```

接着执行

```Java
public static int n = 99;
```

执行完成后，虽然没有打印任何东西，但是应该注意到`n`已经发生了变化。

根据**Java程序执行顺序**一节的介绍的流程，会接着执行

```Java
    static {
        print("静态块");
    }
```

执行完成后，打印下述结果。

```
8:静态块   i=7    n=99
```

接着执行

```Java
public int j = print("j");
```

执行后打印出下述结果

```
9:j       i=8    n=1001
```

这里已经完成了类的静态属性，静态代码块的初始化工作。最后执行`main`这个特殊的静态方法。打印出下述结果。

```
10:构造块  i=9    n=101
11:init   i=10   n=102
```

到这里，整个Java程序便执行完成。最终结果如下所述。

为了方便查看，对格式进行了调整

```text
1:j       i=0    n=0
2:构造块   i=1    n=1
3:t1      i=2    n=2
4:j       i=3    n=3
5:构造块   i=4    n=4
6:t2      i=5    n=5
7:i       i=6    n=6
8:静态块   i=7    n=99
9:j       i=8    n=100
10:构造块  i=9    n=101
11:init   i=10   n=102
```

------

[深入了解Java程序执行顺序](https://www.cnblogs.com/greatfish/p/5771548.html) 
 [想知道Java代码执行顺序么，Let’s go](https://www.jianshu.com/p/55c86e6c5c60)