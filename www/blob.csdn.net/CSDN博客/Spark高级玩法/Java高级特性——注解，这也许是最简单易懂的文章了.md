# Java高级特性——注解，这也许是最简单易懂的文章了 - Spark高级玩法 - CSDN博客
2018年08月27日 09:39:01[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：101
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDd5tXcheeGtYr8aRQpSZTS7fncO0z5lyibEZI7v8YjVHkTXa79OYCZdqQ/640?wx_fmt=png)
最近，浪尖在做flink的项目时source和sink的绑定那块用到了注解，当然新版本1.6以后就变为server load的方式加载。
但是浪尖也是觉得很有毕业讲一下注解，毕竟高级免试也会问答的吧。抗倒这篇文章不错，转发一下。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/cZkI8M0nbClYCYIkqfrY3jKxllBevUDdxOq06gyribibH9twuLMzRTeJWxqepiawF1VTZcLYub1wnWQic3lrqRrSiaw/640?wx_fmt=png)
博主在初学注解的时候看到网上的介绍大部分都是直接介绍用法或者功能，没有实际的应用场景，篇幅又很长导致学习的时候难以理解其意图，而且学完就忘QAQ。本篇文章中我将结合实际的应用场景尽可能由浅入深，平缓的介绍java注解。
##### **java注解是jdk1.5以后新出的特性，对于它的应用非常广泛，我们首先来看一下注解的应用，百度百科上这样说：**
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPWnvvxQmzNVPNQbM51Lw6J8BKbNv88JdXiaHh0TAq4JcRo0WcnBiabMpLuearZxCdlHiaZlDsAnDqibA/640)
我们可以看到，注解的作用有三方面：
- 
编写doc文档：这个就我们很常用的 @return 以及 @author，加了这些注解以后，就可以用jdk帮我们自动生成对应的API文档了
- 
编译检查：这个也很常见 @Override，而且功能很强大，我将会在以后的文章中介绍
- 
进行代码分析：这是本篇文章的重点。这个和编译检查一样也是一个强大的功能，但相比与编译检查由于其用到了反射，在性能上存在一些问题
##### **后台开发中的SSH三大框架，以及咱们安卓端的retrofit，ButterKnife，Lombok等框架和插件也是大量的用到了注解。这里我将通过手撸一个假的ButterKnife来具体演示注解有什么用，怎么用。**
我们首先来看段代码：
```
public class MainActivity extends AppCompatActivity {
@OnClick(R.id.test_btn)
void test(){
    test_tv.setText("恭喜您，绑定成功了！");
}
@FindViewByID(R.id.test_tv)
TextView test_tv;
@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    ButterKnife.bindView(this);
}
}
```
这是一个最基本的activity，里面有2个控件，Button和TextView。
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/QCu849YTaIPWnvvxQmzNVPNQbM51Lw6JFPMaJk17DAqjMRyzZR3NjkMu6tPW4VXO4T2vHhoicGubPFrDLc12DbQ/640)
在我点击Button后TextView的文字被改变。而我所做的仅仅是ButterKnife.bindView(this)并添加2个注解而已，这样就实现了控件的绑定，省去了很多与业务无关的代码，是不是简洁了很多。
看了注解的功能是不是很想了解它是怎么做到的，接下来我就来看看它是什么，怎么用，怎么利用。
##### **什么是注解**
官方把它叫做元数据，即一种描述数据的数据。所以，可以说注解就是源代码的元数据。用它来可以来描述、标记我们的源代码。
##### **怎样定义一个注解**
以下是我上文中定义的一个 @OnClick注解
```
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
public @interface OnClick {
int value() default 0;
}
```
**可见和定义一个类一样，只是将class改为 @interface，并且顶部通过几个原注解来说明这个注解的一些重要信息，具体如下：**
J2SE5.0版本在 java.lang.annotation提供了四种元注解，专门注解其他的注解：
- 
@Documented –注解是否将包含在JavaDoc中
- 
@Retention –什么时候使用该注解
- 
@Target? –注解用于什么地方
- 
@Inherited – 是否允许子类继承该注解
@Documented–一个简单的Annotations标记注解，表示是否将注解信息添加在java文档中，一般不用管。
@Retention– 定义该注解的生命周期，很重要，必须指定，以下是3种生命周期的介绍
> - 
RetentionPolicy.SOURCE – 在编译阶段丢弃。这些注解在编译结束之后就不再有任何意义，所以它们不会写入字节码。@Override, @SuppressWarnings都属于这类注解。
- 
RetentionPolicy.CLASS – 在类加载的时候丢弃。在字节码文件的处理中有用。注解默认使用这种方式。
- 
RetentionPolicy.RUNTIME– 始终不会丢弃，运行期也保留该注解，因此可以使用反射机制读取该注解的信息。我们自定义的注解通常使用这种方式。
@Target – 表示该注解用于什么地方。如果不明确指出，该注解可以放在任何地方。以下是一些可用的参数。需要说明的是：属性的注解是兼容的，如果你想给7个属性都添加注解，仅仅排除一个属性，那么你需要在定义target包含所有的属性。
> 
ElementType.TYPE:用于描述类、接口或enum声明
ElementType.FIELD:用于描述实例变量
ElementType.METHOD
ElementType.PARAMETER
ElementType.CONSTRUCTOR
ElementType.LOCAL_VARIABLE
ElementType.ANNOTATION_TYPE 另一个注释
ElementType.PACKAGE 用于记录java文件的package信息
@Inherited – 定义该注释和子类的关系
##### **那么注解体里的内容有该怎样定义**
Annotations只支持基本类型、String及枚举类型。注释中所有的属性被定义成方法，并允许提供默认值。
```
@Target(ElementType.METHOD)
@Retention(RetentionPolicy.RUNTIME)
@interface Book{
public enum Priority {LOW, MEDIUM, HIGH}
String author() default "Yash";
int price() default 20;
Status status() default Status.NOT_STARTED;
}
```
看看怎么用它
```
@Todo(priority = Todo.Priority.MEDIUM, author = "zsq", status = 
Todo.Status.STARTED)
public void incompleteMethod1() {
}
```
通过字段名 = 的形式给字段赋值，如果没赋值，则使用缺省值。如果注解中只有一个属性，可以直接命名为“value”，使用时无需再标明属性名，例如我定义的 @OnClick注解。
##### **好了，花了那么多精力来认识他，该看看该怎么来利用它了**
我们定义了自己的注解并将其应用在业务逻辑的方法上。现在我们需要写一个用户程序调用我们的注解。这里我们需要使用反射机制。如果你熟悉反射代码，就会知道反射可以提供类名、方法和实例变量对象。所有这些对象都有getAnnotation()这个方法用来返回注解信息。我们需要把这个对象转换为我们自定义的注释(使用 instanceOf()检查之后)，同时也可以调用自定义注释里面的方法。
**所有这些对象都有getAnnotation()！**
##### **所有这些对象都有getAnnotation()！**
##### **所有这些对象都有getAnnotation()！**
重要的API说3遍，另外用到的几个方法也很重要，下面的代码会演示，更多的API使用参考可以去查阅JDK文档。
具体到我们本编文章的实例，调用注解的家伙就是我们刚刚在MainActivity里用到的 ButterKnife，我们通过设置监听的注解来看看它到底做了什么。
```
public static final void bindView(final Activity activity){
    traversalMethod(activity);
    traversalField(activity);
}
```
在我们调用的ButterKnife.bindView(this)中我们拿到了MainActivity的实例，并且通过反射遍历里面所有的方法：
```
private static void traversalMethod(final Activity activity) {
    Method[] methodArray = getObjectMethodArray(activity);
    for (final Method method:methodArray){
        if(isAnnotationPresent(method)){
            int viewID = getViewID(method);
            setOnClickListenerForControl(activity, method, viewID);
        }
    }
}
private static Method[] getObjectMethodArray(Activity activity) {
    return activity.getClass().getMethods();
}
```
接着判断方法是否被我们注解：
```
private static boolean isAnnotationPresent(Method method) {
    return method.isAnnotationPresent(OnClick.class);
}
```
如果是我们用注解标注的方法则通过注解获取注解里保存的空间ID，并且通过MainActivity的实例为其设置点击监听，在监听内调用被注解标注的方法。
```
private static int getViewID(Method method) {
    return method.getAnnotation(OnClick.class).value();
}
private static void setOnClickListenerForControl(final Activity activity, final Method method, int viewID) {
    activity.findViewById(viewID).setOnClickListener(new View.OnClickListener() {
        @Override
        public void onClick(View view) {
            try {
                method.invoke(activity);
            } catch (IllegalAccessException e) {
                e.printStackTrace();
            } catch (InvocationTargetException e) {
                e.printStackTrace();
            }
        }
    });
}
```
**大功告成！是不是很简单~**
我们用反射获取注解的方式实现了ButterKnife的功能，但文章开头说过反射的存在性能上的不足。而实际上ButterKnife本身用的也不是反射，而是用的apt工具在编译时期就可以获取到所有的方法、字段、以及他们的注解，从而避免了使用反射，解决了性能的问题。接下来的文章我会讲解本文开头提到的第三点，也就是ButterKnife实际使用的方法，将我们自己的ButterKnife改为ButterKnife官方的实现方法。
以下github地址是本篇文章讲解用到的demo：
https://github.com/sally519/MyButterKnief
希望看完的人能收获点东西，作者水平有限，如有遗漏和错误，欢迎指正！
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFWF1rkKibTzeA8PicbicYXBsH26a9PXg2HNnlEt1thHBFxUtEjicACeaSlRWictpPziaMdibXmYq34dWfQ9w/640?wx_fmt=png)
