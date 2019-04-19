# 开源的DirectUI界面开发库DUILIB试用demo (Win32程序) - xqhrs232的专栏 - CSDN博客
2012年09月22日 20:43:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2331
原文地址::[http://blog.sina.com.cn/s/blog_3f3c8ca50100pvzr.html](http://blog.sina.com.cn/s/blog_3f3c8ca50100pvzr.html)
相关网帖
1.[duilib使用注意](http://blog.csdn.net/mail_cm/article/details/7565140)----[http://blog.csdn.net/mail_cm/article/details/7565140](http://blog.csdn.net/mail_cm/article/details/7565140)
![开源的DirectUI界面开发库DUILIB试用demo <wbr>(Win32程序)](http://mbmail-m8lite.googlecode.com/files/demo%20V3.png)
[Demo
 第三版源码VC2005工程（附全部.h,ansi/unicode lib,dll）](http://goo.gl/c0L7Q),下载地址：http://goo.gl/c0L7Q
**[](http://mbmail-m8lite.googlecode.com/files/DUILIB%20DEMO%20V2.rar)**开源项目地址：[http://code.google.com/p/duilib/](http://code.google.com/p/duilib/)
该项目基于一个老外viksoe的DirectUI原理分析论文及示例：[http://www.viksoe.dk/code/windowless1.htm](http://www.viksoe.dk/code/windowless1.htm)
SVN checkout了项目的源码进行了试用，由于例子中程序里的List控件的使用，大多是仿Windows标准的ListCtrl，即列表头+纯文本（图标）列表项的应用方式，而DirectUI的应用场景最多的案例就是，类似迅雷等下载软件的复杂结构的List——即列表项中有复杂的文本、进度条、按钮、下拉框等控件（而这也是现代软件包括手机软件例如iPhone,Android最多使用的一种控件布局方式），大概看了下DUILIB的控件源代码，在该界面库中，这种复杂List的实现方法应为
（XML配置方式）： 
- 
<List name="" text="" tooltip="" float="false" bordersize="1" bkcolor="#FFE0F0FE" bordercolor="#FF1B4876" itemselectedbkcolor="#FFC1E3FF" itemhotbkcolor="#FFE9F5FF" header="hidden" inset="3,3,3,3" bkimage="file='downlist_bk.png' ytiled='true'">
- 
<ListContainerElement height="45">
- 
<HorizontalLayout name="" text="" tooltip="" float="false" bkcolor="#006B93B2" childpadding="10" inset="3,5,3,5">
- 
<Button name="" text="" tooltip="" float="false" bordersize="0" width="32" maxheight="26" bkcolor2="#FF757676" bordercolor="#FF000000" bkimage="downlist_app.png" normalimage="file='downlist_ok.png' dest='20,14,32,26'" />
- 
<VerticalLayout>
- 
<Label text="Download_0098_44.exe" textcolor="#FFAAAAAA" showhtml="true"></Label>
- 
<Progress float="false" fgimage="file='progress_fore.png'" min="0" max="100" value="100" maxheight="7" maxwidth="200"/>
- 
</VerticalLayout>
- 
<VerticalLayout width="80">
- 
<Label align="right" text="2.04M " textcolor="#FFAAAAAA" showhtml="true"></Label>
- 
<HorizontalLayout>
- 
<Label text="<b>打开</b>" textcolor="#FFAAAAAA" showhtml="true"></Label>
- 
<Label text="<b>文件夹</b>" textcolor="#FFAAAAAA" showhtml="true"></Label>
- 
</HorizontalLayout>
- 
</VerticalLayout>
- 
</HorizontalLayout>
- 
</ListContainerElement>
- 
</List>
即，ListContainerElement的应用，该控件应该可以作为类似主界面布局容器Container一样使用，即，可以在List控件中实现任意在主界面中可以采用的复杂的列表项布局； 
而动态代码实现方式应为： 
- 
CListUI* pList = static_cast<CListUI*>(m_pm.FindControl(_T("listdemo")));      
- 
CListContainerElementUI *pElement = new CListContainerElementUI;            CTextUI *pTemp1 = new CTextUI;            pTemp1->SetAttribute(_T("text"),
 _T("This is a text"));               pElement->Add(pTemp1);    
- 
CComboUI *pTemp2 = new CComboUI;        
- 
CListLabelElementUI *pLabel = new CListLabelElementUI;        
- 
pLabel->SetAttribute(_T("text"), _T("Combo list1"));      
- 
pTemp2->Add(pLabel);           
- 
pLabel = new CListLabelElementUI;      
- 
pLabel->SetAttribute(_T("text"), _T("Combo list2"));      
- 
pTemp2->Add(pLabel);        pLabel = new CListLabelElementUI;  
- 
pLabel->SetAttribute(_T("text"), _T("Combo list3"));    
- 
pTemp2->Add(pLabel);      
- 
pElement->Add(pTemp2);  
- 
pElement->SetAttribute(_T("height"), _T("30"));
- 
- 
pList->Add(pElement);          
粗略地实现了一下，大体效果见附件截图，演示程序是就是SVN checkout出来的源码里的一个演示程序test app，只简单修改了一下界面xml文件的路径而已，就不再重复放了，exe在压缩包里面，暂时没做事件处理，主要还是演示xml的写法，仅供参考，希望大家多交流，能有更方便的实现方式![开源的DirectUI界面开发库DUILIB试用demo <wbr>(Win32程序)](http://www.duilib.com/images/post/smile/default/2.gif)
PS: 让不付钱连试用版都不给，卖上天价的商业DirectUI开发库去史吧，oyeah~~
