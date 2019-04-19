# ASP.NET中使用网络驱动器 - 左直拳的马桶_日用桶 - CSDN博客
2012年04月13日 12:33:59[左直拳](https://me.csdn.net/leftfist)阅读数：2599
搞了一个图片站点，图片放在机器A，后台管理网站放在机器B。既然是管理，少不了要对图片进行删除什么的。那象我们这种情况，该如何处理？
1、可以在机器A搞个WebService
2、也可以在机器A搞个远程调用
3、在机器A上将图片存放目录共享，然后机器B映射为网络驱动器
决定采用方法3。
步骤如下：
0、假设机器A（图片服务器）， 计算机名为jsj-01，IP为192.168.0.1；
      机器B（后台管理网站）
1、机器A和机器B都分别建立一个名字、密码相同的帐号，比如coder/123456。
2、机器A上将图片目录共享（img），将coder赋予读写权限。注意 共享和安全 两个地方都要赋。
3、新建两个类，代码如下（照抄可也，我也是抄的）
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Runtime.InteropServices;
public class WNetHelper
{
    [DllImport("mpr.dll", EntryPoint = "WNetAddConnection2")]
    private static extern uint WNetAddConnection2(NetResource lpNetResource, string lpPassword, string lpUsername, uint dwFlags);
    [DllImport("Mpr.dll", EntryPoint = "WNetCancelConnection2")]
    private static extern uint WNetCancelConnection2(string lpName, uint dwFlags, bool fForce);
    [StructLayout(LayoutKind.Sequential)]
    public class NetResource
    {
        public int dwScope;
        public int dwType;
        public int dwDisplayType;
        public int dwUsage;
        public string lpLocalName;
        public string lpRemoteName;
        public string lpComment;
        public string lpProvider;
    }
    /// <summary>
    /// 为网络共享做本地映射
    /// </summary>
    /// <param name="username">访问用户名（windows系统需要加计算机名，如：comp-1\user-1）</param>
    /// <param name="password">访问用户密码</param>
    /// <param name="remoteName">网络共享路径（如：\\192.168.0.9\share）</param>
    /// <param name="localName">本地映射盘符</param>
    /// <returns></returns>
    public static uint WNetAddConnection(string username, string password, string remoteName, string localName)
    {
        NetResource netResource = new NetResource();
        netResource.dwScope = 2;
        netResource.dwType = 1;
        netResource.dwDisplayType = 3;
        netResource.dwUsage = 1;
        netResource.lpLocalName = localName;
        netResource.lpRemoteName = remoteName.TrimEnd('\\');
        uint result = WNetAddConnection2(netResource, password, username, 0);
        return result;
    }
    public static uint WNetCancelConnection(string name, uint flags, bool force)
    {
        uint nret = WNetCancelConnection2(name, flags, force);
        return nret;
    }
}
public class LogonImpersonate : IDisposable
{
    static public string DefaultDomain
    {
        get
        {
            return ".";
        }
    }
    const int LOGON32_LOGON_INTERACTIVE = 2;
    const int LOGON32_PROVIDER_DEFAULT = 0;
    [System.Runtime.InteropServices.DllImport("Kernel32.dll")]
    extern static int FormatMessage(int flag, ref   IntPtr source, int msgid, int langid, ref   string buf, int size, ref   IntPtr args);
    [System.Runtime.InteropServices.DllImport("Kernel32.dll")]
    extern static bool CloseHandle(IntPtr handle);
    [System.Runtime.InteropServices.DllImport("Advapi32.dll", SetLastError = true)]
    extern static bool LogonUser(
    string lpszUsername,
    string lpszDomain,
    string lpszPassword,
    int dwLogonType,
    int dwLogonProvider,
    ref   IntPtr phToken
    );
    IntPtr token;
    System.Security.Principal.WindowsImpersonationContext context;
    public LogonImpersonate(string username, string password)
    {
        if (username.IndexOf("\\") == -1)
        {
            Init(username, password, DefaultDomain);
        }
        else
        {
            string[] pair = username.Split(new char[] { '\\' }, 2);
            Init(pair[1], password, pair[0]);
        }
    }
    public LogonImpersonate(string username, string password, string domain)
    {
        Init(username, password, domain);
    }
    void Init(string username, string password, string domain)
    {
        if (LogonUser(username, domain, password, LOGON32_LOGON_INTERACTIVE, LOGON32_PROVIDER_DEFAULT, ref   token))
        {
            bool error = true;
            try
            {
                context = System.Security.Principal.WindowsIdentity.Impersonate(token);
                error = false;
            }
            finally
            {
                if (error)
                    CloseHandle(token);
            }
        }
        else
        {
            int err = System.Runtime.InteropServices.Marshal.GetLastWin32Error();
            IntPtr tempptr = IntPtr.Zero;
            string msg = null;
            FormatMessage(0x1300, ref   tempptr, err, 0, ref   msg, 255, ref   tempptr);
            throw (new Exception(msg));
        }
    }
    ~LogonImpersonate()
    {
        Dispose();
    }
    public void Dispose()
    {
        if (context != null)
        {
            try
            {
                context.Undo();
            }
            finally
            {
                CloseHandle(token);
                context = null;
            }
        }
    }
}
```
4、然后在后台管理网站中使用这个网路驱动器Z。使用的方法就是引用新写的这个类
```
protected void btnDelete_Click(object sender, EventArgs e)
    {
        //删除图片
        using (LogonImpersonate imper = new LogonImpersonate("code", "123456"))
        {
            uint state = 0;
            if (!Directory.Exists("w:"))
            {
                state = WNetHelper.WNetAddConnection(@"jsj-01\coder", "123456", @"\\192.168.0.1\Img", "z:");
            }
            if (!state.Equals(0))
            {
                throw new Exception("添加网络驱动器错误，错误号：" + state.ToString());
            }
             File.Delete(图片路径);
        }
    }
```
将共享目录映射为Z。
首先是将IIS的账户替换，然后访问网络驱动器。注意用上了using，目的是只在访问该网络驱动器的时候使用，用完即弃，恢复默认身份，否则其他的操作就不行了。
参考文章
http://www.cnblogs.com/sqzhuyi/archive/2011/01/15/aspnet-remote.html
