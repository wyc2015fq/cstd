# 如何知道应用程序使用了哪些DLL文件 - 2019跟着小虎玩着去软考 - CSDN博客
2010年03月16日 17:19:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：3948
**如何知道应用程序使用了哪些****DLL****文件**
使用Dependency Walker的软件
# 一．软件从哪儿启动
具体是【Start】Windows主菜单---【Program】--【Microsoft Visual C++ 6.0】---【Microsoft Visual C++ 6.0 Tools】---【Depends】
# 二．Depends
【File】---【Open】打开应用程序即可
# 三．跟VC相关的DLL文件
就是以MSVC开头命名的DLL动态链接库，如
## Debug版：
MSVCRTD.DLL
MSVCP60D.DLL
## Release版：
MSVCRTD.DLL
MSVCP60D.DLL
# 四．跟MFC相关的DLL文件
就是以MFC开头命名的DLL动态链接库，如
## Debug版：
MFCO42D.DLL
MFC42D.DLL
## Release版：
MFCO42.DLL
MFC42.DLL
