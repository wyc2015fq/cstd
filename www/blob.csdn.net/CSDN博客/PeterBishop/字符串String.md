# 字符串String - PeterBishop - CSDN博客





2018年10月01日 09:11:51[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：23
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









### 这节课我们来讲讲字符串String

### 还记得我们在第1课输出的HelloWorld吗？

### System.out.println(“Hello World!”);

### 这里的”Hello World”其实就是一个字符串。

### 可以看到”Hello World”其实是一个对象，它的类型是java.lang.String,也就是我们要讲的字符串类

### Java语法里允许字符串常量对象不被引用对象引用，也就是说允许出现”Hello World”.getClass()这种写法

### 接下来看看String类的使用方法

### public class StringTeach {    public static void main(String[] args) {        //创建String类型的变量并初始化        String str = "Hello World";        //上面一行相当于（并不严谨，看最后补充）        String s = new String("Hello World");        //每次给String类型的变量赋未被引用的字符串常量时都会构建新的对象        //接下来看看String类的一些常用方法        System.out.println(s.length()); //String.length() 返回字符串长度        System.out.println(s.charAt(0));    //String.charAt(int index) 返回第index位置的字符        System.out.println(s.substring(2)); //String.substring(int beginIndex) 提取从beginIndex位置开始直到结尾的字串        System.out.println(s.substring(2,5));   //String.substring(int beginIndex, int endIndex) 提取从beginIndex位置开始直到endIndex位置结尾的字串        System.out.println(s.replace("Hello","Hi"));    //String.replace(String target, String replacement) 用replacement替换对象中的target字串，然后返回        //其他方法可以自行尝试或查阅API文档    }}

### 补充：

![](https://img-blog.csdn.net/20181010213202599?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20181010213302234?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





