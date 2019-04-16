# 设置ListCtrl列表控件其中某一行的字体和背景颜色 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月11日 16:50:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：33
个人分类：[纯编程](https://blog.csdn.net/leixiaohua1020/article/category/1647545)










设置ListCtrl列表控件其中某一行的字体和背景颜色，可以最终达到如下效果：

![](https://img-blog.csdn.net/20131011163815500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

操作步骤如下所示：

1.先添加一个自定义消息

```cpp
ON_NOTIFY ( NM_CUSTOMDRAW,IDC_V_H264_NALLIST, OnCustomdrawMyList )
```





注：在BEGIN_MESSAGE_MAP()和END_MESSAGE_MAP()之间。第二个参数是LIstCtrl的ID，第三个参数是消息响应函数。

2.下面是具体的函数。根据表格内容的不同，设置不同的背景颜色。

比如：“NAL负载类型”为SLICE的时候，背景为青色；为SPS的时候，背景为黄色；为PPS的时候，背景为咖啡色。



```cpp
//ListCtrl加颜色
void CSpecialVH264Dlg::OnCustomdrawMyList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	//This code based on Michael Dunn's excellent article on
	//list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp


	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );


	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;


	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.


		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{


		COLORREF clrNewTextColor, clrNewBkColor;


		int    nItem = static_cast<int>( pLVCD->nmcd.dwItemSpec );


		CString strTemp = m_vh264nallist.GetItemText(nItem,2);
		if(strcmp(strTemp,"SLICE")==0){
			clrNewTextColor = RGB(0,0,0);		//Set the text 
			clrNewBkColor = RGB(0,255,255);		//青色
		}
		else if(strcmp(strTemp,"SPS")==0){
			clrNewTextColor = RGB(0,0,0);		//text 
			clrNewBkColor = RGB(255,255,0);		//黄色
		}
		else if(strcmp(strTemp,"PPS")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,153,0);		//咖啡色
		}else if(strcmp(strTemp,"SEI")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,66,255);			//粉红色
		}else if(strcmp(strTemp,"IDR_SLICE")==0){
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,0,0);			//红色
		}else{
			clrNewTextColor = RGB(0,0,0);		//text
			clrNewBkColor = RGB(255,255,255);			//白色
		}


		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;




		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;




	}
}
```










