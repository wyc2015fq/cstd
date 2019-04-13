
# dotnet new 命令 - 追求卓越,做到专业 - CSDN博客


2018年05月17日 17:22:40[Waldenz](https://me.csdn.net/enter89)阅读数：747


VSCode终端输入 dotnet new 显示的所有命令相关
eg: dotnet new console 可以在当前目录中创建控制台程序
D:\NetCoreDemo\coreconsole>dotnet new
使用情况: new [选项]

选项:
-h, --help          显示有关此命令的帮助。
-l, --list          列出包含指定名称的模板。如果未指定名称，请列出所有模板。
-n, --name          正在创建输出的名称。如果未指定任何名称，将使用当前目录的名称。
-o, --output        要放置生成的输出的位置。
-i, --install       安装源或模板包。
-u, --uninstall     卸载一个源或模板包。
--type              基于可用的类型筛选模板。预定义的值为 "project"、"item" 或 "other"。
--force             强制生成内容，即使该内容会更改现有文件。
-lang, --language   指定要创建的模板的语言。
模板                                                短名称              语言                标记
--------------------------------------------------------------------------------------------------------
Console Application                               console          [C\#], F\#, VB      Common/Console
Class library                                     classlib         [C\#], F\#, VB      Common/Library
Unit Test Project                                 mstest           [C\#], F\#, VB      Test/MSTest
xUnit Test Project                                xunit            [C\#], F\#, VB      Test/xUnit
ASP.NET Core Empty                                web              [C\#], F\#          Web/Empty
ASP.NET Core Web App (Model-View-Controller)      mvc              [C\#], F\#          Web/MVC
ASP.NET Core Web App                              razor            [C\#]              Web/MVC/Razor Pages
ASP.NET Core with Angular                         angular          [C\#]              Web/MVC/SPA
ASP.NET Core with React.js                        react            [C\#]              Web/MVC/SPA
ASP.NET Core with React.js and Redux              reactredux       [C\#]              Web/MVC/SPA
ASP.NET Core Web API                              webapi           [C\#], F\#          Web/WebAPI
global.json file                                  globaljson                         Config
NuGet Config                                      nugetconfig                        Config
Web Config                                        webconfig                          Config
Solution File                                     sln                                Solution
Razor Page                                        page                               Web/ASP.NET
MVC ViewImports                                   viewimports                        Web/ASP.NET
MVC ViewStart                                     viewstart                          Web/ASP.NET

Examples:
dotnet new mvc --auth Individual
dotnet new webconfig
dotnet new --help


