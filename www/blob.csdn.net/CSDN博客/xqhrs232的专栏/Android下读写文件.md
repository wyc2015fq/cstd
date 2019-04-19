# Android下读写文件 - xqhrs232的专栏 - CSDN博客
2011年01月07日 13:55:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：783
原文地址::[http://blog.csdn.net/saint_bxg/archive/2009/06/23/4292516.aspx](http://blog.csdn.net/saint_bxg/archive/2009/06/23/4292516.aspx)
初学Android， 初学JAVA，很不专业的找了一下相关资料，很不专业的还愣是没看全明白，还是老大给了个方案，照着做，还好没错。
在网上有看到过说写SD卡的路径要双斜杠，如：//sdcard//t.txt，但我试下来双斜杠，单斜杠都可以。
写两个小函数，以供调用
删除文件函数，输入参数：文件名(全路径) 如 "/sdcard/test.txt"
public boolean DeleteFile(String filename)
  {
   File file;
   file = new File(filename);
   if (file.exists())
    file.delete();
   else
    return false;
   return true;
  }
写文件函数，输入参数：文件名， 输入缓冲首地址，数据长度
  public boolean WriteFile(String filename, char [] str, int length)
     {
      try 
      {
       FileWriter fw = new FileWriter(filename);
       fw.write(str, 0, length);
       fw.flush();
       fw.close();
      }
      catch(IOException e)
      {
       e.printStackTrace();
       return false;
      }
      return true;
     }
 读文件函数，输入参数：文件名， 读取缓冲首地址，数据长度
  public boolean ReadFile(String filename, char [] str, int length)
     {     
      try 
      {
       FileReader fr = new FileReader(filename);
       fr.read(str, 0, length);
       fr.close();
    }
    catch(IOException e)
    {
     e.printStackTrace();   
     return false;
    }
    return true;
     }
在2.1上运行时，也碰到了无法写SD卡的问题（下面有为兄弟也问过），原因是SDK 1.6及以后版本 新增了对SD卡写入权限, 在
在AndroidManifest.xml加入以下代码
<uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE"></uses-permission>
SDK1.6之前的版本无需此权限也可以写入SD卡。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/saint_bxg/archive/2009/06/23/4292516.aspx](http://blog.csdn.net/saint_bxg/archive/2009/06/23/4292516.aspx)
