# createwindow 创建 STATIC 文本并设置字体 - sandro_zhang的专栏 - CSDN博客
2011年09月28日 10:39:18[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：4409
```cpp
HFONT hf = CreateFont( 16,                                                 //   nHeight 
                              0,                                                   //   nWidth 
                              0,                                                   //   nEscapement 
                              0,                                                   //   nOrientation 
                              FW_BOLD,                                       //   nWeight 
                              FALSE,                                           //   bItalic 
                              FALSE,                                           //   bUnderline 
                              0,                                                   //   cStrikeOut 
                              DEFAULT_CHARSET,                       //   nCharSet 
                              OUT_DEFAULT_PRECIS,                 //   nOutPrecision 
                              CLIP_DEFAULT_PRECIS,               //   nClipPrecision 
                              DEFAULT_QUALITY,                       //   nQuality 
                              DEFAULT_PITCH   |   FF_SWISS,     //   nPitchAndFamily 
                              TEXT( "微软雅黑"));                           //   lpszFacename 
            // Create message label
            HWND htext = CreateWindow(
                        TEXT("STATIC"),                   /*The name of the static control's class*/
                        TEXT("测试消息很长的一段很长的一段很长的一段很长的一段很长的一段很长的一段"),                  /*Label's Text*/
                        WS_CHILD | WS_VISIBLE | SS_LEFT,  /*Styles (continued)*/
                        3,                                /*X co-ordinates*/
                        20,                                /*Y co-ordinates*/
                        194,                               /*Width*/
                        78,                               /*Height*/
                        hWnd,                             /*Parent HWND*/
                        (HMENU) IDC_NOTIFY_MSG_LABEL,              /*The Label's ID*/
                        NULL,                        /*The HINSTANCE of your program*/ 
                        NULL);
            //set font we defined
            SendMessage(htext, WM_SETFONT, (WPARAM)hf, TRUE);
```
另注：
取得系统字体
```cpp
/*
                #define OEM_FIXED_FONT      10
                #define ANSI_FIXED_FONT     11
                #define ANSI_VAR_FONT       12
                #define SYSTEM_FONT         13
                #define DEVICE_DEFAULT_FONT 14
                #define DEFAULT_PALETTE     15
                #define SYSTEM_FIXED_FONT   16
                #if(WINVER >= 0x0400)
                #define DEFAULT_GUI_FONT    17
                #endif 
            */
            HFONT   hSysFont   =   (HFONT)   ::GetStockObject(SYSTEM_FONT);
```
