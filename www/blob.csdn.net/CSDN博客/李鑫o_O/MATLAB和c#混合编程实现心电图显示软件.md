# MATLAB和c#混合编程实现心电图显示软件 - 李鑫o_O - CSDN博客





置顶2016年03月10日 20:32:54[hustlx](https://me.csdn.net/HUSTLX)阅读数：2285













由于MATLAB自带的GUI平台设计的界面不是很美观而且设计过程并不是很方便，我们选择了用c#来做软件界面的实现。我们用MATLAB做信号处理封装成函数，把函数编译成dll格式，然后用c#调用MATLAB的函数即可。在设计过程中遇到两个主要的麻烦，一个是MATLAB和c#数值类型的转化问题，而且c#函数多输出、多输入问题是从来没有遇到过的，另一个问题是实现动态的绘制心电曲线，我最后通过一个定时器不断的刷新画图解决了这个问题。下面详细介绍实现过程。

## 1 c#调用matlab函数

matlab版本2013a，c#版本vs2010.

### 1.1  matlab函数编译成dll文件

（1）首先编写一个函数的m文件，如MatrixOpera表示两个矩阵相加和相减。

%--------------------------------------------------------

function [addRlt,minusRlt]=MatrixOpera(a,b)

% 矩阵相加,相减

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

两个输入参数，两个输出参数，并且都是矩阵形式

（2）在matlab 命令窗口输入deploytool，弹出如下窗口

![clip_image002](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104750600-2125427338.jpg)

或者点击matlab的主菜单 desktop→deploy
 tool获得如下面图所示的窗口，然后在file→new→deployment
 project 中点击。获得上图窗口，修改工程名称和文件后缀名（必须是，.Net Assembly格式）

![clip_image004](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104757679-1443352393.jpg)



![clip_image005](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104759757-1264789899.gif)![clip_image007](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104807085-414597941.jpg)








（3）新建了一个叫matPrj的工程（它相当于c#中的命名空间namespace）；然后给它增加class或类（它就是c#中的类），点[add
 class]比如myMathClass；之后再给它添加方法（它就是函数了），点[add files]，如下图所示。本实例中给它添加了MatrixOpera等函数。

![clip_image008](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104808022-1231526022.gif)![clip_image009](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104808600-88790040.gif)![clip_image010](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104813147-982007276.gif)![clip_image010[1]](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104818366-1009067010.gif)![clip_image012](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104819429-1199989512.jpg)

（4）完了之后就可以编译了。编译出来后需要一两分钟的时间。

![clip_image013](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104820116-1163603459.gif)![clip_image014](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104824819-847403214.gif)![clip_image016](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104838132-565206790.jpg)

（5）找到该工程存放的文件夹，从里面拷出matPrj.dll文件。同时还要从G:\Program
 Files\MATLAB\R2009b\toolbox\dotnetbuilder\bin\win32\v2.0拷贝出MWArray.dll和ManagedCPPAPI.netmodule。第二个文件必须要哦，否则可能会出错。

![clip_image018](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104838866-1542478419.jpg)

### 1.2  c#添加matlab的dll引用

（1）打开vs2008，新建一个窗体应用程序。

![clip_image020](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104844038-325791591.jpg)

（2）将刚才拷贝到的那3个文件一起放在vs工程（CallMatlabDllApp）的debug文件夹下面。然后右键下图中的引用，选择添加引用，弹出一个窗口，选择其中的浏览页面，分别添加matPrj.dll文件和MWArray.dll文件。

![clip_image021](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104844554-1919208948.gif)![clip_image022](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104845116-304053970.gif)![clip_image024](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104846038-1405659270.jpg)

![clip_image026](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104847241-724437017.jpg)









（3）最后在前面，代码的前面添加下面的命名空间即可。

using MathWorks.MATLAB.NET.Arrays;//在MWArray.dll，最常用的

using MathWorks.MATLAB.NET.Utility;// 在MWArray.dll，最常用的

using matPrj;//这个就是我们自己定义的，里面有matlab函数

如果matlab函数复杂，还需要用到其他的空间，则视情况而定，自己凭经验添加。

