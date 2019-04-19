# 使用VS Code开发 调试.NET Core 应用程序 - 文章 - 伯乐在线
原文出处： [LineZero（博客园）](http://www.cnblogs.com/linezero/p/5438074.html)
使用VS Code开发 调试.NET Core RC2应用程序,由于.NET Core 目前还处于预览版。
本文使用微软提供的示例进行开发及调试。
[https://github.com/aspnet/cli-samples](https://github.com/aspnet/cli-samples)
.NET Core 介绍及说明：
[https://github.com/dotnet/cli](https://github.com/dotnet/cli)
[http://dotnet.github.io/getting-started/](http://dotnet.github.io/getting-started/)
## 环境安装
本文开发的实际环境: win10 x64 VSCode 1.0
**下载**
[https://github.com/dotnet/cli#installers-and-binaries](https://github.com/dotnet/cli#installers-and-binaries)
**.NET Core SDK Installer:**
[https://dotnetcli.blob.core.windows.net/dotnet/beta/Installers/Latest/dotnet-dev-win-x64.latest.exe](https://dotnetcli.blob.core.windows.net/dotnet/beta/Installers/Latest/dotnet-dev-win-x64.latest.exe)
**VSCode :**
[https://code.visualstudio.com/](https://code.visualstudio.com/)
**VSCode C#插件：**
[https://github.com/OmniSharp/omnisharp-vscode/releases](https://github.com/OmniSharp/omnisharp-vscode/releases)
最新版：[https://github.com/OmniSharp/omnisharp-vscode/releases/download/v1.0.4-rc2/csharp-1.0.4-rc2.vsix](https://github.com/OmniSharp/omnisharp-vscode/releases/download/v1.0.4-rc2/csharp-1.0.4-rc2.vsix)
安装好VSCode以后，打开VSCode 安装C#插件。
**安装插件：**直接用VSCode 打开插件文件就可以安装了。
安装好以后 F1 会发现多了dotnet 命令，证明也就安装完成。
![](http://jbcdn2.b0.upaiyun.com/2016/05/d45ca459b5264fdee9a03dce79921834.png)
## 开发调试
下载微软的示例代码：[https://github.com/aspnet/cli-samples](https://github.com/aspnet/cli-samples)
下载代码后，使用VSCode 打开文件夹 cli-samples/HelloMvc
![](http://jbcdn2.b0.upaiyun.com/2016/05/1abb5df534497d38eddada377d3bae42.png)
**F1 输入 dotnet restore**
选择对应的dotnet restor (这里显示好像是插件的bug)
![](http://jbcdn2.b0.upaiyun.com/2016/05/f0958e2ff391386eae6e7f51511220a5.png)
**你还可以直接在文件夹打开命令行，输入 dotnet restore 同样可以还原相关引用。**
还原好相关的引用以后我们就可以进行调试了。
点击调试 程序就跑起来。
![](http://jbcdn2.b0.upaiyun.com/2016/05/c957147d60c3c8e37c839d02cff81e3b.png)
![](http://jbcdn2.b0.upaiyun.com/2016/05/ee7d2b82876f29dd23f12d174822f7a6.png)
这样就可以下断点调试。
访问 http://localhost:5000/
![](http://jbcdn2.b0.upaiyun.com/2016/05/e493e640a617c264611dd017f2d5b986.png)
开发
我们来添加一个新的Action
```
```
[HttpGet("/about")]
        public IActionResult About(){
            var useragent=Request.Headers["User-Agent"];
            return Content(useragent+"\r\nabout by linezero");
        }
```
```
访问： http://localhost:5000/about
![](http://jbcdn2.b0.upaiyun.com/2016/05/355fc94d1239dad7aaed87f5af3662c3.png)
下断点调试一下程序，断点设置跟VS一样。
![](http://jbcdn2.b0.upaiyun.com/2016/05/df0481941b91406c9a3d46f6964ce1df.png)
断下来以后，可以查看对应的属性以及值。
左侧有对应的监视器，可以查看各个值。
这样我们已经可以调试.NET Core。
