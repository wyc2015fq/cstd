# C# Log4net日志代码配置解决OCX不写日志问题 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月08日 11:05:45[boonya](https://me.csdn.net/boonya)阅读数：247








问题描述：


通过log4.config配置的时候在OCX（ActiveX IE插件模式 ）安装时默认路径（C:\Program Files (x86)）下权限不够没法创建和写入日志文件，CAB打包后也没有日志配置文件，为了解决这一问题，需要调整日志存储的路径，并将日志通过代码的方式来配置。

日志配置工具类：

```
using log4net;
using log4net.Appender;
using log4net.Core;
using log4net.Filter;
using log4net.Layout;
using log4net.Repository.Hierarchy;
using SQ.Base;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CvNetVideo.Logs
{
    /// <summary>
    /// 日志设置类
    /// </summary>
    public class LogSettings
    {

        RollingFileAppender appenderForInfo = new RollingFileAppender();

        RollingFileAppender appenderForError = new RollingFileAppender();

        private static LogSettings instance;

        private static object myLock = new object();

        public static string GetLogPath()
        {
            return FileHelp.GetAppDataLocalLow() + "\\CvNetVideo\\Logs\\";
        }

        public static string GetInfoPath(string path=null)
        {
            if (path==null)
            {
                return FileHelp.GetAppDataLocalLow() + "\\CvNetVideo\\Logs\\InfoLog\\";
            }
            return FileHelp.GetAppDataLocalLow() + "\\CvNetVideo\\Logs\\InfoLog\\"+path;
        }

        public static string GetErrorPath(string path = null)
        {
            if (path == null)
            {
                return FileHelp.GetAppDataLocalLow() + "\\CvNetVideo\\Logs\\ErrorLog\\";
            }
            return FileHelp.GetAppDataLocalLow() + "\\CvNetVideo\\Logs\\ErrorLog\\" + path;
        }

        /// <summary>
        /// 初始化配置
        /// </summary>
        private LogSettings()
        {
            ConfigInfoLog(GetInfoPath());
            ConfigErrorLog(GetErrorPath());
        }

        /// <summary>
        /// 获取单例对象配置
        /// </summary>
        /// <returns></returns>
        public static LogSettings getInstance()
        {
            lock (myLock)
            {
                if (instance == null)
                {
                    instance = new LogSettings();
                }
                return instance;
            }
        }

        /// <summary>
        /// 设置系统所用Log实例
        /// </summary>
        /// <param name="logType"></param>
        public void SetLogger(LOGTYPE logType)
        {
            Log.Log4= LogManager.GetLogger(logType.ToString());
            Console.WriteLine(Log.Log4);
        }

        /// <summary>
        /// 设置普通日志信息
        /// </summary>
        private void ConfigInfoLog(string path)
        {
            //设置日志名称
            appenderForInfo.Name = LOGTYPE.INFO.ToString();
            //设置文件路径
            appenderForInfo.File = path;
            //是否在文件中追加
            appenderForInfo.AppendToFile = true;
            //按照文件的大小进行变换日志文件
            //appenderForInfo.RollingStyle = RollingFileAppender.RollingMode.Size;// 保持一个文件，路径:appenderForInfo.File ="..../*.log"
            appenderForInfo.RollingStyle = RollingFileAppender.RollingMode.Composite;
            //最大变换数量
            appenderForInfo.MaxSizeRollBackups = 100;
            //最大文件大小
            appenderForInfo.MaximumFileSize = "10MB";
            //日志文件名是否为静态
            appenderForInfo.StaticLogFileName = false;
            //日期格式
            appenderForInfo.DatePattern = "'INFO_'yyyy-MM-dd'.log'";
            //设置输出日志格式
            PatternLayout patternLayout = new PatternLayout();
            patternLayout.ConversionPattern = "%d [%t] %-5p %c [%x]  - %m%n";
            //启用正则输出
            patternLayout.ActivateOptions();
            //模式布局
            appenderForInfo.Layout = patternLayout;
            //添加日志等级过滤
            LevelRangeFilter levelRangeFilter = new LevelRangeFilter();
            //Level级别由低到高:ALL DEBUG INFO WARN ERROR FATAL None
            levelRangeFilter.LevelMax = Level.Info;
            levelRangeFilter.LevelMin = Level.All;
            appenderForInfo.AddFilter(levelRangeFilter);
            //启用当前配置 
            appenderForInfo.ActivateOptions();
            //设置到配置
            log4net.Config.BasicConfigurator.Configure(appenderForInfo);
        }

        /// <summary>
        /// 设置错误日志信息
        /// </summary>
        private void ConfigErrorLog(string path)
        {
            //设置日志名称
            appenderForError.Name = LOGTYPE.ERRORD.ToString();
            //设置文件路径
            appenderForError.File = path;
            //是否在文件中追加
            appenderForError.AppendToFile = true;
            //按照文件的大小进行变换日志文件
            appenderForError.RollingStyle = RollingFileAppender.RollingMode.Composite;
            //最大变换数量
            appenderForError.MaxSizeRollBackups = 100;
            //最大文件大小
            appenderForError.MaximumFileSize = "10MB";
            //日志文件名是否为静态
            appenderForError.StaticLogFileName = false;
            //日期格式
            appenderForError.DatePattern ="'ERROR_'yyyy-MM-dd'.log'";
            //设置输出日志格式
            PatternLayout patternLayout = new PatternLayout();
            patternLayout.ConversionPattern = "%d [%t] %-5p %c [%x]  - %m%n";
            //启用正则输出
            patternLayout.ActivateOptions();
            //模式布局
            appenderForError.Layout = patternLayout;
            //添加日志等级过滤
            LevelRangeFilter levelRangeFilter = new LevelRangeFilter();
            // Level级别由低到高:ALL DEBUG INFO WARN ERROR FATAL None
            levelRangeFilter.LevelMax = Level.Fatal;
            levelRangeFilter.LevelMin = Level.Warn;
            appenderForError.AddFilter(levelRangeFilter);
            //启用当前配置 
            appenderForError.ActivateOptions();
            //设置到配置
            log4net.Config.BasicConfigurator.Configure(appenderForError);
        }
    }
}
```


