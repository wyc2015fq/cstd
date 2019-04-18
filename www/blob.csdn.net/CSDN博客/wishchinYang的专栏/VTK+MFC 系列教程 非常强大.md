# VTK+MFC 系列教程 非常强大 - wishchinYang的专栏 - CSDN博客
2013年09月27日 12:58:08[wishchin](https://me.csdn.net/wishchin)阅读数：5654
虽然QT才是王道！MFC的懂一些也是好的.
原文链接：[http://blog.csdn.net/www_doling_net/article/details/8939115](http://blog.csdn.net/www_doling_net/article/details/8939115)
之前介绍了基于VTK的单文档应用程序开发，并以图像重采样为例，实现了一个简单的图像重采样的应用程序。对于多文档应用程序，与单文档应用程序基本一致，这里就不再讲述。对话框应用程序是MFC应用程序中一个使用非常广泛的框架，本节就以医学图像可视化中常用的四视图框架程序的实现为例，讲述基于VTK的对话框应用程序开发。
#### 1.  利用VS和CMake建立一个空的MFC对话框程序框架。
利用VS创建一个MFC对话框工程vtkDialog，删除其中的工程文件，完成CMakeLists.txt文件，并添加相应的代码文件和链接VTK动态库，利用CMake配置完毕后，打开生成的工程文件vtkDialog.sln，编译执行，即可得到一个空的对话框程序。其中CvtkDialogDlg为该程序的主对话框类。
#### 2.  设计用户界面，添加相应的控件
本程序需要实现的功能有（1）图像读取和管理；（2）图像切分和浏览。一个常见的医学图像可视化程序，包括四个视图，横断面视图，矢状面视图，冠状面视图和三维视图。因此，基于以上设计，我们添加一个树控件，MFC中对应的控件类为CTreeCtrl。树控件是最常用的文件管理控件，能够方便的对文件进行层次化组织和管理。四视图的实现则需要四个控件，这里我们选择CStatic控件，将其添加至对话框窗口中。添加完毕后，为控件生成相应的Control类型的变量。
按照上述设计，需要在CStatic中显示图像。这就需要对CStatic类继续扩展，使其支持VTK可视化管线。一个可行的方法是，设计一个CStatic类的子类，并在该子类中实现VTK可视化管线和处理。
#### 3.  实现VTK图像可视化控件
##### 3.1 首先添加一个MFC类CvtkView
其基类选择为CStatic，并添加至CMakeLists.txt文件中进行管理。
##### 3.2 重载CvtkView类PreSubclassWindow（）函数和OnPaint()函数
PreSubclassWindow函数负责创建VTK可视化管线，OnPaint()函数负责客户区内场景渲染。
##### 3.3 建立VTK可视化管线
VTK可视化管线在第二章中已经介绍过，其中最主要包含 vtkAcor,vtkRenderer,vtkRenderWindow,vtkRenderWindowInteractor四个部分。当然根据需要还可以设置vtkRenderWindowInteractorStyle,以及光照，材质，颜色等。在CvtkView类头文件中定义相关对象，并在PreSubclassWindow函数中实例化和构建可视化管线，代码如下。
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8939115#)[copy](http://blog.csdn.net/www_doling_net/article/details/8939115#)
- void CvtkView::PreSubclassWindow()  
- {  
- // TODO: Add your specialized code here and/or call the base class
-     CRect rect;  
-     GetClientRect(rect);  
- 
-     m_Renderer = vtkSmartPointer<vtkRenderer>::New();  
- 
-     m_RenderWindow = vtkSmartPointer<vtkRenderWindow>::New();  
-     m_RenderWindow->SetParentId(this->m_hWnd);  
-     m_RenderWindow->SetSize(rect.Width(), rect.Height());  
-     m_RenderWindow->AddRenderer(m_Renderer);  
- 
- if(m_RenderWindow->GetInteractor() == NULL)  
-     {  
-         vtkSmartPointer<vtkRenderWindowInteractor> RenderWindowInteractor =  
-             vtkSmartPointer<vtkRenderWindowInteractor>::New();  
-         RenderWindowInteractor->SetRenderWindow(m_RenderWindow);  
-         RenderWindowInteractor->Initialize();  
-     }  
- 
-     m_RenderWindow->Start();   
-     CStatic::PreSubclassWindow();  
- }  
相信通过前面的学习，这里建立可视化管线的流程已经比较熟悉了。需要注意的是，vtkRenderWindow需要通过函数vtkRenderWindow::SetParentId()来建立与控件本身的关联，这样才能将m_RenderWindow中的渲染内容在控件的窗口上进行显示；而vtkRenderWindow::SetSize()则是设置渲染窗口与当前控件客户区保持大小一致。
大家可能会有疑问，怎么没有vtkActor？正常的一个可视化管线中，vtkActor表示需要进行渲染或者绘制的对象。这里需要渲染的对象是图像，而与以前不同的是，没有直接去定义一个图像vtkActor。至于原因暂且不管，随着该类功能的逐步完善，我们再详细说明。VTK渲染管线建立完毕后在OnPaint()函数中调用vtkRenderWindow的Render()函数来实现渲染。
到这里一个基本的VTK显示控件已经实现，在设计界面时，通过MFC自动添加的四个视图变量类型默认为CStatic。由于CvtkView是继承自CStatic，因此我们可以直接将主对话框类CvtkDialogDlg头文件中定义的四个变量类型修改为CvtkView。然后编译运行程序，是不是已经出现了一个四视图的原型了（如下图所示）？由于没有添加任何的渲染对象，因此四个视图均为空的黑色窗口。
![](https://img-blog.csdn.net/20130517123843162)
##### 3.4 交互式图像切分
该控件需要实现两个基本功能：一是交互式图像切分；二是切片图像提取。第一个功能，采用vtkResliceCursorWidget和vtkResliceCursor类。通常两个类同时使用，每个vtkResliceCursorWidget对象中需要定义相应的vtkResliceCursor对象。vtkResliceCursorWidget通过定义的“十”字坐标轴，提供用户方便的切分和交互方式，支持坐标轴的旋转和平移；当坐标系发生改变时即调用vtkResliceCursor来进行图像切分并进行更新到vtkRenderer对象中。
第二个功能采用vtkImagePlaneWidget实现。该类内部定义了一个vtkImageReslice对象，利用vtkResliceCursor中定义的切分平面来切分图像，在其内部通过纹理映射来绘制到一个平面上，并在用户指定的vtkRenderer进行显示。
另外前面在定义可视化管线时，我们并没有定义相关的vtkActor。这主要是因为在视图显示图像时，都是通过相关的widget来实现，我们只需要为widget对象设置相应的vtkRenderer即可，在其内部会自动生成相应的vtkActor对象。根据以上分析在头文件中定义相关对象并在PreSubclassWindow函数中进行实例化。
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8939115#)[copy](http://blog.csdn.net/www_doling_net/article/details/8939115#)
- vtkSmartPointer< vtkImagePlaneWidget >   m_ImagePlaneWidget;  
- vtkSmartPointer< vtkResliceCursorWidget> m_ResliceCursorWidget;  
- vtkSmartPointer< vtkResliceCursor >      m_ResliceCursor;  
- vtkSmartPointer< vtkResliceCursorThickLineRepresentation > m_ResliceCursorRep;  
 在实例化时需要注意，该视图类在默认情况下渲染的是vtkResliceCursorWidget对象的输出，因此需要为vtkResliceCursorWidget对象指定相应的vtkRenderer对象，
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8939115#)[copy](http://blog.csdn.net/www_doling_net/article/details/8939115#)
- m_ResliceCursorWidget->SetInteractor(m_RenderWindow->GetInteractor());   
- m_ResliceCursorWidget->SetDefaultRenderer(m_Renderer);  
这样在vtkResliceCursorWidget对象内部会将切片图像转化为一个vtkActor对象并添加至指定的vtkRenderer对象中进行显示渲染；而vtkImagePlaneWidget中也有一个重要的函数vtkImagePlaneWidget::SetDefaultRenderer(vtkRenderer*) 用于设置相应的vtkRenderer来显示切片。根据本程序的设计，vtkImagePlaneWidget产生的切片需要在三维场景中显示，因此这里并没有调用。也就是说，在默认情况下，本控件类只显示二维切片图像。
##### 3.5 添加图像设置函数并初始化图像切分对象
该控件类需要从外部设置相应的处理图像，因此提供一个接口函数来供外部调用。在3.4中仅仅定义和创建了交互式图像切分对象，并没有设置相应的输入数据，因此每次有新的数据传入时，需要为其进行初始化。
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8939115#)[copy](http://blog.csdn.net/www_doling_net/article/details/8939115#)
- void CvtkView::SetImageData(vtkSmartPointer<vtkImageData> ImageData)  
- {  
- if (ImageData == NULL ) return;  
- 
-     m_ImageData = ImageData;  
-     SetupReslice();  
- }  
- void CvtkView::SetupReslice()  
- {  
- if (m_ImageData == NULL) return;  
- int dims[3];  
-     m_ImageData->GetDimensions(dims);  
- 
- //////////////////////////////////////////////////////////////////////////
-     m_ImagePlaneWidget->SetInput(m_ImageData);   
-     m_ImagePlaneWidget->SetPlaneOrientation(m_Direction);   
-     m_ImagePlaneWidget->SetSliceIndex(dims[m_Direction]/2);   
-     m_ImagePlaneWidget->On();   
-     m_ImagePlaneWidget->InteractionOn();   
- 
- //////////////////////////////////////////////////////////////////////////
-     m_ResliceCursor->SetCenter(m_ImageData->GetCenter());   
-     m_ResliceCursor->SetImage(m_ImageData);   
-     m_ResliceCursor->SetThickMode(0);   
- 
-     m_ResliceCursorRep->GetResliceCursorActor()->   
-         GetCursorAlgorithm()->SetResliceCursor(m_ResliceCursor);  
-     m_ResliceCursorRep->GetResliceCursorActor()->   
-         GetCursorAlgorithm()->SetReslicePlaneNormal(m_Direction);  
- 
-     m_ResliceCursorWidget->SetEnabled(1);   
-     m_Renderer->ResetCamera();   
- 
- //////////////////////////////////////////////////////////////////////////
- double range[2];   
-     m_ImageData->GetScalarRange(range);   
-     m_ResliceCursorWidget->GetResliceCursorRepresentation()->  
-         SetWindowLevel(range[1]-range[0], (range[0]+range[1])/2.0);   
-     m_ImagePlaneWidget->SetWindowLevel(range[1]-range[0], (range[0]+range[1])/2.0);   
- }  
SetupReslice()函数中首先对vtkImagePlaneWidget对象进行初始化，
vtkImagePlaneWidget::SetInput() 设置输入图像
vtkImagePlaneWidget::SetPlaneOrientation()设置切片的方向
vtkImagePlaneWidget::SetSliceIndex() 设置当前方向上默认的层号
注意一定要在设置完输入图像和相应的交互对象后，再开启vtkImagePlaneWidget，相应的函数为：
vtkImagePlaneWidget::On(); 
vtkImagePlaneWidget::InteractionOn();
然后设置vtkResliceCursor对象，
vtkResliceCursor::SetInput() 设置输入图像
vtkResliceCursor::SetCenter() 设置默认的切分中心点
vtkResliceCursor::SetThickMode()设置切分模式
SetThickMode(int)函数当参数为0时每次切分得到一个单层的图像切片；而当参数为1时开启厚度模式，可以通过SetThickness()来设置切片厚度，即得到的是一个多层的厚度图像。这里我们关闭厚度模式。
m_ResliceCursorRep 是一个vtkResliceCursorThickLineRepresentation对象，即在图像切分时屏幕上显示的“十”字坐标轴，利用该对象设置其关联的vtkResliceCursor对象和设置切分的方向。SetDefaultRenderer()用于设置显示切分结果所需要的vtkRenderer。我们这里设置为类成员变量m_Renderer，即每次鼠标进行切分的结果在当前窗口定义的可视化管线中进行显示。同样，在设置vtkResliceCursor对象的输入图像后，开启vtkResliceCursorWidget对象：
vtkResliceCursorWidget::SetEnabled();
m_Direction为方向标志，取值分别为0,1和2，分别代表X轴，Y轴和Z轴方向，可以通过设置不同的方向值，来实现横断面视图，矢状面视图，冠状面视图。
这样一个具有图像切分功能的控件已经完成，该控件支持用户设置切片方向和图像输入，运行时在每个视图中根据用户设置的方向显示相应的十字坐标轴，用户可以拖动该十字来进行交互。当然，我们还没有实现视图之间的同步。
#### 4．完善CvtkDialogDlg类
##### 4.1 四视图初始化
首先在CvtkDialogDlg类的初始化函数OnInitDialog函数中初始化四个视图控件类对象：
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8939115#)[copy](http://blog.csdn.net/www_doling_net/article/details/8939115#)
- BOOL CvtkDialogDlg::OnInitDialog()  
- {  
-     CDialog::OnInitDialog();  
- 
- // Add "About..." menu item to system menu.
- 
- // IDM_ABOUTBOX must be in the system command range.
-     ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);  
-     ASSERT(IDM_ABOUTBOX < 0xF000);  
- 
-     CMenu* pSysMenu = GetSystemMenu(FALSE);  
- if (pSysMenu != NULL)  
-     {  
- BOOL bNameValid;  
-         CString strAboutMenu;  
-         bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);  
-         ASSERT(bNameValid);  
- if (!strAboutMenu.IsEmpty())  
-         {  
-             pSysMenu->AppendMenu(MF_SEPARATOR);  
-             pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);  
-         }  
-     }  
- 
- // Set the icon for this dialog.  The framework does this automatically
- //  when the application's main window is not a dialog
-     SetIcon(m_hIcon, TRUE);         // Set big icon
-     SetIcon(m_hIcon, FALSE);        // Set small icon
- 
- // TODO: Add extra initialization here 
- 
-     m_AxialView.SetSliceDirection(0);  
-     m_AxialView.GetImagePlaneWidget()->SetInteractor(m_3DView.GetInteractor());  
-     m_AxialView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());  
- 
-     m_SagittalView.SetSliceDirection(1);  
-     m_SagittalView.GetImagePlaneWidget()->SetInteractor(m_3DView.GetInteractor());  
-     m_SagittalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());  
-     m_SagittalView.SetResliceCursor(m_AxialView.GetResliceCursor());  
- 
-     m_CoronalView.SetSliceDirection(2);  
-     m_CoronalView.GetImagePlaneWidget()->SetInteractor(m_3DView.GetInteractor());  
-     m_CoronalView.GetImagePlaneWidget()->SetDefaultRenderer(m_3DView.GetRenderer());  
-     m_CoronalView.SetResliceCursor(m_AxialView.GetResliceCursor());  
- 
- return TRUE;  // return TRUE  unless you set the focus to a control
- }  
 从代码可以看出，我们为每个视图指定了切片的方向，然后为每个视图的vtkImagePlaneWidget类对象指定相应的交互对象和绘制对象vtkRenderer。在前面我们也提到过，vtkImagePlaneWidget需要为其指定相应的vtkRenderer类对象，用来显示图像切片。这里vtkImagePlaneWidget的结果需要在三维视图中进行显示，因此通过SetDefaultRenderer()函数将三维视图的vtkRenderer对象设置到每个二维视图中的vtkImagePlaneWidget对象中，这样即可在三维视图中显示三个方向的切片图像。
