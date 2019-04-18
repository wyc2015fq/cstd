# VTK嵌入MFC同步显示 - wishchinYang的专栏 - CSDN博客
2015年05月24日 14:13:26[wishchin](https://me.csdn.net/wishchin)阅读数：2309
         使用VTK嵌入MFC，实现四视图更新，机制和细节参考原文。
         原文链接：[http://blog.csdn.net/www_doling_net/article/details/8939115](http://blog.csdn.net/www_doling_net/article/details/8939115)
原文代码：
```cpp
#pragma once
#include <vtkResliceCursor.h>
#include <vtkResliceCursorWidget.h>
#include <vtkPlane.h>
#include <vtkPlaneSource.h>
#include <vtkPlaneWidget.h>
#include <vtkImagePlaneWidget.h>
#include <vtkResliceCursorThickLineRepresentation.h>
#include <vtkResliceCursor.h>
#include <vtkCommand.h>
#include <vtkViewport.h>
#include <vtkViewDependentErrorMetric.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRendererSource.h>
#include <vtkRenderingOpenGLModule.h>
#include <vtkRenderWindow.h>
#include <vtkWin32OpenGLRenderWindow.h>
#include <vtkWin32RenderWindowInteractor.h>
// CVtkView
class CVtkView : public CStatic
{
	DECLARE_DYNAMIC(CVtkView)
public:
	CVtkView();
	virtual ~CVtkView();
public:
	//3.2 重载CvtkView类PreSubclassWindow（）函数和OnPaint()函数
	//PreSubclassWindow函数负责创建VTK可视化管线，OnPaint()函数负责客户区内场景渲染。
	//vtkAcor,vtkRenderer,vtkRenderWindow,vtkRenderWindowInteractor四个部分。当然根据需要还可以设置vtkRenderWindowInteractorStyle,以及光照，材质，颜色等。
	//在CvtkView类头文件中定义相关对象，并在PreSubclassWindow函数中实例化和构建可视化管线
	void PreSubclassWindow();
	void SetImageData(vtkSmartPointer<vtkImageData> ImageData);
	void SetupReslice();
private:
	vtkSmartPointer< vtkImagePlaneWidget >   m_ImagePlaneWidget;
	vtkSmartPointer< vtkResliceCursorWidget> m_ResliceCursorWidget;
	vtkSmartPointer< vtkResliceCursor >      m_ResliceCursor;
	vtkSmartPointer< vtkResliceCursorThickLineRepresentation > m_ResliceCursorRep;
	vtkSmartPointer<vtkRenderer>         m_Renderer;
	vtkSmartPointer<vtkRenderWindow> m_RenderWindow;
	vtkSmartPointer<vtkImageData>       m_ImageData;
	//m_Direction为方向标志，取值分别为0,1和2，分别代表X轴，Y轴和Z轴方向，
	int m_Direction;
protected:
	DECLARE_MESSAGE_MAP()
};
```
原代码：
```cpp
// VtkView.cpp : implementation file
//
#include "stdafx.h"
#include "iisu2vrMfcs.h"
#include "VtkView.h"
// CVtkView
IMPLEMENT_DYNAMIC(CVtkView, CStatic)
CVtkView::CVtkView()
{
	//在实例化时需要注意，该视图类在默认情况下渲染的是vtkResliceCursorWidget对象的输出，
	//因此需要为vtkResliceCursorWidget对象指定相应的vtkRenderer对象，
	//m_ResliceCursorWidget->SetInteractor(m_RenderWindow->GetInteractor());
	//m_ResliceCursorWidget->SetDefaultRenderer(m_Renderer);
}
CVtkView::~CVtkView()
{
}
void CVtkView::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class  
	CRect rect;
	GetClientRect(rect);
	m_Renderer = vtkSmartPointer<vtkRenderer>::New();
	m_RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	m_RenderWindow->SetParentId(this->m_hWnd);
	m_RenderWindow->SetSize(rect.Width(), rect.Height());
	m_RenderWindow->AddRenderer(m_Renderer);
	if (m_RenderWindow->GetInteractor() == NULL)
	{
		vtkSmartPointer<vtkRenderWindowInteractor> RenderWindowInteractor =
			vtkSmartPointer<vtkRenderWindowInteractor>::New();
		RenderWindowInteractor->SetRenderWindow(m_RenderWindow);
		RenderWindowInteractor->Initialize();
	}
	m_RenderWindow->Start();
	CStatic::PreSubclassWindow();
}
void CVtkView::SetImageData(vtkSmartPointer<vtkImageData> ImageData)
{
	if (ImageData == NULL) return;
	m_ImageData = ImageData;
	SetupReslice();
}
void CVtkView::SetupReslice()
{
	if (m_ImageData == NULL) return;
	int dims[3];
	m_ImageData->GetDimensions(dims);
	//////////////////////////////////////////////////////////////////////////  
	m_ImagePlaneWidget->SetInputData(m_ImageData);
	m_ImagePlaneWidget->SetPlaneOrientation(m_Direction);
	m_ImagePlaneWidget->SetSliceIndex(dims[m_Direction] / 2);
	m_ImagePlaneWidget->On();
	m_ImagePlaneWidget->InteractionOn();
	//////////////////////////////////////////////////////////////////////////  
	m_ResliceCursor->SetCenter(m_ImageData->GetCenter());
	m_ResliceCursor->SetImage(m_ImageData);
	m_ResliceCursor->SetThickMode(0);
	//m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetResliceCursor(m_ResliceCursor);
	//m_ResliceCursorRep->GetResliceCursorActor()->GetCursorAlgorithm()->SetReslicePlaneNormal(m_Direction);
	m_ResliceCursorWidget->SetEnabled(1);
	m_Renderer->ResetCamera();
	//////////////////////////////////////////////////////////////////////////  
	double range[2];
	m_ImageData->GetScalarRange(range);
	m_ResliceCursorWidget->GetResliceCursorRepresentation()->
		SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
	m_ImagePlaneWidget->SetWindowLevel(range[1] - range[0], (range[0] + range[1]) / 2.0);
}
BEGIN_MESSAGE_MAP(CVtkView, CStatic)
END_MESSAGE_MAP()
```
添加类成员：
```cpp
public:
	afx_msg void OnBnClickedButtonViewshow();
	//CStatic m_ShowLU;
	//CStatic m_ShowRU;
	//CStatic m_ShowLD;
	//CStatic m_ShowRD;
	CVtkView m_ShowLU;
	CVtkView m_ShowRU;
	CVtkView m_ShowLD;
	CVtkView m_ShowRD;
	CVtkView m_AxialView;
	CVtkView m_SagittalView;
	CVtkView m_CoronalView;
	CVtkView m_3DView;
```
四视图更新代码：
```cpp
//当用户改变图像切分的坐标轴时（旋转坐标轴或者平移坐标系），图像切分平面会产生相应的改变，
//如果将新的切分平面更新到二维视图的vtkImagePlaneWidget对象中，即可实现三维视图的同步更新操作。
///基于以上设计，实现一个vtkCommand子类，来监听vtkResliceCursorWidget::ResliceAxesChangedEvent消息，并实现相应的更新操作。
class vtkResliceCursorCallback : public vtkCommand
{
public:
	static vtkResliceCursorCallback *New()
	{
		return new vtkResliceCursorCallback;
	}
	CVtkView* view[4];
public:
	void Execute(vtkObject *caller, unsigned long /*ev*/,
		void *callData)
	{
		vtkResliceCursorWidget *rcw = dynamic_cast<vtkResliceCursorWidget *>(caller);
		if (rcw)
		{
			for (int i = 0; i < 3; i++)
			{
				vtkPlaneSource *ps =
					static_cast<vtkPlaneSource *>(view[i]->GetImagePlaneWidget()->GetPolyDataAlgorithm());
				ps->SetOrigin(view[i]->GetResliceCursorWidget()->
					GetResliceCursorRepresentation()->GetPlaneSource()->GetOrigin());
				ps->SetPoint1(view[i]->GetResliceCursorWidget()->
					GetResliceCursorRepresentation()->GetPlaneSource()->GetPoint1());
				ps->SetPoint2(view[i]->GetResliceCursorWidget()->
					GetResliceCursorRepresentation()->GetPlaneSource()->GetPoint2());
				view[i]->GetImagePlaneWidget()->UpdatePlacement();
				view[i]->Render();
			}
			view[3]->Render();
		}
	}
	vtkResliceCursorCallback() {}
	
};
```
最终效果图：
![](https://img-blog.csdn.net/20150524141246044?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
