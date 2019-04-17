# 一个连接的错误__afxForceExclude Lnk2001 - 战斗蜗牛的专栏 - CSDN博客





2011年11月19日 10:21:24[vbskj](https://me.csdn.net/vbskj)阅读数：3255









rror LNK2019: 无法解析的外部符号 _WinMain@16，该符号在函数 ___tmainCRTStartup 中被引用


E:\c++\Projects\time\Debug\time.exe : fatal error LNK1120: 1 个无法解析的外部命令


生成日志保存在“file://e:\c++\Projects\time\time\Debug\BuildLog.htm”


time - 2 个错误，0 个警告


========== 生成: 0 已成功, 1 已失败, 0 最新, 0 已跳过 ==========


       要看你的 工程类型对不对，你用的是console还是windows？ 


如果你选择的是window类型，程序里面的主函数是main那就不对了，main是console下的主函数，windows编译主函数需要_WinMain函数，这些由编译器决定的


error lnk2001其中的一种错误情况


Watermarking1Doc.obj : error LNK2001: unresolved external symbol __afxForceEXCLUDE

Watermarking1.obj : error LNK2001: unresolved external symbol "protected: static struct AFX_MSGMAP const Watermarking1.obj : error LNK2001: unresolved external symbol "protected: virtual void __thiscall CDialog::PreInitDialog(void)" ([?PreInitDialog@CDialog@@MAEXXZ](mailto:?PreInitDialog@CDialog@@MAEXXZ))

DIALOG2.obj : error LNK2001: unresolved external symbol "protected: virtual void __thiscall DIALOG2.obj : error LNK2001: unresolved external symbol "public: virtual int __thiscall CDialog::OnInitDialog(void)" ([?OnInitDialog@CDialog@@UAEHXZ](mailto:?OnInitDialog@CDialog@@UAEHXZ))

Watermarking1View.obj : error LNK2001: unresolved external symbol [__imp__GetDeviceCaps@8](mailto:__imp__GetDeviceCaps@8)

LINK : error LNK2001: unresolved external symbol _WinMainCRTStartup




基本上300个错误，其实只是因为工程的配置出了问题。


把      project->setting->link   的忽略全部默认库   前面的勾去掉 ，即不选这一项，即可解



