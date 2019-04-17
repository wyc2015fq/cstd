# ASP.NET Core 的 Visual Studio Tools for Docker - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月27日 17:41:22[boonya](https://me.csdn.net/boonya)阅读数：214标签：[ASP.NET Core																[Docker](https://so.csdn.net/so/search/s.do?q=Docker&t=blog)](https://so.csdn.net/so/search/s.do?q=ASP.NET Core&t=blog)
个人分类：[ASP.NET](https://blog.csdn.net/boonya/article/category/7665529)








原文地址：[https://docs.microsoft.com/zh-cn/aspnet/core/host-and-deploy/docker/visual-studio-tools-for-docker?view=aspnetcore-2.1](https://docs.microsoft.com/zh-cn/aspnet/core/host-and-deploy/docker/visual-studio-tools-for-docker?view=aspnetcore-2.1)

作者： [Shayne Boyer](https://github.com/spboyer)，[olprod](https://github.com/olprod)

Visual Studio 2017 支持生成、调试和运行面向 .NET Core 的容器化 ASP.NET Core 应用。 Windows 和 Linux 容器均受支持。

[查看或下载示例代码](https://github.com/aspnet/Docs/tree/master/aspnetcore/host-and-deploy/docker/visual-studio-tools-for-docker/samples)（[如何下载](https://docs.microsoft.com/zh-cn/aspnet/core/index?view=aspnetcore-2.1#how-to-download-a-sample)）

## 系统必备
- [Docker for Windows](https://docs.docker.com/docker-for-windows/install/)
- 具有“.NET Core 跨平台开发”工作负载的 [Visual Studio 2017](https://www.visualstudio.com/)

## 安装和设置

要安装 Docker，请先查看[用于 Windows 的 Docker：安装须知](https://docs.docker.com/docker-for-windows/install/#what-to-know-before-you-install)了解相关信息。 然后安装[用于 Windows 的 Docker](https://docs.docker.com/docker-for-windows/install/)。

Docker for Windows 中的[共享驱动器](https://docs.docker.com/docker-for-windows/#shared-drives)必须配置为支持卷映射和调试。 右键单击系统托盘中的 Docker 图标，单击“设置”，然后选择“共享驱动器”。 选择 Docker 存储文件的驱动器。 单击“应用”。

![为容器选择本地驱动器 C 作为共享驱动器的对话框](https://docs.microsoft.com/zh-cn/aspnet/core/host-and-deploy/docker/visual-studio-tools-for-docker/_static/settings-shared-drives-win.png?view=aspnetcore-2.1)

提示

未配置共享驱动器时，Visual Studio 2017 15.6 及更高版本会发出提示。

## 向 Docker 容器添加项目

若要容器化 ASP.NET Core 项目，项目必须面向 .NET Core。 同时支持 Linux 和 Windows 容器。

向项目添加 Docker 支持后，可选择 Windows 或 Linux 容器。 Docker 主机必须运行类型相同的容器。 要更改正在运行的 Docker 实例中的容器类型，请右键单击系统托盘中的 Docker 图标，再选择“切换到 Windows 容器...”或“切换到 Linux 容器...”。

### 新应用

使用“ASP.NET Core Web 应用”项目模板新建应用时，请选中“启用 Docker 支持”复选框：

![“启用 Docker 支持”复选框](https://docs.microsoft.com/zh-cn/aspnet/core/host-and-deploy/docker/visual-studio-tools-for-docker/_static/enable-docker-support-check-box.png?view=aspnetcore-2.1)

如果目标框架是 .NET Core，可通过 OS 下拉列表选择容器类型。

### 现有应用

对于面向 .NET Core 的 ASP.NET Core 项目，可通过两个选项使用工具添加 Docker 支持。 在 Visual Studio 中打开项目，然后选择以下选项之一：
- 从“项目”菜单中选择“Docker 支持”。
- 右键单击“解决方案资源管理器”中的项目，然后选择“添加” > “Docker 支持”。

Visual Studio Tools for Docker 不支持向面向.NET Framework 的现有 ASP.NET Core 项目添加 Docker。

## Dockerfile 概述

Dockerfile，用作创建最终 Docker 映像的方案，添加到项目根目录。 请参阅 [Dockerfile 引用](https://docs.docker.com/engine/reference/builder/)，了解其中的命令。 此特定 Dockerfile 使用[多阶段生成](https://docs.docker.com/engine/userguide/eng-image/multistage-build/)，该生成包含四个不同的命名生成阶段：

dockerfile

```
FROM microsoft/dotnet:2.1-aspnetcore-runtime AS base
WORKDIR /app
EXPOSE 59518
EXPOSE 44364

FROM microsoft/dotnet:2.1-sdk AS build
WORKDIR /src
COPY HelloDockerTools/HelloDockerTools.csproj HelloDockerTools/
RUN dotnet restore HelloDockerTools/HelloDockerTools.csproj
COPY . .
WORKDIR /src/HelloDockerTools
RUN dotnet build HelloDockerTools.csproj -c Release -o /app

FROM build AS publish
RUN dotnet publish HelloDockerTools.csproj -c Release -o /app

FROM base AS final
WORKDIR /app
COPY --from=publish /app .
ENTRYPOINT ["dotnet", "HelloDockerTools.dll"]
```

前面的 Dockerfile 基于 [microsoft/dotnet](https://hub.docker.com/r/microsoft/dotnet/) 映像。 此基础映像包括 ASP.NET Core 运行时和 NuGet 包。 对该包进行了实时 (JIT) 编译，以提高启动性能。

如果选中了新建项目对话框的“为 HTTPS 配置”复选框，则 Dockerfile 公开两个端口。 一个端口用于 HTTP 流量；另一个端口用于 HTTPS。 如果未选中该复选框，则为 HTTP 流量公开单个端口 (80)。

## 向应用添加容器业务流程协调程序支持

Visual Studio 2017 版本 15.7 或更早版本支持 [Docker Compose](https://docs.docker.com/compose/overview/) 作为唯一的容器业务流程解决方案。 可通过“添加” > “Docker 支持”添加 Docker Compose。

Visual Studio 2017 版本 15.8 或更高版本仅在获得指示时添加业务流程解决方案。 右键单击“解决方案资源管理器”中的项目，然后选择“添加” > “容器业务流程协调程序支持”。 提供了两个不同的选择：[Docker Compose](https://docs.microsoft.com/zh-cn/aspnet/core/host-and-deploy/docker/visual-studio-tools-for-docker?view=aspnetcore-2.1#docker-compose) 和 [Service Fabric](https://docs.microsoft.com/zh-cn/aspnet/core/host-and-deploy/docker/visual-studio-tools-for-docker?view=aspnetcore-2.1#service-fabric)。

### Docker Compose

Visual Studio Tools for Docker 通过以下文件向解决方案添加 docker-compose 项目：
- docker-compose.dcproj – 表示项目的文件。 包括 `<DockerTargetOS>` 元素，用于指定要使用的操作系统。
- .dockerignore – 列出在生成生成上下文时要排除的文件和目录模式。
- docker-compose.yml – 基本 [Docker Compose](https://docs.docker.com/compose/overview/) 文件，用于定义要分别通过 `docker-compose build` 和 `docker-compose run` 生成和运行的映像集合。
- docker compose.override.yml – 一个可选文件，通过 Docker Compose 读取，包含服务的配置替代。 Visual Studio 执行 `docker-compose -f "docker-compose.yml" -f "docker-compose.override.yml"` 以合并这些文件。

docker-compose.yml 文件引用在项目运行时创建的映像的名称：

yaml

```
version: '3.4'

services:
  hellodockertools:
    image: ${DOCKER_REGISTRY}hellodockertools
    build:
      context: .
      dockerfile: HelloDockerTools/Dockerfile
```

在前面的示例中，应用在“调试”模式下运行时，`image: hellodockertools` 生成映像 `hellodockertools:dev`。 应用在“发布”模式下运行时，生成 `hellodockertools:latest` 映像。

如果将映像推送到注册表，则需要添加 [Docker Hub](https://hub.docker.com/) 用户名（如 `dockerhubusername/hellodockertools`）作为映像名称的前缀名。 或者，更改映像名称，使其包含专用注册表 URL（如 `privateregistry.domain.com/hellodockertools`），具体取决于所用配置。

如果根据生成配置需要不同行为（例如，调试或发布），请添加特定于配置的 docker-compose 文件。 这些文件应根据生成配置进行命名（例如，docker-compose.vs.debug.yml 和 docker compose.vs.release.yml）并放置在与 docker-compose-override.yml 文件相同的位置。

使用特定于配置的替代文件，可以为调试和发布生成配置指定不同的配置设置（如环境变量或入口点）。

### Service Fabric

除了基础[必备组件](https://docs.microsoft.com/zh-cn/aspnet/core/host-and-deploy/docker/visual-studio-tools-for-docker?view=aspnetcore-2.1#prerequisites)外，[Service Fabric](https://docs.microsoft.com/zh-cn/azure/service-fabric/) 业务流程解决方案还需要以下必备组件：
- [Microsoft Azure Service Fabric SDK](https://www.microsoft.com/web/handlers/webpi.ashx?command=getinstallerredirect&appid=MicrosoftAzure-ServiceFabric-CoreSDK) 版本 2.6 或更高版本
- Visual Studio 2017 的“Azure 开发”工作负荷

Service Fabric 不支持在 Windows 上的本地开发群集中运行 Linux 容器。 如果项目已在使用 Linux 容器，Visual Studio 会提示切换到 Windows 容器。

Visual Studio Tools for Docker 执行以下任务：
- 
向解决方案添加 <project_name>Application Service Fabric 应用程序项目。

- 
向 ASP.NET Core 项目添加 Dockerfile 和 .dockerignore 文件。 如果 ASP.NET Core 项目中已存在 Dockerfile，则其重命名为 Dockerfile.original。 创建类似于以下形式的新 Dockerfile：

dockerfile

- 
```
# See https://aka.ms/containerimagehelp for information on how to use Windows Server 1709 containers with Service Fabric.
# FROM microsoft/aspnetcore:2.0-nanoserver-1709
FROM microsoft/aspnetcore:2.0-nanoserver-sac2016
ARG source
WORKDIR /app
COPY ${source:-obj/Docker/publish} .
ENTRYPOINT ["dotnet", "HelloDockerTools.dll"]
```

- 
将 `<IsServiceFabricServiceProject>` 元素添加到 ASP.NET Core 项目的 .csproj 文件：

XML

- 
```
<IsServiceFabricServiceProject>True</IsServiceFabricServiceProject>
```

- 
向 ASP.NET Core 项目添加 PackageRoot 文件夹。 该文件夹包含服务清单和新服务的设置。


有关详细信息，请参阅[将 Windows 容器中的 .NET 应用部署到 Azure Service Fabric](https://docs.microsoft.com/zh-cn/azure/service-fabric/service-fabric-host-app-in-a-container)。

## 调试

在工具栏的调试下拉列表中选择“Docker”，然后开始调试应用。 “输出”窗口的“Docker”视图显示发生的以下操作：
- 已获取 microsoft/dotnet 运行时映像的 2.1-aspnetcore-runtime 标记（如果缓存中尚不存在）。 该映像可安装 ASP.NET Core 和.NET Core 运行时及其关联的库。 在生产环境中针对运行 ASP.NET Core 应用对其进行了优化。
- `ASPNETCORE_ENVIRONMENT` 环境变量设置为容器内的 `Development`。
- 公开了两个动态分配的端口：分别用于 HTTP 和 HTTPS。 可以使用 `docker ps` 命令查询分配给本地主机的端口。
- 应用已复制到容器。
- 默认浏览器使用动态分配端口启动，带有附加到容器的调试程序。

最终得到的应用的 Docker 映像标记为“开发”。 该映像基于 microsoft/dotnet 基础映像的 2.1-aspnetcore-runtime 标记。 在“包管理器控制台”(PMC) 窗口中运行 `docker images` 命令。 显示了计算机上的映像：

console

```
REPOSITORY        TAG                     IMAGE ID      CREATED         SIZE
hellodockertools  dev                     d72ce0f1dfe7  30 seconds ago  255MB
microsoft/dotnet  2.1-aspnetcore-runtime  fcc3887985bb  6 days ago      255MB
```

备注

因为“调试”配置使用卷装载提供迭代体验，因此，开发映像中缺少应用内容。 要推送映像，请使用“发布”配置。

在 PMC 中运行 `docker ps` 命令。 请注意，应用使用容器运行：

console

```
CONTAINER ID        IMAGE                  COMMAND                   CREATED             STATUS              PORTS                   NAMES
baf9a678c88d        hellodockertools:dev   "C:\\remote_debugge..."   21 seconds ago      Up 19 seconds       0.0.0.0:37630->80/tcp   dockercompose4642749010770307127_hellodockertools_1
```

## 编辑并继续

对静态文件和 Razor 视图的更改会自动更新，无需执行编译步骤。 进行更改，保存并在浏览器中刷新，以查看更新。

必须在容器内编译和重启 Kestrel，才能修改代码文件。 更改后，按 `CTRL+F5` 执行过程，并在容器内启动应用。 未重新生成或停止 Docker 容器。 在 PMC 中运行 `docker ps` 命令。 请注意，截至 10 分钟前，原始容器仍在运行：

console

```
CONTAINER ID        IMAGE                  COMMAND                   CREATED             STATUS              PORTS                   NAMES
baf9a678c88d        hellodockertools:dev   "C:\\remote_debugge..."   10 minutes ago      Up 10 minutes       0.0.0.0:37630->80/tcp   dockercompose4642749010770307127_hellodockertools_1
```

## 发布 Docker 映像

完成应用的开发和调试循环后，Visual Studio Tools for Docker 可帮助创建应用的生产映像。 将配置下拉列表更改为“发布”，然后生成应用。 该工具从 Docker Hub 中获取 compile/publish 映像（如果缓存中尚不存在）。 生成了具有“latest”标签的映像，可以将其推送到专用注册表或 Docker Hub。

在 PMC 中运行 `docker images` 命令，查看映像列表。 显示了类似下面的输出：

console

```
REPOSITORY        TAG                     IMAGE ID      CREATED             SIZE
hellodockertools  latest                  e3984a64230c  About a minute ago  258MB
hellodockertools  dev                     d72ce0f1dfe7  4 minutes ago       255MB
microsoft/dotnet  2.1-sdk                 9e243db15f91  6 days ago          1.7GB
microsoft/dotnet  2.1-aspnetcore-runtime  fcc3887985bb  6 days ago          255MB
```

备注

`docker images` 命令返回存储库名称和标记标识为 <none> （上面未列出）的中间映像。 这些未命名映像由[多阶段生成](https://docs.docker.com/engine/userguide/eng-image/multistage-build/)*Dockerfile* 生成。 它们可提高生成最终映像的效率 — 发生更改时，仅重新生成必要的层。 不再需要中间映像时，请使用 [docker rmi](https://docs.docker.com/engine/reference/commandline/rmi/) 命令将其删除。

可能希望生产或发布映像的大小比开发映像小。 由于卷映射，调试程序和应用从本地计算机运行，而不在容器内运行。 最新映像已打包必要的应用代码，以在主机上运行应用。 因此，增量是应用代码的大小。




