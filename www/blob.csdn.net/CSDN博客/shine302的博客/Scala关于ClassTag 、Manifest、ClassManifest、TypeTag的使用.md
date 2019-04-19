# Scala关于ClassTag 、Manifest、ClassManifest、TypeTag的使用 - shine302的博客 - CSDN博客
2018年08月20日 09:47:20[SmartMap_Joule](https://me.csdn.net/shine302)阅读数：370
Scala中ClassTag 、Manifest、ClassManifest、TypeTag是描述运行时类型信息的，由于Scala是运行在JVM之上的，所以首选需要从JVM说起。JVM中的泛型并不会保存泛型的，我们一般在Java开始时候涉及到的泛型都是源码级别的，当我们反编译打开编译之后的class文件会发现并不存在泛型信息。Scala为了在运行时能够获取到泛型信息，就推出了如上关键字。
首先先看一个在Java中由于运行期间无法获取泛型信息的例子：
- 
`class ArrayDemo<T> {`
- 
`public T[] arrays = new T[10]; //创建泛型数组不可以，编译不通过`
- 
- 
`public T[] makeArray(int size) {`
- 
- 
`return new T[size];//错误信息还是： 创建泛型数组`
- 
`}`
- 
`}`
在Scala中同样存在这个例子：
- 
`class ScalaArrayDemo[T] {`
- 
- 
`//Error: cannot find class tag for element type T`
- 
`def makeTArray(): Array[T] = new Array[T](10)`
- 
- 
`}`
Scala为了解决这个问题就提出了Manifest、ClassManifest、ClassTag 、TypeTag关键字。
**Manifest：**
- 
`/**`
- 
`* Created by Daxin on 2017/8/10.`
- 
`*/`
- 
`class ManifestDemo[T] {`
- 
- 
- 
`//Manifest是类型T的显示描述符`
- 
`def makeTArray[T: Manifest](): Array[T] = new Array[T](10)`
- 
- 
`//等效上面的写法`
- 
`def makeTArray2()(implicit x: Manifest[T]): Array[T] = new Array[T](10)`
- 
- 
`def makeStringArray(): Array[String] = new Array[String](10)`
- 
- 
`}`
- 
- 
- 
`object ManifestDemo extends App {`
- 
- 
`val c = new ManifestDemo[String]`
- 
- 
`c.makeTArray()`
- 
`c.makeTArray2()`
- 
- 
`}`
ClassManifest是Manifest的一个弱化版本，就是保存的类型信息不如Manifest多。不过scala在2.10里却用TypeTag替代了Manifest，用ClassTag替代了ClassManifest，原因是在路径依赖类型中，Manifest存在问题：
- 
`scala> class Foo{class Bar}`
- 
- 
`scala> def m(f: Foo)(b: f.Bar)(implicit ev: Manifest[f.Bar]) = ev`
- 
- 
`scala> val f1 = new Foo;val b1 = new f1.Bar`
- 
`scala> val f2 = new Foo;val b2 = new f2.Bar`
- 
- 
`scala> val ev1 = m(f1)(b1)`
- 
`ev1: Manifest[f1.Bar] = Foo@681e731c.type#Foo$Bar`
- 
- 
`scala> val ev2 = m(f2)(b2)`
- 
`ev2: Manifest[f2.Bar] = Foo@3e50039c.type#Foo$Bar`
- 
- 
`scala> ev1 == ev2 // they should be different, thus the result is wrong`
- 
`res28: Boolean = true`
ev1 不应该等于 ev2 的，因为其依赖路径（外部实例）是不一样的。所以在2.10版本里，使用 TypeTag 替代了 Manifest。而ClassTag是TypeTag的一个弱化版本。
- 
`//由于Manifest存在缺陷，所以后面推出了ClassTag`
- 
`//ClassTag是TypeTag的一个弱化的版本，运行时保存类型信息T。`
- 
`//更多信息参考文档，文档写的非常清楚`
- 
`class ClassTagDemo[T] {`
- 
`def mkArray[T: ClassTag](elems: T*) = Array[T](elems: _*)`
- 
- 
`def mkArray2(elems: T*)(implicit x: ClassTag[T]) = Array[T](elems: _*)`
- 
- 
`// mkArray: [T](elems: T*)(implicit evidence$1: scala.reflect.ClassTag[T])Array[T]`
- 
- 
`// scala> mkArray(42, 13)`
- 
`// res0: Array[Int] = Array(42, 13)`
- 
`//`
- 
`// scala> mkArray("Japan","Brazil","Germany")`
- 
`// res1: Array[String] = Array(Japan, Brazil, Germany)`
- 
`}`
- 
- 
`object ClassTagDemo extends App {`
- 
- 
`val c = new ClassTagDemo[Int]`
- 
`c.mkArray(42, 13)`
- 
`c.mkArray2(42, 13)`
- 
- 
`}`
通过参见SparkContext发现大量使用ClassTag保存泛型信息，没有使用TypeTag。所以一般ClassTag就足以满足我们的需要。
