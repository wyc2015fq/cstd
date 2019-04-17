# 利用ffmpeg解码h264流的代码 - DoubleLi - 博客园






这里也直接给出代码：

h264dec.h:





**[cpp]**[view plain](http://blog.csdn.net/wchm_seu/article/details/7736650)[copy](http://blog.csdn.net/wchm_seu/article/details/7736650)



- #pragma once  
- #include "tdll.h"  
- #include "avcodec.h"  
- #include "postprocess.h"  
- //#include "EMVideoCodec.h"  
- 
- class h264dec /*: public IH264Decoder*/  
- {  
- public:  
- virtual bool InitH264Deocder(int width,int height);  
- virtual bool H264Decode(unsigned char * inbuf, const int & inlen,unsigned char * outbuf,int & outlen);  
- virtual void StopH264Decoder();  
- virtual void ReleaseConnection();  
- 
- public:  
-     h264dec(void);  
- virtual ~h264dec(void);  
- private:  
-     Tdll *dll;  
- 
- bool LoadDllFun();  
- void (*avcodec_init)(void);  
- void (*avcodec_register_all)(void);  
-     AVCodecContext* (*avcodec_alloc_context)(void);  
-     AVFrame* (*avcodec_alloc_frame)(void);  
-     AVCodec *(*avcodec_find_decoder)(enum CodecID id);  
- int (*avcodec_decode_video)(AVCodecContext *avctx, AVFrame *picture,int *got_picture_ptr,  
-                                 uint8_t *buf, int buf_size);  
- int  (*avcodec_open)(AVCodecContext *avctx, AVCodec *codec);  
- int  (*avcodec_close)(AVCodecContext *avctx);  
- void (*av_free)(void *ptr);       
- 
- bool InitPostproc(int w,int h);  
- void ClosePostproc();  
-     pp_context_t *(*pp_get_context)(int width, int height, int flags);  
- void (*pp_free_context)(pp_context_t *ppContext);  
- void (*pp_free_mode)(pp_mode_t *mode);  
-     pp_mode_t *(*pp_get_mode_by_name_and_quality)(char *name, int quality);  
- void  (*pp_postprocess)(uint8_t * src[3], int srcStride[3],  
-                  uint8_t * dst[3], int dstStride[3],  
- int horizontalSize, int verticalSize,  
-                  QP_STORE_T *QP_store,  int QP_stride,  
-          pp_mode_t *mode, pp_context_t *ppContext, int pict_type);  
- private:  
-     AVCodec         *pdec;  
-     AVCodecContext  *pdecContext;  
-     AVFrame         *pdecFrame;  
- int             m_width;  
- int             m_height;  
- 
-     Tdll* prodll;  
-     pp_context_t *pp_context;  
-     pp_mode_t    *pp_mode;  
- };  



h264dec.cpp:







**[cpp]**[view plain](http://blog.csdn.net/wchm_seu/article/details/7736650)[copy](http://blog.csdn.net/wchm_seu/article/details/7736650)



- #include "StdAfx.h"  
- #include ".\h264dec.h"  
- 
- h264dec::h264dec(void)  
- :dll(NULL)  
- ,pdec(NULL)  
- ,pdecContext(NULL)  
- ,pdecFrame(NULL)  
- ,pp_context(NULL)  
- ,pp_mode(NULL)  
- ,prodll(NULL)  
- {  
- }  
- 
- h264dec::~h264dec(void)  
- {  
- }  
- 
- bool h264dec::LoadDllFun()  
- {  
-     dll=new Tdll(L"libavcodec.dll");  
-     dll->loadFunction((void**)&avcodec_init,"avcodec_init");  
-     dll->loadFunction((void**)&avcodec_register_all,"avcodec_register_all");  
-     dll->loadFunction((void**)&avcodec_alloc_context,"avcodec_alloc_context");  
-     dll->loadFunction((void**)&avcodec_alloc_frame,"avcodec_alloc_frame");  
-     dll->loadFunction((void**)&avcodec_find_decoder,"avcodec_find_decoder");  
-     dll->loadFunction((void**)&avcodec_open,"avcodec_open");   
-     dll->loadFunction((void**)&avcodec_decode_video,"avcodec_decode_video");  
-     dll->loadFunction((void**)&avcodec_close,"avcodec_close");  
-     dll->loadFunction((void**)&av_free,"av_free");  
- if (!dll->ok)  
- return false;  
- 
-     prodll = new Tdll(L"postproc.dll");  
-     prodll->loadFunction((void**)&pp_get_context,"pp_get_context");  
-     prodll->loadFunction((void**)&pp_free_context,"pp_free_context");  
-     prodll->loadFunction((void**)&pp_free_mode,"pp_free_mode");  
-     prodll->loadFunction((void**)&pp_get_mode_by_name_and_quality,"pp_get_mode_by_name_and_quality");  
-     prodll->loadFunction((void**)&pp_postprocess,"pp_postprocess");  
- if(!prodll->ok)  
- return false;  
- 
-     avcodec_init();  
-     avcodec_register_all();  
- return true;  
- }  
- 
- bool h264dec::InitH264Deocder(int width,int height)  
- {  
- if(!LoadDllFun())  
- return false;  
- if(!InitPostproc(width,height))  
- return false;  
- 
-     m_width=width;  
-     m_height=height;  
-     pdec = avcodec_find_decoder(CODEC_ID_H264);  
- if (pdec == NULL )    
- return false;  
- 
-     pdecContext = avcodec_alloc_context();  
-     pdecFrame = avcodec_alloc_frame();  
- 
-     pdecContext->width  = width;  
-     pdecContext->height = height;  
-     pdecContext->pix_fmt = PIX_FMT_YUV420P;  
- /* open it */  
- if (avcodec_open(pdecContext, pdec) < 0)   
-     {  
- return false;  
-     }  
- return true;  
- }  
- 
- bool h264dec::InitPostproc(int w,int h)  
- {  
- int i_flags = 0;  
-     i_flags |= PP_CPU_CAPS_MMX | PP_CPU_CAPS_MMX2 | PP_FORMAT_420;  
-     pp_context = pp_get_context( w, h, i_flags );  
- if(!pp_context)  
- return false;  
-     pp_mode = pp_get_mode_by_name_and_quality( "default", 6 );  
- if(!pp_mode)  
- return false;  
- return true;  
- }  
- 
- bool h264dec::H264Decode(unsigned char * inbuf, const int & inlen,unsigned char * outbuf,int & outlen)  
- {  
- int got_frame;  
- BYTE* showImage[3];  
- int showheight[3],showLx[3];  
- 
- int len;  
-     len=avcodec_decode_video(pdecContext, pdecFrame, &got_frame, inbuf, inlen);  
- if(len < 0)  
- return false;  
- 
- if(got_frame)  
-     {  
-         showImage[0]=outbuf;  
-         showImage[1]=showImage[0]+m_width*m_height;  
-         showImage[2]=showImage[1]+m_width*m_height/4;  
-         showLx[0]=m_width;showLx[1]=m_width>>1;showLx[2]=m_width>>1;  
-         showheight[0]=m_height;showheight[1]=m_height>>1;showheight[2]=m_height>>1;  
-         pp_postprocess(pdecFrame->data,pdecFrame->linesize,showImage,showLx,m_width,m_height,pdecFrame->qscale_table,  
-             pdecFrame->qstride,pp_mode,pp_context,pdecFrame->pict_type);  
- //GetImage( pdecFrame->data,  
- //          showImage,  
- //          pdecFrame->linesize,  
- //          showLx,  
- //          showheight);  
-         outlen=m_width*m_height*3/2;  
-     }  
- else  
-     {  
-         outlen = 0;  
-     }  
- 
- return true;  
- }  
- 
- void h264dec::StopH264Decoder()  
- {  
- if (pdecContext != NULL)   
-     {       
-         avcodec_close(pdecContext);  
-         av_free( pdecContext );  
-         pdecContext = NULL;  
- if(pdecFrame){  
-             av_free(pdecFrame);  
-             pdecFrame = NULL;  
-         }  
-     }  
- if(dll){  
- delete dll;  
-         dll=0;  
-     }  
- 
-     ClosePostproc();  
- }  
- 
- void h264dec::ClosePostproc()  
- {  
- if(pp_mode){  
-         pp_free_mode( pp_mode );  
-         pp_mode=0;  
-     }  
- if(pp_context){  
-         pp_free_context(pp_context);  
-         pp_context=0;  
-     }  
- if(prodll){  
- delete prodll;  
-         prodll=0;  
-     }  
- }  
- 
- void h264dec::ReleaseConnection()  
- {  
- delete this;  
- }  



tdll.h:







**[cpp]**[view plain](http://blog.csdn.net/wchm_seu/article/details/7736650)[copy](http://blog.csdn.net/wchm_seu/article/details/7736650)



- #ifndef _TDLL_  
- #define _TDLL_  
- 
- class Tdll  
- {  
- private:  
- HMODULE hdll;  
- void loadDll(const char *dllName);  
- public:  
- bool ok;  
-     Tdll(const TCHAR *dllName1)  
-     {  
-         hdll=LoadLibrary(dllName1);  
- if (!hdll)  
-         {  
-             hdll=NULL;  
-         }  
-         ok=(hdll!=NULL);  
-     };  
-     ~Tdll()  
-     {  
- if (hdll)  
-             FreeLibrary(hdll);  
-     }  
- void loadFunction(void **fnc,const char *name)  
-     {  
-         *fnc=GetProcAddress(hdll,name);  
-         ok&=(*fnc!=NULL);  
-     };  
- };    
- 
- #endif  



main.cpp:







**[cpp]**[view plain](http://blog.csdn.net/wchm_seu/article/details/7736650)[copy](http://blog.csdn.net/wchm_seu/article/details/7736650)



- #include "stdafx.h"  
- #include "h264dec.h"  
- #include "postprocess.h"  
- 
- #define INBUF_SIZE 100 * 1024;  
- 
- 
- static int FindStartCode (unsigned char *Buf, int zeros_in_startcode)  
- {  
- int info;  
- int i;  
- 
-     info = 1;  
- for (i = 0; i < zeros_in_startcode; i++)  
-     {  
- if(Buf[i] != 0)  
-             info = 0;  
-     }  
- 
- if(Buf[i] != 1)  
-         info = 0;  
- return info;  
- }  
- 
- static bool Check_StartCode(unsigned char *Buf, int pos)  
- {  
- int info3 = 0;  
- 
-     info3 = FindStartCode(&Buf[pos-4], 3);  
- return info3 == 1;  
- 
- }  
- 
- static int getNextNal(FILE* inpf, unsigned char* Buf)  
- {  
- int pos = 0;  
- int StartCodeFound = 0;  
- int info2 = 0;  
- int info3 = 0;  
- 
- int nCount = 0;  
- while(!feof(inpf) && ++nCount <= 4)  
-     {  
-         Buf[pos++]=fgetc(inpf);  
-     }  
- 
- if(!Check_StartCode(Buf, pos))  
-     {  
- return 0;  
-     }  
- 
- 
- while(!feof(inpf) && (Buf[pos++]=fgetc(inpf))==0);  
- 
- while (!StartCodeFound)  
-     {  
- if (feof (inpf))  
-         {  
- //          return -1;  
- return pos-1;  
-         }  
-         Buf[pos++] = fgetc (inpf);  
- 
-         StartCodeFound = Check_StartCode(Buf, pos);  
-     }  
- 
-     fseek (inpf, -4, SEEK_CUR);  
- return pos - 4;  
- }  
- 
- int main(int argc, char* argv[])  
- {  
- if (argc != 5)  
-     {  
-         printf("please input: PP_Demo.exe filename1[input] Width Height filename2[output]\n");  
-     }  
- 
- //params set  
-     unsigned short usWidth = atoi(argv[2]);  
-     unsigned short usHeight = atoi(argv[3]);  
- 
- //create dec&pp  
-     h264dec *pdec = new h264dec;  
- if(!pdec->InitH264Deocder(usWidth, usHeight))  
-     {  
- return false;  
-     }  
- 
- 
- 
-     unsigned char *p_In_Frame = new unsigned char[usWidth * usHeight * 3/2];  
-     unsigned char *p_Out_Frame = new unsigned char[usWidth * usHeight * 3/2];  
- FILE* ifp = fopen(argv[1],"rb");  
- FILE* ofp = fopen(argv[4],"wb");  
- 
- bool b_continue = true;  
- int nReadUnit = usWidth * usHeight * 3/2;  
- while(!feof(ifp))  
-     {  
- int nCount = getNextNal(ifp, p_In_Frame);  
- 
- if(nCount == 0)  
-         {  
- continue;  
-         }  
- 
-         unsigned char *ptr = p_In_Frame;  
- int n_Outlen = 0;  
-         pdec->H264Decode(ptr, nCount, p_Out_Frame, n_Outlen);  
- 
- if(n_Outlen > 0)  
-         {  
-             fwrite(p_Out_Frame, 1, n_Outlen, ofp);  
-         }  
-     }  
- 
- 
- //realse  
- delete []p_In_Frame;  
- delete []p_Out_Frame;  
-     pdec->StopH264Decoder();  
-     pdec->ReleaseConnection();  
-     fclose(ifp);  
-     fclose(ofp);  
- 
- return 0;  
- }  
- 










