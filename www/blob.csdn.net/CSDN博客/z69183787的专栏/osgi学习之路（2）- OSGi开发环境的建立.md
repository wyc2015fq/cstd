# osgi学习之路（2）- OSGi开发环境的建立 - z69183787的专栏 - CSDN博客
2014年11月25日 15:47:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1063
**1.1 Equinox是什么**
从代码角度来看，Equinox其实就是OSGi核心标准的完整实现，并且还在这个基础上增加了一些额外的功能（比如为框架增加了命令行和程序执行的入口）。我们在之前入门篇讲解的都东西其实都是OSGi核心标准的一小部分。其实它的核心就是一个jar包，这个jar包既能执行（作为标准Java包的特性），也是一个bundle（Manifest里面含有OSGibundle特有的元数据）。
现在你需要知道的就是，我们能够利用Equinox项目的代码来运行一个实实在在的OSGi框架，框架启动后，你就可以将你开发好bundle放到里面运行。
**1.2下载Equinox**
Equinox在[http://download.eclipse.org/equinox](http://download.eclipse.org/equinox)上有官方下载，里面列出了各个版本供我们选择：
在这里，我们使用3.7版本的Equinox，下载好以后放在一个单独的文件夹下（这里我的路径是E:\OSGiframework\equinox）：
**1.3从命令行启动框架**
如果启动这个框架的话，有了上面的jar包就足够了，我们进入命令行输入如下命令：java –jar org.eclipse.osgi_3.7.0.v20110613.jar -console ，然后就会进入Equinox的控制台：
如果出现osgi>的提示符，就说明启动成功了。
Equinox的控制台的部分基本命令如下（区分大小写）：
## 1.4在Eclipse中建立开发环境
在上一节中大家看到启动和控制框架的方法，是相当简单的一个过程。不过单单只是运行环境还不够，我们还需要开发环境。
### 1.4.1设置
首先我们不需要安装必须的插件，只要你有较新版本的Eclipse就行了。然后进入Eclipse的window->preferences界面，选中Plug-inDevelopment下的Target Platform：
现在右边只有一个Runing Platform的，我们任务是点击“Add…”按钮来增加一个我们自己的的platform，
选择默认的第一个就好，点击next。
再点击这里的“Add…”：
选择“Directory”：
选择你的Equinox的jar包所在的路径,然后点击finish，回到刚才的界面：
这时候你就会发现里面多出来了你刚刚设置的路径，路径后面描述的“1 plug-ins available”则就是说的我们放置的Equinox的jar包。
继续点击finish，回到最开始的界面：
这时候多出来了一个新的targetplatform，勾选上，然后确定。
### 1.4.2启动
打开菜单项Run->Run configurations…，在OSGi Framework项中，新建一个Run configuration：
这里面现在已经自动包含了Equinox的jar包了，点击Run，看看运行的效果：
Eclipse的控制台中也出现来了osgi的提示符，说明你已经成功启动了。
你可以试试刚才讲的那些命令，看看能输出些什么（比如上图中我输入了ss）。
### 1.4.3新建一个project
打开新建project的界面，选择Plug-in Project：
然后输入project的名字，TargetPlatform处选择an OSGiframework->Equinox或者standard都行，点击下一步：
这里实际上是对bundle的Manifest文件的设置，其中的ID就是Bundle-SymbolicName，Version就是bundle的版本号，下面还能决定是否定义BundleActivator，点击finish就创建了一个project：
至此，开发环境已经建立完毕。
