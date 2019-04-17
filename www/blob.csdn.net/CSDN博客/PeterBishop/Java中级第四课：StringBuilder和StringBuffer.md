# Java中级第四课：StringBuilder和StringBuffer - PeterBishop - CSDN博客





2019年01月27日 11:26:54[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：25
个人分类：[Java中级](https://blog.csdn.net/qq_40061421/article/category/8626230)









  这节课我们来讲StringBuilder和StringBuffer



  在java初级里我们连接字符串是直接使用 String对象+String对象 的方法，但是直接使用+有一个弊端就是造成了很大的浪费，因此在需要多次连接字符串时，我们选用StringBuilder或StringBuffer。



  先来看看API文档。

  1.6API版本还没有AbstractStringBuilder这个类，我们来看1.8的API手册

  1.8的也没有，我们直接上源码好了。


public final class StringBuilder

    extends AbstractStringBuilder

    implements java.io.Serializable, CharSequence
可以看到StringBuilder继承了AbstractStringBuilder



我们主要看具体方法：

![](https://img-blog.csdnimg.cn/2019012711253597.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2019012711255468.png)





以上是所有append的重载方法，基本上只要是可以转成字符串的类型的对象都可以直接使用append



append()方法是我们最常用的方法

![](https://img-blog.csdnimg.cn/20190127112606181.png)

![](https://img-blog.csdnimg.cn/20190127112606184.png)

![](https://img-blog.csdnimg.cn/20190127112606180.png)











以下是insert()的所有重载方法

![](https://img-blog.csdnimg.cn/20190127112623312.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190127112623313.png)

![](https://img-blog.csdnimg.cn/20190127112623322.png)

![](https://img-blog.csdnimg.cn/20190127112623324.png)







基本上这些方法够用了。



再来说说StringBuffer
public final class StringBuffer

   extends AbstractStringBuilder

   implements java.io.Serializable, CharSequence


StringBuffer也是AbstractStringBuilder的子类，因此StringBuffer的api和StringBuilder类似，也就是说StringBuffer和StringBuilder类似。



接下来，本课重点:

**为什么****StringBuilder****连接字符串性能比****String****好**？



这个还是要分析源码:
abstract class AbstractStringBuilder implements Appendable, CharSequence {

    */**     * The value is used for character storage.     */    *char[] value;
答案很简单，因为StringBuilder的所有方法是对数组变量的操作，而String的所有方法包括运算符运算均是产生一个新的字符串对象，因此显然对String对象的操作要耗费的时间和空间都会大于StringBuilder，因此当需要多次连接字符串或者操作字符串时，建议用StringBuilder来做，当然如果次数少就直接用String了，这样可以避免多构造一个StringBuilder对象。



下面说下StringBuffer的用处，StringBuffer和StringBuilder的作用类似，也是构造字符串的，但是StringBuilder的方法是线程不安全的，而StringBuffer的方法时线程安全的，下面仅以append()方法为例



以下是StringBuilder的append(String str)方法
@Override

public StringBuilder append(String str) {

    super.append(str);

    return this;

}


以下是StringBuffer的append(String str)方法
@Override

public synchronized StringBuffer append(String str) {

    toStringCache = null;

    super.append(str);

    return this;

}
可以看到StringBuffer的append(String str)方法加了一个synchronized，这就是为了保证线程安全而加的同步锁。



注意：StringBuilder的效率 > StringBuffer的效率 > 运算符+

      StringBuffer的安全性 > StringBuilder的安全性 = 运算符+



Demo:
package com.teach;



import org.junit.Test;



public class StringBuilderAndStringBuffer {



    @Test

    public void method()

    {

        //最后来演示下怎么用

        //现在有一个需求，要求把一个String数组对象转成一个字符串

        //转法如下: 假设String数组有3个元素，分别是"1","2","3"

        //则转成字符串后字符串的内容为{"1","2","3"}

        String[] str = new String[3];

        str[0] = "1";

        str[1] = "2";

        str[2] = "3";

        String s = *change*(str);

        System.*out*.println(s);



    }



    //转法如下: 假设String数组有3个元素，分别是"1","2","3"

    //则转成字符串后字符串的内容为{"1","2","3"}

    public static String change(String[] strArr)

    {

        StringBuilder builder = new StringBuilder();

        builder.append("{");

        for (String s : strArr)

        {

            builder.append("\"");

            builder.append(s);

            builder.append("\",");

        }

        builder.deleteCharAt(builder.length()-1);

        builder.append("}");

        return builder.toString();

    }

}




