# OSGI笔记(一)之Hello World应用程序 - 零度的博客专栏 - CSDN博客
2016年08月09日 10:10:07[零度anngle](https://me.csdn.net/zmx729618)阅读数：726
        OSGI的概念我就不多说了，百度谷歌一堆，随便看看的了，反正咱也不是做讲师，多学自会理解。今天刚开始先记一个HelloWorld，Yes,you are my World。一般情况下，学习一门新的技术，程序员都习惯于首先开发一个 hello world 应用程序，这似乎也是一种“工业标准”。好的，让我们开始吧，开发一个简单的 OSGi 应用程序并不难，步骤如下：- 建立一个 plug-in 工程，**File > New > Project**，选择**Plug-in development > Plug-in Project**
**图 1. 新建 plug-in 工程**
![图 1. 新建 plug-in 工程](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image001.png)
- 在建立工程的第一个向导，填入工程的名称：`osgi.test.helloworld`，使用缺省的工程路径。注意目标平台的选择，由于我们的项目是一个通用的 OSGi bundle，所以选择**equinox**。
**图 2. 填入工程名及选择目标平台**
![图 2. 填入工程名及选择目标平台](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image002.png)
- 在下一个向导界面中，填入需要的一些插件信息（注意 Eclipse 中的插件概念基本类似于 OSGi 中的 bundle 的概念），这里需要填入的是 OSGi 的 provider（供应商）和 classpath 。如果没有特别的设计，一般可以忽略这两个字段 。最后是关于 activator 的部分，如果不是一个 fragment bundle 则需要填入，除非您的 bundle 自己实现框架的事件监听，这个似乎也没有必要。因此，建议使用缺省的设置，如图
 3：
**图 3. 使用缺省设置**
![图 3. 使用缺省设置](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image003.png)
**Activator：**这是 bundle 启动时首先调用的程序入口，相当于 Java 模块中的 main 函数。不同的是，main 需要通过命令行调用，而 OSGi 的 Activator 是被动的接受 OSGi 框架的调用，收到消息后才开始启动。
最佳实践：不要在 Activator 中写太多的启动代码，否则会影响 bundle 启动速度，相关的服务启动可以放到服务的监听器中。
- 最后一步，不使用任何的模板，所以勾掉缺省的选项，点击完成，如图 4：
**图 4. 勾掉缺省的选项**
![图 4. 勾掉缺省的选项](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image004.png)
- 完成，基本的插件视图如图 5，Eclipse 会在工程名下建立相同路径的 Java Package，其中包含了 Activator 类，插件的配置信息也都放在 MANIFEST.MF 文件中，将来我们相当多的工作都是在其中完成。
**图 5. 基本的插件视图**
![图 5. 基本的插件视图](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image005.png)
- 编辑 Activator.java，输入 hello world 语句，代码如下：
**清单 1. 编辑 Activator.java**
<span style="font-family:Arial;font-size:12px;">							
package osgi.test.helloworld; 
import org.osgi.framework.BundleActivator; 
import org.osgi.framework.BundleContext; 
public class Activator implements BundleActivator { 
	 /* 
	 * (non-Javadoc) 
	 * @see org.osgi.framework.BundleActivator 
	 *     #start(org.osgi.framework.BundleContext) 
	 */ 
	 public void start(BundleContext context) throws Exception { 
	    <strong>System.out.println("hello world"); </strong>
	 } 
	 /* 
	 * (non-Javadoc) 
	 * @see org.osgi.framework.BundleActivator 
	 *     #stop(org.osgi.framework.BundleContext) 
	 */ 
	 public void stop(BundleContext context) throws Exception { 
	 } 
 }</span>
我们可以看到每个 Activator 实际都是实现了`BundleActivator`接口，此接口使 Activator 能够接受框架的调用。在框架启动后，启动每个 bundle 的时候都会调用每个
 bundle 的 Activator 。
注意：bundle 的 Activator 必须含有无参数构造函数，这样框架才能使用`Class.newInstance()`方式反射构造 bundle 的 Activator 实例。
这里我们在`start`方法中填入了我们希望输出的 hello world 字符串。那么，怎么才能启动这个 bundle 呢？
- 执行：选择**Run > Open Run Dialog**，进入运行菜单，在 OSGi framework 中右键点击选择**new**一个新的 OSGi 运行环境，如图：
**图 6. 新建 OSGi 运行环境**
![图 6. 新建 OSGi 运行环境](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image006_1.png)![图 6. 新建 OSGi 运行环境](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image006_2.png)
在右边的运行环境对话框中，输入运行环境的名字、start level 和依赖的插件，由于我们目前不需要其它的第三方插件，因此只需要勾上系统的 org.eclipse.osgi 插件，如果不选择此插件，hello world 将无法运行。如图 7，只有当您点击了**validate bundles**按钮 ，并且提示无问题之后，才表明您的运行环境基本 OK 了。
**图 7. 选择 org.eclipse.osgi插件**
![图 7. 选择 org.eclipse.osgi插件](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image007.png)
依赖插件的选择：
**图 8. 依赖插件的选择**
![图 8. 依赖插件的选择](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image008.png)
好的，如果您的运行环境已经 OK，那么就点击**Run**吧。
**图 9. 运行 OSGi 项目**
![图 9. 运行 OSGi 项目](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image009.png)
恭喜您，成功了！
OSGi 控制台
OSGi 控制台对于习惯开发普通 Java 应用程序的开发人员来说，还是比较新鲜的。一般来说，通过 OSGi 控制台，您可以对系统中所有的 bundle 进行生命周期的管理，另外也可以查看系统环境，启动、停止整个框架，设置启动级别等等操作。如图 10，键入`SS`就可以查看所有
 bundle 的状态：
**图 10. 查看所有 bundle 的状态**
![图 10. 查看所有 bundle 的状态](http://www.ibm.com/developerworks/cn/education/opensource/os-eclipse-osgi/image010.png)
下面列出了主要的控制台命令：
**表 1. Equinox OSGi 主要的控制台命令表**
|**类别**|**命令**|**含义**|
|----|----|----|
|控制框架|`launch`|启动框架|
|`shutdown`|停止框架| |
|`close`|关闭、退出框架| |
|`exit`|立即退出，相当于 System.exit| |
|`init`|卸载所有 bundle（前提是已经 shutdown）| |
|`setprop`|设置属性，在运行时进行| |
|控制 bundle|`Install`|安装|
|`uninstall`|卸载| |
|`Start`|启动| |
|`Stop`|停止| |
|`Refresh`|刷新| |
|`Update`|更新| |
|展示状态|`Status`|展示安装的 bundle 和注册的服务|
|`Ss`|展示所有 bundle 的简单状态| |
|`Services`|展示注册服务的详细信息| |
|`Packages`|展示导入、导出包的状态| |
|`Bundles`|展示所有已经安装的 bundles 的状态| |
|`Headers`|展示 bundles 的头信息，即 MANIFEST.MF 中的内容| |
|`Log`|展示 LOG 入口信息| |
|其它|`Exec`|在另外一个进程中执行一个命令（阻塞状态）|
|`Fork`|和 EXEC 不同的是不会引起阻塞| |
|`Gc`|促使垃圾回收| |
|`Getprop`|得到属性，或者某个属性| |
|控制启动级别|`Sl`|得到某个 bundle 或者整个框架的 start level 信息|
|`Setfwsl`|设置框架的 start level| |
|`Setbsl`|设置 bundle 的 start level| |
|`setibsl`|设置初始化 bundle 的 start level| |
MANIFEST.MF
MANIFEST.MF 可能出现在任何包括主类信息的 Jar 包中，一般位于 META-INF 目录中，所以此文件并不是一个 OSGi 特有的东西，而仅仅是增加了一些属性，这样也正好保持了 OSGi 环境和普通 Java 环境的一致性，便于在老的系统中部署。表 2 列出此文件中的重要属性及其含义：
**表 2. MANIFEST.MF 文件属性**
|**属性名字**|**含义**|
|----|----|
|`Bundle-Activator`|Bundle 的启动器|
|`Bundle-SymbolicName`|名称，一般使用类似于 JAVA 包路径的名字命名|
|`Bundle-Version`|版本，注意不同版本的同名 bundle 可以同时上线部署|
|`Export-Package`|导出的 package 声明，其它的 bundle 可以直接引用|
|`Import-Package`|导入的 package|
|`Eclipse-LazyStart`|是否只有当被引用了才启动|
|`Require-Bundle`|全依赖的 bundle，不推荐|
|`Bundle-ClassPath`|本 bundle 的 class path，可以包含其它一些资源路径|
|`Bundle-RequiredExecutionEnvironment`|本 bundle 必须的执行环境，例如 jdk 版本声明|
