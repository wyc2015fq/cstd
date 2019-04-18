# Java String类型值真的不可改变吗? - z69183787的专栏 - CSDN博客
2015年03月05日 17:02:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2229
## 1.Java String 可以改变吗？
一直认为java 中String类型的值不能修改，主要是因为String是final的，而且里面没有设置值的set方法。但是可以通过反射机制改变值。
例1
```
public class Test {
    public static void main(String[] args) throws Exception {
        String s="0123456789";
        System.out.println("改变前:s=" + s);
        Field f = s.getClass().getDeclaredField("value");
        f.setAccessible(true);
        f.set(s, new char[]{'a', 'b', 'c'});
        System.out.println("改变后:s=" + s);
    }
}
```
例1 结果:
```
改变前:s=0123456789
改变后:s=abc
```
例1 原因
`    String 类里有个 private final char value[]成员变量, String 类的值都是保存在这个value属性中。可以通过反射来改变这个值，达到改变String类值的目的。同理，通过这种方式`
