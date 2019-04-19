# opencv blobtrack 大致算法思想 - 三少GG - CSDN博客
2010年12月07日 17:16:00[三少GG](https://me.csdn.net/scut1135)阅读数：4170
程序分为几个模块：
1.前景检测模块（CvFGDetector虚类）：实现前景检测，
2.团块检测模块（CvBlobDetector虚类）：实现运动物体（团块）的的检测
3.团块跟踪模块（CvBlobTracker虚类）：实现运动物体跟踪
4.团块运动轨迹产生模块（CvBlobTrackGen虚类）：实现的功能与模块名字同（下同）
5.团块轨迹后处理模块（CvBlobTrackPostProc虚类）
6.团块轨迹分析模块（CvBlobTrackAnalysis虚类）
7.以及处理流程模块（cvBlobTrackerAuto虚类)：此模块可看成胶水，集成上面的模块。
除了处理流程模块（因为它只控制流程呀~），每一个模块可以用多种算法实现，在程序中，这些算法就是函数。如（15-16行）：
CvFGDetector* cvCreateFGDetector0() {...}
CvFGDetector* cvCreateFGDetector0Simple(){...}
CvFGDetector* cvCreateFGDetector1(){...}
也就是说这三个函数都是能完成前景检测，具体用哪个，由你在main()函数中调用。
其他模块也是这样。
实现这些模块的类以及完成这些算法的函数都由OPENCV帮你实现了，blobtrack要做的就是在main()函数中调用这些函数，初始化函数参数以及各种变量。==========================================================================================[http://hi.baidu.com/reedinheart/blog/item/8aeb51ed4091b83dacafd589.html](http://hi.baidu.com/reedinheart/blog/item/8aeb51ed4091b83dacafd589.html)
在空工程中加入opencv中blobtrack.cpp编译遇到的错误(转）
2010-03-11 11:20
1．在空工程中加入opencv中blobtrack.cpp编译遇到的错误
fatal error C1010: unexpected end of file while looking for precompiled header directive
Error executing cl.exe.
blobtrack.obj - 1 error(s), 0 warning(s)
预编译头文件(precompiled header)
解决方法：
1、如果发生错误的文件是由其他的C代码文件添加进入当前工程而引起的，则Alt+F7进入当前工程的 Settings，选择C/C++选项卡，从Category组合框中选中Precompiled Headers，选择Not Using Precompiled headers。确定。
2、在文件开头添加:
#include "stdafx.h"
对预编译头文件说明如下：
所谓头文件预编译，就是把一个工程(Project)中使用的一些MFC标准头文件(如Windows.H、Afxwin.H)预先编译，以后该工程编译时，不再编译这部分头文件，仅仅使用预编译的结果。这样可以加快编译速度，节省时间。
预编译头文件通过编译stdafx.cpp生成，以工程名命名，由于预编译的头文件的后缀是“pch”，所以编译结果文件是projectname.pch。
编译器通过一个头文件stdafx.h来使用预编译头文件。stdafx.h这个头文件名是可以在project的编译设置里指定的。编译器认为，所有在指令#include   "stdafx.h"前的代码都是预编译的，它跳过#include   "stdafx.   h"指令，使用projectname.pch编译这条指令之后的所有因此，所有的CPP实现文件第一条语句都是：#include   "stdafx.h"。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
unexpected end of file while looking for precompiled header directive Error executing cl.exe.我在文件头上添加了：
#include "stdafx.h" 还是不能运行，请多指教！
Alt+F7进入当前工程的Settings，选择C/C++选项卡，从Category组合框中选中Precompiled Headers，选择Not Using Precompiled headers。确定。
如果发生错误的文件原本是该工程中的，则检查该文件头部有没有＃i nclude "stdafx.h"语句，没有的话添加。
如果还不行，也有可能是定义的类或结构体等最后忘了加分号，注意一下
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
问题：我在编译程序中老出现“fatal error C1010: unexpected end of file while looking for precompiled header directive”这一句，但我查看了程序并没有错，请问这是怎么一回事？
Ａ回答:
肯定是一个新添加的类的.cpp文件开头没包含stdafx.h，在该文件最前面加上即可。
BOBO的意见：
有时可以使用右键点击项目工程中的该cpp文件，选择setting，在c/c++栏，选择PreCompiled headers，然后设置第一选项，选择不使用预编译头，解决这个问题。
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
编译出现问题：unexpected end of file while looking for precompiled header directive
解决方案：
需要在文件头上添加一句：
#include "stdafx.h"
这个文件定义了源程序为C++格式。
否则文件需要保存为.C格式
－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
错误:fatal error C1083: Cannot open precompiled header file: 'Debug/test4.pch': No such file or directory
解决方案：
或rebuild all，或重新编译一下stdafx.cpp就OK了，呵呵，又长知识了。
－－－－－－－－－－－－－－－－－－－－
代码。
－－－－－－－－－－－－－－－－总结－－－－－－
如果工程很大，头文件很多，而有几个头文件又是经常要用的，那么
1。把这些头文件全部写到一个头文件里面去，比如写到preh.h
2。写一个preh.c，里面只一句话：#include "preh.h"
3。对于preh.c，在project setting里面设置creat precompiled headers，对于其他.c文件，设置use precompiled header file
//
我试了一下，效果很明显，不用precompiled header，编译一次我可以去上个厕所，用 precompiled header，编译的时候，我可以站起来伸个懒腰，活动活动就差不多啦
====================================================================================
**OpenCV中blobtrack例程的set_params()函数陷阱 **
**【本文在本人赛迪博客上同期刊出：**[http://itrock.blog.ccidnet.com/blog-htm-do-showone-uid-255426-type-blog-itemid-1398414.html](http://itrock.blog.ccidnet.com/blog-htm-do-showone-uid-255426-type-blog-itemid-1398414.html)**转载请注明出处】**
    近两周一直在研读OpenCV中的光团跟踪例程blobtrack例程。在分析程序代码时，我发现其中有个子函数set_params()的核心代码不会被有效调用，似乎这个函数是冗余的。由于资料的匮乏及该例程中帮助文档不够详尽，以致于让我觉得自己发现了一处OpenCV的Bug。但今晚在调试该程序时，闪念间明白了这个函数的含义，发现自己之前误解了该函数。
///////////////// OpenCV的blobtrack例程中的set_params()函数代码/////////////////////
static void set_params(int argc, char* argv[], CvVSModule* pM, char* prefix, char* module)
{
    int prefix_len = strlen(prefix);
    int i;
    for(i=0;i<argc;++i)
    {
        int j;
        char* ptr_eq = NULL;
        int   cmd_param_len=0;
        char* cmd = argv[i];
        char* cmd2=NULL
        if(strnicmp(prefix,cmd,prefix_len)!=0) continue;
        cmd += prefix_len;        
        if(cmd[0]!=':')continue;  
        cmd++;
        ptr_eq = strchr(cmd,'=');
        if(ptr_eq)cmd_param_len = ptr_eq-cmd;
        for(j=0;;++j)
        {
            int     param_len;
            char*   param = pM->GetParamName(j);
            if(param==NULL) break;
            param_len = strlen(param);
            if(cmd_param_len!=param_len) continue;
            if(strnicmp(param,cmd,param_len)!=0) continue;
            cmd+=param_len;
            if(cmd[0]!='=')continue;
            cmd++;
            pM->SetParamStr(param,cmd);
            printf("%s:%s param set to %g/n",module,param,pM->GetParam(param));
        }
    }
    pM->ParamUpdate();
}/* set_params */
/////////////////////////////////////////////////////////////////////////////////////
    因为该例程中的参数传递中没有用到":"的地方，所以按照这种根本不会包含":"的命令行参数传递规则，语句“ if(cmd[0]!=':')continue; ”之后的内容根本就不可能有机会执行。显然，设置参数的核心语句“ pM->SetParamStr(param,cmd); ”也就不会被执行。这让我，同样让好多曾今试探深刻理解该程序的人们困惑不已。下面摘录的是例程的命令行参数传递规则：
blobtrack.exe [fg=<fg_name>] [bd=<bd_name>]
              [bt=<bt_name>] [btpp=<btpp_name>]
              [bt_corr=<bt_corr_way>] 
              [trackgen=<btgen_name>] 
              [track=<track_file_name>] 
              [scale=<scale val>] 
              [noise=<noise_name>] [IVar=<IVar_name>]
              [res=<res_file_name>] [FGTrainFrames=<FGTrainFrames>]
              <yml_file video1 video2 video3>|<avi_file>
    事实上，不知道是由于OpenCV官方为了简化说明文档，还是由于疏忽的原因，对于命令行的参数传递规则，他们省略了下面我要叙说的部分，正是这部分的缺失导致我产生了之前的误解。
    blobtrack的各模块CvFGDetector，CvBlobDetector，CvBlobTracker，CvBlobTrackGen，CvBlobTrackPostProc，CvBlobTrackAnalysis中有许多内部参数可以初始化(否则将用缺省值)。通过命令行参数初始化这些模块的参数的格式分别为：
     [fg:<参数名1>=<参数值1>]    [fg:<参数名2>=<参数值2>]
     [bd:<参数名1>=<参数值1>]   [bd:<参数名2>=<参数值2>]
     [bt:<参数名1>=<参数值1>]   [bt:<参数名2>=<参数值2>]
     [btgen:<参数名1>=<参数值1>]   [btgen:<参数名2>=<参数值2>]
     [btpp:<参数名1>=<参数值1>]    [btpp:<参数名2>=<参数值2>] 
     [bta:<参数名1>=<参数值1>]    [bta:<参数名2>=<参数值2>]
    当以这种方式从命令行输入参数时，set_params()函数便会把参数值赋值到相应模块的相应参数。例如CvFGDetector中有个参数是char * alpha1,设需要处理的avi文件位于D:/test.avi，则我们在命令行键入如下命令:
     blobtrack.exe  bt=test_bt.avi  D:/test.avi  fg:alpha1=3.14
     则运行程序blobtrack.exe，对D盘的test.avi视频进行运动跟踪处理，并将CvFGDetector模块的内部参数alpha1用3.14来初始化。实际上，各个模块的内部参数很多，具体的可以通过语句“ char*  param = pM->GetParamName(j); ”来查询。
     为了验证如上我所说的，我们可以在set_params()处设置断点，单步调试程序来查看相关变量。 
=========================================================================================
**针对在复杂背景中检测出多批特定运动目标并实施分配批号实行标记跟踪，本文利用OpenCV的运动物体跟踪的数据结构、函数以及基本框架，建立了一个由人机交互界面模块；运动物体的前景检测模块；运动物体的团块特征检测模块；运动物体的团块跟踪模块轨迹生成模块；轨迹后处理模块组成的视频图像运动目标分析系统。**
**====================================================**
**====================================================**
**[http://techbyaurora.blogbus.com/logs/56474811.html](http://techbyaurora.blogbus.com/logs/56474811.html)**
**OpenCV(10_1)Blobtrack**
**终于跑通了blobtrack。用opencv2.0，vc++express2008. 之前几天一直啊报错，camshift又不太适合现在的状况。周二就是死线，压力大死了。**
**报错呀，一直都是**
**error LNK2001: unresolved external symbol "class CvBlobTrackAnalysis * __cdecl cvCreateModuleBlobTrackAnalysisIOR(void)" (?cvCreateModuleBlobTrackAnalysisIOR@@YAPAVCvBlobTrackAnalysis@@XZ)**
**这一类的。**
**一直看了好多贴，都解决不了。然后终于在2：27分，发现是依赖库的问题啊。改了依赖库**
**debug： cxcore200d.lib cv200d.lib highgui200d.lib cvaux200d.lib ml200d.lib**
**release：cxcore200.lib cv200.lib highgui200.lib cvaux200.lib ml200.lib**
**就成功了～好想夸耀一下，真是，出得厅堂，下得厨房，会写代码，能查异常，杀得木马，还会翻墙。。。嘿。那些什么重装1.0，load dll的，都是屁话，屁话，屁话！去死去死。**
**现在的状况是小熊们聚在一起玩的时候，track就t到一起去了，分开的时候会都粘在一只小熊的身上呀。还有熊妈妈的名字要怎么加上去呢？还要继续调参数跟模式。周一要六点起床去动物园拜访熊一家，还要见到熊妈妈本人！**
**先贴代码吧。**
**#include"cvaux.h"**
**#include"highgui.h"**
**#include<stdio.h>**
**/* Select appropriate case insensitive string comparison function: */**
**#ifdefined WIN32 || defined _MSC_VER**
**#define MY_STRNICMP strnicmp**
**#define MY_STRICMP stricmp**
**#else**
**#define MY_STRNICMP strncasecmp**
**#define MY_STRICMP strcasecmp**
**#endif**
**/* List of foreground (FG) DETECTION modules: */**
**static CvFGDetector* cvCreateFGDetector0() { returncvCreateFGDetectorBase(CV_BG_MODEL_FGD,NULL); }**
**以下略了。。。在/Program Files/OpenCV2.0/samples/c/blobtrack.cpp 呀。。。**
**改了之后再说吧～**
