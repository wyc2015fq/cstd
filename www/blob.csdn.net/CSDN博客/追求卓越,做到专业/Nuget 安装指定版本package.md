
# Nuget 安装指定版本package - 追求卓越,做到专业 - CSDN博客


2017年08月03日 14:56:14[Waldenz](https://me.csdn.net/enter89)阅读数：4856


首先打开程序包管理器控制台：工具→Nuget程序包管理器→程序包管理器控制台

打开如下图所示的命令行界面

安装指定版本的Package
PM> Install-Package BenchMarkdotnet -version 0.9.7
更新到指定版本的Package
PM> Update-Package BenchMarkdotnet -version 0.9.7
Update-Package命令相关参数说明：
|Parameter|Type|Description|
|-Id|string|Specifies the Id of the package to be updated.|
|-ProjectName|string|Specifies the name of the project in which packages should be updated.|
|-Version|SemanticVersion*|Specifies the new target version of the package as a result of the update.|
|-Source|string|Specifies where to look for package updates, overriding the package sources that are specified in the Options dialog. This value can be either a url to a remote repository or a path to a local repository or the name of a package source specified in the Options dialog.|
|-IgnoreDependencies|Switch parameter|If set, NuGet will ignore dependency packages and only update the main package.|
|-Safe|Switch parameter|If set, NuGet will only update to a new version that has the same major and minor versions as the previous package. For example, if the old version is 1.2.0, NuGet will accept the update package with version of 1.2.1 or 1.2.9999 but it will|not|accept 1.3.0.|
|-IncludePrerelease|Switch parameter|If set, NuGet will consider prerelease packages as candidates for updates.|
|-Reinstall|Switch parameter|If set, instead of updating the package to the latest version, NuGet will uninstall the package and reinstall the same version. This is useful when, for example, you've updated the target framework of your project, e.g. from .NET 4.0 to .NET 4.5, and you want to reference .NET 4.5-specific assemblies in the package. You can't set this parameter together with the -Version parameter.|
From: http://blog.csdn.net/soapcoder92/article/details/52168023


