# UE4源码Build上传Perforce - 逍遥剑客 - CSDN博客
2017年11月03日 15:02:28[xoyojank](https://me.csdn.net/xoyojank)阅读数：3654
UE4每次升级个版本总是要提交大量文件到Perforce, 是件很麻烦的事. 我们的引擎编译好了会连同Binaries一起上传, 美术和策划都可以直接使用跟程序一样的版本. 这样的话, Perforce上的引擎就需要有这些注意事项:
- 引擎目录完全包含所有代码和引擎依赖资源
- 引擎编译输出的dll, exe等也都需要提交, 保证策划和美术能够直接拉取引擎版本
- 引擎编译过程中会写入的文件, 需要改成writable, 不然程序开发时编译会出错
- 需要避免上传不必要的文件, 如Intermediate目录, exp, obj等
- [可选]如果需要定位美术和策划的引擎崩溃, 也需要上传pdb文件
如果使用P4V GUI客户端来进行这些操作, 在有大量文件的情况下, 会经常性卡死, 再说靠人力一个个去用鼠标去筛选所有附合要求的文件是件很烦人且容易出错的操作.  
在这里就记录一下整个过程, 免得下次升级时又要重新找命令规则步骤之类的:
- 从github拉取UE4的release版本
- 拷贝整个目录到Perforce Workspace, 需要避免.git文件夹也拷过来
- 执行Setup.bat下载引擎依赖资源
- 上传干净的引擎目录到Perforce
- 因为有些编译过程中的exe和dll等在上传后变成了只读属性, 需要改成writable的:
```
p4 edit -t binary+w .\Engine\Binaries\...\*.dll
p4 edit -t xbinary+w .\Engine\Binaries\...\*.exe
p4 edit -t text+w .\Engine\Binaries\...\*.config
```
- 生成sln进行编译, 这个过程中会在Binaries和Plugins下面生成exe和dll等文件
- 由于Binaries和Plugins下面是有子目录的, 所以我们需要递归提交这些文件, 并加上writable标记
```
for /f "delims=" %a in ('dir .\Engine\Binaries\*.dll /b /s') do p4 add -t +w %a
for /f "delims=" %a in ('dir .\Engine\Binaries\*.exe /b /s') do p4 add -t +w %a
for /f "delims=" %a in ('dir .\Engine\Binaries\*.target /b /s') do p4 add -t +w %a
for /f "delims=" %a in ('dir .\Engine\Binaries\*.modules /b /s') do p4 add -t +w %a
for /f "delims=" %a in ('dir .\Engine\Plugins\*.dll /b /s') do p4 add -t +w %a
for /f "delims=" %a in ('dir .\Engine\Plugins\*.modules /b /s') do p4 add -t +w %a
```
因为p4 add命令是不支持路径中的”…”的, 所以需要借助dir命令递归遍历子目录下的文件, 再使用for循环针对每个文件执行add操作. 如果要把这些命令写到批处理文件里, 注意需要把%a改成%%a. 
- 最后, 再提交一次, 完成
