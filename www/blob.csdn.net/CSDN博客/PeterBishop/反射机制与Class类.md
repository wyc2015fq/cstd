# 反射机制与Class类 - PeterBishop - CSDN博客





2018年11月11日 14:25:06[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：33
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  从这节课开始， 我们正式进入java 反射机制的学习



  首先，什么是java的反射机制？来看百度百科:

![](https://img-blog.csdnimg.cn/2018111113444026.png)



重点:
- 在运行状态中
- 对任意一个实体类
- 对任意一个对象
- 动态获取信息
- 动态调用对象方法



  Java有个包，叫java.lang.reflect，这个包下的所有类都是为反射机制服务的，除此之外还有一个非常重要的类：java.lang.Class


public final class Class<T> implements java.io.Serializable,

                              GenericDeclaration,

                              Type,

                              AnnotatedElement {


Class类:
- 反射机制的核心类，这个类包含了我们定义的类的所有属性和方法以及类名
- 这个类的构造器是私有的，所以不用想着new了
- 获取Class对象的方法有 类.class，对象.getClass()， Class.forName(String className)              等



接下来来看看代码:
package reflection;



import helloworld.MyArrayList;



import java.lang.reflect.Field;



public class ClassTeach {



    public static void main(String[] args) throws ClassNotFoundException, NoSuchFieldException, IllegalAccessException {

        */**         * Class**类可以动态获取任意实体类的属性和方法，那么，我们随便挑个类来试试         * 比如之前我们自己写过的MyArrayList         */        *//第一种获取Class对象的方法: 类.class

        Class clazz1 = MyArrayList.class;



        //第二种获取Class对象的方法: 对象.getClass()

        MyArrayList<Object> list = new MyArrayList<>();

        Class clazz2 = list.getClass();



        //第三种获取Class对象的方法: Class.forName() 参数是全类名，即完整包名+类名

        //这个方法可能会抛出ClassNotFoundException，表示要加载的类找不到，这个异常是编译时异常

        Class clazz3 = Class.*forName*("helloworld.MyArrayList");



        */**         * **根据百科里的内容，反射机制是可以动态获取一个类的所有属性和方法的         * 我们来试试         * 首先，查API文档，看看有什么方法可以获取属性和方法         *         * public Field getDeclaredField(String name)         * 返回一个Field对象，它反映此表示的类或接口的指定已声明字段类对象。 name参数是一个String ，它指定了所需字段的简单名称。         *         * OK，这里又牵扯出一个新的类 Field，大家可以暂停视频猜猜在哪个包下         * 揭晓答案， 在java.lang.reflect下,看英语我肯定看得懂，怕你们看不懂，还是来看API文档         *         * Filed 提供有关类或接口的单个字段的信息和动态访问。 反射的字段可以是类（静态）字段或实例字段。         * 所谓字段，其实就是属性         */        *//用clazz1来试试获取属性

        //MyArrayList里有一个size属性，来获取的看看

        //此方法可能报一个异常NoSuchFieldException，表示没有这样的一个属性

        Field sizeField = clazz1.getDeclaredField("size");

        System.*out*.println(sizeField.getName());    //size

        */**         * **来介绍下Field比较常用的三个方法         * getName()  获取属性名称         * set(Object obj, Object value) 给obj的Field对应的属性赋值value         * get(Object obj) 获取obj的Field对应的属性的值         */        *//前面我们new了一个list对象，现在来试试

        //set方法会报一个异常IllegalAccessException，表示非法接触异常，即如果对象是private的你是不能直接接触的

        //刚说完可能报异常立马报了，因为size属性是私有的

        //如果我们非要给size属性赋值怎么办？

        //设置accessible为true即可

        sizeField.setAccessible(true);

        sizeField.set(list,2);

        //不报错了

        //现在来看看list的size属性的值

        System.*out*.println(list.size());    //可以看到已经给list的size属性赋值了

        //再来用引用获取的看看

        //get方法依然会抛异常，而且还是两个 IllegalArgumentException, IllegalAccessException

        //一个非法参数，一个非法接触

        System.*out*.println(sizeField.get(list));

        //可以看到通过get也可以动态获取



        //最后来输出下Class对应的类的名字吧，下节课再来讲动态获取方法

        System.*out*.println(clazz1.getName());   //helloworld.MyArrayList





    }

}


