# OSGI笔记(二)之Hello World应用程序 - 零度的博客专栏 - CSDN博客
2016年08月09日 10:13:34[零度anngle](https://me.csdn.net/zmx729618)阅读数：546标签：[osgi](https://so.csdn.net/so/search/s.do?q=osgi&t=blog)
个人分类：[OSGI](https://blog.csdn.net/zmx729618/article/category/6355178)
发布和使用服务
由于 OSGi 框架能够方便的隐藏实现类，所以对外提供接口是很自然的事情，OSGi 框架提供了服务的注册和查询功能。好的，那么我们实际操作一下，就在 Hello world 工程的基础上进行。（如果不会请参考前一篇：[OSGI笔记（一）HelloWorld](http://blog.csdn.net/perfect2011/article/details/7786457)）
我们需要进行下列的步骤：
- 定义一个服务接口，并且 export 出去供其它 bundle 使用；
- 定义一个缺省的服务实现，并且隐藏它的实现；
- Bundle 启动后，需要将服务注册到 Equinox 框架；
- 从框架查询这个服务，并且测试可用性。
好的，为了达到上述要求，我们实际操作如下：
- 定义一个新的包`osgi.test.helloworld.service`，用来存放接口。单独一个 package 的好处是，您可以仅仅 export 这个 package 给其它 bundle 而隐藏所有的实现类
- 在上述的包中新建接口`IHello`，提供一个简单的字符串服务，代码如下：
**清单 2. IHello**
<span style="font-family:Arial;font-size:12px;">							
package osgi.test.helloworld.service; 
public interface IHello { 
    /** 
     * 得到 hello 信息的接口 . 
     * @return the hello string. 
     */ 
    String getHello(); 
}</span>
- 再新建一个新的包`osgi.test.helloworld.impl`，用来存放实现类。
- 在上述包中新建`DefaultHelloServiceImpl`类，实现上述接口：
**清单 3. IHello 接口实现**
<span style="font-family:Arial;font-size:12px;">							
public class DefaultHelloServiceImpl implements IHello { 
    @Override 
    public String getHello() { 
        return "Hello osgi,service"; 
    } 
 }</span>
- 注册服务，OSGi 框架提供了两种注册方式，都是通过`BundleContext`类实现的：
- `registerService(String,Object,Dictionary)`注册服务对象`object`到接口名`String`下，可以携带一个属性字典`Dictionary`；
- `registerService(String[],Object,Dictionary)`注册服务对象`object`到接口名数组`String[]`下，可以携带一个属性字典`Dictionary`，即一个服务对象可以按照多个接口名字注册，因为类可以实现多个接口；
我们使用第一种注册方式，修改`Activator`类的`start`方法，加入注册代码：
**清单 4. 加入注册代码**
<span style="font-family:Arial;font-size:12px;">							
public void start(BundleContext context) throws Exception { 
	    
    System.out.println("hello world"); 
    context.registerService( 
        IHello.class.getName(), 
        new DefaultHelloServiceImpl(), 
        null); 
	    
}</span>
- 为了让我们的服务能够被其它 bundle 使用，必须在 MANIFEST.MF 中对其进行导出声明，双击 MANIFEST.MF，找到**runtime > exported packages > 点击 add**，如图，选择**service**包即可：
**图 14. 选择导出的服务包**
![图 14. 选择导出的服务包](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image014.png)
- 另外新建一个类似于 hello world 的 bundle 叫：`osgi.test.helloworld2`，用于测试`osgi.test.helloworld`bundle
 提供的服务的可用性；
- 添加 import package：在第二个 bundle 的 MANIFEST.MF 文件中，找到**dependencies > Imported packages > Add …**，选择我们刚才 export 出去的**osgi.test.helloworld.service**包：
**图 15. 选择刚才 export 出去的 osgi.test.helloworld.service  包**
![图 15. 选择刚才 export 出去的 osgi.test.helloworld.service 包](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image015.png)
- 查询服务：同样，OSGi 框架提供了两种查询服务的引用`ServiceReference`的方法：
- `getServiceReference(String)`：根据接口的名字得到服务的引用；
- `getServiceReferences(String,String)`：根据接口名和另外一个过滤器名字对应的过滤器得到服务的引用；
- 这里我们使用第一种查询的方法，在`osgi.test.helloworld2`bundle 的`Activator`的`start`方法加入查询和测试语句：
**清单 5. 加入查询和测试语句**
<span style="font-family:Arial;font-size:12px;">							
public void start(BundleContext context) throws Exception { 
    System.out.println("hello world2"); 
	    
    /** 
        * Test hello service from bundle1. 
    */ 
    IHello hello1 = 
        (IHello) context.getService( 
        context.getServiceReference(IHello.class.getName())); 
        System.out.println(hello1.getHello()); 
}
</span>
- 修改运行环境，因为我们增加了一个 bundle，所以说也需要在运行配置中加入对新的 bundle 的配置信息，如下图所示：
**图 16. 加入对新的 bundle 的配置信息**
![图 16. 加入对新的 bundle 的配置信息](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image016.png)
- 执行，得到下列结果：
**图 17. 执行结果**
![图 17. 执行结果](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image017.png)
恭喜您，成功了！
以上转载自：[http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/section5.html](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/section5.html)
我自己的一些认识：
1、 **实现代码的隐藏：OSGi框架实现了代码的隐藏，服务的具体实现被框架隔离，以下代码就是向OSGI框架注册**
**[java]**[view plain](http://blog.csdn.net/perfect2011/article/details/7786498#)[copy](http://blog.csdn.net/perfect2011/article/details/7786498#)
- context.registerService(   
-             IHello.class.getName(), //接口类
- new DefaultHelloServiceImpl(),//实现类 
- null);   
![](http://static.blog.csdn.net/images/save_snippets.png)
  2、程序的执行流程：
- **程序start level定义了bundle的加载顺序，就当前程序分析首先运行加载了osgi.test.helloworld_1.0.0，也就是运行HelloWorld的Activator的start函数。---打印出HelloWorld**
- **接着加载osgi.test.helloworld2_1.0.0，---打印出hello world2**
- **然后调用接口，OSGI框架会根据接口的名字，在目录中查找实现类---打印出Hello osgi,service**
3、几个菜单：
- **exported packages即是到处服务，发布接口**
- **import package 指定此插件使用的包**
