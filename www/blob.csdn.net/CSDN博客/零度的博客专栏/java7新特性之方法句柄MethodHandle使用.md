# java7新特性之方法句柄MethodHandle使用 - 零度的博客专栏 - CSDN博客
2018年01月04日 11:19:56[零度anngle](https://me.csdn.net/zmx729618)阅读数：3273
     JDK6之前我们会使用java反射来实现动态方法调用，多数框架用反射的比较多，例如mybatis、spring等。在JDK7中，新增了java.lang.invoke.MethodHandle（方法句柄），称之为“现代化反射”。其实反射和java.lang.invoke.MethodHandle都是间接调用方法的途径，但java.lang.invoke.MethodHandle比反射更简洁，用反射功能会写一大堆冗余代码。
       官方api给出的解释：
       A method handle is a typed, directly executable reference to an underlying method, constructor, field, or similar low-level operation, with optional transformations of arguments or return values.其实就是可以获取方法的句柄，类似方法的指针。
      下面看一个例子，使用方法句柄（MethodHandle）调用toString（）方法：
```java
package org.zwc.methodhandletest;
import java.lang.invoke.MethodHandle;
import java.lang.invoke.MethodHandles;
import java.lang.invoke.MethodType;
/**
 * Created by zhangwenchao on 2018/1/3.
 */
public class MHTest {
    public String toString(String s) {
        return "hello," + s + "MethodHandle";
    }
    public static void main(String[] args) {
        MHTest mhTest = new MHTest();
        MethodHandle mh = getToStringMH();  //获取方法句柄
       
        try {
            // 1.调用方法：
            String result = (String) mh.invokeExact(mhTest, "ssssss");  //根据方法句柄调用方法----注意返回值必须强转
            System.out.println(result);
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
        // 2.or like this:
        try {
            MethodHandle methodHandle2 = mh.bindTo(mhTest);
            String toString2 = (String) methodHandle2.invokeWithArguments("sssss");
            System.out.println(toString2);
        } catch (Throwable throwable) {
            throwable.printStackTrace();
        }
        // 得到当前Class的不同表示方法，最后一个最好。一般我们在静态上下文用SLF4J得到logger用。
        System.out.println(MHTest.class);
        System.out.println(mhTest.getClass());
        System.out.println(MethodHandles.lookup().lookupClass()); // like getClass()
    }
    /**
     * 获取方法句柄
     * @return
     */
    public static MethodHandle getToStringMH() {
        MethodType mt = MethodType.methodType(String.class, String.class);  //获取方法类型 参数为:1.返回值类型,2方法中参数类型
        MethodHandle mh = null;
        try {
            mh = MethodHandles.lookup().findVirtual(MHTest.class, "toString", mt);  //查找方法句柄
        } catch (NoSuchMethodException | IllegalAccessException e) {
            e.printStackTrace();
        }
        return mh;
    }
}
```
执行结果：
hello,ssssssMethodHandle
hello,sssssMethodHandle
class org.zwc.methodhandletest.MHTest
class org.zwc.methodhandletest.MHTest
class org.zwc.methodhandletest.MHTest
