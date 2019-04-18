# Duilib介绍 - 深之JohnChen的专栏 - CSDN博客

2017年06月02日 10:48:33[byxdaz](https://me.csdn.net/byxdaz)阅读数：2982



一、Duilib介绍

         Duilib是一个开源 的directui 界面库，开放，共享，惠众，共赢，遵循bsd协议，可以免费用于商业项目，目前支持Windows 32 、Window CE、Mobile等平台。由于简约易扩展的设计以及稳定高效的实现被各大互联网公司普遍接受，广泛应用于包括IM、视频客户端、股票行情软件、导航软件、手机辅助软件、安全软件等多个行业的众多pc客户端软件。Duilib还在不断的发展中，在文档、例子、动画、渲染引擎等多个方面将持续改进。

    提供所见即所得的开发工具UIDesigner。使用DirectUI后将使得我们的设计人员彻底解 放，不会受到开发的束缚，可以充分地发挥其设计能力来设计软件界面，并参与到用户界面开发过程中。 Duilib 目前支持Windows 32 、Window CE、Mobile等平台，使用C++开发，遵循BSD协议，可以免费用于商业项目。

[https://github.com/duilib/duilib](https://github.com/duilib/duilib)

二、Duilib整体**结构**

**![](https://img-blog.csdn.net/20161107141426336)**

三、Duilib框架基本流程

1）：实例句柄与渲染类关联

CPaintManagerUI::SetInstance(hInstance);

           CPaintManagerUI::SetResourcePath(LPCTSTRpStrPath)

2）：初始化COM库,为加载COM库提供支持

::CoInitialize

3）：创建窗口类，主函数中的第二段代码主要完成的是类CDuiFrameWnd对象的创建，我们跟到对应的构造函数中发现它并没有做多余的操作，现在先不管它是如何构造的，它下面就是调用了类的Create函数创建了一个窗口，这个函数的代码如下：

```cpp
HWND CWindowWnd::Create(HWND hwndParent, LPCTSTR pstrName, DWORD dwStyle, DWORD dwExStyle, int x, int y, int cx, int cy, HMENU hMenu)
{
    if( GetSuperClassName() != NULL && !RegisterSuperclass() ) return NULL;
    if( GetSuperClassName() == NULL && !RegisterWindowClass() ) return NULL;
    m_hWnd = ::CreateWindowEx(dwExStyle, GetWindowClassName(), pstrName, dwStyle, x, y, cx, cy, hwndParent, hMenu, CPaintManagerUI::GetInstance(), this);
    ASSERT(m_hWnd!=NULL);
    return m_hWnd;
}
```

4）：调用CWindowWnd::Create创建窗口，Create函数实现了注册窗口，指定回调函数，创建窗口，处理消息等。

5）：窗口居中

CWindowWnd::CenterWindow

6）：处理消息循环

CPaintManagerUI::MessageLoop

7）：CPaintManagerUI::MessageLoop

::CoUninitialize()

8）：窗口类中需要重写MessageHandle函数用于处理我们感兴趣的消息。并且在最后需要调用基类的MessageHandle函数，主要是为了调用DefWindowProc处理我们不感兴趣的消息。

四、实例

 //Helloworld.h

```cpp
#pragma once

#include <ObjBase.h>
#include <UIlib.h>
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
```


//Helloworld.cpp

```cpp
#include "Helloworld.h"

class CDuiFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame"); }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	virtual void    Notify(TNotifyUI & msg) 
	{
		if(msg.sType == _T("click"))
		{
			if (msg.pSender->GetName() == _T("btnHello"))
			{
				::MessageBox(NULL, _T("我是按钮"), _T("点击了按钮"), NULL);
			}
		}
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;

		if( uMsg == WM_CREATE ) 
		{
			m_pCloseBtn = new CButtonUI();
			m_pCloseBtn->SetName(_T("btnHello"));			//设置控件的名称，这个名称用于标识每一个控件，必须唯一
			m_pCloseBtn->SetText(_T("Hello World"));		//设置文字
			m_pCloseBtn->SetBorderSize(10);
			m_pCloseBtn->SetBkColor(0xFF00FF00);			 //设置背景色

			m_PaintManager.Init(m_hWnd);
			m_PaintManager.AttachDialog(m_pCloseBtn);
			m_PaintManager.AddNotifier(this);				//添加控件等消息响应，这样消息就会传达到duilib的消息循环

			return lRes;
		}
		else if(uMsg == WM_PAINT)
		{
			RECT rect = {100,100,200,150};
			m_pCloseBtn->SetPos(rect);
		}
		else if( uMsg == WM_DESTROY ) {
			::PostQuitMessage(0L);
		}
		else if( uMsg == WM_ERASEBKGND ) {
			return 1;
		}

		lRes = 0;
		if(m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
			return lRes;

		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

protected:
	CPaintManagerUI m_PaintManager;
	CControlUI *m_pCloseBtn;
};

//#define  SHOW_MODAL		1						//阻塞显示

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	HRESULT Hr = ::CoInitialize(NULL);
	if( FAILED(Hr) ) 
		return 0;

#ifdef SHOW_MODAL
	CDuiFrameWnd* pFrame = new CDuiFrameWnd();
	if( pFrame == NULL ) 
		return 0;

	pFrame->Create(NULL, _T("简单测试demo-1"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->ShowModal();
#else
	CDuiFrameWnd* pFrame = new CDuiFrameWnd();
	if( pFrame == NULL ) 
		return 0;

	pFrame->Create(NULL, _T("简单测试demo-2"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);
	CPaintManagerUI::MessageLoop();
#endif

	::CoUninitialize();
	return 0;
}
```

五、问题汇总

1、SetPos

如果要重新定义控件的位置，不能使用这个函数，而应该

SetFixedXY   SetFixedHeight    SetFixedWidth 这三个函数，

函数作用分别是：

设置x,y位置，有点奇怪，为啥不弄个POINT作为参数，

设置控件高度，设置控件宽度。

2、duilib xml嵌套

duilib使用嵌套xml可以简化代码的书写，有利于模块化的页面布局分解，duilib库的xml嵌套主要有两种方式

方式一、以创建控件的方式嵌套xml

在CreateControl(LPCTSTR pstrClassname )中创建自定义的控件，并绑定到xml文件

```
CControlUI* FrameWnd::CreateControl( LPCTSTR pstrClassName )
{
    if (_tcsicmp(pstrClassName, _T("Wnd_Xml_1")) == 0)
    {
        CDialogBuilder builder;
        CControlUI* pUI = builder.Create(_T("Wnd_Xml_2.xml")); 
        return pUI;
    }
    else if (_tcsicmp(pstrClassName, _T("Wnd_Xml_2")) == 0)
    {
        CDialogBuilder builder;
        CControlUI* pUI = builder.Create(_T("Wnd_Xml_2.xml")); 
        return pUI;
    }

    return NULL;
}
```

FrameWnd的XML文件使用时

```
<Wnd_Xml_1 height="520" />
 <Wnd_Xml_2 height="521" />
```

Wnd_Xml_1和Wnd_Xml_2则只需按照普通Window的方式来实现即可

方式二、使用TableLayout嵌套xml

```
<TabLayout name="wizard_setup_tab"  >
          <Include source="configpage.xml" />
        <Include source="installingpage.xml" />        
          <Include source="finishpage.xml" />
          <Include source="uninstallpage.xml" />
        <Include source="uninstallingpage.xml" />        
          <Include source="uninstallfinishpage.xml" />
    </TabLayout>
```

使用的时候需要在指定具体的xml进行加载

例如使用finishpage.xml

使用时需要如下操作：

```
CTabLayoutUI * setup_tab = static_cast <CTabLayoutUI*>(m_PaintManager.FindControl(_T("wizard_setup_tab")));
    setup_tab->SelectItem(2);
```

3、如何把资源放入zip？

先SetResourcePath设置资源目录，再SetResourceZip设置压缩资源文件名

4、如何设置鼠标可拖动窗体的范围大小？

设置XML文件的Window标签的caption属性。

5、如何设置窗体可以通过拖动边缘改变大小？

在窗体创建函数的第三个参数设置为UI_WNDSTYLE_FRAME才可响应拖动改变大小，和双击标题事件。

6、为何鼠标移动到边缘没有改变窗体大小的箭头出现，不能通过拖动改变窗体大小？

设置window标签的sizebox属性，例如sizebox="2,2,2,2"

7、窗体不可双击最大化如何实现？

在窗体创建函数的第三个参数设置为UI_WNDSTYLE_DIALOG。

8、应用程序exe图标如何设置？

使用窗体成员函数SetIcon，参数为资源icon的id。

9、初始化时，最大化窗体如何实现？

调用窗体的SendMessage给窗体发送最大化消息SC_MAXIMIZE，SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE,0);

10、动态改变窗体的大小如何实现？

使用窗体函数ResizeClient，参数分别重设的宽和高。

11、窗体透明度如何设置？

设置window标签属性bktrans="true" alpha="200" alpha的值为0-255。这种设置是全体窗体透明度，所有控件都将变透明。

如果想单纯设置背景透明度控件不透明度，可以制作半透明的背景图片，设置window标签的bktrans="true"，并且不设置alpha属性，切记！此时背景透明，其它控件不透明。

单独设置某个控件的透明度，可以使用图片的fade属性，或者mask属性。fade表示设置图片透明度，取值0-255。mask为设置透明的颜色。

12、如何自定义xml控件？

自定义控件和复杂的控件类型都是由简单基本控件组成。

在写好一个自定义的控件xml模板后，

CDialogBuilder dlg_builder;

CControlUI * pControl = dlg_builder.Create("item.xml");

注意这里的item.xml要放在主界面的xml所在的文件夹内，并且无需在指定路径了。

该函数返回一个CControlUI的一个句柄，得到这样一个句柄就是一个控件了。

如果要获取复杂控件的某个子控件的句柄，然后想通过该句柄改变子控件的状态。

首先给这个子控件取一个名字，然后可以通过pControl的FindSubControl("name")来获取该

控件的句柄了。得到句柄后就可以设置它的所有属性了，例如

pbtn->SetAttribute(_T("normalimage"), _T("file='images\\downlist_ok.png' dest='20,14,32,26'"));就能更改它的状态图片了。

13、多线程下如何更改dui的界面信息？

线程里不要操作界面的信息，应该通过SendMessage或者PostMessage给界面的m_hWnd发送自定义消息。然后在界面的消息循环里面在做操作界面的动作。

自定义处理的消息处理函数可以从重写方法

LRESULT MyWnd::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )

{

//这里处理完后，bHandled置true 返回基类，让基类去操作

return WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);

}

