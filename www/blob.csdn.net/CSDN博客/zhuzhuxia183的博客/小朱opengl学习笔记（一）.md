# 小朱opengl学习笔记（一） - zhuzhuxia183的博客 - CSDN博客





2017年11月19日 00:10:33[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：254








# 第一课：vs2015+opengl3.3环境配置(windows环境)

**首先，说明下，本人是一名大三学生，由于这个学期选修了计算机图形学，老师要求我们使用opengl来实现。本人在配置的过程中，遇到很多的疑惑点，可能是由于本人对于c/c++的相关的编译，运行原理不懂，所以花了很多的功夫，让我头疼不已。所以这也是我准备开始这个简单的opengl学习记录专题的动机———–网上的教程很多由于年代比较久远，而且可能是由于大佬写的，所以我们作为新手，对于c/c++确实不熟悉的人，还是很难看懂的。所以废话了这么多，就是相说，，这个教程，我会全程细心的讲，虽然不那么权威，但是我希望能够向读者传达我自己学习当中的教训。**

这篇文章，我就只是想详细说明下，关于opengl在vs上面的相关配置问题。是的，我会在所有重要的地方附图的。

1.首先第一步，就是我们需要安装vs2015， [链接1](https://www.visualstudio.com/zh-hans/vs/older-downloads/)
![](https://img-blog.csdn.net/20171119002328925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)然后我们就可以直接直接安装，安装的时候注意一点吧，就是不要全选，因为会大，记得要选上带visual c++的那一栏，就行啦，后面可选课不选。
2.然后就是要在我们的vs上面配置我们的opengl环境了。关于c/c++的相关环境配置，无非就是要制定相关的库啊，以及相关的头文件，当然在此之前，我先说个坑，就是在window系统上面，认定的c++的头文件就是.h格式，然后库文件分成两类，一类就是动态库（格式为.dll），一类就是静态库(格式为.lib)，这两个格式的话，其实只需要配置一个就ok了，这里笔者采用的就是用静态库，当时老师给我的所有文件中，包含了.lib和.dll文件，我傻傻的以为都要配置。唉。额，这里是文件的[链接](http://download.csdn.net/download/zhuzhuxia183/10124339),打开后为两个文件夹，一个是include文件夹，一个是lib文件夹，分别是头文件和库文件。![这里写图片描述](https://img-blog.csdn.net/20171119014714065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，好的，如果没有问题，那我们继续。 

3.然后我们把所有的东西都准备好后，然后我们首先打开我们的[文件](http://download.csdn.net/download/zhuzhuxia183/10124346)，效果的话，应该是如图![这里写图片描述](https://img-blog.csdn.net/20171119013011877?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后我们点击，使用visual  studio 2105打开，右击项目名，图片左边显示我是用release X86 
![这里写图片描述](https://img-blog.csdn.net/20171119013115670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后我们先要进入到我们下载的头文件，库文件文件夹那里，复制inlcude的路径 
![这里写图片描述](https://img-blog.csdn.net/20171119013422906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后![这里写图片描述](https://img-blog.csdn.net/20171119013439831?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)然后在涂上面的指定位置，粘贴即可。![这里写图片描述](https://img-blog.csdn.net/20171119013525108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

同理，我们要复制我们的lib目录![这里写图片描述](https://img-blog.csdn.net/20171119013550751?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

然后粘贴在相应位置 
![这里写图片描述](https://img-blog.csdn.net/20171119013634186?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

接着我们还需要在制定的地方写入： 
![这里写图片描述](https://img-blog.csdn.net/20171119013730536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

内容为：x 

glfw3.lib 

opengl32.lib 

glew32s.lib 

SOIL.lib
接着（附图）：

![这里写图片描述](https://img-blog.csdn.net/20171119014126555?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![这里写图片描述](https://img-blog.csdn.net/20171119014148393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)‘

![这里写图片描述](https://img-blog.csdn.net/20171119014213183?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

最后之用![这里写图片描述](https://img-blog.csdn.net/20171119014243260?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
成功后，结果： 
![这里写图片描述](https://img-blog.csdn.net/20171119014307869?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1emh1eGlhMTgz/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ok,到这里为止，我们的opengl的环境就配置好了，以后我们如果需要自己写一个项目的时候，首先是新建一个visual c++ 程序，选择空项目，然后同样的右击，后面的步骤都是一样的。但是提醒一点，就是如果空项目当中你没有写一句代码，打开属性后，发现没有c++这个列表，这个问题解决办法是：我们首先要新建一个.cpp文件，然后再写入点代码，随便写点，c t r l+s然后就会出现了。

对了，说下，大家配置的话，还是选择在windows环境下配置吧，不然的话，有些库文件，不好找，得使用gcc自己编译之类的，我也不是很懂，所以对于那些使用mac上面Xcode配置的小伙伴说声抱歉，windows环境和mac环境上面的库文件的格式不同，一个是.lib格式，一个是.dylib格式的，所以对于mac用户来说，如果你也是一名小白，看来只得装虚拟机或者跑个双系统来在windows环境下配置啦。 

好啦，这次的所有配置讲解，就到这啦。加油啊，老哥们。

最后的话，附上本人联系方式：QQ 2956054174。欢迎骚扰。







