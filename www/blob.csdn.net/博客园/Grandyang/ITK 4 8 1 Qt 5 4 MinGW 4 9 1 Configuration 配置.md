# ITK 4.8.1 Qt 5.4 MinGW 4.9.1 Configuration 配置 - Grandyang - 博客园







# [ITK 4.8.1 Qt 5.4 MinGW 4.9.1 Configuration 配置](https://www.cnblogs.com/grandyang/p/4951798.html)







Download ITK 4.8.1

Download Qt 5.4 with MinGW 4.9.1

Download CMake 3.2.0



I assume you've already installed Qt 5.4 with MinGW 4.9.1 and CMake 3.2.0 correctly.

Please intall and configure VTK before the following steps, here is the blog for VTK: [VTK 6.3.0 Qt 5.4 MinGW 4.9.1 Configuration 配置](http://www.cnblogs.com/grandyang/p/4884184.html).



Start CMake 3.2.0, fill the source and destination:

source: **C:/ITK4.8.1/InsightToolkit-4.8.1**

destination: **C:/ITK4.8.1/build**

Click **Configure** and use **MinGW Makefiles** to complie.



When first configure is done, select **Grouped** and **Advanced**.

```
BUILD_EXAMPLES          [ON]  
BUILD_TEST              [ON]  
CMAKE_INSTALL_PREFIX    [C:/ITK4.8.1/MinGW]  
ITKV3_COMPATIBILITY     [ON]  
Module_ITKVtkGlue       [ON]  
VTK_DIR                 [C:/VTK6.3.0/MinGW/lib/cmake/vtk-6.3]  
Module_ITKReview        [ON]
```



click **Configure** again.

After configure is done (No errors show), click **Generate**.



Go back to your build folder, open command line, type: **mingw32-make**

**Wait a long time for make process, have a cup of coffee :)**

After make is done, type **mingw32-make install**



**Now, the configuration is done, enjoy it :)**



References:

[http://qtitkvtkhelp.blogspot.com/2013/06/image-display-on-vtk-viewers.html](http://qtitkvtkhelp.blogspot.com/2013/06/image-display-on-vtk-viewers.html)

[http://blog.csdn.net/numit/article/details/9985045](http://blog.csdn.net/numit/article/details/9985045)

[http://46aae4d1e2371e4aa769798941cef698.devproxy.yunshipei.com/q1302182594/article/details/45955563](http://46aae4d1e2371e4aa769798941cef698.devproxy.yunshipei.com/q1302182594/article/details/45955563)














