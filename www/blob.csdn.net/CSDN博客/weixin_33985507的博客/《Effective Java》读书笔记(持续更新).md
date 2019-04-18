# 《Effective Java》读书笔记(持续更新) - weixin_33985507的博客 - CSDN博客
2016年04月11日 16:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
# 引言
# 创建和销毁对象
何时以及如何创建对象，何时以及如何避免创建对象，如何确保创建的对象能够被适时地销毁，以及如何管理销毁之前必须进行的所有清楚工作
## 第1条 考虑用静态工厂方法代替构造器
- 静态工厂方法与构造器不同的第一大优势在于：它们有名称
- 静态工厂方法与构造器不同的第二大优势在于：不必再每次调用它们的时候都创建一个新对象
- 静态工厂方法与构造器不同的第三大优势在于，它们可以返回原返回类型的任何子类型的对象
服务提供者框架包含三个组件：
> 
**服务接口** 提供者实现
**提供者注册API**   系统用来注册实现，让客户端访问
**服务访问API** 客户端用来获取服务的实例
**服务提供者接口（可选）** 负责创建其服务实现的实例
示例代码
```java
public interface Service
{
    public void service();
}
public interface Provider
{
    Service newService();
}
public class Services
{
    private Services(){}
    private static final Map< String, Provider > = new ConcurrentHashMap< String, Provider >();
    public static final String DEFAULT_PROVIFER_NAME = "<def>";
    
    public static void registerDefaultProvider( Provider p )
    {
        registerProvider( DEFAULT_PROVIDER_NAME, p );
    }
    public static void registerProvider( String name, Provider p )
    {
        providers.put( name, p );
    }
    public static Service newInstance()
    {
        return newInstance( DEFAULT_PROVIDER_NAME );
    }
    public static Service newInstance( String name )
    {
        Provider p = providers.get( name );
        if ( p == null )
        {
            throw new IllegalArgumentException( "No provider registered with name : " + name );
        }
        return p.newServices();
    }
}
```
- 静态工厂方法的第四大优势在于，在创建参数化类型实例的时候，它们使代码变得更加简洁
通过语法糖来简化泛型代码
```java
public static< K, V > HashMap< K, V > newInstance()
{
    return new HashMap< K, V >();
}
Map< String, List< String > > map = HashMap.newInstance();
```
- 静态工厂方法的主要缺点在于，类如果不含公有的或者受保护的构造器，就不能被子类化
- 静态工厂方法的第二个缺点在于，它们与其他的静态方法实际上没有任何区别
静态工厂方法惯用名称：
> 
**valueOf** 返回的实例和其参数具有相同的值，实际上是类型转换方法
**of** valueOf的替代
**getInstance** 根据方法的参数返回相应的实例，对于Singleton，无参数，返回唯一的实例
**newInstance** 和getInstance功能类似，但确保返回的每个实例是新创建的
**getType** 和getInstance功能类似，在工厂方法处于不同的类中的时候使用，Type表示工厂方法所返回的对象类型
**newType** 和newInstance功能类似，在工厂方法处于不同的类中的时候使用
## 第2条 遇到多个构造器参数时要考虑用构建器
静态工厂和构造器的局限：**不能很好地扩展到大量的可选参数**
> 
重载构造器模式可行，但是当有许多参数的时候，客户端代码会很难编写，且难以阅读
**一种解决方法** JavaBean模式
调用一个无参构造器来创建对象，然后调用setter方法来设置每个必要的参数，以及每个相关的可选参数
潜在问题：状态不一致、阻止了把类做成不可变的可能、
**Builder模式** 安全性和可读性的折衷
> 
不直接生成想要的对象，让客户端利用所有必要的参数调用构造器，得到一个builder对象，然后客户端在builder对象上调用类似于setter方法，来设置每个相关的可选参数，最后客户端调用无参的build方法来生成不可变的对象
示例代码
```java
public class NutritionFacts
{
    private final int seringSize;
    private final int servings;
    private final int calories;
    public static class Builder
    {
        private final int servingSize;
        private final int servings;
        
        private final int calories = 0;
        
        public Builder( int servingSize, int servings )
        {
            this.servingSize = servingSize;
            this.servings = servings;
        }
        
        public Builder calories( int val )
        {
            calories = val;
            return this;
        }
        public NutritionFacts build()
        {
            return new NutritionFacts( this );
        }
    }
    
    private NutritionFacts( Builder builder )
    {
        servingSize = builder.servingSize;
        servings = builder.servings;
        calories = builder.calories;
    }
}
//call code
NutritionFacts cocaCola = new NutritionFacts.Builder( 240, 8 ).calories( 100 ).build();
```
**builder模式模拟了具名的可选参数**
Java中传统的抽象工厂实现是Class对象，用newInstance方法充当builer方法一部分。但newInstance方法总是企图调用类的无参构造器，而构造器可能不存在，同时，该方法还会传播由无参构造器抛出的异常，**`Class.newInstance`破坏了编译时的异常检查**
builer不足：为了创建对象，必须先创建其构建器，可能造成性能问题；比重叠构造器模式更加冗长，只有在很多参数的时候才使用。
> 
**如果类的构造器或者静态工厂中具有多个参数，设计时，Builder模式是个不错的选择**
## 第3条 用私有构造器或者枚举类型强化Singleton属性
使类成为Singleton使客户端测试十分困难，因为无法替换其模拟实现，除非实现一个充当类型的接口
两种Singleton方法
```java
public class Elvis
{
    public static final Elvis INSTANCE = new Elvis();
    private Elvis() { ... }
    public void leaveTheBuilding() { ... }
}
//another
public class Elvis
{
    private static final Elvis INSTANCE = new Elvis();
    private Elvis() { ... }
    public static Elvis getInstance() { return INSTANCE; }
    public void leaveTheBuilding(){ ... }
}
```
> 
享有特权的客户端可以借助**`AccessibleObject.setAccessible`**方法，通过反射机制调用私有构造器
第三种Singleton实现方法，编写一个包含单个元素的枚举类型
```java
public enum Elvis
{
    INSTANCE;
    public void leaveTheBuilding() { ... }
}
```
更加简洁，无偿地提供了序列化机制，绝对防止多次实例化，**单元素的枚举类型已经成为实现Singleton的最佳方法**
## 第4条 通过私有构造器强化不可实例化的能力
企图通过将类做成抽象类来强制该类不可被实例化，是行不通的
一种私有构造器实现
```java
public class UtilityClass
{
    private UtilityClass()
    {
        throw new AssertionError();
    }
}
```
## 第5条 避免创建不必要的对象
一般来说，最好能重用对象而不是在每次需要的时候就创建一个相同功能的新对象
```java
String s = new String( "stringtest" );//Donot do this
String s = "stringtest";
```
对于同时提供了静态工厂方法和构造器的不可变类，通常优先使用静态工厂方法，避免创建不必要的对象
```java
public class Person
{
    private final Date birthDate;
    
    //Donot do this
    public boolean isBabyBoomer()
    {
        Calendar gmtCal = Calendar.getInstance( TimeZone.getTimeZone("GMT") );
        gmtCal.set( 1946, Calendar.JANUARY, 1, 0, 0, 0 );
        Date boomStart = gmtCal.getTime();
        gmtCal.set( 1965, Calendar.JANUARY, 1, 0, 0, 0 );
        Date boomEnd = gmtCal.getTime();
        return birthDate.compareTo( boomStart )>=0
                && birthData.compareTo( boomEnd )<0;
    }
}
//better implements
public class Person
{
    private final Date birthDate;
    
    private static final Date BOOM_START;
    private static final Date BOOM_END;
    static
    {
        Calendar gmtCal = Calendar.getInstance( TimeZone.getTimeZone( "GMT" ) );
        gmtCal.set( 1946, Calendar.JANUARY, 1, 0, 0, 0 );
        BOOM_START = gmtCal.getTime();
        gmtCal.set( 1965, Calendar.JANUARY, 1, 0, 0, 0 );
        BOOM_END = gmtCal.getTime();
    }
    
    //Do this
    public boolean isBabyBoomer()
    {
        return birthDate.compareTo( BOOM_START)>=0
                && birthData.compareTo( BOOM_END)<0;
    }
}
```
**要优先使用基本类型而不是装箱基本类型，要当心无意识的自动装箱**
通过维护自己的对象池来避免创建对象并不是一种好的做法，除非池中的对象是非常重量级的，真正正确使用对象池的典型对象示例就是数据库连接池
一般而言，维护自己的对象池会增加代码的复杂性，增加内存占用，还会损害性能
> 
当应该重用现有对象的时候，不要创建新的对象
当该创建新对象的时候，不要重用现有的对象
在提倡使用保护性拷贝的时候，因重用对象而付出的代价要远远大于因创建重复对象而付出的代价
必要时如果没能实施保护性拷贝，将会导致潜在的错误和安全漏洞；不必要地创建对象则只会影响程序的风格和性能
## 第6条 消除过期的对象引用
过期引用，指永远也不会再被解除的引用。
如果一个对象引用被无意识地保留起来了，那么垃圾回收机制不仅不会处理这个对象，而且也不会处理被这个对象所引用的所有其他对象
> 
清空对象引用是一种例外，而不是一种规范行为
只要类是自己管理内存，应该警惕内存泄漏问题
内存泄漏的另外一个常见来源是**缓存**：只要在缓存之后存在对某个项的键的引用，该项就有意义，可以用`WeakHashMap`代表缓存；当缓存中的项过期之后，会自动被删除
> 
只有当所要的缓存项的生命周期是由该键的外部引用而不是由值决定时，`WeakHashMap`才有用处
内存泄漏的第三个常见来源是**监听器和其他回调**：确保回调立即被当做垃圾回收的最佳方法是只保存它们的弱引用
## 第7条 避免使用终结方法
**终结方法(finalize)通常是不可预测的，也是很危险的，一般情况下是不必要的**
终结方法的缺点在于不能保证会被及时地执行
Java语言规范不仅不保证终结方法会被及时地执行，而且根本就不保证其被执行，**不应该依赖终结方法来更新重要的持久状态**
使用终结方法有一个非常严重的性能损失
对于确实需要终止的方法，应**提供一个显示的终止方法**，并要求改类的客户端在每个实例不再有用的时候调用这个方法
**显式的终止方法通常与try-finally结构结合起来使用，以确保及时终止**
终结方法有两种合法用途
- 当对象所有者忘记调用显式终止方法时，终结方法充当“安全网”
- 与对象的本地对等体有关。本地对等体是一个本地对象，普通对象通过本地方法委托给一个本地对象，垃圾回收器无法感知本地对象的存在，当Java对等体被回收时，它不会被回收
“终结方法链”不会被自动执行，需要进行显式调用
另外一种可选方法是**终结方法守卫者**
# 对于所有对象都通用的方法
对equals、hashCode、toString、clone和finalize深入分析
# 类和接口
关于类和接口的一些指导原则
# 泛型
通过泛型，告诉编译器每个集合中接受哪些对象类型，编译器自动为插入进行转化，并在编译时告知是否插入了类型错误的对象，使得程序更加安全和清楚
## 第23条 请不要在新代码中使用原生态类型
泛型类和接口统称为泛型，每种泛型定义一组参数化的类型，每个泛型都定义一个原生态类型，即不带任何实际类型参数的泛型名称
如果不提供类型参数，使用集合类型和其他泛型是合法的，但是不应该这么做。**如果使用原生类型，就失掉了泛型在安全性和表述性方面的所有优势**
泛型有子类型化的规则，List< String>是原生态类型List的一个子类型，而不是参数化类型List< Object >的子类型，**如果使用像List这样的原生态类型，就会失掉类型安全性，但是如果使用像List< Object >这样的参数化类型，则不会**
Java提供了一种对于不确定的类型的安全的替代方法，称作**无限制的通配符类型**：如果要使用泛型，但不确定或者不关心实际的类型参数，就可以使用一个问号代替，可以持有任何集合
> 
通配符类型是安全的，原生态类型是不安全的:
可以将任何元素放进使用原生态类型的集合中，但不能将任何元素( null除外 )放到通配符类型集合中
**例外情况**：
- 在类文字中**必须使用原生态类型**
规范不允许使用参数化类型（虽然允许数组类型和基本类型），如List.class、String[].class和int.class都合法，但是List< String.class >和List< ? >.class不合法
- 与instanceof操作符有关
泛型信息在运行时被擦除，在参数化类型而非无限制通配符类型上使用instanceof操作符是非法的，用无限制通配符类型代替原生态类型，对instanceof操作符的行为不会产生影响
在这种情况下，直接用原生态类型即可，如
```java
if ( 0 instanceof Set )
{
    Set< ? > = ( Set< ? > )o;
    ...
}
```
注意：一旦确定o为Set，就需要将其转换成通配符类型Set< ? >，而不是转换成远程类型Set
> 
使用原生态类型会在运行时导致异常，因此不要在新代码中使用
## 第24条 消除非受捡警告
使用泛型编程时，会遇到许多编译器警告：非受检强制转化警告、非受检方法调用警告、非受捡普通数组创建警告以及非受检转换警告
> 
要尽可能地消除每一个非受检警告
如果无法消除警告，同时可以证明引起警告的代码是类型安全的，（只有在这种情况下才）可以用一个@SuppressWarnings( "unchecked" )注解来禁止这条警告
> 
SuppressWarnings注解可以用在任何粒度的级别中，从单独的局部变量声明到整个类都可以，应该始终在尽可能小的范围内使用SuppressWarnings注解
每当使用SuppressWarnings( "unchecked" )注解时，都要添加一条注释，说明为什么这么做是安全的
## 第25条 列表优先于数组
数组与泛型的不同点
- 数组是协变的，泛型是不可变的
> 
协变表示如果Sub为Super的子类型，那么数组类型Sub[]就是Super[]的子类型
- 数组是具体化的，泛型是通过擦除来实现的
数组会在运行时才知道并检查它们的元素类型约束，泛型只在编译时强化它们的类型信息，并在运行时丢弃（擦除） 它们的元素类型信息
数组和泛型不能很好地混合使用，如创建泛型、参数化类型或者类型参数的数组是非法的，因为泛型数组是非法的，不是类型安全的
从技术角度来说，像E、List< E >和List< String >这样的类型应称作是**不可具体化类型**，不可具体化类型是指其运行时表示法包含的信息比它编译时表示法包含的信息更少的类型
唯一可具体化参数化类型是无限制的通配符类型，如List< ? >和Map< ? >，虽不常用，但创建无限制通配类型的数组是合法的
当得到泛型数组创建错误时，最好的解决方法通常是优先使用集合类型List< E >，而不是数组类型E[]，损失一些性能或者简洁性，换回更高的类型安全性和互用性
```java
static Object reduce( List list, Function f, Object initVal )
{
    synchronized( list )
    {
        Object result = initVal;
        for ( Object o : list )
        {
            result = f.apply( result, o );
        }
        return result;
    }
}
interface Function
{
    Object apply( Object arg1, Object arg2 );
}
```
不要从同步区域中调用外来方法
```java
static Object reduce( List list, Function f, Object initVal )
{
    Object[] snapshot = list.toArray();
    Object result = initVal;
    for ( E e ： snapshot )
    {
        result = f.apply( result, e );
    }
    return result;
}
interface Function< T >
{
    T apply( T arg1, T arg2 );
}
```
一种naive尝试
```java
static <E> E reduce( List<E> list, Function<E> f, E initVal )
{
    E[] snapshot = list.toArray();
    E result = initVal;
    for ( E e : snapshot )
    {
        result = f.apply( result, e );
    }
    return result;
}
```
利用泛型修改
```java
static< E > E reduce( List< E > list, Function< E > f, E initVal )
{
    List< E > snapshot;
    synchronized( list )
    {
        snapshot = new ArrayList< E >( list );
    }
    E result = initVal;
    for ( E e : snapshot )
    {
        result = f.apply( result, e );
    }
    return result;
}
```
## 第26条 优先考虑泛型
泛型stack
```java
public class Stack< E >
{
    private E[] elements;
    private int size = 0;
    private static final int DEFAULT_INITIAL_CAPACITY = 16;
    public Stack()
    {
        //elements = new E[ DEFAULT_INITIAL_CAPACITY ];
        //不能创建不可具体化的类型的数据，两种解决方法
        elements = ( E[] )new Object[ DEFAULT_INITIAL_CAPACITY ];
        //第二种方法，将elements域的类型从E[]改为Object[]
    }
    public void push( E e )
    {
        ensureCapacity();
        elements[ size++ ] = e;
    }
    public E pop()
    {
        if ( size == 0 )
        {
            throw new EmptyStackException();
        }
        E result = elements[ --size ];
        elements[ size ] = null;
        return result;
    }
    //...
}
```
> 
使用泛型比使用需要在客户端代码中进行转换的类型更加安全，也更加容易
在设计新类型的时候，要确保不要需要这种转换就可以使用，也就是把类做成泛型的
## 第27条 优先考虑泛型方法
静态工具方法尤其适合于泛型化，Collections中所有的“算法”方法都泛型化了
利用**有限制的通配符类型**，可以是方法更加灵活
泛型方法的一个显著特征是，无需明确指定类型参数的值，不像调用泛型构造器的时候是必须指定的
> 
编译器通过检查方法参数的类型来计算类型参数的值，这个过程称作**类型推导**
在调用泛型构造器的时候，要明确传递类型参数的值可能有点麻烦，类型参数出现在变量声明的两边，代码冗余
`Map< String, List< String > > anagrams = new HashMap< String, List< String > >();`
可以编写一个**泛型静态工厂方法**
```java
public static< K, V > HashMap< K, V > newHashMap()
{
    return new HashMap< K, V >();
}
Map< String, List< String > > anagrams = newHashMap();
```
**泛型单例工厂**：创建不可变但又适合于许多不同类型的对象
```java
public interface UnaryFunction< T >
{
    T apply( T arg );
}
private static UnaryFunction< Object > IDENTITY_FUNCTION =
 new UnaryFunction< Object >() { 
    public Object apply( Object arg ) { return arg; }
 };
@SuppressWarnings( "unchecked" )
public static< T > UnaryFunction< T > identityFunction()
{
    return ( UnaryFunction< T > ) IDENTITY_FUNCTION;
}
public static void main( String[] args )
{
    String[] strings = { "jute", "hemp", "nylon" };
    UnaryFunction< String > sameString = identityFunction();
    for ( String s : strings )
    {
        System.out.println( sameString.apply( s ) );
    }
    Number[] numbers = { 1, 2.0, 3L };
    UnaryFunction< Number > sameNumber = identityFunction();
    for ( Number n : numbers )
    {
        System.out.println( sameNumber.apply(n) );
    }
}
```
通过某个包含该类型参数本身的表达式来限制类型参数是允许的，这就是**递归类型限制**
递归类型限制最普遍的用途与Comparable接口有关，定义类型的自然顺序
```java
public interface Comparable< T >
{
    int compareTo( T o );
}
//Using a recursive type bound to express mutual comparability
public static< T extends Comparable< T > > T max( List< T > list )
{
    Iterator< T > i = list.iterator();
    T result = i.next();
    while ( i.hasNext() )
    {
        T t = i.next();
        if ( t.compareTo( result ) > 0 )
        {
            result = t;
        }
    }
    return result;
}
```
## 第28条 利用有限制通配符来提升API的灵活性
参数化类型是不可变的，导致泛型间没有子类关系，Java提供了一种特殊的参数化类型，称作**有限制的通配符类型**来处理逻辑上可以存储的泛型结构
```java
public class Stack< E >
{
    public Stack();
    public void push( E e );
    public E pop();
    public boolean isEmpty();
    public void pushAll( Iterable< ? extends E > src )
    {
        for ( E e : src )
        {
            push( e );
        }
    }
    
    public void popAll( Collection< ? super E > dst )
    {
        while ( !isEmpty() )
        {
            dst.add( pop() );
        }
    }
}
```
> 
为了获得最大限度的灵活性，要在表示生产者或消费者的输入参数上使用通配符类型
**PECS表示producer-extends, consumer-super**
```java
public static< E > Set< E > union( Set< ? extends E > s1, Set< ? extends E > s2 )
Set< Integer > integers = ...;
Set< Double > doubles = ...;
//Set< Number > numbers = union( integers, doubles );   //error
Set< Number > numbers = Union.<Number>union( integers, doubles );
```
扩展max方法
```java
public static< T extends Comparable< T > > T max( List< T > list )
//modified
public static< T extends Comparable< ? super T > > T max( List< ? extends T > list )
{
    //Iterator< T > i = list.iterator(); //error
    Iterator< ? extends T > i = list.iterator();
    T result = i.next();
    while ( i.hasNext() )
    {
        T t = i.next();
        if ( t.compareTo(result) > 0 )
        {
            result = t;
        }
    }
    return result;
}
List< ScheduleFuture< ? > > scheduleFutures = ...;
```
```java
public static void swap( List< ? > list, int i, int j )
{
    swapHelper( list, i, j );
}
private static< E > void swapHelper( List< E > list, int i, int j )
{
    list.set( i, list.set( j, list.get(i) ) );
}
```
> 
在API中使用通配符类型需要技巧，但使API变得灵活得多，如果编写的是将被广泛使用的类库，一定要是当地利用通配符类型
基本原则：**producer-extends, consumer-super**
所有的comparable和comparator都是消费者
## 第29条 优先考虑类型安全的异构容器
异构：不像普通的容器，所有键都是不同类型的
```java
public class Favorites
{
    private Map< Class< ? >, Object > favorites = new HashMap< Class< ? >, Object >();
    public < T > void putFavorite( Class< T > type, T instance )
    {
        if ( type == null )
        {
            throw new NullPointerException( "Type is null" );
        }
        favorites.put( type, type.cast( instance ) );
    }
    
    public <T> T getFavorite( Class< T > type )
    {
        return type.cast( favorites.get( type ) );
    } 
}
```
> 
集合API说明泛型的用法：限制容器只能由固定数目的类型参数。
可以通过将类型参数放在键上而不是容器上来避开这一限制
对于类型安全的异构容器，可以用Class对象作为键，以这种方式使用的Class对象称作类型令牌
# 枚举和注解
# 方法
如何处理参数和返回值，如何设计方法签名，如何为方法编写文档，在可用性、健壮性和灵活性上有进一步的提升
# 通用程序设计
讨论局部变量的处理、控制结构、类库的使用、各种数据类型的用法，以及反射机制和本地方法的用法，并讨论优化和命名惯例
## 第45条 将局部变量的作用域最小化
将局部变量的作用域最小化，可以增加代码的可读性和可维护性，并降低出错的可能性
**要使局部变量的作用域最小化，最有效的方式就是在第一次使用它的地方声明**
**局部变量的声明应该包含一个初始化表达式**，例外情况是进行try-catch
> 
如果变量被一个方法初始化，而该方法可能会抛出一个受检的异常，该变量就必须在try块的内部被初始化，如果变量的值在try块的外部被使用，则必须在try块之前声明
**for循环优先于while循环**，for循环更加简短，且不易出错
**使方法小而集中**
## 第46条 for-each循环优先于传统的for循环
**for-each循环在简洁性和预防Bug方面有着传统的for循环无法比拟的优势，并且没有性能损失，应尽可能地使用**
for-each循环不仅可以遍历集合和数组，还可以遍历任何实现Iterable接口的对象
```
public interface Iterable< E >
{
    //return an iterator over the elements in this iterable
    Iterator< E > iterator();
}
```
有三种常见的情况无法使用for-each循环
- 过滤
如果需要遍历集合，删除特定元素，需要使用显式的迭代器，然后调用remove方法
- 转换
如果需要遍历列表或者数组，并取代其部分或者全部的元素值，需要列表迭代器或者数组索引，以设定元素的值
- 平行迭代
如果需要并行地遍历多个集合，则需要显示地控制迭代器或者索引变量，以便所有迭代器或者索引变量都可以得到同步前移
## 第47条 了解和使用类库
## 第48条 如果需要精确的答案，请避免使用float和double
## 第49条 基本类型优先于装箱基本类型
## 第50条 如果其他类型更适合，则尽量避免使用字符串
## 第51条 当心字符串连接的性能
## 第52条 通过接口引用对象
## 第53条 接口优先于反射机制
## 第54条 谨慎地使用本地方法
## 第55条 谨慎地进行优化
## 第56条 遵守普遍接受的命名惯例
# 异常
如何发挥异常的优点，提高程序的可读性、可靠性和可维护性，以及减少使用不当所带来的负面影响，并提供了异常使用的指导原则
## 第57条 只针对异常的情况才使用异常
# 并发
# 序列化
阐述序列化方面的技术，讨论序列化代理模式
