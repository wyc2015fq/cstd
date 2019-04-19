# 如何解决MFCO42D.dll 0xC0000005:Access Violation问题 - 2019跟着小虎玩着去软考 - CSDN博客
2010年03月16日 11:31:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：6180
**如何解决****MFCO42D.dll 0xC0000005:Access Violation****问题**
# 一．问题的提出：
在甲电脑，就是笔者的电脑，程序可以正常运行；
但是在乙电脑，就是同事的电脑，程序运行却出现（MFCO42D.DLL）:0xC0000005:Access Violation错误信息。
# 二．错误提示：
（MFCO42D.DLL）:0xC0000005:Access Violation
# 三．程序死机所在行：
COleDropTarget::Register(pGridCtrl)；
# 四．原因分析：
【Tools】--【Options】--【Directories】--【Show directories for：】
**Library files****选择的库是**
C:/Program Files/Microsoft Visual Studio/VC98/MFC/LIB
**Executable files**
C:/WINDOWS/system32
在笔者的机器上，链接的是MFCO42D.DLL的6.0.8168.0版本
在同事的机器上，链接的是MFCO42D.DLL的6.0.8972.0版本
而笔者又将MFCO42D.DLL放到了笔者运行程序的Debug目录，注意：该库的版本的是6.0.8168.0版本。
程序每次运行的时候：就会首先从Debug目录查找MFCO42D.DLL动态链接库，加载运行。
在乙电脑上，编译的是MFCO42D.LIB的6.0.8972.0版本，而加载的是Debug目录下的6.0.8168.0版本的MFCO42D.DLL动态链接库，版本不匹配，故而运行出错。
# 五．甲乙电脑详细比较是：
|**比较项**|**甲电脑**|**乙电脑**|
|----|----|----|
|MFCO42D.LIB的大小|1,502,076bytes|1,043,972bytes|
|MFCO42D.LIB的Created时间|1998年6月17日, 0:00:00|Friday,February 27,2004,12:00:00 AM|
|MFCO42D.DLL的大小|798,773 bytes|827,445 bytes|
|MFCO42D.DLL的版本号|6.0.8168.0|6.0.8972.0|
# 六．解决方案：
1．将运行程序的Debug目录下的MFCO42D.DLL的文件删除掉；
2．让甲乙两台电脑的VC6开发环境一样，都是VC6+SP6补丁。
