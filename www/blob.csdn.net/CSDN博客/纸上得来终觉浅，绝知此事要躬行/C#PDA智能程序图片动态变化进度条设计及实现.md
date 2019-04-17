# C#PDA智能程序图片动态变化进度条设计及实现 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年05月05日 13:22:46[boonya](https://me.csdn.net/boonya)阅读数：2397








 前言：SmartProject 项目是C#桌面程序的精简版，很多属性和事件可能都没有设置进SDK中。在最近的PDA程序中，我没用使用进度条。为了防止用户乱点，最开始想使用windows提供的进度条，但是觉得又不适合，所以就试图用图片切换的方式来实现。

**  原理：开启线程切换图片实现显示（子线程不影响主线程而继续往下执行）。**

### 1、进度界面设计效果

![](https://img-blog.csdn.net/20140505130428031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 2、界面切换效果

![](https://img-blog.csdn.net/20140505130502734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


点击质检按钮后跳转（during the new thread running the UI will be changed）：

![](https://img-blog.csdn.net/20140505130705359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


等待业务线程（任务查询）执行完毕后，关闭进度界面。

![](https://img-blog.csdn.net/20140505130827359?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


### 2、关键实现代码片段

**2-1、UI界面类**



```
using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Threading;
using SmartDeviceProjectWtms.Domain;
using SmartDeviceProjectWtms.Repository;
using System.Windows.Forms;

namespace SmartDeviceProjectWtms.Forms
{
    public partial class FrmProcessModel : Form
    {
        public FrmProcessModel()
        {
            InitializeComponent();
        }



        // 线程执行业务类型

        // 0-钻井任务下载入库，1-质检任务下载入库
        public int PROCESS_TYPE { get; set; }

        // 线程执行业务数据
        private string PROCESS_DATA { get; set; }

        // 声明一个委托
        private delegate void NewDelegate();
       
        // 创建一个 Service新线程的方法
        public void runCurrentThread()
        {
            Thread thread;
            ThreadStart threadstart = new ThreadStart(startCase);
            thread = new Thread(threadstart);
            thread.Start();
        }

        // 创建一个 UI新线程的方法
        public void runCurrentUIThread()
        {
            Thread thread;
            ThreadStart threadstart = new ThreadStart(startUiCase);
            thread = new Thread(threadstart);
            thread.Start();
        }

        // 【UI处理】解决Control.Invoke 必须用于与在独立线程上创建的控件交互
        private void startUiCase() 
        {
            if (InvokeRequired)
            {
                // 要 努力 工作的 方法
                BeginInvoke(new NewDelegate(changeUI));

            }
        }

        // 【远程处理】解决Control.Invoke 必须用于与在独立线程上创建的控件交互
        private void startCase()
        {
            if (InvokeRequired)
            {
                // 要 努力 工作的 方法
                if (this.PROCESS_TYPE == 0)
                {
                    BeginInvoke(new NewDelegate(SaveRemoteDrillingTasks));
                }
                else if (this.PROCESS_TYPE == 1)
                {
                    BeginInvoke(new NewDelegate(SaveRemoteQualityTasks));
                }
               
            }
        }

     
        /// <summary>
        /// 【UI动态改变】随机切换显示图片
        /// </summary>
        private void doChangePictures() 
        {
            Image imageA = pictureBox0.Image;
            Image imageB = pictureBox1.Image;
            //实例化随机数产生器random；
            Random rad = new Random();
            // 生成大于0小于11的随机数
            int value = rad.Next(0, 11);
            if (value % 2 == 0)
            {
                pictureBox0.Image = imageA; pictureBox2.Image = imageA;
                pictureBox4.Image = imageA; pictureBox6.Image = imageA;
                pictureBox1.Image = imageB; pictureBox3.Image = imageB;
                pictureBox5.Image = imageB; pictureBox7.Image = imageB;
            }
            else 
            {
                pictureBox0.Image = imageB; pictureBox2.Image = imageB;
                pictureBox4.Image = imageB; pictureBox6.Image = imageB;
                pictureBox7.Image = imageA; pictureBox1.Image = imageA;
                pictureBox3.Image = imageA; pictureBox5.Image = imageA;
            }
        }

        // 【UI处理显示】改变UI图片循环展示
        private void changeUI() 
        {
            int index = 0;
            while (Global.QUEST_STATUS && index <= Global.PROCESS_LOOP_COUNT)
            {
                Thread.Sleep(Global.PROCESS_LOOP_FEQUENCE);
                doChangePictures();
                Application.DoEvents();
                index++;
            }
            // 关闭此窗口
            this.Close();
           
        }

        /// <summary>
        /// 【UI切换&业务处理】启动并执行相应的操作类型
        /// </summary>
        public void toStartUI()
        {
            // 允许发起请求
            Global.QUEST_STATUS = true;

            // 执行UI线程
            this.runCurrentUIThread();

            // 执行业务线程
            //this.runCurrentThread();

           
        }

        /******************************线程处理业务*******************************/

        private readonly DrillingTaskRepository drillingTaskDao = new DrillingTaskRepository();

        private static QualityTaskRepository qualityTaskDao = new QualityTaskRepository();


        /// <summary>
        /// 【钻井任务】添加到本地数据库
        /// </summary>
        /// <returns></returns>
        public void SaveRemoteDrillingTasks()
        {
            //获取服务器任务数据
            List<DrillingTaskModel> modelList = RemoteDataService.GetRemoteToLocalDrillingTasksByJson();

            if (modelList != null)
            {
                foreach (DrillingTaskModel model in modelList)
                {
                    if (model != null)
                    {
                        // 添加任务到本地暂存SDF数据库
                        DrillingTask task = new DrillingTask(Convert.ToString(model.id), Convert.ToString(model.projectId), model.measureLineNo, model.wellsNo, model.wellsName, model.wellsLocation, Convert.ToString(model.latlngNorth), Convert.ToString(model.latlngEast), Convert.ToString(model.elevation), model.processStatus, model.planStartDate, model.taskEidtDate, model.captainName, model.areaName, model.projectCode);
                        try
                        {
                            drillingTaskDao.AddDrillingTask(task);
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("钻井任务入库异常：" + ex.Message);
                            continue;
                        }
                        // 更新任务状态下载状态
                        int flag = 1;
                        flag = RemoteDataService.UpdateDrillingTasksDownloadStatus(task, flag);
                        if (flag == 0)
                        {
                            //更新失败删除本地数据库任务
                            try
                            {
                                drillingTaskDao.DeleteDrillingTaskByTaskId(task.TaskId);
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show("本地任务删除异常：" + ex.Message);
                                continue;
                            }
                        }

                    }
                }
            }
            // 终止线程
            Global.QUEST_STATUS = false;
            ProcessThread.UN_QUIT_THREAD = false;
        }

        /// <summary>
        /// 【质检任务】添加到本地数据库
        /// </summary>
        /// <returns></returns>
        public void SaveRemoteQualityTasks()
        {
            //获取服务器任务数据
            List<QualityTaskModel> modelList = RemoteDataService.GetRemoteToLocalQualityTasksByJson();

            if (modelList != null)
            {
                foreach (QualityTaskModel model in modelList)
                {
                    if (model != null)
                    {
                        // 添加任务到本地暂存SDF数据库
                        QualityTask task = new QualityTask(Convert.ToString(model.id), Convert.ToString(model.projectId), model.measureLineNo, model.wellsNo, model.processStatus, model.planEndDate, model.taskEidtDate, model.checker, model.areaName, model.projectCode);
                        try
                        {
                            qualityTaskDao.AddQualityTask(task);
                        }
                        catch (Exception ex)
                        {
                            MessageBox.Show("钻井任务入库异常：" + ex.Message);
                            continue;
                        }
                        // 更新任务下载状态
                        int flag = RemoteDataService.UpdateQualityTasksDownloadStatus(task.TaskId);
                        if (flag == 0)
                        {
                            //更新失败删除本地数据库任务
                            try
                            {
                                qualityTaskDao.DeleteQualityTaskByTaskId(task.TaskId);
                            }
                            catch (Exception ex)
                            {
                                MessageBox.Show("本地任务删除异常：" + ex.Message);
                                continue;
                            }
                        }

                    }
                }
            }
            // 终止线程
           Global.QUEST_STATUS = false;
           ProcessThread.UN_QUIT_THREAD = false;
        }

        // 窗体活动事件
        private void FrmProcessModel_Activated(object sender, EventArgs e)
        {
            this.toStartUI();
        }

        /// <summary>
        /// 【钻井任务】查询远程数据库任务情况
        /// </summary>
        public void SearchDrillingList(string queryMeasureNo, string queryWellsNo, int queryProcess) 
        {
            // 执行远程数据访问

            Global.TEMP_DATA = RemoteDataService.GetRemoteDrillingTasksJsonByQueryParams(Global.DEVICE_CODE, queryMeasureNo, queryWellsNo, queryProcess);

            // 终止线程

            Global.QUEST_STATUS = false;
            ProcessThread.UN_QUIT_THREAD = false;
        }

        /// <summary>
        /// 【质检任务】查询远程数据库任务情况
        /// </summary>
        public void SearchQualityList(string queryMeasureNo, string queryWellsNo, int queryProcess)
        {
            // 执行远程数据访问

            Global.TEMP_DATA = RemoteDataService.GetRemoteQualityTasksJsonByQueryParams(Global.DEVICE_CODE, queryMeasureNo, queryWellsNo, queryProcess);

            // 终止线程

            Global.QUEST_STATUS = false;
            ProcessThread.UN_QUIT_THREAD = false;
        } 
    }
}
```



**2-2、UI线程委托**



```
using System;
using System.Linq;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Threading;
using SmartDeviceProjectWtms.Forms;

namespace SmartDeviceProjectWtms
{
    public class ProcessThread
    {
        public static bool UN_QUIT_THREAD = true;

        private static ProcessThread _instance;

        private static readonly Object syncLock = new Object();

        public static ProcessThread Instance
        {
            get 
            {
                if (ProcessThread._instance == null)
                {
                    lock (syncLock)
                    {
                        if (ProcessThread._instance == null)
                        {
                            ProcessThread._instance = new ProcessThread();
                        }
                    }
                }
                return ProcessThread._instance;
            }
        }

        private ProcessThread()
        {
            
        }

        private Thread waitingThread;

        private FrmProcessModel waitingForm;

        public void CreateForm()
        {
            if (waitingThread != null && waitingForm != null)
            {
                try
                {
                    waitingThread.Abort();
                }
                catch (Exception)
                {
                }
            }

            waitingThread = new Thread(new ThreadStart(delegate()
            {
                waitingForm = new FrmProcessModel();
                Application.Run(waitingForm);
            }));
            waitingThread.Start();
        }

        public FrmProcessModel GetForm() 
        {
            return waitingForm;
        }


        public void CloseForm()
        {
            if (waitingThread != null)
            {
                try
                {
                    waitingThread.Abort();
                }
                catch (Exception)
                {
                }
            }
      
        }
        
    }
}
```

**2-3、主线程调用**




```
private void funcQualityBtn_Click(object sender, EventArgs e)
        {
            string message = "";
            ProcessThread.UN_QUIT_THREAD = true;
            switch (Global.DEVICE_STATUS)
            {
                case 0:
                    message = "禁用";
                    break;
                case 1:

                    // 打开处理进度条
                    this.operateType = 1;

                    ProcessThread processThread = ProcessThread.Instance;

                    processThread.CreateForm();

                    // 睡眠一会儿等待创建进度显示窗口
                    Thread.Sleep(300);
               
                    // 远程数据处理
                    while (ProcessThread.UN_QUIT_THREAD)
                    {
                        processThread.GetForm().SaveRemoteQualityTasks();

                        Application.DoEvents();

                        processThread.CloseForm();

                        // 打开质检窗口主页
                        FrmQualityMain frmQualityMain = new FrmQualityMain();
                        frmQualityMain.Show();
                    }
                    return;
                case 2:
                    message = "归还";
                    break;
                case 3:
                    message = "损坏";
                    break;
            }

            MessageBox.Show("设备已【" + message + "】,操作无效!");

        }
```
**注意：调用时Thread.sleep(300);的位置。设置此休眠是因为：在while之前窗体还没有被创建，所以为了防止空引用故设置之。如果将Thread.sleep(300);放在while循环内第一行，webservice就会陷入无休止的循环请求中，所以要放置在外面。**







