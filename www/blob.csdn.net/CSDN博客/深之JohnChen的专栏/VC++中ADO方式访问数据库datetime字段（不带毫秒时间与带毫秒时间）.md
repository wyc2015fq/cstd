# VC++中ADO方式访问数据库datetime字段（不带毫秒时间与带毫秒时间） - 深之JohnChen的专栏 - CSDN博客

2017年08月25日 16:24:55[byxdaz](https://me.csdn.net/byxdaz)阅读数：1073


//获取时间(毫秒)
BOOL GetDateTime(int nCol,CString & strTime)
{  
    _bstr_t  bstrColName;
    _variant_t varCounter;
    varCounter.vt=VT_I4;
    varCounter.lVal=0;
    _variant_t varValue;

    //取得列名
    bstrColName = m_pRSet->GetFields()->Item[nCol]->GetName() ;
    strColname = (char*)bstrColName ;
    //取得当前行当前列值
    varCounter.lVal = nCol;
    varValue = m_pRSet->GetCollect(varCounter);

   CString strTimeGet;
   _variant_t varTimeGet;
   COleDateTime oleTimeGet;  
   SYSTEMTIME sysTimeGet;
   varTimeGet = varValue;
   if (varTimeGet.vt != VT_NULL)
   {
    //计算毫秒
    DATE   date   =   DATE(varValue); 
    long   l   =   long(date); //天数 
    date   =   date   -   l; 
    date   =   date   *   24   *   60   *   60; 
    l   =   long(date);     //秒数 
    date   =   date   -   l; 
    date   =   date   *   1000; 
    l   =   long(date);     //毫秒数 
    oleTimeGet=(COleDateTime)varTimeGet;
    VariantTimeToSystemTime(oleTimeGet,&sysTimeGet);
    strTimeGet.Format(_T("%d-%d-%d %d:%d:%d.%03d"),sysTimeGet.wYear,sysTimeGet.wMonth,sysTimeGet.wDay,sysTimeGet.wHour,sysTimeGet.wMinute,sysTimeGet.wSecond,l);
    strTime = strTimeGet;
   }

    return TRUE;
}   

//写时间（不带毫秒）
BOOL PutDateTime(char *pFieldName,int nYear,int nMonth,int nDay,int nHour,int nMinute,int nSecond,int nMillSecond)
{
 CString strCurrentTime = "";
 strCurrentTime.Format(_T("%d-%d-%d %d:%d:%d"),nYear,nMonth,nDay,nHour,nMinute,nSecond);
 _variant_t varTime = (LPCTSTR)strCurrentTime;
 m_pRSet->Update(pFieldName,(_variant_t)(COleDateTime)varTime);
 //m_pRSet->Update(pFieldName,(_variant_t)(COleDateTime)sys);

 return TRUE;
}

//写时间（带毫秒）,转换成带毫秒的时间字符串写入到数据库
BOOL PutDateTime(char *pFieldName,int nYear,int nMonth,int nDay,int nHour,int nMinute,int nSecond,int nMillSecond)
{
 CString strCurrentTime = "";
 strCurrentTime.Format(_T("%d-%d-%d %d:%d:%d.%d"),nYear,nMonth,nDay,nHour,nMinute,nSecond,nMillSecond);
 _variant_t varTime = (LPCTSTR)strCurrentTime;
 m_pRSet->Update(pFieldName,varTime);

 return TRUE;
}