具体可以如下使用：

#define ON_PERCENT_MSG              WM_USER + 500

然后在线程函数中发送消息给界面

int DownloadView::on_percent( double percent, int index, INT_PTR user_data )

{

    ::SendMessage(m_hWnd, ON_PERCENT_MSG, (WPARAM)&percent, (LPARAM)user_data);

    return 0;

}

最后在界面消息循环中进行处理消息

LRESULT DownloadView::HandleCustomMessage( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )

{

    switch (uMsg)

    {

    case ON_PERCENT_MSG:

         // 处理界面相关的操作

        break;

    default:

        break;

    }

    return WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);

}

14、duilib绘图部分

图像的绘制大部分使用了绘制引擎的DrawImageString函数，该函数实现知道图像名称绘制到指定目标区域的功能

并且通过指定pStrModify的属性能够进行不同需求的绘制，例如，仅扣取源图像的某个位置绘制到目标的某个区域、

设置四边圆角绘制、为bmp等指定某种颜色为透明颜色、平铺或者拉伸绘制、设置透明度等。

DrawImageString最终又调用CRenderEngine::DrawImage进行绘制。

duilib中所有元素的显示都在整个消息循环的WM_PAINT中进行绘制。

第一次绘制之前会发送一个名为_T("windowinit")的通知。

