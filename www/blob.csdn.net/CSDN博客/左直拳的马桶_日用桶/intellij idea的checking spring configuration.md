# intellij idea的checking spring configuration - 左直拳的马桶_日用桶 - CSDN博客
2018年07月22日 11:37:36[左直拳](https://me.csdn.net/leftfist)阅读数：864
每次在intellij idea打开项目，都要等好长一段时间，据我观察，依次做这么多步骤：
1、首先是打开封面的时候，有个进度条，估计是加载intellij idea本身。在我那台2015年申领的笔记本上，这个步骤也要很长时间
2、load project 
1）load components 
2）load modules 
3）准备workspace
3、scan files to index
4、check spring configuration
这么多步骤里，以最后这个步骤最为漫长。至少要经过三轮进度条。最后还弹出一个气泡，告诉我说有文件与spring不匹配。这个似乎也不影响编译和生成。但就是那么讨厌，每次都出来，生命就在漫长的等待中一次次消耗。
我多次尝试过解决这个警告问题。后来终于按照要求匹配，警告是没有了，但每次打开intellij idea，这个检查动作依旧会做。也没有一个什么选项说可以关掉这个检查。就是这么固执。我目前对spring还不熟悉，也不知道idea为啥每次都要检查。java本身并不难学，造成诸多障碍的是相关的那些配套，这框架那框架，这工具那工具的，说是方便你开发，但你不了解吧，居然就开发不了。而且这些框架、工具还特别多，五花八门，出处不同，大家互相捧场，超耐磨。
这个提示有啥啥文件跟spring不匹配的警告是怎么去掉的呢？就是按照网文的提示弄的： 
打开Project Structure-module，找到相关的module，展开，选”spring”，然后按“+”号，在列表中勾选警告提示中提到的文件，保存或应用即可。 
![这里写图片描述](https://img-blog.csdn.net/20180722112625761?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180722112712712?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
但如果是这么顺畅，就不叫java了。那不是java的作风。为什么这么说呢，当我勾选了文件，想按“Apply”，让修改生效的时候，有错误提示出来了：
```
Content root "E:\Project\cas\cas-5.1.8\webapp\resources" is defined for modules "cas-server-webapp-jetty_main" and "cas-server-webapp-tomcat_main".
Two modules in a project cannot share the same content root.
```
这是什么鬼？想改也改不了啊。原本想修正错误A，结果发现前提要修正错误B。这个错误B咋解决呢？也是在Project Structure-module，找到提示的module，将提示上说的“cas-5.1.8\webapp\resources”剔除掉 
![这里写图片描述](https://img-blog.csdn.net/20180722113538774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
剔除掉之后就能保存了。但下次打开的时候，剔除掉的东东还会重新在那里，如果你要更改什么东西，还麻烦你重新剔除一遍。嗯。
开源的好处就是多人参与，集思广益，各种概念、思想碰撞，百花齐放；缺点就是注重思想和技术的新颖有余，而体验严谨不足。这跟微软系列形成鲜明的对比。你看visual studio，那真的是一个宇宙级别的IDE，反观java系有吗？eclipse免费是免费了，但那种界面谁用谁怕；收费的intellij idea受到追捧，号称比eclipse强了不是一点半点，但我现在折腾得差点想将机器砸了。intellij idea是个吃内存大户，没有12G以上的内存，或者没有固态硬盘充当虚拟内存的话，最好别玩。
