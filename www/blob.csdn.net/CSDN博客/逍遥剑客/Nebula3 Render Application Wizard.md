# Nebula3 Render Application Wizard - 逍遥剑客 - CSDN博客
2009年06月30日 20:33:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2077标签：[wizard																[application																[thread																[测试](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=wizard&t=blog)
个人分类：[引擎研究](https://blog.csdn.net/xoyojank/article/category/337622)
所属专栏：[Nebula引擎研究](https://blog.csdn.net/column/details/thenebuladevice.html)
使用方法同[N3ConsoleAppWizard](http://blog.csdn.net/xoyojank/archive/2009/06/08/4253123.aspx)
1. 修正stdafx.h第一次编译时会找不到的问题
2. 输出目录定位到$(N3SDK)test/win32/下, 这样程序可以自动加载export*.zip里的系统资源
3. DEBUG版的输出文件会加上_d后缀(因为2中输出目录改成跟RELEASE一样了)
注意: 环境变量$(N3SDK)已经改为SDK的根目录, 如"D:/Nebula3 SDK (Apr 2009)/"
Demo截图是一个会随时间变化颜色的矩形
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/EntryImages/20090630/N3RenderAppWizard.JPG)
这里直接发送消息来注册plugin的, 测试代码是写在plugin里:
//------------------------------------------------------------------------------
/**
*/
bool
TestViewerApp::Open()
{
	n_assert(!this->IsOpen());
	if (ViewerApplication::Open())
	{
		// register render thread plugin
		RegisterRTPlugin* msg = RegisterRTPlugin::Create();
		msg->SetType(&MyRTPlugin::RTTI);
		this->graphicsInterface->Send(msg);
		return true;
	}
	return false;
}
//------------------------------------------------------------------------------
/**
*/
void
TestViewerApp::Close()
{
	n_assert(this->IsOpen());
	// unregister render thread plugin
	UnregisterRTPlugin* msg = UnregisterRTPlugin::Create();
	msg->SetType(&MyRTPlugin::RTTI);
	this->graphicsInterface->Send(msg);
	ViewerApplication::Close();
}