然后为CControlUI的绘制，绘制的顺序为：背景颜色->背景图->状态图->文本->边框

会顺序调用CControlUI的以下函数。  

1.PaintBkColor(hDC);            // 绘制背景颜色  

2.PaintBkImage(hDC);          // 绘制背景图  

3.PaintStatusImage(hDC);   // 绘制状态图  

4.PaintText(hDC);                 // 绘制文本  

5.PaintBorder(hDC);            // 绘制边框 

所以有需求在界面上动态绘制一些内容时，可以通过CControlUI进行子类化，然后重写PaintStatusImage，

在PaintStatusImage里面进行绘图操作。绘制时可以直接调用duilib绘制引擎进行绘制，比较便捷。当然也可以使用GDI+等库直接进行绘制。

duilib的图像文件的加载是通过一个开源库stb_image.c实现。

并只使用了该库的stbi_load_from_memory和stbi_image_free这两个功能函数。

该库的链接地址：http://nothings.org/stb_image.c

该库对图片的格式的支持情况说明如下：

JPEG 支持Baseline标准型的JPEG，不支持渐进式Progressive的JPEG

PNG   仅支持8位的png图像

BMP  不支持1bpp的bmp，不支持行程编码RLE的bmp

PSD  紧显示合成的图像，不支持额外的通道

