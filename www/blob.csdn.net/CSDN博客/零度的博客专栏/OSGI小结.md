# OSGI小结 - 零度的博客专栏 - CSDN博客
2016年08月08日 17:32:51[零度anngle](https://me.csdn.net/zmx729618)阅读数：2714
现在越来越多的Java开发人员在谈论OSGi是有其道理的。在几年前上学的时候我进行了比较多的Eclipse插件开发，当时就亲身感觉到Eclipse插件体系的灵活与强大，而该体系与OSGi也可谓一脉相承、同气连枝。最近打算重新系统的研究一下OSGi，一边学习一边写一个OSGi探秘系列作为总结，和大家分享，也请大家多给评价和建议。
### 1、OSGi概述
       我想先回答一个基本的问题：什么是OSGi？其实如果要很好地回答这个问题，也许先明白是什么导致了OSGi的产生和发展会有些帮助。相信每个Java程序员都知道Classpath是什么。Java的类加载器(Classloader)是一种分层结构，如下图所示，分为引导类加载器(Bootstrap Class Loader)，扩展类加载器(Extension Class Loader)，系统类加载器(System
 Class Loader)以及用户定义的类加载器(User-defined Class Loader)。引导类加载器在JVM时负责加载rt.jar里面的类，扩展类加载器负责加载在扩展目录下的jar文件中的类，系统类加载器则在Classpath上面搜索类加载器，用户定义的类加载器则从用户指定的路径（比如一个网络URI）加载类。在该类加载体系中，一个类加载器总是先去上层类加载器加载类，一层一层迭代，当无法找到需要的类时在自己加载。
![](https://img-blog.csdn.net/20130820183222046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb2t1aTAwOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
       在这种类加载机制中，存在以下几个问题：1）类版本冲突：当类路径上存在同一个类的不同版本时，如果类加载器找到一个版本，则不再搜索加载下一个版本；2）无法确定jar之间的依赖关系：现有的JAR标准中缺乏对与Jar文件之间依赖关系的定义支持，因此只有在运行时间无法找到所需的类时，才会打出java.lang.ClassNotFoundException，但这通常不能有效帮助开发人员解决问题；3）信息隐藏：如果一个jar在类路径上并且被加载，那么所有该jar中的公共类（public
 class）都会被加载，无法避免某些类被隐藏从而不被加载。尽管在J2EE中改进了类加载机制，可以支持以war或者ear应用为单元进行加载，但是这些问题还是没有被很好地解决，并且热部署效果让人忧心。
       OSGi就是为了克服这些问题而生，却又不局限与这些问题，对Java开发人员而言的确是好多顿大餐。OSGi是一个动态的Java模块（Module）系统，它规定了如何定义一个Module以及这些模块之间如何交互。每个OSGi的Java模块被称为一个bundle。每个bundle都有自己的类路径，可以精确规定哪些Java包和类可以被导出，需要导入哪些其它bundle的哪些类和包，并从而指明bundle之间的依赖关系。另外bundle可以被在运行时间安装，更新，卸载并且不影响整个应用。通过这种方式，分层的类加载机制变成了网状的类加载机制。在应用程序启动之前，OSGi就可以检测出来是否所有的依赖关系被满足，并在不满足时精确报出是哪些依赖关系没被满足。
### 2、OSGi开发环境的建立
#### 2.1 Equinox是什么
从代码角度来看，Equinox其实就是OSGi核心标准的完整实现，并且还在这个基础上增加了一些额外的功能（比如为框架增加了命令行和程序执行的入口）。我们在之前入门篇讲解的都东西其实都是OSGi核心标准的一小部分。其实它的核心就是一个jar包，这个jar包既能执行（作为标准Java包的特性），也是一个bundle（Manifest里面含有OSGibundle特有的元数据）。
现在你需要知道的就是，我们能够利用Equinox项目的代码来运行一个实实在在的OSGi框架，框架启动后，你就可以将你开发好bundle放到里面运行。
#### 2.2下载Equinox
Equinox在[http://download.eclipse.org/equinox](http://download.eclipse.org/equinox)上有官方下载，里面列出了各个版本供我们选择：
在这里，我们使用3.7版本的Equinox，下载好以后放在一个单独的文件夹下（这里我的路径是E:\OSGiframework\equinox）：
#### 2.3从命令行启动框架
如果启动这个框架的话，有了上面的jar包就足够了，我们进入命令行输入如下命令：java –jar org.eclipse.osgi_3.7.0.v20110613.jar -console ，然后就会进入Equinox的控制台：
如果出现osgi>的提示符，就说明启动成功了。
Equinox的控制台的部分基本命令如下（区分大小写
#### 2.4 在Eclipse中建立开发环境
### 在上一节中大家看到启动和控制框架的方法，是相当简单的一个过程。不过单单只是运行环境还不够，我们还需要开发环境。
2.4.1 设置
### 首先我们不需要安装必须的插件，只要你有较新版本的Eclipse就行了。然后进入Eclipse的window->preferences界面，选中Plug-inDevelopment下的Target Platform：
现在右边只有一个Runing Platform的，我们任务是点击“Add…”按钮来增加一个我们自己的的platform，
选择默认的第一个就好，点击next。
再点击这里的“Add…”：
选择“Directory”：
选择你的Equinox的jar包所在的路径,然后点击finish，回到刚才的界面：
这时候你就会发现里面多出来了你刚刚设置的路径，路径后面描述的“1 plug-ins available”则就是说的我们放置的Equinox的jar包。
继续点击finish，回到最开始的界面：
这时候多出来了一个新的targetplatform，勾选上，然后确定。
2.4.2
启动
### 打开菜单项Run->Run configurations…，在OSGi Framework项中，新建一个Run configuration：
这里面现在已经自动包含了Equinox的jar包了，点击Run，看看运行的效果：
Eclipse的控制台中也出现来了osgi的提示符，说明你已经成功启动了。
你可以试试刚才讲的那些命令，看看能输出些什么（比如上图中我输入了ss）。
2.4.3新建一个project
### 打开新建project的界面，选择Plug-in Project：
然后输入project的名字，TargetPlatform处选择an OSGiframework->Equinox或者standard都行，点击下一步：
这里实际上是对bundle的Manifest文件的设置，其中的ID就是Bundle-SymbolicName，Version就是bundle的版本号，下面还能决定是否定义BundleActivator，点击finish就创建了一个project：
至此，开发环境已经建立完毕。
### 3、osgi的helloworld 
现在可谓是万事具备，只欠Helloworld了。为了将OSGi框架的三个层次都涵盖到，这个Helloworld可能会比其他你见到的OSGi
 Helloworld程序要复杂一点点。如果对代码中的一些API感到生疏，记得回到之前的入门篇中找到对应的内容，这样对你理解代码会有帮助。里面的关键代码已经用黄色高亮显示。（出于篇幅考虑，代码中的import语句都省略）
#### 3.1 HelloWorld的定义与实现
首先我们创建一个工程org.serc.helloworld，在这个工程里面，我们创建一个包含sayHello方法的接口，准备作为服务接口：
**[java]**[view plain](http://blog.csdn.net/xiaokui008/article/details/9662937#)[copy](http://blog.csdn.net/xiaokui008/article/details/9662937#)
- package org.serc.helloworld;  
- 
- publicinterface Hello {  
- void sayHello();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
然后，对这个接口进行实现：
**[java]**[view plain](http://blog.csdn.net/xiaokui008/article/details/9662937#)[copy](http://blog.csdn.net/xiaokui008/article/details/9662937#)
- package org.serc.helloworld.impl;  
- 
- publicclass HelloImpl implements Hello{  
- final String helloString;  
- 
- public HelloImpl(String helloString){  
- this.helloString = helloString;  
- }  
- 
- publicvoid sayHello(){  
-     System.out.println(this.helloString);  
- }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这个类实现的sayHello所做的工作就是输出一个在对象构造的时候得到的helloString 字符串。 为了将这个接口暴露出来，我们需要在MANIFEST文件中加入如下条目：
**[java]**[view plain](http://blog.csdn.net/xiaokui008/article/details/9662937#)[copy](http://blog.csdn.net/xiaokui008/article/details/9662937#)
- Export-Package:  org.serc.helloworld;version="1.0"
![](http://static.blog.csdn.net/images/save_snippets_01.png)
接下来，为了把这个服务注册到框架中，我们定义了一个Activator：
**[java]**[view plain](http://blog.csdn.net/xiaokui008/article/details/9662937#)[copy](http://blog.csdn.net/xiaokui008/article/details/9662937#)
- package org.serc.helloworld.activator;  
- 
- publicclass Activator implements BundleActivator {  
- private List<ServiceRegistration> registrations = new ArrayList<ServiceRegistration>();  
- 
- publicvoid start(BundleContext ctx) {  
-                             registrations.add(ctx.registerService(Hello.class.getName(),new HelloImpl("Hello, OSGi"), null));  
- }  
- 
- publicvoid stop(BundleContext ctx) {  
- for (ServiceRegistration registration : registrations) {  
-         System.out.println("unregistering: "+ registration);  
-         registration.unregister();  
-     }  
![](http://static.blog.csdn.net/images/save_snippets.png)
我们为这个HelloImpl传入了"Hello, OSGi"的字符串 为了让这个Activator能够工作，需要在MANIFEST文件中做如下定义：
**[java]**[view plain](http://blog.csdn.net/xiaokui008/article/details/9662937#)[copy](http://blog.csdn.net/xiaokui008/article/details/9662937#)
- Bundle-Activator: org.serc.helloworld.activator.Activator  
![](http://static.blog.csdn.net/images/save_snippets.png)
这个bundle最终的MANIFEST内容如下：
**[java]**[view plain](http://blog.csdn.net/xiaokui008/article/details/9662937#)[copy](http://blog.csdn.net/xiaokui008/article/details/9662937#)
- Bundle-ManifestVersion: 2
- Bundle-SymbolicName: org.serc.helloworld  
- Bundle-Version: 1.0
- Bundle-Activator: org.serc.helloworld.activator.Activator  
- Import-Package: org.osgi.framework  
- Export-Package:  org.serc.helloworld;version="1.0"
![](http://static.blog.csdn.net/images/save_snippets.png)
你的Eclipse工程中现在应该是这样：
#### 3.2 获得并执行SayHello服务
创建一个工程org.serc.helloworld.client，创建一个叫HelloUser的BundleActivator，其中的start方法会获得接口为Hello的服务对象，并且通过这个对象来调用sayHello方法：
**[java]**[view plain](http://blog.csdn.net/xiaokui008/article/details/9662937#)[copy](http://blog.csdn.net/xiaokui008/article/details/9662937#)
- package org.serc.helloworld.client;  
- 
- publicclass HelloUser implements BundleActivator {  
- 
- publicvoid start(BundleContext ctx) {  
-     ServiceReference ref = ctx.getServiceReference(Hello.class.getName());  
- if (ref != null) {  
-         Hello hello = null;  
- try {  
-             hello = (Hello) ctx.getService(ref);  
- if (hello != null)  
-                 hello.sayHello();  
- else
-                 System.out.println("Service:Hello---object null");  
-         } catch (RuntimeException e) {  
-             e.printStackTrace();  
-         } finally {  
-             ctx.ungetService(ref);  
-             hello = null;  
-         }  
-     } else {  
-         System.out.println("Service:Hello---not exists");  
-     }  
- }  
- 
- publicvoid stop(BundleContext ctx) throws Exception {  
- 
- }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
为了获得Hello这个接口的定义，我们还需要在MANIFEST文件中import Hello所在的package：
**[html]**[view plain](http://blog.csdn.net/xiaokui008/article/details/9662937#)[copy](http://blog.csdn.net/xiaokui008/article/details/9662937#)
- Bundle-ManifestVersion: 2  
- Bundle-SymbolicName: org.serc.helloworld.client  
- Bundle-Version: 1.0  
- Bundle-Activator: org.serc.helloworld.client.HelloUser  
- Import-Package: org.serc.helloworld;version="[1.0,2.0)",org.osgi.framework  
![](http://static.blog.csdn.net/images/save_snippets.png)
#### 3.3 HelloWorld程序的流程
可能光看代码会比较不容易看清楚程序的执行流程，下图表示了这几个类的各个功能的相互依赖关系，整个关系从Hello接口的定义开始；然后到Hello接口被实现，得到HelloImpl；再到Activator将HelloImpl注册为框架中的一个服务，再到HelloUser通过与框架交互得到刚才注册的服务，并且使用这个服务从而输出字符串；最后一个可选流程是当我们stop
 org.serc.helloworld这个bundle的时候，程序会将之前注册的服务注销掉。
![](https://img-blog.csdn.net/20130827100635015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhb2t1aTAwOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
#### 3.4 程序的执行
       通过上面的工作，我们得到了两个自己定义的bundle： org.serc.helloworldorg.serc.helloworld.client 现在打开Run configurations界面，我们会看见Bundles标签里面多出来了这两个bundle：
也就是说，OSGi框架在启动的时候，会自动install和start这2个bundle，我们点击Run按钮，看看会有什么结果：
#### 3.5利用命令行查看程序执行过程中框架状态的变化
       3.4其实只给出了一个结果，如果你还不太清楚这个结果具体是怎么出来的，那么这一节的内容应该能够帮助你更好的理解输出结果的过程。下面我们通过Equinox的一些命令行来一步一步安装和执行bundle，并且查看过程中框架的状态变化，来让你们搞清楚这个结果是怎么来的。
首先在Run configuration中取消两个helloworldbundle的自动启动：
然后点击Run，这时候就不会立即输出Hello， OSGi字符串了，现在我们先用“ss”命令查看bundle的状态：
可见两个bundle并不是出于ACTIVE状态，说明并没有启动，现在我们执行“start 8”来启动org.serc.helloworld这个bundle：
在用services命令查看当前已经注册的服务，我们会看到一大堆的系统服务中多出来如下一项服务：
这显然是我们在start以后注册上去的，但是现在还没有任何一个bundle在使用这个服务。 接下来我们start 9号bundle，也就是我们用来调用服务的bundle：
这时就输出了“Hello，OSGi”的字符串。
那么如果我们先启动9号bundle而不启动8号bundle会怎么样呢？大家可以试一试，因为我们在代码中已经对没有服务的异常情况做了处理，届时会有相应的输出。我们先停止8号bundle（这里的图中bundleID增加了，大家对号入座）：
大家可以看见刚才注册的服务已经被注销了，现在我们执行refresh 11（也就是刚才的9号bundle）来重新执行其中BundleActivator的start方法：
可见Hello服务已经不复存在了。从这里我们可以看出来，其实Bundle的启动顺序也是一个需要注意的环节，有时候你所定义的bundle是具有顺序敏感性的，必须要某些前置bundle启动了以后，后面的bundle才能正确启动。
[](http://blog.csdn.net/z69183787/article/details/41482947#)[](http://blog.csdn.net/z69183787/article/details/41482947#)[](http://blog.csdn.net/z69183787/article/details/41482947#)[](http://blog.csdn.net/z69183787/article/details/41482947#)
