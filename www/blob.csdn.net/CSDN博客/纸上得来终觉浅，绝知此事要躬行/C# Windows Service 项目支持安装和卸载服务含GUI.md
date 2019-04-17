# C# Windows Service 项目支持安装和卸载服务含GUI - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月25日 16:24:52[boonya](https://me.csdn.net/boonya)阅读数：143








Windows Service 项目支持安装和卸载服务，本篇内容主要尝试实现服务的安装和卸载。实现过程中已支持使用此服务进行任何exe的启动，只要配置好exe和log的全路径记录。

##### 创建windows service项目

![](https://img-blog.csdn.net/2018062515535087?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改项目名称，然后确定。

##### 添加安装控件并修改用户权限

在新建项目的文件中打开“ProjectInstaller.cs”,右键添加“添加安装程序”，然后得到如下所示界面。

![](https://img-blog.csdn.net/20180625155910566?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改处理安装控件的用户级别，改成“LogSystem”方便访问：

![](https://img-blog.csdn.net/20180625160000134?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改服务安装控件的名称，让它看起来是有意义的windows系统服务：

![](https://img-blog.csdn.net/20180625160057711?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 编写安装和卸载bat脚本

安装：

```
cd %~dp0

SET regpath=%~dp0

%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\InstallUtil.exe  %regpath%\RTVSWindowsService.exe

Net Start RTVSWindowsService

::echo "开机自动运行"

sc config RTVSWindowsService start= AUTO

::echo "允许服务与桌面交互"

sc config RTVSWindowsService type= interact type= own

pause
```


卸载：

```
cd %~dp0

SET regpath=%~dp0

%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\InstallUtil.exe /u %regpath%\RTVSWindowsService.exe

pause
```


##### 启动哪个exe使用XML配置文件来决定
此时，我们需要编写配置文件和读取配置文件的工具类，工具类如下：

```
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

namespace RTVSWindowsService
{
    /// <summary>
    /// 系统XML配置属性
    /// </summary>
    class SystemConfig
    {
        /// <summary>
        /// 得到工作文件夹路径
        /// </summary>
        /// <returns></returns>
        public static string GetWorkDirectory()
        {
            try
            {
                return Path.GetDirectoryName(typeof(SystemConfig).Assembly.Location);
            }
            catch
            {
                return AppDomain.CurrentDomain.BaseDirectory;
            }
        }

        /// <summary>
        /// 推断字符串是否为空串
        /// </summary>
        /// <param name="szString">目标字符串</param>
        /// <returns>true:为空串;false:非空串</returns>
        private static bool IsEmptyString(string szString)
        {
            if (szString == null)
                return true;
            if (szString.Trim() == string.Empty)
                return true;
            return false;
        }
        /// <summary>
        /// 创建一个制定根节点名的XML文件
        /// </summary>
        /// <param name="szFileName">XML文件</param>
        /// <param name="szRootName">根节点名</param>
        /// <returns>bool</returns>
        private static bool CreateXmlFile(string szFileName, string szRootName)
        {
            if (szFileName == null || szFileName.Trim() == "")
                return false;
            if (szRootName == null || szRootName.Trim() == "")
                return false;

            XmlDocument clsXmlDoc = new XmlDocument();
            clsXmlDoc.AppendChild(clsXmlDoc.CreateXmlDeclaration("1.0", "GBK", null));
            clsXmlDoc.AppendChild(clsXmlDoc.CreateNode(XmlNodeType.Element, szRootName, ""));
            try
            {
                clsXmlDoc.Save(szFileName);
                return true;
            }
            catch
            {
                return false;
            }
        }

        /// <summary>
        /// 从XML文件获取相应的XML文档对象
        /// </summary>
        /// <param name="szXmlFile">XML文件</param>
        /// <returns>XML文档对象</returns>
        private static XmlDocument GetXmlDocument(string szXmlFile)
        {
            if (IsEmptyString(szXmlFile))
                return null;
            if (!File.Exists(szXmlFile))
                return null;
            XmlDocument clsXmlDoc = new XmlDocument();
            try
            {
                clsXmlDoc.Load(szXmlFile);
            }
            catch
            {
                return null;
            }
            return clsXmlDoc;
        }

        /// <summary>
        /// 将XML文档对象保存为XML文件
        /// </summary>
        /// <param name="clsXmlDoc">XML文档对象</param>
        /// <param name="szXmlFile">XML文件</param>
        /// <returns>bool:保存结果</returns>
        private static bool SaveXmlDocument(XmlDocument clsXmlDoc, string szXmlFile)
        {
            if (clsXmlDoc == null)
                return false;
            if (IsEmptyString(szXmlFile))
                return false;
            try
            {
                if (File.Exists(szXmlFile))
                    File.Delete(szXmlFile);
            }
            catch
            {
                return false;
            }
            try
            {
                clsXmlDoc.Save(szXmlFile);
            }
            catch
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// 获取XPath指向的单一XML节点
        /// </summary>
        /// <param name="clsRootNode">XPath所在的根节点</param>
        /// <param name="szXPath">XPath表达式</param>
        /// <returns>XmlNode</returns>
        private static XmlNode SelectXmlNode(XmlNode clsRootNode, string szXPath)
        {
            if (clsRootNode == null || IsEmptyString(szXPath))
                return null;
            try
            {
                return clsRootNode.SelectSingleNode(szXPath);
            }
            catch
            {
                return null;
            }
        }

        /// <summary>
        /// 获取XPath指向的XML节点集
        /// </summary>
        /// <param name="clsRootNode">XPath所在的根节点</param>
        /// <param name="szXPath">XPath表达式</param>
        /// <returns>XmlNodeList</returns>
        private static XmlNodeList SelectXmlNodes(XmlNode clsRootNode, string szXPath)
        {
            if (clsRootNode == null || IsEmptyString(szXPath))
                return null;
            try
            {
                return clsRootNode.SelectNodes(szXPath);
            }
            catch
            {
                return null;
            }
        }

        /// <summary>
        /// 创建一个XmlNode并加入到文档
        /// </summary>
        /// <param name="clsParentNode">父节点</param>
        /// <param name="szNodeName">结点名称</param>
        /// <returns>XmlNode</returns>
        private static XmlNode CreateXmlNode(XmlNode clsParentNode, string szNodeName)
        {
            try
            {
                XmlDocument clsXmlDoc = null;
                if (clsParentNode.GetType() != typeof(XmlDocument))
                    clsXmlDoc = clsParentNode.OwnerDocument;
                else
                    clsXmlDoc = clsParentNode as XmlDocument;
                XmlNode clsXmlNode = clsXmlDoc.CreateNode(XmlNodeType.Element, szNodeName, string.Empty);
                if (clsParentNode.GetType() == typeof(XmlDocument))
                {
                    clsXmlDoc.LastChild.AppendChild(clsXmlNode);
                }
                else
                {
                    clsParentNode.AppendChild(clsXmlNode);
                }
                return clsXmlNode;
            }
            catch
            {
                return null;
            }
        }

        /// <summary>
        /// 设置指定节点中指定属性的值
        /// </summary>
        /// <param name="parentNode">XML节点</param>
        /// <param name="szAttrName">属性名</param>
        /// <param name="szAttrValue">属性值</param>
        /// <returns>bool</returns>
        private static bool SetXmlAttr(XmlNode clsXmlNode, string szAttrName, string szAttrValue)
        {
            if (clsXmlNode == null)
                return false;
            if (IsEmptyString(szAttrName))
                return false;
            if (IsEmptyString(szAttrValue))
                szAttrValue = string.Empty;
            XmlAttribute clsAttrNode = clsXmlNode.Attributes.GetNamedItem(szAttrName) as XmlAttribute;
            if (clsAttrNode == null)
            {
                XmlDocument clsXmlDoc = clsXmlNode.OwnerDocument;
                if (clsXmlDoc == null)
                    return false;
                clsAttrNode = clsXmlDoc.CreateAttribute(szAttrName);
                clsXmlNode.Attributes.Append(clsAttrNode);
            }
            clsAttrNode.Value = szAttrValue;
            return true;
        }

        #region"配置文件的读取和写入"
        private static string CONFIG_FILE = "SystemConfig.xml";
        /// <summary>
        ///  读取指定的配置文件里指定Key的值
        /// </summary>
        /// <param name="szKeyName">读取的Key名称</param>
        /// <param name="szDefaultValue">指定的Key不存在时,返回的值</param>
        /// <returns>Key值</returns>
        public static int GetConfigData(string szKeyName, int nDefaultValue)
        {
            string szValue = GetConfigData(szKeyName, nDefaultValue.ToString());
            try
            {
                return int.Parse(szValue);
            }
            catch
            {
                return nDefaultValue;
            }
        }

        /// <summary>
        ///  读取指定的配置文件里指定Key的值
        /// </summary>
        /// <param name="szKeyName">读取的Key名称</param>
        /// <param name="szDefaultValue">指定的Key不存在时,返回的值</param>
        /// <returns>Key值</returns>
        public static float GetConfigData(string szKeyName, float fDefaultValue)
        {
            string szValue = GetConfigData(szKeyName, fDefaultValue.ToString());
            try
            {
                return float.Parse(szValue);
            }
            catch
            {
                return fDefaultValue;
            }
        }

        /// <summary>
        ///  读取指定的配置文件里指定Key的值
        /// </summary>
        /// <param name="szKeyName">读取的Key名称</param>
        /// <param name="szDefaultValue">指定的Key不存在时,返回的值</param>
        /// <returns>Key值</returns>
        public static bool GetConfigData(string szKeyName, bool bDefaultValue)
        {
            string szValue = GetConfigData(szKeyName, bDefaultValue.ToString());
            try
            {
                return bool.Parse(szValue);
            }
            catch
            {
                return bDefaultValue;
            }
        }

        /// <summary>
        ///  读取指定的配置文件里指定Key的值
        /// </summary>
        /// <param name="szKeyName">读取的Key名称</param>
        /// <param name="szDefaultValue">指定的Key不存在时,返回的值</param>
        /// <returns>Key值</returns>
        public static string GetConfigData(string szKeyName, string szDefaultValue)
        {
            string szConfigFile = string.Format("{0}\\{1}", GetWorkDirectory(), CONFIG_FILE);
            if (!File.Exists(szConfigFile))
            {
                return szDefaultValue;
            }

            XmlDocument clsXmlDoc = GetXmlDocument(szConfigFile);
            if (clsXmlDoc == null)
                return szDefaultValue;

            string szXPath = string.Format(".//key-value[@name='{0}']", szKeyName);
            XmlNode clsXmlNode = SelectXmlNode(clsXmlDoc, szXPath);
            if (clsXmlNode == null)
            {
                return szDefaultValue;
            }

            XmlNode clsValueAttr = clsXmlNode.Attributes.GetNamedItem("value");
            if (clsValueAttr == null)
                return szDefaultValue;
            return clsValueAttr.Value;
        }

        /// <summary>
        ///  保存指定Key的值到指定的配置文件里
        /// </summary>
        /// <param name="szKeyName">要被改动值的Key名称</param>
        /// <param name="szValue">新改动的值</param>
        public static bool WriteConfigData(string szKeyName, string szValue)
        {
            string szConfigFile = string.Format("{0}\\{1}", GetWorkDirectory(), CONFIG_FILE);
            if (!File.Exists(szConfigFile))
            {
                if (!CreateXmlFile(szConfigFile, "SystemConfig"))
                    return false;
            }
            XmlDocument clsXmlDoc = GetXmlDocument(szConfigFile);

            string szXPath = string.Format(".//key-value[@name='{0}']", szKeyName);
            XmlNode clsXmlNode = SelectXmlNode(clsXmlDoc, szXPath);
            if (clsXmlNode == null)
            {
                clsXmlNode = CreateXmlNode(clsXmlDoc, "key-value");
            }
            if (!SetXmlAttr(clsXmlNode, "name", szKeyName))
                return false;
            if (!SetXmlAttr(clsXmlNode, "value", szValue))
                return false;
            //
            return SaveXmlDocument(clsXmlDoc, szConfigFile);
        }
        #endregion
    }
}
```
SystemConfig.xml配置文件：

```
<?xml version="1.0" encoding="utf-8" ?>
<SystemConfig>
  <!--配置RTVS可执行文件的全路径-->
  <key-value name="RTVS_FULL_EXE_FILE_PATH" value="D:\DEVELOPERS\vs_workspace\SQWL\RealTimeVideo\RTVS\bin\Debug\RTVS.exe"></key-value>
  <!--服务启动停止输出日志全路径-->
  <key-value name="LOG_FULL_FILE_PATH" value="D:\DEVELOPERS\vs_workspace\SQWL\RealTimeVideo\RTVSWindowsService\bin\Debug\StartOrStopLog.log"></key-value>
</SystemConfig>
```

注意：这里的xml节点读取有规则，如key-value标签。

使用工具类来实现启动和关闭服务的调用方法：



```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace RTVSWindowsService
{
    public partial class RTVSWindowsService : ServiceBase
    {
        //全局变量
        string exePath = SystemConfig.GetConfigData("RTVS_FULL_EXE_FILE_PATH", "");

        string logPath = SystemConfig.GetConfigData("LOG_FULL_FILE_PATH", "");

        string path= AppDomain.CurrentDomain.BaseDirectory;

        public RTVSWindowsService()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            LogWrite("Start........");//日志
            LogWrite("Work dictionary:"+ SystemConfig.GetWorkDirectory());
            LogWrite("SystemConfig.xml RTVS_FULL_EXE_FILE_PATH node value:" + exePath);
            LogWrite("SystemConfig.xml LOG_FULL_FILE_PATH node value:" + logPath);
            //启动目标应用程序 winform
            StartProgram(exePath);
            LogWrite("Started!");//日志
        }

        protected override void OnStop()
        {
            LogWrite("Stop........");//日志
            //杀死目标应用程序 winform
            StopProgram(exePath);
            LogWrite("Stopped!");//日志
        }

        /// <summary>
        /// 启动所有要启动的程序 ProgramPath：完整路径
        /// </summary>
        private void StartProgram(string ProgramPath)
        {
            try
            {
                string fileName = System.IO.Path.GetFileNameWithoutExtension(ProgramPath);
                if (!IsExistProcess(fileName))
                {
                    ProcessStartInfo startInfo = new ProcessStartInfo(ProgramPath);
                    startInfo.WindowStyle = ProcessWindowStyle.Normal;
                    Process.Start(startInfo);
                    LogWrite("Windows Service: " + fileName + " started.");
                }
            }
            catch (Exception err)
            {
                LogWrite(err.Message);
            }
        }

        /// <summary>
        /// 杀死启动的程序 ProgramPath：完整路径
        /// </summary>
        private void StopProgram(string ProgramPath)
        {
            try
            {
                string fileName = System.IO.Path.GetFileNameWithoutExtension(ProgramPath);
                if (IsExistProcess(fileName))
                {
                    Process[] MyProcesses = Process.GetProcesses();
                    foreach (Process MyProcess in MyProcesses)
                    {
                        if (MyProcess.ProcessName.CompareTo(fileName) == 0)
                        {
                            MyProcess.Kill();
                            break;
                        }
                    }
                }
            }
            catch (Exception err)
            {
                LogWrite(err.Message);
            }
        }

        /// <summary>
        /// 检查该进程是否已启动
        /// </summary>
        /// <param name="processName"></param>
        /// <returns></returns>
        private bool IsExistProcess(string processName)
        {
            Process[] MyProcesses = Process.GetProcesses();
            foreach (Process MyProcess in MyProcesses)
            {
                if (MyProcess.ProcessName.CompareTo(processName) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 写日志
        /// </summary>
        public void LogWrite(string str)
        {
            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(logPath, true))
            {
                sw.WriteLine(DateTime.Now.ToString("[yyyy-MM-dd HH:mm:ss] ") + str);
            }
        }

    }

}
```


##### 测试安装和卸载服务

最终我们的项目是这样的，如下图所示：

![](https://img-blog.csdn.net/20180625160909184?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

debug下面的可执行文件及文件结构（没有的需要放置到此目录下）：

![](https://img-blog.csdn.net/20180625161023380?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


注意：运行的时候要以管理员身份运行，否则安装和卸载都会失败。


源码基本都在上面了，示例debug可执行打包文件：[https://download.csdn.net/download/boonya/10498262](https://download.csdn.net/download/boonya/10498262)

参考文章：



[c#读取xml文件配置文件Winform及WebForm-Demo具体解释](https://www.cnblogs.com/mengfanrong/p/5211720.html)

[C#创建window service服务启动另外的winform](http://www.piaoyi.org/c-sharp/C-window-service-winform.html)

##### Windows服务改进项目重构

可执行文件使用相对路径，并且实现服务可配置化处理。通用性处理，这里对项目进行了更名处理:

![](https://img-blog.csdn.net/20180627103314856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

先看SystemConfig.xml配置：

```
<?xml version="1.0" encoding="utf-8" ?>
<SystemConfig>
  <!--服务名称-->
  <key-value name="WINDOWS_SERVICE_NAME" value="RTVSWindowsService"></key-value>
  <!--服务显示名称-->
  <key-value name="WINDOWS_SERVICE_DISPLAY_NAME" value="RTVSWindowsService"></key-value>
  <!--服务功能描述-->
  <key-value name="WINDOWS_SERVICE_DESCRIBE" value="******科技有限公司转发应用服务(客户端、设备与网关数据转发服务)"></key-value>
  <!--执行exe的名称-->
  <key-value name="EXECUTE_EXE_FULL_NAME" value="RTVS.exe"></key-value>
</SystemConfig>
```

注：如果想要配置文件输出的话需要修改如下属性（bat文件同理）：

![](https://img-blog.csdn.net/20180627105314206?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


修改ProjectInstaller.cs

```
using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Configuration.Install;
using System.Linq;
using System.Threading.Tasks;

namespace RTVSWindowsService
{
    [RunInstaller(true)]
    public partial class ProjectInstaller : System.Configuration.Install.Installer
    {
        const string WINDOWS_SERVICE_NAME = "WINDOWS_SERVICE_NAME";
        const string WINDOWS_SERVICE_DISPLAY_NAME = "WINDOWS_SERVICE_DISPLAY_NAME";
        const string WINDOWS_SERVICE_DESCRIBE = "WINDOWS_SERVICE_DESCRIBE";
        public ProjectInstaller()
        {
            InitializeComponent();
            // 赋予本地系统权限
            this.serviceProcessIntaller1.Account =System.ServiceProcess.ServiceAccount.LocalSystem;
            // 设置服务名称
            this.serviceInstaller1.ServiceName = SystemConfig.GetConfigData(WINDOWS_SERVICE_NAME, "DeveloperWindowsService");
            // 设置服务名称
            this.serviceInstaller1.DisplayName = SystemConfig.GetConfigData(WINDOWS_SERVICE_DISPLAY_NAME, "DeveloperWindowsService");
            // 设置服务名称
            this.serviceInstaller1.Description = SystemConfig.GetConfigData(WINDOWS_SERVICE_DESCRIBE, "DeveloperWindowsService(开发者WindowsService)");
        }
    }
}
```

具体服务的修改WindowsService.cs

```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;

namespace RTVSWindowsService
{
    public partial class WindowsService : ServiceBase
    {
        string service = SystemConfig.GetConfigData("WINDOWS_SERVICE_DISPLAY_NAME", "");

        string exe = SystemConfig.GetConfigData("EXECUTE_EXE_FULL_NAME", "");

        string path = AppDomain.CurrentDomain.BaseDirectory;

        public WindowsService()
        {
            InitializeComponent();
        }

        protected override void OnStart(string[] args)
        {
            LogWrite("Start........");//日志
            LogWrite("CurrentDirectory:" + SystemConfig.GetWorkDirectory());
            LogWrite("BaseDirectory path:" +path);
            DirectoryInfo dictionaryInfo = Directory.GetParent(path);
            LogWrite("ParentDictionay:" + dictionaryInfo.Parent.FullName);
            string exePath = Path.Combine(dictionaryInfo.Parent.FullName, @exe);
            LogWrite("EXE PATH:" + exePath);
            //启动目标应用程序 winform
            StartProgram(exePath); 
            LogWrite("Started!");//日志

        }

        protected override void OnStop()
        {
            LogWrite("Stop........");//日志
            DirectoryInfo dictionaryInfo = Directory.GetParent(path);
            LogWrite("ParentDictionay:" + dictionaryInfo.Parent.FullName);
            string exePath = Path.Combine(dictionaryInfo.Parent.FullName, @exe);
            LogWrite("EXE PATH:" + exePath);
            //杀死目标应用程序 winform
            StopProgram(exePath);
            LogWrite("Stopped!");//日志
        }

        /// <summary>
        /// 启动所有要启动的程序 ProgramPath：完整路径
        /// </summary>
        private void StartProgram(string ProgramPath)
        {
            try
            {
                string fileName = System.IO.Path.GetFileNameWithoutExtension(ProgramPath);
                if (!IsExistProcess(fileName))
                {
                    ProcessStartInfo startInfo = new ProcessStartInfo(ProgramPath);
                    startInfo.WindowStyle = ProcessWindowStyle.Normal;
                    Process.Start(startInfo);
                    LogWrite("Windows Service: "+service +" "+ fileName + " started.");
                }
            }
            catch (Exception err)
            {
                LogWrite(err.Message);
            }
        }

        /// <summary>
        /// 杀死启动的程序 ProgramPath：完整路径
        /// </summary>
        private void StopProgram(string ProgramPath)
        {
            try
            {
                string fileName = System.IO.Path.GetFileNameWithoutExtension(ProgramPath);
                if (IsExistProcess(fileName))
                {
                    Process[] MyProcesses = Process.GetProcesses();
                    foreach (Process MyProcess in MyProcesses)
                    {
                        if (MyProcess.ProcessName.CompareTo(fileName) == 0)
                        {
                            MyProcess.Kill();
                            break;
                        }
                    }
                }
            }
            catch (Exception err)
            {
                LogWrite(err.Message);
            }
        }

        /// <summary>
        /// 检查该进程是否已启动
        /// </summary>
        /// <param name="processName"></param>
        /// <returns></returns>
        private bool IsExistProcess(string processName)
        {
            Process[] MyProcesses = Process.GetProcesses();
            foreach (Process MyProcess in MyProcesses)
            {
                if (MyProcess.ProcessName.CompareTo(processName) == 0)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 写日志
        /// </summary>
        public void LogWrite(string str)
        {
            using (System.IO.StreamWriter sw = new System.IO.StreamWriter(Path.Combine(path, @"StartOrStopLog.log"), true))
            {
                sw.WriteLine(DateTime.Now.ToString("[yyyy-MM-dd HH:mm:ss] ") + str);
            }
        }
    }

}
```

##### Windows服务改进项目GUI实现

GUI有个问题，运行时bat执行需要以“管理员身份运行”，可以让程序申请管理员权限来实现，下一篇文章会讲如何快速申请管理员权限。

![](https://img-blog.csdn.net/20180627110340306?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


GUI服务管理项目结构如下：

![](https://img-blog.csdn.net/20180627105835357?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


服务管理界面设计如下：

![](https://img-blog.csdn.net/20180627105922254?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

GUI管理代码如下：

```
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.ServiceProcess;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using RTVSWindowsService;

namespace RTVS
{
    /// <summary>
    /// RTVS系统服务管理GUI
    /// </summary>
    public partial class FormServiceGUI : Form
    {
        ServiceController serviceController = null;

        string serviceName = SystemConfig.GetConfigServiceData("WINDOWS_SERVICE_DISPLAY_NAME", "RTVSWindowsService");

        // 每次执行的间隔毫秒时长1000ms
        System.Timers.Timer timer = new System.Timers.Timer(1000);

        public FormServiceGUI()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 判断服务是否存在
        /// </summary>
        /// <returns></returns>
        public bool ExistService()
        {
            bool Exits = false;
            ServiceController[] services = ServiceController.GetServices();

            foreach (ServiceController service in services)
            {
                if (service.ServiceName.ToLower().Equals(serviceName.ToLower()))
                {
                    serviceController = service;
                    Exits = true;
                    break;
                }
            }
            return Exits;
        }

        /// <summary>
        /// 安装服务
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnInstall_Click(object sender, EventArgs e)
        {
            if (ExistService())
            {
                MessageBox.Show("服务已经安装，请卸载后进行重装!","操作提示:");
                return;
            }
            ShowState("开始安装服务......");
            try
            {
                string CurrentDirectory = System.Environment.CurrentDirectory;
                System.Environment.CurrentDirectory = CurrentDirectory + "\\Service";
                ShowState("查找服务目录");
                Process process = new Process();
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.FileName = "INSTALL.bat";
                process.StartInfo.CreateNoWindow = true;
                process.Start();
                System.Environment.CurrentDirectory = CurrentDirectory;
                ShowState("服务安装完成!");
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
                if (ex.InnerException != null)
                    msg += ex.InnerException.Message;
                ShowState("安装服务出现错误：" + msg);
            }
        }

        /// <summary>
        /// 卸载服务
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnUninstall_Click(object sender, EventArgs e)
        {
            ShowState("开始卸载服务.......");
            try
            {
                string CurrentDirectory = System.Environment.CurrentDirectory;
                System.Environment.CurrentDirectory = CurrentDirectory + "\\Service";
                Process process = new Process();
                process.StartInfo.UseShellExecute = false;
                process.StartInfo.FileName = "UNINSTALL.bat";
                process.StartInfo.CreateNoWindow = true;
                process.Start();
                System.Environment.CurrentDirectory = CurrentDirectory;
                ShowState("服务卸载完成!");
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
                if (ex.InnerException != null)
                    msg += ex.InnerException.Message;
                ShowState("卸载服务出现错误：" + msg);
            }
        }

        /// <summary>
        /// 启动服务
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnStart_Click(object sender, EventArgs e)
        {
            try
            {

                ShowState("启动服务开始");
                ServiceController serviceController = new ServiceController(serviceName);
                serviceController.Start();
                ShowState("启动服务完成。");
            }
            catch (Exception ex)
            {
                string msg = ex.Message;
                if (ex.InnerException != null)
                    msg += ex.InnerException.Message;
                ShowState("启动服务出现错误：" + msg);
            }
        }

        /// <summary>
        /// 停止服务
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnStop_Click(object sender, EventArgs e)
        {
            try
            {

                ShowState("停止服务开始");
                ServiceController serviceController = new ServiceController(serviceName);
                if (serviceController.CanStop)
                    serviceController.Stop();

                ShowState("停止服务完成。");
            }
            catch (Exception ex)
            {

                string msg = ex.Message;
                if (ex.InnerException != null)
                    msg += ex.InnerException.Message;
                ShowState("停止服务出现错误：" + msg);
            }
        }

        /// <summary>
        /// 清理日志
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void btnClearLog_Click(object sender, EventArgs e)
        {
            this.textBoxMessage.Text = "";
        }

        /// <summary>
        /// 初始化批处理脚本
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FormInstallerService_Load(object sender, EventArgs e)
        {
            string Installbat_content = @"%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\InstallUtil.exe {0}
Net Start " + serviceName +"\n"+
"sc config " + serviceName + " start=AUTO" + "\n" +
"sc config " + serviceName + " type= interact type=own";
            string directory = Path.Combine(System.Environment.CurrentDirectory, "Service");
            File.WriteAllText(Path.Combine(directory, "INSTALL.bat"), string.Format(Installbat_content, Path.Combine(directory, "WindowsService.exe")));

            string Unistallbat_content = @"%SystemRoot%\Microsoft.NET\Framework\v4.0.30319\InstallUtil.exe /u {0}";

            File.WriteAllText(Path.Combine(directory, "UNINSTALL.bat"), string.Format(Unistallbat_content, Path.Combine(directory, "WindowsService.exe")));

           

            ShowState("系统服务管理运行中.........");
            ShowState("系统服务:"+ serviceName+(ExistService() ? "已安装" : "未安装"));
            if (serviceController!=null)
            {
                ShowState("系统服务:" + serviceName + "状态:"+serviceController.Status);
            }
            this.StartPosition = FormStartPosition.CenterScreen;

            //到达时间的时候执行事件
            timer.Elapsed += new System.Timers.ElapsedEventHandler(TimerTask);
            //设置是执行一次（false）还是一直执行(true)
            timer.AutoReset = true;
            //是否执行System.Timers.Timer.Elapsed事件
            timer.Enabled = true;
        }

        public void TimerTask(object source, System.Timers.ElapsedEventArgs e)
        {
            ShowState("系统服务:" + serviceName + (ExistService() ? "已安装,当前状态:" + serviceController.Status:"未安装"));
        }

        /// <summary>
        /// 操作状态
        /// </summary>
        /// <param name="message"></param>
        private void ShowState(string message)
        {
            this.textBoxMessage.AppendText(DateTime.Now.ToString("[yyyy-MM-dd HH:mm:ss] ") +message +"\n");
        }
    }
}
```


服务与GUI的关系：

![](https://img-blog.csdn.net/20180627110026178?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这里我们直接让服务代码输出到Service中：

![](https://img-blog.csdn.net/20180627110201379?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

得到如下图所示输出文件：

![](https://img-blog.csdn.net/20180627110106454?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这样我们的RTVS项目就可以使用windows服务了。服务运行界面如下：

![](https://img-blog.csdn.net/20180627110428182?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


包含服务和可以参考实现的GUI，另外还提供可配置的Windows服务，下载地址：[https://download.csdn.net/download/boonya/10502384](https://download.csdn.net/download/boonya/10502384)




