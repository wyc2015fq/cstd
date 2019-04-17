# OpenCV 3.0 VS2010 Configuration - Grandyang - 博客园







# [OpenCV 3.0 VS2010 Configuration](https://www.cnblogs.com/grandyang/p/4446161.html)







**Add in the system Path:**

C:\opencv\build\x86\vc10\bin;



**Project->Project Property->Configuration Properties->VC++Directories ->Include Directories:**

C:\opencv\build\include\opencv

C:\opencv\build\include\opencv2

C:\opencv\build\include



**Project->Project Property->Configuration Properties->VC++Directories ->Library Directories:**

C:\opencv\build\x86\vc10\lib

C:\opencv\build\x86\vc10\staticlib



**Project->Project Property->Configuration Properties->Linker->Input:**

opencv_ts300d.lib
opencv_world300d.lib
opencv_ts300.lib
opencv_world300.lib














