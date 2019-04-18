# RapidMiner 5.3.015源代码下载并且正确的运行 - YZXnuaa的博客 - CSDN博客
2018年05月29日 11:12:49[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：251
一，RapidMiner介绍 
1， RapidMiner是一个开源数据挖掘工具，可以进行机器学习，数据挖掘，文本挖掘，预测性分析和商业分析，具有拖拽功能的图形化工具。无需编程。它开发且可扩展，拥有上百种数据载入，转化，建立和可视化的方法，可以连接到一系列复杂的数据源，如Excel，Access， IBM DB2， MySQL。你甚至可以很简单地利用RapidMiner提供的开放式扩展API将自己的算法嵌入到RapidMiner中。
2，rapidminer中的功能均是通过连接各类算子（operator）形成流程（process）来实现的，整个流程可以看做是工厂车间的生产线，输入原始数据，输出模型结果。算子可以看做是执行某种具体功能的函数，不同算子有不同的输入输出特性。
二，RapidMiner 5.3.015版本是开源的版本，现在要在IDEA上安装这个RapidMiner 5.3.015版本并且分析它，现在RapidMiner 7.3.0000版本不开源的。 
1，RapidMiner 5.3.015版本的界面如下： 
![这里写图片描述](https://img-blog.csdn.net/20161123195020492)
界面每个框架的介绍： 
![这里写图片描述](https://img-blog.csdn.net/20161123195308140)
2，RapidMiner 7.3.000版本的界面如下： 
![这里写图片描述](https://img-blog.csdn.net/20161123195118837)
三，RapidMiner 5.3.015源代码下载并且正确运行 
1，先下载好RapidMiner 5.3.015源代码，下载网址： 
[http://download.csdn.net/detail/xwd18280820053/8906681](http://download.csdn.net/detail/xwd18280820053/8906681)
2，下载好的源代码如下： 
![这里写图片描述](https://img-blog.csdn.net/20161123195823240)
3，把RapidMiner 5.3.015源代码可以导入myeclipse或者IntelliJ IDEA。我这里是把项目导入了IntelliJ IDEA，导入后如下： 
![这里写图片描述](https://img-blog.csdn.net/20161123200024623)
4，导入后，出现了错误，错误就是找不到如下的包： 
import com.sun.javadoc.ClassDoc; 
import com.sun.javadoc.RootDoc; 
import com.sun.javadoc.SeeTag; 
import com.sun.javadoc.Tag; 
import com.sun.tools.doclets.Taglet; 
. 
解决的方案： 
这些包就在我们的JDK中就有，打开我们的JDK安装目录，然后进入到lib目录， 
在里面的tools.jar就是了。即 jdkjdk1.8.0_91\lib\tools.jar. 
我的是如下： 
![这里写图片描述](https://img-blog.csdn.net/20161123200242511)
因为我的jdk是jdk1.8.0_91。然后把tools.jar包导入到刚才的项目里，如下： 
![这里写图片描述](https://img-blog.csdn.net/20161123200618797)
具体步骤： 
点击项目右键出现—-》Open Module Settings—>点击Modules—>中间点击绿色加号—–》找到刚才提到的tools.jar路径，添加tools.jar—-》点击Apply 确定即可。这样就可以解决刚才出现错误的地方了。 
![这里写图片描述](https://img-blog.csdn.net/20161123201127913)
5，解决了刚才的问题了，然后按如下步骤找到程序入口，并且运行它 
src—->com—->rapidminer—->gui——>RapidMinerGUI ，直接运行RapidMinerGUI，就可以了： 
![这里写图片描述](https://img-blog.csdn.net/20161123201435871)
因为 ，以GUI方式启动RapidMiner时，其入口是类com.rapidminer.gui.RapidMinerGUI 
main函数如下： 
![这里写图片描述](https://img-blog.csdn.net/20161123201640004)
6，运行其程序，界面如下： 
![这里写图片描述](https://img-blog.csdn.net/20161123201723473)
四，大家可以RapidMiner 5.3.015源代码进行研究了，尤其是算子的拖拽实现的代码也可以进行了解了。