调用方式（DEBUG才允许调用）：

```
#if DEBUG

            // 调试用代码
            // 设置系统日志
            LogSettings.getInstance().SetLogger(LOGTYPE.INFO);
            Log.WriteLog4("LogSettings.getInstance().SetLogger(LOGTYPE.INFO)" + DateTime.Now);

#endif
```

打开文件路径：

```
private void tsmiLogManage_Click(object sender, EventArgs e)
        {
            Log.WriteLog4(DateTime.Now+"----------------------------------查看运行日志-----------------------------");
            System.Diagnostics.Process.Start(@FileHelp.GetAppDataLocalLow() + "\\Logs");
        }
```


效果如下图：

![](https://img-blog.csdn.net/20180608110436796?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

系统文件操作：

```
/// <summary>
        /// 用户路径
        /// </summary>
        /// <returns></returns>
        public static string GetUserProfile()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
        }
```

```
/// <summary>
        /// IE保护模式下的低权限操作路径(%userprofile%/AppData/LocalLow)
        /// 参考：https://blog.csdn.net/xt_xiaotian/article/details/5336809
        /// </summary>
        /// <returns></returns>
        public static string GetAppDataLocalLow()
        {
            return GetUserProfile() + "\\AppData\\LocalLow";
        }
```


输出样式说明：

```
%m(message):输出的日志消息，如ILog.Debug(…)输出的一条消息
%n(new line):换行
%d(datetime):输出当前语句运行的时刻
%r(run time):输出程序从运行到执行到当前语句时消耗的毫秒数
%t(thread id):当前语句所在的线程ID
%p(priority): 日志的当前优先级别，即DEBUG、INFO、WARN…等
%c(class):当前日志对象的名称，例如：
%f(file):输出语句所在的文件名。
%l(line)：输出语句所在的行号。
%数字：表示该项的最小长度，如果不够，则用空格填充，如“%-5level”表示level的最小宽度是5个字符，如果实际长度不够5个字符则以空格填充。
```





