# C#项目的生成事件及批处理文件 - 左直拳的马桶_日用桶 - CSDN博客
2015年02月05日 22:07:35[左直拳](https://me.csdn.net/leftfist)阅读数：3547
一个C#项目，如果为同一个解决方案的其他项目所引用，则其编译后，会将DLL拷贝到引用项目中；但如果它并不被其他项目引用，但又想编译后能够自动将生成的东西拷贝过去，可以在项目的生成事件中，写上一些批处理命令，拷贝指定的内容。
项目的生成事件，可以右击项目，弹出菜单中选属性，即可见到。
下面是一个批处理示例：
```
SET Area=Test
SET SolutionDir=D:\svn\trunk\Code\Solution\
SET ProjectName=SubWeb
set PluginDirectory="%SolutionDir%Web\Plugins\%Area%\"
set PluginViewDirectory="%SolutionDir%Web\Plugins\%Area%\Views"
set PluginBinDirectory="%SolutionDir%Web\Plugins\%Area%\bin"
set PluginContentDirectory="%SolutionDir%Web\Plugins\%Area%\Content"
set PluginScirptsDirectory="%SolutionDir%Web\Plugins\%Area%\Scripts"
set PluginWebConfig="%SolutionDir%Web\Plugins\%Area%\"
set PluginTemplateDirectory="%SolutionDir%Web\Plugins\%Area%\Template"
set PluginFunctionXmlFile="%SolutionDir%Web\Plugins\%Area%\"
set ProjectViewDirectory="%SolutionDir%%ProjectName%\Views"
set ProjectBinDirectory="%SolutionDir%%ProjectName%\bin"
set ProjectContentDirectory="%SolutionDir%%ProjectName%\Content"
set ProjectScirptsDirectory="%SolutionDir%%ProjectName%\Scripts"
set ProjectWebConfig="%SolutionDir%%ProjectName%\Web.config"
set ProjectTemplateDirectory="%SolutionDir%%ProjectName%\Template"
set ProjectFunctionXmlFile="%SolutionDir%%ProjectName%\menu.xml"
C:\Windows\System32\xcopy.exe /s /y %ProjectViewDirectory%\*.cshtml %PluginViewDirectory%
rem C:\Windows\System32\xcopy.exe /s /y %ProjectBinDirectory% %PluginBinDirectory%
rem C:\Windows\System32\xcopy.exe /s /y %ProjectScirptsDirectory% %PluginScirptsDirectory%
rem C:\Windows\System32\xcopy.exe /s /y %ProjectWebConfig% %PluginWebConfig%
rem C:\Windows\System32\xcopy.exe /s /y %ProjectTemplateDirectory% %PluginTemplateDirectory%
rem C:\Windows\System32\xcopy.exe /s /y %ProjectFunctionXmlFile% %PluginFunctionXmlFile%
```
这样，每次这个项目编译的时候，之后就会触发这个事件。
我们也可以将这些批处理命令写到一个批处理文件，然后在VS里添加一个指向该BAT的外部命令。这样在VS里就可以随时点击触发，方便得很。
这种方法尤其适合只拷贝一些非DLL等无须编译的文件。
添加外部命令的方法是在VS中，工具-外部工具，编辑好后，命令会自动出现在工具菜单中。
