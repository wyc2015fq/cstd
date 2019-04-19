# Wince下检测SD/USB插拔 - xqhrs232的专栏 - CSDN博客
2013年04月22日 15:08:18[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：6201
原文地址::[http://blog.csdn.net/hugohong/article/details/6072138](http://blog.csdn.net/hugohong/article/details/6072138)
相关网帖
1、[WINCE下检测SD卡](http://blog.csdn.net/huang5556019/article/details/7773532)----[http://blog.csdn.net/huang5556019/article/details/7773532](http://blog.csdn.net/huang5556019/article/details/7773532)
在wince系统中，如何检测USB/SD卡的拔插呢？可能有的会写一个线程专门去侦察USB/SD 的拔插情况，我觉得这样又费资源又费时间，很不划算，有没有更好的办法呢？当然有，今天我就介绍一种新的方法：WM_DEVICECHANGE，如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hugohong/article/details/6072138#)[copy](http://blog.csdn.net/hugohong/article/details/6072138#)
- LRESULT Cxxx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)  
- {  
- // TODO: 在此添加专用代码和/或调用基类
- if (message == WM_DEVICECHANGE )  
-     {  
-         PDEV_BROADCAST_HDR lpdb = (PDEV_BROADCAST_HDR)lParam;  
- switch(wParam)  
-         {  
- case DBT_DEVICEARRIVAL:  
- if( lpdb->dbch_devicetype == DBT_DEVTYP_PORT )  
-             {  
-                 PDEV_BROADCAST_PORT lpbp;  
-                 lpbp = (PDEV_BROADCAST_PORT)lpdb;  
-                 TRACE(_T("device:%s/r/n"), lpbp->dbcp_name);  
- if (_tcscmp(lpbp->dbcp_name, _T("DSK2:")) == 0)  
-                 {  
-                     AfxMessageBox(_T("sd device insert!"));  
-                 }  
- elseif (_tcscmp(lpbp->dbcp_name, _T("DSK3:")) == 0)  
-                 {  
-                     AfxMessageBox(_T("usb device insert!"));  
-                 }  
-             }  
- break;  
- 
- case DBT_DEVICEREMOVECOMPLETE:  
- if( lpdb->dbch_devicetype == DBT_DEVTYP_PORT )  
-             {  
-                 PDEV_BROADCAST_PORT lpbp;  
-                 lpbp = (PDEV_BROADCAST_PORT)lpdb;  
-                 TRACE(_T("device:%s/r/n"), lpbp->dbcp_name);  
- if (_tcscmp(lpbp->dbcp_name, _T("DSK2:")) == 0)  
-                 {  
-                     AfxMessageBox(_T("sd remove!"));  
-                 }  
- elseif (_tcscmp(lpbp->dbcp_name, _T("DSK3:")) == 0)  
-                 {  
-                     AfxMessageBox(_T("usb remove!"));  
-                 }  
-             }  
- break;  
- 
- default:  
- break;  
-         }  
-     }  
- 
- return CDialog::DefWindowProc(message, wParam, lParam);  
- }  
这样的话，又快又准确; 不过注意的是，我在注册表里已经设定的SD卡为DSK2, USB为DSK3,要不然的话那个先插那个就是DSK2(flash 为DSK1)了， 就这么简单！
//====================================================================================================
备注::
1、记得包含头文件------#include "dbt.h"

