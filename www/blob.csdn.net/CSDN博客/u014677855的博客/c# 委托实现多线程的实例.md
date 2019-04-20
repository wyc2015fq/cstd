# c# 委托实现多线程的实例 - u014677855的博客 - CSDN博客
2018年09月04日 14:47:00[xiaobigben](https://me.csdn.net/u014677855)阅读数：1439
前面学习了委托，事件和多线程相关的知识。今天我们就通过在项目中常见的实例来加深对多线程的应用。
## 一、使用线程在窗体中显示进度条
在Winform应用程序中，经常用进度条显示进度信息。这时候就可能用到多线程。如果不采用多线程控制进度条的话，窗口界面很容易假死（无法看到进度信息，看起来像界面卡住了）。 
在这个实例中，我们建立一个窗体，窗体中包括一个后台组件，一个进度条控件。当在主窗体中按“开始”按钮时，进度条开始显示进度。 
该实例中用Backgroundworker组件，表示后台工作。通过后台工作组件实现进度条。 
主窗体： 
![这里写图片描述](https://img-blog.csdn.net/20180904133453966?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
进度条窗体： 
![这里写图片描述](https://img-blog.csdn.net/2018090413350881?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
进度条窗体的代码如下：
```
public partial class ProcessForm : Form
    {
        //后台操作
        private BackgroundWorker backgroundworker1;
        public ProcessForm(BackgroundWorker backgroundworker1)
        {
            InitializeComponent();
            this.backgroundworker1 = backgroundworker1;
            this.backgroundworker1.ProgressChanged += new ProgressChangedEventHandler(backgroundworker1_ProcessChanged);
            this.backgroundworker1.RunWorkerCompleted +=new RunWorkerCompletedEventHandler( Backgroundworker1_RunWorkerCompleted);
        }
        /// <summary>
        /// 当辅助线程完成时引发
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void Backgroundworker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            this.Close();
        }
        private void backgroundworker1_ProcessChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar1.Value = e.ProgressPercentage;//异步任务的进度百分比
        }
        private void button1_Click(object sender, EventArgs e)
        {
            backgroundworker1.WorkerSupportsCancellation = true;
            backgroundworker1.CancelAsync();
            button1.Enabled = false;
            Close();
        }
    }
```
主窗体代码如下：
```
public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void btnBegin_Click(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();//开始执行后台操作
            ProcessForm pf = new ProcessForm(backgroundWorker1);//显示进度条窗体，将后台程序传给进度条窗体
            pf.ShowDialog(this);
            pf.Close();
        }
        /// <summary>
        /// 当辅助线程完成时引发
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                MessageBox.Show(e.Error.Message);
            }
        }
        /// <summary>
        /// 操作开始时在另一个线程上运行的事件处理程序
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;
            for(int i=0;i<100;i++)
            {
                Thread.Sleep(100);
                worker.WorkerReportsProgress = true;
                worker.ReportProgress(i);
                if(worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }
            }
        }
    }
```
运行结果： 
![这里写图片描述](https://img-blog.csdn.net/20180904134729326?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
该例是通过后台程序组件完成对进度条的展示。 
也可以在主窗体中通过后台组件完成进度条的显示，窗体设计如下： 
![这里写图片描述](https://img-blog.csdn.net/20180904141021810?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码如下：
```
public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void btnBegin_Click(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();//开始执行后台操作
        }
        /// <summary>
        /// 当辅助线程完成时引发
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                MessageBox.Show(e.Error.Message);
            }
            GC.Collect();
        }
        /// <summary>
        /// 操作开始时在另一个线程上运行的事件处理程序
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;
            for(int i=0;i<101;i++)
            {
                Thread.Sleep(100);
                worker.WorkerReportsProgress = true;
                worker.ReportProgress(i);
                BeginInvoke(new Action(() =>
                {
                    label1.Text = progressBar1.Value.ToString();
                }));
                if(worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }
            }
            BeginInvoke(new Action(() =>
            {
                label1.Text = "完成";
            }));
        }
        /// <summary>
        /// 取消
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            backgroundWorker1.WorkerSupportsCancellation = true;
            backgroundWorker1.CancelAsync();
            button1.Enabled = false;
            Close();
        }
        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar1.Value = e.ProgressPercentage;
        }
    }
```
运行结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20180904141254894?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQ2Nzc4NTU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 二、实例二
功能说明：有时候在处理大量数据的时候，有时候方法的执行需要一定的时间，这时候往往会造成页面或程序的假死状态，对于用户的体验度也很不好。为了避免这种情况的发生，我们可以加一个进度条和一个文本框。来显示程序处理的进度。
```
public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }
        private void btnBegin_Click(object sender, EventArgs e)
        {
            backgroundWorker1.RunWorkerAsync();//开始执行后台操作
        }
        /// <summary>
        /// 当辅助线程完成时引发
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                MessageBox.Show(e.Error.Message);
            }
            GC.Collect();
        }
        /// <summary>
        /// 操作开始时在另一个线程上运行的事件处理程序
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;
            for(int i=0;i<101;i++)
            {
                Thread.Sleep(100);
                //报告进度的属性设为true，允许报告进度
                worker.WorkerReportsProgress = true;
                worker.ReportProgress(i,i+"你好！\r\n");
                if(worker.CancellationPending)
                {
                    e.Cancel = true;
                    break;
                }
            }
        }
        /// <summary>
        /// 取消
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void button1_Click(object sender, EventArgs e)
        {
            backgroundWorker1.WorkerSupportsCancellation = true;
            backgroundWorker1.CancelAsync();
            button1.Enabled = false;
            Close();
        }
        private void backgroundWorker1_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {
            progressBar1.Value = e.ProgressPercentage;
            label1.Text += e.UserState.ToString();
        }
    }
```
