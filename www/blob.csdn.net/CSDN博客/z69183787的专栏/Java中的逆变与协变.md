# Java中的逆变与协变 - z69183787的专栏 - CSDN博客
置顶2016年06月06日 21:21:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1135
看下面一段代码
```java
```java
Number num = new Integer(1);  
ArrayList<Number> list = new ArrayList<Integer>(); //type mismatch
List<? extends Number> list = new ArrayList<Number>();
list.add(new Integer(1)); //error
list.add(new Float(1.2f));  //error
```
```
有人会纳闷，为什么`Number`的对象可以由`Integer`实例化，而`ArrayList<Number>`的对象却不能由`ArrayList<Integer>`实例化？list中的
```
<?
 extends Number>
```
声明其元素是Number或Number的派生类，为什么不能add `Integer`和`Float`?为了解决这些问题，我们需要了解Java中的逆变和协变以及泛型中通配符用法。
# 1. 逆变与协变
在介绍逆变与协变之前，先引入`Liskov替换原则`（Liskov
 Substitution Principle, LSP）。
## Liskov替换原则
LSP由Barbara Liskov于1987年提出，其定义如下：
> 
所有引用基类（父类）的地方必须能透明地使用其子类的对象。
LSP包含以下四层含义：
- 
子类完全拥有父类的方法，且具体子类必须实现父类的抽象方法。
- 
子类中可以增加自己的方法。
- 
当子类覆盖或实现父类的方法时，方法的形参要比父类方法的更为宽松。
- 
当子类覆盖或实现父类的方法时，方法的返回值要比父类更严格。
前面的两层含义比较好理解，后面的两层含义会在下文中详细解释。根据LSP，我们在实例化对象的时候，可以用其子类进行实例化，比如：
```java
```java
Number num = new Integer(1);
```
```
## 定义
逆变与协变用来描述类型转换（type transformation）后的继承关系，其定义：如果、表示类型，表示类型转换，表示继承关系（比如，表示是由派生出来的子类）；
- 是逆变（contravariant）的，当时有成立；
- 是协变（covariant）的，当时有；
- 是不变（invariant）的，当时上述两个式子均不成立，即与相互之间没有继承关系。
## 类型转换
接下来，我们看看Java中的常见类型转换的协变性、逆变性或不变性。
泛型
令`f(A)=ArrayList<A>`，那么时逆变、协变还是不变的呢？如果是逆变，则`ArrayList<Integer>`是`ArrayList<Number>`的父类型；如果是协变，则`ArrayList<Integer>`是`ArrayList<Number>`的子类型；如果是不变，二者没有相互继承关系。开篇代码中用`ArrayList<Integer>`实例化`list`的对象错误，则说明泛型是不变的。
数组
令`f(A)=[]A`，容易证明数组是协变的：
```java
```java
Number[] numbers = new Integer[3];
```
```
方法
~~方法的形参是协变的、返回值是逆变的：~~
通过与网友iamzhoug37的讨论，更新如下。
调用方法
```
result
 = method(n)
```
；根据Liskov替换原则，传入形参n的类型应为method形参的子类型，即
```
typeof(n)≤typeof(method's
 parameter)
```
；result应为method返回值的基类型，即
```
typeof(methods's
 return)≤typeof(result)
```
：
```java
```java
static Number method(Number num) {
    return 1;
}
Object result = method(new Integer(2)); //correct
Number result = method(new Object()); //error
Integer result = method(new Integer(2)); //error
```
```
在Java 1.4中，子类覆盖（override）父类方法时，形参与返回值的类型必须与父类保持一致：
```java
```java
class Super {
    Number method(Number n) { ... }
}
class Sub extends Super {
    @Override 
    Number method(Number n) { ... }
}
```
```
从Java 1.5开始，子类覆盖父类方法时允许协变返回更为具体的类型：
```java
```java
class Super {
    Number method(Number n) { ... }
}
class Sub extends Super {
    @Override 
    Integer method(Number n) { ... }
}
```
```
# 2. 泛型中的通配符
## 实现泛型的协变与逆变
Java中泛型是不变的，可有时需要实现逆变与协变，怎么办呢？这时，通配符`?`派上了用场：
- 
```
<?
 extends>
```
实现了泛型的协变，比如：
```java
```java
List<? extends Number> list = new ArrayList<Integer>();
```
```
- 
```
<?
 super>
```
实现了泛型的逆变，比如：
```java
```java
List<? super Number> list = new ArrayList<Object>();
```
```
## extends与super
为什么（开篇代码中）
```
List<?
 extends Number> list
```
在add `Integer`和`Float`会发生编译错误？首先，我们看看add的实现：
```java
```java
public interface List<E> extends Collection<E> {
    boolean add(E e);
}
```
```
在调用add方法时，泛型`E`自动变成了
```
<?
 extends Number>
```
，其表示list所持有的类型为`在Number与Number派生子类中的某一类型`，其中包含Integer类型却又不特指为Integer类型（Integer像个备胎一样！！！），故add `Integer`时发生编译错误。为了能调用add方法，可以用`super`关键字实现：
```java
```java
List<? super Number> list = new ArrayList<Object>();
list.add(new Integer(1));
list.add(new Float(1.2f));
```
```
```
<?
 super Number>
```
表示list所持有的类型为`在Number与Number的基类中的某一类型`，其中Integer与Float必定为这`某一类型`的子类；所以add方法能被正确调用。从上面的例子可以看出，`extends`确定了泛型的上界，而`super`确定了泛型的下界。
## PECS
现在问题来了：究竟什么时候用extends什么时候用super呢？《Effective Java》给出了答案：
> 
PECS: producer-extends, consumer-super.
比如，一个简单的Stack API：
```java
```java
public class  Stack<E>{
    public Stack();
    public void push(E e):
    public E pop();
    public boolean isEmpty();
}
```
```
要实现
```
pushAll(Iterable<E>
 src)
```
方法，将src的元素逐一入栈：
```java
```java
public void pushAll(Iterable<E> src){
    for(E e : src)
        push(e)
}
```
```
假设有一个实例化`Stack<Number>`的对象stack，src有`Iterable<Integer>`与 `Iterable<Float>`；在调用pushAll方法时会发生type
 mismatch错误，因为Java中泛型是不可变的，`Iterable<Integer>`与 `Iterable<Float>`都不是`Iterable<Number>`的子类型。因此，应改为
