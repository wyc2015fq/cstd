# [ListControl]MFC中实现list控件的编辑操作 - TaigaComplex求职中 - 博客园







# [[ListControl]MFC中实现list控件的编辑操作](https://www.cnblogs.com/TaigaCon/archive/2012/06/29/2570422.html)





最近一直没睡好觉，彻底成了夜猫子一个，

最近用了两天时间做了个数据库课程设计，总体说是完成得还好，不过后面回想一下的话就是太简单了点。

这次做的是报刊订购系统，具体流程就不多说了，反正也就连接数据库，操纵各种数据，

得到的数据是显示在ListControl控件上的，由于我有个功能是想实现即时对List控件的编辑，所以上网找了一下方法，发现了这样的一篇文章：

[http://www.vckbase.com/index.php/wv/1548](http://www.vckbase.com/index.php/wv/1548)

我根据自己的需要做了一下改动

我自己的最终实现的方法就是，

生成一个新的子类继承于CListCtrl

```
1 class CEditListCtrl : public CListCtrl
```

下面是响应控件的点击事件OnLButtonDown

具体流程如下

![](https://pic002.cnblogs.com/images/2012/421096/2012062920434698.jpg)

代码如下

```
1 void CEditListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
 2 {
 3     // TODO: 在此添加消息处理程序代码和/或调用默认值
 4 
 5     m_bFocus = TRUE;
 6     LVHITTESTINFO  lvhit;
 7     lvhit.pt = point;
 8     int item = SubItemHitTest(&lvhit);
 9 
10     //if (over a item/subitem)
11     if (item != -1 && (lvhit.flags & LVHT_ONITEM))
12     {
13         CListCtrl::OnLButtonDown(nFlags, point);
14 
15         if(m_bHighLight && m_iItem == item )
16         {
17             //第二次单击
18             m_edtItemEdit.ShowWindow(SW_SHOW);
19             m_edtItemEdit.SetFocus();
20             //m_edtItemEdit.SetSel(-1);
21             return;
22         }
23         else
24         {
25             //第一次单击
26             if(m_editCreate)
27             {
28                 m_edtItemEdit.ShowWindow(SW_HIDE);
29                 SetListText();
30                 m_edtItemEdit.SetWindowTextW(NULL);
31             }
32             m_iItem =item;
33             m_iSubItem = lvhit.iSubItem;
34             m_bHighLight = TRUE;
35 
36 
37 
38         }
39     }
40     else
41     {
42         if(m_edtItemEdit.m_hWnd == NULL)
43         {
44             //未出现文本编辑框时
45             m_bHighLight = FALSE;
46         }
47 
48         CListCtrl::OnLButtonDown(nFlags, point);
49     }
50 
51     Invalidate(); //强制重绘控件
52 
53     if (m_iSubItem >= 0)
54     {
55         if(m_iItem != item) return;
56         CRect  rcSubItem;
57         m_iSubItem=2;
58         GetSubItemRect( item, m_iSubItem, LVIR_BOUNDS, rcSubItem);
59 
60         //get edit control and subclass
61 
62         if(!m_editCreate)
63 
64         {
65             if(m_edtItemEdit.Create(WS_CHILD|WS_VISIBLE|WS_BORDER,rcSubItem,this,IDC_EDIT))
66                 m_editCreate=TRUE;
67             m_edtItemEdit.SetFocus();
68             m_edtItemEdit.ShowWindow(SW_HIDE);
69         }
70         //ASSERT(hWnd != NULL);
71         //VERIFY(m_edtItemEdit.SubclassWindow(hWnd));
72 
73         //move edit control text 4 pixel to the right of org label,
74         //as Windows does it...编辑框定位
75         rcSubItem.left += 4;
76         m_edtItemEdit.MoveWindow (&rcSubItem) ;
77         m_edtItemEdit.SetWindowText(GetItemText(item, m_iSubItem));
78     }
79 
80 }
```

注：本人的代码是只能修改列号为2的单元格












