# 第一个DirectShow程序 - 逍遥剑客 - CSDN博客
2008年06月24日 17:28:00[xoyojank](https://me.csdn.net/xoyojank)阅读数：2393
 1:  4: #include 
 5:  6:  7: #include dshowh
 8:  9: 10: 	IGraphBuilder pGraph  NULL
11: 	IMediaControl pControl  NULL
12: 	IMediaEvent   pEvent  NULL
13: 14: 		HRESULT hr  CoInitializeNULL
16: 	 FAILEDhr
17: 18: 		printf
19: 20: 21: 22: 		hr  CoCreateInstanceCLSID_FilterGraph NULL CLSCTX_INPROC_SERVER 
24: 		IID_IGraphBuilder  pGraph
25: 	 FAILEDhr
26: 27: 		printf
28: 29: 30: 31: 	hr  pGraphQueryInterfaceIID_IMediaControl  pControl
32: 	hr  pGraphQueryInterfaceIID_IMediaEvent  pEvent
33: 34: 		hr  pGraphRenderFileL NULL
36: 	 SUCCEEDEDhr
37: 38: 				hr  pControlRun
40: 		 SUCCEEDEDhr
41: 42: 						 evCode
44: 			pEventWaitForCompletionINFINITE evCode
45: 46: 49: 50: 	pControlRelease
51: 	pEventRelease
52: 	pGraphRelease
53: 	CoUninitialize
54: ![](https://p-blog.csdn.net/images/p_blog_csdn_net/xoyojank/DirectShow_First_Test.JPG)
