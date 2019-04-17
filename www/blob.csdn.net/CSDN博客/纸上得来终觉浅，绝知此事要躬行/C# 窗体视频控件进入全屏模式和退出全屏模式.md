# C# 窗体视频控件进入全屏模式和退出全屏模式 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月13日 11:02:34[boonya](https://me.csdn.net/boonya)阅读数：1941








窗体控件进入全屏模式和退出全屏模式，视频播放的时候用到此功能。

#### 工具类代码

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CvNetVideo.Play
{
    class FullScreenHelper
    {
        bool m_bFullScreen = false;
        IntPtr m_OldWndParent = IntPtr.Zero;
        WINDOWPLACEMENT m_OldWndPlacement = new WINDOWPLACEMENT();
        Control m_control = null;

        public FullScreenHelper(Control c)
        {
            m_control = c;
        }
        struct POINT
        {
            int x;
            int y;
        };
        struct RECT
        {
            public int left;
            public int top;
            public int right;
            public int bottom;
        };
        //锁定指定窗口，禁止它更新。同时只能有一个窗口处于锁定状态。锁定指定窗口，禁止它更新。同时只能有一个窗口处于锁定状态
        [DllImport("User32.dll")]
        public static extern bool LockWindowUpdate(IntPtr hWndLock);

        //函数来设置弹出式窗口，层叠窗口或子窗口的父窗口。新的窗口与窗口必须属于同一应用程序
        [DllImport("User32.dll")]
        public static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        //函数设置指定窗口的显示状态和恢复，最大化，最小化位置。函数功能： 函及原型   
        [DllImport("User32.dll")]
        public static extern bool SetWindowPlacement(IntPtr hWnd, ref WINDOWPLACEMENT lpwndpl);

        //函数将创建指定窗口的线程设置到前台，并且激活该窗口。键盘输入转向该窗口，并为用户改各种可视的记号
        [DllImport("User32.dll")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);

        //该函数返回桌面窗口的句柄。桌面窗口覆盖整个屏幕。桌面窗口是一个要在其上绘制所有的图标和其他窗口的区域
        [DllImport("User32.dll")]
        public static extern IntPtr GetDesktopWindow();

        //函数名。该函数返回指定窗口的显示状态以及被恢复的、最大化的和最小化的窗口位置
        [DllImport("User32.dll")]
        public static extern bool GetWindowPlacement(IntPtr hWnd, ref WINDOWPLACEMENT lpwndpl);

        //是用于得到被定义的系统数据或者系统配置信息的一个专有名词  
        [DllImport("User32.dll")]
        public static extern int GetSystemMetrics(int nIndex); 

        [DllImport("user32.dll", EntryPoint = "GetParent", SetLastError = true)]
        public static extern IntPtr GetParent(IntPtr hWnd);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern int SetWindowPos(IntPtr hWnd, int hWndInsertAfter, int x, int y, int Width, int Height, int flags);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern System.IntPtr GetForegroundWindow();
        [DllImport("user32")]
        public static extern bool GetWindowRect(IntPtr hwnd, out RECT lpRect);
        [DllImport("user32.dll")]
        public static extern uint ScreenToClient(IntPtr hwnd, ref POINT p);
         public void FullScreen(bool flag)
        {
            m_bFullScreen = flag;
            if (!m_bFullScreen)
            {
                LockWindowUpdate(m_control.Handle);
                SetParent(m_control.Handle, m_OldWndParent);
                SetWindowPlacement(m_control.Handle, ref m_OldWndPlacement);
                SetForegroundWindow(m_OldWndParent);
                LockWindowUpdate(IntPtr.Zero);
            }
            else
            {
                GetWindowPlacement(m_control.Handle, ref m_OldWndPlacement);
                int nScreenWidth = GetSystemMetrics(0);
                int nScreenHeight = GetSystemMetrics(1);
                m_OldWndParent = GetParent(m_control.Parent.Handle);
                SetParent(m_control.Handle, GetDesktopWindow());
                WINDOWPLACEMENT wp1 = new WINDOWPLACEMENT();
                wp1.length = (uint)Marshal.SizeOf(wp1);
                wp1.showCmd = 1;
                wp1.rcNormalPosition.left = 0;
                wp1.rcNormalPosition.top = 0;
                wp1.rcNormalPosition.right = nScreenWidth;
                wp1.rcNormalPosition.bottom = nScreenHeight;
                SetWindowPlacement(m_control.Handle, ref wp1);
                SetForegroundWindow(GetDesktopWindow());
                SetForegroundWindow(m_control.Handle);
            }
            m_bFullScreen = !m_bFullScreen;
        }
        struct WINDOWPLACEMENT
        {
            public uint length;
            public uint flags;
            public uint showCmd;
            public POINT ptMinPosition;
            public POINT ptMaxPosition;
            public RECT rcNormalPosition;
        };
    }
}
```

#### 调用方式

```
/// <summary>
        /// 全屏事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UCVideo_DoubleClick(object sender, EventArgs e)
        {
            //全屏设置
            //sdlVideo.SDL_MaximizeWindow();
            fullScreenHelper = new CvNetVideo.Play.FullScreenHelper(this);
            fullScreenHelper.FullScreen(true);
            Console.WriteLine("Entrance FullScreen Mode");
        }
        /// <summary>
        ///  按键弹起事件
        /// </summary>
        private void UCVideo_KeyUp(object sender, KeyEventArgs e)
        {
            // ESC 退出全屏
            if (e.KeyCode == Keys.Escape&& fullScreenHelper!=null)
            {
                fullScreenHelper.FullScreen(false);
                fullScreenHelper = null;
                Console.WriteLine("Exit FullScreen Mode");
            }
            
        }
