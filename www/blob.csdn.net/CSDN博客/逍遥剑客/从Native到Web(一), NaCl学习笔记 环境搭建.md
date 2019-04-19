# 从Native到Web(一), NaCl学习笔记: 环境搭建 - 逍遥剑客 - CSDN博客
2012年11月03日 00:14:22[xoyojank](https://me.csdn.net/xoyojank)阅读数：22461

# 下载 Native Client SDK
[https://developers.google.com/native-client/sdk/download](https://developers.google.com/native-client/sdk/download)
![](https://img-my.csdn.net/uploads/201211/03/1351872115_3848.png)
需要设置两个环境变量, Python2.7.3的安装路径加到PATH里, NACL_SDK_ROOT设置为对应版本目录
![](https://img-my.csdn.net/uploads/201211/03/1351872218_9283.png)
CHROME_PATH这个是后面VS调试用的, 如果不用VS开发, 可以不设置
# C++ 教程
[https://developers.google.com/native-client/devguide/tutorial](https://developers.google.com/native-client/devguide/tutorial)
先确保所有环境变量有效
![](https://img-my.csdn.net/uploads/201211/03/1351872278_3584.png)
设置下Chrome, 让它能够运行本地的NaCl程序
![](https://img-my.csdn.net/uploads/201211/03/1351872397_6424.png)![](https://img-my.csdn.net/uploads/201211/03/1351872400_7116.png)
编译: nacl_sdk\pepper_21\examples>make
启动服务器: nacl_sdk\pepper_21\examples>httpd.cmd
运行示例: http://localhost:5103
![](https://img-my.csdn.net/uploads/201211/03/1351872518_5077.png)
使用这个退出: http://localhost:5103/?quit=1
# 使用Visual Studio调试
[https://developers.google.com/native-client/dev/devguide/devcycle/vs-addin](https://developers.google.com/native-client/dev/devguide/devcycle/vs-addin)
前提条件:
- 64位的Vista/Win7/Win8
- VS2010 SP1
- Chrome 23或更高
- NaCl SDK pepper_23(或更高)
- CHROME_PATH 环境变量指向chrome.exe
- NACL_SDK_ROOT 环境变量指向nacl_sdk\pepper_23\
下载测试版本SDK: 
nacl_sdk>naclsdk update pepper_23
下载VS插件: 
nacl_sdk>naclsdk update vs_addin
安装插件: 
vs_addin\install.bat
打开工程
vs_addin\examples\hello_world_gles\hello_world_gles.sln
平台选NaCl64, 编译运行
![](https://img-my.csdn.net/uploads/201211/03/1351873047_6316.png)
调试需要把平台换成PPAPI
![](https://img-my.csdn.net/uploads/201211/03/1351873100_3808.png)
搞定, 之后就可以用VS开发和调试了!

