# C#调用matlab函数详细步骤和例程 - fighting！！！ - CSDN博客
2017年11月03日 22:18:16[dujiahei](https://me.csdn.net/dujiahei)阅读数：289
                
C#调用matlab函数详细步骤和例程 - CSDN博客
http://blog.csdn.net/zhangqiagn1104/article/details/45673775
 在项目过程中需要在ASP.NET页面显示三维效果的曲面图，因C#中没有相关的控件（暂时没找到），需要调用其他软件实现此功能，实现的方法有多种，这里只介绍在C#中调用matlab函数，实现绘制三维图的功能。当然，matlab还有强大的数据处理能力，如果在C#中涉及到处理大量数据，也可以通过该方法调用matlab函数。
       系统平台：matlab2013a（破解的2013b未破解完全，谨慎使用）
                           VS2010
       注意：将来使用调用matlab的C#程序时候，可以不安装matlab程序，但必须安装matlab的运行环境，即MCRinstall文件，这个在matlab安装包里面一般可以找到。
       操作步骤：1.在matlab的command窗口输入mbuild（留空格）-setup，选择编译器，这里要选择我们打算调用matlab的软件，即VS2010；然后在command窗口输入deploytool命令，同样选择编译环境为VS2010。之后，会弹出下图1所示界面。注意：这里的Name就是C#里要调用的dll文件名，Location就是查找dll文件的位置，Type要选择.NET Asembly，点击OK按钮。
![](https://img-blog.csdn.net/20150512222133088?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdxaWFnbjExMDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图1
                        2.出现下图所示的页面，点击Add Class添加类，这里注意修改默认的Class类名，Class类名就是我们在C#中要使用的类名称，自己要记下来。修改完类名点击Add file添加我们要使用的m文件，如图3所示，这里m文件的名称就是我们在C#中要使用的类的成员函数，也要把这个名称记下来。之后，点击图3右上方的编译按钮，就会出现图4的页面，等待出现build finished即可（时间会有点长，要5-10分钟）。这样，我们在MATlab环境下的操作就完成了，请摄像机把镜头切换到VS2010环境中~~~
![](https://img-blog.csdn.net/20150512222546435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdxaWFnbjExMDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图2
![](https://img-blog.csdn.net/20150512222618587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdxaWFnbjExMDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图3
![](https://img-blog.csdn.net/20150512222449773?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdxaWFnbjExMDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图4
                               3.打开C#程序的“设计”页面，在“解决方案资源管理器”里添加“引用”，打开图5所示的界面，这里至少要添加两个dll文件，一个是MWArray.dll（文件目录为D:\MATLAB\R2011b_x64\toolbox\dotnetbuilder\bin\win64\v2.0），另一个是我们自己生成的dll文件（目录在图1Location中指定的）。
![](https://img-blog.csdn.net/20150512222742452?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemhhbmdxaWFnbjExMDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
图5
                           4.在ASP.NET的***.aspx.cs页面下，要注意添加dll文件的至少2个命名空间，一个是using MathWorks.MATLAB.NET.Arrays;另一个是我们自己生成的dll文件的命名空间，如using TuShowOut;
                           一个简单的例程：建议先使用一个最简单的例程测试一下我们前面所设置的编译环境是否正确，不要直接调用项目中用到的复杂的matlab函数。
                           1.matlab程序如下：
[plain][view
 plain](http://blog.csdn.net/zhangqiagn1104/article/details/45673775#)[copy](http://blog.csdn.net/zhangqiagn1104/article/details/45673775#)
- function y=raicos(t)  
- result=2t;  
- end  
                2.在C#中调用matlab的函数，使用上面的4个步骤，添加引用之后，C#程序如下：
[csharp][view
 plain](http://blog.csdn.net/zhangqiagn1104/article/details/45673775#)[copy](http://blog.csdn.net/zhangqiagn1104/article/details/45673775#)
- using System;  
- using System.Collections.Generic;  
- using System.Linq;  
- using System.Web;  
- using System.Web.UI;  
- using System.Web.UI.WebControls;  
- using MathWorks.MATLAB.NET.Arrays;//系统dll文件
- using Demodll;//自己生成的dll文件
- namespace WebApplication1  
- {  
- public partial class WebForm26 : System.Web.UI.Page  
-     {  
- protectedvoid Page_Load(object sender, EventArgs e)  
-         {  
-             demo h = new demo();//demo为class名称，h为定义的成员函数
-             MWArray x;//定义x为矩阵变量，将普通数转化为matlab能识别的类型
-             x = demo.raicos((MWArray)i);//调用matlab里的raicos函数
-         }  
-     }  
- }  
- 
