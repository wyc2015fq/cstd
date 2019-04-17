# 多个类的DLL封装及调用 - DoubleLi - 博客园







**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/7737682#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7737682#)





- #define FaceLIBDLL  
- 
- #include "stdafx.h"  
- #include "facedll.h"  
- #include <opencv2/opencv.hpp>  
- 
- //////////头文件中函数的实现  
- FaceRecognizer::FaceRecognizer()   
- {     
- 
- }   
- 
- FaceRecognizer::~FaceRecognizer()   
- {     
- 
- }   

生成（Build）工程，在debug文件夹中会生成相应的DLL及LIB文件：facedll.dll   facedll.lib



## 多个类封装DLL

封装好一个类之后，在后面的类可以调用这个类生成的dll，再封装新类的dll。

需要在工程中添加需要引用的头文件，如facedll.h。在debug中拷贝facedll.lib文件。在 Properties->Linker->Input->  Additional Dependecies中添加facedll.lib（或写全路径："..\debug\facedll.lib"）

然后一样的方法再封装新的类就可以了~





**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/7737682#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7737682#)





- #pragma once  
- #ifdef HEARTLIBDLL  
- #define HEARTAPI _declspec(dllexport)  
- #else  
- #define HEARTAPI  _declspec(dllimport)  
- #endif  
- 
- #include <opencv2/opencv.hpp>  
- #include "facedll.h"  
- #include "datadll.h"  
- 
- class HEARTAPI  HRMeasure  
- {  
- }；  




## 调用DLL

调用需要各个dll的.h、.dll、.lib文件。

将头文件添加到工程中，并#include到需要用的地方。将lib文件拷贝到项目中，并在 Properties->Linker->Input->  Additional Dependecies 中写入：facedll.lib;heartdll.lib。

或者在程序中写入：




**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/7737682#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7737682#)





- #pragma  comment(lib,"facedll.lib")  
- #pragma  comment(lib,"heartdll.lib")  


之后程序中就可以直接使用封装成DLL的类了：




**[cpp]**[view plain](http://blog.csdn.net/xiaowei_cqu/article/details/7737682#)[copy](http://blog.csdn.net/xiaowei_cqu/article/details/7737682#)





- HRMeasure *hrMea=new HRMeasure();  
- 











