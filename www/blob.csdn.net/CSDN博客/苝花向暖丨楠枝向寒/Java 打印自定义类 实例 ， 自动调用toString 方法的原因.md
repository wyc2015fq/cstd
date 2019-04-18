# Java 打印自定义类 实例 ， 自动调用toString 方法的原因 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年01月19日 14:34:29[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：136


PrintStream.java

    public void println(Object x) {
        String s = String.valueOf(x);
        synchronized (this) {
            print(s);
            newLine();
        }
    }


String.java

    public static String valueOf(Object obj) {
        return (obj == null) ? "null" : obj.toString();
    }


通过源码可知，实际上输出语句 是调用了 String.balueOf(Object obj)  方法，这个方法主要判断我们要输出的内容是否为空，

为空则 返回 null  不为空则 调用 toString 方法。 注意，当我们 输出的内容是 自定义类时，需要 重写Object 中的toString()方法,才能按照我们想要输出的方式输出。 且 打印时会自动调用，无需手动调用。  


