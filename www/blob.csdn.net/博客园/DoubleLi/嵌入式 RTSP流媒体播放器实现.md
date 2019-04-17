# 嵌入式 RTSP流媒体播放器实现 - DoubleLi - 博客园







最近需要做一个RTSP流媒体播放器，研究了一下，封装了一个RTSP播放类CRTSPPlayer，解码库采用ffmpeg。由于需求比较简单，时间也有限，目前只实现了播放、停止、暂停几个基本的接口。下面是基于CRTSPPlayer类实现的简单RTSP播放器。

![](http://img.my.csdn.net/uploads/201303/27/1364396276_5277.JPG)

       目前视频只测试了H264格式，其它格式的视频还未做测试。播放器也支持直接打开本地视频播放，但播放的帧率和原始视频的码率不同步。目前还不清楚如何处理这个问题，希望懂这方面的大侠指教。

       另外，还有一个开源的库VLC也可以用来开发流媒体播放器，它支持多种流媒体协议，如RTP、RTSP等，CodeProject上已经有牛人在VLCLib的基础上封装可更易使用的库VLCWrapper（地址：[http://www.codeproject.com/Articles/38952/VLCWrapper-A-Little-C-wrapper-Around-libvlc](http://www.codeproject.com/Articles/38952/VLCWrapper-A-Little-C-wrapper-Around-libvlc)）。用它可以很方便的开发视频播放器。

        以下是CRTSPPlayer完整的代码：

头文件:



**[cpp]**[view plain](http://blog.csdn.net/firehood_/article/details/8727914)[copy](http://blog.csdn.net/firehood_/article/details/8727914)





- /********************************************************************  
- filename:   CRTSPPlayer.h  
- created:    2013-03-25  
- author:     firehood  
- purpose:    ffmpeg库实现的RTSP视频播放器 
- *********************************************************************/   
- #pragma once  
- #include "windows.h"  
- 
- extern "C"  
- {  
- #include "libavformat\avformat.h"  
- #include "libavcodec\avcodec.h"  
- #include "libswscale\swscale.h"  
- };  
- 
- // 播放状态  
- enum RTSP_PLAYSTATUS  
- {  
-     RTSP_PLAYSTATUS_NONE,       // 未知状态（未播放）  
-     RTSP_PLAYSTATUS_PLAYING,    // 正在播放  
-     RTSP_PLAYSTATUS_PAUSE,      // 已暂停  
-     RTSP_PLAYSTATUS_STOP,       // 已停止  
- };  
- 
- class CRTSPPlayer  
- {  
- public:  
-     CRTSPPlayer(HWND hWnd, LPRECT lpRect);  
-     ~CRTSPPlayer(void);  
- public:  
- // 打开媒体文件  
- BOOL OpenMedia(LPCTSTR pFileName);  
- // 播放  
- void Play();  
- // 暂停  
- void Pause();  
- // 停止  
- void Stop();  
- // 获取播放状态  
-     RTSP_PLAYSTATUS GetPlayStatus(void);  
- private:  
- // 解码初始化  
- int DecodeInit(LPCTSTR pFileName);  
- // 卸载  
- void DecodeUninit();  
- // 开始解码线程  
- BOOL StartDecodeThread();  
- // 停止解码线程  
- void StopDecodeThread();  
- // 解码线程  
- static int WINAPI ThreadDecodeVideo(LPVOID lpParam);  
- // 开始解码任务  
- int BeginDecode();  
- // 显示  
- void Display();  
- // 图像转换  
- int ImgConvert(AVPicture * dst, PixelFormat dstFormt, const AVPicture * src, PixelFormat srcFormt, int src_width, int src_height);  
- // 设置播放状态  
- void SetPlayStatus(RTSP_PLAYSTATUS playStatus);  
- private:  
- HANDLE  m_hDecodeThread;  
- BOOL    m_bExitDecodeThread;  
- TCHAR   m_strFilePath[MAX_PATH];  
- 
-     AVFormatContext* m_pFormatContext;  
-     AVCodecContext*  m_pCodecContext;  
-     AVCodec* m_pCodec;  
-     AVPacket m_struPacket;  
- int m_nStreamIndex;  
-     AVFrame* m_pFrameYUV;  
-     AVFrame* m_pFrameRGB;  
- int     m_nFrameWidth;   
- int     m_nFrameHeight;  
- BYTE*   m_pBufRGB;        // 解码后的RGB数据  
- 
-     RTSP_PLAYSTATUS  m_nPlayStatus;  
- HWND    m_hWnd;  
-     RECT    m_rcWnd;  
- };  




源文件：



**[cpp]**[view plain](http://blog.csdn.net/firehood_/article/details/8727914)[copy](http://blog.csdn.net/firehood_/article/details/8727914)





- /********************************************************************  
- filename:   CRTSPPlayer.cpp  
- created:    2013-03-25  
- author:     firehood  
- purpose:    ffmpeg库实现的RTSP视频播放器 
- *********************************************************************/   
- #include "StdAfx.h"  
- #include "RTSPPlayer.h"  
- 
- #pragma comment(lib, "avformat.lib")  
- #pragma comment(lib, "avcodec.lib")  
- #pragma comment(lib, "swscale.lib")  
- #pragma comment(lib, "avutil.lib")  
- 
- #define SHOW_TITLE  
- 
- const char* WcharToUtf8(const wchar_t *pwStr)    
- {    
- if (pwStr == NULL)    
-     {    
- return NULL;    
-     }    
- 
- int len = WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, NULL, 0, NULL, NULL);    
- if (len <= 0)    
-     {    
- return NULL;    
-     }    
- char *pStr = new char[len];    
-     WideCharToMultiByte(CP_UTF8, 0, pwStr, -1, pStr, len, NULL, NULL);    
- return pStr;    
- }    
- 
- CRTSPPlayer::CRTSPPlayer(HWND hWnd, LPRECT lpRect):  
- m_hWnd(hWnd),  
- m_rcWnd(*lpRect),  
- m_hDecodeThread(NULL),  
- m_bExitDecodeThread(FALSE),  
- m_nFrameWidth(0),  
- m_nFrameHeight(0),  
- m_pFormatContext(NULL),  
- m_pCodecContext(NULL),  
- m_pCodec(NULL),  
- m_nStreamIndex(-1),  
- m_pFrameYUV(NULL),  
- m_pFrameRGB(NULL),  
- m_pBufRGB(NULL),  
- m_nPlayStatus(RTSP_PLAYSTATUS_NONE)  
- {  
-     memset(m_strFilePath,0,sizeof(m_strFilePath));  
- }  
- 
- CRTSPPlayer::~CRTSPPlayer(void)  
- {  
-     DecodeUninit();  
- }  
- 
- // 打开媒体文件  
- BOOL CRTSPPlayer::OpenMedia(LPCTSTR pFileName)  
- {  
- if(pFileName == NULL)  
- return FALSE;  
-     DecodeUninit();  
-     memcpy(m_strFilePath,pFileName,sizeof(m_strFilePath));  
-     DecodeInit(m_strFilePath);  
- return TRUE;  
- }  
- 
- // 播放  
- void CRTSPPlayer::Play()  
- {   
- if(GetPlayStatus() == RTSP_PLAYSTATUS_STOP)  
-     {  
-         DecodeInit(m_strFilePath);  
-     }  
- BOOL bRet = StartDecodeThread();  
- if(bRet)  
-     {  
-         SetPlayStatus(RTSP_PLAYSTATUS_PLAYING);  
-     }  
- }  
- 
- // 暂停  
- void CRTSPPlayer::Pause()  
- {  
-     StopDecodeThread();  
-     SetPlayStatus(RTSP_PLAYSTATUS_PAUSE);  
- }  
- 
- // 停止  
- void CRTSPPlayer::Stop()  
- {  
-     StopDecodeThread();  
-     DecodeUninit();  
-     SetPlayStatus(RTSP_PLAYSTATUS_STOP);  
- }  
- 
- BOOL CRTSPPlayer::StartDecodeThread()  
- {  
- if(m_hDecodeThread == NULL)  
-     {  
-         m_hDecodeThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadDecodeVideo, this, 0, NULL);  
-     }  
- return m_hDecodeThread ? TRUE : FALSE;  
- }  
- 
- void CRTSPPlayer::StopDecodeThread()  
- {  
- if(m_hDecodeThread)  
-     {  
-         m_bExitDecodeThread = TRUE;  
-         WaitForSingleObject(m_hDecodeThread,INFINITE);  
-         CloseHandle(m_hDecodeThread);  
-         m_hDecodeThread = NULL;  
-     }  
- }  
- 
- int CRTSPPlayer::ImgConvert(AVPicture * dst, PixelFormat dst_pix_fmt, const AVPicture * src, PixelFormat src_pix_fmt, int src_width, int src_height)  
- {  
- 
-     unsigned char * srcSlice[4];  
- int srcStride[4] = {0};  
- 
-     unsigned char * dstSlice[4];  
- int dstStride[4] = {0};  
- 
- 
- for (int i=0; i<4; i++)  
-     {  
-         srcSlice[i] = src->data[i];  
-         srcStride[i] = src->linesize[i];  
- 
-         dstSlice[i] = dst->data[i];  
-         dstStride[i] = dst->linesize[i];  
-     }  
- 
-     SwsContext *pSwsContext = sws_getContext(src_width, src_height, src_pix_fmt, src_width, src_height, dst_pix_fmt, SWS_BICUBIC, NULL, NULL, NULL);  
- 
- int nRet = sws_scale(pSwsContext, srcSlice, srcStride, 0, src_height, dstSlice, dstStride);  
- 
- if (pSwsContext != NULL)  
-     {  
-         sws_freeContext(pSwsContext);  
-     }  
- 
- return nRet;  
- }  
- 
- int WINAPI CRTSPPlayer::ThreadDecodeVideo(LPVOID lpParam)  
- {  
-     CRTSPPlayer *pPlayer = (CRTSPPlayer*)lpParam;  
- 
-     pPlayer->BeginDecode();  
- 
- return 0;  
- }  
- 
- int CRTSPPlayer::DecodeInit(LPCTSTR pFileName)  
- {  
- if(pFileName == NULL)  
-     {  
- return -1;  
-     }  
- 
-     av_register_all();  
- 
- #ifdef  UNICODE     
- const char *filePath = WcharToUtf8(pFileName);   
- // Open video  
- if (av_open_input_file(&m_pFormatContext, filePath, NULL, 0, NULL) != 0)  
-     {  
- return -2; // Couldn't open file  
-     }  
- delete[] filePath;  
- #else  
- // Open video  
- if (av_open_input_file(&m_pFormatContext, pFileName, NULL, 0, NULL) != 0)  
-     {  
- return -2; // Couldn't open file  
-     }  
- #endif  
- // Retrieve stream information  
- if (av_find_stream_info(m_pFormatContext) < 0)  
-     {  
- return -3; // Couldn't find stream information  
-     }  
- 
- // Find the first video stream  
- for (UINT i=0; i<m_pFormatContext->nb_streams; i++)  
-     {  
- if (m_pFormatContext->streams[i]->codec->codec_type == CODEC_TYPE_VIDEO)  
-         {  
-             m_nStreamIndex = i;  
- break;  
-         }  
-     }  
- if (m_nStreamIndex == -1)  
-     {  
- return -4; // Didn't find a video stream  
-     }  
- 
- // Get a pointer to the codec context for the video stream  
-     m_pCodecContext = m_pFormatContext->streams[m_nStreamIndex]->codec;  
- 
- // Find the decoder for the video stream  
-     m_pCodec = avcodec_find_decoder(m_pCodecContext->codec_id);  
- if (m_pCodec == NULL)  
-     {  
- return -5 ; // Codec not found  
-     }  
- 
- // Inform the codec that we can handle truncated bitstreams -- i.e.,  
- // bitstreams where frame boundaries can fall in the middle of packets  
- if (m_pCodec->capabilities & CODEC_CAP_TRUNCATED)  
-     {  
-         m_pCodecContext->flags |= CODEC_FLAG_TRUNCATED;  // we do not send complete frames  
-     }  
- 
- // Open codec  
- if (avcodec_open(m_pCodecContext, m_pCodec) < 0)  
-     {  
- return -6; // Could not open codec  
-     }  
- 
- // Allocate video frame  
-     m_pFrameYUV = avcodec_alloc_frame();  
- 
- // Allocate an AVFrame structure  
-     m_pFrameRGB = avcodec_alloc_frame();  
- 
- // Determine required buffer size and allocate buffer  
- int numBytes = avpicture_get_size(PIX_FMT_BGR24, m_pCodecContext->width, m_pCodecContext->height);  
-     m_pBufRGB = new BYTE [numBytes];  
-     memset(m_pBufRGB,0,numBytes);  
- // Assign appropriate parts of buffer to image planes in m_pFrameRGB  
-     avpicture_fill((AVPicture *)m_pFrameRGB, m_pBufRGB, PIX_FMT_BGR24, m_pCodecContext->width, m_pCodecContext->height);  
- 
-     m_nFrameWidth  = m_pCodecContext->width;  
-     m_nFrameHeight = m_pCodecContext->height;  
- 
- return 0;  
- }  
- 
- void CRTSPPlayer::DecodeUninit()  
- {  
- // Close the codec  
- if (m_pCodecContext)  
-     {  
-         avcodec_close(m_pCodecContext);  
- //av_free(m_pCodec);  
-         m_pCodecContext = NULL;  
-         m_pCodec = NULL;  
-     }  
- 
- // Close the video file  
- if (m_pFormatContext)  
-     {  
-         av_close_input_file(m_pFormatContext);  
-         m_pFormatContext = NULL;  
-     }  
- 
- if (m_pFrameYUV)  
-     {  
-         av_free(m_pFrameYUV);  
-         m_pFrameYUV = NULL;  
-     }  
- 
- if (m_pFrameRGB)  
-     {  
-         av_free(m_pFrameRGB);  
-         m_pFrameRGB = NULL;  
-     }  
- 
- if (m_pBufRGB)  
-     {  
- delete [] m_pBufRGB;  
-         m_pBufRGB = NULL;  
-     }  
- }  
- 
- int CRTSPPlayer::BeginDecode()  
- {  
- int bytesRemaining = 0, bytesDecoded;  
- BYTE * rawData = NULL;  
- 
- int frameFinished = 0;  
- 
-     m_struPacket.data = NULL;  
-     m_struPacket.size = 0;  
- 
-     m_bExitDecodeThread = FALSE;  
- 
- while (!m_bExitDecodeThread && m_pFormatContext)  
-     {  
- // Read the next packet, skipping all packets that aren't for this stream  
- do  
-         {  
- // Read new packet  
- if (av_read_frame(m_pFormatContext, &m_struPacket) < 0)  
-             {  
- 
- return -2;  
-             }  
-         } while (m_struPacket.stream_index != m_nStreamIndex);  
- 
-         bytesRemaining = m_struPacket.size;  
-         rawData = m_struPacket.data;  
- 
- // Work on the current packet until we have decoded all of it  
- while (bytesRemaining > 0)  
-         {  
- // Decode the next chunk of data  
-             bytesDecoded = avcodec_decode_video(m_pCodecContext, m_pFrameYUV, &frameFinished, rawData, bytesRemaining);  
- 
- // Was there an error?  
- if (bytesDecoded < 0)  
-             {  
- return -1;  
-             }  
- 
-             bytesRemaining -= bytesDecoded;  
-             rawData += bytesDecoded;  
- 
- // Did we finish the current frame? Then we can return  
- if (frameFinished)  
-             {  
-                 ImgConvert(  
-                     (AVPicture *)m_pFrameRGB,  
-                     PIX_FMT_BGR24,  
-                     (AVPicture *)m_pFrameYUV,  
-                     m_pCodecContext->pix_fmt,  
-                     m_pCodecContext->width,  
-                     m_pCodecContext->height);  
- 
-                 Display();  
-             }  
-         }  
-     }  
-     m_hDecodeThread = NULL;  
- return 0;  
- }  
- 
- void CRTSPPlayer::Display()  
- {  
- HDC hdc = GetDC(m_hWnd);  
- // 创建内存DC  
- HDC hMemDc = CreateCompatibleDC(hdc);       
- 
- // 创建位图  
-     BITMAPINFOHEADER bmpHdr = {0};    
-     bmpHdr.biSize = sizeof (BITMAPINFOHEADER);    
-     bmpHdr.biWidth = m_nFrameWidth;    
-     bmpHdr.biHeight = -m_nFrameHeight;    
-     bmpHdr.biPlanes = 1;    
-     bmpHdr.biBitCount = 24;    
-     bmpHdr.biCompression = BI_RGB;    
- 
- BYTE *pData = NULL;     
- HBITMAP hBitmap = CreateDIBSection (NULL, (BITMAPINFO *)&bmpHdr, DIB_RGB_COLORS, (void**)&pData, NULL, 0);    
- 
- try  
-     {  
-         memcpy(pData, m_pBufRGB, m_nFrameWidth * m_nFrameHeight * 3);  
-     }  
- catch (CMemoryException* e)  
-     {  
- 
-     }  
- 
- HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDc, hBitmap);  
- 
- #ifdef SHOW_TITLE  
- // 设置字体参数  
-     LOGFONT logfont;  
-     memset(&logfont, 0, sizeof(LOGFONT));  
-     logfont.lfHeight = 40;  
-     logfont.lfWidth = 0;      
-     logfont.lfEscapement = 0;  
-     logfont.lfOrientation = 0;  
-     logfont.lfWeight = 30;  
-     logfont.lfItalic = 0;  
-     logfont.lfUnderline = 0;  
-     logfont.lfStrikeOut = 0;  
-     logfont.lfCharSet = DEFAULT_CHARSET;     
-     logfont.lfOutPrecision= OUT_DEFAULT_PRECIS;     
-     logfont.lfClipPrecision= OUT_DEFAULT_PRECIS;     
-     logfont.lfQuality = DEFAULT_QUALITY;     
-     logfont.lfPitchAndFamily= DEFAULT_PITCH;    
- 
- // 创建字体并选入环境  
- HFONT hFont = CreateFontIndirect(&logfont);  
- HFONT hOldFont = (HFONT)SelectObject(hMemDc, hFont);  
- 
- // 设置绘图环境  
-     SetBkMode(hMemDc, TRANSPARENT);    
-     SetTextColor(hMemDc, RGB(255, 255, 0));  
- 
- // 绘制文字  
-     TextOut(hMemDc,0,0,m_strFilePath,_tcslen(m_strFilePath));  
- 
- // 恢复环境释放字体  
-     SelectObject(hMemDc, hOldFont);  
- #endif  
-     StretchBlt(    
-         hdc,    
-         m_rcWnd.left,     
-         m_rcWnd.top,     
-         m_rcWnd.right-m_rcWnd.left,     
-         m_rcWnd.bottom-m_rcWnd.top,     
-         hMemDc,    
-         0,     
-         0,     
-         m_nFrameWidth,     
-         m_nFrameHeight,     
-         SRCCOPY);    
- 
- // 恢复并释放环境      
-     SelectObject(hMemDc,hOldBitmap);    
-     DeleteObject(hBitmap);    
-     DeleteDC(hMemDc);    
- }  
- 
- // 获取播放状态  
- RTSP_PLAYSTATUS CRTSPPlayer::GetPlayStatus(void)  
- {  
- return m_nPlayStatus;  
- }  
- 
- // 设置播放状态  
- void CRTSPPlayer::SetPlayStatus(RTSP_PLAYSTATUS playStatus)  
- {  
-     m_nPlayStatus = playStatus;  
- }  

















