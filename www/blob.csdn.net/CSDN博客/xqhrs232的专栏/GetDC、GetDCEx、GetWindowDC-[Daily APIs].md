# GetDC、GetDCEx、GetWindowDC - [Daily APIs] - xqhrs232的专栏 - CSDN博客
2012年10月14日 22:14:36[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：749
原文地址::[http://www.cnblogs.com/Lisen/archive/2010/01/22/1654384.html](http://www.cnblogs.com/Lisen/archive/2010/01/22/1654384.html)
相关网帖
1.GDI编程小结----[http://www.360doc.com/content/10/1020/20/3550092_62550872.shtml](http://www.360doc.com/content/10/1020/20/3550092_62550872.shtml)
2.[关于DC的理解](http://blog.csdn.net/cys1991/article/details/7648805)----[http://blog.csdn.net/cys1991/article/details/7648805](http://blog.csdn.net/cys1991/article/details/7648805)
3.详解windows设备内容(device context)----[http://www.58os.com/BBS/TechBSS/Article-5967.html](http://www.58os.com/BBS/TechBSS/Article-5967.html)
4.Vc中关于DC（设备环境函数（Device
 Context））的理解----[http://caodong8962.itpub.net/post/42952/519845](http://caodong8962.itpub.net/post/42952/519845)
5.**bitmap相关的一些知识----[http://smycll.blog.hexun.com/74043325_d.html](http://smycll.blog.hexun.com/74043325_d.html)**
GetDC
**该函数为指定窗口的客户区域取回显示DC的句柄。随后的GDI函数中可以使用显示DC在窗口客户区域绘图。**
**原型：**
HDCGetDC( HWNDhWnd);
**参数：**
hWnd：将要取回DC的窗口句柄。如果该值为NULL，GetDC取回整个屏幕的DC。
返回值：
成功，获得指定窗口客户区域的DC句柄；失败返回NULL。调用GetLastError获取扩展错误信息。
备注：
   根据为指定窗口指定的类类型来决定取回一个公共、类或私有DC(a common, class, or private device context )。
   每次取回公共DC时，GetDC函数给它赋值默认属性。
   取回类或私有DC时，GetDC函数保持之前赋值的属性不变。
   在使用公共DC绘图后，必须调用**ReleaseDC**来释放DC。
   类和私有DC不必释放。
   可用内存数决定DC的数量。
GetDCEx
**该函数为指定窗口的客户区域或整个屏幕取回显示DC的句柄。随后的GDI函数中可以该句柄在窗口客户区域绘图。**
**GetDCEx 是GetDC的扩展，对如何剪裁客户区域或是否剪裁客户区域提供给程序更多的控制。**
**原型：**
HDCGetDCEx( 
HWNDhWnd, 
HRGNhrgnClip, 
DWORDflags);
参数：
hWnd：将要取回DC的窗口句柄。如果该值为NULL，GetDCEx取回整个屏幕的DC。
hrgnClip：指定可能和DC的可见区域合并的剪切区域。
flags：指定DC的创建方式。可能为下表中的值的组合：
|值|描述|
|----|----|
|**DCX_WINDOW**|返回于窗口矩形而不是客户端矩形对应的DC。|
|**DCX_CACHE  **|从缓存中取回DC，而不是OWNDC 或 CLASSDC 窗口。实际上重写了CS_OWNDC 和CS_CLASSDC 了。|
|**DCX_PARENTCLIP  **|使用父窗口的可见区域。取消父窗口的WS_CLIPCHILDREN 和 CS_PARENTDC 标志位。DC的起始坐标设置为由hWnd标识的窗口的左上角。|
|**DCX_CLIPSIBLINGS  **|排除由hWnd标识的窗口的所有同级窗口的可见区域。|
|**DCX_CLIPCHILDREN**|排除由hWnd标识的窗口的所有子窗口的可见区域。|
|**DCX_NORESETATTRS  **|当该DC释放时，不用设置该DC的属性为默认值。|
|**DCX_EXCLUDERGN  **|从返回的DC的可见区域里去除hrgnClip标识的剪切区域。|
|**DCX_EXCLUDEUPDATE**|返回一个区域，该区域去除了窗口更新的区域。|
|**DCX_INTERSECTRGN  **|hrgnClip表示的剪切区域和返回DC的可见区域取交集。|
|**DCX_INTERSECTUPDATE  **|返回一个区域，该区域包含窗口更新的区域。|
|**DCX_VALIDATE  **|当指定DCX_INTERSECTUPDATE，引起DC完全有效(causes the device context to be completely validated.)。 在该函数中使用DCX_INTERSECTUPDATE 和DCX_VALIDATE 等价于使用BeginPaint函数。|
**返回值：**
   成功，返回指定窗口DC句柄。失败返回NULL。如果传入的hWnd为无效值，返回失败。调用GetLastError获取扩展错误信息。
**备注：**
   flags支持这些值：**DCX_WINDOW**, **DCX_CACHE**, **DCX_CLIPCHILDREN**, **DCX_CLIPSIBLINGS**,**DCX_EXCLUDERGN**, **DCX_INTERSECTRGN**, **DCX_EXCLUDEUPDATE **和**DCX_INTERSECTUPDATE**。
   和GetDC返回的DC不同，GetDCEx返回的DC不会和相关窗口保持同步。
   如果窗口移动或改变大小，可能会产生异常错误。如果程序获取DC、绘图然后再释放DC，错误的机会将会减少。
   除非显示DC属于窗口类，否则在绘图后必须调用**ReleaseDC**来释放DC。
   因为一次只能有5个公共DC可用，如果释放DC失败，将可能导致其他程序访问DC失败。
   如果**CS_CLASSDC, CS_OWNDC **或** CS_PARENTDC**被指定为WNDCLASS结构中的一个类型，当该窗口类被注册时，函数将返回属于窗口类的DC。
GetWindowDC
**函数取回整个窗口DC，包括标题栏、菜单和滚动栏。**
**原型：**
HDCGetWindowDC(HWNDhWnd);
**参数：**
hWnd：要取回DC的窗口句柄。如果该值为NULL，取回整个屏幕的DC。
**返回值：**
   成功，返回指定窗口DC的句柄。NULL表示失败或传入的hWnd为无效参数。调用GetLastError获取扩展错误信息。
**备注：**
**GetWindowDC**专为在窗口非客户区域绘图而设计。不推荐在任何窗口的非客户区域绘图。
   使用**GetSystemMetrics**函数获取象标题栏、菜单和滚动栏等非客户区域的尺寸大小。
**GetDC**用来取回整个**屏幕**的DC。
   在绘图完成后，必须调用ReleaseDC来释放DC。如果不释放窗口DC，应用程序的绘图请求将会产生不良影响。
//===============================================
备注::
1.因为一次只能有5个公共DC可用，如果释放DC失败，将可能导致其他程序访问DC失败。-----不知道真这话是不是一个真理，有时间的好好验证一下！！！
2.bitmap只可以选入"内存DC",一个bitmap不能同时选入多个DC
