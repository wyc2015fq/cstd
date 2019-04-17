# 处理使用node-gpy时遇到的Can't find "msbuild.exe"错误 - cocoonyang的专栏 - CSDN博客





2015年04月04日 13:47:11[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：5926










在Windows中使用node-gpy编译Node.js C++原生扩展时，需要使用Visual Studio C++的"msbuild.exe"。有时尽管Visual Studio安装运行正常，在调用node-gyp时仍会遇到找不到"msbuild.exe"的错误提示：

Error: Can't find "msbuild.exe". Do you have Mircrosoft Visual Studio C++ 2008+ instlled?


node-gpy在搜索"msbuild.exe"时，是通过查询Windows注册表，提取Visual Studio C++版本号来获取"msbuild.exe"安装路径。出现上述运行错误的原因之一就是node-gpy查询Windows注册表出现差错。node-gpy查询Windows注册表的脚本代码在
C:\Users\abc\AppData\Roaming\npm\node_modules\node-gyp\lib\build.js


其中"abc"为当前用户名。 具体代码如下：
```
function findMsbuild () {
    log.verbose('could not find "msbuild.exe" in PATH - finding location in registry')
    var notfoundErr = new Error('Can\'t find "msbuild.exe". Do you have Microsoft Visual Studio C++ 2008+ installed?')
    var cmd = 'reg  query "HKLM\\Software\\Microsoft\\MSBuild\\ToolsVersions" /s'
```


其中最后一行语句即为运行出错代码。如代码所示，node-gpy通过调用"reg.exe"查询Windows注册表。node-gpy的脚本代码中没有设置"reg.exe"路径。 "reg.exe"一般保存在'C:\\Windows\\System32\\'文件夹中，将这行代码做如下修改：
var cmd = 'C:\\Windows\\System32\\reg.exe query "HKLM\\Software\\Microsoft\\MSBuild\\ToolsVersions" /s'
node-gpy就能获取"msbuild.exe"的安装路径。







