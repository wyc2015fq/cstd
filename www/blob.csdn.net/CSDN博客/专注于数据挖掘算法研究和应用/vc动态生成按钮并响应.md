# vc动态生成按钮并响应 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年06月30日 14:19:00[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3871








void CBtnDlg::OnOK() 
{
 // TODO: Add extra validation here
 // 动态构建按钮代码
 CButton* pNewButton = new CButton();// 也可以定义为类的成员变量。
 CRect rcButton( 10, 10, 50, 30); // 按钮在对话框中的位置。
 pNewButton->Create("按钮", 0, rcButton, this, 10098);
 pNewButton->ShowWindow(SW_SHOW);

 //CDialog::OnOK();
}

LRESULT CBtnDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
 // TODO: Add your specialized code here and/or call the base class
 if ( WM_COMMAND== message )
 {
  WORD wID = LOWORD(wParam);
  if ( 10098 == wID)
  {
   CDC* pDC = GetDC();
   pDC->TextOut( 100, 100, "文字");
   ReleaseDC(pDC);
  }
    }
 return CDialog::DefWindowProc(message, wParam, lParam);
}







