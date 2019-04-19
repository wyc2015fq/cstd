# 打造最小的DirectX安装程序 - 逍遥剑客 - CSDN博客
2008年12月22日 10:41:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2559
DirectX Runtime
核心组件有Direct3D, DirectSound和DirectInput, 可以看作是操作系统的一部分. DirectX9.0c的核心组件自从DirectX SDK Summer 2004后就再也没有变过了, 除了Vista上的DX10. 而DirectX9.0c已经包含在WinXP SP2中了, 我想现在应该还没有人在用SP1或2000而不装DX9.0c的吧?(除非不玩游戏). 换句话说, 我们可以假设核心组件在用户的机器都已经存在了, 我们没有必要再给他们安装一次. 
可选的组件有D3DX库, XACT, XINPUT和Managed DirectX. 
DirectX 的安装
在SDK/Redist目录下已经包含了一份完整的DX Runtime安装程序, 完全可以随应用程序一起发布给用户. 如果不想显示安装界面, 可以用"dxsetup.exe/silent"来调用安装程序. 
DirectX安装程序至少需要以下几个文件:
- dxsetup.exe
- dsetup32.dll
- dsetup.dll
- dxupdate.cab
- dxdllreg_x86.cab
加起一共才3M. 当然, 这只是安装程序, 并不包含你所要升级的组件. 
首先, 如果不是64位程序(32位程序在64位系统下使用的仍然是32位库), 那么所有的*_x64.cab都可以去掉. 
如果核心库不需要安装(前面说过, 假设用户机器上已经存在DirectX9.0c), 那么DirectX.cab和dxnt.cab都可以去掉. 
然后是可选组件, 如果你使用的是August 2008的SDK, 而一般情况下都会用到D3DX库, 所以Aug2008_d3dx9_39_x86.cab是必需的. 至于XACT, XINPUT和MDX用到哪个加上哪个的cab. 
如果没有使用DirectShow, BDA.cab, BDANT.cab和BDAXP.cab也可以忽略(也可以假设用户机器上已经存在)
好了, 经过这些精简, 剩下的文件应该不会超过10M^_^, 对于一个客户端来说是九牛一毛, 嘿嘿
