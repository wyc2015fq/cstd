# C#与MATLAB混合编程 - fighting！！！ - CSDN博客
2017年11月03日 22:16:33[dujiahei](https://me.csdn.net/dujiahei)阅读数：1693
                
原地址：C#与MATLAB混合编程 - SteveWang - 博客园
http://www.cnblogs.com/eniac12/p/4390845.html
C#和MatLab的混合编程，充分利用了winform的直观显示和matlab的强大计算能力。在此以一个小例子的形式给大家讲述一下二者混合编程的实现。
**　　一、软件的配置说明**
C#版本：VS2010        matlab版本：matlab2013a
**　　二、把 matlab函数编译成dll文件　**
**　　2.1首先写一个函数的.m文件（如下函数MatrixOpera实现两个矩阵的相加和相减）**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
%--------------------------------------------------------
function [addRlt,minusRlt]=MatrixOpera(a,b)
% 矩阵相加,相减
[m1,n1]=size(a);
[m2,n2]=size(b);
if m1~=m2 || n1~=n2
    display('矩阵大小不相同')
    error('参数错误');
end
addRlt=a+b;
minusRlt=a-b;
end
%-----------------------------------------------------------
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
** 　　2.2 创建制作的dll文件的工程**
在MatLab的命令窗口中，输入deploytool打开创建dll文件的工程创建窗口，如下图所示
![](http://images.cnitblog.com/blog2015/739525/201504/031848422645797.png)
　　在图中，主要分为三部分，Name：你想要命的工程名，prj是工程的后缀；Location：创建工程的储存位置；Type：针对c#而言，我们需要选择.NET Assembly这一项
　　点击Ok选项，工程已经创建完毕
**　　2.3 向所建工程中添加对应的元素并编译**
上述过程中，新建了一个叫matPrj的工程（它相当于c#中的命名空间namespace）；然后给它增加class（它就是c#中的类），点[add class]添加myMathClass；之后再给它添加方法（它就是函数了），点[add files]，本实例中选中之前编写的MatrixOpera函数。完了之后就可以编译了，编译需要一两分钟的时间，如下图所示
![](http://images.cnitblog.com/blog2015/739525/201504/031904142956791.png)
**　　三、C#中添加matlab的dll引用**
**　　3.1打开vs2010，新建一个窗体应用程序**
**![](http://images.cnitblog.com/blog2015/739525/201504/031910487647410.png)**
**　3.2完成dll文件的拷贝**
　　将matlab工程(matPrj)src文件夹下的matPrj.dll和D:\Program Files\MATLAB\R2013a\toolbox\dotnetbuilder\bin\win64\v2.0(前边为matlab的安装路径)文件夹下的MWArray.dll拷贝到VS工程(CallMatlabDllApp)下的\bin\Debug文件夹下
![](http://images.cnitblog.com/blog2015/739525/201504/031920452486182.png)
**3.3在VS里添加dll文件的引用，然后在代码前引用相应的命名空间**
**![](http://images.cnitblog.com/blog2015/739525/201504/031926377019505.png)**
**![](http://images.cnitblog.com/blog2015/739525/201504/031928016076071.png)**
　　如果matlab函数复杂，还需要用到其他的空间，则视情况而定，自己凭经验添加。
　　至此，已经可以利用c#调用我们用matlab编写的函数了。
**　　四、在C#中调用matlab提供的函数**
matlab中一个m*n的矩阵对应.net中的一个1*（m*n）的数组，因此在C#中调用matlab函数的关键是掌握数据类型一致的一个矩阵与一个数组之间的转换。包括以下两个问题：
　　a、将C#中的参数输入到matlab函数时，要将参数转化为matlab的参数形式，通常是MWArray类型
      b、matlab函数返回的参数，也要转化为C#用的类型，比如数组或者数值类型
　　在C#中调用本例函数的代码如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
            matPrj.myMathClass test = new matPrj.myMathClass(); //将myMathClass类实例化
            double[] a = { 1, 2, 3, 4, 5, 6 };//定义两个输入参数
            double[] b = { 1, 1, 1, 1, 1, 1 };//它们是两个一维静态数组
            double[,] c = new double[3, 2];//定义C#中接收输出参数的类型
            double[,] d = new double[3, 2];//是两个二维数组
            //把两个输入参数都转换成中间类型，中间类型也是矩阵所以要指明维数
            //这里将两个输入参数转换为两个三行两列的矩阵
            MWNumericArray matlab_a = new MWNumericArray(3, 2, a);
            MWNumericArray matlab_b = new MWNumericArray(3, 2, b);
            //输入参数成功转化为两个MWArray元素类型
            MWArray[] agrsIn = new MWArray[] { matlab_a, matlab_b };
            //声明输出参数是两个MWArray元素类型，一定要写数量
            MWArray[] agrsOut = new MWArray[2];
            //调用matlab函数，2表示输出参数的个数，输出参数前需要加 ref 关键字
            //此例实现了两个三行两列的矩阵相加减
            test.MatrixOpera(2, ref agrsOut, agrsIn);
            //把两个输出参数转换成中间类型
            MWNumericArray net_c = agrsOut[0] as MWNumericArray;//matlab函数第一个输出参数
            MWNumericArray net_d = agrsOut[1] as MWNumericArray;//第二个输出参数
　　　　　　　//转换成C#中的接收参数
            c = (double[,])net_c.ToArray();//转化为二维数组
            d = (double[,])net_d.ToArray();
            //一定要注意最后接收参数的转化，不同类型的接收参数用的转换函数不同
            //二维数组用ToArray()函数转换
            //一维数组用ToVector(MWArrayComponent.Real)函数转换
            //单个double值用ToScalarDouble()函数转换
　　　　　　　//单个int值用ToScalarInteger()函数转换
            for (int i = 0 ; i <= 2; i++)//输出结果验证
            {
                for (int j = 0; j <= 1; j++)
                {
                    textBox1.Text += c[i, j].ToString() + " ";
                    textBox2.Text += d[i, j].ToString() + " ";
                }
                textBox1.Text += "\r\n";//换行
                textBox2.Text += "\r\n";
            }            
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　MWNumericArray是MWArray与C#等语言数据转换中间类，实现C#与matlab函数间传递参数的功能
　　MWArray是总类型，其它的以MW开头，以Array结尾的变量类型都可以直接对它进行赋值或取值，它是.m文件的编译后内部的标准类型，一切C#类型都要最终转换成此类型，方可作为参数调用matlab函数
　　我在运行上述程序时，第一句将myMathClass类实例化报错
　　但是单看这语句是没有任何问题的
　　matPrj.myMathClass test = new matPrj.myMathClass();
　　查阅了关于错误的信息，但是解决的都基本是换了一个matlab版本才解决的，偶然看见说HRESULT:0x8007000B的错误和编译平台有些关系，就开始着手试试是不是因为我这平台的问题导致的，果然，换了编译的平台就好了。
　　具体解决：
　　1：右键项目名称—>属性—>生成 
　　2：目标平台选择x64（因为我的matlab2013a是64位的）
![](http://images.cnitblog.com/blog2015/739525/201504/032058367791030.png)
再次运行程序，结果如下：
![](http://images.cnitblog.com/blog2015/739525/201504/032103396546132.png)
上述结果同下图在matlab中实现效果相同，验证结果的正确性
![](http://images.cnitblog.com/blog2015/739525/201504/032115533423086.png)
　　由于我的项目中其他引用是32位软件生成的，为保重整个系统的各个模块能同时工作，我重下了个32位的matlab2012a重新生成dll文件实现同样的功能，C#项目程序不变，目标平台改回x86（同时因为通常情况下，为保证我们的程序既能在64位机子上运行，也能在32位机子上运行，我们的目标平台也会选择x86），重复上述步骤进行操作，发现最后运行程序时再一次出现与之前相同的错误
![](http://images.cnitblog.com/blog2015/739525/201504/032053026235050.png)
　　很明显还是matlab版本的问题，但我的C#项目的目标平台已经是x86，上网查找各种资料，有的说是因为matlab中被编译的.m文件和编译生成的工程所在路径上有中文，改之，问题未解决；有的说要重现装一下MCR(MATLAB Compiler Runtime)，装之，问题未解决；有人接着说装完MCR后还要把他的路径增加到环境变量path中，加之，问题未解决（后来发现这些做法根本就是多此一举，系统安装matlab时就已经安装了MCR，而MCR在安装成功后，路径会被系统自动添加到环境变量中）。加上反反复复的安装和卸载matlab共花费了大约2~3天的时间，问题仍然没解决，正当我对人生充满疑惑时，一次偶然的机会（直接运行打包后的程序），我看到了错误的信息是matlab引用与配置文件信息（2.13.0.0）不相符，我马上找到了工程下的配置文件app.config，打开后果然发现里边的配置信息有2.13.0.0的字眼，这很明显是我最开始使用matlab2013a与C#混编时自动写入的，而现在我换成了matlab2012a，与配置文件中的版本信息不相符，所以报错，找到了问题所在，就容易解决了：把app.config中matlab版本信息2.13.0.0改成2.12.0.0，问题解决！
**　　脱离matlab安装的环境下，如何实现程序的运行自如： **
　　通过com组建的形式进行混合编程，如果机器上没有安装MATLAB，编译生成的软件照样可以使用，但是需要安装MATLAB提供的MATLAB Compiler Runtime(MCR)，这个软件在MATLAB的安装目录下：X:\MATLAB\R2013a\toolbox\compiler\deploy\win32\MCRInstaller.exe（X是MATLAB安装的盘符），如果你的电脑上安装了MATLAB，则不需要在重新安装这个软件。另外，不同的MATLAB版本提供的MATLAB Compiler Runtime可能会不同，如果在另一台电脑上自己编写的软件运行有问题，可能是编译器的版本不同引起的
参考资料：http://wenku.baidu.com/link?url=mgBBgQFt89s-7rKrNbJ-630AH0q_2mMMYBbvCWx9IiGkuOQ7qFTkL_pZRhT5FqazmtFoD6UUM6vWLeDrprF2OAGqUcmrHEugVntjWvzCwxy
　　　　　http://www.cnblogs.com/liuxiaowei0543/p/3682377.html
　　　　　http://wenku.baidu.com/view/6ad3b62b59eef8c75fbfb3ed.html
