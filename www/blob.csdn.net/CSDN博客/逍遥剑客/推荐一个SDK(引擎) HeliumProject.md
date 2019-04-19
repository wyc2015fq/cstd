# 推荐一个SDK(引擎): HeliumProject - 逍遥剑客 - CSDN博客
2011年03月21日 11:36:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4205
N3停止更新之后, 这个是目前我觉得质量最高的代码
[http://heliumproject.org/](http://heliumproject.org/)
代码在GitHub上: https://github.com/HeliumProject
默认是没有sln文件的, 需要用premake4生成:
- 打开Visual Studio 2008 Command Prompt
- 转到HeliumProject目录
- 如果不想编译, 可以把premake4.lua里的Helium.BuildXXX那一大段都注释掉
- 如果没有装Python和FBX SDK, 可以把Helium.CheckEnvironment()注释掉
- 执行Utilities/Win32/premake4.exe vs2008
好了, Premake目录下就有生成好的所有sln/vcproj文件了
