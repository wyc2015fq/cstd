# C# 实现多分屏视频播放-自定义控件集合无序性问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月09日 23:01:08[boonya](https://me.csdn.net/boonya)阅读数：926








参考：[自定义控件使其填充方格且自动变换大小](https://www.cnblogs.com/dyee/archive/2011/12/30/2307270.html)文章，据此生成了多分屏的视频播放器。这里需要注意Controls的数组的无序性。

这里看下自定义控件的代码：

```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using CvNetVideo.Event;
using System.Runtime.InteropServices;
using CvNetVideo.Play;
using static CvNetVideo.UCVideo;

namespace CvNetVideo
{
    [Guid("f3943464-829d-4bb8-8b05-10516831ceed")]
    //指示应用该属性的对象对COM可见
    [ComVisible(true)]
    //将事件接收接口连接到托管类
    [ComSourceInterfacesAttribute(typeof(MulitiControlEvents))]
    public partial class UserControlVideo : UserControl
    {
        #region 属性变量
        /// <summary>
        /// 选择的分屏索引
        /// </summary>
        public int SelectedIndex { set; get; }

        /// <summary>
        /// 操作的分屏索引
        /// </summary>
        private int OperationIndex;


        /// <summary>
        /// 控件原始宽度
        /// </summary>
        public int OldWidth { set; get; }
        /// <summary>
        /// 控件原始高度
        /// </summary>
        public int OldHeight { set; get; }
        public bool IsFullScreenInSystem { get; set; }

        public bool IsFullScreen = false;

        #endregion

        public UserControlVideo()
        {
            InitializeComponent();
        }

        # region 事件回调

        /// <summary>
        /// 全屏或退出全屏事件
        /// </summary>
        public event MulitiFullScreenEvent OnFullScreenEvent;
        /// <summary>
        /// 直播退出事件
        /// </summary>
        public event MulitiRealTimeLiveAllStopEvent OnRealTimeLiveAllStopEvent;
        /// <summary>
        /// 控件双击事件(控制全屏)
        /// </summary>
        public event MulitiControlDoubleClickEvent OnControlDoubleClickEvent;
        /// <summary>
        /// 异步远程视频回放事件
        /// </summary>
        public event MulitiAsyncRemoteVideoPlaybackEvent OnAsyncRemoteVideoPlaybackEvent;
        /// <summary>
        /// 发生某个事件触发事件
        /// </summary>
        public event MulitidglEventTrigger OnEventTrigger;
        /// <summary>
        /// 异步FTP文件上传事件
        /// </summary>
        public event MulitiAsyncFtpFileUploadEvent OnAsyncFtpFileUploadEvent;
        /// <summary>
        /// 实时视频播放超时事件
        /// </summary>
        public event MulitiRealtimeVideoOuttimeEvent OnRealtimeVideoOuttimeEvent;
        /// <summary>
        /// OCX操作优先级通知事件
        /// </summary>
        public event MulitiRealtimeVideoPriorityNotifyEvent OnRealtimeVideoPriorityNotifyEvent;
        /// <summary>
        /// 视频截图完成回调事件
        /// </summary>
        public event MulitiScreenshotCompletedEvent OnScreenshotCompletedEvent;
        /// <summary>
        /// 视频点击选中回调事件
        /// </summary>
        public event MulitiScreenSelectEvent OnScreenSelectEvent;
        /// <summary>
        /// 视频分屏重载回调事件
        /// </summary>
        public event MulitiReloadControlScreenEvent OnReloadControlScreenEvent;
        /// <summary>
        /// 全部关闭实时视频回调事件
        /// </summary>
        public event MulitiStopAllRealtimeVideoScreenEvent OnStopAllRealtimeVideoScreenEvent;

        /// <summary>
        /// 视频控件选中事件回调
        /// </summary>
        /// <param name="id"></param>
        public void ControlSelectdCallback(int id)
        {
            this.SelectedIndex = id;
            this.OperationIndex = this.SelectedIndex;
            for (var i = 0; i < this.Controls.Count; i++)
            {
                UCVideo video = this.Controls[i] as UCVideo;
                if (this.SelectedIndex == video.Id)
                {
                    video.BorderStyle = BorderStyle.Fixed3D;
                }
                else
                {
                    video.BorderStyle = BorderStyle.FixedSingle;
                }
            }
            // 网页选中视频事件回调
            OnScreenSelectEvent?.Invoke(id);
        }

        /// <summary>
        /// 全屏回调事件回调
        /// </summary>
        /// <param name="id"></param>
        /// <param name="isFullScreen"></param>
        public void ControlFullScreenEventCallback(int id, bool isFullScreen)
        {
            OpenOrCloseFullScreen(id, isFullScreen);
            OnFullScreenEvent?.Invoke(id, isFullScreen);
        }

        /// <summary>
        /// 视频关闭事件回调
        /// </summary>
        /// <param name="id"></param>
        public void ControlRealTimeLiveAllStopEventCallback(int id)
        {
            OnRealTimeLiveAllStopEvent?.Invoke(id);
        }

        /// <summary>
        /// 视频双击(全屏/退出全屏)事件回调
        /// </summary>
        /// <param name="id"></param>
        /// <param name="isFullScreen"></param>
        public void ControlDoubleClickEventCallback(int id, bool isFullScreen)
        {
            OnControlDoubleClickEvent?.Invoke(id, isFullScreen);
        }

        /// <summary>
        /// 视频回放异步远程事件回调
        /// </summary>
        /// <param name="id"></param>
        /// <param name="json"></param>
        public void ControlAsyncRemoteVideoPlaybackEventCallback(int id, string json)
        {
            OnAsyncRemoteVideoPlaybackEvent?.Invoke(id, json);
        }

        /// <summary>
        /// 触发ID事件回调
        /// </summary>
        /// <param name="id"></param>
        /// <param name="eventId"></param>
        public void ControldglEventTriggerCallback(int id, int eventId)
        {
            OnEventTrigger?.Invoke(id,eventId);
        }

        /// <summary>
        /// 视频异步FTP文件上传事件回调
        /// </summary>
        /// <param name="id"></param>
        /// <param name="json"></param>
        public void ControlAsyncFtpFileUploadEventCallback(int id, string json)
        {
            OnAsyncFtpFileUploadEvent?.Invoke(id,json);
        }

        /// <summary>
        /// 实时视频播放超时事件回调
        /// </summary>
        /// <param name="id"></param>
        public void ControlRealtimeVideoOuttimeEventCallback()
        {
            OnRealtimeVideoOuttimeEvent?.Invoke();
        }

        /// <summary>
        /// 视频操作优先级通知回调
        /// </summary>
        /// <param name="id"></param>
        /// <param name="priority"></param>
        public void ControlRealtimeVideoPriorityNotifyEventCallback(int id, int priority)
        {
            OnRealtimeVideoPriorityNotifyEvent?.Invoke(id,priority);
        }

        /// <summary>
        /// 视频截图完成事件回调
        /// </summary>
        /// <param name="id"></param>
        /// <param name="fileLocation"></param>
        public void ControlScreenshotCompletedEventCallback(int id, string fileLocation)
        {
            OnScreenshotCompletedEvent?.Invoke(id,fileLocation);
        }

        /// <summary>
        /// 全部关闭实时视频事件回调
        /// </summary>
        public void ControlStopAllRealtimeVideoScreenEventCallback()
        {
            OnStopAllRealtimeVideoScreenEvent?.Invoke();
        }
        #endregion

        #region 全屏操作
        FullScreenObject fullScreenObject;
        /// <summary>
        /// 打开或关闭全屏
        /// </summary>
        public void OpenOrCloseFullScreen(int id,bool isOpen)
        {
            int Count = (int)Math.Sqrt(this.Controls.Count);
            int perWidth = OldWidth / Count;
            int perHeight = OldHeight / Count;
            if (isOpen)
            {
                OldWidth = this.Width;
                OldHeight = this.Height;
                if (fullScreenObject == null)
                {
                    if (IsFullScreenInSystem)
                    {
                        // 系统级别的全屏
                        fullScreenObject = new FullScreenHelper(this);
                    }
                    else
                    {
                        // 容器内的全屏
                        fullScreenObject = new FullScreenInContainerHelper(this);
                    }
                }
                fullScreenObject.FullScreen(isOpen);
                IsFullScreen = isOpen;
                Console.WriteLine("Entrance FullScreen Mode");

                // 全屏显示选择的屏幕视频窗口
                UCVideo fullscreenUcVideo = null;
                foreach (var ucVideo in this.Controls)
                {
                    fullscreenUcVideo = ucVideo as UCVideo;
                    if (id == fullscreenUcVideo.Id)
                    {

                        fullscreenUcVideo.Width = this.Width;
                        fullscreenUcVideo.Height = this.Height;
                        fullscreenUcVideo.Location = new Point(0, 0);
                        fullscreenUcVideo.Visible = true;
                        Console.WriteLine(">>>>>>>>>>>全屏Full ID=" + fullscreenUcVideo.Id);
                        break;
                    }
                }
            }
            else
            {
                if (fullScreenObject != null && IsFullScreen)
                {
                    fullScreenObject.FullScreen(isOpen);
                    IsFullScreen = isOpen;

                    // 复位全屏元素
                    UCVideo fullscreenUcVideo = null;
                    foreach (var ucVideo in this.Controls)
                    {
                        fullscreenUcVideo = ucVideo as UCVideo;
                        if (id == fullscreenUcVideo.Id)
                        {

                            fullscreenUcVideo.Width = perWidth;
                            fullscreenUcVideo.Height = perHeight;
                            fullscreenUcVideo.Location = new Point(fullscreenUcVideo.X, fullscreenUcVideo.Y);
                            fullscreenUcVideo.Visible = true;
                            Console.WriteLine(">>>>>>>>>>>复位Full ID=" + fullscreenUcVideo.Id);
                            break;
                        }
                    }
                    this.Width = OldWidth;
                    this.Height = OldHeight;
                    Console.WriteLine("Exit FullScreen Mode");
                }
            }
        }

        #endregion

        #region 分屏处理
        /// <summary>
        /// 获取视频对象
        /// </summary>
        /// <param name="id"></param>
        /// <returns></returns>
        public UCVideo GetUCVideoById(int id)
        {
            return this.Controls[id] as UCVideo;
        }

        /// 填充视频
        /// </summary>
        /// <param name="num">数量</param>
        public void FillUCVideo(int num)
        {
            // 触发重载事件
            OnReloadControlScreenEvent?.Invoke();
            this.OperationIndex = 0;
            this.Controls.Clear();
            //填充num*num个方格,现在放置的是罗列着的
            for (int i = 0; i < num * num; i++)
            {
                UCVideo uCVideo = new UCVideo();
                // 设置默认ID
                uCVideo.Id = i;
                // 注册视频选择事件
                uCVideo.ControlSelectdCallback = ControlSelectdCallback;
                // 注册视频全屏事件
                uCVideo.ControlFullScreenEventCallback = ControlFullScreenEventCallback;
                // 注册视频退出事件
                uCVideo.ControlRealTimeLiveAllStopEventCallback = ControlRealTimeLiveAllStopEventCallback;
                // 注册视频双击(全屏/退出全屏)事件
                uCVideo.ControlForDoubleClickEventCallback = ControlDoubleClickEventCallback;
                // 注册视频回放查询回调事件
                uCVideo.ControlAsyncRemoteVideoPlaybackEventCallback = ControlAsyncRemoteVideoPlaybackEventCallback;
                // 注册触发视频执行ID事件
                uCVideo.ControldglEventTriggerCallback = ControldglEventTriggerCallback;
                // 注册视频异步FTP文件上传事件
                uCVideo.ControlAsyncFtpFileUploadEventCallback = ControlAsyncFtpFileUploadEventCallback;
                // 注册实时视频播放超时事件
                //uCVideo.ControlRealtimeVideoOuttimeEventCallback = ControlRealtimeVideoOuttimeEventCallback;
                // 注册实时视频操作优先级通知事件
                uCVideo.ControlRealtimeVideoPriorityNotifyEventCallback = ControlRealtimeVideoPriorityNotifyEventCallback;
                // 注册视频截图完成回调事件
                uCVideo.ControlScreenshotCompletedEventCallback = ControlScreenshotCompletedEventCallback;
                // 是否开启桌面超时事件
                uCVideo.SetStartOuttimeListener(false);
                this.Controls.Add(uCVideo);
            }
            //定义方法,因为需要改变大小,所以单独
            this.LayoutUCVideos();
        }

        /// 填充视频
        /// </summary>
        /// <param name="num">数量</param>
        /// <param name="controlExeSelectd">选择代理函数</param>
        public void FillUCVideo(int num, ControlExeSelectd controlExeSelectd)
        {
            this.Controls.Clear();
            //填充num*num个方格,现在放置的是罗列着的
            for (int i = 0; i < num*num; i++)
            {
                UCVideo uCVideo = new UCVideo();
                // 设置默认ID
                uCVideo.Id = i;
                // 注册视频选择事件
                uCVideo.ControlSelectdCallback = ControlSelectdCallback;
                // 注册exe视频选择事件
                uCVideo.ControlExeSelectdCallback = controlExeSelectd;
                //注册视频全屏事件
                uCVideo.ControlFullScreenEventCallback = ControlFullScreenEventCallback;
                this.Controls.Add(uCVideo);
            }
            //定义方法,因为需要改变大小,所以单独
            this.LayoutUCVideos();
        }

        /// <summary>
        /// 布局视频
        /// </summary>
        /// <returns></returns>
        public void LayoutUCVideos()
        {
            OldWidth = this.Width;
            OldHeight = this.Height;
            //去除启动状态,以免开启的时候FillBtn_SizeChanged会报错
            if (this.Controls.Count == 0)
            {
                return;
            }
            //循环多少次?计算出来
            int Count = (int)Math.Sqrt(this.Controls.Count);
            //计算每个视频的的宽度和高度
            int perWidth = this.Width / Count;
            int perHeight = this.Height / Count;

            int Index = 0;
            int X = 0, Y = 0;
            //竖向的循环嵌套横着的循环
            for (int verticalIndex = 0; verticalIndex < Count; verticalIndex++)
            {
                Y = verticalIndex * perHeight;
                //水平向的循环
                for (int horizontalIndex = 0; horizontalIndex < Count; horizontalIndex++)
                {
                    X = horizontalIndex * perWidth;
                    //获取要放置的方格
                    foreach (var ucVideo in this.Controls) 
                    {
                        UCVideo video = ucVideo as UCVideo;
                        if (video.Id==Index)
                        {
                            video.X = X;
                            video.Y = Y;
                            video.Width = perWidth;
                            video.Height = perHeight;
                            //设置当前方格的位置
                            video.Location = new Point(X, Y);
                            Console.WriteLine("=========ID:" + video.Id);
                            break;
                        }
                       
                    }
                    //下一个方格
                    Index++;
                   
                }
            }
        }
        #endregion

        #region 控件事件
        /// <summary>
        /// 控件初始化时加载默认分屏
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UserControlVideo_Load(object sender, EventArgs e)
        {
            FillUCVideo(4);
            LayoutUCVideos();
        }

        private void UserControlVideo_Resize(object sender, EventArgs e)
        {
            if (!IsFullScreen)
            {
                LayoutUCVideos();
            }
        }
        #endregion
    }
}
```

关键代码处理：

```
//获取要放置的方格
                    foreach (var ucVideo in this.Controls) 
                    {
                        UCVideo video = ucVideo as UCVideo;
                        if (video.Id==Index)
                        {
                            video.X = X;
                            video.Y = Y;
                            video.Width = perWidth;
                            video.Height = perHeight;
                            //设置当前方格的位置
                            video.Location = new Point(X, Y);
                            Console.WriteLine("=========ID:" + video.Id);
                            break;
                        }
                       
                    }
```

这里的id不能理解为数组下标，因为是可变的。

![](https://img-blog.csdn.net/20180709230014469?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