GIF *comp always reports as 4-channel

stb_image & stb_image_write库使用方法 参考https://blog.csdn.net/qq_40953281/article/details/87902580

所以在duilib中使用的图片时需要注意到这个限制，否则有些图片将显示不出来。

duilib中把图片文件加载到内存后，由库stb_image.c解析并最终转为DIB处理。

duilib支持从图片文件、从包含图片文件的压缩包文件和VC资源中载入图片。

载入后最终返回TImageInfo结构体指针，包含HBITMAP、图像宽度、图像高度、alpha通道、mask和资源类型等信息。

载入函数为：TImageInfo* CRenderEngine::LoadImage(STRINGorID bitmap, LPCTSTR type, DWORD mask);

duilib中所有的图片资源只从加载一次，然后保存在CPaintManagerUI中的m_mImageHash成员中，该成员为一个

hashmap存储TImageInfo*信息，当已经存在要绘制显示的图片信息时，直接从map中获取显示，否则从文件加载

在添加到map中。

duilib 动态加载非资源图片

思路：使用bitmap加载外部图片，将Bitmap转为HBITMAP，加载到paintmanager中，然后控件加载paintmanager中的HBITMAP资源。

Bitmap btm(_T(“E:\pic\test.bmp”));

HBITMAP hBitmap = NULL;

if (Gdiplus::Ok == btm.GetHBITMAP(NULL, &hBitmap))

{

    CDuiString sCtrlKey = _T("your_key_name");

    if(m_pCodeLab)

    {    

        m_PaintManager.RemoveImage(sCtrlKey);    

        m_PaintManager.AddImage(sCtrlKey, hBitmap,w, h, false);        //w图片宽度  h图片高度

        m_pCodeLab->SetBkImage(sCtrlKey);

    }

}

15、如何管理duilib界面皮肤资源？

把所有XML文件在同一个目录下，图片资源另外放在一个地方。当如下进行布局资源时，skin目录为设置资源的目录，imagedir1目录放置1.xml文件利用到的图片资源，imagedir2放置2.xml文件利用到的资源。

然后1.xml文件文件中的图片资源名称需要加上相对于当前xml文件的路径信息即可。例如：normalimage="imagedir1/1.png"这样的形式。使用设计器为XML添加资源时会自动转化为相对路径。

skin/

1.xml

2.xml

imagedir1/

1.png

2.png

imagedir2/

1.png

2.png

16、Combo不显示值的问题

需要pCombo->SetInternVisible(true);

Combo

