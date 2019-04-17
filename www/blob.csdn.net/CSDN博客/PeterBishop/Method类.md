# Method类 - PeterBishop - CSDN博客





2018年11月11日 14:40:30[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：27
个人分类：[Java学习](https://blog.csdn.net/qq_40061421/article/category/8087498)









  这节课我们来讲Method类



  Method类自然表示的是方法的信息，前面我们说了可以动态获取实体类的所有的属性和方法，下面我们来看看怎么获取方法:


package reflection;



import helloworld.MyArrayList;



import java.lang.reflect.InvocationTargetException;

import java.lang.reflect.Method;



public class MethodTeach {



    public static void main(String[] args) throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {

        //还是MyArrayList举例

        MyArrayList<Object> list = new MyArrayList();

        Class<? extends MyArrayList> listClass = list.getClass();



        //我们先来获取add方法

        //getMethod方法可能抛一个异常NoSuchMethodException，表示没有这个方法

        //由于java支持方法重载，所以在获取方法时一定要传参数类型

        Method method = listClass.getMethod("add",Object.class);



        //通过invoke来动态调用方法，第一个参数是要调用这个方法的对象，后面的是参数数组，或者动态参数

        method.invoke(list,"xiaoye");

        System.*out*.println(list.size());

        //可以看到list里成功添加了一个元素

        System.*out*.println(list.get(0));

        //可以看到确实是我们刚刚添加过的元素



        */**         * **下面说明一下getMethod 和 getDeclaredMethod的区别         * getMethod只能获取public 的方法         * getDeclaredMethod是只要是声明在这个类里的方法都可以获取         *         * getField和getDeclaredField的区别同上         *         * 私有方法可以通过调用setAccessible()方法来让其变的可以调用         */        *//下面来看一个方法getDeclaredMethods() 获取该类的所有声明的方法

        Method[] declaredMethods = listClass.getDeclaredMethods();



        //输出MyArrayList类声明的所有方法

        for (Method method1 : declaredMethods) {

            System.*out*.print(method1.getName() + "() ");

        }

        

        //同样也可以获取所有的属性，方法类似，我就不写了，有兴趣可以去写

    }

}




