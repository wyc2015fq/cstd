# Windows中的文件查找命令-where - 小灰笔记 - CSDN博客





2017年03月31日 23:27:07[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：9485








       一直以来，查找某个windows下的命令或者可执行程序我都是借助于一个免安装版本的bash命令which。这样是比较方面的，为了功能更强大一点也可以去安装一下Cygwin。不过，就这么一个命令的使用在公司基本上也是一种小奢侈，因为现在公司办公电脑环境非常不自由，很多时候我没法选择自己喜欢的工具。

       不过，作为一个能够折腾的人，我想了很多替代性的方法。比如，很多常用的功能我自己用Perl或者Python实现一个类似的脚本或者程序。其实，这方面Perl或者Python应该说已经非常强大，惭愧的是我学的都不是很深。

       今天在折腾powershell的时候没事儿查看Windows系统安装目录中的一个文件，看到一个where程序。不知道是干什么用的，于是简单运行了一下试试。结果有一点欣喜，基本上能够完成我依赖于which的功能，同时还能提供文件查找的功能。

       等效的which示范：

C:\Users\ThinkPad\Desktop>whereperl

C:\Strawberry\perl\bin\perl.exe



C:\Users\ThinkPad\Desktop>wherepython

e:\da_workspace\User\Scripts\python.exe

E:\da_workspace\User\python.exe

       其实上面的结果有点让我意外，我的电脑上有两个Python解析器？

查看where的帮助信息：

C:\Users\ThinkPad\Desktop>where/?



WHERE [/R dir][/Q] [/F] [/T] pattern...



描述:

    显示符合搜索模式的文件位置。在默认情况下，搜索是在当前目录和 PATH

    环境变量指定的路径中执行的。



参数列表:

    /R      从指定目录开始，递归性搜索并显示符合指定模式的文件。



    /Q      只返回退出代码，不显示匹配文件列表。(安静模式)



             匹配文件。(安静模式)



    /F      显示所有相配文件并用双引号括上。



    /T      显示所有相配文件的文件的文件。



    pattern 指定要匹配的文件的搜索模式。通配符 * 和 ? 可以用在模式中。

             也可以指定"$env:pattern" 和 "path:pattern" 格式; 其中

             "env" 是环境变量，搜索是在 "env" 变量的指定的路径中执行的。

             这些格式不应该跟/R 一起使用。此搜索也可以用将 PATHEXT 变

             量扩展名附加于此模式的方式完成。



     /?     显示此帮助消息。



  注意: 如果搜索成功，此工具返回错误级别 0; 如果不成功，返回 1; 如果失

        败或发生错误，返回 2。



示例:

    WHERE /?

    WHERE myfilename1 myfile????.*

    WHERE $windir:*.*

    WHERE /R c:\windows *.exe *.dll *.bat

    WHERE /Q ??.???

    WHERE"c:\windows;c:\windows\system32:*.dll"

WHERE /F /T *.dll

看起来上面的功能中，我可能会用得到的有一个指定目录的搜索功能。这个功能能够启动递归式搜索，能够搜索一个文件目录及其子目录中的被搜索对象信息。例如，在D盘中寻找是否有javac.exe程序，搜索命令以及执行结果如下：

C:\Users\ThinkPad\Desktop>where /R D:\ javac.exe

D:\software\Java\jdk1.8.0_121\bin\javac.exe

       程序执行的速度还是挺快的，看起来程序的表现还不错。这样看来，虽说是本不值得去深入学习的Windows批处理还是有必要好好学习下的，除非换个公司换个工作更为自由一点的环境，在一个开发工具可以自由选择的地方工作这种替代性手段倒真是没有必要了。



