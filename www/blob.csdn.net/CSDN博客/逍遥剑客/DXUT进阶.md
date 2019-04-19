# DXUT进阶 - 逍遥剑客 - CSDN博客
2007年05月06日 03:13:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2386

**概要**
这个指南涵盖了更多DXUT的高级应用. 这个指南里的大部分功能是可选的, 为了以最小的代价来增强你的应用程序. DXUT提供了一个简单的基于GUI系统的精灵和一个设备设置对话框. 另外, 它还提供了一些摄像机类.
这个指南的结果是一个具有完善功能的GUI 用于更改设备和场景的设置. 它将有按钮, 滑块, 和文本来示范这些功能.
**导航**
- DXUT摄像机
- DXUT对话框
- 资源管理程序初始化
- 3D设置对话框
- 文本渲染
**DXUT ****摄像机**
DXUT中的CModelViewerCamera类可以简单的的管理视图变换和透视变换, 就像GUI 的功能一样.
CModelViewerCamera g_Camera;// A model viewing camera
摄像机类提供的第一个功能是创建视图和透视矩阵. 有了这个摄像机,没有必要担心这些矩阵. 反而, 你可以指定你在哪, 你看着什么,还有窗口的大小. 然后, 把这些参数传递给摄像机对象, 它会在后台创建这些矩阵.
这里我们设置摄像机的一部分视图参数. 我们指定我们在哪,和我们看着什么.
// Initialize the camera
D3DXVECTOR3 Eye( 0.0f, 0.0f, -800.0f );
D3DXVECTOR3 At( 0.0f, 0.0f, 0.0f );
g_Camera.SetViewParams( &Eye, &At );
接下来,我们指定摄像机的投影参数. 也就是说, 我们需要提供观察角度,纵横比, 和视图截锥的近、远裁剪面. 这些信息跟之前的指南提供的一样, 不同的是, 你不必担心怎么去创建矩阵.
// Setup the camera's projection parameters
float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
g_Camera.SetProjParams( D3DX_PI/4, fAspectRatio, 0.1f, 5000.)0f );
g_Camera.SetWindow( pBackBufferSurfaceDesc->Width, p00BackBufferSurfaceDesc->Height );
另外, 这个摄像机还封装了简单的鼠标反馈. 在这里, 我们指定三个鼠标按键给提供的鼠标操作: 模型旋转, 放缩, 还有摄像机旋转.试着编译工程并用每个按键体会一下各个操作.
g_Camera.SetButtonMasks( MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON );
在这个按钮组的基础上, 摄像机会监听这些输入并产生相应效果. 为了响应用户输入, 需要加入一个监听到MsgProc回调函数（DXUT消息处理函数）.
// Pass all remaining windows messages to camera so it can respondto user input
g_Camera.HandleMessages( hWnd, uMsg, wParam, lParam );
最后,在把所有数据都输入摄像机后, 就是取出真正的变换矩阵的时候了. 这里我们用相关函数获取投影矩阵和视图矩阵. 摄像机对象会自己计算这些矩阵.
g_pProjectionVariable->SetMatrix( (float*)g_Camera.GetProjMatrix() );
g_pViewVariable->SetMatrix( (float*)g_Camera.GetViewMatrix() );
**DXUT ****对话框**
用户交互可以用CDXUTDialog来实现, 它在一个包含控件的对话框里接受用户输入,并且通过程序句柄传递它们. 首先, 对话框类要实例化, 然后每个单独的控件才能加入.
**声明**
在这个指南里, 有两个对话框会被加入, 一个叫 g_HUD跟D3D10示例共享相同的代码, 另一个叫 g_SampleUI 用于显示函数细节给这个指南. 第二个对话框用来控制模型的"胖瘦",就是设置一个变量传递给shaders.
CDXUTDialog g_HUD;// manages the 3D UI
CDXUTDialog g_SampleUI;// dialog for sample specific controls
这些对话框被CDXUTDialogResourceManager控制着.这个管理程序会在对话框之间传递消息并处理共享资源.
CDXUTDialogResourceManager g_DialogResourceManager;// manager for shared resources of dialogs
最后, 一些回调函数与GUI处理的实际消息相关联. 这个函数用于处理控件间的交互.
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext );
**对话框初始化**
既然已经介绍了许多有用的东西, 那么需要被初始化, 这个指南将这些模块的真正初始化转移到一个单独的函数, 叫 InitApp().
每个对话框的控件被初始化就是在这个函数里. 每个对话框都需要调用它的初始化函数, 并传递给资源管理者来指定把控件供给谁. 同样, 它们设置回调函数来处理GUI 响应. 既然这样, 这个相关的回调函数是 OnGUIEvent.
g_HUD.Init( &g_DialogResourceManager );
g_SampleUI.Init( &g_DialogResourceManager );
g_HUD.SetCallback( OnGUIEvent );
g_SampleUI.SetCallback( OnGUIEvent );
在它们被初始化后, 每个都对话框可以加入它们想要使用的控件. 对话框 HUD加入了3个按钮用于基本功能：切换全屏, 切换引用 (软件) 渲染器, 和改变设备.
要加入一个按钮, 你要指定你想要使用的IDC标识符：一个用于显示的字符串, 坐标, 宽和高, 还有可选的与它关联的热键. 热键可以用键盘快速地开关这些按钮.
注意指定的坐标与对话框的定位相关.
int iY = 10; 
g_HUD.AddButton( IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 35, iY, 125, 22 );
g_HUD.AddButton( IDC_TOGGLEREF, L"Toggle REF (F3)", 35, iY += 24, 125, 22 );
g_HUD.AddButton( IDC_CHANGEDEVICE, L"Change device (F2)", 35, iY += 24, 125, 22, VK_F2 );
同样对于sample UI, 加入了这些控件：一个静态文本,一个滑块, 还有一个复选按钮.
传递给静态文本的参数有IDC 标识符,字符串,坐标, 和宽高.
滑块的参数有IDC 标识符,字符串,坐标, 和宽高, 还有滑块的最大值和最小值, 最后是存储结果的变量.
复选按钮包括IDC标识符,一个字符串标签,坐标, 宽高,还有存储结果的布尔值.
iY = 10;
WCHAR sz[100];
iY += 24;
StringCchPrintf( sz, 100, L"Puffiness: %0.2f", g_fModelPuffiness ); 
g_SampleUI.AddStatic( IDC_PUFF_STATIC, sz, 35, iY += 24, 125, 22 );
g_SampleUI.AddSlider( IDC_PUFF_SCALE, 50, iY += 24, 100, 22, 0, 2000, (int)(g_fModelPuffiness*100.0f) );
iY += 24;
g_SampleUI.AddCheckBox( IDC_TOGGLESPIN, L"Toggle Spinning", 35, iY += 24, 125, 22, g_bSpinning );
初始化完成后, 这个对话框要显示在屏幕上. 这个由 OnD3D10ResizedSwapChain调用来完成。因为这个每次交换链重建屏幕坐标会发生变化(可能由于窗口大小的变化).
g_HUD.SetLocation( pBackBufferSurfaceDesc->Width-170, 0 );
g_HUD.SetSize( 170, 170 );
g_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width-170, pBackBufferSurfaceDesc->Height-300 );
g_SampleUI.SetSize( 170, 300 );
对话框需要鉴别它们自己在哪，这是最后一步： OnD3D10FrameRender函数. 一个对话框如果你不绘制它而且用户看不到不太好吧?
//
// Render the UI
//
g_HUD.OnRender( fElapsedTime ); 
g_SampleUI.OnRender( fElapsedTime );
**资源管理程序初始化**
资源管理程序在每次回调相关的初始化和销毁时都需要被初始化. 这是因为在每次设备被重建的时候 GUI需要被重建, 或者在交换链被重建的时候. CDXUTDialogResourceManager类包含符合每个这样的回调的函数, 具有相同的名字. 因此, 它仅仅是在合适的地方插入代码来调用它们.
V_RETURN( g_DialogResourceManager.OnD3D10CreateDevice( pd3dDevice ) );
V_RETURN( g_DialogResourceManager.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
g_DialogResourceManager.OnD3D10ReleasingSwapChain();
g_DialogResourceManager.OnD3D10DestroyDevice();
**响应GUI事件**
当全部初始化完成后, 我们最后可以开始写代码来处理GUI交互了. 在我们初始化对话框的时候,我们设置这些对话框的回调函数做为OnGUIEvent. 现在我们将要创建 OnGUIEvent 函数, 它会监听并处理GUI 相关事件(被框架调用的).
这是一个简单的对每个IDC标识符都包含一个case代码块的函数，它在对话框创建时被监听.每个case块中是处理代码,假设用户以某种方式控制的话.这里的处理控制的代码跟 Win32 代码很像.
跟HUD相关的控制实际上调用DXUT内置的函数. 有一个DXUT 函数关系到切换全屏, 绑定引用软件渲染器, 并且更改设备设置(它会调用下面提到的3D设置对话框).
SampleUI对话框包含自定义的代码来操作跟滑块相关联的变量. 它会收集数值, 更新与它相关的文本, 并把数值传递给滑块.
void CALLBACK OnGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
switch( nControlID )
{
case IDC_TOGGLEFULLSCREEN: DXUTToggleFullScreen(); break;
case IDC_TOGGLEREF:DXUTToggleREF(); break;
case IDC_CHANGEDEVICE:g_D3DSettingsDlg.SetActive( !g_D3DSettingsDlg.IsActive() ); break;
case IDC_TOGGLESPIN:
{
g_bSpinning = g_SampleUI.GetCheckBox( IDC_TOGGLESPIN )->GetChecked();
break;
}
case IDC_PUFF_SCALE: 
{
g_fModelPuffiness = (float) (g_SampleUI.GetSlider( IDC_PUFF_SCALE )->GetValue() * 0.01f);
WCHAR sz[100];
StringCchPrintf( sz, 100, L"Puffiness: %0.2f", g_fModelPuffiness ); 
g_SampleUI.GetStatic( IDC_PUFF_STATIC )->SetText( sz );
g_pPuffiness->SetFloat( g_fModelPuffiness );
break;
}
}
}
**更新消息处理**
既然现在我们有对话框消息和用户交互, 那就会有传递给应用程序的消息需要被解析, 如果适用的话.那些代码会在DXUT提供的 MsgProc回调函数中被处理. 在之前的指南里, 这段是空白因为没有消息需要被处理. 但是现在,我们要确保发送给资源管理程序和对话框的消息被适当地发送.
不需要专门的消息处理代码; 我们只需要对每个对话框调用MsgProcs来确保消息被处理了.这通过调用每个单独的类的相应的 MsgProc函数来完成. 应该注意到这个函数提供了一个标记通报给框架，不需要更多的处理, 因此,可以退出.
LRESULT CALLBACK MsgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext )
{
// Always allow dialog resource manager calls to handle globalmessages
// so GUI state is updated correctly
*pbNoFurtherProcessing = g_DialogResourceManager.MsgProc( hWnd, uMsg, wParam, lParam );
if( *pbNoFurtherProcessing )
return 0;
if( g_D3DSettingsDlg.IsActive() )
{
g_D3DSettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
return 0;
}
// Give the dialogs a chance to handle the message first
*pbNoFurtherProcessing = g_HUD.MsgProc( hWnd, uMsg, wParam, lParam );
if( *pbNoFurtherProcessing )
return 0;
*pbNoFurtherProcessing = g_SampleUI.MsgProc( hWnd, uMsg, wParam, lParam );
if( *pbNoFurtherProcessing )
return 0;
if( uMsg == WM_CHAR && wParam == '1' )
DXUTToggleFullScreen();
return 0;
}
**3D****设置对话框**
有一个专门的内置对话框用来控制D3DDevice的设置. DXUT提供的这个对话框就是CD3DSettingsDlg. 它的功能像一个自定义对话框,但是它会提供给用户所有的他们需要修改的设置选项.
CD3DSettingsDlgg_D3DSettingsDlg; // Device settings dialog
初始化就像其它对话框一样, 你可以调用初始化函数. 然而, 每次Direct3D 改变它的交换链或者设备,这个对话框必须被更新.因此, 它必须包含一个调用在OnD3D10CreateDevice和OnD3D10ResizedSwapChain里, 相对应地命名, 来反映这些改变. 同样地,销毁对象的改变必须被通报, 因而, 需要在 OnD3D10DestroyDevice里调用.
g_D3DSettingsDlg.Init( &g_DialogResourceManager );
V_RETURN( g_D3DSettingsDlg.OnD3D10CreateDevice( pd3dDevice ) );
V_RETURN( g_D3DSettingsDlg.OnD3D10ResizedSwapChain( pd3dDevice, pBackBufferSurfaceDesc ) );
g_D3DSettingsDlg.OnD3D10DestroyDevice();
在渲染的这一边, 因为对话框的出现可以被改变,它通过一个叫 IsActive()的标记来转换. 如果这个标记被设成false, 那么这个面板将不会被渲染.这个面板的改变是受上面说过的HUD对话框处理的. 标记The IDC_CHANGEDEVICE与HUD 控件相关, 就像上面提到的那样.
if( g_D3DSettingsDlg.IsActive() )
{
g_D3DSettingsDlg.MsgProc( hWnd, uMsg, wParam, lParam );
return 0;
}
一旦这些初始步骤都完成了, 你就可以在你的程序中加入这个对话框. 试着编译这个指南并与更改设置的面板进行交互来观察它的效果. 真正的D3DDevice或交换链的重构在DXUT内部完成.
**文本渲染**
一个程序不会有趣如果用户不知道干什么. 所以就DXUT包含了一个工具类用于在屏幕上绘制2D 文本, 用来给用户反馈. 这个类, CDXUTD3D10TextHelper, 允许你在屏幕的任意位置绘制一行行的文本, 并有简单的字符串输入. 开始之前, 我们要实例化这个类. 既然文本渲染与大多数初始化过程独立, 在这个指南里我们让大部分代码在RenderText10里.
CDXUTD3D10TextHelper txtHelper( g_pFont, g_pSprite, 15 );
**初始化**
我们传入的第一个参数是我们要绘制的字体. 字体的类型是D3DX提供的ID3DXFont. 要初始化这个字体, 你调用D3DX10CreateFont, 并且你要传入设备, 高度, 宽度, 重量, mip层次 (一般取 1), 斜体, 字符集, 精度, 质量, 倾斜度和家族, 字体名称, 还有你的对象的指针. 虽然它看起来像很多字符, 只有开始的4个和最后的2个真正有意义.
V_RETURN( D3DX10CreateFont( pd3dDevice, 15, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, 
OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
L"Arial", &g_pFont ) );
第2个参数需要我们初始化一个ID3DXSprite类. 要做这个的话, 我们可以调用 D3DX10CreateSprite (notice the trend here?). 这个函数只需要设备和对象的指针做为参数.
// Initialize the sprite
V_RETURN( D3DX10CreateSprite( pd3dDevice, &g_pSprite ) );
当然, 像其它对象一样, 字体和精灵在我们完成时必须被销毁. 这可以用常见的 SAFE_RELEASE 宏完成.
SAFE_RELEASE( g_pFont );
SAFE_RELEASE( g_pSprite );
**渲染**
在这个示例里包含的文本里有渲染的统计信息. 另外, 还有一个区域显示怎样用鼠标操作模型.
渲染必须在OnD3D10FrameRender里调用, 而且在这个指南里, 它是在帧渲染调用里调用RenderText10来完成的.
第一个区域一直在绘制, 因此, 它最先完成. 第一个文本渲染调用是Begin(). 它通知引擎我要开始输出文本到屏幕. 在那之后, 我们设置好“指针”的位置, 文本的颜色, 我们就准备绘制了.
输出一个字符串文本要调用DrawTextLine来完成. 你传入字符串, 它就会在当前位置输出. 它在文本书写的同时增加指针, 因此, 如果q包含了一个/n, 它会自动移动指针到下一行.
// Output statistics
txtHelper.Begin();
txtHelper.SetInsertionPos( 2, 0 );
txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
txtHelper.DrawTextLine( DXUTGetFrameStats() );
txtHelper.DrawTextLine( DXUTGetDeviceStats() );
还有另一个方法来输出文本, 就好似通常使用的printf. 你用特殊的字符来格式化字符串, 然后插入变量到字符串里. 这通过DrawFormattedTextLine来实现.
txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ) );
txtHelper.DrawFormattedTextLine( L"fTime: %0.1fsin(fTime): %0.4f", fTime, sin(fTime) );
既然帮助绘制用一种相似的方式, 所以不需要涵盖那部分代码了. 一定要注意你能在任何时候通过调用SetInsertionPos来重新设置指针的位置.
最后, 当你对文本输出满意后, 调用 End() 来通知引擎你完成了.
[Feedback? Please provide us with your comments on this topic.](mailto:dxsdkdoc@microsoft.com?body=Topic%20ID:Tutorial%2010:%20Advanced%20DXUT%0D%0A%0D%0AThe%20DirectX%20documentation%20team%20appreciates%20your%20feedback!%20You%20can%20have%20a%20great%20impact%20on%20DirectX%20customers%20and%20end%20users%20worldwide!%20Please%20provide%20any%20suggestions%20you%20may%20have%20for%20the%20documentation,%20such%20as%20corrections%20or%20helpful%20information%20you%20think%20ought%20to%20be%20included.%20If%20you%20have%20more%20general%20questions%20not%20directly%20related%20to%20the%20documentation,%20please%20visit%20the%20MSDN%20DirectX%20Developer%20site%20instead%20at%20http://msdn.microsoft.com/directx%20%09%09%0D%0A%0D%0AYour%20comments%20on%20this%20topic:&subject=DirectX%20Docs%20Feedback:Tutorial%2010:%20Advanced%20DXUT)
[For more help, visit the DirectX Developer Center](http://msdn.microsoft.com/directx/)
© 2005 Microsoft Corporation. All rights reserved.
