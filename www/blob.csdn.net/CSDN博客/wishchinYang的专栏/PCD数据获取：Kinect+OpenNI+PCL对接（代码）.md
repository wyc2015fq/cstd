# **PCD数据获取：Kinect+OpenNI+PCL对接（代码） - wishchinYang的专栏 - CSDN博客
2013年08月20日 08:47:12[wishchin](https://me.csdn.net/wishchin)阅读数：4200
个人分类：[场景处理/RgbD累积																[三维重建/SLAM](https://blog.csdn.net/wishchin/article/category/5723249)](https://blog.csdn.net/wishchin/article/category/1524721)
**前言**：
           PCL使用点云作为数据格式，Kinect可以直接作为三维图像的数据源产生三维数据，其中的桥梁是OpenNI和PrimeSense。为了方便地使用Kinect的数据，还是把OpenNI获取的基础数据格式转换为点云格式。并且PCD格式的数据操作起来更为直观。
            切记：理论自己慢慢看，代码是最重要的，且测试可行且正确![害羞](http://static.blog.csdn.net/xheditor/xheditor_emot/default/shy.gif)
### (1)：Microsoft Kinect SDK 和 PrimeSense OpenNI 的区别
        原文链接：[http://blog.csdn.net/wdxzkp/article/details/6608817](http://blog.csdn.net/wdxzkp/article/details/6608817)
注解：作者写了一系列博文，可以借鉴一下：
      After playing with both the Microsoft Kinect SDK and the PrimeSense OpenNI SDK here are some of my thoughts,Note that the Microsoft’s SDK version is the Beta version, so things may change when the final one is released)
Microsoft’s Kinect SDK (Beta)pro: 优点
- support for audio 支持音频
- support for motor/tilt 支持马达
- full body tracking: 全身追踪- does not need a calibration pose 不需要标定姿势（投降姿势）
- includes head, hands, feet, clavicles 包括头，手，脚和锁骨
- seems to deal better with occluded joints 看起来处理关节闭塞更好些 
- supports multiple sensors 支持多传感器（多台Kinect）
- single no-fuss installer 简化安装（安装更容易）
- SDK has events for when a new Video or new Depth frame is available 当新的视频或深度图有效时，SDK会有可用事件 
con: 缺点
- licensed for non-commercial use only 非商用（商业需要付费）
- only tracks full body (no mode for hand only tracking)  只能追踪全身（不包含特定的追踪模式：例如只追踪手）
- does not offer alignment of the color&depth image streams to one another yet 
	- 
- although there are features to align individual coordinates
- and there are hints that support may come later
	full body tracking:  全身追踪- 
- only calculates positions for the joints, not rotations 关节只有坐标数据，没有旋转数据only tracks the full body, no upperbody or hands only mode 只能追踪全身，不包含特定的追踪模式：例如只追踪手或上半身
- seems to consume more CPU power than OpenNI/NITE (not properly benchmarked)  和OpenNI/NITE相比，看起来更消耗CPU（没有采用适当的基准）
	no gesture recognition system  不包含手势识别系统 no support for the PrimeSense and the ASUS WAVI Xtion sensors? (can anyone confirm this?)  不支持PrimeSense和华硕的WAVI Xtion硬件平台 only supports Win7 (x86 & x64)   只支持Win7(32位和64位） no support for Unity3D game engine  不支持Unity3D游戏引擎 no built in support for record/playback to disk  不支持数据记录或回放到硬盘 no support to stream the raw InfraRed video data  不支持红外线视频数据流
- SDK does not have events for when new user enters frame, leaves frame etc  SDK没有此类发生事件，例如当一个用户被侦测到或用户丢失等等。 
PrimeSense OpenNI/NITEpro: 优点
- license includes commercial use 可以商用（不需要付费）
- includes a framework for hand tracking 包含手部追踪框架
- includes a framework for hand gesture recognition 包含手势识别框架
- can automatically align the depth image stream to the color image  可以自动对齐深度图数据到彩色图数据
- full body tracking:  全身追踪 - also calculates rotations for the joints 包含坐标数据和旋转数据
- support for hands only mode 支持特殊跟踪模式：例如：只追踪手和头或上半身
- seems to consume less CPU power than Microsoft Kinect SDK’s tracker (not properly benchmarked) 和微软的SDK相比消耗的CPU更少
- also supports the Primesense and the ASUS WAVI Xtion sensors 支持Primesense和华硕的WAVI Xtion硬件平台
- supports multiple sensors although setup and enumeration is a bit quirky 支持多传感器但是需要安装和枚举，这一点有点古怪。
- supports Windows (including Vista&XP), Linux and Mac OSX 支持Windows（包括Vista&XP&WIN7）,Linux系统和苹果操作系统（翻者：也支持Android）
- comes with code for full support in Unity3D game engine  自带的代码全面支持Unity3D游戏引擎（翻者：也支持Ogre）
- support for record/playback to/from disk 支持数据记录到硬盘或从硬盘回放数据
- support to stream the raw InfraRed video data 支持红外数据流
- SDK has events for when new User enters frame, leaves frame etc SDK有此类发生事件，例如：当一个用户被侦测到或者用户丢失。（提供回调函数供开发者使用） 
con: 缺点
- no support for audio 不支持音频
- no support for motor/tilt (although you can simultaneously use the CL-NUI motor drivers) 不支持马达（翻者：马达是微软的专利，所以primesense公司不想惹微软）
- full body tracking:  全身追踪 - lacks rotations for the head, hands, feet, clavicles 缺乏以下关节：头，手，脚，和锁骨
- needs a calibration pose to start tracking (although it can be saved/loaded to/from disk for reuse) 需要一个标定姿势（投降姿势）才能开始追踪骨骼（注意:标定数据是可以存储和提取的方便重用）
- occluded joints are not estimated 关节闭塞没有被估算
- supports multiple sensors although setup and enumeration is a bit quirky 支持多感应器但是需要安装和枚举，这一点有点古怪。
- three separate installers and a NITE license string (although the process can be automated with my auto driver installer)  需要单独安装NITE
- SDK does not have events for when new Video or new Depth frames is available SDK没有此类发生事件，例如：当新的视频或者深度图数据有效时。（翻者：OpenNI提供了类似功能的函数可使用，虽然不是回调函数，但是也很好用） 
(Personal) conclusion:Microsoft seems to have the edge when working with skeletons and/or audio.
微软在骨骼识别和音频方面有优势。（翻者：本人非常认同，微软的音频识别将会在未来的体感游戏里发挥重要的作用！）
OpenNI seems to be best suited when working on colored pointclouds, on non-Win7 platforms and/or for commercial projects.
OpenNI似乎更适合做一些带颜色的点云的工作，和在非Win7平台来开发商业项目。
When working with gestures in specific: 手势识别 
- If your sensor only sees the upperbody/hands and/or you want an existing framework to start with use OpenNI/NITE. 
如果你想开发基于上半身或手识别的项目，可以使用OpenNI和NITE
- When your sensor can see the full body the more stable Microsoft skeleton may be the best to use, however you’ll have to code your own gesture recognitions. (You’d also have to extend OpenNI/NITE for fullbody gestures btw)
全身识别毋庸置疑微软的SDK是最好的，然而你必须自己编写你自己的手势识别代码。
**（2）：数据获取借鉴的三篇文章：**
## 透過 OpneNI 讀取 Kinect 深度影像資料
：[http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=215](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=215)
## 透過 OpneNI 合併 Kinect 深度以及彩色影像資料
[http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=216](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=216)
## 透過 OpenNI 建立 Kinect 3D Point Cloud：
[http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=217](http://viml.nchc.org.tw/blog/paper_info.php?CLASS_ID=1&SUB_ID=1&PAPER_ID=217)
[还有一篇文章，比较详细：]()
### [Kinect运用OpenNI产生点云](http://blog.csdn.net/opensource07/article/details/7804246)
原文链接：http://blog.csdn.net/opensource07/article/details/7804246
（1）：作者的例程：利用OpenNI读取Kinect...
```cpp
#include <stdlib.h>
#include <iostream>
#include <string>
#include <XnCppWrapper.h>
using namespace std;
void CheckOpenNIError( XnStatus eResult, string sStatus )
{
  if( eResult != XN_STATUS_OK )
    cerr << sStatus << " Error: " << xnGetStatusString( eResult ) << endl;
}
int main( int argc, char** argv )
{
  XnStatus eResult = XN_STATUS_OK;
 
  // 2. initial context
  xn::Context mContext;
  eResult = mContext.Init();
  CheckOpenNIError( eResult, "initialize context" );
 
  // set map mode
  XnMapOutputMode mapMode;
  mapMode.nXRes = 640;
  mapMode.nYRes = 480;
  mapMode.nFPS = 30;
 
  // 3. create depth generator
  xn::DepthGenerator mDepthGenerator;
  eResult = mDepthGenerator.Create( mContext );
  CheckOpenNIError( eResult, "Create depth generator" );
  eResult = mDepthGenerator.SetMapOutputMode( mapMode );
 
  // 4. start generate data
  eResult = mContext.StartGeneratingAll();
 
  // 5. read data
  eResult = mContext.WaitAndUpdateAll();
  if( eResult == XN_STATUS_OK )
  {
    // 5. get the depth map
    const XnDepthPixel*  pDepthMap = mDepthGenerator.GetDepthMap();
    // 6. Do something with depth map
  }
  // 7. stop
  mContext.StopGeneratingAll();
  mContext.Shutdown();
 
  return 0;
}
```
（2）：合并深度和RGB图像：基于对昌邑程序的修改。
```cpp
#include <stdlib.h>
#include <iostream>
#include <string>
 
#include <XnCppWrapper.h>
  
using namespace std;
 
void CheckOpenNIError( XnStatus eResult, string sStatus )
{
  if( eResult != XN_STATUS_OK )
    cerr << sStatus << " Error : " << xnGetStatusString( eResult ) << endl;
}
 
int main( int argc, char** argv )
{
  XnStatus eResult = XN_STATUS_OK;
 
  // 2. initial context
  xn::Context mContext;
  eResult = mContext.Init();
  CheckOpenNIError( eResult, "initialize context" );
 
  // 3. create depth generator
  xn::DepthGenerator mDepthGenerator;
  eResult = mDepthGenerator.Create( mContext );
  CheckOpenNIError( eResult, "Create depth generator" );
  // 4. create image generator
  xn::ImageGenerator mImageGenerator;
  eResult = mImageGenerator.Create( mContext );
  CheckOpenNIError( eResult, "Create image generator" );
  // 5. set map mode
  XnMapOutputMode mapMode;
  mapMode.nXRes = 640;
  mapMode.nYRes = 480;
  mapMode.nFPS = 30;
  eResult = mDepthGenerator.SetMapOutputMode( mapMode );
  eResult = mImageGenerator.SetMapOutputMode( mapMode );
 
  // 6. correct view port
  mDepthGenerator.GetAlternativeViewPointCap().SetViewPoint( mImageGenerator );
 
  // 7. tart generate data
  eResult = mContext.StartGeneratingAll();
 
  // 8. read data
  eResult = mContext.WaitNoneUpdateAll();
  if( eResult == XN_STATUS_OK )
  {
    // 9a. get the depth map
    const XnDepthPixel*  pDepthMap = mDepthGenerator.GetDepthMap();
    // 9b. get the image map
    const XnUInt8*    pImageMap = mImageGenerator.GetImageMap();
  }
  
  // 10. stop
  mContext.StopGeneratingAll();
  mContext.Shutdown();
 
  return 0;
}
```
（3）：建立3D点云：对上一个程序的修改
       定义一个简单的结构体：SColorPoint3D：
```cpp
struct SColorPoint3D
{
  float  X;
  float  Y;
  float  Z;
  float  R;
  float  G;
  float  B;
 
  SColorPoint3D( XnPoint3D pos, XnRGB24Pixel color )
  {
    X = pos.X;
    Y = pos.Y;
    Z = pos.Z;
    R = (float)color.nRed / 255;
    G = (float)color.nGreen / 255;
    B = (float)color.nBlue / 255;
  }
};
```
六个点值：分別記錄這個點的位置、以及顏色；
建構子的部分：則是傳入 OpenNI 定義的結構的参数：代表位置的 XnPoint3D  以及代表 RGB 顏色的 XnRGB24Pixel。
       把座標轉換的部分寫成一個函数 GeneratePointCloud()，其內容如下：
```cpp
void GeneratePointCloud( xn::DepthGenerator& rDepthGen,
                         const XnDepthPixel* pDepth,
                         const XnRGB24Pixel* pImage,
                         vector<SColorPoint3D>& vPointCloud )
{
  // 1. number of point is the number of 2D image pixel
  xn::DepthMetaData mDepthMD;
  rDepthGen.GetMetaData( mDepthMD );
  unsigned int uPointNum = mDepthMD.FullXRes() * mDepthMD.FullYRes();
 
  // 2. build the data structure for convert
  XnPoint3D* pDepthPointSet = new XnPoint3D[ uPointNum ];
  unsigned int i, j, idxShift, idx;
  for( j = 0; j < mDepthMD.FullYRes(); ++j )
  {
    idxShift = j * mDepthMD.FullXRes();
    for( i = 0; i < mDepthMD.FullXRes(); ++i )
    {
      idx = idxShift + i;
      pDepthPointSet[idx].X = i;
      pDepthPointSet[idx].Y = j;
      pDepthPointSet[idx].Z = pDepth[idx];
    }
  }
 
  // 3. un-project points to real world
  XnPoint3D* p3DPointSet = new XnPoint3D[ uPointNum ];
  rDepthGen.ConvertProjectiveToRealWorld( uPointNum, pDepthPointSet, p3DPointSet );
  delete[] pDepthPointSet;
 
  // 4. build point cloud
  for( i = 0; i < uPointNum; ++ i )
  {
    // skip the depth 0 points
    if( p3DPointSet[i].Z == 0 )
      continue;
 
    vPointCloud.push_back( SColorPoint3D( p3DPointSet[i], pImage[i] ) );
  }
  delete[] p3DPointSet;
}
```
       函数把 xn::DepthGenerator 以及讀到的深度影像和彩色影像傳進來，用來當作資料來源；
       同時也傳入一個 vector<SColorPoint3D>，作為儲存轉換完成後的 3D 點位資料。
       其中，深度影像的格式還是一樣用 XnDepthPixel 的 const 指標，不過在彩色影像的部分，Heresy 則是改用把 RGB 封包好的 XnRGB24Pixel，這樣可以減少一些索引值的計算；而因為這樣修改，之前讀取彩色影像的程式也要
constXnUInt8* pImageMap = mImageGenerator.GetImageMap();
修改為
   constXnRGB24Pixel* pImageMap = mImageGenerator.GetRGB24ImageMap();
       回到主程序的部分，本來讀取資料的程序是：
```cpp
// 8. read data
eResult = mContext.WaitNoneUpdateAll();
if( eResult == XN_STATUS_OK )
{
  // 9a. get the depth map
  const XnDepthPixel*  pDepthMap = mDepthGenerator.GetDepthMap();
  // 9b. get the image map
  const XnUInt8*    pImageMap = mImageGenerator.GetImageMap();
}
```
       前面也提過，Heresy 這邊不打算提及用 OpenGL 顯示的部分，所以這邊為了不停地更新資料，所以改用一個無窮迴圈的形式來不停地更新資料、並進行座標轉換；
       而轉換後的結果，也很簡單地只輸出它的點的數目。
修改后：
```cpp
// 8. read data vector<SColorPoint3D> vPointCloud; while( true ) { eResult = mContext.WaitNoneUpdateAll(); // 9a. get the depth map const XnDepthPixel* pDepthMap = mDepthGenerator.GetDepthMap(); // 9b. get the image map const XnRGB24Pixel* pImageMap = mImageGenerator.GetRGB24ImageMap(); // 10 generate point cloud vPointCloud.clear(); GeneratePointCloud( mDepthGenerator, pDepthMap, pImageMap, vPointCloud ); cout << "Point number: " << vPointCloud.size() << endl; }
```
如果是要用 OpenGL 畫出來的話，基本上就是不要使用無窮迴圈，而是在每次要畫之前，再去讀取 Kinect 的資料、並透過 GeneratePointCloud() 做轉換了∼而如果不打算重建多邊形、而是像 Heresy 直接一點一點畫出來的話，結果大概就會像上面的影片一樣了∼
若用 OpenGL 畫出來的話，基本上就是不要使用无穷循环，而是在每次要畫之前，再去讀取 Kinect 的資料、並透過 GeneratePointCloud() 做轉換∼而如果不打算重建多邊形、而是像 Heresy 直接一點一點畫出來的話，結果大概就會像上面的影片一樣∼
**后记**：
       至此摘抄完毕，编译了程序，点云显示之后，测试程序检测无误....
