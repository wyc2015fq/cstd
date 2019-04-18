# OSGi的helloworld - z69183787的专栏 - CSDN博客
2014年11月25日 16:02:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：638
个人分类：[Osgi](https://blog.csdn.net/z69183787/article/category/2738261)
  该helloworld例子程序主要的功能是在启动和停止一个Bundle的时候来做些事情（输出信息）。
1、创建bundle工程
（1）在eclipse中创建一个Plug-in工程
![](http://dl.iteye.com/upload/attachment/586855/117e49d5-15ae-3e18-b500-a71ef81dbccf.jpg)
（2）输入工程相关信息
![](http://dl.iteye.com/upload/attachment/586859/17bdb3e7-9277-35d3-98b4-90be59001db8.jpg)
（3）输入Bundle的相关元数据信息：
       Plug_in ID指的是Bundle的唯一标识，在实际项目中可以采用类似java的包名组织策略来保证标识的唯一性
       Plug-in Version  指的是Bundle的版本
       Plug-in Name 指的是BundleBundle的更具有意义的名称
       Plug-in Provider 指的是Bundle的提供商
       Execution Environment 指的是Bundle运行需要的环境
       剩下的关键就是Activitor部分了，填入自己的一个类名就可以了，在工程建立时Eclipse会自动建立这个类
![](http://dl.iteye.com/upload/attachment/586875/86bf8997-e444-3a0c-95ea-5bb93129b026.jpg)
（4）完成了Bundle工程的创建后，在Package视图中就可以看到如下图所示，表明工程创建成功：
![](http://dl.iteye.com/upload/attachment/586896/03fc7467-4b92-345b-8113-62f6423625a2.jpg)
2、完成Activator的代码
（1）打开默认的Activator.java
![](http://dl.iteye.com/upload/attachment/586899/be0ce363-4ad3-32ef-8c87-fd932daa9058.jpg)
 这个类实现了BundleActivator接口,其中有两个空的方法--start和stop。start方法是在Bundle被启动的时候调用的，stop是在Bundle被停止时调用的，加入代码：
![](http://dl.iteye.com/upload/attachment/586923/dc078c73-c6cc-3736-83c0-2a4f642718a2.jpg)
 HelloWorld的例子完成！
3、运行
（1）首先创建一个HelloWorld用的运行配置
（2）然后设置HelloWorldde 的Auto-Start属性为false
![](http://dl.iteye.com/upload/attachment/586939/42cc65e7-2b23-37de-9dbd-fbb3bcfbe20b.jpg)
（3）点击Run，在Console中出现"osgi>"提示。表明成功启动一个OSGi应用。
        在osgi>提示符后面输入 ss，按回车键，如下图：
![](http://dl.iteye.com/upload/attachment/586942/0bec495e-95a8-3774-be66-aa8fb82a7921.jpg)
 可以看到，HelloWorld Bundle已经被安装并且完成了解析，但是 还没有启动。下面在osgi>提示符下输入 start 1,然后回车，然后再输入ss:
![](http://dl.iteye.com/upload/attachment/586948/00622b4e-adbe-3248-a126-7eefdaf6e7b6.jpg)
 看到HelloWorld启动了，通过ss可以看到HelloWorld Bundle的状态改变为ACTIVE，说明启动成功。并且在Activitor的start方法中加入的输出信息也正确的打印出来了。
  接着 输入 stop 1 ，并且再用ss命令查看bundle的状态:
![](http://dl.iteye.com/upload/attachment/586950/3a198747-994a-35ad-87ec-81660afc381f.jpg)
 这个时候Bundle停止。
至此，一个OSGi Bundle顺利完成，第一个OSGi程序顺利运行！
            
