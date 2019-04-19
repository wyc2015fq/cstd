# 使用VS Code 开发.NET Core 应用程序 部署到Linux 跨平台 - 文章 - 伯乐在线
原文出处： [LineZero](http://www.cnblogs.com/linezero/p/5460759.html)
使用VS Code 开发.NET Core 应用程序 部署到Linux 跨平台。前面讲解了VSCode开发调试 .NET Core。都只是在windows下运行。.NET Core真正的核心是跨平台，我们现在来了解学习 .NET Core 跨平台。在windows 下开发.NET Core 应用程序，然后部署到Linux 平台运行。
.NET Core RC2版基本上已经完成。https://github.com/dotnet/cli/milestones
可以看到
1.0.0-rc2
Due by May 6, 2016  Last updated about 22 hours ago  100% complete.
Due by June 30, 2016 1.0.0-rtm.
## 环境安装
本文开发环境: win10 x64 VSCode 1.0
**下载**
**.NET Core SDK Installer:**
https://dotnetcli.blob.core.windows.net/dotnet/beta/Installers/Latest/dotnet-dev-win-x64.latest.exe
**VSCode :**
https://code.visualstudio.com/
**VSCode C#插件：**
https://github.com/OmniSharp/omnisharp-vscode/releases
## 开发调试
采用微软官方示例，进行开发调试，并最终部署到Linux。
https://github.com/aspnet/cli-samples
我们来添加一个新的Action。


C#
```
[HttpGet("/info")]
        public IActionResult Info(){
            return Content(".NET Core跨平台 LineZero");
        }
```
![](http://jbcdn2.b0.upaiyun.com/2016/05/ac392fc874c68ac5a674ec0d4f6b6046.png)
访问  http://localhost:5000/info
![](http://jbcdn2.b0.upaiyun.com/2016/05/53d9e215a1798eb6485fd6c1bbb7dcc8.png)
## 跨平台
.NET Core 跨平台
### 环境:
Ubuntu 14.04.4 LTS 服务器版 全新安装系统。
下载地址：http://mirrors.neusoft.edu.cn/ubuntu-releases/14.04.4/ubuntu-14.04.4-server-amd64.iso
你也可以下载桌面版安装。你可以直接在Ubuntu中使用VSCode 开发调试。
下载地址：http://mirrors.neusoft.edu.cn/ubuntu-releases/14.04.4/
本文部署环境：Ubuntu 14.04.4 LTS 服务器版
### SDK安装方法：
http://dotnet.github.io/getting-started/
使用root 用户 执行下面命令。


Shell
```
sh -c 'echo "deb [arch=amd64] http://apt-mo.trafficmanager.net/repos/dotnet/ trusty main" > /etc/apt/sources.list.d/dotnetdev.list'
apt-key adv --keyserver apt-mo.trafficmanager.net --recv-keys 417A0893
apt-get update
apt-get install dotnet-dev-1.0.0-rc2-002673
```
安装好以后 输入 **dotnet –info  **显示如下信息：
![](http://jbcdn2.b0.upaiyun.com/2016/05/3e4feba75eaeb8b3af49a6a684a2b4f2.png)
这里SDK 就安装好了，我们就可以在Linux 中开发 .NET Core 应用程序。
### 跨平台运行：
下面我们来发布项目 使用的是** dotnet publish**
在windows下定位到 HelloMvc 文件夹打开命令行，输入**dotnet publish**
**![](http://jbcdn2.b0.upaiyun.com/2016/05/dcda007fd70ca47ad1fa84a30fcb43c8.png)**
发布好以后，上传 **publish **文件夹到linux 系统。
然后**cd publish**
直接输入 **dotnet HelloMvc.dll** 就可以执行了。
![](http://jbcdn2.b0.upaiyun.com/2016/05/ab9cedbf008e956e1f2fb72ae824857d.png)
实现了.NET Core 跨平台 ,这也就是.NET Core 的真正魅力所在。
期待.NET Core 的正式版 Due by June 30, 2016 1.0.0-rtm.