![clip_image027](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104847913-1097307765.gif)![clip_image029](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104850069-224751583.jpg)

至此，已经可以利用c#调用我们用matlab编写的函数了。

### 1.3  函数调用

函数调用前必须注意：

（1）将c#的参数输入到matlab函数时，要将参数转化为matlab的参数形式，通常是MWArray类型。

（2）matlab返回的参数，也要转化为c#用的类型，比如数组或者数值类型。

直接将值传递给已经初始化的MWArray数组中的成员

直接将数据类型赋值给已经初始化的MWNumericArray变量。

直接将字符串类赋值给已经初始化的MWCharArray变量。

如果是数组类型：

直接赋值给MWNumericArray变量；

赋值给MWArray变量，则在前面加上类型转换如：（MWNumericArray）进行强制转换。

总之，MWArray是总类型，其它的以MW开头，以Array结尾的变量类型都可以直接对它进行赋值或取值。

M类型到C++/C#数据类型

MWArray

M类型，它是M文件的编译后内部的标准类型，一切C++/C#类型都要最终转换成此类型，方可作为参数调用M语言函数。

MWCharArray

M的字符串类型，使用它可以将M中的字符类型转换成C++/C#的字符串类型。

MWNumericArray

MWNumericArray是MWArray与C#等语言的转换中间类型。

常用的转换函数：

①　public Array ToArray(MWArrayComponent component);

将M类型转换成C#的Array类型，然后可以直接转换成其它类型的数组。

②　public byte ToScalarByte();

将M类型转换成C#的字节类型；

③　public double ToScalarDouble();

将M类型转换成C#的双精度类型；

double temp = ((MWNumericArray)(mwArgout[0])).ToScalarDouble();

④　public float ToScalarFloat();

将M类型转换成C#的单精度类型；

⑤　public int ToScalarInteger();

将M类型转换成C#的整型类型；

⑥　public long ToScalarLong();

将M类型转换成C#的长整C/C++/C#数据型类型；

⑦　public short ToScalarShort();

将M类型转换成C#的短整型类型；

⑧　 public override string ToString();

将M类型转换成C#的字符串类型；string
 arror = mwArgout[2].ToString();

⑨　 public Array ToVector(MWArrayComponent component);

将M类型转换成C#的Array类型，然后可以直接转换成其它类型的数组。

下面使用调试过的代码示例表述①⑨两个函数的区别：

①

double[,] Temp1 = new double[1,3];          

Temp1= (double[,])((MWNumericArray)mwArgout[1]).ToArray(MWArrayComponent.Real);

⑨

double[] s1 = new double[2];               

s1 = (double[])((MWNumericArray)mwArgout[1]).ToVector(MWArrayComponent.Real);

（3）必须注意到多参数输入和多参数返回的问题。刚开始碰到这个这个问题比较头疼，后来经过不懈的努力，终于从网上找到答案。

声明这部分是我参考别人的想法自己写的额，网上百度知道也有我（lwq123_321）回答的。

//输入这里想传入的2个输入参数，为了支持矩阵好通用，所以得弄成Array

            double[] a = { 1, 2, 3, 4, 5, 6 };//输入参数1

            double[] b = { 2, 4, 6, 8, 10, 12 };//输入参数2

            double[,] c = new double[3, 2];//输出参数1

            double[,] d = new double[3, 2];//输出参数2

//这些参数都是矩阵

            MWNumericArray ma = new MWNumericArray(3, 2, a);//转换成matlab需求的格式

            MWNumericArray mb = new MWNumericArray(3, 2, b);

//输出参数是一个MWArray数组

            MWArray[] agrsOut = new MWArray[2];//两个输出参数，一定要写数量

            //输出几个输出参数可以是不同类型的，比如第一个元素是矩阵，第二个是数值

 //同理，输入参数也是一个MWArray数组

            MWArray[] agrsIn = new MWArray[] { ma,mb};

//调用函数，输出参数需要加 ref 关键字

            myFun.MatrixOpera(2, ref agrsOut, agrsIn);

                        //2表示输入参数的个数，输出结构都在argsOut中，类似于c的指针参数输入

