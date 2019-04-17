# Java-唠下《泛型》 - DEVELOPER - CSDN博客





2018年03月19日 12:56:01[学术袁](https://me.csdn.net/u012827205)阅读数：112
所属专栏：[JAVA-语言](https://blog.csdn.net/column/details/20397.html)









> 
JDK5版本中引入了泛型的概念。目的是提高程序的健壮性，将可能会出现在运行时的错误暴露在编译时。例如，我们经常遇到的错误：ClassCastException.


定义泛型，当顿悟之后，也很简单。但是想要有更熟练的程度熟读下源码，还是很不错的；

### 使用泛型需要注意的地方：
- 实例类共享

在程序运行时，泛型类是被所有这种类的实例所共享的。因为实际上在编译后的字节码类中，泛型会被擦除。比如 ArrayList 和 ArrayList 类型。他们俩在运行时共享同一个ArrayList 类的Class实例。

```java
ArrayList<String> str = new ArrayList<String>();
ArrayList<Integer> integer = new ArrayList<Integer>();

//答案是 true
System.out.println("str.getClass 和 integer.getClass 是否相等："+(str.getClass() == integer.getClass()));
```
- 非法的方法重载

编译器不允许在一个类中定义两个同名的方法，分别以相同类型的类作为参数，否则编译报错。 
![这里写图片描述](https://img-blog.csdn.net/20180319110446833?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)- instanceof 错误操作

不能对确切的泛型使用 instanceof 操作，否则编译报错。 
![这里写图片描述](https://img-blog.csdn.net/20180319110840320?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180319111000495?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
- 泛型的强制类型转换

不能使用泛型类型来进行强制类型转换，否则存在安全隐患。如下面便宜产生警告，运行时出现 ClassCastException 异常

```java
public static void main(String[] args) {
        Collection collection = new ArrayList<String>();
        collection.add("hello");

        ArrayList<Integer> integers = (ArrayList<Integer>) collection;
        integers.add(666);
        for (Integer integer : integers) {
            System.out.println(integer);
        }
    }
```

运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20180319111520336?watermark/2/text/Ly9ibG9nLmNzZG4ubmV0L3UwMTI4MjcyMDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Java集合的泛型

```java
List<String> list = new ArrayList<String>();
```

> 
代码中通过声明List表中只能存放的类型元素String，如果再次向表中加入其他的类型的元素。例如 list.add(8);编译时就会出现类型不兼容的错误，能够及时发现。


### 泛型类、泛型接口和泛型数组

> 
通过给类添加泛型，类似在抽象化该类型。因为通过类的泛型，可以实现到各个的具体事物。


使用 ‘extends’ 关键字来限定类型参数；使用通配符 ‘?’ 可代指任意数据类型；

```java
public class JavaTest<K extends Integer, V, T>{
    private Map map = new HashMap<K, V>();
    private T[] t;


    public static void main(String[] args) {
        JavaTest java1 = new JavaTest<Integer, String, String>();   
        java1.putMap(1, "hello");
        java1.putMap(2, "world");
        System.out.println(java1.getMap().get(1));

        Integer[] ints = new Integer[] {666, 518};
        JavaTest java2 = new JavaTest<Integer, String, Integer>();
        java2.setArray(ints);
        for (Integer integer : ints) {
            System.out.println(integer);
        }

    }


    public void putMap(K k, V v) {
        map.put(k, v);
    }

    public Map<K, V> getMap() {
        return this.map;
    }


    public void setArray(T[] t) {
        this.t = t;
    }

    public T[] getArray() {
        return this.t;
    }

}
```

### 泛型方法

> 
方法中，参数或者返回值的类型带有`<T>`形式的类型参数，那么这个方法成为泛型方法。


```python
public class Dollar{

    //泛型方法 printArray
    public static <E> void printArray(E[] args) {
        for (int i = 0; i < args.length; i++) {
            System.out.println("打印数组内容->>>"+args[i]);
        }
    }

    //泛型方法 printMax
    public static <T extends Comparable<T>> T printMax(T x, T y) {

        return x.compareTo(y) > 0 ? x : y;
    }

    public static void main(String[] args) {
        Integer[] ints = {518, 666, 668};
        printArray(ints);

        System.out.println(printMax(ints[1], ints[0]));
    }
}
```

> 
泛型允许在定义类或者方法时候，声明类型参数（如`<T>`），当程序访问类或者放法时，可以提供明确的类型参数。 

  泛型的主要两大作用：编译器在编译时，就能根据泛型参数来检查各种赋值操作是否是类型兼容。从而避免 ClassCastException 运行时异常；简化程序代码，不必使用强制类型的转换。







