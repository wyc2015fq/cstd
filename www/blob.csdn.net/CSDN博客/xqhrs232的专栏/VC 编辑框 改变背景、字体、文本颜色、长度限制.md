# VC 编辑框 改变背景、字体、文本颜色、长度限制 - xqhrs232的专栏 - CSDN博客
2013年12月24日 09:20:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1069
原文地址::[http://blog.csdn.net/shen_001/article/details/5655956](http://blog.csdn.net/shen_001/article/details/5655956)
1。长度限制
OnInitDialog()中：
m_edit1.SetLimitText(8);                           //m_edit1为编辑框的成员变量
或者
CEdit*pEdt=(CEdit*)GetDlgItem(IDC_EDIT1);
pEdt->SetLimitText(8);                              //限制编辑框输入长度为8字节
说明：在使用多字节字符集且不使用XP风格时，SetLimitText设置的是最大字节数；在使用Unicode字符集或使用XP风格时，SetLimitText设置的是最大字符数。
2。汉字判断
方法一、
**[cpp]**[view
 plain](http://blog.csdn.net/shen_001/article/details/5655956#)[copy](http://blog.csdn.net/shen_001/article/details/5655956#)
- CString str="ab你c好。。";  
- for(int i=0;i<str.GetLength();i++)  
- {  
- 
- if(   (BYTE)str[i]   <   0x80 )  
- {    
-       MessageBox("非汉字");  
- }        
- 
- else//汉字  
- {    
-     MessageBox("是汉字");  
- }                                     
                                  //方法不好，只能判断有没有汉字
方法二、
**[cpp]**[view
 plain](http://blog.csdn.net/shen_001/article/details/5655956#)[copy](http://blog.csdn.net/shen_001/article/details/5655956#)
- CString   ss="dd你aa今bb真cc";  
- 
- int i=0;  
- while(i<ss.GetLength())  
- {  
- if(IsDBCSLeadByte(ss[i])) //   是DBCS
-     {        
-        i += 2;  
- 
-        AfxMessageBox("汉字");  
- 
-      }  
- else//   英文
- 
-     {       
- 
-       i ++;    
-       AfxMessageBox("English");  
- 
-      }  
-    }  
3。字体及大小
定义一全局变量或成员变量CFont   font;                                        //不要定义成局部变量，否则没效果
CEdit*pEdt=(CEdit*)GetDlgItem(IDC_EDIT1);
font.CreatePointFont(266,"Arial");
pEdt->SetFont(&font);
4。背景及文本颜色
定义一成员变量CBrush m_brush;
OnInitDialog()中进行初始化工作m_brush.CreateSolidBrush(RGB(255,0,0));
然后在OnCtrlColor中
**[c-sharp]**[view
 plain](http://blog.csdn.net/shen_001/article/details/5655956#)[copy](http://blog.csdn.net/shen_001/article/details/5655956#)
- HBRUSH C***Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)  
- 
- {  
- 
- HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);  
- 
- if(nCtlColor==CTLCOLOR_EDIT &&  
- 
-                            pWnd->GetDlgCtrlID()==IDC_EDIT1)//注意此处的（pWnd->），否则没效果
- 
- {  
- 
- pDC->SetTextColor(RGB(255,0,0));  
- 
- pDC->SetBkColor(RGB(255,255,0));//设置文本背景色
- 
- pDC->SetBkMode(TRANSPARENT);//设置背景透明
- 
- hbr = (HBRUSH)m_brush;  
- 
- }  
- 
- return hbr;  
- 
- }  
对于nCtlColor的类型，如下：
CTLCOLOR_BTN   Button control
CTLCOLOR_DLG   Dialog box
CTLCOLOR_EDIT   Edit control
CTLCOLOR_LISTBOX   List-box control
CTLCOLOR_MSGBOX   Message box
CTLCOLOR_SCROLLBAR   Scroll-bar control
CTLCOLOR_STATIC   Static control
派生Edit子类，实现字符限制输入！！！
void CEditChar::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{ 
 if ((nChar>='A'&&nChar<='Z')||(nChar>='a'&&nChar<='z'))
 {
  CEdit::OnChar(nChar, nRepCnt, nFlags);
 } 
}