```

#### 测试效果图

![](https://img-blog.csdn.net/20180313110100383)


![](https://img-blog.csdn.net/2018031311011695)


注意：在使用SDL的全屏操作过程中设置是无效的，播放视频过程中不能实现修改。代码如下：

```
public void SDL_MaximizeWindow()
        {
            Console.WriteLine("设置全屏");

            SDL.SDL_MaximizeWindow(screen);
            SDL.SDL_SetWindowFullscreen(screen, SDL.SDL_GetWindowFlags(screen));
            SDL.SDL_ShowWindow(screen);

            //int width, height;
            //// 获取最大窗口值
            //SDL2.SDL.SDL_GetWindowMaximumSize(screen, out width, out height);
            //// 设置最大窗口值
            //if (width>0&&height>0)
            //{
            //    SDL2.SDL.SDL_SetWindowMaximumSize(screen, width, height);
            //    Console.WriteLine("设置全屏......成功!width=" + width + ",height=" + height);
            //}
            //else
            //{
            //    Console.WriteLine("设置全屏......失败!width=" + width + ",height=" + height);
            //    SDL2.SDL.SDL_SetWindowMaximumSize(screen, w, h);
            //}
        }
```

# 工具代码功能改进

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace CvNetVideo.Play
{
    /// <summary>
    /// 定义全屏抽象类
    /// </summary>
    public abstract class FullScreenObject
    {
       public abstract void FullScreen(bool flag);
    }
    /// <summary>
    /// 桌面全屏
    /// </summary>
    public unsafe class FullScreenHelper: FullScreenObject
    {
        bool m_bFullScreen = false;

        WINDOWPLACEMENT m_OldWndPlacement = new WINDOWPLACEMENT();

        UCVideo m_control = null;

        public FullScreenHelper(UCVideo control)
        {
            m_control = control;
        }

        private IntPtr m_OldWndParent = IntPtr.Zero;

        DockStyle old_docker_style;
        int old_left;
        int old_width;
        int old_height;
        int old_top;

        public override void FullScreen(bool flag)
        {
            m_bFullScreen = flag;
            if (!m_bFullScreen)
            {
                #region 方式一：窗体容积改变时不能全屏，未能解决IE全屏显示不全问题
                //ShellSDK.LockWindowUpdate(m_control.Handle);
                //ShellSDK.SetParent(m_control.Handle, m_OldWndParent);
                //ShellSDK.SetWindowPlacement(m_control.Handle, ref m_OldWndPlacement);
                //ShellSDK.SetForegroundWindow(m_OldWndParent);
                //ShellSDK.LockWindowUpdate(IntPtr.Zero);
                #endregion

                #region 方式二：在容器改变时可以实现全屏，未能解决IE全屏显示不全问题
                // 取消全屏设置
                m_control.Dock = old_docker_style;
                m_control.Left = old_left;
                m_control.Top = old_top;
                m_control.Width = old_width;
                m_control.Height = old_height;
                ShellSDK.SetParent(m_control.Handle, m_OldWndParent);
                #endregion
            }
            else
            {
                #region 方式一：窗体容积改变时不能全屏，未能解决IE全屏显示不全问题
                //ShellSDK.GetWindowPlacement(m_control.Handle, ref m_OldWndPlacement);
                //int nScreenWidth = ShellSDK.GetSystemMetrics(0);
                //int nScreenHeight = ShellSDK.GetSystemMetrics(1);
                //m_OldWndParent = ShellSDK.GetParent(m_control.Handle);
                //ShellSDK.SetParent(m_control.Handle, ShellSDK.GetDesktopWindow());
                //WINDOWPLACEMENT wp1 = new WINDOWPLACEMENT();
                //wp1.length = (uint)Marshal.SizeOf(wp1);
                //wp1.showCmd = 1;
                //wp1.rcNormalPosition.left = 0;
                //wp1.rcNormalPosition.top = 0;
                //wp1.rcNormalPosition.right = Screen.PrimaryScreen.Bounds.Width/*nScreenWidth*/;
                //wp1.rcNormalPosition.bottom = Screen.PrimaryScreen.WorkingArea.Height/* nScreenHeight*/;
                //ShellSDK.SetWindowPlacement(m_control.Handle, ref wp1);
                //ShellSDK.SetForegroundWindow(ShellSDK.GetDesktopWindow());
                //ShellSDK.SetForegroundWindow(m_control.Handle);
                #endregion

                #region 方式二：在容器改变时可以实现全屏，未能解决IE全屏显示不全问题
                // 记录原来的数据
                old_docker_style = m_control.Dock;
                old_left = m_control.Left;
                old_width = m_control.Width;
                old_height = m_control.Height;
                old_top = m_control.Top;
                m_OldWndParent = ShellSDK.GetParent(m_control.Handle);
                // 设置全屏数据
                int nScreenWidth = ShellSDK.GetSystemMetrics(0);
                int nScreenHeight = ShellSDK.GetSystemMetrics(1);
                m_control.Dock = DockStyle.None;
                m_control.Left = 0;
                m_control.Top = 0;
                m_control.Width = nScreenWidth;
                m_control.Height = nScreenHeight;
                ShellSDK.SetParent(m_control.Handle, ShellSDK.GetDesktopWindow());
                ShellSDK.SetWindowPos(m_control.Handle, -1, 0, 0, m_control.Right - m_control.Left, m_control.Bottom - m_control.Top, 0);
                #endregion
            }
            m_bFullScreen = !m_bFullScreen;
        }

    }

    /// <summary>
    /// 在容器内部全屏
    /// </summary>
    public class FullScreenInContainerHelper : FullScreenObject
    {
        bool m_bFullScreen = false;

        Control m_control = null;

        public FullScreenInContainerHelper(Control control)
        {
            m_control = control;
        }

        private IntPtr m_OldWndParent = IntPtr.Zero;
        private IntPtr m_father_hwnd;
        private RECT m_rect = new RECT();

        public override void FullScreen(bool flag)
        {
            m_bFullScreen = flag;
            if (!m_bFullScreen)
            {
                ShellSDK.SetParent(m_control.Handle, m_father_hwnd);
                ShellSDK.SetWindowPos(m_control.Handle, 0, m_rect.left, m_rect.top, m_rect.right - m_rect.left, m_rect.bottom - m_rect.top, 0);
                ShellSDK.SetForegroundWindow(m_father_hwnd);
            }
            else
            {
                m_father_hwnd = ShellSDK.GetParent(m_control.Handle);
                ShellSDK.GetWindowRect(m_control.Handle, out RECT rect);
                POINT pt = new POINT();
                pt.x = rect.left;
                pt.y = rect.top;
                ShellSDK.ScreenToClient(m_father_hwnd, ref pt);
                rect.right = rect.right - rect.left + pt.x;
                rect.bottom = rect.bottom - rect.top + pt.y;
                rect.left = pt.x;
                rect.top = pt.y;
                m_rect = rect;
                ShellSDK.GetWindowRect(m_father_hwnd, out RECT rect_fature);
                ShellSDK.SetWindowPos(m_control.Handle, 0, 0, 0, rect_fature.right - rect_fature.left, rect_fature.bottom - rect_fature.top, 0);
            }
            m_bFullScreen = !m_bFullScreen;
        }
    }

    /// <summary>
    /// Windows系统API-SDK
    /// </summary>
    public class ShellSDK
    {
        //锁定指定窗口，禁止它更新。同时只能有一个窗口处于锁定状态。锁定指定窗口，禁止它更新。同时只能有一个窗口处于锁定状态
        [DllImport("User32.dll")]
        public static extern bool LockWindowUpdate(IntPtr hWndLock);

        //函数来设置弹出式窗口，层叠窗口或子窗口的父窗口。新的窗口与窗口必须属于同一应用程序
        [DllImport("User32.dll")]
        public static extern IntPtr SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

        //函数设置指定窗口的显示状态和恢复，最大化，最小化位置。函数功能： 函及原型   
        [DllImport("User32.dll")]
        public static extern bool SetWindowPlacement(IntPtr hWnd, ref WINDOWPLACEMENT lpwndpl);

        //函数将创建指定窗口的线程设置到前台，并且激活该窗口。键盘输入转向该窗口，并为用户改各种可视的记号
        [DllImport("User32.dll")]
        public static extern bool SetForegroundWindow(IntPtr hWnd);

        //该函数返回桌面窗口的句柄。桌面窗口覆盖整个屏幕。桌面窗口是一个要在其上绘制所有的图标和其他窗口的区域
        [DllImport("User32.dll")]
        public static extern IntPtr GetDesktopWindow();

        //函数名。该函数返回指定窗口的显示状态以及被恢复的、最大化的和最小化的窗口位置
        [DllImport("User32.dll")]
        public static extern bool GetWindowPlacement(IntPtr hWnd, ref WINDOWPLACEMENT lpwndpl);

        //是用于得到被定义的系统数据或者系统配置信息的一个专有名词  
        [DllImport("User32.dll")]
        public static extern int GetSystemMetrics(int nIndex); 

        [DllImport("user32.dll", EntryPoint = "GetParent", SetLastError = true)]
        public static extern IntPtr GetParent(IntPtr hWnd);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern int SetWindowPos(IntPtr hWnd, int hWndInsertAfter, int x, int y, int Width, int Height, int flags);
        [DllImport("user32.dll", CharSet = CharSet.Auto)]
        public static extern System.IntPtr GetForegroundWindow();
        [DllImport("user32")]
        public static extern bool GetWindowRect(IntPtr hwnd, out RECT lpRect);
        [DllImport("user32.dll")]
        public static extern uint ScreenToClient(IntPtr hwnd, ref POINT p);
    }

    /// <summary>
    /// 图像区域对象
    /// </summary>
    public struct RECT
    {
        public int left;
        public int top;
        public int right;
        public int bottom;
    }

    /// <summary>
    /// 图像点位位置
    /// </summary>
    public struct POINT
    {
        public int x;
        public int y;
    }

    /// <summary>
    /// 图像窗口对象
    /// </summary>
    public struct WINDOWPLACEMENT
    {
        public uint length;
        public uint flags;
        public uint showCmd;
        public POINT ptMinPosition;
        public POINT ptMaxPosition;
        public RECT rcNormalPosition;
    }
}
```


更多sdl窗口操作参考:

SDL_Window ：[http://blog.csdn.net/leixiaohua1020/article/details/40701203](http://blog.csdn.net/leixiaohua1020/article/details/40701203)

SDL 多线程窗口 : [http://blog.csdn.net/x_iya/article/details/52198688](http://blog.csdn.net/x_iya/article/details/52198688)



