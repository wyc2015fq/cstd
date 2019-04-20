# 限制EDITTEXT可输入的字符个数 - sandro_zhang的专栏 - CSDN博客
2011年09月13日 13:11:13[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：1195标签：[authentication																[callback																[dialog																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=authentication&t=blog)
个人分类：[vc++](https://blog.csdn.net/sandro_zhang/article/category/884062)
打开窗体初始句柄
```cpp
LPCDLGTEMPLATE hDlg;   // handle to loaded resource
		hDlg = notify_config::GetResourceDialog( IDD_AUTHENTICATION_DIALOG );
		DialogBoxIndirectParam( NULL, 
								hDlg, 
								NULL,// GetActiveWindow(), 
								AuthenticationDlgProc,
								(LPARAM)&authPrintJob);
```
在回调函数 AuthenticationDlgProc 中message 的  case 为 WM_INITDIALOG 时进行设置，用到函数 SendMessage 来对EDITTEXT进行 EM_SETLIMITTEXT 设置
具体原型如下：
```cpp
INT_PTR CALLBACK AuthenticationDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
	{
	case WM_INITDIALOG:
        {
         //Limit edit text lengths
         HWND usredit = GetDlgItem(hDlg, IDC_USERNAME);
         SendMessage( usredit, EM_SETLIMITTEXT, 256, 0);
        }
    }
}
```
