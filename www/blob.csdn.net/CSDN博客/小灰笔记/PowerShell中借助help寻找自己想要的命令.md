# PowerShell中借助help寻找自己想要的命令 - 小灰笔记 - CSDN博客





2017年04月09日 12:54:57[grey_csdn](https://me.csdn.net/grey_csdn)阅读数：433








       Help不仅仅可以用于查询命令的帮助信息，也可以借助于help命令搜索自己并不熟悉的指令。比如，我想找关于HTML的命令，可以通过help加关键字和通配符组合为参数用于查看：

使用示范如下：

PSC:\Windows\system32> help *HTML*



名称

    ConvertTo-Html



语法

    ConvertTo-Html [[-Property]<Object[]>] [[-Head] <string[]>] [[-Title] <string>] [[-Body]<string[]>]  [<CommonPara

    meters>]



    ConvertTo-Html [[-Property]<Object[]>] [<CommonParameters>]





别名

    无





备注

    Get-Help 在此计算机上找不到该cmdlet 的帮助文件。它仅显示部分帮助。

        -- 若要下载并安装包含此cmdlet 的模块的帮助文件，请使用 Update-Help。

        -- 若要联机查看此cmdlet 的帮助主题，请键入: "Get-Help ConvertTo-Html-Online" 或

           转到 [http://go.microsoft.com/fwlink/?LinkID=113290](http://go.microsoft.com/fwlink/?LinkID=113290)。



       通过以上结果，我们找到了一个关于HTML的命令。这个命令可以把其它Cmdlet命令的输出结果转换为HTML格式。



