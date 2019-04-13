
# transient HashMap使用目的分析 - 无界 - CSDN博客

2019年04月01日 19:27:53[21aspnet](https://me.csdn.net/21aspnet)阅读数：118


看HashSet源码有这么一句：
```python
private transient HashMap<E,Object> map;
```
![](https://img-blog.csdnimg.cn/2019040119022873.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
再看HashSet的Add方法：
![](https://img-blog.csdnimg.cn/20190401190349880.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
实际上HashSet是复用HashMap了。
而我们去看看HashMap也会发现一样使用了transient
![](https://img-blog.csdnimg.cn/20190401190642664.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
而不管是HashSet还是HashMapdou都要求是Serializable的：
```python
public class HashMap<K,V> extends AbstractMap<K,V>
    implements Map<K,V>, Cloneable, Serializable {
```
```python
public class HashSet<E>
    extends AbstractSet<E>
    implements Set<E>, Cloneable, java.io.Serializable
```
而transient实际上是不序列化的，这就好像有点“矛盾”，再回答这个问题之前先看看transient。
测试代码如下：
```python
import java.io.Serializable;
public class EmployeeTransient implements Serializable {
    public String getConfidentialInfo() {
        return confidentialInfo;
    }
    public void setConfidentialInfo(String confidentialInfo) {
        this.confidentialInfo = confidentialInfo;
    }
    public String getFirstName() {
        return firstName;
    }
    public void setFirstName(String firstName) {
        this.firstName = firstName;
    }
    public String getLastName() {
        return lastName;
    }
    public void setLastName(String lastName) {
        this.lastName = lastName;
    }
    private transient String confidentialInfo;
    private String firstName;
    private String lastName;
}
```
现在让我们先序列化再**反序列化回java对象**，并验证是否保存了“*confidentialInfo*”？
```python
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
public class TransientTest {
    public static void main(String args[]) {
        try {
            ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("empInfo.ser"));
            EmployeeTransient emp = new EmployeeTransient();
            emp.setFirstName("Lokesh");
            emp.setLastName("Gupta");
            emp.setConfidentialInfo("password");
            //Serialize the object
            oos.writeObject(emp);
            oos.close();
        } catch (Exception e) {
            System.out.println(e);
        }
        try {
            ObjectInputStream ooi = new ObjectInputStream(new FileInputStream("empInfo.ser"));
            //Read the object back
            EmployeeTransient readEmpInfo = (EmployeeTransient) ooi.readObject();
            System.out.println(readEmpInfo.getFirstName());
            System.out.println(readEmpInfo.getLastName());
            System.out.println(readEmpInfo.getConfidentialInfo());
            ooi.close();
        } catch (Exception e) {
            System.out.println(e);
        }
    }
}
```
很明显，“*confidentialInfo*”在序列化时没有保存到持久状态，这正是我们在java中使用“**transient**”关键字的原因。
![](https://img-blog.csdnimg.cn/20190401191052577.PNG)
## 什么时候应该在java中使用transient关键字？
现在我们对“**transient**”关键字非常了解。让我们通过确定您需要**使用transient关键字**的情况来扩展理解。
第一个也是非常合乎逻辑的情况是，您可能拥有从类实例中的其他字段派生/计算的字段。应该每次都以编程方式计算它们，而不是通过序列化来保持状态。一个例子可以是基于时间戳的价值; 例如人的年龄或时间戳和当前时间戳之间的持续时间。在这两种情况下，您将根据当前系统时间而不是序列化实例来计算变量的值。
第二个逻辑示例可以是任何不应以任何形式泄漏到JVM外部的安全信息（在数据库或字节流中）。
另一个例子可能是在JDK或应用程序代码中未标记为“Serializable”的字段。未实现Serializable接口并在任何可序列化类中引用的类无法序列化; 并将抛出“java.io.NotSerializableException”异常。在序列化主类之前，应将这些不可序列化的引用标记为“transient”。
最后，有时候序列化某些字段根本没有意义。期。例如，在任何类中，如果添加了记录器引用，那么序列化该记录器实例的用途是什么。绝对没用。您可以逻辑地序列化表示实例状态的信息。Loggers永远不要分享实例的状态。它们只是用于编程/调试目的的实用程序。类似的例子可以是一个Thread类的引用。线程表示任何给定时间点的进程状态，并且没有用于将线程状态存储到您的实例中; 仅仅因为它们不构成你班级实例的状态。

```python
//final field 1
public final transient String confidentialInfo = "password";
```
现在当我再次运行序列化（写/读）时，会输出出password。
原因是，只要任何最终字段/引用被评估为“**[常量表达式](https://docs.oracle.com/javase/specs/jls/se7/html/jls-15.html#jls-15.28)**”，它就会被JVM序列化，忽略transient关键字的存在。
如果要保持非可序列化字段的状态，请使用readObject（）和writeObject（）方法。writeObject（）/ readObject（）通常在内部链接到序列化/反序列化机制，因此自动调用。 阅读更多：[java中的SerialVersionUID及相关](https://howtodoinjava.com/java/serialization/serialversionuid/)

## HashMap如何使用transient关键字？
`HashMap`用于存储键值对，我们都知道。并且我们还知道内部密钥的位置`HashMap`是基于例如密钥获得的哈希码来计算的。现在当我们序列化一个`HashMap`意味着内部的所有键`HashMap`和键的各个值也将被序列化。序列化后，当我们反序列化`HashMap`实例时，所有键实例也将被反序列化。我们知道在这个序列化/反序列化过程中，可能会丢失信息（用于计算哈希码），最重要的是它本身就是一个新的实例。
在java中，**任何两个实例（即使是同一个类）都不能具有相同的哈希码**。这是一个很大的问题，因为根据新的哈希码应该放置键的位置不在正确的位置。检索键的值时，您将在此新HashMap中引用错误的索引。
> 阅读更多：
> [在java中使用hashCode和equals方法](https://howtodoinjava.com/java/basics/java-hashcode-equals-methods/)
因此，当序列化哈希映射时，这意味着哈希索引，因此表的顺序不再有效，不应保留。这是问题陈述。
`HashMap`类使用**writeObject（）**和**readObject（）**方法，如下所示：
```python
/**
     * Save the state of the <tt>HashMap</tt> instance to a stream (i.e.,
     * serialize it).
     *
     * @serialData The <i>capacity</i> of the HashMap (the length of the
     *             bucket array) is emitted (int), followed by the
     *             <i>size</i> (an int, the number of key-value
     *             mappings), followed by the key (Object) and value (Object)
     *             for each key-value mapping.  The key-value mappings are
     *             emitted in no particular order.
     */
    private void writeObject(java.io.ObjectOutputStream s)
        throws IOException {
        int buckets = capacity();
        // Write out the threshold, loadfactor, and any hidden stuff
        s.defaultWriteObject();
        s.writeInt(buckets);
        s.writeInt(size);
        internalWriteEntries(s);
    }
```
```python
// Callbacks to allow LinkedHashMap post-actions
    void afterNodeAccess(Node<K,V> p) { }
    void afterNodeInsertion(boolean evict) { }
    void afterNodeRemoval(Node<K,V> p) { }
    // Called only from writeObject, to ensure compatible ordering.
    void internalWriteEntries(java.io.ObjectOutputStream s) throws IOException {
        Node<K,V>[] tab;
        if (size > 0 && (tab = table) != null) {
            for (int i = 0; i < tab.length; ++i) {
                for (Node<K,V> e = tab[i]; e != null; e = e.next) {
                    s.writeObject(e.key);
                    s.writeObject(e.value);
                }
            }
        }
    }
```
下面是HashSet中的实现，思路都是一样的。都是把key,value都写到序列化里去，
```python
/**
     * Save the state of this <tt>HashSet</tt> instance to a stream (that is,
     * serialize it).
     *
     * @serialData The capacity of the backing <tt>HashMap</tt> instance
     *             (int), and its load factor (float) are emitted, followed by
     *             the size of the set (the number of elements it contains)
     *             (int), followed by all of its elements (each an Object) in
     *             no particular order.
     */
    private void writeObject(java.io.ObjectOutputStream s)
        throws java.io.IOException {
        // Write out any hidden serialization magic
        s.defaultWriteObject();
        // Write out HashMap capacity and load factor
        s.writeInt(map.capacity());
        s.writeFloat(map.loadFactor());
        // Write out size
        s.writeInt(map.size());
        // Write out all elements in the proper order.
        for (E e : map.keySet())
            s.writeObject(e);
    }
```
然后反序列化的时候就是重新hash一次的过程
下面是HashMap的readObject：
![](https://img-blog.csdnimg.cn/20190401192712457.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
下面是HashSet的readObject：
![](https://img-blog.csdnimg.cn/20190401192519843.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
使用上面的代码，`HashMap`仍然可以像通常那样处理非transient字段，但是它们一个接一个地在字节数组的末尾写入存储的键值对。在反序列化时，它会通过默认的反序列化过程处理非transient变量，然后逐个读取键值对。**对于每个键，哈希和索引再次计算并插入到表中的正确位置，**以便可以再次检索它而不会出现任何错误。
参考：[Java transient关键字示例](https://howtodoinjava.com/java/basics/transient-keyword-in-java-with-real-time-example/)
扩展阅读：
[为什么HashMap的哈希表标记为transient，尽管该类是可序列化的](https://stackoverflow.com/questions/9144472/why-is-the-hash-table-of-hashmap-marked-as-transient-although-the-class-is-seria)
![](https://img-blog.csdnimg.cn/2019040119342674.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)
[图解集合4：HashMap](https://www.cnblogs.com/xrq730/p/5030920.html)
![](https://img-blog.csdnimg.cn/20190401193643810.PNG?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9saW51eHN0eWxlLmJsb2cuY3Nkbi5uZXQ=,size_16,color_FFFFFF,t_70)

