# VS编译时使用/去除NuGet管理库 - DoubleLi - 博客园






原文链接：[VS编译时自动下载NuGet管理的库](http://www.cnblogs.com/codesee/p/3900944.html)

之前一直使用NuGet来管理一些第三方的库，但是每次check in代码时候为了保证编译通过，都需要把对应的packages check in。

比较耗费时间，特别是往github上同步代码，而且这些库在NuGet上都有的，是没有必要check in上去的。

应广大的同胞的要求，NuGet在2.0版本开始支持在编译的时候自动同步对应的packages。不过在2.0-2.6版本，默认是没有勾选自动下载的，2.7之后默认是勾选的。

一、设置自动下载

![](https://images0.cnblogs.com/i/639149/201408/091442499591223.png)

二、设置启用NuGet restore功能

在solution上右键

![](https://images0.cnblogs.com/i/639149/201408/091443445063620.png)

最后效果如下：

![](https://images0.cnblogs.com/i/639149/201408/091447543509850.png)

设置完成后，每当编译工程的时候，Build Task都会检查工程里的packages.config文件里的库是否在packages文件夹下，如果没有则会自动下载并解压到packages文件夹下。

在下载过程中，NuGet都会检查对应的版本，不会自动升级的。



二、关闭自动下载

同样，在相应的设置界面关闭即可。

若只是想使用本地的一些软件包，而不是网络版本，打开工程的package.config文件，删除掉特定的行即可。



三。关闭工程中使用特定包

这次使用记事本打开工程的vcxproj文件，注销掉与包有关的网络方案，替换为本地包。

比如：

把<Import Project="..\..\..\NugetPackages\gflags.2.1.2.1\build\native\gflags.props" Condition="Exists('..\..\..\NugetPackages\gflags.2.1.2.1\build\native\gflags.props')" />
删除，

贴错了![难过](http://static.blog.csdn.net/xheditor/xheditor_emot/default/sad.gif)

把 <Import Project="..\..\..\NugetPackages\LevelDB-vc120.1.2.0.0\build\native\LevelDB-vc120.targets" Condition="Exists('..\..\..\NugetPackages\LevelDB-vc120.1.2.0.0\build\native\LevelDB-vc120.targets')" />   
删除，

把<Error Condition="!Exists('..\..\..\NugetPackages\LevelDB-vc120.1.2.0.0\build\native\LevelDB-vc120.targets')" Text="$([System.String]::Format('$(ErrorText)', '..\..\..\NugetPackages\LevelDB-vc120.1.2.0.0\build\native\LevelDB-vc120.targets'))" />
删除，

即可去除到所有使用Nuget的包含，然后替换为本地包。









