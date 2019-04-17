# CUDA Dll的开发流程 - 战斗蜗牛的专栏 - CSDN博客





2012年11月03日 09:56:51[vbskj](https://me.csdn.net/vbskj)阅读数：740









下载了例子，运行有问题，所以把openhero的文章找出来，重新做一遍。

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



前言：很久没写CUDA相关的文章了，其实也不是忙，只是零碎的事情比较多，不能抽出完整的时间写一些东西，在[http://blog.csdn.net/openhero](http://blog.csdn.net/openhero) 上写本来想写一些列CUDA编程的文章，不过现在看来，很多朋友还是只是处在开发的初级阶段，一些基本的编程环节还需要讲解一下，其实像lib编程，dll编程，都不是CUDA的内容，这个只是windows，linux系统编程的内容，只要有时间，好好静下心来做几个项目就可以明白很多，而且网上就可以找到很多资料。


正文：CUDA的DLL开发其实和一般的C/C++的DLL开发是一个原理，当然，DLL的开发就有几种方式，这里就讲最容易理解的，也最直接的方式，然后把代码放出来。大家自己可以琢磨一下其它的方式。


**1. 创建DLL**

用我的[Wizard](http://blog.csdn.net/OpenHero/archive/2009/03/10/3975962.aspx) 可以创建一个DLL项目工程：


![clip_image002](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image002_thumb_633748829357197500.jpg)

选择DLL项目，然后可以看到下面的项目文件结构： 

![clip_image004](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image004_thumb_633748829369541250.jpg)

其中sampe_cu.h文件为头文件，stdafx.h和stdafx.cpp是windows的加载的相关文件，这里可以不用理会，DLL_Test.cpp是其中的一个导出函数文件，也是声明了DkkMain的入口函数，这里是在DLL创建，加载，卸载的时候需要处理的一些内容，这里如果不熟悉DLL也不用管它。


针对CUDA，这里我们就来看sample.cu文件： 

/************************************************************************/ 

/* HelloCUDA */ 

/************************************************************************/ 

int TestHelloCUDA(void) 

{ 

if(!InitCUDA()) { 

return 0; 

} 

char *device_result = 0; 

char host_result[12] ={0}; 

CUDA_SAFE_CALL( cudaMalloc((void**) &device_result, sizeof(char) * 11)); 

unsigned int timer = 0; 

CUT_SAFE_CALL( cutCreateTimer( &timer)); 

CUT_SAFE_CALL( cutStartTimer( timer)); 

HelloCUDA<<<1, 1, 0>>>(device_result, 11); 

CUT_CHECK_ERROR("Kernel execution failed/n"); 

CUDA_SAFE_CALL( cudaThreadSynchronize() ); 

CUT_SAFE_CALL( cutStopTimer( timer)); 

printf("Processing time: %f (ms)/n", cutGetTimerValue( timer)); 

CUT_SAFE_CALL( cutDeleteTimer( timer)); 

CUDA_SAFE_CALL( cudaMemcpy(&host_result, device_result, sizeof(char) * 11, cudaMemcpyDeviceToHost));


printf("%s/n", host_result); 

CUDA_SAFE_CALL( cudaFree(device_result)); 

//CUT_EXIT(argc, argv); 

return 0; 

} 

这个就是sample.cu文件里面的实现的dll的接口函数，当然还有一个def文件，这个文件是用来做动态导出用的，看看def文件： 

LIBRARY "DLL_Test" 

EXPORTS 

TestHelloCUDA 

这个是sample.def文件的内容，这个文件是为了能动态导出DLL里面的函数接口而用的。编译器通过这个文件把声明的TestHelloCUDA，接口导出。 

编译，就可以在相应的目录里面得到一个xxxx.dll文件. 

**2. 调用DLL**

这里就是测试DLL文件，当然我在同一个工程里面创建一个调用的测试工程： 

![clip_image006](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image006_thumb_633748829412353750.jpg)

这里添加一个测试工程，当然你也可以再另外的地方创建一个测试工程，或者加载到自己的工程里面； 

选择一个win32工作作为测试例子； 

![clip_image008](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image008_thumb_633748829426260000.jpg)

这个例子就是一个简单的测试项目，都选默认得就可以； 

![clip_image010](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image010_thumb_633748829437353750.jpg)

这里默认的就可以了，其他的自己去查找意思J不能偷懒到什么都告诉你…… 

![clip_image012](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image012_thumb_633748829448447500.jpg)

这个时候就可以看到两个工程，一个是dll工程，一个是测试工程，然后在测试工程里面，我们采用动态加载DLL的方法来调用DLL； 

![clip_image014](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image014_thumb.jpg)

上面就是调用DLL的代码，这里为什么加__T()自己去找unicode的调用方法J 

里面的__T()里面的dll的文件名就是我们要加载的DLL，当然这里是路径，入下图，我们的dll和exe在同一个文件夹里面，如果是不同的文件夹，那就用相对路径来找就okJ


![clip_image016](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image016_thumb.jpg)

然后运行例子，就可以得到下图结果: 

![clip_image018](https://p-blog.csdn.net/images/p_blog_csdn_net/OpenHero/453683/o_clip_image018_thumb.jpg)

这里的DLL就ok了~~~ 

代码可以再这里下载：

[http://d.download.csdn.net/down/1188868/OpenHero](http://d.download.csdn.net/down/1188868/OpenHero)

[http://groups.google.com/group/cudagroup/web/DLLTest.rar?hl=zh-CN](http://groups.google.com/group/cudagroup/web/DLLTest.rar?hl=zh-CN)




