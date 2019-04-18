# 仿腾讯 QQ 和 Skype 通过URL触发自己的程序 - ljx0305的专栏 - CSDN博客
2008年02月15日 17:54:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：537标签：[url																[qq																[腾讯																[browser																[浏览器																[hook](https://so.csdn.net/so/search/s.do?q=hook&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=browser&t=blog)](https://so.csdn.net/so/search/s.do?q=腾讯&t=blog)](https://so.csdn.net/so/search/s.do?q=qq&t=blog)](https://so.csdn.net/so/search/s.do?q=url&t=blog)
**仿腾讯 QQ 和 Skype 通过URL触发自己的程序**
作者：[谢红伟](mailto:chrys@163.com)
[下载源代码](http://www.vckbase.com/code/network/internet/CustomWebProtocol.rar)
　　如果你电脑中装有QQ，在IE地址栏输入：“tencent://Message/?menu=yes&exe=&uin=13231462”然后[回车]，立即可以与我的QQ建立临时会话，如下图： 
![](http://www.vckbase.com/document/journal/vckbase52/images/qqskypeimg1.gif)
Skype也有类似的功能。到底是如何实现的呢？看MSDN中有这么一段话： 
***　　The IURLSearchHook interface is used by the browser to translate the address of an unknown URL protocol. When attempting to browse to a URL address that does not contain a protocol, the browser will first attempt to determine the correct protocol from the address. If this is not successful, the browser will create URL Search Hook objects and call each object's Translate method until the address is translated or all of the hooks have been queried. ***
　　IURLSearchHook接口被浏览器用来转换一个未知的URL协议地址。当浏览器企图去打开一个未知协议的URL地址时，浏览器首先尝试从这个地址得到当前的协议，如果不成功，浏览器将创建在系统中注册的URL Search Hook对象并调用每一个对象的Translate方法，直到地址被转换或所有的URL Search Hook都尝试过。
　　也就是说，我们可以注册一种目前不存在的协议（类似HTTP），当浏览器遇到新的协议时会自动调用Translate方法来翻译我们的协议，甚至激活我们自己的程序。
以下源代码将实现注册一个新的自定义的Web协议： 
//
// 注册自定义的Web协议
// return : ------------------------------------------------------------------------
//		0	-	失败
//		1	-	成功
//		2	-	已经存在
//
int RegWebProtocol ( LPCTSTR lpszProtocolName, LPCTSTR lpszAssociatedApp, int nIconIndex/*=0*/ )
{
	if ( !lpszProtocolName || 
	     lstrlen(lpszProtocolName) < 1 || 
	     !lpszAssociatedApp || 
	     lstrlen(lpszAssociatedApp) < 1 )   
		return 0;
	CString csSubKey;
	DWORD dwBufSize = 0;
	// 该协议已经存在
	HKEY hKey = NULL;
	if ( RegOpenKeyEx ( HKEY_CLASSES_ROOT,
				lpszProtocolName,
				0,
				KEY_ALL_ACCESS,
				&hKey ) == ERROR_SUCCESS )
	{
		return 2;
	}
	else hKey = NULL;
	// 创建协议子键
	if ( !CreateRegisterSubKey ( HKEY_CLASSES_ROOT, lpszProtocolName ) )
		return 0;
	// 设置协议描述字符串
	CString csProtocolDesc; csProtocolDesc.Format ( _T("%sProtocol"), lpszProtocolName );
	dwBufSize = csProtocolDesc.GetLength();
	if ( !WriteRegister ( HKEY_CLASSES_ROOT, lpszProtocolName,
		_T(""), REG_EXPAND_SZ, (PUCHAR)csProtocolDesc.GetBuffer(0),&dwBufSize) )
		return 0;
	CString csAppFile; csAppFile.Format ( _T("%s"), lpszAssociatedApp );
	dwBufSize = csAppFile.GetLength();
	if ( !WriteRegister ( HKEY_CLASSES_ROOT, lpszProtocolName,
		_T("URL Protocol"), REG_EXPAND_SZ, (PUCHAR)csAppFile.GetBuffer(0),&dwBufSize) )
		return 0;
	// DefaultIcon 子键
	csSubKey.Format ( _T("%s//DefaultIcon"), lpszProtocolName );
	if ( !CreateRegisterSubKey ( HKEY_CLASSES_ROOT, csSubKey ) )
		return 0;
	CString csIconParameter; csIconParameter.Format ( _T("%s,%d"), lpszAssociatedApp, nIconIndex );
	dwBufSize = csIconParameter.GetLength();
	if ( !WriteRegister ( HKEY_CLASSES_ROOT, csSubKey,
		_T(""), REG_EXPAND_SZ, (PUCHAR)csIconParameter.GetBuffer(0),&dwBufSize) )
		return 0;
	// shell/open/command 子键
	csSubKey.Format ( _T("%s//shell//open//command"), lpszProtocolName );
	if ( !CreateRegisterSubKey ( HKEY_CLASSES_ROOT, csSubKey ) )
		return 0;
	CString csCommand; csCommand.Format ( _T("/"%s/" /"%%1/""), lpszAssociatedApp );
	dwBufSize = csCommand.GetLength();
	if ( !WriteRegister ( HKEY_CLASSES_ROOT, csSubKey,
		_T(""), REG_EXPAND_SZ, (PUCHAR)csCommand.GetBuffer(0),&dwBufSize) )
		return 0;
	return 1;
}
以下源代码将删除自定义的Web协议：
//
// 卸载自定义的Web协议
//
BOOL UnRegWebProtocol ( LPCTSTR lpszProtocolName )
{   
	if ( !lpszProtocolName || lstrlen(lpszProtocolName) < 1 )
		return FALSE;
	return RegDeleteAllSubKey ( HKEY_CLASSES_ROOT, lpszProtocolName );
}
我提供一个详细的源代码可以演示整个过程，程序界面如下：
![](http://www.vckbase.com/document/journal/vckbase52/images/qqskypeimg2.gif)
　　先设置好需要注册的协议名称和关联的执行程序文件名，然后点[Register Protocol]按钮完成注册，点击[Test Protocol]按钮测试注册后的效果。
　　如本例中注册一个“HXXP”协议，关联程序就是本程序，当点击[Test Protocol]按钮时就会执行该程序的另一个副本，同时[Test URL Parameter]输入的参数也被显示出来了。您也可以将“HXXP://Para1/Para2/Para2”复制到IE地址栏中执行，实现相同的效果，如下图：
![](http://www.vckbase.com/document/journal/vckbase52/images/qqskypeimg3.gif)
最后，谢谢曹昌利同志讲述QQ临时会话的相关知识。
 转自[http://www.vckbase.com/document/viewdoc/?id=1804](http://www.vckbase.com/document/viewdoc/?id=1804)
