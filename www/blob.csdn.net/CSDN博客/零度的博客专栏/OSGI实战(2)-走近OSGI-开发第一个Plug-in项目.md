# OSGI实战(2)-走近OSGI-开发第一个Plug-in项目 - 零度的博客专栏 - CSDN博客
2017年01月17日 11:21:17[零度anngle](https://me.csdn.net/zmx729618)阅读数：885
                
上一次我们介绍了什么是OSGI和OSGI在我们的项目中起到了什么作用。这次我们来亲手使用OSGI技术。
主要利用Eclipse开发工具提供给我们的plug-in插件工程模板来创建我们的工程。零距离的体验一下OSGI项目创建的基本过程，和运行流程以及其它相关的知识。
所有语言或技术的第一次测试通常都会以“HelloWorld”作为代表性语句，那么我们接下来将利用OSGI技术，创建一个HelloWorld的插件工程。
(1)创建项目
在Eclipse中我们首先新建一个“plug-in project”项目:
![](https://img-blog.csdn.net/20160316123457937?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击下一步
![](https://img-blog.csdn.net/20160316123510762?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击下一步
![](https://img-blog.csdn.net/20160316123522328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注:其中Equinox是OSGI的一种框架(容器)，这里我们先不讲解。
点击下一步
![](https://img-blog.csdn.net/20160316123535185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击下一步
![](https://img-blog.csdn.net/20160316123557375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们这里选择HelloWorld模板
点击下一步：
![](https://img-blog.csdn.net/20160316123606766?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后点击完成，项目创建完毕。
![](https://img-blog.csdn.net/20160316123616233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(2)项目结构介绍
然后我们打开META-INF文件夹下的MANIFEST.MF文件:
![](https://img-blog.csdn.net/20160316123641030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们以后开发的精力要放在META-INF文件夹下，因为我们很多配置文件的存放和参数的配置都是在这个文件夹中进行的(如spring等)，以后这个文件夹会经常的用到。
工程的所有信息都保存在MANIFEST.MF文件下，我们先将MANIFEST.MF文件的选项卡选择至“OverView”，来看一下都是什么参数:
![](https://img-blog.csdn.net/20160316123653827?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316123706905?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后旁边的Dependencies和Runtime
![](https://img-blog.csdn.net/20160316123729313?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中Dependencies
![](https://img-blog.csdn.net/20160316123744719?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后Runtime
在这里你可以选择对外公布的包。不对外公布的包同工程下的其它插件是无法看到也无法引入的，相同的，此插件能在Dependencies
中引入的插件也必须是其它工程对外公布(Exported)的。
![](https://img-blog.csdn.net/20160316123800376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后我们可以看到还有一个build.properties文件:
![](https://img-blog.csdn.net/20160316123811750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
build.properties文件是我们在进行项目构建的时候用到的一个文件。我们可以在MANIFEST.MF文件下的build选项卡中对其参数进行设置:
![](https://img-blog.csdn.net/20160316123827954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
进行打包、输出的配置。
然后上面是我们插件的引入环境
![](https://img-blog.csdn.net/20160316123845204?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中org.eclipse.osgi_xxxxxx.jar是我们osgi技术的核心，运行osgi的时候首先运行的就是它。
整个文件组成结构我们介绍完了，还是比较简单的结构的。
那么以后我们完整开发一个项目的目录结构大概是什么样子呢？给一张图片给大家做一个参考:
![](https://img-blog.csdn.net/20160316123900283?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当我们开发WebService的时候，还会加入一个文件夹OSGI-INF，用来定义远程服务配置文件。
(3)项目运行
我们新创建的plug-in项目会不会像我们以前创建的java project或者是javaWeb project一样能运行起来吗？
我们下面来运行一下我们的plug-in项目:
我们首先要配置我们的运行选项
![](https://img-blog.csdn.net/20160316123916470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们new一个OSGI Framework:
![](https://img-blog.csdn.net/20160316123927846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
刚出来的时候会默认选中所有bundle:
![](https://img-blog.csdn.net/20160316124058502?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们在workspace中只选中我们的HelloWorld01:
![](https://img-blog.csdn.net/20160316124116395?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意下面这个选项不要勾选:
![](https://img-blog.csdn.net/20160316124134878?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果勾选的话每创建一个bundle它就加进Workspace中，如果加入的bundle是别的项目的，会影响我们的配置，所以我们不要勾选它。
我们选择HelloWorld01之后，在Target Platform中选择一下插件
![](https://img-blog.csdn.net/20160316124151846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后Apply，然后点击Run来运行。
以后运行就不需要这么麻烦了，编译器会直接将我们的Run配置放到选项卡中，我们点击运行就可以了
![](https://img-blog.csdn.net/20160316124204537?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
注意：可以使用这个按钮检查插件是否添加完毕:
![](https://img-blog.csdn.net/20160316124251206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果没有添加完所需的所有插件，运行的时候会报“Missing Bundle”之类的错误。我们可以使用这个按钮来检测我们的环境插件是否添加完毕。如果没有添加完毕，我们可以根据它的提示寻找所需的插件jar。
如果添加的环境可以使系统跑起来，当你点击检测按钮时，那么它会弹出下面这个对话框:
![](https://img-blog.csdn.net/20160316124311226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时你就可以放心大胆的运行了。
运行结果：
![](https://img-blog.csdn.net/20160316124324644?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们输入ss指令，来查看一下当前在容器中运行的模块:
![](https://img-blog.csdn.net/20160316124339523?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
没错，就是我们刚刚配置的那些插件，全部都运行起来了。
(4)查看代码
我们先让项目保持启动状态，我们回到项目目录中去，可以发现系统自动为我们生成了一个Activator.java类:
![](https://img-blog.csdn.net/20160316124434301?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们点击进入之后发现以下代码：
**[java]**[view plain](http://blog.csdn.net/acmman/article/details/50904044#)[copy](http://blog.csdn.net/acmman/article/details/50904044#)[print](http://blog.csdn.net/acmman/article/details/50904044#)[?](http://blog.csdn.net/acmman/article/details/50904044#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/1611945/fork)
- package helloworld01;  
- 
- import org.osgi.framework.BundleActivator;  
- 
- import org.osgi.framework.BundleContext;  
- 
- publicclass Activator implements BundleActivator {  
- /*
-        * (non-Javadoc)
-        * @see org.osgi.framework.BundleActivator#start(org.osgi.framework.BundleContext)
-        */
- publicvoid start(BundleContext context) throws Exception {  
- 
-         System.out.println("Hello World!!");  
- 
-       }  
- 
- /*
-        * (non-Javadoc)
-        * @see org.osgi.framework.BundleActivator#stop(org.osgi.framework.BundleContext)
-        */
- publicvoid stop(BundleContext context) throws Exception {  
- 
-         System.out.println("Goodbye World!!");  
- 
-       }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
package helloworld01;
import org.osgi.framework.BundleActivator;
import org.osgi.framework.BundleContext;
public class Activator implements BundleActivator {
      /*
       * (non-Javadoc)
       * @see org.osgi.framework.BundleActivator#start(org.osgi.framework.BundleContext)
       */
      public void start(BundleContext context) throws Exception {
		System.out.println("Hello World!!");
      }
	
      /*
       * (non-Javadoc)
       * @see org.osgi.framework.BundleActivator#stop(org.osgi.framework.BundleContext)
       */
      public void stop(BundleContext context) throws Exception {
		System.out.println("Goodbye World!!");
      }
}
```
Activator控制了bundle的生命周期。其中start和stop方法，分别指定了当此bundle项目ACTIVE的时候和RESOLVED的时候所要执行的动作。我们在这里分别向控制台输出了“Hello World!!”和“Goodbye World!!”。
我们在控制台中将HelloWorld01停掉，看到它的确输出了“Goodbye World!!”,而且HelloWorld01也进入了RESOLVED状态:
![](https://img-blog.csdn.net/20160316124505260?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
我们重启HelloWorld01项目，发现它输出了“Hello World!!”，并且恢复了ACTIVE状态:
![](https://img-blog.csdn.net/20160316124519229?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这就是bundle的生命周期转换。
我们从运行的情况来看，我们的项目运行的时候就是会自动激活，这是因为我们配置设置的时候，是允许HelloWorld01自启动的:
![](https://img-blog.csdn.net/20160316124531308?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当我们将Auto-Start设置为false的时候，一开始HelloWorld01的状态就变成了RESOLVED状态。
参数Start LeveL是用来设置bundle的启动优先级，数字越小优先级越大。以后我们某些插件依赖某些核心插件，那么核心插件的优先级一定要大于依赖这些核心插件的bundle。
(5)关于控制台/命令行
我们刚刚讲到ss命令，实际上我们Bundle运行起来之后，出现的这个Console界面就是一个命令行界面:
![](https://img-blog.csdn.net/20160316124545379?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
以前我们做Web开发的时候，运行TomCat什么的在Console里面出的都是一些反馈信息之类的，不能输入命令，所以应该有很多朋友很不理解，为什么Console打印东西的地方同时也可以做命令行来输入命令呢？
回想一下，我们在JavaSE开发的时候，其实Console窗口也是可以输入东西的，我们在代码当中加入Scanner(System.in)，就可以向Console窗口中出入数据。
有些朋友对白底黑字的命令行很不习惯，觉得它不像命令行，觉得这种才是标准的命令行(cmd):
![](https://img-blog.csdn.net/20160316124558660?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其实cmd也是可以调整样式的，我们这样调整一番之后就会变成和Eclipse的Console一模一样的效果:
![](https://img-blog.csdn.net/20160316124611114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316124626293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316124636606?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
当然java的Console也可以变成类似cmd的黑底白字:
![](https://img-blog.csdn.net/20160316124649559?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160316124700481?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
效果
![](https://img-blog.csdn.net/20160316124713364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
怎么样，是不是有命令行的感觉了？(其实黑底不爽，我换回来了)
所以我们的osgi>其实就是一个命令行输出与查看的控制台。
很多指令现在不去关注，等到后面我们做项目的时候使用到了再一一讲解。
通过这个项目的创建和演示，基本上知道了遵照什么过程来开发OSGI插件。
**转载请注明出处：[http://blog.csdn.net/acmman/article/details/50904044](http://blog.csdn.net/acmman/article/details/50904044)**
