# ServiceLoader的使用 - z69183787的专栏 - CSDN博客
2016年05月04日 06:34:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1694
  发现ServiceLoader是个类似spring的东西，可以指定接口和实现，通过ServiceLoader去载入。 
下面是我的一个例子：
Java代码  ![收藏代码](http://huangyunbin.iteye.com/images/icon_star.png)
- package com.test2;  
- 
- import java.util.ServiceLoader;  
- import com.test.IHello;  
- 
- publicclass Main {  
- publicstaticvoid main(String[] args) {  
-         ServiceLoader<IHello> loder=ServiceLoader.load(IHello.class);  
- for (IHello hello : loder) {  
-             System.out.println(hello.getClass());  
-             hello.sayHello();  
-         }  
-     }  
- 
- }  
依赖的IHello是在另外一个jar包：test.jar.在test.jar里有一个接口，两个实现，Hello1和Hello2. 通过在的META-INF下的services文件夹的com.test.IHello文件来指定实现，多个实现的时候使用换行。 
如我的项目中就是
Java代码  ![收藏代码](http://huangyunbin.iteye.com/images/icon_star.png)
- com.test.Hello1  
- com.test.Hello2  
注意，这个只能是打成jar之后给其他项目用，自己是不可以用自己的。
![](http://dl2.iteye.com/upload/attachment/0085/4921/298e6af4-f70c-3350-865f-c2b1f33b5832.png)
