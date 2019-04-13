
# C\#使用System.IO.Path获取文件路径、文件名 - 致林 - 博客园






# [C\#使用System.IO.Path获取文件路径、文件名](https://www.cnblogs.com/bincoding/p/8116596.html)

classProgram
    {staticvoidMain(string[] args)
        {//获取当前运行程序的目录stringfileDir =Environment.CurrentDirectory;
            Console.WriteLine("当前程序目录："+fileDir);//一个文件目录stringfilePath ="C:\\bin\\files\\test.xml";
            Console.WriteLine("该文件的目录："+filePath);stringstr ="获取文件的全路径："+ Path.GetFullPath(filePath);//-->C:\bin\files\test.xmlConsole.WriteLine(str);
            str="获取文件所在的目录："+ Path.GetDirectoryName(filePath);//-->C:\bin\filesConsole.WriteLine(str);
            str="获取文件的名称含有后缀："+ Path.GetFileName(filePath);//-->test.xmlConsole.WriteLine(str);
            str="获取文件的名称没有后缀："+ Path.GetFileNameWithoutExtension(filePath);//-->testConsole.WriteLine(str);
            str="获取路径的后缀扩展名称："+ Path.GetExtension(filePath);//-->.xmlConsole.WriteLine(str);
            str="获取路径的根目录："+ Path.GetPathRoot(filePath);//-->C:\Console.WriteLine(str);
            Console.ReadKey();
        }
    }

|说明|
|[ChangeExtension](https://msdn.microsoft.com/zh-cn/library/system.io.path.changeextension(v=vs.110).aspx)|(String, String)|更改路径字符串的扩展名。
|
|[Combine](https://msdn.microsoft.com/zh-cn/library/fyy7a5kt(v=vs.110).aspx)|(String, String)|将两个字符串组合成一个路径。
|
|[Combine](https://msdn.microsoft.com/zh-cn/library/dd784047(v=vs.110).aspx)|(String, String, String)|将三个字符串组合成一个路径。
|
|[Combine](https://msdn.microsoft.com/zh-cn/library/dd782933(v=vs.110).aspx)|(String, String, String, String)|将四个字符串组合成一个路径。
|
|[Combine](https://msdn.microsoft.com/zh-cn/library/dd991142(v=vs.110).aspx)|(String[])|将字符串数组组合成一个路径。
|
|[GetDirectoryName](https://msdn.microsoft.com/zh-cn/library/system.io.path.getdirectoryname(v=vs.110).aspx)|(String)|返回指定路径字符串的目录信息。
|
|[GetExtension](https://msdn.microsoft.com/zh-cn/library/system.io.path.getextension(v=vs.110).aspx)|(String)|返回指定的路径字符串的扩展名。
|
|[GetFileName](https://msdn.microsoft.com/zh-cn/library/system.io.path.getfilename(v=vs.110).aspx)|(String)|返回指定路径字符串的文件名和扩展名。
|
|[GetFileNameWithoutExtension](https://msdn.microsoft.com/zh-cn/library/system.io.path.getfilenamewithoutextension(v=vs.110).aspx)|(String)|返回不具有扩展名的指定路径字符串的文件名。
|
|[GetFullPath](https://msdn.microsoft.com/zh-cn/library/system.io.path.getfullpath(v=vs.110).aspx)|(String)|返回指定路径字符串的绝对路径。
|
|[GetInvalidFileNameChars](https://msdn.microsoft.com/zh-cn/library/system.io.path.getinvalidfilenamechars(v=vs.110).aspx)|()|获取包含不允许在文件名中使用的字符的数组。
|
|[GetInvalidPathChars](https://msdn.microsoft.com/zh-cn/library/system.io.path.getinvalidpathchars(v=vs.110).aspx)|()|获取包含不允许在路径名中使用的字符的数组。
|
|[GetPathRoot](https://msdn.microsoft.com/zh-cn/library/system.io.path.getpathroot(v=vs.110).aspx)|(String)|获取指定路径的根目录信息。
|
|[GetRandomFileName](https://msdn.microsoft.com/zh-cn/library/system.io.path.getrandomfilename(v=vs.110).aspx)|()|返回随机文件夹名或文件名。
|
|[GetTempFileName](https://msdn.microsoft.com/zh-cn/library/system.io.path.gettempfilename(v=vs.110).aspx)|()|在磁盘上创建磁唯一命名的零字节的临时文件并返回该文件的完整路径。
|
|[GetTempPath](https://msdn.microsoft.com/zh-cn/library/system.io.path.gettemppath(v=vs.110).aspx)|()|返回当前用户的临时文件夹的路径。
|
|[HasExtension](https://msdn.microsoft.com/zh-cn/library/system.io.path.hasextension(v=vs.110).aspx)|(String)|确定路径是否包括文件扩展名。
|
|[IsPathRooted](https://msdn.microsoft.com/zh-cn/library/system.io.path.ispathrooted(v=vs.110).aspx)|(String)|获取一个值，该值指示指定的路径字符串是否包含根。
|

字段

|名称|说明|
|[AltDirectorySeparatorChar](https://msdn.microsoft.com/zh-cn/library/system.io.path.altdirectoryseparatorchar(v=vs.110).aspx)|提供平台特定的替换字符，该替换字符用于在反映分层文件系统组织的路径字符串中分隔目录级别。
|
|[DirectorySeparatorChar](https://msdn.microsoft.com/zh-cn/library/system.io.path.directoryseparatorchar(v=vs.110).aspx)|提供平台特定的字符，该字符用于在反映分层文件系统组织的路径字符串中分隔目录级别。
|
|[InvalidPathChars](https://msdn.microsoft.com/zh-cn/library/system.io.path.invalidpathchars(v=vs.110).aspx)|已过时。|提供平台特定的字符数组，这些字符不能在传递到|Path 类的成员的路径字符串参数中指定。
|
|[PathSeparator](https://msdn.microsoft.com/zh-cn/library/system.io.path.pathseparator(v=vs.110).aspx)|用于在环境变量中分隔路径字符串的平台特定的分隔符。
|
|[VolumeSeparatorChar](https://msdn.microsoft.com/zh-cn/library/system.io.path.volumeseparatorchar(v=vs.110).aspx)|提供平台特定的卷分隔符。
|


参考：https://msdn.microsoft.com/zh-cn/library/system.io.path(v=vs.110).aspx





