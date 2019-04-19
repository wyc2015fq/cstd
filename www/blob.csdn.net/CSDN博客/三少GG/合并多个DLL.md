# 合并多个DLL - 三少GG - CSDN博客
2013年03月05日 08:18:30[三少GG](https://me.csdn.net/scut1135)阅读数：1886

### 合并多个DLL,EXE为一个DLL或EXE
有时候自己写个小工具或者其它啥的物件,引用了好多第三方控件,如log4net,aspnetpager啥的,发布出去让别人看到自己竟然用了这么多的第三方DLL,
会对自己的能力产生怀疑,那有什么办法可以解决这种情况嗫?
这就是下面要推荐一个小工具,名称叫"ILMerge",这个东东不开源
使用倒也算简单,下载地址:http://www.microsoft.com/downloads/details.aspx?familyid=22914587-b4ad-4eae-87cf-b14ae6a939b0
使用方法非常简单
在项目属性窗口中,选择"生成事件",在"生成后事件命令行"下的文本框中输入
ilmerge /ndebug /t:dll /log c:\1\DevExpress.Data.v7.3.dll c:\1\DevExpress.OfficeSkins.v7.3.dll c:\1\DevExpress.Utils.v7.3.dll c:\1\log4net.dll /out:c:\1\kkun\Tstring.Core.DLL
然后编译就可以合并多个DLL到一个DLL了,
其中有两个非常重要的参数,更多的参数可以看安装后的帮助文件
/log 后边直接跟你要合并的DLL,多个DLL用空格隔开即可
/out:参数后跟要生成的DLL名称,该DLL可以实际存在,也可以不存在
更多高级技巧还要自己多看帮助,
这里有一些小技巧,例如,ILMerge合并后的DLL将默认使用第一个输入的DLL的AssemblyInfo的声明,
那现在你可以把自己写的DLL放在第一位,
而把类似log4net,aspnetpager等第三方DLL放到后面,这样生成的DLL就是你自己的AssemblyInfo了,
单看程序集别人基本上不会猜到你用了那些第三方控件了!呵呵!
帮助文件下载:[http://files.cnblogs.com/kkun/ILMerge.rar](http://files.cnblogs.com/kkun/ILMerge.rar)
[合并多个DLL或EXE](http://www.cnblogs.com/stangray/archive/2010/07/22/1782846.html)
发现博客上有许多介绍这个的方法,但大家都只是给出了命令行,但是由于引用路径问题,很多都执行不成功.那么我来说一下,使用VS来编译合并,提高成功率
1.首先要去微软的网站下一个ILMerge.exe
网址:http://www.microsoft.com/downloads/details.aspx?familyid=22914587-b4ad-4eae-87cf-b14ae6a939b0&displaylang=en
2.接下来复制到系统目录下\WINNT(2000) 或\WINDOWS(xp,2003)
3.打开你需要合并的项目,(以VS2008为例),在项目上右击Properties->Build Events->Post-build event command line:框中输入(注意是Post那个不是Pre那个)
ilmerge   /out:$(TargetDir)程序名字.exe   $(TargetPath)   $(TargetDir)AjaxPro.2.dll $(TargetDir)Microsoft.Office.Interop.Excel.dll $(TargetDir)RemoteAccess.dll
$(TargetDir):输出路径
$(TargetPath):主程序所在全路径
后面跟多个DLL,以空格分隔如:$(TargetDir)AjaxPro.2.dll
注意输出版本为Release 不是Debug的
# [多个DLL合并成一个DLL，把DLL合并到Exe中的解决方案](http://www.cnblogs.com/haoliansheng/archive/2010/07/08/1773468.html)
1:) 下载 [http://download.microsoft.com/download/1/3/4/1347C99E-9DFB-4252-8F6D-A3129A069F79/ILMerge.msi](http://download.microsoft.com/download/1/3/4/1347C99E-9DFB-4252-8F6D-A3129A069F79/ILMerge.msi)
2:) 开始-》运行 -》 "cmd" -> "cd C:\Program Files\Microsoft\ILMerge"
3:)
3.1、合并file1.dll、file2.dll到destination.dll  
　　　ILmerge /ndebug /target:dll /out:C:\destination.dll /log C:\file1.dll C:\file2.dll 
        注：
        1:) 其中 "/out:C:\destination.dll" 为合并后的 dll目录及名称
        2:) "C:\file1.dll C:\file2.dll "  为要合并的名称
3.2、合并file1.dll、file2.dll以及myApp.exe到newApp.exe  
       ILmerge /ndebug /target:winexe /out:C:\newApp.exe /log C:\myapp.exe C:\file1.dll C:\file2.dll 
       注：
　　 1:) 合并为exe时，myapp.exe 必须写在file1.dll、file2.dll 前面
