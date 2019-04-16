# 最简单的基于libVLC的例子：最简单的基于libVLC的视频播放器（图形界面版） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年01月09日 12:51:15[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：25214










=====================================================

最简单的基于libVLC的例子文章列表：

[最简单的基于libVLC的例子：最简单的基于libVLC的视频播放器](http://blog.csdn.net/leixiaohua1020/article/details/42363079)

[最简单的基于libVLC的例子：最简单的基于libVLC的视频播放器（图形界面版）](http://blog.csdn.net/leixiaohua1020/article/details/42363593)

[最简单的基于libVLC的例子：最简单的基于libVLC的推流器](http://blog.csdn.net/leixiaohua1020/article/details/42363701)

=====================================================

本文记录使用libVLC开发的一个简单的图形界面的视频播放器。由于是示例程序，只包含了最简单的媒体播放方面的功能。其中有一些功能还不完善，有时间以后再做修改。

![](https://img-blog.csdn.net/20150103152439596)



## 最简单的基于libVLC的视频播放器（图形界面版）

这是使用libVLC基于MFC开发的一个示例播放器。实现了一个播放器的基本功能：播放，暂停/继续，停止，播放时间轴的显示，以及从任一点开始播放媒体。并且支持将媒体文件拖拽至播放器进行播放。播放前将媒体文件的路径输入到“URL”栏中，然后单击“Start”即可开始播放。

![](https://img-blog.csdn.net/20150103152526453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


播放时候的效果截图如下所示。

![](https://img-blog.csdn.net/20150103152523979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


源代码比较长，不再详细记录。简单记录一下其中几个主要功能的实现机制。

### 视频的播放/暂停/继续/停止

#### 播放
视频“播放”的源代码如下所示。简单来说，完成了以下视频播放的初始化工作：
（1）输入的URL转换为UTF-8编码（后文详细记录）
（2）将显示视频画面的控件的句柄提供给libVLC
（3）初始化libVLC并开始播放
（4）开启定时器，用于更新视频播放的进度（后文详细记录）

```cpp
void CplayerGUIDlg::OnBnClickedStart()
{
	CStringW cstr_url;

#ifdef _UNICODE
	m_url.GetWindowText(cstr_url);
#else
	USES_CONVERSION;
	CStringA cstr_urla;
	m_url.GetWindowText(cstr_urla);
	cstr_url.Format(L"%s",A2W(cstr_urla));
#endif

	std::string str_url;
	UNICODE_to_UTF8(cstr_url, str_url);
	const char *char_url=str_url.c_str();

	if(strcmp(char_url,"")==0){
		AfxMessageBox(_T("Input URL is NULL!"));
		return;
	}
	
	HWND screen_hwnd=NULL;
	screen_hwnd = this->GetDlgItem(IDC_SCREEN)->m_hWnd; 

	if(playerstate!=STATE_PREPARE){
		AfxMessageBox(_T("Media is playing now."));
		return;
	}


     /* Create a new item */
     //m = libvlc_media_new_location (libvlc_inst, "http://mycool.movie.com/test.mov");

     libvlc_m = libvlc_media_new_path (libvlc_inst, char_url);

     /* Create a media player playing environement */
     libvlc_mp = libvlc_media_player_new_from_media (libvlc_m);
     
     /* No need to keep the media now */
     libvlc_media_release (libvlc_m);
 
    //on windows
     libvlc_media_player_set_hwnd (libvlc_mp,screen_hwnd);
 
     /* play the media_player */
     int x=libvlc_media_player_play (libvlc_mp);
    
     //_sleep (30000); /* Let it play a bit */
    

	playerstate=STATE_PLAY;
	SetBtn(STATE_PLAY);
	SetTimer(1,1000,NULL);
}
```


#### 暂停/继续
视频“暂停/继续”的源代码如下所示。其中调用了libvlc_media_player_set_pause()设定“暂停”或者是“继续”。

```cpp
void CplayerGUIDlg::OnBnClickedPause()
{
	if(playerstate==STATE_PLAY){
		libvlc_media_player_set_pause(libvlc_mp,1);
		playerstate=STATE_PAUSE;
		GetDlgItem(ID_PAUSE)->SetWindowText(_T("Resume"));
	}else if(playerstate==STATE_PAUSE){
		libvlc_media_player_set_pause(libvlc_mp,0);
		playerstate=STATE_PLAY;
		GetDlgItem(ID_PAUSE)->SetWindowText(_T("Pause"));
	}
}
```


#### 停止
视频“停止”的源代码如下所示。其中调用了libvlc_media_player_stop()停止视频播放，并且调用libvlc_media_player_release()释放相应的libvlc_media_player_t结构体。

```cpp
void CplayerGUIDlg::OnBnClickedStop()
{
	if(libvlc_mp!=NULL){
		libvlc_media_player_stop (libvlc_mp);
		libvlc_media_player_release (libvlc_mp);
		KillTimer(1);
	}
	
	SystemClear();
}
```


### 视频播放进度杂时间轴的显示
随着视频的播放，需要在视频播放进度的时间轴上更新播放进度信息。在程序中使用了一个定时器完成这个功能。
在视频开始播放的时候，调用SetTimer()开启定时器。时间间隔设置为1000ms。

```cpp
SetTimer(1,1000,NULL);
```
在视频停止播放的时候，调用KillTimer()结束定时器。

```cpp
KillTimer(1);
```
在定时器的消息响应函数中，调用libvlc_media_player_get_time()获取当前视频的播放进度，此外调用libvlc_media_player_get_length()获取视频的总时长。
经过计算之后，就可以把计算的结果设置到相应的编辑框（Edit Control）以及滑动控制条（Slider Control）上。

```cpp
void CplayerGUIDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 1){
		CString curtimestr,durationstr;
		int curtime;
		int duration;
		int tns, thh, tmm, tss;
		int progress;
		//ms
		curtime = libvlc_media_player_get_time(libvlc_mp);
		if(curtime!=0){
			//change to second
			tns = curtime/1000;
			thh  = tns / 3600;
			tmm  = (tns % 3600) / 60;
			tss  = (tns % 60);
			curtimestr.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
			m_curtime.SetWindowText(curtimestr);
		}
		duration  = libvlc_media_player_get_length(libvlc_mp);
		if(duration!=0){
			tns = duration/1000;
			thh  = tns / 3600;
			tmm  = (tns % 3600) / 60;
			tss  = (tns % 60);
			durationstr.Format(_T("%02d:%02d:%02d"),thh,tmm,tss);
			m_duration.SetWindowText(durationstr);

			progress=curtime*100/duration;
			m_progress.SetPos(progress);
		}
	}

	//Stop in the end
	if(libvlc_media_player_get_state(libvlc_mp)==libvlc_Ended)
		OnBnClickedStop();


	CDialogEx::OnTimer(nIDEvent);
}
```



### 视频播放点的调整
当鼠标拖动滑动控制条（Slider Control）控件上的滑块的时候，需要根据拖动的位置设置视频的播放进度。此时调用libvlc_media_player_set_position()设定视频的播放进度。消息响应函数中的代码如下所示。

```cpp
void CplayerGUIDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar->GetSafeHwnd() == m_progress.GetSafeHwnd()){
		float posf=0.0;
		if(nSBCode==SB_THUMBPOSITION){
			posf=(float)nPos/100.0;
			libvlc_media_player_set_position(libvlc_mp,posf);
		}
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
```


### libVLC中输入中文文件路径问题

libVLC使用英文作为输入路径的时候是没有问题的。但是当我们直接传递中文路径的时候，会出现libVLC将中文解析为乱码从而导致无法播放的问题。这个问题卡了我一阵子时间。造成这一问题的原因在于VLC的输入文件路径是使用UTF-8编码的。因此我们需要将输入路径转码为UTF-8编码。转码之后这一问题即得到了解决。
Unicode转码为UTF-8的函数代码如下所示。

```cpp
void CplayerGUIDlg::UNICODE_to_UTF8(CStringW& unicodeString, std::string& str)
{
	int stringLength = ::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), NULL, 0, NULL, NULL);

	char* buffer = new char[stringLength + 1];
	::WideCharToMultiByte(CP_UTF8, NULL, unicodeString, wcslen(unicodeString), buffer, stringLength, NULL, NULL);
	buffer[stringLength] = '\0';

	str = buffer;

	delete[] buffer;
}
```


## 下载




**Simplest libVLC Example**




**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestlibvlcexample/](https://sourceforge.net/projects/simplestlibvlcexample/)

Github：[https://github.com/leixiaohua1020/simplest_libvlc_example](https://github.com/leixiaohua1020/simplest_libvlc_example)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_libvlc_example](http://git.oschina.net/leixiaohua1020/simplest_libvlc_example)





CDSN下载地址： [http://download.csdn.net/detail/leixiaohua1020/8342413](http://download.csdn.net/detail/leixiaohua1020/8342413)

本工程是包含了一些基于libVLC的示例程序。一共包含了如下几个子程序。
playerGUI: 最简单的基于libVLC的播放器-图形界面版。
simplest_libvlc_example: 最简单的基于libVLC的播放器。
simplest_libvlc_streamer: 最简单的基于libVLC的推流器。




