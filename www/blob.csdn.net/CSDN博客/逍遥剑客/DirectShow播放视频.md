# DirectShow播放视频 - 逍遥剑客 - CSDN博客
2008年06月30日 21:26:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：4979
本来想用VMR9绘制到D3D的Texture上的, 试着弄了一下, 慢得真够可以的, 还是直接全屏(窗口)放得了
代码也懒得写了, 直接拿SDK里的VMRPlayer中CMovie类用就行 
 1: CMovie g_movie  NULL
 2:  g_movie   CMovieg_hWnd
 4: g_movieOpenMovie
 6: g_moviePlayMovie
 8: RECT rc
10: GetClientRectg_hWnd rc
11: g_moviePutMoviePositionrcleft rctop rcrightrcleft rcbottomrctop
12: g_movieRepaintVideog_hWnd GetDCg_hWnd
![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DirectShow_Video_Test.JPG)
其实这个类一看就明白怎么用.... 
 1:  5:  EMovieMode
 6:  7: 	MOVIE_NOTOPENED  
 8: 	MOVIE_OPENED     
 9: 	MOVIE_PLAYING    
10: 	MOVIE_STOPPED    
11: 	MOVIE_PAUSED     
12: 13: 14:  IMpegAudioDecoder
15:  IMpegVideoDecoder
16:  IQualProp
17: 18:  CMovie
19: 20: 21: 		EMovieMode       m_Mode
23: 	HANDLE           m_MediaEvent
24: 	HWND             m_hwndApp
25: 	GUID             m_TimeFormat
26: 27: 	IFilterGraph            m_Fg
28: 	IGraphBuilder           m_Gb
29: 	IMediaControl           m_Mc
30: 	IMediaSeeking           m_Ms
31: 	IMediaEvent             m_Me
32: 	IVMRWindowlessControl9   m_Wc
33: 34: 	HRESULT AddVideoMixingRendererToFG
35: 	HRESULT FindInterfaceFromFilterGraph
36: 		REFIID iid 		LPVOID lp  		
39: 40: 41: 	CMovieHWND hwndApplication
42: 	CMovie
43: 44: 	HRESULT         OpenMovieTCHAR lpFileName
45: 	DWORD           CloseMovie
46: 47: 	BOOL            PlayMovie
48: 	BOOL            PauseMovie
49: 	BOOL            StopMovie
50: 51: 	OAFilterState   GetStateMovie
52: 	HANDLE          GetMovieEventHandle
53: 	            GetMovieEventCode
54: 55: 	BOOL            PutMoviePositionLONG x LONG y LONG cx LONG cy
56: 	BOOL            GetMoviePositionLONG x LONG y LONG cx LONG cy
57: 	BOOL            GetNativeMovieSizeLONG cx LONG cy
58: 	BOOL            CanMovieFrameStep
59: 	BOOL            FrameStepMovie
60: 61: 	REFTIME         GetDuration
62: 	REFTIME         GetCurrentPosition
63: 	EMovieMode      StatusMovie
64: 65: 	BOOL            SeekToPositionREFTIME rtBOOL bFlushData
66: 	BOOL            IsTimeFormatSupportedGUID Format
67: 	BOOL            IsTimeSupported
68: 	BOOL            SetTimeFormatGUID Format
69: 70: 	GUID            GetTimeFormat
71: 	            SetFocus
72: 	BOOL            ConfigDialogHWND hwnd
73: 	BOOL            RepaintVideoHWND hwnd HDC hdc
74: 75: 	HRESULT         SetAppImageVMR9AlphaBitmap lpBmpInfo
76: 	HRESULT         UpdateAppImageVMR9AlphaBitmap lpBmpInfo
77: 	            SetBorderClrCOLORREF clr
78: 79: 	            DisplayModeChanged
80: 81: 	HRESULT         GetCurrentImageLPBYTE lplpDib
82: 83: 	IVMRMixerControl9 m_pMixControl
84: 
