# PowerShell中查看命令帮助 - 小灰笔记 - CSDN博客





2017年04月09日 12:24:16[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：1248







之前也学习了几个批处理命令，跟之前的同事学到了一个机巧，使用/?参数可以查看命令的帮助信息。具体的使用示范如下：

C:\Users\ThinkPad>where /?



WHERE [/R dir] [/Q] [/F] [/T] pattern...



描述:

    显示符合搜索模式的文件位置。在默认情况下，搜索是在当前目录和 PATH

    环境变量指定的路径中执行的。



参数列表:

    /R       从指定目录开始，递归性搜索并显示符合指定模式的文件。



    /Q       只返回退出代码，不显示匹配文件列表。(安静模式)



             匹配文件。(安静模式)



    /F       显示所有相配文件并用双引号括上。



    /T       显示所有相配文件的文件的文件。



    pattern  指定要匹配的文件的搜索模式。通配符 * 和 ? 可以用在模式中。

             也可以指定 "$env:pattern" 和 "path:pattern" 格式; 其中

             "env" 是环境变量，搜索是在 "env" 变量的指定的路径中执行的。

             这些格式不应该跟 /R 一起使用。此搜索也可以用将 PATHEXT 变

             量扩展名附加于此模式的方式完成。



     /?      显示此帮助消息。



  注意: 如果搜索成功，此工具返回错误级别 0; 如果不成功，返回 1; 如果失

        败或发生错误，返回 2。



示例:

    WHERE /?

    WHERE myfilename1 myfile????.*

    WHERE $windir:*.*

    WHERE /R c:\windows *.exe *.dll *.bat

    WHERE /Q ??.???

    WHERE "c:\windows;c:\windows\system32:*.dll"

    WHERE /F /T *.dllC:\Users\ThinkPad>where /?



WHERE [/R dir] [/Q] [/F] [/T] pattern...



描述:

    显示符合搜索模式的文件位置。在默认情况下，搜索是在当前目录和 PATH

    环境变量指定的路径中执行的。



参数列表:

    /R       从指定目录开始，递归性搜索并显示符合指定模式的文件。



    /Q       只返回退出代码，不显示匹配文件列表。(安静模式)



             匹配文件。(安静模式)



    /F       显示所有相配文件并用双引号括上。



    /T       显示所有相配文件的文件的文件。



    pattern  指定要匹配的文件的搜索模式。通配符 * 和 ? 可以用在模式中。

             也可以指定 "$env:pattern" 和 "path:pattern" 格式; 其中

             "env" 是环境变量，搜索是在 "env" 变量的指定的路径中执行的。

             这些格式不应该跟 /R 一起使用。此搜索也可以用将 PATHEXT 变

             量扩展名附加于此模式的方式完成。



     /?      显示此帮助消息。



  注意: 如果搜索成功，此工具返回错误级别 0; 如果不成功，返回 1; 如果失

        败或发生错误，返回 2。



示例:

    WHERE /?

    WHERE myfilename1 myfile????.*

    WHERE $windir:*.*

    WHERE /R c:\windows *.exe *.dll *.bat

    WHERE /Q ??.???

    WHERE "c:\windows;c:\windows\system32:*.dll"

WHERE /F /T *.dll
其实，这个命令在PowerShell中也是存在的。不过，临时我还是有点分不清批处理以及PowerShell的命令差异。而且，有时候两个控制台上同样的命令操作有着不同的执行表现。比如说where的PowerShell操作如下：

PS C:\Users\ThinkPad> where /?

PS C:\Users\ThinkPad>
运行没有报错，但是却没有任何输出。对于其他的命令，也有一个可以一个报错的现象，比如说cd。

Cmd中的执行记录：

C:\Users\ThinkPad>cd /?

显示当前目录名或改变当前目录。



CHDIR [/D] [drive:][path]

CHDIR [..]

CD [/D] [drive:][path]

CD [..]



  ..   指定要改成父目录。



键入 CD drive: 显示指定驱动器中的当前目录。

不带参数只键入 CD，则显示当前驱动器和目录。



使用 /D 开关，除了改变驱动器的当前目录之外，

还可改变当前驱动器。



如果命令扩展被启用，CHDIR 会如下改变:



当前的目录字符串会被转换成使用磁盘名上的大小写。所以，

如果磁盘上的大小写如此，CD C:\TEMP 会将当前目录设为

C:\Temp。



CHDIR 命令不把空格当作分隔符，因此有可能将目录名改为一个

带有空格但不带有引号的子目录名。例如:



     cd \winnt\profiles\username\programs\start menu



与下列相同:



     cd "\winnt\profiles\username\programs\start menu"



在扩展停用的情况下，你必须键入以上命令。

PowerShell中的运行记录：



PS C:\Users\ThinkPad> cd /?

cd : 找不到路径“/?”，因为该路径不存在。

所在位置 行:1 字符: 1

+ cd /?

+ ~~~~~

    + CategoryInfo          : ObjectNotFound: (/?:String) [Set-Location], ItemNotFoundException

+ FullyQualifiedErrorId : PathNotFound,Microsoft.PowerShell.Commands.SetLocationCommand

同样的操作居然出现了错误，这就否定了我之前的一种想法。其实，PowerShell并不是一个加强版的批处理平台。

其实，除了这个简单的命令参数方式，还有两种方法：一种是使用help命令，另一种是使用man。两种方法也是在不同平台上有着不同的表现，尤其是help。因为man指令比较容易区分，这个只在PowerShell中存在。

PowerShell中的man：

PS C:\Users\ThinkPad> man cd



名称

    Set-Location



语法

    Set-Location [[-Path] <string>]  [<CommonParameters>]



    Set-Location  [<CommonParameters>]



    Set-Location  [<CommonParameters>]





别名

    sl

    cd

    chdir





备注

    Get-Help 在此计算机上找不到该 cmdlet 的帮助文件。它仅显示部分帮助。

        -- 若要下载并安装包含此 cmdlet 的模块的帮助文件，请使用 Update-Help。

        -- 若要联机查看此 cmdlet 的帮助主题，请键入: "Get-Help Set-Location -Online" 或

           转到 http://go.microsoft.com/fwlink/?LinkID=113397。

CMD模式下的man：

C:\Users\ThinkPad>man cd

'man' 不是内部或外部命令，也不是可运行的程序

或批处理文件。
在能够深入了解整个PowerShell之前，需要查命令的时候只能是几种方法全都试一下，好在查找帮助并不会对日常的使用带来多大的影响。
            


