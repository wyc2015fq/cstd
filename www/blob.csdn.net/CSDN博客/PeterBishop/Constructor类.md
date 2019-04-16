# Constructor类 - PeterBishop - CSDN博客





2018年11月11日 15:55:17[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：130








  这节课我们来讲Constructor类，这个类表示构造器，直接看代码


package reflection;



import helloworld.MyArrayList;



import java.lang.reflect.Constructor;

import java.lang.reflect.InvocationTargetException;



public class ConstructorTeach {

    public static void main(String[] args) throws NoSuchMethodException, IllegalAccessException, InvocationTargetException, InstantiationException {

        //依然用MyArrayList做例子

        //说明一下，Class是有泛型的

        Class<MyArrayList> myArrayListClass = MyArrayList.class;



        //获取构造器，和获取方法类似



        //首先来试试获取无参构造器

        Constructor<MyArrayList> noneArgumentConstructor = myArrayListClass.getConstructor();



        //再来试试获取有参构造器

        //基本数据类型也可以获取Class对象，但基本数据类型的变量没有getClass方法

        Constructor<MyArrayList> intConstructor = myArrayListClass.getConstructor(int.class);



        //有了构造器就可以构造对象

        MyArrayList list1 = noneArgumentConstructor.newInstance();//无参构造器并不用传参数



        MyArrayList list = intConstructor.newInstance(15);  //有参构造器是必须传入对应参数的



        System.*out*.println(list == null);

        System.*out*.println(list1 == null);

        //可以看到对象被成功的构造出来了



        //注意，如果想调用无参构造器，可以直接调用Class对象.newInstance()

        //注意，该方法仅在该类有无参构造器时可以调用，否则运行时会抛异常

        MyArrayList list2 = myArrayListClass.newInstance();

        System.*out*.println(list2 == null);

    }

}




