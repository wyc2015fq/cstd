# VC中Tab control的用法 - L_Andy的专栏 - CSDN博客

2013年06月28日 15:07:29[卡哥](https://me.csdn.net/L_Andy)阅读数：6078
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



                
 1. 新建一个MFC工程, 取名MyTab, 选择Dialog based, 然后Finish.

2. 删除对话框上默认添加的三个控件. 添加Tab Control控件并在Property属性中设置ID为IDC_TABTEST 在More Styles里勾上Bottom. 调速尺寸使其布满整个对话框, 我这边Tab Control的尺寸最后为164X203. 在ClassWizard为其添加变量, 变量名为m_tab. 类型为CTabCtrl.

3. 在对话框的初始化函数OnInitDialog里面添加如下代码:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_tab.InsertItem(0,"参数一");  //添加参数一选项卡
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_tab.InsertItem(1,"参数二");  //添加参数二选项卡
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_tab.InsertItem(2,"结果");    //添加结果选项卡
4.在对话框资源里面添加三个对话框资源, ID分别命名为IDD_PARA1, IDD_PARA2, IDD_RESULT. 字体为宋体, 字号为9, style为Child, Border为None, 宽度调整为161. 再分别为其添加对应的基于CDialog类CPara1, CPara2, CResult.

5. 在CMyTabDlg类中添加三个成员变量m_para1, m_para2, m_result, 分别是三个子对话框的实例. 代码如下:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CResult m_result;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CPara2 m_para2;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CPara1 m_para1;

6. 在IDD_PARA1对话框上添加静态文本控件内容为"参数一" 再在后面插入一个文本框控件, 用ClassWizard将其关联为一个int型变量,名为m_nPara1;

在IDD_PARA2对话框上添加静态文本控件内容为"参数二" 再在后面插入一个文本框控件, 用ClassWizard将其关联为一个int型变量,名为m_nPara2;

在IDD_RESULT对话框上添加静态文本控件内容为"结果" 再在后面插入一个文本框控件, 用ClassWizard将其关联为一个int型变量,名为m_nResult;

7. 为CPara1类添加成员函数int GetParaValue() 代码如下:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int CPara1::GetParaValue()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return m_nPara1;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}

为CPara2类添加成员函数int GetParaValue() 代码如下:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int CPara2::GetParaValue()
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)return m_nPara2;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}

为CResult类添加成员函数void SetResultValue(int nResult) 代码如下:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)void CResult::SetResultValue(int
 nResult)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)     m_nResult= nResult;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)}

8. 在IDD_MYTAB_DIALOG对话框的初始化函数OnInitDialog里面添加如下代码:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//关联对话框,并且将IDC_TABTEST控件设为父窗口
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_para1.Create(IDD_PARA1,GetDlgItem(IDC_TABTEST));
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_para2.Create(IDD_PARA2,GetDlgItem(IDC_TABTEST));
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_result.Create(IDD_RESULT,GetDlgItem(IDC_TABTEST));
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//获得IDC_TABTEST客户区大小
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CRect rs;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_tab.GetClientRect(&rs);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//调整子对话框在父窗口中的位置
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rs.top+=1;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rs.bottom-=60;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rs.left+=1;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rs.right-=2;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//设置子对话框尺寸并移动到指定位置
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_para1.MoveWindow(&rs);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_para2.MoveWindow(&rs);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_result.MoveWindow(&rs);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//分别设置隐藏和显示
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_para1.ShowWindow(true);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_para2.ShowWindow(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_result.ShowWindow(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//设置默认的选项卡
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_tab.SetCurSel(0);
9. 添加Tab Control控件的TCN_SELCHANGE事件响应函数OnSelchangeTabtest(NMHDR* pNMHDR, LRESULT* pResult) ,函数体代码如下:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)int CurSel= m_tab.GetCurSel();
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)switch(CurSel)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockStart.gif)![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ContractedBlock.gif)...{
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)case0:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_para1.ShowWindow(true);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_para2.ShowWindow(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_result.ShowWindow(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)break;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)case1:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_para1.ShowWindow(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_para2.ShowWindow(true);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_result.ShowWindow(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)break;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)case2:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_para1.ShowWindow(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_para2.ShowWindow(false);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         m_result.ShowWindow(true);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)break;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)default:
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/InBlock.gif)         ;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/ExpandedBlockEnd.gif)     }
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)*pResult=0;
10. 在IDD_MYTAB_DIALOG对话框下面添加一个按钮, 标题为"计算" 为其添加事件响应函数, 代码如下:

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_para1.UpdateData(true);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)     m_para2.UpdateData(true);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)     m_result.SetResultValue(m_para1.GetParaValue()+m_para2.GetParaValue());
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)     m_result.UpdateData(false); 

11.这个小程序很简单,但是他说明了Tab Control控件的基本用法.

注意：

1：//关联对话框,并且将IDC_TABTEST控件设为父窗口（方法2）

m_para1.SetParent(GetDlgItem(IDC_TABTEST));

m_para2.SetParent(GetDlgItem(IDC_TABTEST));

m_result.SetParent(GetDlgItem(IDC_TABTEST));

2：

![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//获得IDC_TABTEST客户区大小
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)CRect rs;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)m_tab.GetClientRect(&rs);
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)//调整子对话框在父窗口中的位置
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rs.top+=1;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rs.bottom-=60;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rs.left+=1;
![](http://images.csdn.net/syntaxhighlighting/OutliningIndicators/None.gif)rs.right-=2;

这里一点要注意：调整距离，否则，子对话框将覆盖Tab control控件，将不能显示
将出现这中情况：

![](https://img-blog.csdn.net/20130628150442656?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 //调整子对话框在父窗口中的位置 如下

 rs.top+=20; 

 rs.bottom-=20; 

 rs.left+=20; 

 rs.right-=20;
![](https://img-blog.csdn.net/20130628150700734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

哈哈，注意啊。

