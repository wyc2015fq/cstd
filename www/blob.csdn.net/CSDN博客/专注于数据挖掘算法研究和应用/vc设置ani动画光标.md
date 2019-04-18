# vc设置ani动画光标 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年07月02日 19:23:04[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3058








1.制作自己的ani动态鼠标，可到[http://shu.skycx.com/index.html](http://shu.skycx.com/index.html)下载；



2.以dialog为例：

1）在.h头文件类中增加成员变量：HCURSOR   m_Cursor; 

2）在.cpp文件类构造函数中赋空值：m_Cursor =  NULL; 

3）OnInitDialog()增加如下代码，加载ani文件；

 if(m_Cursor   ==   NULL)  m_Cursor=   LoadCursorFromFile("res\\cur02.ani"); //存放ani文件具体路径

4）dialog截取 WM_SETCURSOR消息，实现OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 函数：

BOOL C**Dlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
 // TODO: Add your message handler code here and/or call default
 if(m_Cursor!=NULL) SetCursor(m_Cursor); 
 return   TRUE; 
 //return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

5）可通过具体控件截取WM_SETCURSOR消息，从而在需要改变鼠标的地方调用SetCursor()函数。



