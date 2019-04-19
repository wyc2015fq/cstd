# vs2008去掉“正在更新IntelliSense” - 三少GG - CSDN博客
2013年03月06日 10:37:44[三少GG](https://me.csdn.net/scut1135)阅读数：1088

使用VS2008或者2005时候，当切换配置，总是显示“正在更新IntelliSense...”。例如从Debug切换到Release，这时候VS开始更新IntelliSense，CPU占用上升到100%。甚至导致vs2008未响应。
解决方法:
将/Microsoft Visual Studio 9.0/VC/vcpackages/feacp.dll改名，或者直接删除.反正用Visual Assist完全可以取代VS这个功能了.
不过我一般改成feacp.dll.backup.因为以后的ATL控件还会用到这个东东.
修改之后变得很快了.
"
过了些天，要弄一个atl控件出来。新建一个atl控件dll出来后，添加一个atl简单类时，却不能成功，报该类只能在完全的atl控件下。高手一看，感觉不对，以前他也是这样做的，而且这个就是一个完全的atl控件。百思不得其解下，想起当日对vc做了一下手脚，心里过意不去。于是把feacp.dll.backup改回了feacp.dll，重启了一下vc，再来了一把，添加类成功.
"
