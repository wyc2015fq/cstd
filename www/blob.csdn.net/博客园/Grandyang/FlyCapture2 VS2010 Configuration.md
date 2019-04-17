# FlyCapture2 VS2010 Configuration - Grandyang - 博客园







# [FlyCapture2 VS2010 Configuration](https://www.cnblogs.com/grandyang/p/4263490.html)







**Add in the system Path:**

C:\Program Files (x86)\Point Grey Research\FlyCapture2\bin



**Project->Project Property->Configuration Properties->VC++Directories ->Include Directories:**

C:\Program Files (x86)\Point Grey Research\FlyCapture2\include
C:\Program Files (x86)\Point Grey Research\FlyCapture2\include\FC1
C:\Program Files (x86)\Point Grey Research\FlyCapture2\include\C



**Project->Project Property->Configuration Properties->VC++Directories ->Library Directories:**

C:\Program Files (x86)\Point Grey Research\FlyCapture2\lib
C:\Program Files (x86)\Point Grey Research\FlyCapture2\lib\C
C:\Program Files (x86)\Point Grey Research\FlyCapture2\lib\FC1



**Project->Project Property->Configuration Properties->C/C++ ->Preprocessor->Preprocessor Definitions:**

WIN32
_DEBUG
_CONSOLE



**Project->Project Property->Configuration Properties->Linker->Input:**

FlyCapture2_C.lib
FlyCapture2GUI_C.lib
FlyCapture2.lib
FlyCapture2GUI.lib
FlyCapture2GUId.lib



**Include in the headfile:**

#include "FlyCapture2.h"
#include "FlyCapture2_C.h"





------------------------------------------------------------------------------------------------------------------------------------------

If we install the SDK at "C:\PointGreyResearch\" in order to get rid of spaces 

Only use C API, then do the following:

**Project->Project Property->Configuration Properties->VC++Directories ->Include Directories:**

C:\PointGreyResearch\FlyCapture2\include



**Project->Project Property->Configuration Properties->VC++Directories ->Library Directories:**
C:\PointGreyResearch\FlyCapture2\lib\C



**Project->Project Property->Configuration Properties->Linker->Input:**

FlyCapture2_C.lib
FlyCapture2_Cd.lib
FlyCapture2GUI_C.lib
FlyCapture2GUI_Cd.lib



**Include in the headfile:**

#include "C/FlyCapture2_C.h"



Only use C++ API, then do the following:

**Project->Project Property->Configuration Properties->VC++Directories ->Include Directories:**

C:\PointGreyResearch\FlyCapture2\include



**Project->Project Property->Configuration Properties->VC++Directories ->Library Directories:**
C:\PointGreyResearch\FlyCapture2\lib



**Project->Project Property->Configuration Properties->Linker->Input:**
FlyCapture2.lib
FlyCapture2d.lib
FlyCapture2GUI.lib
FlyCapture2GUId.lib



**Include in the headfile:**

#include "FlyCapture2.h"