另外需要注意的是，
m_SagittalView.SetResliceCursor(m_AxialView.GetResliceCursor());
m_CoronalView.SetResliceCursor(m_AxialView.GetResliceCursor());
这里分别将m_SagittalView和m_CoronalView类中的vtkResliceCursor对象都设置为m_AxialView的vtkResliceCursor对象，即在三个二维视图中统一使用一个vtkResliceCursor。我们知道在二维视图类中定义了vtkResliceCursorWidget对象，该对象通过用户交互，利用vtkResliceCursor来实现图像的交互式切分。但是在图像切分的同时，三个方向应该保持同步，即当一个图像切分中心发生改变时，其他两个方向的视图应该及时进行更新来保持同步。因此，这里将三个视图中统一使用一个共同的vtkResliceCursor对象，便于实现视图的同步。
##### 4.2 四视图同步
由于三个二维视图使用同一个vtkResliceCursor对象，因此当任意一个vtkResliceCursor对象的图像切分参数（即切分平面参数：中心点和法向）发生改变时，其他视图同样会发生改变。不过由于在视图内部并不能直接检测参数变化，我们需要通过VTK的Observer-Command模式来监听参数改变的消息，并进行相应的处理。切分参数的改变，是通过用户拖动或者旋转视图“十”字坐标轴来实现，此时vtkResliceCursorWidget内部会产生一个消息vtkResliceCursorWidget::ResliceAxesChangedEvent，我们只需要监听该消息即可。因此，定义一个vtkCommand类，为vtkResliceCursorWidget::ResliceAxesChangedEvent实现相应的处理操作。
此外，当用户改变图像切分的坐标轴时（旋转坐标轴或者平移坐标系），图像切分平面会产生相应的改变，如果将新的切分平面更新到二维视图的vtkImagePlaneWidget对象中，即可实现三维视图的同步更新操作。基于以上设计，实现一个vtkCommand子类，来监听vtkResliceCursorWidget::ResliceAxesChangedEvent消息，并实现相应的更新操作。
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8939115#)[copy](http://blog.csdn.net/www_doling_net/article/details/8939115#)
- class vtkResliceCursorCallback : public vtkCommand   
- {   
- public:   
- static vtkResliceCursorCallback *New()   
-     { returnnew vtkResliceCursorCallback; }   
- 
- void Execute( vtkObject *caller, unsigned long/*ev*/,   
- void *callData )   
-     {   
-         vtkResliceCursorWidget *rcw = dynamic_cast<   
-             vtkResliceCursorWidget * >(caller);   
- if (rcw)   
-         {    
- for (int i = 0; i < 3; i++)   
-             {   
-                 vtkPlaneSource *ps = static_cast< vtkPlaneSource * >(   
-                     view[i]->GetImagePlaneWidget()->GetPolyDataAlgorithm());   
- 
-                 ps->SetOrigin(view[i]->GetResliceCursorWidget()->  
-                     GetResliceCursorRepresentation()->GetPlaneSource()->GetOrigin());  
-                 ps->SetPoint1(view[i]->GetResliceCursorWidget()->  
-                     GetResliceCursorRepresentation()->GetPlaneSource()->GetPoint1());  
-                 ps->SetPoint2(view[i]->GetResliceCursorWidget()->  
-                 GetResliceCursorRepresentation()->GetPlaneSource()->GetPoint2());  
- 
-                 view[i]->GetImagePlaneWidget()->UpdatePlacement();   
-                 view[i]->Render();  
-             }   
-             view[3]->Render();  
-         }   
- 
-     }   
- 
-     vtkResliceCursorCallback() {}   
-     CvtkView *view[4];  
- };  
每当监听到vtkResliceCursorWidget::ResliceAxesChangedEvent消息后，即可执行vtkResliceCursorCallback::Execute函数来实现视图同步与刷新操作。该函数更新三个视图中vtkImagePlaneWidget对象的切分平面参数，即利用每个视图中vtkResliceCursorThickLineRepresentation::GetPlaneResource()
获取当前方向的纹理映射平面对象vtkPlaneSource，并获取平面的原点，以及定义平面内坐标轴的两个点，
vtkPlaneSource::GetOrigin(),
vtkPlaneSource::GetPoint1();
vtkPlaneSource::GetPoint2();
将三个点坐标设置到到vtkImagePlaneWidget的切分平面对象中，通过调用函数
vtkImagePlaneWidget::UpdatePlacement()
更新切分平面的空间位置和姿态，从而得到新的图像切面，最后刷新四个视图完成四视图的同步与更新。
定义完该类后，在初始化函数BOOL CvtkDialogDlg::OnInitDialog()中，定义该类对象并设置相应的监听消息实现视图同步。
**[cpp]**[view plain](http://blog.csdn.net/www_doling_net/article/details/8939115#)[copy](http://blog.csdn.net/www_doling_net/article/details/8939115#)
- vtkSmartPointer<vtkResliceCursorCallback> cbk =   
- vtkSmartPointer<vtkResliceCursorCallback>::New();   
- 
- cbk->view[0] = &m_AxialView;  
- m_AxialView.GetResliceCursorWidget()->AddObserver(   
- vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );   
- 
- cbk->view[1] = &m_SagittalView;  
- m_SagittalView.GetResliceCursorWidget()->AddObserver(   
- vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );  
- 
- cbk->view[2] = &m_CoronalView;  
- m_CoronalView.GetResliceCursorWidget()->AddObserver(   
- vtkResliceCursorWidget::ResliceAxesChangedEvent, cbk );  
- 
- cbk->view[3]= &m_3DView;  
##### 4.3 数据管理
采用树控件来管理图像文件，为其响应右键单击消息和左键单击消息。右键单击消息响应树控件的根节点的右键菜单功能，主要定义了图像读取函数，图像保存，图像清空等功能。当图像读入后，即将其设置到三个视图中，并调用相应的CvtkView::Render()函数来更新视图（CvtkView定义的一个视图刷新函数）。树节点的左键按下消息，实现图像的切换；每次点击一个图像节点，即将当前的图像更新至四视图中。
至此，一个完整的四视图图像显示Demo已经完成。该Demo中能够实现横断面，矢状面和冠状面三个方向的二维切片显示，以及三个切片的三维显示；实现了四视图的同步显示，即一个图像中的切分平面发生变化时，其他视图会同步进行更新。在实现过程中，基于两个VTK Widget类进行实现，分别是vtkResliceCursorWidget和vtkImagePlaneWidget。vtkResliceCursorWidget集成了一个vtkResliceCursorThickLineRepresentation对象用户交互调整切分平面，一个vtkResliceCursor对象根据用户设置的切分平面用于图像切分；而vtkImagePlaneWidget同样根据vtkResliceCursor对象根据用户设置的切分平面利用内部定义的vtkImageReslice来计算切分图像并进行显示。本实例仅仅是实现了一个四视图的Demo程序，还存在许多需要完善的地方，各位读者有兴趣的话，可以在此基础上进行修改和完善。
![](https://img-blog.csdn.net/20130517133618981)
**==========欢迎转载，转载时请保留该声明信息==========**
**版权归[@东灵工作室](http://weibo.com/dolingstudio)所有，更多信息请访问[东灵工作室](http://www.doling.net/)**
**教程系列导航：**[**http://blog.csdn.net/www_doling_net/article/details/8763686**](http://blog.csdn.net/www_doling_net/article/details/8763686)
