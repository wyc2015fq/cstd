# C# 写成ActiveX空间后对目录操作没有权限 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年03月27日 13:15:47[boonya](https://me.csdn.net/boonya)阅读数：131








在使用截图和录像的过程中遇到目录创建不成功的情况。


##### 输出的BUG日志

```
System.UnauthorizedAccessException: 对路径“D:\Program Files (x86)\CvNavi\CvVideoOcx\record”的访问被拒绝。
   在 System.IO.__Error.WinIOError(Int32 errorCode, String maybeFullPath)
   在 System.IO.Directory.InternalCreateDirectory(String fullPath, String path, Object dirSecurityObj, Boolean checkHost)
   在 System.IO.Directory.InternalCreateDirectoryHelper(String path, Boolean checkHost)
   在 System.IO.Directory.CreateDirectory(String path)
   在 CvNetVideo.Utils.NVCheji.StartRecord() 位置 D:\DEVELOPERS\vs_workspace\SQWL\RealTimeVideo\CvNetVideo\Utils\NVCheji.cs:行号 62
   在 CvNetVideo.UCVideo.mbtnRecord_Click(Object sender, EventArgs e) 位置 D:\DEVELOPERS\vs_workspace\SQWL\RealTimeVideo\CvNetVideo\UCVideo.cs:行号 194
   在 System.Windows.Forms.ToolStripItem.RaiseEvent(Object key, EventArgs e)
   在 System.Windows.Forms.ToolStripMenuItem.OnClick(EventArgs e)
   在 System.Windows.Forms.ToolStripItem.HandleClick(EventArgs e)
   在 System.Windows.Forms.ToolStripItem.HandleMouseUp(MouseEventArgs e)
   在 System.Windows.Forms.ToolStripItem.FireEventInteractive(EventArgs e, ToolStripItemEventType met)
   在 System.Windows.Forms.ToolStripItem.FireEvent(EventArgs e, ToolStripItemEventType met)
   在 System.Windows.Forms.ToolStrip.OnMouseUp(MouseEventArgs mea)
   在 System.Windows.Forms.ToolStripDropDown.OnMouseUp(MouseEventArgs mea)
   在 System.Windows.Forms.Control.WmMouseUp(Message& m, MouseButtons button, Int32 clicks)
   在 System.Windows.Forms.Control.WndProc(Message& m)
   在 System.Windows.Forms.ScrollableControl.WndProc(Message& m)
   在 System.Windows.Forms.ToolStrip.WndProc(Message& m)
   在 System.Windows.Forms.ToolStripDropDown.WndProc(Message& m)
   在 System.Windows.Forms.Control.ControlNativeWindow.OnMessage(Message& m)
   在 System.Windows.Forms.Control.ControlNativeWindow.WndProc(Message& m)
   在 System.Windows.Forms.NativeWindow.Callback(IntPtr hWnd, Int32 msg, IntPtr wparam, IntPtr lparam)
```


##### 使用系统拥有权限的目录

C:\\ProgramData\\

C:\\Documents and Settings\\All Users\\Application Data\\


```
var baseDir = "C:\\ProgramData\\screenshot\\";
                        //var baseDir = "C:\\Documents and Settings\\All Users\\Application Data\\screenshot\\";
                        // 创建截图保存目录
                        Directory.CreateDirectory(baseDir);
```

##### 使用管理员运行代码授权

```
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Threading.Tasks;

namespace CvNetVideo.Play
{
    /// <summary>
    /// 系统路径和文件授权辅助类
    /// </summary>
    public  class AuthorizationHelper
    {
        /// <summary>
        /// 为文件添加users，everyone用户组的完全控制权限
        /// </summary>
        /// <param name="filePath"></param>
        public static void AddSecurityControll2File(string filePath)
        {
            try
            {
                //获取文件信息
                FileInfo fileInfo = new FileInfo(filePath);
                //获得该文件的访问权限
                System.Security.AccessControl.FileSecurity fileSecurity = fileInfo.GetAccessControl();
                //添加ereryone用户组的访问权限规则 完全控制权限
                fileSecurity.AddAccessRule(new FileSystemAccessRule("Everyone", FileSystemRights.FullControl, AccessControlType.Allow));
                //添加Users用户组的访问权限规则 完全控制权限
                fileSecurity.AddAccessRule(new FileSystemAccessRule("Users", FileSystemRights.FullControl, AccessControlType.Allow));
                //设置访问权限
                fileInfo.SetAccessControl(fileSecurity);
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                Console.WriteLine("AddSecurityControll2File is executed.");
            }
        }

        /// <summary>
        ///为文件夹添加users，everyone用户组的完全控制权限
        /// </summary>
        /// <param name="dirPath"></param>
        public static void AddSecurityControll2Folder(string dirPath)
        {
            try
            {
                //获取文件夹信息
                DirectoryInfo dir = new DirectoryInfo(dirPath);
                //获得该文件夹的所有访问权限
                System.Security.AccessControl.DirectorySecurity dirSecurity = dir.GetAccessControl(AccessControlSections.All);
                //设定文件ACL继承
                InheritanceFlags inherits = InheritanceFlags.ContainerInherit | InheritanceFlags.ObjectInherit;
                //添加ereryone用户组的访问权限规则 完全控制权限
                FileSystemAccessRule everyoneFileSystemAccessRule = new FileSystemAccessRule("Everyone", FileSystemRights.FullControl, inherits, PropagationFlags.None, AccessControlType.Allow);
                //添加Users用户组的访问权限规则 完全控制权限
                FileSystemAccessRule usersFileSystemAccessRule = new FileSystemAccessRule("Users", FileSystemRights.FullControl, inherits, PropagationFlags.None, AccessControlType.Allow);
                bool isModified = false;
                dirSecurity.ModifyAccessRule(AccessControlModification.Add, everyoneFileSystemAccessRule, out isModified);
                dirSecurity.ModifyAccessRule(AccessControlModification.Add, usersFileSystemAccessRule, out isModified);
                //设置访问权限
                dir.SetAccessControl(dirSecurity);
            }
            catch (Exception ex)
            {
                throw ex;
            }
            finally
            {
                Console.WriteLine("AddSecurityControll2Folder is executed.");
            }
        }
    }
}
```

注：此种方法必须以管理员方式运行才能拥有权限。

参考文章：[https://www.cnblogs.com/wolf-sun/p/4591734.html](https://www.cnblogs.com/wolf-sun/p/4591734.html)




