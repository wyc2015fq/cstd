# C#程序验证系统登录用户与目录权限 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年05月15日 09:52:21[boonya](https://me.csdn.net/boonya)阅读数：542








Windows用户类型：Administrator、Guest、自定义用户等；文件的权限不外乎：Read、Write、Modify、Remove等等，其中拥有所有权限的字符叫做FullControl。


####   C#当前程序用户路径权限验证

```
/// <summary>
        /// 是否拥有程序数据路径权限
        /// </summary>
        private static  bool HasProgramDataRights=false;

        public static bool IsAdministrator()
        {
            System.Security.Principal.WindowsIdentity identity = System.Security.Principal.WindowsIdentity.GetCurrent();
            System.Security.Principal.WindowsPrincipal principal = new System.Security.Principal.WindowsPrincipal(identity);
            return principal.IsInRole(System.Security.Principal.WindowsBuiltInRole.Administrator);
        }

        /// <summary>
        /// 读取系统程序数据路径权限&判断是否可以使用临时目录进行保存操作
        /// </summary>
        /// <param name="path"></param>
        public  void ReadProgramDataRights(string path)
        {
            // 通过判定文件的权限来控制路径
            System.Security.AccessControl.DirectorySecurity dirSec = new System.IO.DirectoryInfo(path).GetAccessControl();
            System.Security.AccessControl.AuthorizationRuleCollection rules = dirSec.GetAccessRules(true, true, typeof(System.Security.Principal.NTAccount));
            foreach (System.Security.AccessControl.FileSystemAccessRule rule in rules)
            {
                Console.WriteLine(path + "文件目录权限字符:" + rule.FileSystemRights);
                // 判定文件是否具有FullControl或Write权限
                if (rule.FileSystemRights.ToString().Equals("FullControl") || rule.FileSystemRights.ToString().Equals("Write"))
                {
                    HasProgramDataRights = true;
                    break;
                }
            }
            // 为IE保护模式创建临时保存路径
            if (HasProgramDataRights)
            {
                // 创建操作路径
                string recordTemp = FileHelp.GetAppDataLocalLow() + "\\CvNetVideo\\Record\\";
                string screenshotTemp = FileHelp.GetAppDataLocalLow() + "\\CvNetVideo\\Screenshot\\";
                string recordDst = FileHelp.GetCommonApplicationData() + "\\CvNetVideo\\Record\\";
                string screenshotDst = FileHelp.GetCommonApplicationData() + "\\CvNetVideo\\Screenshot\\";
                Directory.CreateDirectory(recordTemp);
                Directory.CreateDirectory(screenshotTemp);
                Directory.CreateDirectory(recordDst);
                Directory.CreateDirectory(screenshotDst);
            }
        }
```

#### C#文件操作工具类

```
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
            return GetLocalApplicationData()+ "\\Microsoft\\Windows\\Temporary Internet Files\\Low";
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
```
注：如果您是开发OCX遇到路径问题请使用工具类中IE保护模式下的低权限路径。



