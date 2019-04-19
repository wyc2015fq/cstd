# 使用VS Code开发调试ASP.NET Core 1.0 - 文章 - 伯乐在线
原文出处： [LineZero](http://www.cnblogs.com/linezero/p/VSCodeASPNETCore.html)
使用VS Code开发调试ASP.NET Core 1.0,微软在今天凌晨发布了.NET Core 1.0，ASP.NET Core 1.0 与 Entity Framewok 1.0。
之前跟大家讲解过VS Code开发调试.NET Core RC2应用程序。今天讲一下VS Code开发 ASP.NET Core。
## 环境安装
**.NET Core SDK Installer:**
**win x64:**
https://download.microsoft.com/download/A/3/8/A38489F3-9777-41DD-83F8-2CBDFAB2520C/packages/DotNetCore.1.0.0-SDK.Preview2-x64.exe
SDK 如果安装过 RC2，请先卸载然后再安装。
更多版本下载：
https://www.microsoft.com/net/download
**VSCode :**
https://code.visualstudio.com/
VSCode C#扩展：
Ctrl+P 打开Quick Open 输入： **ext install csharp  **选择C# 安装。
安装好插件以后重启VS Code。
## **创建项目**
首先确保 DotNetCore.1.0.0-SDK 安装成功。
**dotnet –version**
输出如下：
1.0.0-preview2-003121
然后就可以创建项目，.NET Core 1.0 增加创建web 项目。
**dotnet new -t web**
**dotnet restore**
**dotnet run**
**![](http://jbcdn2.b0.upaiyun.com/2016/06/6c8b4c4fd5e5ea497f84303ce3e4dfa3.png)**
![](http://jbcdn2.b0.upaiyun.com/2016/06/6e4777511aedcca4394f773e839ac098.png)
ctrl + c 停止项目，使用VS Code 打开 aspnetcore 文件夹。
如果是第一次用 VS Code 打开 C# 项目这里需要等待其下载 .NET Core Debugger 。
出现下图所示，代表安装好。然后点击Yes
![](http://jbcdn2.b0.upaiyun.com/2016/06/aec3edb91907c32c1d87f2d509abb091.png)
然后我们切到调试模块，点击调试。
![](http://jbcdn2.b0.upaiyun.com/2016/06/c8dd2ebca4ec9267df935753d1f32847.png)
成功调试，VS Code 会自动打开  http://localhost:5000
## 安装yo
首先我们需要安装nodejs，然后命令行输入：
**npm install -g yo**
**npm install -g generator-aspnet**
这样我们可以通过yo 生成项目、Controller 及View
### 添加 Controller
![](http://jbcdn2.b0.upaiyun.com/2016/06/edaf639ccbd174c63612cac0fcc28829.png)
**yo aspnet:MvcController DemoController**
**![](http://jbcdn2.b0.upaiyun.com/2016/06/067970f52d0dcdcb2466853a8f969993.png)**
### 添加 View
我们新建一个Demo 文件夹，然后在下面添加View
![](http://jbcdn2.b0.upaiyun.com/2016/06/a2cd69c52abd4ee943779adabc78be73.png)
**yo aspnet:MvcView Index**
访问： http://localhost:5000/demo
![](http://jbcdn2.b0.upaiyun.com/2016/06/07a596032f8b9bb3a37935ebd56a3520.png)
调试：
直接下断点既可调试。
![](http://jbcdn2.b0.upaiyun.com/2016/06/09a2f4cb0102809c580ad3f020a97eac.png)
