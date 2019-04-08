# 使用devenv或MSBuild在命令行编译sln或csproj



## 一 使用devenv来build单个project

 

devenv是VisualStudio的可执行程序，一般安装在“C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\IDE”下。用来在命令行或GUI方式运行VisualStudio。其中devenv.com是命令行程序，devenv.exe是GUI的程序。默认地当你调用devenv的时候其实是启动devenv.com，除非你显示地调用devenv.exe才会启动GUI的VisualStudio。

 

使用devenv来build一个.sln的实例：

```shell
devenv d:\Build\MyProject\Src\MyProject.sln /Build "Release|Win32"
```



使用devenv来build一个.sln中的某个的实例：

```shell
devenv.exe d:\Build\MyProject\Src\NyProject.sln /build "Release|Win32" /Project MyProject1
```

 

注意：通常地.sln中的多个Projects间有依赖关系，所以虽然你只是build一个.sln中的某个Project，但是还是需要指定Project所在.sln，然后通过/Project来指定Project的名字。

 

 

如果只是单个的Project，没有引用其他的projects，这个时候可以不指定.sln，直接build Project，如下实例：

devenv d:\Build\MyProject\Src\MyProject.vcxproj /Build "Release|Win32"

注意此时实际上devenv做如下事：

此时devenv将在此project文件的父目录中查找与Project相同名字的.sln；

如果没有找到的话，然后查找其他的引用了此Project的.sln；

如果还是没有找到的话会创建临时的不保存的与Project同名的.sln。

 

 

## 二 devenv的更多帮助 

可以使用devenv /?来查看详细的帮助。

.sln或project的路径有空格时，需要对路径加"";

多个/开关间使用空格隔开；

devenv不支持通配符或正则表达式语法；

 

## 三 MSBuild

如果你的机器上没有装有VisualStudio，那么可以使用MSBuild来build .sln或project。MSBuild可以通过安装.NETFramework来安装，一般的安装路径为C:\Windows\Microsoft.NET\Framework。其实devenv执行build时候，后台也是调用MSBuild来build的。

 

可以使用msbuild /?来查看详细的帮助；

 

简单实例如下： 

```
MSBuild MyApp.sln /t:Rebuild /p:Configuration=Release
MSBuild MyApp.csproj /t:Clean /p:Configuration=Debug;/p:Platform=x86;TargetFrameworkVersion=v3.5
```

 

同样注意，如果project引用了其他的projects的时候，最好build整个.sln。 

 





# 命令行编译项目





```vbscript
call "D:\Program Files\Microsoft Visual Studio 11.0\VC\vcvarsall.bat"
cd /d %~dp0
msbuild "WPEX.vcxproj" /p:Configuration=Debug /m
```

这里要简单介绍一下在这个bat批处理文件中用到的“cd /d %~dp0”的含义。

在批处理开头加入cd /d %~dp0 一行代码就能做到“一次编写，到处运行”。%0是批处理文件本身的路径，%~dp进行扩展， d向前扩展到驱动器，p往后扩展到路径。例如，我的bat文件在e:\work\ct08\bin\0.bat，则%0就是e:\work\ct08\bin\0.bat，%~dp0是e:\work\ct08\bin\。而cd命令使用 /D 命令行开关（Windows下命令和参数不区分大小写），除了改变驱动器的当前目录之外，还可改变当前驱动器。

另外，%i提取第i个命令选项，例如%1提取第1个option，i可以取值从1到9

%~0： 取文件名（名+扩展名）

%~f0：取全路径

%~d0：取驱动器名

%~p0：只取路径（不包驱动器）

%~n0：只取文件名

%~x0：只取文件扩展名

%~s0：取缩写全路径名

%~a0：取文件属性

%~t0：取文件创建时间

%~z0：取文件大小

以上选项可以组合起来使用。