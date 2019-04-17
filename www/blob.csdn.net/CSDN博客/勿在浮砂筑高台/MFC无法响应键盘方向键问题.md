# MFC无法响应键盘方向键问题 - 勿在浮砂筑高台 - CSDN博客





2014年12月02日 13:30:30[勿在浮砂筑高台](https://me.csdn.net/luoshixian099)阅读数：1607








在做MFC小游戏过程中，响应方向键一个方法。

方向键码值：

VK_UP


VK_DOWN

VK_LEFT

VK_RIGHT

在程序中重载了OnChar()函数，以及OnPretranslateMessage(),OnKeyDown()，调试发现，按方向键后，并不执行这些函数。暂时还不清楚什么原因，希望博友们可以知道告诉一下我。

这里想出了一个解决办法：注册热键的方式

1.   重载OnInitDialog()


**BOOL RegisterHotKey(   HWND ***[hWnd]()***,   int ***[id]()***,   UINT ***[fsModifiers]()***,   UINT ***[vk]()***);**
hwnd 对话框句柄



id  热键的标示号

fsmodifiers 组合键选择（这里设为NULL）

vk  键值




::OnInitDialog()


{

 ....

 ....

RegisterHotKey(m_hWnd,1,NULL,VK_UP);

RegisterHotKey(m_hWnd,2,NULL,VK_DOWN);

RegisterHotKey(m_hWnd,3,NULL,VK_LEFT);

RegisterHotKey(m_hWnd,4,NULL,VK_RIGHT);

 ....

}

2.重载OnHotKey()

::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)


{

  switch (nHotKetld)

 {

   case 1:break;

   case 2:break;

   ...

 }

....

}

这样就可以响应方向键了。






