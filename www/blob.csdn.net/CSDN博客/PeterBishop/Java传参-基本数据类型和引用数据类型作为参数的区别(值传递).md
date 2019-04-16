# Java传参-基本数据类型和引用数据类型作为参数的区别(值传递) - PeterBishop - CSDN博客





2018年10月06日 22:36:26[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：267








java中的方法可以传递参数，参数的传递方法就是值传递。

参数有形参和实参，定义方法时写的参数叫形参，真正调用方法时，传递的参数叫实参。

调用方法时，会把实参传递给形参，方法内部其实是在使用形参。

所谓值传递就是**当参数是基本类型时，传递参数的值，比如传递i=10，真实传参时，把10赋值给了形参。**

**当参数是对象时，传递的是对象的值，也就是对象的首地址。就是把对象的地址赋值给形参。**

基本类型当做方法的形参时
![](https://img-blog.csdn.net/20170927200043555?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHlwaGY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20170927200424090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHlwaGY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20170927204505747?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHlwaGY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**当引用类型当做参数的形参时**
![](https://img-blog.csdn.net/20170928004729032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveHlwaGY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**基本数据类型和引用数据类型作为参数的区别**

基本数据类型的变量中直接存放数据值本身，所以改的时候改的是数据值本身；

但是引用类型不同的地方在于真正的数据并没有在**栈区**的变量中保存，而是在**堆区**里面保存着，所以虽然也拷贝了一份，也是副本，但是二者指向的是同一块堆区。

引用数据类型就好比如说，两位同学使用的是同一份复习资料，其中一人把资料撕毁了，另一人当然也会受到影响。

而基本数据类型就好比复印了一份，其中一人将自己的资料撕了，并不影响别人。

**总结：**

1）.当使用**基本数据类型**作为方法的形参时，在方法体中对形参的修改**不会**影响到实参的数值

2）.当使用**引用数据类型**作为方法的形参时，若在方法体中修改形参指向的数据内容，则**会**
** 对实参变量的数值产生影响，因为形参变量和实参变量共享同一块堆区；**

3）.当使用**引用数据类型**作为方法的**形参**时，**若在方法体中修改形参变量的指向，此时不会**
** 对实参变量的数值产生影响，因此形参变量和实参变量分别指向不同的堆区；**

### （引用数据类型本质是形参接收了实参的地址，如果不分配新的地址那么是会影响实参的（公用堆区），否则互不相干）



