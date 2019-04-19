# MFC对话框使用CPrintDialog实现打印，指定打印机、后台打印 - xqhrs232的专栏 - CSDN博客
2018年01月25日 11:16:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：712
原文地址::[http://blog.csdn.net/xuqiang918/article/details/39962341](http://blog.csdn.net/xuqiang918/article/details/39962341)
相关文章
1、vc6.0 c++打印机设备编写代码----[http://blog.csdn.net/aile770339804/article/details/7655629](http://blog.csdn.net/aile770339804/article/details/7655629)
2、[vc
 实现打印机打印功能](http://www.cnblogs.com/MMLoveMeMM/articles/3138000.html)----[https://www.cnblogs.com/MMLoveMeMM/articles/3138000.html](https://www.cnblogs.com/MMLoveMeMM/articles/3138000.html)
[http://blog.csdn.net/xjkstar/article/details/25828097](http://blog.csdn.net/xjkstar/article/details/25828097)
推荐下 不错。
对话框打印，网上一搜一大堆，基本分2类：
A类： CPrintDialog.DoModal，然后在模态对话框里选打印机、打印配置；
B类：GetPrinterDeviceDefaults，调用默认打印机。
我的工作内容是理解以上2类后，再根据MSDN，实现MDF对话框后台指定打印机打印。
废话不多说，上菜~
功能：基于对话框的MFC打印（非文档视图结构），指定打印机，后台打印（不弹出对话框）
思路：
1、枚举打印机，并选择其中一个；
2、CPrintDialog实例指定到选中的打印机；
3、CPrintDialog后台打印
具体实现：
1、变量（控件）。在对话框上添加一个combobox（IDC_COMBO1，对应变量m_cboPrint）、一个edit（IDC_EDIT1），edit允许回车，多行（代码就不贴了，知道MFC应该就懂）；
2、在OnInitDialog里枚举打印机设备，如果报函数未定义，加入头文件#include <winspool.h>
需要调用两次EnumPrinters函数，第一次的到结构体的大小，第二次得到打印机列表
[cpp][view
 plain](http://blog.csdn.net/xjkstar/article/details/25828097#)[copy](http://blog.csdn.net/xjkstar/article/details/25828097#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/345503/fork)
- // TODO: 在此添加额外的初始化代码
- DWORD dwNeeded;  
- DWORD dwReturn;  
- DWORD dwFlag = PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL;  
- 
-     EnumPrinters(dwFlag, NULL, 4, NULL, 0, &dwNeeded, &dwReturn);  
- 
-     PRINTER_INFO_4* p4;  
-     p4 = new PRINTER_INFO_4[dwNeeded];  
-     EnumPrinters(dwFlag, NULL, 4, (PBYTE)p4, dwNeeded, &dwNeeded, &dwReturn);  
- 
- for (int i = 0; i<(int)dwReturn; i++)  
- this->m_cboPrint.AddString(p4[i].pPrinterName);  
- 
- delete []p4;  
3、操作对话框，在IDC_EDIT1里输入打印的内容，在IDC_COMBO1里选中打印机；
4、打印（我是用OK按钮打印的，大家随便）
[cpp][view
 plain](http://blog.csdn.net/xjkstar/article/details/25828097#)[copy](http://blog.csdn.net/xjkstar/article/details/25828097#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/345503/fork)
- // TODO: 在此添加控件通知处理程序代码
- //  CDialogEx::OnOK();
- this->UpdateData();  
- 
-     CString strMessage;  
-     CString strPrintDevice;  
- 
- this->GetDlgItem(IDC_EDIT1)->GetWindowTextW(strMessage);  
-     strMessage += _T("\r\n");       //添加结尾，方便后面循环读取打印数据
- 
- this->GetDlgItem(IDC_COMBO1)->GetWindowTextW(strPrintDevice);  
- 
- DWORD dwFlag = PD_ALLPAGES | PD_NOPAGENUMS | PD_USEDEVMODECOPIES | PD_HIDEPRINTTOFILE;  //打印配置界面的按钮可用性，因为后台打印，其实这个配置没什么意义
- 
-     CPrintDialog pPrintdlg(FALSE, dwFlag, this);                                            //CPrintDialog实例化，因为MFC的打印设备无关性，可以理解为这就是一台打印机
- 
- HGLOBAL hDevMode = NULL;  
- HGLOBAL hDevNames = NULL;  
- if (GetPrinterDevice(strPrintDevice.GetBuffer(0), &hDevNames, &hDevMode))               //获得指定打印机的配置、名字
-         AfxGetApp()->SelectPrinter(hDevNames, hDevMode);  
- else
-         AfxMessageBox(_T("Failed to select custom printer"));  
- 
-     strPrintDevice.ReleaseBuffer();  
- 
-     pPrintdlg.m_pd.hDevMode = hDevMode;                                                     //让pPrintdlg使用我们指定的打印机
-     pPrintdlg.m_pd.hDevNames = hDevNames;  
- 
-     CDC dc;  
-     dc.Attach(pPrintdlg.CreatePrinterDC());                                                 //后台打印创建法，如果需要弹出打印对话框，请用DoModal
- 
-     DOCINFO di;                                                                             //下面的内容网上很多，就不解释了
-     di.cbSize = sizeof(DOCINFO);  
-     di.lpszDocName = _T("有驱打印测试");  
-     di.lpszDatatype = NULL;  
-     di.lpszOutput = NULL;  
-     di.fwType = 0;  
- 
-     dc.StartDocW(&di);  
-     dc.StartPage();  
-     dc.SetMapMode(MM_TEXT);  
- 
-     CRect recPrint(0, 0, dc.GetDeviceCaps(LOGPIXELSX), dc.GetDeviceCaps(LOGPIXELSY));  
-     dc.DPtoLP(&recPrint);  
-     dc.SetWindowOrg(0, 0);  
- 
-     CFont newFont;  
-     VERIFY(newFont.CreatePointFont(120, _T("宋体"), &dc));  
-     CFont* oldFont = dc.SelectObject(&newFont);  
- 
-     dc.SetTextAlign(TA_TOP | TA_LEFT);  
- 
-     CString strPrint;  
- int nIndex = 0;  
- int x = 50;  
- int y = 50;  
-     CSize textSize;  
-     textSize = dc.GetTextExtent(_T("00"), 2);                           //根据当前字体的宽、高，后面以此高度为行高
- 
- while ((nIndex = strMessage.Find(_T("\r\n"))) > -1)                  //将IDC_EDIT1编辑框中内容打印，支持换行，一次换行等于'\r\n'，所以在开头strMessage += _T("\r\n")
-     {  
-         strPrint = strMessage.Left(nIndex);  
-         strMessage = strMessage.Mid(nIndex+2);  
- 
-         dc.TextOutW(x, y, strPrint);  
- 
-         y += textSize.cy;                                               //下移一行，行高为字体高度
-     }  
- 
-     dc.SelectObject(oldFont);  
-     newFont.DeleteObject();  
-     dc.EndPage();  
-     dc.EndDoc();  
-     DeleteDC(dc.Detach());  
关于GetPrinterDevice，来自微软的一篇文章，[点我跳转](http://support.microsoft.com/kb/166129)
代码也贴出来
[cpp][view
 plain](http://blog.csdn.net/xjkstar/article/details/25828097#)[copy](http://blog.csdn.net/xjkstar/article/details/25828097#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/345503/fork)
- BOOL CMFCApplication2Dlg::GetPrinterDevice(LPTSTR pszPrinterName, HGLOBAL* phDevNames, HGLOBAL* phDevMode)  
- {  
- // if NULL is passed, then assume we are setting app object's
- // devmode and devnames
- if (phDevMode == NULL || phDevNames == NULL)  
- return FALSE;  
- 
- // Open printer
- HANDLE hPrinter;  
- if (OpenPrinter(pszPrinterName, &hPrinter, NULL) == FALSE)  
- return FALSE;  
- 
- // obtain PRINTER_INFO_2 structure and close printer
- DWORD dwBytesReturned, dwBytesNeeded;  
-     GetPrinter(hPrinter, 2, NULL, 0, &dwBytesNeeded);  
-     PRINTER_INFO_2* p2 = (PRINTER_INFO_2*)GlobalAlloc(GPTR,  
-         dwBytesNeeded);  
- if (GetPrinter(hPrinter, 2, (LPBYTE)p2, dwBytesNeeded,  
-         &dwBytesReturned) == 0) {  
-             GlobalFree(p2);  
-             ClosePrinter(hPrinter);  
- return FALSE;  
-     }  
-     ClosePrinter(hPrinter);  
- 
- // Allocate a global handle for DEVMODE
- HGLOBAL  hDevMode = GlobalAlloc(GHND, sizeof(*p2->pDevMode) +  
-         p2->pDevMode->dmDriverExtra);  
-     ASSERT(hDevMode);  
-     DEVMODE* pDevMode = (DEVMODE*)GlobalLock(hDevMode);  
-     ASSERT(pDevMode);  
- 
- // copy DEVMODE data from PRINTER_INFO_2::pDevMode
-     memcpy(pDevMode, p2->pDevMode, sizeof(*p2->pDevMode) +  
-         p2->pDevMode->dmDriverExtra);  
-     GlobalUnlock(hDevMode);  
- 
- // Compute size of DEVNAMES structure from PRINTER_INFO_2's data
- DWORD drvNameLen = lstrlen(p2->pDriverName)+1;  // driver name
- DWORD ptrNameLen = lstrlen(p2->pPrinterName)+1; // printer name
- DWORD porNameLen = lstrlen(p2->pPortName)+1;    // port name
- 
- // Allocate a global handle big enough to hold DEVNAMES.
- HGLOBAL hDevNames = GlobalAlloc(GHND,  
- sizeof(DEVNAMES) +  
-         (drvNameLen + ptrNameLen + porNameLen)*sizeof(TCHAR));  
-     ASSERT(hDevNames);  
-     DEVNAMES* pDevNames = (DEVNAMES*)GlobalLock(hDevNames);  
-     ASSERT(pDevNames);  
- 
- // Copy the DEVNAMES information from PRINTER_INFO_2
- // tcOffset = TCHAR Offset into structure
- int tcOffset = sizeof(DEVNAMES)/sizeof(TCHAR);  
-     ASSERT(sizeof(DEVNAMES) == tcOffset*sizeof(TCHAR));  
- 
-     pDevNames->wDriverOffset = tcOffset;  
-     memcpy((LPTSTR)pDevNames + tcOffset, p2->pDriverName,  
-         drvNameLen*sizeof(TCHAR));  
-     tcOffset += drvNameLen;  
- 
-     pDevNames->wDeviceOffset = tcOffset;  
-     memcpy((LPTSTR)pDevNames + tcOffset, p2->pPrinterName,  
-         ptrNameLen*sizeof(TCHAR));  
-     tcOffset += ptrNameLen;  
- 
-     pDevNames->wOutputOffset = tcOffset;  
-     memcpy((LPTSTR)pDevNames + tcOffset, p2->pPortName,  
-         porNameLen*sizeof(TCHAR));  
-     pDevNames->wDefault = 0;  
- 
-     GlobalUnlock(hDevNames);  
-     GlobalFree(p2);   // free PRINTER_INFO_2
- 
- // set the new hDevMode and hDevNames
-     *phDevMode = hDevMode;  
-     *phDevNames = hDevNames;  
- return TRUE;  
- }  
基本上是完整代码了，如果有内存错误，请联系我

