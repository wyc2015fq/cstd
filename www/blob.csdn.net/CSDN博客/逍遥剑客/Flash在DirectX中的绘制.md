# Flash在DirectX中的绘制 - 逍遥剑客 - CSDN博客
2010年06月27日 22:34:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：6692标签：[flash																[direct3d																[编程																[mfc																[优化																[脚本](https://so.csdn.net/so/search/s.do?q=脚本&t=blog)](https://so.csdn.net/so/search/s.do?q=优化&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=direct3d&t=blog)](https://so.csdn.net/so/search/s.do?q=flash&t=blog)
个人分类：[视频音频](https://blog.csdn.net/xoyojank/article/category/484788)
这里使用的是之前我说过的[OLE控件在Direct3D中的渲染方法](http://blog.csdn.net/xoyojank/archive/2008/03/15/2185829.aspx), 自己不进行swf的解析, 这不现实. 
- 前提条件: COM编程, OLE编程, DirectX编程, ActionScript/Flash(可选). 
- 创建一个ShockwaveFlashObjects::IShockwaveFlash的对象
- 实现一个IOleClientSite来做为IShockwaveFlash的容器
- 绘制 
- 通过OleDraw来把GDI的像素数据绘制到DC上(IShockwaveFlash是一个IViewObject)
- 把DC的像素数据拷贝到D3D的Texture上. 中间涉及像素格式的内存操作, 需要明白图像数据的内存格式.
- 半透明支持(可选): 如果不需要半透明支持的话, 其实可以直接OleDraw到Texture的DC上, 不用再多一次拷贝. 但是有时候不得不用(像UI), 可以这参考[Transparent Flash Control in plain C++](http://www.codeproject.com/KB/COM/flashcontrol.aspx), 用黑色背景和白色背景绘制两次, 比较两次结果 的Red通道计算出相应的Alpha值.
- 脏矩形优化: 如果每帧都进行整个纹理的更新, 你会发现CPU占用率相当高. 实际上ActiveX本身是有这个优化的, 只不过我们需要自己找出这个矩形. 通过实现IOleInPlaceSiteWindowless::InvalidateRect可以得到相应的"脏"矩形区域, 然后只需要更新相应的纹理区域就可以让效率达到商用的标准.
- 事件响应 
- 幸运的是, IShockwaveFlash是一个IOleInPlaceObjectWindowless对象, 那么我们就可以直接使用IOleInPlaceObjectWindowless::OnWindowMessage进行消息的处理. 注意鼠标坐标的转换和键盘输入焦点的控制.
- 脚本交互, 参考[MFC的思路](http://blog.csdn.net/xoyojank/archive/2008/10/23/3122679.aspx): 
- C++调用AS: IShockwaveFlash::CallFunction/SetVariable
- AS调用C++: 需要实现ShockwaveFlashObjects::_IShockwaveFlashEvents, 可以监听fscommand和ExternalInterface.call两种调用方式. 
关于具体实现, 已经有人先于我完成了, 所以请直接参考[Hikari](http://code.google.com/p/hikari-library/). 接下来再把IE搞定
