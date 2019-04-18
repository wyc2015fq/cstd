# VC读写ini文件  - 深之JohnChen的专栏 - CSDN博客

2007年09月13日 11:17:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2194标签：[ini																[path																[class																[api](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=ini&t=blog)

VC读写ini文件 

关于ini文件读写的API函数 有下列这些:

GetPrivateProfileInt  
GetPrivateProfileSection  
GetPrivateProfileSectionNames  
GetPrivateProfileString  
GetPrivateProfileStruct  
GetProfileInt  
GetProfileSection  
GetProfileString  
WritePrivateProfileSection  
WritePrivateProfileString  
WritePrivateProfileStruct  
WriteProfileSection  
WriteProfileString
注:关于这些函数的详细解释请参考MSDN

为更好的了解ini文件的操作,下面给出一个实例,希望能起到抛砖引玉的作用:

#if !defined AFX_INI_HEAR_H
#define AFX_INI_HEAR_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
 *  ver 1.0.0.0
 *   读写ini文件的内容
 *   johnchen
 */
#define  MAX_KEYS 1024
#include "afxtempl.h"
class CIni
{
public:
 CIni();
 ~CIni();
 void SetIniPathName(CString strFileName);
    void GetValueByKey(CString strAppName,CString strKey,CString & strValue);
 void SetValueByKey(CString strAppName,CString strKey,CString strValue);
 int GetValueByAppName(CList<int,int> & listKey,CList<CString,CString&> & listValue,CString strAppName="sealremarktype"); // 根据字段得到所有键值
private:
 CString m_strIniPathName;
};

#endif

#include <stdafx.h>
#include "ini.h"

CIni::CIni()
{
 CString strBuffer;
 char szBuffer[MAX_PATH];
 memset(szBuffer,0,MAX_PATH);
 ::GetCurrentDirectory(MAX_PATH,(LPSTR)strBuffer.GetBuffer(MAX_PATH));
    strcpy(szBuffer,(char *)strBuffer.GetBuffer(MAX_PATH));
 strcat(szBuffer,"//sealremark.ini");
    m_strIniPathName = szBuffer;
}

CIni::~CIni()
{

}

// 设置ini文件路径
void CIni::SetIniPathName(CString strFileName)
{
    m_strIniPathName = strFileName;
}

// 得到键值
void CIni::GetValueByKey(CString strAppName,CString strKey,CString & strValue)
{
 GetPrivateProfileString(strAppName.GetBuffer(strAppName.GetLength()),strKey.GetBuffer(strKey.GetLength()),"公章",strValue.GetBuffer(MAX_PATH),MAX_PATH,m_strIniPathName);
}

// 设定键值
void CIni::SetValueByKey(CString strAppName,CString strKey,CString strValue)
{
    ::WritePrivateProfileString(strAppName.GetBuffer(strAppName.GetLength()),strKey.GetBuffer(strKey.GetLength()),strValue.GetBuffer(strValue.GetLength()),m_strIniPathName);
}

// 函数功能：根据字段得到所有键值
// 参数说明：
// listKey 键
// listValue 值
// strAppName 字段
// 返回值：int 键值数量
int CIni::GetValueByAppName(CList<int,int> & listKey,CList<CString,CString&> & listValue,CString strAppName)
{
 TCHAR charKeyName[MAX_KEYS];
 memset(charKeyName,0,sizeof(TCHAR)*MAX_KEYS);
 int iNum = 0; // 键值总数
 DWORD dw = GetPrivateProfileSection(strAppName.GetBuffer(strAppName.GetLength()),charKeyName,MAX_KEYS,m_strIniPathName);
 if(dw==0)  // 没有记录
 {
  return iNum;
 }
 for(int i=0;i<MAX_KEYS;i++)
 {
  if(charKeyName[i]==0)
  {
   iNum++;
   if(charKeyName[i]==charKeyName[i+1])
    break;
  }
 }
 int iCount = iNum,iPos = 0,iOldPos=0,j=0;
 TCHAR szKey[128],szValue[128];
 CString strTemp = "";

 for(i=0;i<=dw;i++)
    {
  if(charKeyName[i]==0)
  {
   iCount--;
   iPos = i;
   memset(szKey,0,128*sizeof(TCHAR));
   memset(szValue,0,128*sizeof(TCHAR));
   for(j=iOldPos;j<iPos;j++)
   {
    if(charKeyName[j]=='=')
    {
     memcpy(szKey,&charKeyName[iOldPos],j-iOldPos);
     memcpy(szValue,&charKeyName[j+1],iPos-j-1);
     listKey.AddTail((int)atoi(szKey));
     strTemp = szValue;
     listValue.AddTail(strTemp);
     iOldPos = iPos+1;
     break;
    }
   }
  }
    }

 return iNum;
}



