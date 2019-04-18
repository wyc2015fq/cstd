# MFC中创建透明窗口 - L_Andy的专栏 - CSDN博客

2018年02月28日 17:46:19[卡哥](https://me.csdn.net/L_Andy)阅读数：267


第一种  全透明  

1.设置窗口透明

```
//Set WS_EX_LAYERED on this window
  ::SetWindowLong(GetSafeHwnd(),
        GWL_EXSTYLE,
       ::GetWindowLongPtr(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);

  //Make this window 70% alpha transparent
  this->SetLayeredWindowAttributes(0,(255*70)/100,LWA_ALPHA);
```

2.去除窗口透明

```
//Remove WS_EX_LAYERED from window style
   ::SetWindowLong(GetSafeHwnd(),
      GWL_EXSTYLE,
    ::GetWindowLongPtr(GetSafeHwnd(),GWL_EXSTYLE)&~WS_EX_LAYERED);
  //Repaint
  this->RedrawWindow();
```

3.实际运行效果 
![这里写图片描述](https://img-blog.csdn.net/20180228174435342?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180228174451322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

第二种    让指定的部分为透明色彩

OnInitialDialog中

```
COLORREF maskColor = RGB(255,255,255);

 SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
 HINSTANCE hInst=LoadLibrary(_T("User32.DLL"));
 if(hInst)
 {
 typedef BOOL (WINAPI * MYFUNC)(HWND , COLORREF,BYTE,DWORD);
 MYFUNC fun=NULL;
 fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes");
 if(fun)fun(this->GetSafeHwnd(),maskColor,255,1);
 FreeLibrary(hInst);
 }
```

在Opaint函数的  else函数中

```
else
 {
 CRect rect;
 CPaintDC dc(this);
 GetClientRect(rect);
 dc.FillSolidRect(rect,RGB(255,255,255));
 //dc.FillSolidRect(rect,RGB(108,108,108));

 CBitmap bmp1,bmp2;
 //bmp1.LoadBitmap(IDB_B_1);
 bmp1.LoadBitmap(IDB_BITMAP6);
 bmp2.LoadBitmap(IDB_BITMAP7);

 //bmp2.LoadBitmap(IDB_B_2);
 //CPaintDC dc(this); // device context for painting
 CDC memDC;
 memDC.CreateCompatibleDC(&dc);
 CBitmap* pbmpOld;

 pbmpOld = memDC.SelectObject(&bmp1);
 dc.BitBlt(0, 0, 2000, 1500, &memDC, 0, 0, SRCPAINT);  // 注意最后一个参数

 memDC.SelectObject(&bmp2);
 dc.BitBlt(0, 0, 2000, 1500, &memDC, 0, 0, SRCAND);  // 注意最后一个参数

 memDC.SelectObject(pbmpOld);
 memDC.DeleteDC();

 CDialog::OnPaint();
 }
```

