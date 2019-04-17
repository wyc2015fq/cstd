# ASP.NET Core Web MVC项目创建支持Docker发布 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年09月13日 10:00:22[boonya](https://me.csdn.net/boonya)阅读数：265








## 下载.Net SDK支持

我们需要运行的环境支持：

[https://www.microsoft.com/net/learn/get-started-with-dotnet-tutorial#install](https://www.microsoft.com/net/learn/get-started-with-dotnet-tutorial#install)

有了这个包，我们可以运行项目的dll文件。

## 创建Core web MVC项目

打开vs2017，菜单"文件">"新建">"项目"（同时会创建解决方案）：

![](https://img-blog.csdn.net/20180913091518848?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

 点击"确定"按钮，下一步操作选择“Core web MVC”，勾上docker支持就可以了，创建就OK了。我们得到如下的项目结构：

![](https://img-blog.csdn.net/20180913092053996?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

已经为我们生成了Docker的Dockerfile。

## 项目的Docker支持

Dockerfile（镜像构建文件）

```
FROM microsoft/dotnet:2.1-aspnetcore-runtime AS base
WORKDIR /app
EXPOSE 55162
EXPOSE 44331

FROM microsoft/dotnet:2.1-sdk AS build
WORKDIR /src
COPY ["NetCoreWeb/NetCoreWeb.csproj", "NetCoreWeb/"]
RUN dotnet restore "NetCoreWeb/NetCoreWeb.csproj"
COPY . .
WORKDIR "/src/NetCoreWeb"
RUN dotnet build "NetCoreWeb.csproj" -c Release -o /app

FROM build AS publish
RUN dotnet publish "NetCoreWeb.csproj" -c Release -o /app

FROM base AS final
WORKDIR /app
COPY --from=publish /app .
ENTRYPOINT ["dotnet", "NetCoreWeb.dll"]
```

说明：From是我们的依赖镜像，我们要运行也可以cmd直接定位到运行项目目录，然后运行dotnet run命令。 Docker里面run的是RUN dotnet restore "NetCoreWeb/NetCoreWeb.csproj"。

**另外，还有一个比较关键的配置文件，launchSettings.json：**

```
{
  "iisSettings": {
    "windowsAuthentication": false,
    "anonymousAuthentication": true,
    "iisExpress": {
      "applicationUrl": "http://localhost:55162",
      "sslPort": 44331
    }
  },
  "profiles": {
    "IIS Express": {
      "commandName": "IISExpress",
      "launchBrowser": true,
      "environmentVariables": {
        "ASPNETCORE_ENVIRONMENT": "Development"
      }
    },
    "NetCoreWeb": {
      "commandName": "Project",
      "launchBrowser": true,
      "environmentVariables": {
        "ASPNETCORE_ENVIRONMENT": "Development"
      },
      "applicationUrl": "https://localhost:5001;http://localhost:5000"
    },
    "Docker": {
      "commandName": "Docker",
      "launchBrowser": true,
      "launchUrl": "{Scheme}://localhost:{ServicePort}"
    }
  }
}
```

这里定义了我们可以运行的环境：IIS Docker等。

IIS启动访问：[http://localhost:55162](http://localhost:55162)转[https://localhost:44331](https://localhost:44331)

Docker访问：[http://localhost:5000](http://localhost:5000)转[https://localhost:5001](https://localhost:5001/)

## Docker容器运行输出

```
1>NetCoreWeb -> D:\DEVELOPERS\vs_workspace\NCWS\NetCoreWeb\bin\Debug\netcoreapp2.1\NetCoreWeb.dll
1>NetCoreWeb -> D:\DEVELOPERS\vs_workspace\NCWS\NetCoreWeb\bin\Debug\netcoreapp2.1\NetCoreWeb.Views.dll
1>docker run -dt -v "C:\Users\Administrator\vsdbg\vs2017u5:/remote_debugger:rw" -v "D:\DEVELOPERS\vs_workspace\NCWS\NetCoreWeb:/app" -v "C:\Users\Administrator\AppData\Roaming\ASP.NET\Https:/root/.aspnet/https:ro" -v "C:\Users\Administrator\AppData\Roaming\Microsoft\UserSecrets:/root/.microsoft/usersecrets:ro" -v "C:\Users\Administrator\.nuget\packages\:/root/.nuget/fallbackpackages2" -v "C:\Program Files\dotnet\sdk\NuGetFallbackFolder:/root/.nuget/fallbackpackages" -e "DOTNET_USE_POLLING_FILE_WATCHER=1" -e "ASPNETCORE_ENVIRONMENT=Development" -e "ASPNETCORE_URLS=https://+:443;http://+:80" -e "ASPNETCORE_HTTPS_PORT=44331" -e "NUGET_PACKAGES=/root/.nuget/fallbackpackages2" -e "NUGET_FALLBACK_PACKAGES=/root/.nuget/fallbackpackages;/root/.nuget/fallbackpackages2" -p 55162:80 -p 44331:443 --entrypoint tail netcoreweb:dev -f /dev/null
1>8fc76cf1a08136ac25ecc74c985a81adce588e5d46a65bdd06006a107bce6c2c
```

VS以Docker方式运行，运行起来后我们看到页面如下：

![](https://img-blog.csdn.net/20180913093139418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

来看我们构建好的镜像：

![](https://img-blog.csdn.net/20180913093313403?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

VS关闭之后IIS应用就停止了：

![](https://img-blog.csdn.net/20180913093503342?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

但是，容器还在运行

![](https://img-blog.csdn.net/20180913093850460?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们可以访问[http://localhost:5000](http://localhost:5000)进行验证，但是很遗憾失败了：

![](https://img-blog.csdn.net/20180913093820117?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

我们需要通过docker-compose进行启动，稍后再研究。最近在创建普通的aps.net web项目时，勾选了Docker支持选项，可以将docker-compose添加出来，ASP.net Core应该也是可以将其按照项目导入的。

![](https://img-blog.csdnimg.cn/20181130100457644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)



