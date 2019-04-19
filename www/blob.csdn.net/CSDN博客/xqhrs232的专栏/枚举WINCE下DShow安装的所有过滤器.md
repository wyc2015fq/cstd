# 枚举WINCE下DShow安装的所有过滤器 - xqhrs232的专栏 - CSDN博客
2010年08月20日 10:41:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：782标签：[wince																[null																[嵌入式																[windows																[编程																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=嵌入式&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=wince&t=blog)
个人分类：[DirectShow多媒体技术](https://blog.csdn.net/xqhrs232/article/category/906987)
原文地址::Windows CE嵌入式高级编程及其实例详解: 用C++实现 作者:汪兵
OnBnClickedBtnEnumfilter()
{
 // TODO: Add your control notification handler code here
   HRESULT   hr;
   IFilterMapper   *pMapper=NULL;
   IEnumRegFilters  *pEnum=NULL;
   REGFILTER   *pRegFilter=NULL;
   ULONG   cFetched=0;
     CoInitialize(NULL);
     hr=CoCreateInstance(CLSID_FilterMapper,NULL,CLSCTX_INPROC,IID_IFilterMapper,(void**)&pMapper);
     if(FAILED(hr))
     {
      return;
     }
     hr=pMapper->EnumMatchingFilters(&pEnum,0,FALSE,GUID_NULL,GUID_NULL,FALSE,FALSE,GUID_NULL,GUID_NULL);
  if(FAILED(hr))
  {
   return;
  }
  while(pEnum->Next(1,&pRegFilter,&cFetched)==S_OK)
  {
         for(int i=0;i<cFetched;i++)
   {
    TRACE(L"%s/n",(pRegFilter+i)->Name);
   }
            CoTaskMemFree(pRegFilter);
  }
    pMapper->Release();
 pEnum->Release();
 CoUninitialize();
}
