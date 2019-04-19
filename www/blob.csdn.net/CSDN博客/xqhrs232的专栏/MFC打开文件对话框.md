# MFC打开文件对话框 - xqhrs232的专栏 - CSDN博客
2013年06月08日 22:32:47[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：748
原文地址::[http://blog.chinaunix.net/uid-14827902-id-3058983.html](http://blog.chinaunix.net/uid-14827902-id-3058983.html)
相关网帖
1、[MFC打开/保存文件对话框:CFileDialog](http://www.cnblogs.com/jcss2008/archive/2008/10/10/1308301.html)----[http://www.cnblogs.com/jcss2008/archive/2008/10/10/1308301.html](http://www.cnblogs.com/jcss2008/archive/2008/10/10/1308301.html)
 CFileDialog类封装了Windows常用的文件对话框。常用的文件对话框提供了一种简单的与Windows标准相一致的文件打开和文件存盘对话框功能。
CFileDialog的语法：
CFileDialog(BOOL bOpenFileDialog,
LPCTSTR lpszDefExt=NULL,
LPCTSTR lpszFileName=NULL,
DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
LPCTSTR lpszFilter=NULL,
CWnd * pParentWnd=NULL); 
第一个参数 bOpenFileDialog为TRUE或FALSE。TRUE为打开文件；FALSE为保存文件。 
第二个参数 lpszDefExt为缺省的扩展名。 
第三个参数 lpszFileName为显示在文件名组合框的编辑框的文件名，一般可选NULL 
第四个参数 dwFlags为对话框风格，一般为OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 即隐藏只读选项和覆盖已有文件前提示。
OFN_ALLOWMULTISELECT:允许选择多个文件
OFN_CREATEPROMPT:如果输入的文件名不存在，则对话框返回询问用户是否根据次文件名创建文件的消息框
OFN_FILEMUSTEXIST:只能输入已存在的文件名
OFN_FORCESHOWHIDDEN:可以显示隐藏的文件
OFN_NOREADONLYRETURN:不返回只读文件
OFN_OVERWRITEPROMPT:保存的文件已存在时，显示文件已存在的信息
第五个参数 LpszFilter为下拉列表枢中显示文件类型。 
lpszFilter 是最重要的一个参数，它指明可供选择的文件类型和相应的扩展名。参数格式如：
　　"JPG Files (*.jpg)|*.jpg|All Files (*.*)|*.*||";文件类型说明和扩展名间用 | 分隔，同种类型文件的扩展名间可以用 ; 分割，每种文件类型间用 | 分隔，末尾用
 || 指明。
因为vs 2005之后项目使用的是unicode字符集，所以如果直接强制类型转换为LPCTSTR, 运行会发现文件类型处显示为乱码,可以在“项目-属性”中将"unicode字符集"换成"多字节字符集"就行了，也可以将文本内容放在_T()里面，不变字符集
第六个参数 pParentWnd为父窗口指针, 一般可选NULL。
下面给出示例代码:
- 
void CMyDialog::OnBnClickedOpennc()
- {
- // TODO: 在此添加控件通知处理程序代码
- 
    CString FilePathName;
- 
    CFileDialog dlg(TRUE,//TRUE为OPEN对话框，FALSE为SAVE
 AS对话框
- NULL,
- NULL,
- 
        OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
- (LPCTSTR)_TEXT("JPG
 Files (*.jpg)|*.jpg|All Files (*.*)|*.*||"),
- NULL);
- if(dlg.DoModal()==IDOK)
- {
- 
        FilePathName=dlg.GetPathName();//文件名保存在了FilePathName里
- }
-     else
-     {
-     return;
-     }
- }
