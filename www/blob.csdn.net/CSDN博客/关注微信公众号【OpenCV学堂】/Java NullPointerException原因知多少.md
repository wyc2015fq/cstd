# Java NullPointerException原因知多少 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2010年11月07日 16:25:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：28584










有一句古诗云：夜来风雨声，花落知多少，套用一下空指针异常抛出来，原因知多少。

使用Java语言开发应用程序不管是J2SE还是J2EE的，没有几个开发者没经历空指针异常

（NullPointerException）错误的，作为一个runtime Exception最为常见的几种原因是：

1.对象未初始化而直接引用对象值或者方法到的，代码示例：

**package** com.fish.codesample;

**public****class** JavaNullSample {



**private** String name;

**public** JavaNullSample() {

}

**public** String getName() {

**return**name;

}

**public****static****void** main(String[] args) {

**new** JavaNullSample().getName().length();

}

}

2.对象引用已经不存在或者被JDBC关闭

一个经典的例子是JDBC connection已经关闭，ResultSet对象仍然被使用中，这个时候**NullPointerExceptio**n就被抛出。



3.违反某些Java容器的限制，读写Null 值

这个方面首推的就java.util.HashTable,它不接受Null 作为Key或者Value，如果试图用Null作为Key去读取HashTable将会得到NullPointerException，代码示例：

Hashtable<String,String> table = **new** Hashtable<String,String>(); 

String tKey = **null**;

// the following codes always throw NullPointerException

tKey = table.get(**null**); 

table.put(tKey, **null**);

当使用TreeMap/TreeSet的时候，一定确保所有对象的属性的值不为Null，否则在调用get/set方法时候会抛出NullPointerException，代码示例：

TreeMap<Person, String> tMap = **new** TreeMap<Person, String>();

Person personer1 = **new** JavaTreeMap(). **new** Person();

Person personer2 = **new** JavaTreeMap(). **new** Person();

personer1.setName("fish");

tMap.put(personer1, "person1");

tMap.put(personer2, "person2");// throw NullPointerException





还有几种相对比较难的Java NullPointerException异常

1.Java Swing/AWT NullPointerException

1)最常见的错误时忘记初始化某个Swing组件，当调用JFrame.getContentPane().add()方法时导致NullPointerException

2)当调用JFrame.pack()的时候抛出NullPointerException,最可能的一个原因是JFrame的容器中的Swing组件可能被开发者自己重写而导致不适用pack().

3)Java JDK6与JDK4的Swing监听机制已经有很大的不同，如果在初始化的时候触发Listener导致执行listener的某些方很可能导致抛出NullPointerException

4)某些Java2D/Java3D的组件可能导致NullPointerException



2.Socket连接丢失导致IO流的Java NullPointerException



3.资源文件加载错误导致的NullPointerException，最常见的代码示例如下：

InputStreamin=this.getClass().getResourceAsStream(PropertiesName);

props.load(in);  // throw NullPointerException if xml/property files missing



4.多线程导致的NullPointerException

这种原因导致的NullPointerException一般很难被发现，特别是在大型和复杂的系统中

而且一段时间内业很难被重现，修复这样的bug成本很高，不修复这样的bug代价很大，意味着用户可能失去对该产品的信任。



后记：Java NullPointerException做为Java Runtime 异常，在开发中减少这样错误的发生就是对代码完成Junit的单元测试，对于一些重要的代码要提高code review的次数从而提高代码质量，增加产品的可靠性。