```java
```java
// Wildcard type for parameter that serves as an E producer
public void pushAll(Iterable<? extends E> src) {
    for (E e : src)
        push(e);
}
```
```
要实现
```
popAll(Collection<E>
 dst)
```
方法，将Stack中的元素依次取出add到dst中，如果不用通配符实现：
```java
```java
// popAll method without wildcard type - deficient!
public void popAll(Collection<E> dst) {
    while (!isEmpty())
        dst.add(pop());   
}
```
```
同样地，假设有一个实例化`Stack<Number>`的对象stack，dst为`Collection<Object>`；调用popAll方法是会发生type
 mismatch错误，因为`Collection<Object>`不是`Collection<Number>`的子类型。因而，应改为：
```java
```java
// Wildcard type for parameter that serves as an E consumer
public void popAll(Collection<? super E> dst) {
    while (!isEmpty())
        dst.add(pop());
}
```
```
在上述例子中，在调用pushAll方法时生产了E 实例（produces E instances），在调用popAll方法时dst消费了E 实例（consumes E instances）。Naftalin与Wadler将PECS称为Get and Put Principle。
java.util.Collections的copy方法(JDK1.7)完美地诠释了PECS：
```java
```java
public static <T> void copy(List<? super T> dest, List<? extends T> src) {
    int srcSize = src.size();
    if (srcSize > dest.size())
        throw new IndexOutOfBoundsException("Source does not fit in dest");
    if (srcSize < COPY_THRESHOLD ||
        (src instanceof RandomAccess && dest instanceof RandomAccess)) {
        for (int i=0; i<srcSize; i++)
            dest.set(i, src.get(i));
    } else {
        ListIterator<? super T> di=dest.listIterator();
        ListIterator<? extends T> si=src.listIterator();
        for (int i=0; i<srcSize; i++) {
            di.next();
            di.set(si.next());
        }
    }
}
```
```
PECS总结：
- 
要从泛型类取数据时，用extends；
- 
要往泛型类写数据时，用super；
- 
既要取又要写，就不用通配符（即extends与super都不用）。
