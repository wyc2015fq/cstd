# DShow下获取Pin支持的媒体类型 - xqhrs232的专栏 - CSDN博客
2010年08月20日 14:01:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：987标签：[stream																[audio																[video																[null																[2010](https://so.csdn.net/so/search/s.do?q=2010&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=audio&t=blog)](https://so.csdn.net/so/search/s.do?q=stream&t=blog)
个人分类：[DirectShow多媒体技术](https://blog.csdn.net/xqhrs232/article/category/906987)
原文地址：：[http://blog.csdn.net/markman101/archive/2010/06/21/5684784.aspx](http://blog.csdn.net/markman101/archive/2010/06/21/5684784.aspx)
//获取Pin媒体类型的函数: 
typedef  enum
{
PIN_UNKNOWN=0,
PIN_VIDEO,
PIN_AUDIO,
PIN_INTERLEAVED,
PIN_STREAM,
}ENUM_PIN_MEDIA;
ENUM_PIN_MEDIA   GetPinMediaType(IPin   *apPin) 
{ 
 ENUM_PIN_MEDIA   lPinMedia   =   PIN_UNKNOWN; 
 if   (apPin) 
 { 
  IEnumMediaTypes   *lpEnumTypes   =   NULL; 
  if   (SUCCEEDED(apPin-> EnumMediaTypes(&lpEnumTypes))) 
  { 
   lpEnumTypes-> Reset(); 
   AM_MEDIA_TYPE   *lpMediaType   =   0; 
   ULONG   lulFetchCount   =   0; 
   if   (SUCCEEDED(lpEnumTypes-> Next(1,   &lpMediaType,   &lulFetchCount))   &&   lulFetchCount)
   { 
    if   (lpMediaType) 
    { 
     if   (lpMediaType-> majortype   ==   MEDIATYPE_Video) 
     { 
      lPinMedia   =   PIN_VIDEO; 
     } 
     else   if   (lpMediaType-> majortype   ==   MEDIATYPE_Audio) 
     { 
      lPinMedia   =   PIN_AUDIO; 
     } 
     else   if   (lpMediaType-> majortype   ==   MEDIATYPE_Interleaved) 
     { 
      lPinMedia   =   PIN_INTERLEAVED; 
     } 
     else   if   (lpMediaType-> majortype   ==   MEDIATYPE_Stream) 
     { 
      lPinMedia   =   PIN_STREAM; 
     } 
     DeleteMediaType(lpMediaType); 
     lpMediaType   =   NULL; 
    } 
   } 
   lpEnumTypes-> Release(); 
  } 
 } 
 return   lPinMedia; 
}
