# VC 判断选中的 单选框 复选框的 - L_Andy的专栏 - CSDN博客

2013年12月23日 18:35:07[卡哥](https://me.csdn.net/L_Andy)阅读数：3336


**一 vc对话框中的组框控件起什么作用?**

1）它可将多个单选按钮圈起来，再将你想设为默认按钮的那个按钮的Group属性选中，其它按钮的Group属性取消。

这样这些RADIO按钮就成了一组可以实现在他们之间进行单选,这4个按钮只有group属性的可以映射一个变量,其类型为int,这个变量可以用来返回用户选择了哪个选项,0表示选择了第一个,1表示选择了第二个,

....依此类推

这样以后就可以分别对不同的RADIO按钮进行消息映射了

2）基本上就是为了画面效果，将相关的一些控件圈起来，起个抬头。

在对话框里拉一个GroupBox，设定抬头内容，把需要的控件圈起来即可。

3）被分到一组里的单选框是互斥的，不在一个组里的单选框是互不影响的（组框里的单选框只能一个被选中）

**二 判断被选中的单选框**

VC中建立基于对话框的类，在对话框控件中添加组框，在组框中放三个单选框，ID分别为IDC_RADIO1，IDC_RADIO2，IDC_RADIO3.可以在对话框控件中添加一个Button控件，在Button控件单击命令响应中添加如下命令（代码仅起示例作用）

UINT nSex=GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO3);//参数分别为第一个、最后一个单选框的ID号

 switch(nSex)

{

 case IDC_RADIO1:

  ::AfxMessageBox("选中第一个");break;

         case IDC_RADIO2:

  ::AfxMessageBox("选中第2个");break;

  case IDC_RADIO3:

  ::AfxMessageBox("选中第3个");break;


}

**三 判断被选中的复选框**

VC中建立基于对话框的类，在对话框控件中添加组框（可以不添加），在组框中放三个复选框，ID分别为IDC_CHECK1，IDC_IDC_CHECK12，IDC_IDC_CHECK3.可以在对话框控件中添加一个Button控件，在Button控件单击命令响应中添加如下命令（代码仅起示例作用）

if((IsDlgButtonChecked(IDC_CHECK1))&&(IsDlgButtonChecked(IDC_CHECK2)))

 ::AfxMessageBox("选中第1 第2个复选框");

 if((IsDlgButtonChecked(IDC_CHECK1))&&(IsDlgButtonChecked(IDC_CHECK3)))

 ::AfxMessageBox("选中第1 第3个复选框");

 if((IsDlgButtonChecked(IDC_CHECK3))&&(IsDlgButtonChecked(IDC_CHECK2)))

 ::AfxMessageBox("选中第2 第3个复选框");

四 [**CheckDlgButton()**](http://blog.csdn.net/xcq1109/article/details/3705800)

函数功能：该函数改变按钮控制的选中状态。

　　函数原型：BOOL CheckDlgButton（HWNDhDlg，int nlDButton,UINT uCheck）；

　　参数：

　　hDlg：指向含有该按钮的对话框的句柄。

　　nlDButton：标识要修改的按钮。

　　uCheck：给定该按钮的选中状态。该参数可取下列值，这些值的含义如下：

　　BST_CHECKED：设置按钮状态为己选中（checked）。

　　BST_INDETERMINATE：设置按钮状态变灰，表示不确定状态。只有在该按钮具有BS_3STATE或BS_AUTO3STATE样式时才能使用该值。

　　BST_UNCHECKED：设置按钮为未选中状态（unchecked）。

　　返回值：如果函数执行成功，返回值非零；如果函数失败，则返回值为零。若想获取更多错误信息，请调用 GetLastError函数。

示例程序

CheckDlgButton(IDC_L_SHOW_TITLE,g_SystemPara.m_bShowTitle);
CheckDlgButton(IDC_L_SHOW_BACKGROUND,g_SystemPara.m_bShowBackground);
CheckDlgButton(IDC_L_SHOW_NAME,g_SystemPara.m_bShowName);
CheckDlgButton(IDC_L_SHOW_ID,g_SystemPara.m_bShowID);
CheckDlgButton(IDC_L_SHOW_GROUPID,g_SystemPara.m_bShowGroupID);
CheckDlgButton(IDC_L_SHOW_LON, g_SystemPara.m_bShowLon);
CheckDlgButton(IDC_L_SHOW_LAT, g_SystemPara.m_bShowLat);
CheckDlgButton(IDC_L_SHOW_HEIGHT, g_SystemPara.m_bShowHeight);
CheckDlgButton(IDC_ALWAYS_SHOW,g_SystemPara.m_bAlwaysShow);

注释：示例程序中变量为BOOL类型，而vc中，定义如下，注意使用技巧：

#define BST_UNCHECKED      0x0000

#define BST_CHECKED        0x0001

#define BST_INDETERMINATE  0x0002

