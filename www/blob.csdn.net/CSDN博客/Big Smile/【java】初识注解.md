# 【java】初识注解 - Big Smile - CSDN博客
2018年07月22日 21:38:33[王啸tr1912](https://me.csdn.net/tr1912)阅读数：77标签：[java																[注解](https://so.csdn.net/so/search/s.do?q=注解&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java学习](https://blog.csdn.net/tr1912/article/category/6453602)
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        注解这个东西，只要是一个java的开放人员都不会陌生，但是我们用了那么多的注解，我们的注解是怎样为我们工作的呢？通过前段时间的学习，我对注解有了一点新的理解。
# 一、什么是注解
        注解在我们通俗的看来，就是一个对于Annotation接口的一个实现，他和类还有接口是处于同一个声明和使用级别的，都继承了Object基类，并且都有.class属性。
        但是注解本身真的起作用吗？
        不信你自己定义一个注解试试，然后放在你的代码上，结果就是毛用都没有。
        我们可以将注解看做是一个带有属性的标记，我们给我们的代码打上这个标记，就意味着我们的代码具有了注解所代表的某些特性了，但是不代表我们给他打上注解的一瞬间就有这种特性了。
        我们的代码在运行前是需要进行编译的，有的时候在运行时我们也是需要进行动态编译的，这个时候，如果我们嵌入了反射，或者动态代理的代码去解析这个类，并给这个类加上他应有的特性，这个时候类才具有了注解所代表的意义。
        举个例子，比如我们在上幼儿园的时候，上厕所是需要举手，然后老师带领着去的，这个时候，举手就代表你打上了上厕所的注解，如果老师不鸟你的话，你是上不了厕所的，只能憋着，如果老师这个时候扫视全班，发现你举手了，她才会去你那里带着你和一帮都在举手的同学一起去上厕所。这个时候你才发挥了上厕所的本领。
# 二、实现
        我们看到的注解，大部分其实都不是一开始java给你定义好的，一开始就规定了的注解只有最开始的四个元注解，他们分别是：
-    @Documented –注解是否将包含在JavaDoc中
-    @Retention –什么时候使用该注解
-    @Target –注解用于什么地方
-    @Inherited – 是否允许子类继承该注解
@Documented ，这个注解的意思就是，在我们生成javaDoc的时候是否把这个类的说明，或者方法的说明放在我们的java文档中。一般如果不用java自带的项目文档工具生成文档的话是没有什么用处的。
@Retention，这个注解表示了我们定义注解的生命周期，下面就是它的各种赋值和作用说明：
- RetentionPolicy.SOURCE : 在编译阶段丢弃。这些注解在编译结束之后就不再有任何意义，所以它们不会写入字节码。@Override, @SuppressWarnings都属于这类注解。
-  RetentionPolicy.CLASS : 在类加载的时候丢弃。在字节码文件的处理中有用。注解默认使用这种方式
-  RetentionPolicy.RUNTIME : 始终不会丢弃，运行期也保留该注解，因此可以使用反射机制读取该注解的信息。我们自定义的注解通常使用这种方式
@Target ，表示了注解用于什么地方的标记，默认为标记任何元素，可以给里面赋值ElementType的值：
-  ElementType.CONSTRUCTOR:用于描述构造器
-  ElementType.FIELD:成员变量、对象、属性（包括enum实例）
-  ElementType.LOCAL_VARIABLE:用于描述局部变量
-  ElementType.METHOD:用于描述方法
-  ElementType.PACKAGE:用于描述包
-  ElementType.PARAMETER:用于描述参数
-  ElementType.TYPE:用于描述类、接口(包括注解类型) 或enum声明
@Inherited ，定义该注释和子类的关系，@Inherited 元注解是一个标记注解，@Inherited阐述了某个被标注的类型是被继承的。如果一个使用了@Inherited修饰的annotation类型被用于一个class，则这个annotation将被用于该class的子类。
我们可以自定义一个注解：
```java
import java.lang.annotation.Documented;
import java.lang.annotation.Retention;
import java.lang.annotation.Target;
import static java.lang.annotation.ElementType.FIELD;
import static java.lang.annotation.RetentionPolicy.RUNTIME;
/**
 * 水果名称注解
 */
@Target(FIELD)
@Retention(RUNTIME)
@Documented
public @interface FruitName {
    String value() default "";
}
```
上述的元注解就是为了自定义注解服务的。
# 三、总结
        总之，注解就是一个用来标记代码的标记，我们可以通过扫描不同的注解来实现不同的注解方法，通过java的动态代理和反射我们可以轻松的获得我们注解标记的内容，从而操作我们写好的类或者方法，下篇文章，我将定义一个自定义的注解，并写出他的实现。
