# 枚举过滤器的所有PIN接口 - xqhrs232的专栏 - CSDN博客
2010年08月20日 11:56:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：927标签：[codec																[input																[output																[video																[null](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=video&t=blog)](https://so.csdn.net/so/search/s.do?q=output&t=blog)](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=codec&t=blog)
个人分类：[DirectShow多媒体技术](https://blog.csdn.net/xqhrs232/article/category/906987)
//IGraphBuilder *pGB = NULL;
      IEnumPins     *pEnumPins ;
      IBaseFilter   *pVR ;
      IPin          *pPin;
      ULONG          ul ;
      PIN_DIRECTION  pd ;
      int  j;
         hr = pGB->FindFilterByName(L"MPEG Video Codec", &pVR) ;
  hr = pVR->EnumPins(&pEnumPins) ;
  while(pEnumPins->Next(1, &pPin, &ul)==S_OK)
  {
         for(j=0;j<ul;j++)
   {
            pPin->QueryDirection(&pd) ;
   if(pd==PINDIR_INPUT)
                RETAILMSG(1, (TEXT ("the pin is input type/r/n")));
   else
    RETAILMSG(1, (TEXT ("the pin is output type/r/n")));
   }
  }
  pEnumPins->Release() ;
