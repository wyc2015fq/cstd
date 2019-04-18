# VC控件自动排列位置显示 - 深之JohnChen的专栏 - CSDN博客

2017年05月10日 15:26:25[byxdaz](https://me.csdn.net/byxdaz)阅读数：611


为了实现将一些控件放到某一个区域显示，并且对这些控件动态排序，这里提供一个函数。

```cpp
/*
	功能说明：更新控件位置
	参数说明：listControl：控件列表；nPanelLeft：panel控件左位置；nPanelTop：panel控件上位置；nPanelRight：panel控件右位置；nPanelBottom：panel控件底位置；
			  nHeightSpan：控件行间隔；	nWidthSpan：控件列间隔；nPanelStartLeftSpan：首个开始控件偏移位置左坐标；nPanelStartTopSpan：首个开始控件偏移位置右坐标；
			  nAlignType：对齐方法（0：左对齐，1：右对齐，2：中间对齐）
	返回值：控件被占用RECT区域
	*/
	RECT UpdatePanelControlPosition(list<CWnd *> listControl,int nPanelLeft,int nPanelTop,int nPanelRight,int nPanelBottom,int nHeightSpan = 5,int nWidthSpan = 5,int nPanelStartLeftSpan = 10,int nPanelStartTopSpan = 18,int nAlignType=0);
```


```cpp
//更新控件位置位置
RECT UpdatePanelControlPosition(list<CWnd *> listControl,int nPanelLeft,int nPanelTop,int nPanelRight,int nPanelBottom,int nHeightSpan,int nWidthSpan,int nPanelStartLeftSpan,int nPanelStartTopSpan,int nAlignType)
{
	RECT rectAll;
	CWnd *pWndObject = NULL;
	RECT rectControl;
	RECT rectPrev,rectNew;
	int nControlWidth = 0;
	int nControlHeight = 0;
	list<CWnd *>::iterator iter = listControl.begin(),iterEnd = listControl.end();
	int nn = 0;
	bool bNewRowShow = true;
	int nTotalLeft = 0;
	int nTotalTop = 0;
	int nLineMaxBottom = 0;
	if(nAlignType == 2)
	{
		//中间对齐
		int nAllControlNumbers = 0;
		list<CWnd *> lastLineListControl;		//最后一行控件列表
		list<RECT>   lastLineListRect;			//最后一行控件位置
		int nLastLineWidth = 0,nLastLineLeft = 0,nLastLineTop = 0;
		nTotalLeft = nPanelLeft + nPanelStartLeftSpan;
		nTotalTop = nPanelTop + nPanelStartTopSpan;
		nLineMaxBottom = nTotalTop + nHeightSpan;
		rectAll.left = 65535;
		rectAll.top = 65535;
		rectAll.right = 0;
		rectAll.bottom = 0;
		//对所有控件调整位置
		for(iter; iter!=iterEnd; iter++)
		{
			pWndObject = *iter;
			pWndObject->GetClientRect(&rectControl);
			ClientToScreen(&rectControl);
			nControlWidth = rectControl.right - rectControl.left;
			nControlHeight = rectControl.bottom - rectControl.top;
			//判断是否需要换行显示
			if(bNewRowShow)
			{
				//新行
				nAllControlNumbers++;
				lastLineListControl.clear();
				lastLineListRect.clear();
				nLastLineLeft = nPanelLeft + nWidthSpan;
				nLastLineTop = nPanelTop + nHeightSpan;
				nLastLineWidth = nControlWidth;

				rectNew.left = nTotalLeft + nWidthSpan;
				rectNew.right = rectNew.left + nControlWidth;
				rectNew.top = nTotalTop + nHeightSpan;
				rectNew.bottom = rectNew.top + nControlHeight;
				//插入控件位置
				lastLineListControl.push_back(pWndObject);
				lastLineListRect.push_back(rectNew);
				bNewRowShow = false;
			}
			else
			{
				//同一行
				if((rectPrev.right + nControlWidth) >= nPanelRight)
				{
					bNewRowShow = true;
				}
				if(bNewRowShow)
				{
					//新行
					nAllControlNumbers++;
					lastLineListControl.clear();
					lastLineListRect.clear();
					nLastLineLeft = nPanelLeft + nWidthSpan;
					nLastLineTop = nLineMaxBottom + nHeightSpan;
					nLastLineWidth = nControlWidth;

					rectNew.left = nTotalLeft + nWidthSpan;
					rectNew.right = rectNew.left + nControlWidth;
					rectNew.top = nLineMaxBottom + nHeightSpan;
					rectNew.bottom = rectNew.top + nControlHeight;
					//插入控件位置
					lastLineListControl.push_back(pWndObject);
					lastLineListRect.push_back(rectNew);
					bNewRowShow = false;
				}
				else
				{
					rectNew.left = rectPrev.right + nWidthSpan;
					rectNew.right = rectNew.left + nControlWidth;
					rectNew.top = rectPrev.top;
					rectNew.bottom = rectNew.top + nControlHeight;
					//插入控件位置
					lastLineListControl.push_back(pWndObject);
					lastLineListRect.push_back(rectNew);
					nLastLineWidth += nWidthSpan;
					nLastLineWidth += nControlWidth;
				}
			}

			rectPrev = rectNew;
			//获取一行控件最大底位置
			if(nLineMaxBottom < rectNew.bottom)
			{
				nLineMaxBottom = rectNew.bottom;
			}

			if(rectNew.left < rectAll.left)
			{
				rectAll.left = rectNew.left;
			}
			if(rectNew.top < rectAll.top)
			{
				rectAll.top = rectNew.top;
			}
			if(rectNew.right > rectAll.right)
			{
				rectAll.right = rectNew.right;
			}
			if(rectNew.bottom > rectAll.bottom)
			{
				rectAll.bottom = rectNew.bottom;
			}
			ScreenToClient(&rectNew);
			::SetWindowPos(pWndObject->GetSafeHwnd(),NULL,rectNew.left,rectNew.top,rectNew.right-rectNew.left,rectNew.bottom-rectNew.top,SWP_NOSIZE|SWP_SHOWWINDOW);
			nn++;
		}
		
		//对最后一行重新调整位置
		if(lastLineListControl.size() > 0)
		{
			int nDiffWidth = (nPanelRight - nPanelLeft)/2 - nLastLineWidth/2;
			nn = 0;
			list<RECT>::iterator iterRect;
			iterRect = lastLineListRect.begin();
			iter = lastLineListControl.begin();
			iterEnd = lastLineListControl.end();
			for(iter; iter!=iterEnd; iter++,iterRect++)
			{
				pWndObject = *iter;
				rectNew = *iterRect;
				//调整左右位置
				rectNew.left +=nDiffWidth;
				rectNew.right +=nDiffWidth;

				if(rectNew.left < rectAll.left)
				{
					rectAll.left = rectNew.left;
				}
				if(rectNew.top < rectAll.top)
				{
					rectAll.top = rectNew.top;
				}
				if(rectNew.right > rectAll.right)
				{
					rectAll.right = rectNew.right;
				}
				if(rectNew.bottom > rectAll.bottom)
				{
					rectAll.bottom = rectNew.bottom;
				}
				ScreenToClient(&rectNew);
				::SetWindowPos(pWndObject->GetSafeHwnd(),NULL,rectNew.left,rectNew.top,rectNew.right-rectNew.left,rectNew.bottom-rectNew.top,SWP_NOSIZE|SWP_SHOWWINDOW);
				nn++;
			}
		}
	}
	else if(nAlignType == 0)
	{
		//左对齐
		nTotalLeft = nPanelLeft + nPanelStartLeftSpan;
		nTotalTop = nPanelTop + nPanelStartTopSpan;
		rectAll.left = 66535;
		rectAll.top = 65535;
		rectAll.right = 0;
		rectAll.bottom = 0;
		nLineMaxBottom = nTotalTop + nHeightSpan;
		for(iter; iter!=iterEnd; iter++)
		{
			pWndObject = *iter;
			pWndObject->GetClientRect(&rectControl);
			ClientToScreen(&rectControl);
			nControlWidth = rectControl.right - rectControl.left;
			nControlHeight = rectControl.bottom - rectControl.top;
			//判断是否需要换行显示
			if(bNewRowShow)
			{
				//新行
				rectNew.left = nTotalLeft + nWidthSpan;
				rectNew.right = rectNew.left + nControlWidth;
				rectNew.top = nTotalTop + nHeightSpan;
				rectNew.bottom = rectNew.top + nControlHeight;
				bNewRowShow = false;
			}
			else
			{
				//同一行
				if((rectPrev.right + nControlWidth) >= nPanelRight)
				{
					bNewRowShow = true;
				}
				if(bNewRowShow)
				{
					//新行
					rectNew.left = nTotalLeft + nWidthSpan;
					rectNew.right = rectNew.left + nControlWidth;
					rectNew.top = nLineMaxBottom + nHeightSpan;
					rectNew.bottom = rectNew.top + nControlHeight;
					bNewRowShow = false;
				}
				else
				{
					rectNew.left = rectPrev.right + nWidthSpan;
					rectNew.right = rectNew.left + nControlWidth;
					rectNew.top = rectPrev.top;
					rectNew.bottom = rectNew.top + nControlHeight;
				}
			}

			rectPrev = rectNew;
			//获取一行控件最大底位置
			if(nLineMaxBottom < rectNew.bottom)
			{
				nLineMaxBottom = rectNew.bottom;
			}

			if(rectNew.left < rectAll.left)
			{
				rectAll.left = rectNew.left;
			}
			if(rectNew.top < rectAll.top)
			{
				rectAll.top = rectNew.top;
			}
			if(rectNew.right > rectAll.right)
			{
				rectAll.right = rectNew.right;
			}
			if(rectNew.bottom > rectAll.bottom)
			{
				rectAll.bottom = rectNew.bottom;
			}
			ScreenToClient(&rectNew);
			::SetWindowPos(pWndObject->GetSafeHwnd(),NULL,rectNew.left,rectNew.top,rectNew.right-rectNew.left,rectNew.bottom-rectNew.top,SWP_NOSIZE|SWP_SHOWWINDOW);
			//::MoveWindow(pWndObject->GetSafeHwnd(),rectNew.left,rectNew.top,rectNew.right-rectNew.left,rectNew.bottom-rectNew.top,TRUE);
			nn++;
		}
	}
	else
	{
		//右对齐，暂未实现
		nTotalLeft = nPanelLeft + nPanelStartLeftSpan;
		nTotalTop = nPanelTop + nPanelStartTopSpan;
	}

	return rectAll;
}
```

```cpp

```

调用代码：

RECT rectDevicePannelInfo;//设备pannel信息区域
GetDlgItem(IDC_STATIC_DEVICE_INFO)->GetWindowRect(&rectDevicePannelInfo);
list<CWnd *> listAllContorl;
CStatic *pStaMonitorName = &staMonitorName;//监控名称CStatic控件
CEdit *pEditMonitorName = &editMonitorName;//监控名称CEdit控件
CStatic *pStaMonitorAliasName = &staMonitorAliasName;//监控别名CStatic控件
CEdit *pEditMonitorAliasName = &editMonitorAliasName；//监控别名CEdit控件
listAllContorl.push_back(pStaMonitorName);
listAllContorl.push_back(pEditMonitorName);
listAllContorl.push_back(pStaMonitorAliasName);
listAllContorl.push_back(pEditMonitorAliasName);
//将listAllControl控件放到设备pannel信息区域显示
UpdatePanelControlPosition(listAllContorl,rectDevicePannelInfo.left,rectDevicePannelInfo.top,rectDevicePannelInfo.right,rectDevicePannelInfo.bottom);



