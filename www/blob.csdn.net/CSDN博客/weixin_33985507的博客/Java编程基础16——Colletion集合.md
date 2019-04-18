# Java编程基础16——Colletion集合 - weixin_33985507的博客 - CSDN博客
2018年09月18日 08:46:39[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
### 1_对象数组的概述和使用
- 
A:案例演示
- 需求：我有5个学生，请把这个5个学生的信息存储到数组中，并遍历数组，获取得到每一个学生信息。
```
import net.allidea.bean.Student;
public class Colletion_1_Array {    
    public static void main(String[] args) {
//        int[] arr = new int[5];                    //创建基本数据类型数组
        Student[] arr = new Student[5];            //创建引用数据类型数组
        arr[0] = new Student("张三", 23);        //创建一个学生对象，存储在数组第一个位置
        arr[1] = new Student("李四", 24);
        arr[2] = new Student("王五", 25);
        arr[3] = new Student("赵六", 26);
        arr[4] = new Student("马哥", 20);
        
        for (int i = 0; i < arr.length; i++) {
            System.out.println(arr[i]);            //toString
        }
    }
}
```
- 
B:画图演示
- 把学生数组的案例画图讲解
- 数组和集合存储引用数据类型,存的都是地址值
![](https://image-static.segmentfault.com/331/409/3314090012-5ba1be75ec028_articlex)
### 2_集合的由来及集合继承体系图
- 
A:集合的由来
- 数组长度是固定,当添加的元素超过了数组的长度时需要对数组重新定义,太麻烦,java内部给我们提供了集合类，能存储任意对象，长度是可以改变的，随着元素的增加而增加，随着元素的减少而减少 。
- 
B:数组和集合的区别
- 
区别1 :
- 数组既可以存储基本数据类型，又可以存储引用数据类型，基本数据类型存储的是值，引用数据类型存储的是地址值
- 集合只能存储引用数据类型(对象)集合中也可以存储基本数据类型，但是在存储的时候会自动装箱变成对象。
- 
区别2:
- 数组长度是固定的，不能自动增长。
- 集合的长度的是可变的，可以根据元素的增加而增长。
- 
C:数组和集合什么时候用
- 1,如果元素个数是固定的推荐用数组。
- 2,如果元素个数不是固定的推荐用集合。
- 
D:集合继承体系图
- 
Collection(单列集合的根接口)
- 
List(有序，存取顺序一致，有索引，可以重复存储)
- ArrayList(数组实现)
- LinkedList(链表实现)
- Vector(1.0数组实现)
- 
Set(无序，不一致，无索引，不可重复存储)
- HashSet(哈希算法)
- TreeSet(二叉树算法)**
### 3_Collection集合的基本功能测试
- 
A:案例演示
- 基本功能演示
boolean add(E e)
boolean remove(Object o)
void clear()
boolean contains(Object o)
boolean isEmpty()
int size()
- 
B:注意:
- collectionXxx.java使用了未经检查或不安全的操作.
注意:要了解详细信息,请使用 -Xlint:unchecked重新编译，java编译器认为该程序存在安全隐患，温馨提示:这不是编译失败,所以先不用理会,等学了泛型你就知道了。
- add方法如果是List集合一直都返回true，因为List集合中是可以存储重复元素的，如果是Set集合当存储重复元素时候，就会返回false
- ArrayList的父类的父类重写toString方法，所以在打印对象的引用的时候，输出的结果不是Object类中的toString结果
```
import java.util.ArrayList;
import java.util.Collection;
import net.allidea.bean.Student;
@SuppressWarnings({ "rawtypes", "unchecked" })
public class Colletion_2_demo {    
    public static void main(String[] args) {
//        addDemo();
        Collection c = new ArrayList();        //父类引用指向子类对象
        c.add("a");
        c.add("b");
        c.add("c");
        c.add("d");
        c.add("e");
        
//        c.remove("b");                        //删除指定元素
        c.clear();                            //清空集合
//        System.out.println(c);
//        System.out.println(c.contains("b"));//判断是否包含
        System.out.println(c.isEmpty());    //判断是否为空
        System.out.println(c.size());        //获取元素的个数
    }
    private static void addDemo() {
        Collection c = new ArrayList();        //父类引用指向子类对象
        boolean b1 = c.add("abc");
        boolean b2 = c.add(true);            //自动装箱new Boolean(true);
        boolean b3 = c.add(100);
        boolean b4 = c.add(new Student("张三",23));        //
        boolean b5 = c.add("abc");
        
        System.out.println(b1);
        System.out.println(b2);
        System.out.println(b3);
        System.out.println(b4);
        System.out.println(b5);
        System.out.println(c.toString());
    }
}
```
### 4_集合的遍历之集合转数组遍历
- 
A:集合的遍历
- 其实就是依次获取集合中的每一个元素。
- 
B:案例演示
- 把集合转成数组，可以实现集合的遍历
```
import java.util.ArrayList;
import java.util.Collection;
import net.allidea.bean.Student;
@SuppressWarnings({ "rawtypes", "unchecked" })
public class Colletion_3_demo {
    public static void main(String[] args) {
//        demo1();
        Collection c = new ArrayList();            //Object obj = new Student("张三"，23);
        c.add(new Student("张三", 23));
        c.add(new Student("李四", 24));
        c.add(new Student("王五", 25));
        c.add(new Student("赵六", 26));
        
        Object[] arr = c.toArray();                //将集合转换成数组
        for (int i = 0; i < arr.length; i++) {
//            System.out.println(arr[i]);
            Student s = (Student)arr[i];            //向下转型
            System.out.println(s.getName() + "..." + s.getAge());
        }
    }
    private static void demo1() {
        Collection c = new ArrayList();
        c.add("a");
        c.add("b");
        c.add("c");
        c.add("d");
        
        Object[] arr = c.toArray();            //将集合转换成数组
        for (int i = 0; i < arr.length; i++) {
            System.out.println(arr[i]);
        }
    }
}
```
### 5_Collection集合的带All功能测试
- A:案例演示
带All的功能演示
boolean addAll(Collection c)
boolean removeAll(Collection c)
boolean containsAll(Collection c)
boolean retainAll(Collection c)
```
import java.util.ArrayList;
import java.util.Collection;
@SuppressWarnings({ "unchecked", "rawtypes" })
public class Collection_4_demo {
    public static void main(String[] args) {
//        addAllDemo();
//        removeAllDemo();
//        containsAllDemo();
        Collection c1 = new ArrayList();
        c1.add("a");
        c1.add("b");
        c1.add("c");
        c1.add("d");
        
        Collection c2 = new ArrayList();
        c2.add("a");
        c2.add("b");
//        c2.add("z");
        
        boolean b = c1.removeAll(c2);//取交集,如果调用的集合改变就返回true，如果调用的集合不变就返回false
        System.out.println(b);
        System.out.println(c1);
    }
    private static void containsAllDemo() {
        Collection c1 = new ArrayList();
        c1.add("a");
        c1.add("b");
        c1.add("c");
        c1.add("d");
        
        Collection c2 = new ArrayList();
//        c2.add("a");
//        c2.add("b");
        c2.add("z");
        
        boolean b = c1.containsAll(c2);            //判断调用的集合是否包含传入的集合
        System.out.println(b);
    }
    private static void removeAllDemo() {
        Collection c1 = new ArrayList();
        c1.add("a");
        c1.add("b");
        c1.add("c");
        c1.add("d");
        
        Collection c2 = new ArrayList();
        c2.add("a");
        c2.add("b");
        c2.add("z");
        boolean b = c1.removeAll(c2);        //删除的是交集内容
        System.out.println(b);
        System.out.println(c1);
    }
    private static void addAllDemo() {
        Collection c1 = new ArrayList();
        c1.add("a");
        c1.add("b");
        c1.add("c");
        c1.add("d");
        
        Collection c2 = new ArrayList();
        c2.add("aa");
        c2.add("bb");
        c2.add("cc");
        c2.add("dd");
        
        c1.addAll(c2);                    //将c2整体元素添加到c1中
        c1.add(c2);                        //将c2看成一个对象添加到c1中
        System.out.println(c1);
    }
}
```
### 6_集合的遍历之迭代器遍历
- 
A:迭代器概述
- 集合是用来存储元素,存储的元素需要查看,那么就需要迭代(遍历)
- 
B:案例演示
- 迭代器的使用
- Collection存储自定义对象并用迭代器遍历
```
import java.util.ArrayList;
import java.util.Collection;
import java.util.Iterator;
import net.allidea.bean.Student;
@SuppressWarnings({ "unchecked", "rawtypes" })
public class Collection_5_demo {    
    public static void main(String[] args) {
//        demo1();
        Collection c = new ArrayList();
        c.add(new Student("张三",23));            //Object obj = new Student("张三",23);
        c.add(new Student("李四",24));
        c.add(new Student("王五",25));
        c.add(new Student("赵六",26));
        c.add(new Student("田七",27));
/*        for(Iterator it = c.iterator();it.hasNext();) {
            Student s = (Student)it.next();                        //向下转型
            System.out.println(s.getName() + "," + s.getAge());    //获取对象中的姓名和年龄
        }*/
//        获取迭代器
        Iterator it = c.iterator();
        while(it.hasNext()) {
//            System.out.println(it.next());
            //System.out.println(((Student)(it.next())).getName() + "," + ((Student)(it.next())).getAge());
            Student s = (Student)it.next();        //向下转型
            System.out.println(s.getName() + "..." + s.getAge());
        }
    }
    private static void demo1() {
        Collection c = new ArrayList();
        c.add("a");
        c.add("b");
        c.add("c");
        c.add("d");
        
        //对集合中的元素迭代（遍历）
        Iterator it = c.iterator();            //获取迭代器
        /*boolean b1 = it.hasNext();            //判断集合中是否有元素，有就返回true
        Object obj1 = it.next();
        System.out.println(b1);
        System.out.println(obj1);
        
        boolean b2 = it.hasNext();            //判断集合中是否有元素，有就返回true
        Object obj2 = it.next();
        System.out.println(b2);
        System.out.println(obj2);*/
        
        while(it.hasNext()) {
            System.out.println(it.next());
        }
    }
}
```
### 7_迭代器的原理及源码解析(了解)
- 
A:迭代器原理
- 迭代器原理:迭代器是对集合进行遍历,而每一个集合内部的存储结构都是不同的,所以每一个集合存和取都是不一样,那么就需要在每一个类中定义hasNext()和next()方法,这样做是可以的,但是会让整个集合体系过于臃肿,迭代器是将这样的方法向上抽取出接口,然后在每个类的内部,定义自己迭代方式,这样做的好处有二,第一规定了整个集合体系的遍历方式都是hasNext()和next()方法,第二,代码有底层内部实现,使用者不用管怎么实现的,会用即可
- 
B:迭代器源码解析
- 1,在eclipse中ctrl + shift + t找到ArrayList类
- 2,ctrl+o查找iterator()方法
- 3,查看返回值类型是new Itr(),说明Itr这个类实现Iterator接口
- 4,查找Itr这个内部类,发现重写了Iterator中的所有抽象方法
### 8_List集合的特有功能概述和测试
- 
A:List集合的特有功能概述
- void add(int index,E element)
- E remove(int index)
- E get(int index)
- E set(int index,E element)
```
import java.util.ArrayList;
import java.util.List;
public class List_1_demo {
/*    * A:List集合的特有功能概述
    * void add(int index,E element)
    * E remove(int index)
    * E get(int index)
    * E set(int index,E element)*/
    
    public static void main(String[] args) {
//        demo1();
//        demo2();
//        demo3();
//        demo4();
        demo5();
        
    }
    private static void demo5() {
        List list = new ArrayList();
        list.add("a");
        list.add("b");
        list.add("c");
        list.add("d");
        
        list.set(1, "z");
        System.out.println(list);        //将制定位置的元素修改
    }
    private static void demo4() {
        List list = new ArrayList();
        list.add("a");
        list.add("b");
        list.add("c");
        list.add("d");
        
//        Object obj1 = list.get(1);
//        System.out.println(obj1);
        //通过索引遍历list集合
        for (int i = 0; i < list.size(); i++) {
            System.out.println(list.get(i));
        }
    }
    private static void demo3() {
        List list = new ArrayList();
        list.add(111);
        list.add(222);
        list.add(333);
        
        list.remove(0);                        //删除的时候不会自动装箱，把111当索引
        System.out.println(list);
    }
    private static void demo2() {
        List list = new ArrayList();
        list.add("a");
        list.add("b");
        list.add("c");
        list.add("d");
        
        Object obj = list.remove(1);        //通过索引删除元素，将被删除的元素返回
        System.out.println(obj);
        System.out.println(list);
    }
    private static void demo1() {
        List list = new ArrayList();        //开发时直接创建子类对象
        list.add("a");
        list.add("b");
        list.add("c");
        list.add("d");
        list.add(1, "f");        //index <= size且>=零都可以
//        list.add(1, "e");
//        list.add(10,"z");        //使用不存在的索引时，索引越界异常java.lang.IndexOutOfBoundsException
        System.out.println(list);
    }
}
```
### 9_List集合存储学生对象并遍历
- 
A:案例演示
- 通过size()和get()方法结合使用遍历。
```
import java.util.List;
import java.util.ArrayList;
import net.allidea.bean.Student;
@SuppressWarnings({ "unchecked", "rawtypes" })
public class List_2_demo {    
    public static void main(String[] args) {
        List list = new ArrayList();
        list.add(new Student("张三",23));            //Object obj = new Student("张三",23);
        list.add(new Student("李四",24));
        list.add(new Student("王五",25));
        list.add(new Student("赵六",26));
        list.add(new Student("田七",27));
        
        for (int i = 0; i < list.size(); i++) {        //通过索引获取每一个元素
//            System.out.println(list.get(i));
            Student s = (Student)list.get(i);
            System.out.println(s.getName() + "..." + s.getAge());
        }
    }
}
```
### 10_并发修改异常产生的原因及解决方案
- 
A:案例演示
- 需求：我有一个集合，请问，我想判断里面有没有"world"这个元素，如果有，我就添加一个"javaee"元素，请写代码实现。
```
List list = new ArrayList<>();
        list.add("a");        //Object obj = new String();
        list.add("b");
        list.add("c");
        list.add("world");
        list.add("d");
        list.add("e");
        
        Iterator it = list.iterator();    //获取迭代器
        while(it.hasNext()) {
            String str = (String)it.next();//向下转型
            if("world".equals(str)) {
                list.add("javaee");        //遍历的同时在增加元素，并发修改ConcurrentModificationException
            }
            System.out.println(list);
        }
```
- 
B:ConcurrentModificationException出现
- 迭代器遍历，集合修改集合
- 
C:解决方案
- a:迭代器迭代元素，迭代器修改元素(ListIterator的特有功能add)
- b:集合遍历元素，集合修改元素
```
ListIterator lit = list.listIterator();    //获取迭代器，list集合特有的
        while(lit.hasNext()) {
            String str = (String)lit.next();//向下转型
            if("world".equals(str)) {
//                list.add("javaee");        //遍历的同时在增加元素，并发修改ConcurrentModificationException
                lit.add("javaee");
            }
            System.out.println(list);
        }
```
### 11_ListIterator(了解)
- boolean hasNext()是否有下一个
- boolean hasPrevious()是否有前一个
- Object next()返回下一个元素
- Object previous();返回上一个元素
```
import java.util.ArrayList;
import java.util.List;
import java.util.ListIterator;
public class List_4_ListIterator {
    public static void main(String[] args) {
        List list = new ArrayList();
        list.add("a");        //Object obj = new String();
        list.add("b");
        list.add("c");
        list.add("world");
        list.add("d");
        list.add("e");
        
        ListIterator lit = list.listIterator();
        /*while(lit.hasNext()) {
            System.out.println(lit.next());
        }*/
        System.out.println("------------");
        while(lit.hasPrevious()) {
            System.out.println(lit.previous());
        }
    }
}
```
### 12_Vector的特有功能(了解)
- A:Vector类概述
- 
B:Vector类特有功能
- public void addElement(E obj)
- public E elementAt(int index)
- public Enumeration elements()
- 
C:案例演示
- Vector的迭代
```
import java.util.Enumeration;
import java.util.Vector;
public class List_5_Vector {
    public static void main(String[] args) {
        Vector v = new Vector();            //创建集合对象,List的子类
        v.addElement("a");
        v.addElement("b");
        v.addElement("c");
        v.addElement("d");
        //Vector迭代
        Enumeration en = v.elements();        //获取枚举
        while(en.hasMoreElements()) {        //判断集合中是否有元素
            System.out.println(en.nextElement());//获取集合中的元素    
        }
    }
}
```
### 13_数据结构之数组和链表
- 
A:数组
- 查询快修改也快(有索引)
- 增删慢（排队模式）
- 
B:链表
- 查询慢,修改也慢（传花法）
- 增删快（无顺序）
### 14_List的三个子类的特点
- 
A:List的三个子类的特点
- 
ArrayList:
- 底层数据结构是数组，查询快，增删慢。
- 线程不安全，效率高。
- 
Vector:
- 底层数据结构是数组，查询快，增删慢。
- 线程安全，效率低。
- Vector相对ArrayList查询慢(线程安全的)
- Vector相对LinkedList增删慢(数组结构)
- 
LinkedList:
- 底层数据结构是链表，查询慢，增删快。
- 线程不安全，效率高。
- 
Vector和ArrayList的区别
- Vector是线程安全的,效率低
- ArrayList是线程不安全的,效率高
- 共同点:都是数组实现的
- 
ArrayList和LinkedList的区别
- ArrayList底层是数组结果,查询和修改快
- LinkedList底层是链表结构的,增和删比较快,查询和修改比较慢
- 共同点:都是线程不安全的
- 
B:List有三个儿子，我们到底使用谁呢?
- 查询多用ArrayList
- 增删多用LinkedList
- 如果都多ArrayList
