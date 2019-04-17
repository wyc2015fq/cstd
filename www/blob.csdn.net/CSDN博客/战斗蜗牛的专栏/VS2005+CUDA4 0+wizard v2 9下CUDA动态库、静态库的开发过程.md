# VS2005+CUDA4.0+wizard v2.9下CUDA动态库、静态库的开发过程 - 战斗蜗牛的专栏 - CSDN博客





2012年11月07日 12:30:14[vbskj](https://me.csdn.net/vbskj)阅读数：1859








参考了开勇的dll过程，其中有一些细节的地方给予说明，原文中没有提及；此外，提供的src可能是老版本的，在新环境下无法运行。



**1. 创建DLL**

1）用开勇的[Wizard](http://blog.csdn.net/OpenHero/archive/2009/03/10/3975962.aspx)v2.9 可以创建一个DLL项目工程：

![clip_image002](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image002_thumb_633748829357197500.jpg)

2）选择DLL项目，然后可以看到下面的项目文件结构： 

![clip_image004](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image004_thumb_633748829369541250.jpg)

其中sampe_cu.h文件为头文件，stdafx.h和stdafx.cpp是windows的加载的相关文件，这里可以不用理会，DLL_Test.cpp是其中的一个导出函数文件，也是声明了DkkMain的入口函数，这里是在DLL创建，加载，卸载的时候需要处理的一些内容，这里如果不熟悉DLL也不用管它。



3）注意：此时编译工程，会出现错误，说“找不到cutil64D.lib”。此时，需要来到目录“C:\ProgramData\NVIDIA Corporation\NVIDIA GPU Computing SDK 4.0\C\common”，用x64模式编译Cutil动态库和静态库，编译完毕后，拷贝Cutil64D.lib、Cutil64.lib到“C:\Program Files\NVIDIA GPU Computing Toolkit\CUDA\v4.0\lib\x64”中，然后创建的dll工程正常运行。



4）工程里面的sample.cu文件里面的就是实现的dll的接口函数，当然还有一个def文件，这个文件是用来做动态导出用的，看看def文件：

LIBRARY "DLL_Test" 

EXPORTS 

TestHelloCUDA 

这个是sample.def文件的内容，这个文件是为了能动态导出DLL里面的函数接口而用的。编译器通过这个文件把声明的TestHelloCUDA，接口导出。



5）总的来说，在dll向导下，你只需要修改sample.cu、sample_cu.h、sample.def三个文件即可。第一个是代码，第二个是调用dll所需要的函数定义，第三个是dll动态导出。至此，dll创建完毕。

编译，就可以在相应的目录里面得到一个xxxx.dll文件. 



**2. 创建lib静态链接库**

静态库其实 就是对于函数代码的打包，实现起来更加容易；使用wizard，选择“Static Library”选项即可；



注意：和dll不同，lib只需要修改sample.cu，sample _cu.h两个文件即可。







**3. 调用dll**

为了说明过程，重新建立一个win32工程： 



![](https://img-my.csdn.net/uploads/201211/07/1352257586_2872.jpg)


然后，一路默认下去即能创建一个简单的win32工程：

![](https://img-my.csdn.net/uploads/201211/07/1352257592_4731.jpg)

********************************

下面是重点了：



1）在新建的win32工程中 ，右键工程属性-》配置属性-》链接器，在附加依赖项中加入，刚才cuda_dll工程产生的同名lib文件；

![](https://img-my.csdn.net/uploads/201211/07/1352257616_9641.jpg)



2）在新建的win32工程中 ，包含cuda_dll工程输出的头文件，然后在程序代码中加入dll的输出函数“TestHelloCUDA（）”

![](https://img-my.csdn.net/uploads/201211/07/1352257693_5718.jpg)



3） 并把cuda_dll工程产生的dll文件、cutil64d.dll两个动态库，拷贝到debug目录，运行成功如下所示：

![](https://img-my.csdn.net/uploads/201211/07/1352257697_9873.jpg)



4）下面测试如何加入静态链接库，首先把上述的两个dll拷贝走，再尝试运行exe，会出如下所示，证明当前的exe还是依赖动态库的，接着往后看如何修改成依赖静态库；



![](https://img-my.csdn.net/uploads/201211/07/1352257712_6660.jpg)![](https://img-my.csdn.net/uploads/201211/07/1352257716_8703.jpg)



￥￥￥￥￥￥￥￥￥￥￥￥￥￥

加入静态库



1）还是在属性-》配置属性-》链接器重，把“附加依赖项”，修改为刚才static library输出的lib文件；但是还是不够的，还需要添加cudart.lib以及cutil64d.lib。





![](https://img-my.csdn.net/uploads/201211/07/1352257735_5130.jpg)



![](https://img-my.csdn.net/uploads/201211/07/1352258256_9625.jpg)



2）为什么还需要添加cudart.lib以及cutil64d.lib？看看下图，dll和static两个的工程属性，dll带两个基本库，所以在使用的时候只添加动态库就够了；而static工程中，仅仅是对于代码的打包，不包含cuda基本库，所以在使用的时候还需要加上，不然出现一堆链接的错误，说找不到函数。





![](https://img-my.csdn.net/uploads/201211/07/1352258238_6242.jpg)





![](https://img-my.csdn.net/uploads/201211/07/1352258252_7210.jpg)



3）如下图所示，不仅添加基本库的lib，还需要在VC++目录中，添加cuda toolkit的lib目录，以便编译器搜索。



![](https://img-my.csdn.net/uploads/201211/07/1352258262_2924.jpg)



4） 最后，static library版本成功运行，虽然不需要TestHelloCUDA函数动态库，但是还是需要基本库的dll。



至此，ok。



如需要代码，请留下邮箱。











