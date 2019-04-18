# 一个基于Directshow实现的音频播放器，支持歌词显示 - 爱写代码的自由人 - CSDN博客





2018年02月24日 19:43:35[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：438








之前在VC知识库上下载了一个基于Directshow做的音乐播放器，带歌词显示功能，觉得挺酷的。我下载了代码，编译了工程之后，运行起来的界面效果如下：

![](https://img-blog.csdn.net/20180224183701234)


 这个播放器支持的功能有：

  支持播放MP3/AAC/WAV等常见的音频格式；

  音乐播放控制（播放、暂停、停止、跳跃播放）；

  调节音量；

   音乐播放过程中同步显示歌词；

   歌词的文字颜色和背景色可调，并且可设置滚动方向；

   歌词显示支持淡入淡出；

   因为播放引擎是Directshow，这个播放器支持的音频格式挺广泛的，只要你系统上装了某种格式的解码器，都能解码对应格式的音频。播放音乐的操作封装为一个类来处理，这个类是CDXGraph，类声明如下：

```cpp
class CDXGraph
{
private:
	IGraphBuilder *     mGraph;  
	IMediaControl *		mMediaControl;
	IMediaEventEx *		mEvent;
	IBasicVideo *		mBasicVideo;
	IBasicAudio *		mBasicAudio;
	IVideoWindow  *		mVideoWindow;
	IMediaSeeking *		mSeeking;

	DWORD				mObjectTableEntry; 

public:
	CDXGraph();
	virtual ~CDXGraph();

public:
	virtual bool Create(void);
	virtual void Release(void);
	virtual bool Attach(IGraphBuilder * inGraphBuilder);

	IGraphBuilder * GetGraph(void); // Not outstanding reference count
	IMediaEventEx * GetEventHandle(void);

	bool ConnectFilters(IPin * inOutputPin, IPin * inInputPin, const AM_MEDIA_TYPE * inMediaType = 0);
	void DisconnectFilters(IPin * inOutputPin);

	//bool SetDisplayWindow(HWND inWindow);
	bool SetNotifyWindow(HWND inWindow);
	//bool ResizeVideoWindow(long inLeft, long inTop, long inWidth, long inHeight);

	void HandleEvent(WPARAM inWParam, LPARAM inLParam);

	bool Run(void);        // Control filter graph
	bool Stop(void);
	bool Pause(void);
	bool IsRunning(void);  // Filter graph status
	bool IsStopped(void);
	bool IsPaused(void);

	//bool SetFullScreen(BOOL inEnabled);
	//bool GetFullScreen(void);

	// IMediaSeeking
	bool GetCurrentPosition(LONGLONG * outPosition); //获得当前播放时间(单位为100纳秒, 等于10^(-4) ms )
	bool GetStopPosition(LONGLONG * outPosition);
	bool SetCurrentPosition(LONGLONG inPosition); //设置当前播放时间(单位为100纳秒, 等于10^(-4) ms )
	bool SetStartStopPosition(LONGLONG inStart, LONGLONG inStop);
	bool GetDuration(LONGLONG * outDuration); //获得文件时间长度(单位为100纳秒, 等于10^(-4) ms )
	bool SetPlaybackRate(double inRate); //设置播放速度

	// Attention: range from -10000 to 0, and 0 is FULL_VOLUME.
	bool SetAudioVolume(long inVolume);//调节音量
	long GetAudioVolume(void);
	// Attention: range from -10000(left) to 10000(right), and 0 is both.
	bool SetAudioBalance(long inBalance);
	long GetAudioBalance(void);

	bool RenderFile(const char * inFile);


private:
	void AddToObjectTable(void) ;
	void RemoveFromObjectTable(void);
	
	bool QueryInterfaces(void);
};
```

在分析歌词显示的代码之前，有必要介绍一下LRC歌词文件的语法和规则：

**LRC ****歌词**是一种包含着“[*:*]”形式的“标签(tag)”的、基于纯文本的歌词专用格式。最早由郭祥祥先生(Djohan)提出并在其程序中得到应用。这种歌词文件既可以用来实现卡拉OK功能（需要专门程序），又能以普通的文字处理软件查看、编辑。当然，实际操作时通常是用专门的LRC歌词编辑软件进行高效编辑的。以下具体介绍LRC格式中的“标签”。

**时间标签（Time-tag）**

形式为"[mm:ss]"或"[mm:ss.fff]"（分钟数:秒数）。
数字须为非负整数，比如"[12:34.5]"是有效的，而"[0x0C:-34.5]"无效。

它可以位于某行歌词中的任意位置。一行歌词可以包含多个时间标签（比如歌词中的迭句部分）。
根据这些时间标签，用户端程序会按顺序依次高亮显示歌词，从而实现卡拉OK功能。另外，标签无须排序。

**标识标签（ID-tags）**

其格式为"[标识名:值]"。大小写等价。以下是预定义的标签。

[ar:艺人名] 
[ti:曲名] 
[al:专辑名] 
[by:编者（指编辑LRC歌词的人）] 
[offset:时间补偿值] 其单位是毫秒，正值表示整体提前，负值相反。这是用于总体调整显示快慢的。

样例

[ar:unknown] 
[ti:sample] 
[al:none] 
[by:me] 
[01:02.355][00:00]This line should be sung twice 
[00:05.7]And this one... once only. 

接着，看一个真正的歌词文件---青花瓷.lrc：

[ti:青花瓷]
[ar:周杰伦]
[al:我很忙]
[by:High Templar]

[00:00.00]周杰伦 - 青花瓷
[00:01.21]词/方文山  曲/周杰伦
[00:04.88]编辑：High Templar
[00:21.98]素胚勾勒出青花笔锋浓转淡
[00:26.42]瓶身描绘的牡丹一如你初妆
[00:30.93]冉冉檀香透过窗心事我了然
[00:35.26]宣纸上走笔至此搁一半
[00:39.77]釉色渲染仕女图韵味被私藏
[00:44.04]而你嫣然的一笑如含苞开放
[00:48.65]你的美一缕飘散
[00:51.03]去到我去不了的地方
[03:19.79][03:02.06][02:26.43][01:15.35][00:57.61]天青色等烟雨 而我在等你
[03:06.31][02:30.72][01:01.88]炊烟袅袅升起 隔江千万里
[03:10.94][02:35.30][01:06.45]在瓶底书刻隶仿前朝的飘逸
[03:15.36][02:39.70][01:10.80]就当我为遇见你伏笔
[03:24.18][01:19.64]月色被打捞起 云开了结局
[01:24.41]如传世的青花瓷自顾自美丽
[01:27.53]你眼带笑意
[03:40.60][01:32.11]
[01:50.86]色白花青的锦鲤跃然于碗底
[01:55.41]临摹宋体落款时却惦记着你
[01:59.75]你隐藏在窑烧里千年的秘密
[02:04.18]极细腻犹如绣花针落地
[02:08.70]帘外芭蕉惹骤雨门环惹铜绿
[02:13.04]而我路过那江南小镇惹了你
[02:17.74]在泼墨山水画里
[02:19.86]你从墨色深处被隐去
[02:44.17]天青色等烟雨　而我在等你
[02:48.68]月色被打捞起　云开了结局


[03:28.63][02:53.06]如传世的青花瓷自顾自美丽 你眼带笑意

 理解LRC格式很简单，这里就不多讲了。

下面讲一下歌词显示窗口的实现。滚动显示歌词由一个名为CLiveWnd的类来处理，这个类继承于CWnd，是一个窗口控件。

 程序里定义一个结构体TIMETAG，记录一行歌词的信息，其信息包括：该行歌词的LineIndex，播放时间，水平坐标，整行歌词的Text宽度。

```cpp
typedef struct tagTime{
  int     lineNum;
  float   timeValue;
  int     dd;//一行歌词在水平滚动模式下的横向坐标（第一句歌词的位置为原点）
  int     lineWidth; //一行歌词的Text宽度
}TIMETAG;
```

而歌词的字符串内容呢？它被记录到另外一个数据结构里面，用一个数组来存储所有的歌词（每个元素为一行内容）。

```cpp
std::vector<CString> m_vWords; //存储每句歌词
```

  而TIMETAG的信息也作为元素放到另外一个数组里面：


```cpp
std::vector<TIMETAG> m_vTimeTags; //歌词播放的时间标签
```
  这两个数组的元素是按顺序一一对应的，比如m_vWords数组的第一个元素就对应m_vTimeTags数组的第一个TIMETAG的信息。（疑问：为什么作者不把两个信息都弄到一个结构里面就不得而知了！）

   读歌词的相关函数实现如下：

```cpp
//函数作用：切分一行歌词成多个带时间标签的歌词(一行歌词可以包含多个时间标签),重唱的歌词需要分配一个独立的时间标签；
// 时间标签存储在数组m_vTimeTags里, 每句歌词存储在数组m_vWords里，两个数组通过lineNumber做映射；
// 如果一行中有多个时间标签，表示这句歌词在几个不同时间里被重唱，m_vTimeTags里的多个时间标签可以对应m_vWords的相同的歌词内容
// 如果成功添加一行歌词（不包括带说明意义的标签），返回TRUE，否则返回FALSE
BOOL CLiveWnd::SpliceLine( LPCSTR lpsz, int lineNumber)
{
  CString str(lpsz);
  CString sz_time_tag;
  CString strHead;

   str.TrimLeft();
   str.TrimRight();

  int pos, pos2;
  pos = str.Find("[");
  if(pos == -1)
	  return FALSE;

  bool bAddStr = true;

  while(pos !=-1 )
  {

    str = str.Mid(pos+1);
    pos = str.Find("]");
	if(pos!=-1)
	{
	   if(lineNumber<5)
	   {
		   if( (pos2 = str.Find(":")) != -1)
		   {
			   strHead = str.Left(pos2);

				if( strHead== "ti" || strHead== "ar" || strHead=="by" || strHead=="al" || strHead == "offset" )
				{
					str = str.Left(str.GetLength()-1);
					bAddStr = false;
					break;
				}
		    }  
	   }
	    AddTimeTag(str.Left(pos),lineNumber);
        str = str.Mid(pos+1);
	}
	else
	{
		bAddStr = false;
		return bAddStr;
	}
	  
	pos = str.Find("[");

 }//while


  if(bAddStr)
  {
	str.TrimLeft();
    str.TrimRight();
    AddWord(str);
  }

  //TRACE("%d, %d \n", m_vTimeTags.size(), m_vWords.size());
  
  return bAddStr;
}
```

```cpp
BOOL CLiveWnd::ReadFile(LPCSTR lpszFile)
{
  CStdioFile file;
  if( !file.Open(lpszFile, CFile::modeRead) )
	  return FALSE;

 m_vTimeTags.clear();
 m_vWords.clear();

  CString line;
  int i = 0;
  while( file.ReadString(line)!= FALSE ) //读取每行字符串
  {
     //TRACE("%s\n", line);
	  if(!line.IsEmpty())//不是空行
	  {
		  //解析每一行
	      if(SpliceLine(line, i))
		  {
		    i++;
		  }
	  }
      line.Empty();
	 
   }
  file.Close();

  //最后增加一个无歌词的空行
  m_vWords.push_back("");
  TIMETAG tm;
  tm.lineNum = m_vWords.size()-1;
  tm.timeValue = ((CTestLyricDlg*)AfxGetMainWnd())->GetSongTotalTime(); //时间值为文件的总时间长度
  m_vTimeTags.push_back(tm);
  
  Sort();

  TRACE("TimeTag Count: %d, Word Count: %d \n", m_vTimeTags.size(), m_vWords.size());

  SetHorzData();

  return TRUE;
}
```

CLiveWnd重载了定时器OnTimer函数，用于定时刷新窗口中滚动显示的歌词，函数实现如下：

```cpp
void CLiveWnd::OnTimer(UINT nIDEvent)
{
	CWnd::OnTimer(nIDEvent);

	int CurrentTime = GetPlayPos();
	m_iTime = CurrentTime;

	int CurrentCentHeight = m_iDrawHeight/2 + m_nTextHeight;
	int CurrentCentWidth  = m_iDrawWidth/2  + m_nTextSpace;

	int i,lc,TY,TX;
	for(i = 0; i<= GetLineCount()-1; i++)
	{
		if(GetTimeValue(i) > CurrentTime ) break;
	}

	if(m_bVertical)
	{ 
		if ( i >= 1 && i < GetLineCount() )
		{
			lc = m_nTextHeight * (CurrentTime - GetTimeValue(i-1) ) / (GetTimeValue(i-1) - GetTimeValue(i));
			CurrentCentHeight = lc + CurrentCentHeight;//当前高亮行的Y坐标
			TY = CurrentCentHeight - (i - 1) * m_nTextHeight;   
		}
		else
		{
			TY =  CurrentCentHeight - m_nTextHeight;
		}

		m_iCurrent = i-1;

		if( abs(m_iTextOutY-TY)>1 )
		{
			m_iTextOutY = TY;
			TextToScreen();
			InvalidateRect(m_rectangle, 0 );
		}
	}//if

	else  //水平滚动
	{
		if ( i >= 1 && i < GetLineCount() )
		{
			int len = GetHorzTextW(i-1);
			// TRACE("%d,TextLen:%d,CurrentTime:%d\n",i-1, len,CurrentTime/1000);

			lc = (GetHorzTextW(i-1))*(CurrentTime - GetTimeValue(i-1) ) / (GetTimeValue(i-1) - GetTimeValue(i));
			CurrentCentWidth = lc + CurrentCentWidth;

			// int xx = GetHorzLen(i-1);
			TX = CurrentCentWidth - GetHorzLen(i-1); 
		}
		else
		{
			TX =  CurrentCentWidth + m_nTextSpace;
		}

		m_iCurrent = i-1;

		if(abs(m_iTextOutX- TX)>1)
		{
			m_iTextOutX = TX;
			TextToScreen();
			InvalidateRect(m_rectangle, 0 );
		}	 
	}//else

	//UpdateWindow();
}
```

 显示歌词的核心函数是TextToScreen，代码如下：

```cpp
BOOL CLiveWnd::TextToScreen( void )
{
	DWORD dwRead = 0;

	FillRect( m_dcMemoryDC, &m_rectangle, m_hBrushBackground );
	SetBkMode(m_dcMemoryDC, TRANSPARENT);

	CRect rcLine;
	CString strWord;

	if(m_bVertical)   //垂直滚动
	{
		for(int i = 0; i<GetLineCount(); i++)
		{

			int y = GetY(i);
			if(y<-m_nTextHeight) continue;
			if(y>m_iDrawHeight)  break;

			rcLine.SetRect(0, y, m_iDrawWidth, y+m_nTextHeight);
			strWord = GetString(i);

			COLORREF cLeft,cRight,color;

			if(m_iCurrent-1 == i && i>=0 )
			{
				float iOnBand = m_iTime - GetTimeValue(m_iCurrent);
				float iDelta  = (GetTimeValue(m_iCurrent+1) - GetTimeValue(m_iCurrent))/2; //取相邻两个时间标签的时间间隔的一半为过渡时间

				if(m_bFadeEffect && iOnBand<iDelta) //淡出
				{
					cLeft  = m_clrTextHighlighted;
					cRight = m_clrForeground;
					color = RGB((GetRValue(cRight)-GetRValue(cLeft))*((float)iOnBand)/iDelta +GetRValue(cLeft),
						(GetGValue(cRight)-GetGValue(cLeft))*((float)iOnBand)/iDelta +GetGValue(cLeft),
						(GetBValue(cRight)-GetBValue(cLeft))*((float)iOnBand)/iDelta +GetBValue(cLeft));

					::SetTextColor(m_dcMemoryDC, color);
				}
				else
				{
					::SetTextColor(m_dcMemoryDC, m_clrForeground);
				}
			}

			else if(m_iCurrent== i )
			{

				float iOnBand = m_iTime - GetTimeValue(m_iCurrent);
				float iDelta  = ( GetTimeValue(m_iCurrent+1) - GetTimeValue(m_iCurrent) )/2; //取相邻两个时间标签的时间间隔的一半为过渡时间

				if(m_bFadeEffect && iOnBand<iDelta) //淡入
				{
					cLeft  = m_clrForeground;
					cRight = m_clrTextHighlighted;
					color = RGB((GetRValue(cRight)-GetRValue(cLeft))*((float)iOnBand)/iDelta +GetRValue(cLeft),
						(GetGValue(cRight)-GetGValue(cLeft))*((float)iOnBand)/iDelta +GetGValue(cLeft),
						(GetBValue(cRight)-GetBValue(cLeft))*((float)iOnBand)/iDelta +GetBValue(cLeft));

					::SetTextColor(m_dcMemoryDC, color);
				}
				else
				{
					::SetTextColor(m_dcMemoryDC, m_clrTextHighlighted);
				}
			}

			else
			{
				::SetTextColor(m_dcMemoryDC, m_clrForeground);
			}

			DrawTextEx( m_dcMemoryDC, strWord.GetBuffer(0), strWord.GetLength(), &rcLine, DT_CENTER, 0 ); //居中显示
			strWord.ReleaseBuffer();
		}//for

	}

	//水平滚动
	else
	{
		for(int i = 0; i<GetLineCount(); i++)
		{
			int x = GetX(i);
			int w = GetHorzTextW(i);
			if(x<-w)               continue;
			if(x>m_iDrawWidth)     break;

			rcLine.SetRect(x, 0, x+w, m_iDrawHeight);
			strWord = GetString(i);

			COLORREF cLeft,cRight,color;

			if(m_iCurrent-1 == i && i>=0 )
			{
				float iOnBand = m_iTime - GetTimeValue(i+1);
				float iDelta  = 1000;

				if(m_bFadeEffect && iOnBand<iDelta)
				{
					cLeft  = m_clrTextHighlighted;
					cRight = m_clrForeground;
					color = RGB((GetRValue(cRight)-GetRValue(cLeft))*((float)iOnBand)/iDelta +GetRValue(cLeft),
						(GetGValue(cRight)-GetGValue(cLeft))*((float)iOnBand)/iDelta +GetGValue(cLeft),
						(GetBValue(cRight)-GetBValue(cLeft))*((float)iOnBand)/iDelta +GetBValue(cLeft));

					::SetTextColor(m_dcMemoryDC, color);
				}
				else
				{
					::SetTextColor(m_dcMemoryDC, m_clrForeground);
				}

			}

			else if(m_iCurrent== i )
			{
				::SetTextColor(m_dcMemoryDC, m_clrTextHighlighted);
			}

			else
			{
				::SetTextColor(m_dcMemoryDC, m_clrForeground);
			}

			DrawTextEx( m_dcMemoryDC, strWord.GetBuffer(0), strWord.GetLength(), &rcLine, 
				DT_LEFT|DT_VCENTER|DT_SINGLELINE, 0 ); //左对齐显示
			strWord.ReleaseBuffer();
		}//for
	}

	return true;
}
```

 另外，我运行程序时，在水平滚动模式下当歌词显示到最后一行时，程序出现崩溃，仔细检查代码发现错误是在CLiveWnd：：GetHorzTextW，原来是数组越界了。我修改了一下代码，后面就没问题了。

详细的实现细节大家看代码吧， 从我的资源里下载这个工程代码：

http://download.csdn.net/download/zhoubotong2012/10257555




