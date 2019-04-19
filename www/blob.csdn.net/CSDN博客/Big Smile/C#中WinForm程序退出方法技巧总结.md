# C#中WinForm程序退出方法技巧总结 - Big Smile - CSDN博客
2017年10月09日 19:15:08[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1362
本文出处：[http://www.cnblogs.com/healer007/p/5062189.html](http://www.cnblogs.com/healer007/p/5062189.html)
# 一、关闭窗体 
在c#中退出WinForm程序包括有很多方法，如:this.Close(); Application.Exit();Application.ExitThread(); System.Environment.Exit(0); 等他们各自的方法不一样，下面我们就来详细介绍一下。 
1.this.Close();   只是关闭当前窗口，若不是主窗体的话，是无法退出程序的，另外若有托管线程（非主线程），也无法干净地退出； 
2.Application.Exit();  强制所有消息中止，退出所有的窗体，但是若有托管线程（非主线程），也无法干净地退出； 
3.Application.ExitThread(); 强制中止调用线程上的所有消息，同样面临其它线程无法正确退出的问题； 
4.System.Environment.Exit(0);   这是最彻底的退出方式，不管什么线程都被强制退出，把程序结束的很干净。
# 二、登录窗体与主窗体的关闭 
有许多人在用做c# 做登录窗体时会遇到这样的问题，登录成功后当前登录Form关闭，打开Main窗体，但Main窗体点关闭按钮后程序仍在进程仍旧在运行。 
事实上，关闭Main窗体，只是关闭了Main窗体的线程，并没有关闭程序的主线程，即程序的主线程为登录From。 
方法一：（FrmLogin 为登录窗体，FrmMain 为主窗体） //登录窗体中“登录”按钮的单击事件 
```
private void button1_Click(object sender, EventArgs e)        
 { 
       if (txtName.Text == "123456")  
       { 
              FrmMain fm = new FrmMain();    
              fm.Show(); 
              this.Dispose(false);  
        }   
}
```
//主窗体的FormClosing事件或FormClosed事件代码 
private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)         
{ 
            Application.Exit();         
}  
//主窗体的FormClosed事件代码 
private void FrmMain_FormClosed(object sender, FormClosedEventArgs e)         
{ 
            Application.Exit();         
}  
说明：FormClosing事件或FormClosed事件二选一，这两个的区别在于 FormClosed 在关闭后发生，窗体的关闭动作不可取消； 
FormClosing 在关闭前发生，可取消，只要在里面使用 e.Cancel=true; 就可以让窗口不能关闭。  
进阶技巧：在退出程序前弹出确认退出程序的对话框 //主窗体的FormClosing事件代码 
private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)         
{             
            if (MessageBox.Show("真的要退出程序吗？", "退出程序", MessageBoxButtons.OKCancel) == DialogResult.Cancel) 
            { 
                e.Cancel = true; 
            }         
} 
////主窗体的FormClosed事件代码 
private void FrmMain_FormClosed(object sender, FormClosedEventArgs e)         
{ 
            Dispose(); 
            Application.Exit();         
}  
小贴士：以下代码会导致显示确定退出的两次对话框，第一次单击“确定”第二次单击任何一个都会退出。 
private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)         
{             
            if (MessageBox.Show("真的要退出程序吗？", "退出程序", MessageBoxButtons.OKCancel) == DialogResult.OK) 
  { 
                Application.Exit();
             }
             else
             { 
                e.Cancel = true;
             }         
}  
分析：第一次单击了退出对话框的“确定”按钮后，Application.Exit();会导致触发第二次FrmMain_FormClosing()事件，所以就会显示两个确认退出的对话框，但第二对话框的选择对是否退出没有影响。可以改为判断如果单击了不是确认按钮则e.Cancel = true; ，确定按钮什么都不写或不做判断，如果要加上Application.Exit(); 可以将其添加到窗体的FormClosed事件中。  方法二： 
//program.cs  程序入问的Main()方法代码，其中FrmLogin为登录窗体，FrmMain为主窗体  
static void Main()         
{ 
            Application.EnableVisualStyles(); 
            Application.SetCompatibleTextRenderingDefault(false); 
            FrmLogin fl = new FrmLogin(); 
            if (fl.ShowDialog() == DialogResult.OK) 
            { 
                Application.Run(new FrmMain());
             }                     
}  
//登录窗体中的“登录”按钮代码 
private void button1_Click(object sender, EventArgs e)         
{ 
            if (txtName.Text == "123456") 
            { 
                this.DialogResult = DialogResult.OK; 
                this.Close();
             }         
} 
