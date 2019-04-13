
# dotnet sdk命令 - 追求卓越,做到专业 - CSDN博客


2018年05月17日 17:47:40[Waldenz](https://me.csdn.net/enter89)阅读数：745


new              初始化 .NET 项目。
restore          通过NuGet来下载定义在project.json文件中的依赖，然后放到用户目录下的.nuget/packages文件夹中。默认情况下，下载依赖组建的过程是并行进行的。dotnet restore命令运行完毕后，会生成project.lock.json文件。
build              编译项目。生成 .NET 项目。将项目中的所有源代码文件和依赖的组件编译为二进制的dll文件。
该命令会读取project.lock.json，如果项目中找不到该文件，则需要先运行dotnet restore命令。
执行完dotnet build命令后，会在bin/Debug/netcoreapp1.0文件夹下生成.dll文件和.pbd文件等
run              编译并立即执行 .NET 项目。
publish          发布 .NET 项目以进行部署(包括运行时)。Debug 和 Release，默认Debug。命令会编译代码，然后读取project.json文件中定义的所有依赖组件，最后将这些东西输出到一个文件夹中。生成的文件默认会输出到\bin\Debug\netcoreapp1.0\publish中，你可以通过-o或–output选项来修改输出位置。当你需要发布你的代码时，该命令的输出文件将是你所需要的全部文件。
`dotnet publish - c release -r win-x64
dotnet publish -h
使用情况: dotnet publish [选项] <PROJECT>
参数:
  <PROJECT>   要操作的项目文件。如果未指定文件，则命令会在当前目录下搜索一个文件。
选项:
  -h, --help                            显示命令行帮助。
  -o, --output <OUTPUT_DIR>             要放置已发布项目的输出目录。
  -f, --framework <FRAMEWORK>           要发布的目标框架。必须在项目文件中指定目标框架。
  -r, --runtime <RUNTIME_IDENTIFIER>    针对其发布应用程序的目标运行时。
                                        默认发布不含可执行文件的依赖框架的应用程序。
  -c, --configuration <CONFIGURATION>   要发布的配置。大多数项目的默认值是 "Debug"。
  --version-suffix <VERSION_SUFFIX>     设置生成项目时使用的 $(VersionSuffix) 属性的值。
  --manifest <MANIFEST>                 指向目标清单文件的路径，该文件包含要通过发布步骤执行的包的列表。
  --no-build                            发布之前不要生成项目。Implies --no-restore.
  --self-contained                      随附应用程序发布 .NET Core 运行时，免除在目标计算机上安装运行时的需求。如果指定了运行时标识符，则默认为 “true”。
  --no-restore                          生成前请勿还原项目。
  -v, --verbosity <LEVEL>               设置 MSBuild 详细级别。允许值为 q[uiet]、m[inimal]、n[ormal]、d[etailed] 和 diag[nostic]。
  --no-dependencies                     请勿还原项目到项目引用，仅还原指定项目。
  --force                               强制解析所有依赖项，即使最后一次还原已经成功。
                                        这等效于删除 project.assets.json。`test             使用项目中指定的测试运行程序运行单元测试。
pack             创建 NuGet 包。编译代码并生成一个NuGet包，具体来说就是在bin\Debug目录下生成一个.nupkg文件和一个.symbols.nupkg文件。
migrate          将基于 project.json 的项目迁移到基于 MSBuild 的项目。
clean            清除生成输出。
sln              修改解决方案(SLN)文件。
将项目添加到sln中。
`dotnet sln add .\web\web.csproj`add              将引用添加到项目中。在web项目中添加models的引用
`dotnet add web/web.csproj reference models/models.csproj`remove       从项目中删除引用。
list               列出项目中的引用。
nuget          提供其他 NuGet 命令。
msbuild       运行 Microsoft 生成引擎 (MSBuild)。
vstest          运行 Microsoft 测试执行命令行工具。

