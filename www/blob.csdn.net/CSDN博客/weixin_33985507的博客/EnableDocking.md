# EnableDocking - weixin_33985507的博客 - CSDN博客
2014年10月07日 20:39:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4

CFrameWnd::**EnableDocking**
void **EnableDocking**(DWORD dwDockStyle);
參数： dwDockStyle 指定框架窗体的哪一边可作为控件条的停靠点，可为下面之一或几个： 
· CBRS_ALIGN_TOP 同意停靠到用户区的顶部。 
· CBRS_ALIGN_BOTTOM 同意停靠到用户区底部。 
· CBRS_ALIGN_LEFT 同意停靠到用户区左边。 
· CBRS_ALIGN_RIGHT 同意停靠到用户区右边。 
· CBRS_ALIGN ANY 同意停靠到用户区随意一边。 
说明：
调用此成员函数使框架窗体中的可停靠控件条可用。缺省时，控件条会下面面顺序停靠于框架窗体中：顶部、底部、左部、右部。
请參阅：
CControlBar::**EnableDocking**, CFrameWnd::DockControlBar, CFrameWnd::FloatControlBar
[](http://baike.360.cn/wiki/item/EnableDocking#top)CControlBar类成员 
[ [编辑本段](http://baike.360.cn/wiki/index.php?c=entry&a=editparam&wid=4259804&param=e978b81f) ] 
CControlBar::**EnableDocking**
void **EnableDocking**( DWORD dwStyle );
參数： dwStyle 指定是否同意控件条处于停靠状态和它在父窗体中停靠的位置。停靠位置參数例如以下： · CBRS_ALIGN_TOP 同意停靠在客户区的上方。 
· CBRS_ALIGN_BOTTOM 同意停靠在客户区的下方。 
· CBRS_ALIGN_LEFT 同意停靠在客户区的左边。 
· CBRS_ALIGN_RIGHT 同意停靠在客户区的右边。 
· CBRS_ALIGN_ANY 同意停靠在客户区的随意一边 
· CBRS_FLOAT_MULTI 同意在一个小的框架窗体中浮动有多个控件条。 
为0时（即没有指明标志）不同意停靠控件条。
说明：
本函数设置是否同意控件条处于停靠状态。能够给控件条指定停靠位置。
请參阅：
CFrameWnd::**EnableDocking**, CFrameWnd::DockControlBar, CFrameWnd::FloatControlBar, CControlBar::SetBarStyle
