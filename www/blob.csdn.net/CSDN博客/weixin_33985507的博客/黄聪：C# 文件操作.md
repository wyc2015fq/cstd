# 黄聪：C# 文件操作 - weixin_33985507的博客 - CSDN博客
2010年03月26日 15:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
C# 文件操作
文件操作: 检查 创建 读取 写入 修改 删除
目录操作: 检查 创建 读取 写入 修改 删除
--------------------------------------------------------------------------------
文件操作
若要执行此操作... 请参阅本主题中的示例...
创建文本文件 向文件写入文本 
写入文本文件 向文件写入文本 
读取文本文件 从文件读取文本 
向文件中追加文本 File.AppendText FileInfo.AppendText 
重命名或移动文件 File.Move FileInfo.MoveTo 
删除文件 File.Delete FileInfo.Delete 
复制文件 File.Copy FileInfo.CopyTo 
获取文件大小 FileInfo.Length 
获取文件属性 File.GetAttributes 
设置文件属性 File.SetAttributes 
确定文件是否存在 File.Exists 
读取二进制文件 对刚创建的数据文件进行读取和写入 
写入二进制文件 对刚创建的数据文件进行读取和写入 
检索文件扩展名 Path.GetExtension 
检索文件的完全限定路径 Path.GetFullPath 
检索路径中的文件名和扩展名 Path.GetFileName 
更改文件扩展名 Path.ChangeExtension 
目录操作
System.IO 类
目录操作
string[] drives = Directory. GetLogicalDrives_r(); //本地驱动器的名,如:C:\等
string path = Directory. GetCurrentDirectory_r();  //获取应用程序的当前工作目录
Path. GetFileName_r(@"c:\dir\file.txt");           //获取子目录的名字,result的结果是file.txt
Directory. GetFiles_r(路径及文件名)                //获取指定目录中的文件名(文件列表)
DirectoryInfo di = new DirectoryInfo(@"f:\MyDir"); //构造函数创建目录
DirectoryInfo di=Directory.CreateDirectory(@"f:\bbs"); //创建对象并创建目录
if (di.Exists == false) //检查是否存在此目录
di.Create(); //创建目录
DirectoryInfo dis = di.CreateSubdirectory("SubDir"); //以相对路径创建子目录
dis.Delete(true); //删除刚创建的子目录
di.Delete(true); //删除创建目录
文件操作
Directory.Delete(@"f:\bbs2", true); //删除目录及其子目录和内容(如为假不能删除有内容的目录包括子目录)
Directory.GetDirectories 方法 //获取指定目录中子目录的名称
string[] dirs = Directory. GetDirectories_r(@"f:\", "b*");
Console.WriteLine("此目录中以b开头的子目录共{0}个!", dirs.Length);
foreach (string dir in dirs) { Console.WriteLine(dir); }
Directory.GetFileSystemEntries //获取指定目录中的目录及文件名
Directory.GetLogicalDrives //检索此计算机上格式为“<驱动器号>:\”的逻辑驱动器的名称,【语法同上】
Directory.GetParent //用于检索父目录的路径。
DirectoryInfo a = Directory. GetParent_r(path);
Console.WriteLine(a.FullName);Directory.Move //移动目录及其在内的所有文件
Directory.Move(@"f:\bbs\1", @"f:\bbs\2"); //将文件夹1内的文件剪到文件夹2内 文件夹2是刚创建的
Stream // 对字节的读写操作(包含对异步操作的支持) Reading Writing Seeking
BinaryReader和BinaryWriter // 从字符串或原始数据到各种流之间的读写操作
FileStream类通过Seek()方法进行对文件的随机访问,默认为同步
TextReader和TextWriter //用于gb2312字符的输入和输出
StringReader和StringWriter //在字符串中读写字符
StreamReader和StreamWriter //在流中读写字符
BufferedStream 为诸如网络流的其它流添加缓冲的一种流类型.
MemoryStream 无缓冲的流
NetworkStream 互联网络上的流
//编码转换
Encoding e1 = Encoding.Default;               //取得本页默认代码
Byte[] bytes = e1. GetBytes_r("中国人民解放军"); //转为二进制
string str = Encoding. GetEncoding_r("UTF-8"). GetString_r(bytes); //转回UTF-8编码
--------------------------------------------------------------------------------
//文本文件操作：创建/读取/拷贝/删除
using System;
using System.IO;
class Test
{
   string path = @"f:\t.txt";
   public static void Main()
   {      
      //创建并写入(将覆盖已有文件)
      if (!File.Exists(path))
      {         
         using (StreamWriter sw = File.CreateText(path))
         {
            sw.WriteLine("Hello");
         }
      }
      //读取文件
      using (StreamReader sr = File.OpenText(path))
      {
        string s = "";
        while ((s = sr.ReadLine()) != null)
        {
           Console.WriteLine(s);
        }
     }
     //删除/拷贝
     try
     {
        File.Delete(path);
        File.Copy(path, @"f:\tt.txt");
     }
     catch (Exception e)
     {
        Console.WriteLine("The process failed: {0}", e.ToString());
     }
   }
}
//流文件操作
private const string name = "Test.data";
public static void Main(String[] args)
{
    //打开文件()  ,或通过File创建立如：fs = File.Create(path, 1024)
    FileStream fs = new FileStream(name, FileMode.CreateNew);
    //转换为字节 写入数据(可写入中文)
    Byte[] info = new UTF8Encoding(true). GetBytes_r("This is some text in the file.");
    //字节数组,字节偏移量,最多写入的字节数
    fs.Write(info, 0, info.Length);
    w.Close();
    fs.Close();
    //打开文件
    fs = new FileStream(name, FileMode.Open, FileAccess.Read);
    //读取
    BinaryReader r = new BinaryReader(fs);
    for (int i = 0; i < 11; i++)
    {
        Console.WriteLine(r.ReadInt32());
    }
    w.Close();
    fs.Close();
}
