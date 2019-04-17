# 使用VS创建第一个窗口程序 - 小灰笔记 - CSDN博客





2017年07月10日 08:00:50[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：6041








       我觉得C#或者说整个VS的开发环境或者功能对我最有吸引力的不是什么控制台程序，这种程序或者功能的实现使用Python等脚本语言实现起来似乎是更为便捷。对我真正有吸引力的是Windows的窗口程序。

       接下来使用VS社区版本创建一个窗体程序：

1，在创建项目的时候选择Windows窗体应用程序

![](https://img-blog.csdn.net/20170710080010587?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




2，从左侧的工具栏中加入一个按钮

![](https://img-blog.csdn.net/20170710080021249?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




3，双击按钮，进入到按键激活的程序代码段，编写代码如下：

using System;

usingSystem.Collections.Generic;

usingSystem.ComponentModel;

usingSystem.Data;

usingSystem.Drawing;

usingSystem.Linq;

usingSystem.Text;

usingSystem.Threading.Tasks;

usingSystem.Windows.Forms;



namespaceWindowsFormsApplication1

{

public
partialclass
Form1 : Form

   {

public Form1()

       {

            InitializeComponent();

       }



private
void button1_Click(object sender, 
EventArgs e)

       {

MessageBox.Show("FirstWindows Forms App!");

       }

   }

}

       值得一提的一个地方：与老版本的VS相比，新的版本代码行数的显示都是默认的，不需要自己额外调试，这是非常好的一点改进。

4，编译运行后，弹出设计的界面，点击上面的按钮会弹出一个对话框：

![](https://img-blog.csdn.net/20170710080033125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3JleV9jc2Ru/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




       这样，第一个Windows窗体程序的设计就完成了。