相关xml:

 <Combo name="comboUpdateFrequency"  width="60" height="30" normalimage="file='ComboBox/Combo_nor.bmp' corner='5,2,30,2'" hotimage="file='ComboBox/Combo_over.bmp' corner='5,2,30,2'" pushedimage="file='ComboBox/Combo_over.bmp' corner='5,2,30,2'" >

    <ListLabelElement name="listDay" text=" 每天" selected="true"/>

    <ListLabelElement name="listWeek" text=" 每周" />

    <ListLabelElement name="listMonth" text=" 每月" />

</Combo>

相关代码片段：

void SetUpdateFrequency(int iUpdateFrequency)

    {

        CComboUI* pComboUpdateFrequency = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("comboUpdateFrequency")));

        pComboUpdateFrequency->SetInternVisible(true);

        if (iUpdateFrequency == 0)

        {

            pComboUpdateFrequency->SelectItem(0);

        }

        else if (iUpdateFrequency == 1)

        {

            pComboUpdateFrequency->SelectItem(1);

        }

        else if (iUpdateFrequency == 2)

        {

            pComboUpdateFrequency->SelectItem(2);

        }

    }

17、RECT类型示例

RECT rc;

rc.left = 20;

rc.top = 0;

rc.right = 9999;

rc.bottom = 9999;

CTextUI* pText = new CTextUI;

pText->SetFloat(true);

pText->SetPos(rc);

18、TreeNode的bug

如果直接在TreeNode设置文字，那么textcolor属性无效，需要使用

itemattr="textcolor="#FFFF0000" font="3"" 

但是此属性在鼠标悬浮后失效（如果是黑色的无影响），目前为深入研究解决方案。所以推荐采用TreeNode嵌套Text实现。

19、不显示边框阴影

window必须roundcorner属性才能实现阴影。

<Window size="920,600" mininfo="920,600"  roundcorner="5,5" caption="0,0,0,30" sizebox="4,4,4,4" >

</Window>

20、ApplyAttributeList 陷阱

name="123" pos="0,0,10,10"

1）. “=”两边不能有任何的空格

2）.对于属性之间必须有且只能有一个空格

21、基于Duilib的多标签浏览器（IE内核）

https://blog.csdn.net/duisharp/article/details/54342964

22、Duilib中基于wke的浏览器控件（webkit内核）

https://blog.csdn.net/mfcing/article/details/43973377

23、duilib自绘

方法1：从CControlUI派生一个子类，然后直接重写DoPaint接口，在里面绘制即可。这样有1个限制，duilib默认是后添加的控件Z序高，而实际绘制全部控件的时候，Z序高的控件在上层。因此如果这个控件的Z序比较低，是有可能被其他控件盖住的，这个要看具体的布局了。

方法2：从CControlUI派生一个子类，然后直接重写DoPostPaint接口。然后调用PaintManager的AddPostPaint接口将这个控件指针加入的m_aPostPaintControls数组即可。此时在全部控件绘制完毕后，会触发DoPostPaint接口的调用，在里面绘制即可。这种方法与1的不同之处在于，仍然保留了原先控件的绘制。并且是在所有控件绘制之后绘制的，因此它的绘制好像是在最上层的，不会由于控件的Z序等导致绘制被其他控件覆盖。

24、Duilib使用wke显示echarts

https://www.cnblogs.com/fyluyg/p/6150545.html

25、duilib模态对话框

```
模态对话框的创建:风格不能变，否则找不到父窗口指针

       m_pNewCasesDlg = new NewCasesDlg();
        if (m_pNewCasesDlg == NULL) { Close(); return; }    
       m_pNewCasesDlg->Create(this->GetHWND(), _T(""), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);        
        //    m_pLoginFrame->SetIcon(IDI_ICON_DUILIB);
        m_pNewCasesDlg->CenterWindow();     
        m_pNewCasesDlg->ShowModal(); 
        m_pNewCasesDlg = NULL;

重载OnFinalMessage在退出时Close(IDOK)销毁自己

void CMingdeDlg::OnFinalMessage(HWND hWnd )
{
    WindowImplBase::OnFinalMessage(hWnd);
    delete this;

}
```

