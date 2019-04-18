# VC ON_CONTROL_RANGE多个控件响应一个方法 - L_Andy的专栏 - CSDN博客

2014年07月03日 18:24:28[卡哥](https://me.csdn.net/L_Andy)阅读数：4063


                
步骤/方法

　　分三个步骤

在头文件中声明函数例如

　　afx_msg void onNum(UINT uID)

在.cpp文件中添加函数体

　　void CCalculatorDlg::OnNum(UINT uID)

　　{

　　UINT index=uID-IDC_NUM_0;

　　CString num;

　　num.Format(_T("%d"),index);

　　AfxMessageBox(num);

　　}

4

添加消息映射

　　ON_CONTROL_RANGE(BN_CLICKED,IDC_NUM_0,IDC_NUM_9,OnNum)

　　第一个参数 事件

　　第二个参数 控件的第一个id

　　第三个参数 空间的最后一个id

　　第四个参数 响应事件的方法
            