//转换得到实际的输出参数

            MWNumericArray x1 = agrsOut[0] as MWNumericArray;

                       MWNumericArray x2 = agrsOut[1] as MWNumericArray;

                       c = (double[,])x1.ToArray();

                      d = (double[,])x2.ToArray();

//一定要注意最后c和d的转化，不同类型的转换差异很大厄

//ToArray()对应n*m的数组

//ToScalarDouble()对应单个数值

//ToVetor()对应1维数组

到此，已经实现了c#调用matlab函数的整个过程。

## 2软件界面的设计

## 2.1软件设计

C#有很方便的界面设计平台。下图是我最后完成的效果：

![clip_image031](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104851913-236109740.jpg)



我实现了心电曲线的实时绘制，正常QT间期范围设置、QT间期、QR间期的实时显示、可以显示一分钟之内的平均QT间期、QT间期状态一栏可实现间期预警，如果QT间期大于正常QT间期范围则状态颜色由绿色变成红色。用了一个100ms的定时器，每次100ms之后触发一次画图，让曲线往右移一格，由于100ms很短，所以在视觉上形成了连续移动的效果。核心代码如下：



```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
using MathWorks.MATLAB.NET.Arrays;//在MWArray.dll，最常用的 
using MathWorks.MATLAB.NET.Utility;// 在MWArray.dll，最常用的 
using third;
namespace MsPaint
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        double[,] rbegin;
        double[,] pend;
        double[,] rpk;
        double qt_mean;
        double[] ce = new double[5];
        MWArray d1;
        double[,] f1;
        MWArray[] agrsOut = new MWArray[4];//两个输出参数，一定要写数量
        public Point[] ptlist;//存放点的数组
        Point[] data = new Point[12000];
        //Random rm = new Random();//随机数产生器
        Timer mytimer = new Timer();//定时器
        third.Class1 pcl = new Class1();
        int 网格间距 = 12; //网格间距
        int 网格偏移 = 0;   //网格偏移
        Pen 网格颜色 = new Pen(Color.FromArgb(0x00, 0x80, 0x40));
        Pen 曲线颜色 = new Pen(Color.Lime);
        Pen R颜色 = new Pen(Color.Red,1);
        Pen Q颜色 = new Pen(Color.DeepSkyBlue, 1);
        Pen T颜色 = new Pen(Color.DeepPink, 1);
        int time_count = 20;
        //窗口加载时调用
        private void Form1_Load(object sender, EventArgs e)
        {
            //设置控件的样式和行为，以减少图像的闪烁
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint, true);
            this.SetStyle(ControlStyles.ResizeRedraw, true);
            this.UpdateStyles();
        }
 
        draw drawtest = new draw();//创建类 draw 的实例
        private void 打开OToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog d = new OpenFileDialog();
            d.Filter = "(*.mat)|*.mat|所有文件(*.*)|*.*"; if (d.ShowDialog() == DialogResult.OK)
            {
                FileStream fs = File.OpenRead(d.FileName); StreamReader sr = new StreamReader(fs); string s;
                string filename = d.FileName;
                d1 =pcl.loaddata(filename);
               // MessageBox.Show(filename);
                MWArray[] agrsIn = new MWArray[] {d1};
                pcl.pces(4, ref agrsOut, agrsIn);
                MWNumericArray x1 = agrsOut[0] as MWNumericArray;
                MWNumericArray x2 = agrsOut[1] as MWNumericArray;
                MWNumericArray x3 = agrsOut[2] as MWNumericArray;
                MWNumericArray x4 = agrsOut[3] as MWNumericArray;
                rbegin = (double[,])x1.ToArray();
                pend = (double[,])x2.ToArray();
                rpk = (double[,])x3.ToArray();
                f1 = (double[,])d1.ToArray();
                qt_mean = x4.ToScalarDouble();
                textBox4.Text = qt_mean.ToString();
                for (int i = 0; i < 12000; i++)
                {
                    data[i].X = (int)i;//强制类型转换，将double转为int，可能会丢失数据
                    data[i].Y = (int)((1000-f1[0, i*5]) * 250 / 4500 + 100);
                }
                this.timer1.Enabled = true;//可以使用
                this.timer1.Interval = 100;//定时时间为100毫秒
                this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
 
                this.timer1.Start();//启动定时器
            }
        }
        private void button1_Click(object sender, EventArgs e)
        {
            //动态添加一个定时器
            timer1.Start();//启动定时器
            textBox1.Enabled = false;
            textBox5.Enabled = false;
        }
        private void button2_Click(object sender, EventArgs e)
        {
            timer1.Stop();
            textBox1.Enabled = true;
            textBox5.Enabled = true;
        }
        private void timer1_Tick(object sender, EventArgs e)
        {
            if (time_count == 1100)
                time_count = 0;
            else
                time_count++;
          
           网格偏移 = (网格偏移 + 1000) % 网格间距;
            Invalidate();
            Point[] temp = new Point[600];
            for (int i = 0; i < 600; i++)
            {
                temp[i].X = i;
                temp[i].Y = data[time_count * 10 + i].Y;
            }
            //调用绘图函数,这里的参数可以根据不同的测量给定不同的实参
            //drawtest.DrawLineS(Color.Blue, 1200, 600, pictureBox1, ptlist);
            int index = (int)time_count / 20;
            double QT = (pend[0, index]-rbegin[0, index])/1000;
            Graphics g2 = pictureBox2.CreateGraphics();//创建 PictureBox窗体的画布
            if ((QT < double.Parse(textBox1.Text)) || (QT > double.Parse(textBox5.Text)))
            {
                g2.FillRectangle(Brushes.Red, g2.ClipBounds);
            }
            else
            {
                g2.FillRectangle(Brushes.LightGreen, g2.ClipBounds);
            }
            textBox2.Text = Convert.ToString(QT);
            double QR = (rpk[0, index]-rbegin[0, index])/1000;
            textBox3.Text = Convert.ToString(QR);
           // drawtest.DrawLineS(Color.Blue, 400, 2400, pictureBox1, data);
            Graphics g1 = pictureBox1.CreateGraphics();//创建 PictureBox窗体的画布
            g1.FillRectangle(Brushes.Black, g1.ClipBounds);
            //绘制纵线 从右向左绘制
            for (int i = pictureBox1.Width - 网格偏移; i >= 0; i -= 网格间距)
                g1.DrawLine(网格颜色, i, 0, i, pictureBox1.Height);
            //绘制横线
            for (int i = pictureBox1.Height; i >= 0; i -= 网格间距)
                g1.DrawLine(网格颜色, 0, i, pictureBox1.Width, i);
            for (int i = 0; i < 60; i++)
            {
                    int line = (int)(rpk[0, i]/5-time_count * 10);
                    g1.DrawLine(R颜色, line, 0, line, pictureBox1.Height);
            }
            for (int i = 0; i < 60; i++)
            {
                int line = (int)(pend[0, i] / 5 - time_count * 10);
                g1.DrawLine(T颜色, line, 0, line, pictureBox1.Height);
            }
            for (int i = 0; i < 60; i++)
            {
                int line = (int)(rbegin[0, i] / 5 - time_count * 10);
                g1.DrawLine(Q颜色, line, 0, line, pictureBox1.Height);
            }
            //绘制曲线 若想曲线从右向左移动，则必须先绘制后面的      
            Pen p = new Pen(Color.Yellow, 1);//画笔
                g1.DrawLines(p, temp);//五点绘图，直线连接
        }
    }
}
```






## 2.2软件的使用



在\MsPaint\bin\Debug目录下，双击：实时绘制曲线.exe运行如下图：



![clip_image033](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104906132-1018625572.jpg)



单击：文件-打开

![clip_image035](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104906897-1275203812.jpg)



由于我们只做了最后一个的处理，故只能选择最后一个打开，打开之后则可看到程序运行效果：



![clip_image037](http://images2015.cnblogs.com/blog/904258/201603/904258-20160311104907804-849570403.jpg)







项目github：[https://github.com/HUSTLX/electrocardiogram](https://github.com/HUSTLX/electrocardiogram)




