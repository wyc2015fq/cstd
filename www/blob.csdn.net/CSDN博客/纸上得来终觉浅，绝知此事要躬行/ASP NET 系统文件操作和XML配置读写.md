# ASP.NET 系统文件操作和XML配置读写 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月26日 17:28:54[boonya](https://me.csdn.net/boonya)阅读数：157








这里将工作中用到的两个工具分享一下：（1）、系统文件操作工具（2）、XML读写配置文件工具。

**目录**

[文件操作工具](#%E6%96%87%E4%BB%B6%E6%93%8D%E4%BD%9C%E5%B7%A5%E5%85%B7)

[XML配置文件读写](#XML%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E8%AF%BB%E5%86%99)

[XML配置文件读取示例](#XML%E9%85%8D%E7%BD%AE%E6%96%87%E4%BB%B6%E8%AF%BB%E5%8F%96%E7%A4%BA%E4%BE%8B)

### 文件操作工具

```
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace Base
{
    public class FileHelp
    {
        public enum GetDirectoryType
        {
            ByAppDomain,
            ByAssembly
        }
        public static string GetCurrentDirectory(GetDirectoryType type = GetDirectoryType.ByAppDomain)
        {
            switch (type)
            {
                case GetDirectoryType.ByAppDomain:
                    return AppDomain.CurrentDomain.BaseDirectory;
                case GetDirectoryType.ByAssembly:

                    return Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location);
                default:
                    return AppDomain.CurrentDomain.BaseDirectory;
            }
        }
        public static string GetCurrentDirectoryByAssembly()
        {
            return GetCurrentDirectory(GetDirectoryType.ByAssembly);
        }
        /// <summary>
        /// 获取配置文件路径 优先取环境变量中MyDataPath值 如果找不到则取当前路径
        /// </summary>
        /// <returns></returns>
        public static string GetMyConfPath()
        {
            var path = Environment.GetEnvironmentVariable("MyDataPath");
            if (string.IsNullOrWhiteSpace(path))
            {
                path = GetCurrentDirectory();
            }
            if (!path.EndsWith("/") && !path.EndsWith("\\"))
            {
                path += "/";
            }
            return path;
        }
        /// <summary>
        ///程序数据路径- C:\ProgramData
        /// </summary>
        /// <returns></returns>
        public static string GetCommonApplicationData()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.CommonApplicationData);
        }

        /// <summary>
        /// 用户数据路径
        /// </summary>
        /// <returns></returns>
        public static string GetApplicationData()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData);
        }

        /// <summary>
        /// 用户数据本地路径
        /// </summary>
        /// <returns></returns>
        public static string GetLocalApplicationData()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData);
        }

        /// <summary>
        /// 用户路径
        /// </summary>
        /// <returns></returns>
        public static string GetUserProfile()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.UserProfile);
        }

        /// <summary>
        /// 用户的图片路径
        /// </summary>
        /// <returns></returns>
        public static string GetMyPictures()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.MyPictures);
        }

        /// <summary>
        /// 用户的视频路径
        /// </summary>
        /// <returns></returns>
        public static string GetMyVideos()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.MyVideos);
        }

        /// <summary>
        /// 用户的文档路径
        /// </summary>
        /// <returns></returns>
        public static string GetMyDocuments()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.MyDocuments);
        }

        /// <summary>
        /// IE保护模式下的低权限操作路径(Temporary Internet Files/Low)
        /// 参考：https://blog.csdn.net/xt_xiaotian/article/details/5336809
        /// </summary>
        /// <returns></returns>
        public static string GetTemporaryInternetFiles()
        {
            return GetLocalApplicationData() + "\\Microsoft\\Windows\\Temporary Internet Files\\Low";
        }

        /// <summary>
        /// IE保护模式下的低权限操作路径(%userprofile%/AppData/LocalLow)
        /// 参考：https://blog.csdn.net/xt_xiaotian/article/details/5336809
        /// </summary>
        /// <returns></returns>
        public static string GetAppDataLocalLow()
        {
            return GetUserProfile() + "\\AppData\\LocalLow";
        }

        /// <summary>
        /// 获取系统字体文件路径
        /// </summary>
        /// <returns></returns>
        public static string GetFonts()
        {
            return Environment.GetFolderPath(Environment.SpecialFolder.Fonts);
        }

        /// <summary>
        /// 二进制文件读取
        /// </summary>
        /// <param name="FileUrl">文件路径</param>
        /// <returns></returns>
        public static byte[] BinaryRead(string FileUrl)
        {
            List<byte> lst = new List<byte>();
            try
            {
                //文件路径
                String filename = FileUrl;
                //打开文件
                FileStream FStream;
                if (File.Exists(filename))
                {
                    FStream = new FileStream(filename, FileMode.Open);
                }
                else
                {
                    return null;
                }
                int BufferSize = 1048576; //每次读取的字节数 每次读取1MB
                byte[] Buffer = new byte[BufferSize];
                long FileLength = FStream.Length;//文件流的长度
                int ReadCount = (int)(FileLength / BufferSize) + 1; //需要对文件读取的次数
                                                                    //数据读取
                BinaryReader BWrite = new BinaryReader(FStream);
                //br.BaseStream.Seek(0, SeekOrigin.Begin);
                //while (br.BaseStream.Position < br.BaseStream.Length){}
                for (int a = 0; a < ReadCount; a++)
                {
                    Buffer = BWrite.ReadBytes(BufferSize);
                    lst.AddRange(Buffer);
                }
                BWrite.Close();
                BWrite.Close();
                return lst.ToArray();
            }
            catch (System.Exception ex)
            {
                Log.WriteLog4Ex("FileHelp.BinaryRead", ex);
                return null;
            }
        }

        /// <summary>
        /// 二进制文件写入
        /// </summary>
        /// <param name="Bts"></param>
        /// <param name="DirectoryUrl">文件目录路径</param>
        /// <param name="FileName">文件名称</param>
        /// <returns></returns>
        public static bool BinaryWrite(byte[] Bts, string DirectoryUrl, string FileName)
        {
            try
            {
                //文件路径
                string Filepath = DirectoryUrl + "\\" + FileName;
                //目录创建
                if (!Directory.Exists(DirectoryUrl))
                    Directory.CreateDirectory(DirectoryUrl);
                //文件创建
                FileStream FStream;
                if (File.Exists(Filepath))
                {
                    FStream = new FileStream(Filepath, FileMode.Append);
                }
                else
                {
                    FStream = new FileStream(Filepath, FileMode.Create);
                }
                //数据写入
                BinaryWriter BWrite = new BinaryWriter(FStream);
                BWrite.Write(Bts);
                BWrite.Close();
                FStream.Close();
                return true;
            }
            catch (System.Exception ex)
            {
                Log.WriteLog4Ex("FileHelp.BinaryWrite", ex);
                return false;
            }
        }

        /// <summary>
        /// 二进制文件删除
        /// </summary>
        /// <param name="FileUrl">文件路径</param>
        public static void FileDelete(string FileUrl)
        {
            try
            {
                //文件路径
                String filename = FileUrl;
                //删除文件
                if (File.Exists(filename))
                {
                    File.Delete(filename);
                }
            }
            catch (System.Exception ex)
            {
                Log.WriteLog4Ex("FileHelp.FileDelete", ex);
            }
        }


    }
}
```

### XML配置文件读写

泛型是具体的配置文件属性

```
using RTVSWeb.Models;
using Base;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace RTVSWeb.Utils
{
    /// <summary>
    /// 配置文件读取工具类
    /// </summary>
    public class ConfigReader<T>
    {
        private string XMLmlFile;

        public T Config;

        /// <summary>
        /// 构造函数
        /// </summary>
        /// <param name="XMLmlFile">如：VersionConfig.xml</param>
        public ConfigReader(string XMLmlFile)
        {
            this.XMLmlFile= XMLmlFile;
        }


        /// <summary>
        /// 读取配置信息
        /// </summary>
        /// <returns></returns>
        public bool ReadConfig()
        {
            try
            {
                Config = SerializableHelper.DeserializeSetting<T>(FileHelp.GetMyConfPath() + XMLmlFile);
                return true;
            }
            catch (Exception ex)
            {
                Log.WriteLog4("ReadConfig:" + ex.ToString(), LOGTYPE.ERRORD);
                return false;
            }
        }
        /// <summary>
        /// 保存配置信息
        /// </summary>
        /// <returns></returns>
        public bool SaveConfig(T Config)
        {
            try
            {
                SerializableHelper.SerializeSetting<T>(Config, FileHelp.GetMyConfPath() + XMLmlFile);
                return true;
            }
            catch (Exception ex)
            {
                Log.WriteLog4("SaveConfig:" + ex.ToString(), LOGTYPE.ERRORD);
                return false;
            }
        }
    }
}
```

### XML配置文件读取示例

定义一个属性类：

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace RTVSWeb.Models
{
    /// <summary>
    /// 版本管理配置 
    /// </summary>
    public class VersionConfig
    {
        /// <summary>
        /// 构造函数初始化
        /// </summary>
        public VersionConfig()
        {
            VersionFirst = 1;
            VersionSecond = 0;
            VersionThird = 0;
            UpgradeTag = "VersionThird";
            CurrentVersion = "1.0.0";
        }
        /// <summary>
        /// 一级版本号
        /// </summary>
        public int VersionFirst { get; set; }


        /// <summary>
        /// 二级版本号
        /// </summary>
        public int VersionSecond { get; set; }

        /// <summary>
        /// 三级版本号
        /// </summary>
        public int VersionThird { get; set; }

        /// <summary>
        /// 升级标签(VersionFirst|VersionSecond|VersionThird)
        /// </summary>
        public string UpgradeTag { get; set; }

        /// <summary>
        /// 当前版本号
        /// </summary>
        public string CurrentVersion { get; set; }
    }
}
```

读取配置：


```
ConfigReader reader = new ConfigReader(“VersionConfig.xml”);

 // 读取版本配置文件
 if (!reader.ReadConfig())
{
       reader.Config = new Models.VersionConfig();
       reader.SaveConfig();
}
```

不存在时输出如下内容：

```
<?xml version="1.0" encoding="utf-8"?>
<VersionConfig xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <VersionFirst>1</VersionFirst>
  <VersionSecond>0</VersionSecond>
  <VersionThird>0</VersionThird>
  <UpgradeTag>VersionThird</UpgradeTag>
  <CurrentVersion>1.0.0</CurrentVersion>
</VersionConfig>
```





