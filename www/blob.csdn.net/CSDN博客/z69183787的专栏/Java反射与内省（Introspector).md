# Java反射与内省（Introspector) - z69183787的专栏 - CSDN博客
2015年09月21日 21:26:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：640
个人分类：[反射-机制](https://blog.csdn.net/z69183787/article/category/2175689)

一、java反射机制
    JAVA反射机制是在运行状态中，对于任意一个类，都能够得到这个类的所有属性和方法；对于任意一个对象，都能够调用它的任意一个方法；这种动态获取的信息以及动态调用对象的方法的功能称为java语言的反射机制。
    用一句比较白的话来概括，反射就是让你可以通过名称来得到对象 ( 类，属性，方法 ) 的技术。例如我们可以通过类名来生成一个类的实例；知道了方法名，就可以调用这个方法；知道了属性名就可以访问这个属性的值。
Java反射机制主要提供了以下功能：
1、为一个类生成对应的Class对象
   运用（已知对象） getClass（）：Object类中的方法，每个类都拥有此方法。
        如：String str=new String（）；Class strClass=str.getClass();
     运用（已知子类的class） Class.getSuperclass（）：Class类中的方法，返回该Class的父类的Class;
     运用（已知类全名）Class.forName（）静态方法
     运用（已知类）类名.class
2、通过类名来构造一个类的实例
   a、调用无参的构造函数：
   Class newoneClass = Class.forName(类全名)；
   newoneClass.newInstance()；
   b、调用有参的构造函数：我们可以自定义一个函数。
   public Object newInstance(String className, Object[] args) throws Exception {
        //args为参数数组
        Class newoneClass = Class.forName(className);
        //得到参数的Class数组(每个参数的class组成的数组)，由此来决定调用那个构造函数
        Class[] argsClass = new Class[args.length];
        for (int i = 0, j = args.length; i < j; i++) {
            argsClass[i] = args[i].getClass();
        }
        Constructor cons = newoneClass.getConstructor(argsClass); //根据argsClass选择函数
        return cons.newInstance(args); //根据具体参数实例化对象。
   }
3、得到某个对象的属性
   a、非静态属性：首先得到class，然后得到这个class具有的field，然后以具体实例为参数
      调用这个field
   public Object getProperty(Object owner, String fieldName) throws Exception {
       Class ownerClass = owner.getClass();//首先得到class
       Field field = ownerClass.getField(fieldName);
       //然后得到这个class具有的field,也可以通过getFields（）得到所有的field
       Object property = field.get(owner);
       //owner指出了取得那个实例的这个属性值，
         如果这个属性是非公有的，这里会报IllegalAccessException。
       return property;
   }
   b、静态属性：
      只有最后一步不同，由于静态属性属于这个类，所以只要在这个类上调用这个field即可
      Object property = field.get(ownerClass);
4、执行某对象的方法
  public Object invokeMethod(Object owner, String methodName,
                             Object[] args) throws Exception {
     Class ownerClass = owner.getClass(); //也是从class开始的
     //得到参数的class数组，相当于得到参数列表的类型数组，来取决我们选择哪个函数。
     Class[] argsClass = new Class[args.length];
     for (int i = 0, j = args.length; i < j; i++) {
         argsClass[i] = args[i].getClass();
     }
     //根据函数名和函数类型来选择函数
     Method method = ownerClass.getMethod(methodName, argsClass);
     return method.invoke(owner, args);//具体实例下，具体参数值下调用此函数
  }
5、执行类的静态方法
   和上面的相似只是最后一行不需要指定具体实例
   return method.invoke(null, args);
6、判断是否为某个类的实例
   public boolean isInstance(Object obj, Class cls) {
        return cls.isInstance(obj);
   }
二、java内省机制
    内省是 Java 语言对 Bean 类属性、事件的一种处理方法（也就是说给定一个javabean对象，我们就可以得到/调用它的所有的get/set方法）。例如类 A 中有属性 name, 那我们可以通过 getName,setName 来得到其值或者设置新的值。通过 getName/setName 来访问 name 属性，这就是默认的规则。 Java 中提供了一套 API 用来访问某个属性的 getter/setter 方法，通过这些 API 可以使你不需要了解这个规则，这些 API 存放于包 java.beans
 中。
    一般的做法是通过类 Introspector 来获取某个对象的 BeanInfo 信息，然后通过 BeanInfo 来获取属性的描述器（ PropertyDescriptor ），通过这个属性描述器就可以获取某个属性对应的 getter/setter 方法，然后我们就可以通过反射机制来调用这些方法。下面我们来看一个例子，这个例子把某个对象的所有属性名称和值都打印出来：
//定义一个javabean
public class PersonBean {
   public String name;
   public String[] childname;
   public String[] getChildname() {
      return childname;
   }
   public void setChildname(String[] childname) {
      this.childname = childname;
   }
   public String getName() {
      return name;
   }
   public void setName(String name) {
      this.name = name;
   }
}
//定义一个测试类，来进行一下set和get方法的调用举例
import java.beans.BeanInfo;
import java.beans.IntrospectionException;
import java.beans.Introspector;
import java.beans.PropertyDescriptor;
import java.lang.reflect.InvocationTargetException;
public class TestIntrospector {
   public static void main(String[] args) throws IllegalArgumentException,
                           IllegalAccessException, SecurityException, NoSuchMethodException,
                           InvocationTargetException, IntrospectionException {
  //演示一下get方法的调用
      //初始化一个javabean对象
      PersonBean pb=new PersonBean();
      pb.setName("kangjian");
      String[] childs=new String[]{"kk","jj","nn"};
      pb.setChildname(childs);
      //将该javabean中的属性放入到BeanInfo中。第二个参数为截止参数，表示截止到此类之前，
       不包括此类。如果不设置的话，那么将会得到本类以及其所有父类的info。
      BeanInfo bi=Introspector.getBeanInfo(pb.getClass(), Object.class);
      //将每个属性的信息封装到一个PropertyDescriptor形成一个数组
        其中包括属性名字，读写方法，属性的类型等等
      PropertyDescriptor[] pd=bi.getPropertyDescriptors();
      //演示如何get
      for (int i = 0; i < pd.length; i++) {
         if(pd[i].getPropertyType().isArray())  //getPropertyType得到属性类型。
         {
            //getReadMethod()得到此属性的get方法----Method对象，然后用invoke调用这个方法
            String[] result=(String[]) pd[i].getReadMethod().invoke(pb, null);
            System.out.println(pd[i].getName()+":");//getName得到属性名字
            for (int j = 0; j < result.length; j++) {
               System.out.println(result[j]);
            }
         }
         else
         {
            System.out.println(pd[i].getName()+":"+pd[i].getReadMethod().invoke(pb, null));
         }
      }
  //演示一下set方法的调用
      //初始化一个尚未set的javabean
      PersonBean pb0=new PersonBean();
      //模拟一个数据（数据名字和javabean的属性名一致）
      String name="luonan";
      String[] childname=new String[]{"luo","nan"};
      BeanInfo bi0=Introspector.getBeanInfo(pb0.getClass(), Object.class);
      PropertyDescriptor[] pd0=bi0.getPropertyDescriptors();
      for (int i = 0; i < pd0.length; i++) {
         if(pd0[i].getPropertyType().isArray())
         {
            if(pd0[i].getName().equals("childname"));
            {
               if(pd0[i].getPropertyType().getComponentType().equals(String.class))
               {//getComponentType()可以得到数组类型的元素类型
                  //getWriteMethod()得到此属性的set方法---Method对象，然后用invoke调用这个方法
                  pd0[i].getWriteMethod().invoke(pb0,new Object[]{childname});
               }
            }
         }
         else
         {
            if(pd0[i].getName().equals("name"));
            {
               pd0[i].getWriteMethod().invoke(pb0,name);
            }
         }
      }
      System.out.println(pb0.getName());
      String[] array=pb0.getChildname();
      for (int i = 0; i < array.length; i++) {
         System.out.println(array[i]);
      }
   }
}
更难的应用请参考[http://hi.baidu.com/shunxinyangkun/blog/item/dd2c4ff46ae61aeb7609d7ba.html](http://hi.baidu.com/shunxinyangkun/blog/item/dd2c4ff46ae61aeb7609d7ba.html)
三、关于内省的思考
    struts2的action（还有struts1的formbean）就是这么实现的。
    前台的form标签具有一些属性（在配置文件中知道这个form提交到那个action，而这个action有和这个form相对应的属性及其 get/set），提交以后，由struts的servlet拦下来转发给某个具体的action.而在转发给action之前struts通过内省的方式将form中的值set到了action中去。
    其实只要有个set**或者get**，内省就会理解为存在这样的**属性，这样可以方便我们把 Bean 类通过一个接口来定义而不用去关心具体实现，不用去关心 Bean 中数据的存储。比如我们可以把所有的 getter/setter 方法放到接口里定义，但是真正数据的存取则是在具体类中去实现，这样可提高系统的扩展性。
四、总结
    将 Java 的反射以及内省应用到程序设计中去可以大大的提供程序的智能化和可扩展性。有很多项目都是采取这两种技术来实现其核心功能，例如我们前面提到的 Struts ，还有用于处理 XML 文件的 Digester 项目，其实应该说几乎所有的项目都或多或少的采用这两种技术。在实际应用过程中二者要相互结合方能发挥真正的智能化以及高度可扩展性。
