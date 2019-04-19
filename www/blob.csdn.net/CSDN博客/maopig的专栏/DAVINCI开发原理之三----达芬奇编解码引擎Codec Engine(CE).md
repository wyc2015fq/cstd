# DAVINCI开发原理之三----达芬奇编解码引擎Codec Engine(CE) - maopig的专栏 - CSDN博客
2011年11月02日 19:41:56[maopig](https://me.csdn.net/maopig)阅读数：2329标签：[引擎																[codec																[算法																[server																[服务器																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=服务器&t=blog)](https://so.csdn.net/so/search/s.do?q=server&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)](https://so.csdn.net/so/search/s.do?q=引擎&t=blog)
个人分类：[DM368](https://blog.csdn.net/maopig/article/category/919673)
                
DaVinci是DSP和ARM双核架构的SOC芯片。对芯片与外界的交互通过ARM端的Montavista Linux和相关驱动与应用程序来管理，DSP端只处理编解码相关的算法。DSP和ARM之间的通讯和交互是通过引擎(Engine)和服务器(Server)来完成的。
**1. 编解码引擎(Codec Engine)****a. 核心引擎API**    从应用来说，CE就是用来调用xDAIS算法的一组API的集合，用户可以通过这些API来实例化和调用xDAIS算法。达芬奇提供了一组VISA接口，用于给应用程序与xDM兼容的xDAIS算法相交互。需要注意，不管算法是运行在本地(ARM端)，还是远端(DSP端)，也不管硬件体系是只有ARM或是只有DSP或两者都有，也不管OS是Linux、VxWorks、DSP/BIOS,还是WinCE,对算法的接口调用都是一致的。这点通过引擎的配置文件*.cfg可以看出来，而且通过配置文件可以决定自己的codec是运行在ARM端还是DSP端。
    CE包括核心引擎API和VISA API，核心引擎API相关接口模块为:初始化模块(CERuntime_)、CE运行时模块(Engine_)、抽象层内存模块(Memory_);
VISA API的接口模块我们常用的有: 视频编码接口(VIDENCx_)、视频解码接口(VIDDECx_)、音频编码接口(AUDENCx_)、音频解码接口(AUDDECx_),各模块分别包含在对应的头文件中。
    应用程序必须使用CE的核心引擎的三个相关模块去打开和关闭编解码引擎的实例。 需要注意的是引擎的句柄是非线程保护的，对单独使用CE的每个线程来说，必须执行Engine_open并管理好自己的引擎句柄，而对多线程应用来说，也可以顺序的访问一个共享的引擎实例，我们目前采用的就是后者，只定义了一个引擎句柄，多个解码器共用。编解码引擎同时还提供相关的API用以访问系统的内存使用状况和CPU的负载信息，接口如下：
    >Engine_open:         打开一个编解码引擎;
    >Engine_close:        关闭一个编解码引擎,通常是在删除算法实例后调用之来释放相关资源;
    >Engine_getCpuLoad:   获取CPU的使用百分比;
    >Engine_getLastError: 获取最后一个失败操作所引发的错误代码;
    >Engine_getUsedMem:   获取内存使用状况
具体定义引擎所需要包含的头文件和如何定义和使用引擎可参考工程实例example_dsp1. 目前我们都是多个解码器共用一个引擎句柄，比如:
    static String engineName   = "videodec"; /*定义引擎名字，ceapp.cfg配置文件中会用到*/
    Engine_Handle ceHandle_264 = NULL; /*创建一个264解码器引擎句柄*/
    Engine_Error errorcode; /*用于返回引擎打开的状况信息，不同返回值代表的意义可参考相应头文件*/
    ceHandle_264 = Engine_open(engineName, NULL, &errorcode);
根据上述理解，我觉得如果多个线程需要单独使用自己的引擎时，应该可以定义多个引擎名字，创建多个引擎句柄，此时每个线程必须单独执行Engine_open(),并管理好自己的引擎句柄。
**b. VISA API**    @创建一个算法实例: *_create()
编解码引擎ceHandle_264创建完毕后，可以通过它来创建自己的算法实例,需要调用*_create(),其中*可以是VIDEO或AUDIO的相应编解码模块的名字，例如:
    static String decoderName = "h264dec"; /*定义解码模块名字，用于标识算法名字，ceapp.cfg会用到*/
    VIDDEC_Handle 264Handle;               /*创建解码器句柄*/
    264Handle = VIDDEC_create(ceHandle_264, decoderName, NULL); /*在引擎上分配和初始化解码器，第三个参数可以用来初始化算法的相关参数，这些参数控制着算法的各种行为，参数结构依VISA中编码或解码器的不同而不同，具体结构内容可参考头文件*/
    @关闭一个算法实例: *_delete()
    VIDDEC_delete(264Handle); /*注意:只有当与算法相关的内存片清除后，才可以调用之删除算法实例*/
    @控制一个算法实例: *_control()
VIDDEC_control(264Handle, XDM_SETPARAME, dynamicParamsPtr, &encStatus);
    第一个参数是已经打开的算法实例句柄; 第二个参数是一整型的command id,它定义在xdm.h中; 第三个参数是需要动态改变算法的参数，比如在create中第三个参数已经为解码器初始化了一些参数，在这里可以对之做修改，但修改有条件，其具体结构内容可以参考头文件; 第四个参数是一个结构体变量，不同模块具有不同的结构，具体参考头文件。
    @通过算法实例处理数据: *_process()
    status = VIDDEC_process(264Handle &inBufDesc,&outBufDesc, &inArgs, &outArgs);
    第二和第三个参数是XDM_BufDesc类型的结构体，其中包含了内存片段的数目和开始地址以及长度信息；第四第五个参数分别为算法实例提供输入和输出地址。
    上述所有结构体都可以在: \opt\dvevm_#_#\xdais_#_#\packages\xdais\dm下面找到，并可以做修改。只是现在还不知道这些结构体具体怎么使用。
**c. 编译"一个"编解码引擎----引擎配置文件(ceapp.cfg)**    引擎的配置文件是以*.cfg文件形式存储的，目前我们工程里面含两个*.cfg: app里面含ceapp.cfg，里面包含对引擎的配置，还有一个是video_copy.cfg，在server下，是对服务器的配置文件之一，后面会讲到。ceapp.cfg通过Makefile文件使用package.xdc来产生一个*.c文件和一个链接命令脚本文件。一个引擎配置文件包含如下内容: 引擎的名字以及包含在引擎内的编解码器和它们的名字。从这里可以看出，前面定义"h264dec"等名字的作用，用于应用程序中标识算法类别，也可以看出一个引擎是可以由几个编解码器模块共用的。我们以ceapp.cfg文件的内容为例说明配置参数的含义：
    /*--------------set up OSAL----------------*/
    var osalGlobal = xdc.useModule('ti.sdo.ce.osal.Global');
    osalGlobal.runtimeEnv = osalGlobal.DSPLINK_LINUX;
注：这两句是设置全局的模块使配置脚本生效，然后是设置引擎的运行环境，即需要用的DSP/BIOS Link和Linux OS).
    /*--------------get codec modules;i.e.implementation of codecs-------*/
var H264DEC = xdc.useModule('codecs.h264dec.H264DEC');
    注：设置需要用到的编解码器，即我们将要用到给定目录下的H264DEC。注意我们目前使用的都是codec目录下ti提供的videnc_copy，实际上我们可以修改的，另前面我们定义解码器的名字时用的小写的'h264dec',这里配置改成大写.
    /*---------------Engine Cofiguation---------------*/
    var Engine = xdc.useModule('ti.sdo.ce.Engine');
    var demoEngine = Engine.create("videodec", [
    {name: "h264dec", mod: H264DEC, local: false},
    /* {name: "h264enc", mod: H264ENC, local: false} ... 如果有的话*/
]);
    注：首先使ti.sdo.ce目录下的引擎可用，然后用create()创建一个引擎。每个引擎都有一个名字，这个名字会被开发人员用到(如打开引擎的时候,前面我们定义的引擎名字是"h264dec")。create()的参数是一关于算法描述的数组，每个算法描述包含下面几个字段:
    >name: 算法实例的名字，在标识算法时要用到，VIDEC_creat()等VISA API的参数，如前面定义264解码器名字"h264dec";
    >mod: 用来标识实际的算法实现模块，通常就是name的大写，如H264DEC.
    >local: 如果为真，算法实例在ARM端实现，否则通过codec server来创建DSP端的算法实例。
    demoEngine.server = "./encodeCombo.x64P";
    注：用于指明Codec Server.
-----------------------------------------------------------------------------------------------------------------------------------
两个cfg的比较:
    从ARM与DSP的通讯调用机制可看出，ARM端客户应用程序指令通过CE打包由OSAL层经DSPLINK发送到远端DSP服务器，在服务器端，先有server骨架经OSAL层解包获得指令及特定codec的申请，DSP codec处理完毕再经server打包经OSAL层由DSPLINK返回到本地ARM端CE，由此，两个cfg配置文件都需要设置OSAL及通讯环境DSPLINK，并申明好所使用到的各种编解码器，被申明的各编码器或各解码器(通常编码器s与解码器s分别放在两个文件夹下)，两cfg文件中被声明的编码器或解码器路径是一致的，如:
 var VIDDEC_COPY = xdc.useModule('codecs.viddec_copy.VIDDEC_COPY');(.表示目录路径, viddec_copy目录下有个VIDDEC_COPY.xs文件)
-
-
**-另一个人的介绍：**
编解码服务器(CS)就是一个二进制文件，它集成了编解码器，框架组件和一些系统代码，当CS运行在DSP上时，它使用了DSP/BIOS作为其内核。CS同时包括了对客户请求的相关DSP/BIOS线程。CS可以代表实际的DSP硬件、导入到DSP上的镜像文件以及正在运行的任务，其配置需要两个步骤：
 >通过TCF脚本语言配置DSP/BIOS；
   >通过XDC配置剩下的组件，比如:FC组件、DSP/BIOS Link、Codec Engine等。
   配置完成的服务器镜像文件是在引擎配置文件(ceapp.cfg)中使用的，如前所述的demoEngine.server = "./encodeCombo.x64P";
a. 编译一个编解码服务器
       CS镜像文件的创建过程是通过前面介绍的XDC工具来完成的，所不同的是，CS在创建时需要一个main.c和相关的BIOS配置脚本.tcf文件. 
>tcf: 脚本文件主要是对DSP/BIOS内核进行配置，如: 定义DSP的内存映射，设置DSP的中断向量表，创建和初始化其他DSP/BIOS数据对象等，具体可参见video_copy.tcf,注意我在里面添了一个关于trace的参数配置是原来没有的。
>main.c: 只要你的算法实现了XDM接口，就需要一个main.c的程序去初始化CE，然后用其他配置脚本来创建一个服务器镜像*.x64P. 在main.c里面除了调用CERuntime_init()初始化CE外，就是对于trace相关函数的初始化和处理，这部分暂时没有细究。另有一点值得注意，在这里可以实现对cache的重新配置，因为在tcf文件里面对cache的配置可能会不起作用，这是可以在这里以函数代码的方式来配，这点以前没有注意过。这里有点不明白的是，在ceapp_init()中已经做了一次CERuntime_init()，为何在CS处还要做一次?(我觉得是因为先编译CS，生成*.x64P，然后才编译app端，这样就可以理解成:
 只要你的算法实现了XDM接口，就需要对CE做初始化CERuntime_init()，而CS的编译是调用了经XDM封装后的codec生成的*.a64P的)
>XDC相关文件:
>>>package.xdc
    /*--------------声明包名-----------------*/
    package server{}   
    (我们目前的.xdc是: package server.video.copy, 此即\server\video_copy\..xdc,应该无需改动)
>>>package.bld: 声明所必须包含的包，链接命令脚本，tcf文件和一些源文件，定义编译属性、平台和对象等。
>>>server.cfg: 这是CS配置的重点，说明如下。
/*第一部分: 声明运行环境和各种编解码模块, 与CE.cfg类似*/
/*--------------set up OSAL----------------*/
var osalGlobal = xdc.useModule('ti.sdo.ce.osal.Global');
osalGlobal.runtimeEnv = osalGlobal.DSPLINK_BIOS;
注：这两句是设置全局的模块使配置脚本生效，然后是设置引擎的运行环境，即需要用的DSP/BIOS Link，与CE.cfg有点区别).
/*---------------server Cofiguation---------------*/
var Server = xdc.useModule('tisdo.ce.Server');
Server.threadAttrs.stackSize = 16384;
Server.threadAttrs.priority = Server.MINPRI;
/*--------------get codec modules;i.e.implementation of codecs-------*/
var H264DEC = xdc.useModule('codecs.h264dec.H264DEC'); //与CE.cfg同，注意H264DEC标识的使用。
Server.algs = [
{name:"h264dec", mod:H264DEC,threadAttrs:{stackSize:4096,stackMemId:0,priority:Server.MINPRI+1} }, {...if have..},
   ];
/*第二部分: DSKT2 and DMAN3的配置：XDAIS算法内存和DMA的分配，参考配置文件*/
