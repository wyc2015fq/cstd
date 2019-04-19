# WINCE6.0下用SilverLight技术实现的图片浏览器(支持手势操作，在模拟器下可以运行) - xqhrs232的专栏 - CSDN博客
2009年12月30日 12:10:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：3094标签：[silverlight																[wince																[浏览器																[scroll																[image																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=image&t=blog)](https://so.csdn.net/so/search/s.do?q=scroll&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)](https://so.csdn.net/so/search/s.do?q=silverlight&t=blog)
个人分类：[WINCE](https://blog.csdn.net/xqhrs232/article/category/906924)
支持滚动的左右手势进行图片切换，支持滚动的上下手势进行图片的放大与缩小。
(在模拟器里面上与下正好反了，左与右正好反了，动画的效果还有点问题估计在实际的硬件环境下不会有问题)
//###############################################################################################################
//Main.cpp
//Designed by xqh
//Date:2009--12--29
//#include <pwinuser.h> 
#include <XamlRuntime.h>
#include <XRDelegate.h>
#include <XRPtr.h>
#include <Ehm.h>
//=================================================================================================================
IXRApplication* pApplication = NULL;
IXRVisualHost* pVisualHost = NULL;
IXRImagePtr  CenterImage;
IXRImagePtr  LeftImage;
IXRImagePtr  RightImage;
LPCTSTR   strFilePath[8];
class BtnEventHandler //单独响应按钮操作
{
protected:
 IXRStoryboardPtr     sboard;
public:
   BtnEventHandler(IXRStoryboardPtr& storyboard)
 {       
      sboard=storyboard;
 }
    HRESULT Btn_OnClick(IXRDependencyObject* source,XRMouseButtonEventArgs* args)   
 {      
#if  1
  //MessageBox(NULL,TEXT("Btn_OnClick!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
        RETAILMSG(1,(TEXT("++++BtnEventHandler::Btn_OnClick( )----/r/n")));
#else
      UINT  exitCode=0;
     // pApplication->StopProcessing(exitCode);
      pVisualHost->EndDialog(0);
#endif
     return S_OK;    
 }
//====================================================================================================
    HRESULT Image_MouseLeftButtonDown(IXRDependencyObject* source,XRMouseButtonEventArgs* args)   
 {      
     static  int  ImageIndex=0;
//先弄一个动画效果
       HRESULT        hr;        
    XRClockState ckstate;
//MessageBox(NULL,TEXT("Image_MouseLeftButtonDown!"),TEXT("Silverlight for Windows Embedded test"),MB_OK);  
 RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_MouseLeftButtonDown( )----/r/n")));
        if (FAILED(hr=sboard->GetCurrentState(&ckstate)))            
   return hr;
        if (ckstate==XRClockState_Stopped)        
  {     
      if (FAILED(hr=sboard->Begin()))                
       return hr;        
  }        
  else        
  {            
      if (FAILED(hr=sboard->Stop()))                
       return hr;        
  }
      if(ImageIndex>6)
    ImageIndex=0;
   else
    ImageIndex++;
       IXRBitmapImagePtr    BitmapImage;
//------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
BitmapImage->SetUriSource(strFilePath[ImageIndex]);
if(LeftImage==NULL)
RETAILMSG(1,(TEXT("++++the LeftImage is NULL!/r/n")));
LeftImage->SetSource(BitmapImage);  //这个地方设置不成功！！！
//--------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
if(ImageIndex>6)
BitmapImage->SetUriSource(strFilePath[7-ImageIndex]);
else
BitmapImage->SetUriSource(strFilePath[ImageIndex+1]);
CenterImage->SetSource(BitmapImage);
//----------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
if(ImageIndex>5)
BitmapImage->SetUriSource(strFilePath[ImageIndex-6]);
else
BitmapImage->SetUriSource(strFilePath[ImageIndex+2]);
RightImage->SetSource(BitmapImage);
     return S_OK;    
 }
//====================================================================================================
 HRESULT Image_Gesture(IXRDependencyObject* source,XRGestureEventArgs* args)   
 {  
   DWORD  dwID;
   GESTUREINFO   GestureInfo;
   static   int  iCurImage=0;
   static   float  fScaleX=1.0,fScaleY=1.0; //设置放大/缩小的倍数
   float   fActualX,fActualY;
   float   fWidth,fHeight;
   IXRBitmapImagePtr    BitmapImage;
   HRESULT        hr;        
   XRClockState ckstate;
   IXRScaleTransform*  pRenderTransform;
   GestureInfo=args->GestureInfo;
   dwID=GestureInfo.dwID;
   RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----/r/n")));
   switch(dwID)
   {
   case  GID_BEGIN:  //手势开始
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_BEGIN!/r/n")));
    break;
   case  GID_END:  //手势结束
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_END!/r/n")));
    break;
   case  GID_PAN:  //滑动----速度慢一点
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_PAN!/r/n")));
    break;
   case  GID_ROTATE:  //旋转----保留没有使用
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_ROTATE!/r/n")));
    break;
   case  GID_SCROLL://滚动---速度要求比PAN快
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_SCROLL!/r/n")));
            switch( GID_SCROLL_DIRECTION(GestureInfo.ullArguments) ) //判断滚动的方向
   {
       case  ARG_SCROLL_NONE:
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_SCROLL--case  ARG_SCROLL_NONE!/r/n")));
    break;
               case  ARG_SCROLL_UP:
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_SCROLL--case  ARG_SCROLL_UP!/r/n")));
//进行图片缩小的处理
if(fScaleX>0.5)
fScaleX-=0.1;
if(fScaleY>0.5)
fScaleY-=0.1;
pApplication->CreateObject(IID_IXRScaleTransform,&pRenderTransform);
CenterImage->GetActualX(&fActualX);  //获取对应的坐标值
CenterImage->GetActualY(&fActualY);
CenterImage->GetWidth(&fWidth); //获取控件的大小
CenterImage->GetHeight(&fHeight);  
pRenderTransform->SetCenterX(fActualX+fWidth/2);
pRenderTransform->SetCenterY(fActualY+fHeight/2);
pRenderTransform->SetScaleX(fScaleX);
pRenderTransform->SetScaleY(fScaleY);
CenterImage->SetRenderTransform(pRenderTransform );
    break;
                case  ARG_SCROLL_DOWN:
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_SCROLL--case  ARG_SCROLL_DOWN!/r/n")));
//进行图片放大的处理
if(fScaleX<2.0)
fScaleX+=0.1;
if(fScaleY<2.0)
fScaleY+=0.1;
pApplication->CreateObject(IID_IXRScaleTransform,&pRenderTransform);
CenterImage->GetActualX(&fActualX);  //获取对应的坐标值
CenterImage->GetActualY(&fActualY);
CenterImage->GetWidth(&fWidth); //获取控件的大小
CenterImage->GetHeight(&fHeight);  
pRenderTransform->SetCenterX(fActualX+fWidth/2);
pRenderTransform->SetCenterY(fActualY+fHeight/2);
pRenderTransform->SetScaleX(fScaleX);
pRenderTransform->SetScaleY(fScaleY);
CenterImage->SetRenderTransform(pRenderTransform );
    break;
                 case  ARG_SCROLL_LEFT:
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_SCROLL--case  ARG_SCROLL_LEFT!/r/n")));
//先弄一个动画效果
/*
        if (FAILED(hr=sboard->GetCurrentState(&ckstate)))  //为什么会是XRClockState_Filling状态？         
   return hr;
        if (ckstate==XRClockState_Stopped)        
  {     
      if (FAILED(hr=sboard->Begin()))  //1S钟就结束              
       return hr;        
  }        
  else        
  {            
      if (FAILED(hr=sboard->Stop()))                
       return hr;        
  }
*/
if(iCurImage>6)
    iCurImage=0;
else
    iCurImage++;
//------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
BitmapImage->SetUriSource(strFilePath[iCurImage]);
LeftImage->SetSource(BitmapImage);  
//--------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
if(iCurImage>6)
   BitmapImage->SetUriSource(strFilePath[7-iCurImage]);
else
   BitmapImage->SetUriSource(strFilePath[iCurImage+1]);
CenterImage->SetSource(BitmapImage);
//----------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
if(iCurImage>5)
   BitmapImage->SetUriSource(strFilePath[iCurImage-6]);
else
  BitmapImage->SetUriSource(strFilePath[iCurImage+2]);
RightImage->SetSource(BitmapImage);
    break;
               case  ARG_SCROLL_RIGHT:
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_SCROLL--case  ARG_SCROLL_RIGHT!/r/n")));
//先弄一个动画效果
/*
        if (FAILED(hr=sboard->GetCurrentState(&ckstate)))            
   return hr;
        if (ckstate==XRClockState_Stopped)        
  {     
      if (FAILED(hr=sboard->Begin()))                
       return hr;        
  }        
  else        
  {            
      if (FAILED(hr=sboard->Stop()))                
       return hr;        
  }
*/
if(iCurImage==0)
    iCurImage=7;
else
    iCurImage--;
 IXRBitmapImagePtr    BitmapImage;
//------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
BitmapImage->SetUriSource(strFilePath[iCurImage]);
LeftImage->SetSource(BitmapImage);  
//--------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
if(iCurImage>6)
   BitmapImage->SetUriSource(strFilePath[7-iCurImage]);
else
   BitmapImage->SetUriSource(strFilePath[iCurImage+1]);
CenterImage->SetSource(BitmapImage);
//----------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
if(iCurImage>5)
   BitmapImage->SetUriSource(strFilePath[iCurImage-6]);
else
  BitmapImage->SetUriSource(strFilePath[iCurImage+2]);
RightImage->SetSource(BitmapImage);
    break;
   }
//---------------------------------------
/*
            switch( GID_SCROLL_VELOCITY(GestureInfo.ullArguments) ) //判断滚动的速度
   {
      default:
    break;
             }
//---------------------------------------
            switch( GID_SCROLL_ANGLE(GestureInfo.ullArguments) ) //判断滚动的角度
   {
                default:
    break;
             }
*/
    break;
   case  GID_HOLD://保持
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_HOLD!/r/n")));
    break;
   case  GID_SELECT://单击选择
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_SELECT!/r/n")));
    break;
   case  GID_DOUBLESELECT://双击选择----要求两次单击的时间间隔不要太长
RETAILMSG(1,(TEXT("++++BtnEventHandler::Image_Gesture( )----case  GID_DOUBLESELECT!/r/n")));
    break;
   }
      return S_OK;    
    }
};
void InitPicturePathStr(void)
{
strFilePath[0]=L"//Storage Card//XAMLPERF//Bmps//1.png";
strFilePath[1]=L"//Storage Card//XAMLPERF//Bmps//2.png";
strFilePath[2]=L"//Storage Card//XAMLPERF//Bmps//3.png";
strFilePath[3]=L"//Storage Card//XAMLPERF//Bmps//4.png";
strFilePath[4]=L"//Storage Card//XAMLPERF//Bmps//5.png";
strFilePath[5]=L"//Storage Card//XAMLPERF//Bmps//6.png";
strFilePath[6]=L"//Storage Card//XAMLPERF//Bmps//7.png";
strFilePath[7]=L"//Storage Card//XAMLPERF//Bmps//8.png";
}
//==========================================================================================================
INT WINAPI WinMain(
    HINSTANCE hInstance, 
    HINSTANCE hPrevInstance, 
    LPWSTR lpCmdLine, 
    int nCmdShow
    )
{
   int    exitCode = -1;
   BOOL   bRet;
   HRESULT hr;
   bRet= XamlRuntimeInitialize();
   GetXRApplicationInstance(&pApplication);
   XRXamlSource SourceXaml;
   SourceXaml.SetFile(L"//Storage Card//XAMLPERF//Page_PictureBrowser.xaml");
//###############################################################
XRWindowCreateParams WindowParameters;
ZeroMemory(&WindowParameters, sizeof(WindowParameters));
WindowParameters.Style       = WS_POPUP;
WindowParameters.pTitle      = L"Title Name";
WindowParameters.Left        = 0;
WindowParameters.Top         = 0;
//################################################################
//IXRVisualHost* pVisualHost = NULL;
pApplication->CreateHostFromXaml(&SourceXaml, &WindowParameters, &pVisualHost);
if(!pVisualHost)
 RETAILMSG(1,(TEXT("++++the pVisualHost is NULL!/r/n")));
//################################################################
//找到根目录
IXRFrameworkElementPtr root;
if (FAILED(hr=pVisualHost->GetRootElement(&root)))       
return -1;
//################################################################
InitPicturePathStr( );
if (FAILED(hr=root->FindName(TEXT("LeftImage"), &LeftImage)))        
return -1;
if (FAILED(hr=root->FindName(TEXT("CenterImage"), &CenterImage)))        
return -1;
if (FAILED(hr=root->FindName(TEXT("RightImage"), &RightImage)))        
return -1;
//=================================================================
IXRStoryboardPtr sboard;
    if (FAILED(hr=root->FindName(TEXT("CenterImageStoryboard"), &sboard)))       
  return -1;
//BtnEventHandler handlerImage;
BtnEventHandler handlerImage(sboard);
/*
//注册鼠标左键单击事件
IXRDelegate<XRMouseButtonEventArgs>* clickdelegateImage;
if (FAILED(hr=CreateDelegate(&handlerImage,&BtnEventHandler::Image_MouseLeftButtonDown,&clickdelegateImage)))       
  return -1;
if (FAILED(hr=CenterImage->AddMouseLeftButtonDownEventHandler(clickdelegateImage)))  //注册交互事件     
  return -1;
*/
//注册触摸屏手势事件
IXRDelegate<XRGestureEventArgs>* gesturedelegateImage;
if (FAILED(hr=CreateDelegate(&handlerImage,&BtnEventHandler::Image_Gesture,&gesturedelegateImage)))       
  return -1;
if (FAILED(hr=CenterImage->AddGestureEventHandler(gesturedelegateImage)))  //注册交互事件     
  return -1;
//=================================================================================
IXRBitmapImagePtr    BitmapImage;
//-------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
BitmapImage->SetUriSource(strFilePath[0]);
LeftImage->SetSource(BitmapImage);
//--------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
BitmapImage->SetUriSource(strFilePath[1]);
CenterImage->SetSource(BitmapImage);
//----------------------------------------------------------------------
pApplication->CreateObject(IID_IXRBitmapImage,&BitmapImage);
BitmapImage->SetUriSource(strFilePath[2]);
RightImage->SetSource(BitmapImage);
//###############################################################
pVisualHost->StartDialog(NULL);//显示对话框
UINT StartProcessCode = 0;
pApplication->StartProcessing(&StartProcessCode);
//##################################################################
//clickdelegateImage->Release();
gesturedelegateImage->Release();
   return exitCode;
}
//======================================================================================
//相关Page_PictureBrowser.xaml的内容
<UserControl
 xmlns="[http://schemas.microsoft.com/winfx/2006/xaml/presentation](http://schemas.microsoft.com/winfx/2006/xaml/presentation)"
 xmlns:x="[http://schemas.microsoft.com/winfx/2006/xaml](http://schemas.microsoft.com/winfx/2006/xaml)"
 x:Class="SilverlightApplication_PictureBrowser.Page"
 Width="640" Height="480" xmlns:d="[http://schemas.microsoft.com/expression/blend/2008](http://schemas.microsoft.com/expression/blend/2008)" xmlns:mc="[http://schemas.openxmlformats.org/markup-compatibility/2006](http://schemas.openxmlformats.org/markup-compatibility/2006)"
 mc:Ignorable="d">
 <UserControl.Resources>
  <Storyboard x:Name="CenterImageStoryboard">
   <DoubleAnimationUsingKeyFrames BeginTime="00:00:00" Storyboard.TargetName="CenterImage" Storyboard.TargetProperty="(UIElement.RenderTransform).(TransformGroup.Children)[2].(RotateTransform.Angle)">
    <SplineDoubleKeyFrame KeyTime="00:00:01" Value="360"/>
   </DoubleAnimationUsingKeyFrames>
  </Storyboard>
 </UserControl.Resources>
 <Grid x:Name="LayoutRoot" Background="White">
  <Image HorizontalAlignment="Right" Margin="0,96,215.717,232" Width="240.33" Source="border.png" Stretch="Fill" d:LayoutOverrides="HorizontalAlignment, Width"/>
  <Image HorizontalAlignment="Left" Margin="3.135,142.319,0,238.318" Width="156.865" Source="border.png" Stretch="Fill" d:LayoutOverrides="Width"/>
  <Image Height="96" HorizontalAlignment="Right" Margin="0,143.871,15.777,0" VerticalAlignment="Top" Width="152" Source="border.png" Stretch="Fill"/>
  <Image Height="65.799" HorizontalAlignment="Right" Margin="0,151.099,24,0" VerticalAlignment="Top" Width="136" Source="3.png" Stretch="Fill" d:LayoutOverrides="Height" x:Name="RightImage"/>
  <Image Height="103.556" Margin="202.415,104,224,0" VerticalAlignment="Top" Source="1.png" Stretch="Fill" d:LayoutOverrides="Height" x:Name="CenterImage" RenderTransformOrigin="0.5,0.5">
   <Image.RenderTransform>
    <TransformGroup>
     <ScaleTransform/>
     <SkewTransform/>
     <RotateTransform/>
     <TranslateTransform/>
    </TransformGroup>
   </Image.RenderTransform>
  </Image>
  <Image Height="67.307" HorizontalAlignment="Left" Margin="14,148.736,0,0" VerticalAlignment="Top" Width="138.354" Source="2.png" Stretch="Fill" d:LayoutOverrides="Width, Height" x:Name="LeftImage"/>
 </Grid>
</UserControl>
//==================================================================================
//相关参考文章
[http://www.cnblogs.com/Terrylee/archive/2008/03/21/Silverlight2-step-by-step-part32.html](http://www.cnblogs.com/Terrylee/archive/2008/03/21/Silverlight2-step-by-step-part32.html)
