# Linux下：Live555+S5pV210的mfc模块硬解方案实现 - xqhrs232的专栏 - CSDN博客
2017年03月19日 00:26:31[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：300
原文地址::[http://www.tuicool.com/articles/FzYviy](http://www.tuicool.com/articles/FzYviy)
相关文章
1、基于三星S5PV210上的LinuxKernel的移植----[http://www.docin.com/p-1306923680.html](http://www.docin.com/p-1306923680.html)
本文介绍Live555作为rtsp客户端，s5pv210做媒体终端，硬解码得Nv12图像序列。
mplayer 播放yuv命令：
mplayer -demuxer rawvideo -rawvideo w=700:h=480 /sdcard/Resource/Img_700_480.yuv
编码：
1）还是和编码一样superboot，必不可少，u-boot不行
2）调用SsbSipMfcDecInit()之前一定要先调用SsbSipMfcDecGetInBuf()然后填入header(SPS/PPS/SEI)信息。
3）SsbSipMfcDecGetOutBuf()返回的status只有是MFC_GETOUTBUF_DISPLAY_DECODING或者MFC_GETOUTBUF_DISPLAY_ONLY时YVirAddr和CVirAddr才有效，如果返回MFC_GETOUTBUF_DECODING_ONLY则需要再次调用SsbSipMfcDecExe()和SsbSipMfcDecGetOutBuf()
4）YVirAddr和CVirAddr指向的数据是tiled nv12的格式（64x32 tiled)，需要转化成现行格式才能正常现实。这个mfc api已经提供了csc_tiled_to_linear()，头文件color_space_convertor.h
5）测试程序有几个问题：不支持multi-slice的帧；最后buffer里的几帧偷懒没有处理直接丢掉了；如果第一个header后的数据帧不是IDR的话，会出warning，直到第一个IDR帧为止，之前的non-IDR帧可能译码不正确或者丢帧。
下图是函数流程
![](http://img0.tuicool.com/muuENv.jpg!web)
而Live555的嵌入过程如下：
关键流程如下函数所示：
```cpp
int H264Decoder_c::SsbSipMfcDecode264Stream(char* frame,int framelen)
{
	char* ImgPtr = NULL ; int tmp_width=-1 ; int tmp_height = -1 ;
	ImgPtr = SsbSipMfcUsrDecodeAFrame(frame,framelen,m_SsbSipMfc_virbuf,m_SsbSipMfc_openHandle,&tmp_width,&tmp_height);
	if(ImgPtr!=NULL){
		u_int8_t* src[3] ;
		int linesize[3] ;
		src[0] = (u_int8_t*)ImgPtr ;  src[1] = src[0] + tmp_width*tmp_height ; src[2] = src[1] + tmp_width*tmp_height/4 ;
		linesize[0] = tmp_width ; linesize[1] = tmp_width/2 ; linesize[2] = linesize[1];
		ScaleYUVImgToRGB(tmp_width,tmp_height,src,linesize,tmp_width,tmp_height) ;
		CallImgPaint(g_cenptr,tmp_width,tmp_height);
		free(ImgPtr);
	}
	return framelen ;
}
```
```cpp
int SsbSipMfcUsrOpenMfcDecodeModule(char *spspps,int spsppslen,void **openHandle,void **virInBuf)
{
	int i=0 ; int set_conf_val ;
	void * phyInBuf;
	SSBSIP_MFC_ERROR_CODE err;
	//unsigned int buf_type = CACHE;
	unsigned int buf_type = CACHE;
	*openHandle = SsbSipMfcDecOpen(&buf_type);
	if(!*openHandle) {
		fprintf(stderr,"Error: SsbSipMfcDecOpen.\n");
		return -1 ;
	}
	printf("SsbSipMfcDecOpen succeeded.\n");
	*virInBuf = SsbSipMfcDecGetInBuf(*openHandle, &phyInBuf, MAX_DECODER_INPUT_BUFFER_SIZE);
	// MAX_DECODER_INPUT_BUFFER_SIZE was defined in SsbSipMfcApi.h, = 1024*3072 = 3 MiB
	if(!*virInBuf) {
		fprintf(stderr,"Error: SsbSipMfcDecGetInBuf.\n");
		return -1 ;
	}
	printf("SsbSipMfcDecGetInBuf succeeded.\n");
#if 1 // this might improve performance. to be confirmed
	set_conf_val = 3;
	SsbSipMfcDecSetConfig(openHandle, MFC_DEC_SETCONF_EXTRA_BUFFER_NUM, &set_conf_val);
	set_conf_val = 4;
	SsbSipMfcDecSetConfig(openHandle, MFC_DEC_SETCONF_DISPLAY_DELAY, &set_conf_val);
#endif
	memcpy(*virInBuf,spspps,spsppslen);
#if 1
	printf("Header: ");
	for(i=0;i<spsppslen;i++)
		printf("%x ",((uint8_t*)(*virInBuf))[i]);
	printf("\n");
#endif
	err = SsbSipMfcDecInit(*openHandle,H264_DEC,spsppslen);
	if(err<0) {
		fprintf(stderr,"Error: SsbSipMfcDecInit. Code %d\n",err);
		printf("Error: SsbSipMfcDecInit. Code %d\n",err);
		return -1 ;
	}
	printf("SsbSipMfcDecInit succeeded.\n");
	return 0 ;
}
void SsbSipMfcUsrShowStatus(SSBSIP_MFC_DEC_OUTPUT_INFO *oinfo,SSBSIP_MFC_DEC_OUTBUF_STATUS status)
{
	printf("Status: ");
	switch (status) {
		case MFC_GETOUTBUF_DECODING_ONLY:
			printf("Decoding Only\n");
			break;
		case MFC_GETOUTBUF_DISPLAY_DECODING:
			printf("Display and Decoding\n");
			break;
		case MFC_GETOUTBUF_DISPLAY_ONLY:
			printf("Display Only\n");
			break;
		case MFC_GETOUTBUF_DISPLAY_END:
			printf("DPB is empty\n");
			break;
		default:
			printf("Unknown Status!\n");
			break;
	}
	printf("img_width=%d, img_height=%d\n",oinfo->img_width,oinfo->img_height);
	printf("YVirAddr=%x, CVirAddr=%x\n",oinfo->YVirAddr,oinfo->CVirAddr);
	printf("buf_width=%d, buf_height=%d\n",oinfo->buf_width,oinfo->buf_height);
	printf("consumedByte=%d\n",oinfo->consumedByte);
}
char* SsbSipMfcUsrNV12Dump(SSBSIP_MFC_DEC_OUTPUT_INFO *oinfo,SSBSIP_MFC_DEC_OUTBUF_STATUS status,int *width,int *height)
{
	static int channel=1 ; int i=0 ;
	if(status==MFC_GETOUTBUF_DISPLAY_DECODING || status==MFC_GETOUTBUF_DISPLAY_ONLY)
	{
		char *NV12=NULL ;
		NV12=malloc((int)(oinfo->img_width*oinfo->img_height*1.5));
		if(!NV12) {
			fprintf(stderr,"Out of memory.\n");
		}
		// converted tiled to linear nv12 format - Y plane
		csc_tiled_to_linear(NV12,oinfo->YVirAddr,oinfo->img_width,oinfo->img_height);
		// converted tiled to linear nv12 format - C plane
		csc_tiled_to_linear(NV12+oinfo->img_width*oinfo->img_height,oinfo->CVirAddr,oinfo->img_width,oinfo->img_height/2);
		SsbSipMfcUsrNV12ToYV12(NV12,oinfo->img_width,oinfo->img_height);
		*width=oinfo->img_width; *height=oinfo->img_height;
		//Fprintf_Binary("SsbSipMfcYUV.yuv",NV12,(int)(oinfo->img_width*oinfo->img_height*1.5),"ab");
		//free(NV12);
		return NV12 ;
	}
	return NULL ;
}
char* SsbSipMfcUsrDecodeAFrame(char *framebuf,int framelen,void *virInBuf,void *openHandle,int *width,int *height)
{
	static int cnt=0 ;
	void *phyInBuf ;
	SSBSIP_MFC_ERROR_CODE err;
	SSBSIP_MFC_DEC_OUTPUT_INFO oinfo;
	SSBSIP_MFC_DEC_OUTBUF_STATUS status;
	char* ImgPtr=NULL ;
	status = MFC_GETOUTBUF_STATUS_NULL;
	//printf("length:%d\n",SsbSipMfcUsrReadOneFrame(&framebuf,framelen));
	memcpy(virInBuf,framebuf,framelen);
s5pv210_decode:   // take care of goto ,use as less as possible
	err = SsbSipMfcDecExe(openHandle, framelen);
	if(err<0) {
		fprintf(stderr,"Error: SsbSipMfcDecExe. Code %d\n",err);
		return -1 ;
	}
	else{
//		printf("SsbSipMfc Decode OK frame %d !!\n",cnt++);
		if(cnt>10000)
			cnt=0 ;
	}
	memset(&oinfo,0,sizeof(oinfo));
	status = SsbSipMfcDecGetOutBuf(openHandle,&oinfo);
	if(status==MFC_GETOUTBUF_DECODING_ONLY){
		printf("decode not done redecode\n");
		goto s5pv210_decode ;
	}
	else if((status==MFC_GETOUTBUF_DISPLAY_DECODING) ||(status==MFC_GETOUTBUF_DISPLAY_ONLY))
	{
		static int cnt =0 ;
		if((cnt++)%2){
			if(cnt>1000)
				cnt = 0 ;
			return NULL ;
		}
		//SsbSipMfcUsrShowStatus(&oinfo,status);
		ImgPtr = SsbSipMfcUsrNV12Dump(&oinfo,status,width ,height);
		return ImgPtr ;
	}
	else{
		printf("s5pv210 decode unexpected status goto decode again\n");
		goto s5pv210_decode ;
	}
	return NULL ;
}
```
同时，函数中的调用，经过优化，只需要两步，open->decode完事。
参考文章：
[Android S5PV210 fimc驱动分析 - fimc_regs.c](http://blog.csdn.net/mirkerson/article/details/8192600)
[http://blog.csdn.net/mirkerson/article/details/8192600](http://blog.csdn.net/mirkerson/article/details/8192600)
[s5pv210中MFC的编码过程](http://blog.csdn.net/mirkerson/article/details/8953469)
[http://blog.csdn.net/mirkerson/article/details/8953469](http://blog.csdn.net/mirkerson/article/details/8953469)
## s5pv210 mfc子解码
[http://bbs.csdn.net/topics/390385062](http://bbs.csdn.net/topics/390385062)
[s5pv210 HDMI 显示实现](http://blog.csdn.net/liujia2100/article/details/21788667)
[http://blog.csdn.net/liujia2100/article/details/21788667](http://blog.csdn.net/liujia2100/article/details/21788667)
NV12转RGB
[http://www.armbbs.net/forum.php?mod=viewthread&tid=18938](http://www.armbbs.net/forum.php?mod=viewthread&tid=18938)
[【成功】tiny210在Linux下的MFC库移植及硬编码测试源码](http://www.arm9home.net/read.php?tid-28647.html)
[http://www.arm9home.net/read.php?tid-28647.html](http://www.arm9home.net/read.php?tid-28647.html)
[【这次是解码】tiny210在Linux下的MFC库移植及硬解码测试源码](http://www.arm9home.net/read.php?tid-28822.html)
[http://www.arm9home.net/read.php?tid-28822.html](http://www.arm9home.net/read.php?tid-28822.html)
[TQ210 linux下NV12T转RGB显示问题](http://www.armbbs.net/forum.php?mod=viewthread&tid=18938)
[http://www.armbbs.net/forum.php?mod=viewthread&tid=18938](http://www.armbbs.net/forum.php?mod=viewthread&tid=18938)
