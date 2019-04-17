# C# 子线程中访问主线程中的控件 - DumpDoctorWang的博客 - CSDN博客





2017年01月31日 20:12:05[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：1763








```
/*
 * 要想线程操作主线程的控件，首先声明一个委托(delegate)D，
 * 然后实例化委托d,再然后再子线程中使用this.BeginInvoke(d),
 * 就可以访问主线程的控件。
 * */

using System;
using System.Threading;
using System.Windows.Forms;

namespace FormTest
{
    public partial class Form1 : Form
    {
        private Thread mThread;
        private delegate void Edit(Label l, string s); //声明委托
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.mThread = new Thread(new ThreadStart(this.ThreadMethod));
            this.mThread.Start();
        }

        private void ThreadMethod()
        {
            Edit e = new Edit(this.ChangeLableInfo); //实例化委托为e
            this.BeginInvoke(e, this.label1, "点击按钮后"); //调用this.BeginInvoke()函数，把e和e需要的参数用都好分隔开,传入BeginInvoke()函数
        }

        private void ChangeLableInfo(Label l, string s)
        {
            l.Text = s;
        }
    }
}
```





