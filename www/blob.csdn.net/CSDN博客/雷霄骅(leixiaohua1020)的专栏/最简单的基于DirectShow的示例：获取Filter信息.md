# 最简单的基于DirectShow的示例：获取Filter信息 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2015年01月12日 21:03:08[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：15661








=====================================================


最简单的基于DirectShow的示例文章列表：

[最简单的基于DirectShow的示例：视频播放器](http://blog.csdn.net/leixiaohua1020/article/details/42372419)

[最简单的基于DirectShow的示例：视频播放器图形界面版](http://blog.csdn.net/leixiaohua1020/article/details/42372631)


[最简单的基于DirectShow的示例：视频播放器自定义版](http://blog.csdn.net/leixiaohua1020/article/details/42394509)


[最简单的基于DirectShow的示例：获取Filter信息](http://blog.csdn.net/leixiaohua1020/article/details/42649379)


=====================================================


本文记录一个获取DirectShow的Filter信息的程序。该程序可以遍历当前系统中的DirectShow Filter并且将它们的信息打印到控制台中。通过本程序可以搞清楚DirectShow中Filter，Pin，MediaType等几个关键结构之间的关系，以及它们的信息的获取方法。

![](https://img-blog.csdn.net/20150112205346925)


## 流程图

该程序的流程图如下所示。由于该图的尺寸比较大，在页面中显示不下，所以在相册中上传了一份：

[![](https://img-blog.csdn.net/20150112205415785?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://img-my.csdn.net/uploads/201501/12/1421034450_5485.jpg)



[查看大图](https://my.csdn.net/leixiaohua1020/album/detail/1802307)

### 接口


该流程图中涉及到以下接口：

> ICreateDevEnum *pSysDevEnum：设备列举接口。
IEnumMoniker *pEnumCat：Moniker（别名）枚举接口。
IMoniker *pMoniker：Moniker（别名）接口。
IPropertyBag *pPropBag：存储属性值的接口。
IBaseFilter *pFilter：Filter接口。
IEnumPins * pinEnum：Filter枚举接口。
IPin * pin： Pin接口。
PIN_INFO pinInfo：存储Pin的信息的结构体。
IEnumMediaTypes *mtEnum：MediaType枚举接口。

AM_MEDIA_TYPE   *mt：描述媒体类型的结构体。


### 流程图

该流程图中涉及到以下函数：

【初始化】

> 
CoInitialize()：初始化COM运行环境。

CoCreateInstance(…,pSysDevEnum)：用指定的类标识符创建一个Com对象。在该示例中类标识符为“IID_ICreateDevEnum”，用于创建ICreateDevEnum。

【Filter的枚举】

> pSysDevEnum->CreateClassEnumerator(…,pEnumCat)：通过ICreateDevEnum查询IEnumMoniker枚举接口，枚举指定类型目录下的设备Moniker（别名）。
pEnumCat->Next(…,pMoniker)：通过IEnumMoniker查询下一个IMoniker接口。
pMoniker->BindToStorage(…,pPropBag)：通过IMoniker查询IPropertyBag接口（用于获取Filter信息）。
pPropBag->Read("FriendlyName")：通过IPropertyBag获取“FriendlyName”属性的值。
pMoniker->BindToObject(…,pFilter)：通过IMoniker查询IBaseFilter接口（用于获取Filter，注意和BindToStorage()区别）。

【Pin的枚举】

> pFilter->EnumPins(pinEnum)：通过IBaseFilter查询IEnumPins枚举接口。
pinEnum->Next(…,pin)：通过IEnumPins查询下一个IPin接口。
pin->QueryPinInfo(PinInfo)：通过IPin获取Pin的信息。

【MediaType的枚举】

> pin->EnumMediaTypes(&mtEnum)：通过IPin查询IEnumMediaTypes枚举接口。
mtEnum->Next(…, &mt)：通过IEnumMediaTypes查询下一个AM_MEDIA_TYPE。
GuidToString(mt->majortype)：把AM_MEDIA_TYPE的GUID转换成字符串（方便输出）。


【释放】

> 
CoUninitialize()：释放CoInitialize()初始化的COM运行环境。




再附上一张代码中涉及到的接口之间的关系：

![](https://img-blog.csdn.net/20150112205836312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以看出从上到下他们之间顺序的排列如下所示：

**ICreateDevEnum-->IEnumMoniker-->IMoniker-->IBaseFilter-->IEnumPins-->IPin-->IEnumMediaTypes-->AM_MEDIA_TYPE**




## 源代码

```cpp
/**
 * 最简单的Directshow信息显示例子
 * Simplest DirectShow Info
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序是一段获取DirectShow滤镜信息的代码。通过本代码可以获得
 * DirectShow滤镜信息。适合初学者学习DirectShow。
 * 
 * This code can be used to get Directshow Filter's information.
 * Suitable for the beginner of DirectShow.
 */

#include "stdafx.h"
#include <dshow.h>
#include <atlconv.h>

#define OUTPUT_PIN       1
#define OUTPUT_MEDIATYPE 1


char* GuidToString(const GUID &guid)
{
	int buf_len=64;
	char *buf =(char *)malloc(buf_len);
	_snprintf(
		buf,
		buf_len,
		"{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1],
		guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5],
		guid.Data4[6], guid.Data4[7]);
	//printf("%s\n",buf);
	return buf;
}


int _tmain(int argc, _TCHAR* argv[])
{
	USES_CONVERSION;
	// Init COM
	HRESULT hr=NULL;
	hr= CoInitialize(NULL);
	if (FAILED(hr)){
		printf("Error, Can not init COM.");
		return -1;
	}
	printf("===============Directshow Filters ===============\n");
	ICreateDevEnum *pSysDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if (FAILED(hr)){
		return hr;
	}

	IEnumMoniker *pEnumCat = NULL;
	//Category	
	/************************************************************************
	Friendly Name	                      CLSID
	-------------------------------------------------------------------------
	Audio Capture Sources	              CLSID_AudioInputDeviceCategory
	Audio Compressors	                  CLSID_AudioCompressorCategory
	Audio Renderers	                      CLSID_AudioRendererCategory
	Device Control Filters	              CLSID_DeviceControlCategory
	DirectShow Filters	                  CLSID_LegacyAmFilterCategory
	External Renderers	                  CLSID_TransmitCategory
	Midi Renderers	                      CLSID_MidiRendererCategory
	Video Capture Sources	              CLSID_VideoInputDeviceCategory
	Video Compressors	                  CLSID_VideoCompressorCategory
	WDM Stream Decompression Devices	  CLSID_DVDHWDecodersCategory
	WDM Streaming Capture Devices	      AM_KSCATEGORY_CAPTURE
	WDM Streaming Crossbar Devices	      AM_KSCATEGORY_CROSSBAR
	WDM Streaming Rendering Devices	      AM_KSCATEGORY_RENDER
	WDM Streaming Tee/Splitter Devices	  AM_KSCATEGORY_SPLITTER
	WDM Streaming TV Audio Devices	      AM_KSCATEGORY_TVAUDIO
	WDM Streaming TV Tuner Devices	      AM_KSCATEGORY_TVTUNER
	WDM Streaming VBI Codecs	          AM_KSCATEGORY_VBICODEC 
	************************************************************************/
	hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoCompressorCategory, &pEnumCat, 0);
	//hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnumCat, 0);
	//hr = pSysDevEnum->CreateClassEnumerator(CLSID_AudioCompressorCategory, &pEnumCat, 0);
	//hr = pSysDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEnumCat, 0);
	//hr = pSysDevEnum->CreateClassEnumerator(CLSID_MediaMultiplexerCategory, &pEnumCat, 0);
	//hr = pSysDevEnum->CreateClassEnumerator(CLSID_LegacyAmFilterCategory, &pEnumCat, 0);

	if (hr != S_OK) {
		pSysDevEnum->Release();
		return -1;
	}
	
	IMoniker *pMoniker = NULL;
	ULONG monikerFetched;
	//Filter
	while(pEnumCat->Next(1, &pMoniker, &monikerFetched) == S_OK){
		IPropertyBag *pPropBag;
		VARIANT varName;
		IBaseFilter *pFilter;
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,(void **)&pPropBag);
		if (FAILED(hr)){
			pMoniker->Release();
			continue;
		}
		VariantInit(&varName);
		hr = pPropBag->Read(L"FriendlyName", &varName, 0);
		//"FriendlyName": The name of the device.
		//"Description": A description of the device.
		//Filter Info================
		printf("[%s]\n",W2A(varName.bstrVal));
		VariantClear(&varName);
		//========================
#if OUTPUT_PIN
		hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)&pFilter);
		if (!pFilter){
			continue;
		}

		IEnumPins * pinEnum = NULL;
		IPin * pin = NULL;
		ULONG pinFetched = 0;
		if (FAILED(pFilter->EnumPins(&pinEnum))){
			pinEnum->Release();
			continue;	
		}
		pinEnum->Reset();
		//Pin Info
		while (SUCCEEDED(pinEnum->Next(1, &pin, &pinFetched)) && pinFetched){
			if (!pin){
				continue;
			}
			PIN_INFO pinInfo;
			if (FAILED(pin->QueryPinInfo(&pinInfo))){
				continue;
			}
			printf("\t[Pin] ");
				switch(pinInfo.dir){
				case PINDIR_INPUT:printf("Dir:Input  \t");break;
				case PINDIR_OUTPUT:printf("Dir:Output \t");break;
				default:printf("Dir:Unknown\n");break;
			}
			printf("Name:%s\n",W2A(pinInfo.achName));
					
			//MediaType
#if OUTPUT_MEDIATYPE
			IEnumMediaTypes *mtEnum=NULL;
			AM_MEDIA_TYPE   *mt=NULL;
			if( FAILED( pin->EnumMediaTypes( &mtEnum )) )
				break;
			mtEnum->Reset();
			
			ULONG mtFetched = 0;

			while (SUCCEEDED(mtEnum->Next(1, &mt, &mtFetched)) && mtFetched){

				printf("\t\t[MediaType]\n");
				//Video
				char *MEDIATYPE_Video_str=GuidToString(MEDIATYPE_Video);
				//Audio
				char *MEDIATYPE_Audio_str=GuidToString(MEDIATYPE_Audio);
				//Stream
				char *MEDIATYPE_Stream_str=GuidToString(MEDIATYPE_Stream);
				//Majortype
				char *majortype_str=GuidToString(mt->majortype);
				//Subtype
				char *subtype_str=GuidToString(mt->subtype);

				printf("\t\t  Majortype:");
				if(strcmp(majortype_str,MEDIATYPE_Video_str)==0){
					printf("Video\n");
				}else if(strcmp(majortype_str,MEDIATYPE_Audio_str)==0){
					printf("Audio\n");
				}else if(strcmp(majortype_str,MEDIATYPE_Stream_str)==0){
					printf("Stream\n");
				}else{
					printf("Other\n");
				}
				printf("\t\t  Subtype GUID:%s",subtype_str);

				free(MEDIATYPE_Video_str);
				free(MEDIATYPE_Audio_str);
				free(MEDIATYPE_Stream_str);
				free(subtype_str);
				free(majortype_str);
				printf("\n");

			}
#endif
			pin->Release();
				
		}
		pinEnum->Release();

		pFilter->Release();
#endif
		
		pPropBag->Release();
		pMoniker->Release();
	}
	pEnumCat->Release();
	pSysDevEnum->Release();
	printf("=================================================\n");
	CoUninitialize();
	return 0;
}
```



## 运行结果

程序运行的结果如下图所示。从图中可以看出，程序打印出了系统中DirectShow的Filter信息。每个Filter的信息中包含了它的Pin的信息。每个Pin中又包含了Pin中的MediaType信息。

![](https://img-blog.csdn.net/20150112210046035?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


可以通过定义在代码最前面宏 控制输出的Filter信息的类型。定义成“0”的话则不会输出该类的信息。如下所示。



```cpp
#define OUTPUT_PIN       1
#define OUTPUT_MEDIATYPE 1
```




## 下载

**Simplest DirectShow Example**[](https://sourceforge.net/projects/simplestdirectshowexample/)





**项目主页**

SourceForge：[https://sourceforge.net/projects/simplestdirectshowexample/](https://sourceforge.net/projects/simplestdirectshowexample/)

Github：[https://github.com/leixiaohua1020/simplest_directshow_example](https://github.com/leixiaohua1020/simplest_directshow_example)

开源中国：[http://git.oschina.net/leixiaohua1020/simplest_directshow_example](http://git.oschina.net/leixiaohua1020/simplest_directshow_example)


CDSN下载地址：[http://download.csdn.net/detail/leixiaohua1020/8348163](http://download.csdn.net/detail/leixiaohua1020/8348163)

本程序包含了DirectShow开发的示例程序。适合DirectShow初学者进行学习。
它包含了以下几个子程序：
simplest_directshow_player: 最简单的基于DirectShow的视频播放器。
simplest_directshow_player_custom: 最简单的基于DirectShow的视频播放器（Custom）。
playerGUI: 最简单的基于DirectShow的播放器-图形界面版。
simplest_directshow_info: 最简单的Directshow信息显示例子。
simplest_directshow_filter: 目前还未完成。



