# CxImage的编译及简单使用举例 - DoubleLi - 博客园






1、  从[http://sourceforge.net/projects/cximage/](http://sourceforge.net/projects/cximage/)下载最新的CxImage 702源码；

2、  解压缩后，以管理员身份打开CxImageFull_vc10.sln工程，在编译之前先将每个工程属性的Character Set由原先的Use Unicode Character Set改为Use Multi-ByteCharacter Set，首先编译jasper、jbig、jpeg、libdcr、libpsd、mng、png、tiff、zlib这9个库，然后编译cximage，cximagecrtdll，在接着编译cximagemfcdll，在编译cximagemfcdll之前，先修改其属性，linker->input->Additional Dependencies,将$(OutDir)png.lib等改为../../Debug/png.lib(../../Release/png.lib)，最后编译demo、demodll；全部编译完后即可生成相应的静态库和动态库；

3、  目前CxImage支持的图像格式包括：bmp、gif、jpg、png、ico、tif、tga、pcx、wbmp、wmf、jp2、jpc、pgx、pnm、ras、jbg、mng、ska、raw和psd；

4、  CxImage中所包含的图像操作可通过打开index.htm来查看；        

5、新建一个控制台工程testCxImage，将Character Set设为Use Multi-Byte Character Set，各个文件的内容为：

stdafx.h:





**[cpp]**[view plain](http://blog.csdn.net/fengbingchun/article/details/38795713#)[copy](http://blog.csdn.net/fengbingchun/article/details/38795713#)



- #pragma once  
- 
- #include "targetver.h"  
- 
- #include <stdio.h>  
- 
- #include "../../cximage702_full/CxImage/ximage.h"  



stdafx.cpp:







**[cpp]**[view plain](http://blog.csdn.net/fengbingchun/article/details/38795713#)[copy](http://blog.csdn.net/fengbingchun/article/details/38795713#)



- #include "stdafx.h"  
- 
- // TODO: reference any additional headers you need in STDAFX.H  
- // and not in this file  
- 
- #ifdef _DEBUG  
-     #pragma comment(lib, "../../cximage702_full/Debug/cximage.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/jasper.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/jbig.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/jpeg.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/libdcr.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/libpsd.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/mng.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/png.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/tiff.lib")  
-     #pragma comment(lib, "../../cximage702_full/Debug/zlib.lib")  
- #else  
-     #pragma comment(lib, "../../cximage702_full/Release/cximage.lib")   
-     #pragma comment(lib, "../../cximage702_full/Release/jasper.lib")  
-     #pragma comment(lib, "../../cximage702_full/Release/jbig.lib")  
-     #pragma comment(lib, "../../cximage702_full/Release/jpeg.lib")  
-     #pragma comment(lib, "../../cximage702_full/Release/libdcr.lib")  
-     #pragma comment(lib, "../../cximage702_full/Release/libpsd.lib")  
-     #pragma comment(lib, "../../cximage702_full/Release/mng.lib")  
-     #pragma comment(lib, "../../cximage702_full/Release/png.lib")  
-     #pragma comment(lib, "../../cximage702_full/Release/tiff.lib")  
-     #pragma comment(lib, "../../cximage702_full/Release/zlib.lib")  
- #endif  



testCxImage.cpp:







**[cpp]**[view plain](http://blog.csdn.net/fengbingchun/article/details/38795713#)[copy](http://blog.csdn.net/fengbingchun/article/details/38795713#)



- #include "stdafx.h"  
- #include <iostream>  
- #include <string>  
- 
- using namespace std;  
- 
- int main(int argc, char* argv[])  
- {  
-     CxImage image;  
-     string imageName = "1.jpg";  
-     string imageSave = "2.tif";  
- 
-     image.Load(imageName.c_str(), CXIMAGE_FORMAT_JPG);  
- 
-     cout<<image.GetBpp()<<endl;  
- 
- if (image.IsValid()) {  
-         image.GrayScale();  
-         image.Save(imageSave.c_str(), CXIMAGE_FORMAT_TIF);  
- 
-         cout<<"success"<<endl;  
-     }  
- 
-     cout<<"ok"<<endl;  
- return 0;  
- }  



**GitHub**：[https://github.com/fengbingchun/CxImage_Test](https://github.com/fengbingchun/CxImage_Test)









