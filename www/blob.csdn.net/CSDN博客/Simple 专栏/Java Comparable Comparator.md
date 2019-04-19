# Java Comparable Comparator - Simple 专栏 - CSDN博客
2012年01月11日 19:23:17[Simple_Zz](https://me.csdn.net/love284969214)阅读数：404标签：[java																[Java面试																[Java编程](https://so.csdn.net/so/search/s.do?q=Java编程&t=blog)](https://so.csdn.net/so/search/s.do?q=Java面试&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java](https://blog.csdn.net/love284969214/article/category/1235868)
所属专栏：[Java 浅谈](https://blog.csdn.net/column/details/27397.html)
### 对于自己定义的数据结构，进行排序时，需要我们给予他一定的排序规则，这就涉及到了Java中的两个常用的接口Comparable<T>和Comparator<T>的实现。
# Comparable
**`public interface Comparable<T>`**
- Comparable 接口对实现它的每个类的对象强加一个整体排序。
- 这个排序被称为类的自然排序 ，类的 compareTo 方法被称为其自然比较方法。
- 该接口有且只有一个方法 `int compareTo(T o) `所以继承此接口需要实现该方法。compareTo返回值-1、0、1。 
- Collections.sort （和 Arrays.sort ）可以自动对实现此接口的对象进行列表（和数组）排序。
- 实现该接口的对象，可以使用如在键sorted map或作为在元件sorted set ，而不需要指定一个comparator 。
### Comparator
**`public interface Comparator<T>`**
- 比较功能，对一些对象的集合施加了一个整体排序 。
- 可以将比较器传递给排序方法（如Collections.sort或Arrays.sort ），以便对排序顺序进行精确控制。
- 比较器还可以用来控制某些数据结构（如顺序sorted sets或sorted maps ），或对于不具有对象的集合提供的排序natural ordering 。 
- 通过比较c上的一组元素S的确定的顺序对被认为是与equals一致当且仅当c.compare(e1, e2)==0具有用于S每e1和e2相同布尔值e1.equals(e2)。 
- 一般是在比较器例如: `Collections.sort(List<T> list, Comparator<? super T> c)` ，`Arrays.sort(T[] a, Comparator<? super T> c)`根据指定的比较器引起的顺序对指定的列表进行排序。
- 在Comparator比较器中重写 `int compare(T o1, T o2)` 如果遇到数字的比较，直接在方法内返回两个对象的属性的差值，例如`o1.getValue()-o2.getValue()` 是升序，`o2.getValue()-o1.getValue()` 是降序。
- 如果遇到字符形式的比较利用`compareTo(T o)` 方法进行比较，该方法比较从头开始每一个字符，当前者大于后者返回1，当前者小于后者返回-1。
### 说了很多该上点实际的例子了
```
import java.util.*;
public class HashTest {
    public static void main(String[] args){
        List<Node> list = new ArrayList<Node>();
        list.add(new Node("yguo",25));
        list.add(new Node("msdfj",22));
        list.add(new Node("skf",20));
        list.add(new Node("sfe",23));
        System.out.println("===Age排序===");
        Collections.sort(list, new Comparator<Node>() {
            @Override
            public int compare(Node o1, Node o2) {
                return o1.getAge()-o2.getAge();
            }
        });
        for(Iterator<Node> it = list.iterator(); it.hasNext();){
            System.out.println(it.next());
        }
        System.out.println("===Name排序====");
        Collections.sort(list, new Comparator<Node>() {
            @Override
            public int compare(Node o1, Node o2) {
                return o1.getName().compareTo(o2.getName());
            }
        });
        for(Iterator<Node> it = list.iterator(); it.hasNext();){
            System.out.println(it.next());
        }
        System.out.println("===Age排序===");
        Collections.sort(list);
        for(Iterator<Node> it = list.iterator(); it.hasNext();){
            System.out.println(it.next());
        }
    }
}
class Node implements Comparable<Node>{
    private String name;
    private int age;
    public Node(String name, int age){
        this.name = name;
        this.age = age;
    }
    public String getName() {
        return name;
    }
    public void setName(String name) {
        this.name = name;
    }
    public int getAge() {
        return age;
    }
    public void setAge(int age) {
        this.age = age;
    }
    public int compareTo(Node other){
        if(age > other.getAge())
            return -1;
        if(age < other.getAge())
            return 1;
        return 0;
    }
    public String toString(){
        return "Name "+name+" age "+age;
    }
}
// 输出
===Age排序===
Name skf age 20
Name msdfj age 22
Name sfe age 23
Name yguo age 25
===Name排序====
Name msdfj age 22
Name sfe age 23
Name skf age 20
Name yguo age 25
===Age排序===
Name yguo age 25
Name sfe age 23
Name msdfj age 22
Name skf age 20
```
PS:利用上述比较器，我们就可以实现像excel的排序操作一样，按某一列的顺序排序。
