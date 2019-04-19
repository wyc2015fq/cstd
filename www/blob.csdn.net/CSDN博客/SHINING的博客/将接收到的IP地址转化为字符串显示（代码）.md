# 将接收到的IP地址转化为字符串显示（代码） - SHINING的博客 - CSDN博客
2010年03月23日 17:24:00[snsn1984](https://me.csdn.net/snsn1984)阅读数：1908标签：[byte](https://so.csdn.net/so/search/s.do?q=byte&t=blog)
个人分类：[VC++/MFC																[C++](https://blog.csdn.net/snsn1984/article/category/488621)](https://blog.csdn.net/snsn1984/article/category/420804)
 BYTE nField0,nField1,nField2,nField3;
 m_ipaddd.GetAddress(nField0,nField1,nField2,nField3);
 //m_ipadd.SetAddress(nField0,nField1,nField2,nField3);
 CString   strData1, strData2, strData3, strData4;   
 strData1.Format(_T("%.3d"),  nField0 );
 strData2.Format(_T("%.3d"),  nField1 );
 strData3.Format(_T("%.3d"),  nField2 );
 strData4.Format(_T("%.3d"),  nField3);
 CString ipaddress=strData1+'.'+strData2+'.'+strData3+'.'+strData4;
 MessageBox(ipaddress);
