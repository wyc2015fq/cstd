# 最简单的基于librtmp的示例：接收（RTMP保存为FLV） - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2014年12月25日 00:50:26[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：27995标签：[librtmp																[rtmp																[flv																[流媒体](https://so.csdn.net/so/search/s.do?q=流媒体&t=blog)
个人分类：[libRTMP																[我的开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1843731)](https://blog.csdn.net/leixiaohua1020/article/category/1362941)





=====================================================


最简单的基于libRTMP的示例系列文章列表：

[最简单的基于librtmp的示例：接收（RTMP保存为FLV）](http://blog.csdn.net/leixiaohua1020/article/details/42104893)


[最简单的基于librtmp的示例：发布（FLV通过RTMP发布）](http://blog.csdn.net/leixiaohua1020/article/details/42104945)


[最简单的基于librtmp的示例：发布H.264（H.264通过RTMP发布）](http://blog.csdn.net/leixiaohua1020/article/details/42105049)


=====================================================


本文记录一个基于libRTMP的接收流媒体的程序：Simplest libRTMP Receive。该程序可以将RTMP流保存成本地FLV文件。实际上本文记录的程序就是一个“精简”过的RTMPDump。RTMPDump功能比较多，因而其代码比较复杂导致很多初学者不知从何下手。而本文记录的这个程序只保留了RTMPDump中最核心的函数，更加方便新手入门学习libRTMP。

![](https://img-blog.csdn.net/20150110200148145)

## 流程图
使用librtmp接收RTMP流的函数执行流程图如下图所示。

![](https://img-blog.csdn.net/20141223170153132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

流程图中关键函数的作用如下所列：
InitSockets()：初始化Socket
RTMP_Alloc()：为结构体“RTMP”分配内存。
RTMP_Init()：初始化结构体“RTMP”中的成员变量。
RTMP_SetupURL()：设置输入的RTMP连接的URL。
RTMP_Connect()：建立RTMP连接，创建一个RTMP协议规范中的NetConnection。
RTMP_ConnectStream()：创建一个RTMP协议规范中的NetStream。
RTMP_Read()：从服务器读取数据。
RTMP_Close()：关闭RTMP连接。
RTMP_Free()：释放结构体“RTMP”。
CleanupSockets()：关闭Socket。

其中NetStream和NetConnection是RTMP协议规范中的两个逻辑结构。NetStream建立在NetConnection之上。一个NetConnection可以包含多个NetStream。它们之间的关系如下图所示。

![](https://img-blog.csdn.net/20141223170215265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




## 源代码

```cpp
/**
 * Simplest Librtmp Receive
 *
 * 雷霄骅，张晖
 * leixiaohua1020@126.com
 * zhanghuicuc@gmail.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序用于接收RTMP流媒体并在本地保存成FLV格式的文件。
 * This program can receive rtmp live stream and save it as local flv file.
 */
#include <stdio.h>
#include "librtmp/rtmp_sys.h"
#include "librtmp/log.h"

int InitSockets()
{
	WORD version;
	WSADATA wsaData;
	version = MAKEWORD(1, 1);
	return (WSAStartup(version, &wsaData) == 0);
}

void CleanupSockets()
{
	WSACleanup();
}

int main(int argc, char* argv[])
{
	InitSockets();
	
	double duration=-1;
	int nRead;
	//is live stream ?
	bool bLiveStream=true;				
	
	
	int bufsize=1024*1024*10;			
	char *buf=(char*)malloc(bufsize);
	memset(buf,0,bufsize);
	long countbufsize=0;
	
	FILE *fp=fopen("receive.flv","wb");
	if (!fp){
		RTMP_LogPrintf("Open File Error.\n");
		CleanupSockets();
		return -1;
	}
	
	/* set log level */
	//RTMP_LogLevel loglvl=RTMP_LOGDEBUG;
	//RTMP_LogSetLevel(loglvl);

	RTMP *rtmp=RTMP_Alloc();
	RTMP_Init(rtmp);
	//set connection timeout,default 30s
	rtmp->Link.timeout=10;	
	// HKS's live URL
	if(!RTMP_SetupURL(rtmp,"rtmp://live.hkstv.hk.lxdns.com/live/hks"))
	{
		RTMP_Log(RTMP_LOGERROR,"SetupURL Err\n");
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}
	if (bLiveStream){
		rtmp->Link.lFlags|=RTMP_LF_LIVE;
	}
	
	//1hour
	RTMP_SetBufferMS(rtmp, 3600*1000);		
	
	if(!RTMP_Connect(rtmp,NULL)){
		RTMP_Log(RTMP_LOGERROR,"Connect Err\n");
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}

	if(!RTMP_ConnectStream(rtmp,0)){
		RTMP_Log(RTMP_LOGERROR,"ConnectStream Err\n");
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		CleanupSockets();
		return -1;
	}

	while(nRead=RTMP_Read(rtmp,buf,bufsize)){
		fwrite(buf,1,nRead,fp);

		countbufsize+=nRead;
		RTMP_LogPrintf("Receive: %5dByte, Total: %5.2fkB\n",nRead,countbufsize*1.0/1024);
	}

	if(fp)
		fclose(fp);

	if(buf){
		free(buf);
	}

	if(rtmp){
		RTMP_Close(rtmp);
		RTMP_Free(rtmp);
		CleanupSockets();
		rtmp=NULL;
	}	
	return 0;
}
```



## 运行结果 

程序运行后，会将URL为“rtmp://live.hkstv.hk.lxdns.com/live/hks”的直播流（实际上是香港卫视）在本地保存为“receive.flv”。保存后的文件使用播放器就可以观看。




## 下载

**Simplest LibRTMP Example**


**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestlibrtmpexample/](https://sourceforge.net/projects/simplestlibrtmpexample/)

Github：[https://github.com/leixiaohua1020/simplest_librtmp_example](https://github.com/leixiaohua1020/simplest_librtmp_example)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_librtmp_example](http://git.oschina.net/leixiaohua1020/simplest_librtmp_example)



CSDN下载：[http://download.csdn.net/detail/leixiaohua1020/8291757](http://download.csdn.net/detail/leixiaohua1020/8291757)

本工程包含了LibRTMP的使用示例，包含如下子工程：
simplest_librtmp_receive: 接收RTMP流媒体并在本地保存成FLV格式的文件。
simplest_librtmp_send_flv: 将FLV格式的视音频文件使用RTMP推送至RTMP流媒体服务器。
simplest_librtmp_send264: 将内存中的H.264数据推送至RTMP流媒体服务器。](https://so.csdn.net/so/search/s.do?q=flv&t=blog)](https://so.csdn.net/so/search/s.do?q=rtmp&t=blog)](https://so.csdn.net/so/search/s.do?q=librtmp&t=blog)




