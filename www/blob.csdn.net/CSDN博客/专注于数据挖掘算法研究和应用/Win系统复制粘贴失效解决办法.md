# Win系统复制粘贴失效解决办法 - 专注于数据挖掘算法研究和应用 - CSDN博客





2013年05月13日 09:07:45[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：6279








问题描述：Win系统下复制粘贴失效，表现为复制了，但是粘贴却失败，无论是快捷键还是右键菜单。可能复制事件捕捉失败，所以粘贴下去就不是新复制的内容而是之前复制的内容！

解决办法：重新注册以下DLL文件。 

        regsvr32 Shdocvw.dll


        regsvr32 Shell32.dll


        regsvr32 Oleaut32.dll


        regsvr32 Actxprxy.dll


        regsvr32 Mshtml.dll


        regsvr32 Urlmon.dll


        执行命令后重新启动windows。



