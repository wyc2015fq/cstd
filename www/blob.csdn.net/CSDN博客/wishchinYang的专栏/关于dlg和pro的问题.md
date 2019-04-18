# 关于dlg和pro的问题 - wishchinYang的专栏 - CSDN博客
2013年11月10日 21:42:31[wishchin](https://me.csdn.net/wishchin)阅读数：753
个人分类：[MFC编程](https://blog.csdn.net/wishchin/article/category/1508409)
# 微软链接：[http://technet.microsoft.com/zh-cn/subscriptions/bb983387.aspx](http://technet.microsoft.com/zh-cn/subscriptions/bb983387.aspx)
# CDialogEx::CDialogEx
构造 **CDialogEx** 对象。 

CDialogEx(
   UINT nIDTemplate,
   CWnd* pParent=NULL 
);
CDialogEx(
   LPCTSTR lpszTemplateName,
   CWnd* pParentWnd=NULL 
);
![](http://i.technet.microsoft.com/Areas/Global/Content/clear.gif)参数
- [in] nIDTemplate
对话框模板的资源ID。
- [in] lpszTemplateName
对话框模板资源名称。
- [in] pParent
父窗口的指针。
默认值为 NULL。
- [in] pParentWnd
父窗口的指针。
默认值为 NULL。
![](http://i.technet.microsoft.com/Areas/Global/Content/clear.gif)要求
标头: afxdialogex.h
若使用
CDialogEx
，可能需要包含：afxdialogex.h
