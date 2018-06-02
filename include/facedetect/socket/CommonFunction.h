#pragma once
#include "windows.h"
#include "io.h"
#include "string"
#include "list"
#include "../ext/cJSON.h"
using namespace std;

const DWORD REG_READ_LENGTH_MAX = 256;
const char REG_SOFTWARE_UNINSTALL_KEY_64[] = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall"; 
const char REG_SOFTWARE_UNINSTALL_KEY_32[] = "SOFTWARE\\Wow6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"; 
const DWORD INSTALL_INFO_KEY_COUNT = 5;
const string s_strInstallInfoKeyList[INSTALL_INFO_KEY_COUNT] = {
	"DisplayName",
	"DisplayVersion",
	"InstallLocation",
	"QuietUninstallString",
	"UninstallString"
};

struct UNINSTALLSOFTWAREINFO
{
	string strDisplayName;
	string strDisplayVersion;
	string strInstallLocation;
	string strQuietUninstallString;
	string strUninstallString;

	void Clear()
	{
		strDisplayName = "";
		strDisplayVersion = "";
		strInstallLocation = "";
		strQuietUninstallString = "";
		strUninstallString = "";
	}

	void SetValueByKey(const string& strKey, const string& strVal)
	{
		if ("DisplayName" == strKey){
			strDisplayName = strVal;
		}else if("DisplayVersion" == strKey){
			strDisplayVersion = strVal;
		}else if("InstallLocation" == strKey){
			strInstallLocation = strVal;
		}else if("QuietUninstallString" == strKey){
			strQuietUninstallString = strVal;
		}else if("UninstallString" == strKey){
			strUninstallString = strVal;
		}
	}

	string GetStructJson(){
		string strRet = "{\"DisplayName\":\"" + strDisplayName + 
			"\",\"DisplayVersion\":\"" + strDisplayVersion + 
			"\",\"InstallLocation\":\"" + strInstallLocation + 
			"\",\"QuietUninstallString\":\"" + strQuietUninstallString + 
			"\",\"UninstallString\":\"" + strUninstallString + 
			"\"}";

		return strRet;
	}
};

enum eWriteMode
{
	eWriteMode_OverWrite,
	eWriteMode_Apend,
};

struct MyFileNode
{
	string strFileName;
	MyFileNode* pNext;

	MyFileNode()
	{
		strFileName = "";
		pNext = NULL;
	}
};

struct FolderNode
{
	string strFolderName;
	string strFullPath;
	MyFileNode* pFileList;
	FolderNode* pSubFolderList; // used to identified if there is sub folder or no
	FolderNode* pNext; // used to identified the same level folder

	FolderNode()
	{
		strFolderName = "";
		strFullPath = "";
		pFileList = NULL;
		pSubFolderList = NULL;
		pNext = NULL;
	}
};

struct FolderInfoNode
{
	string strFullPath;
	FolderNode* pFolderInfo;
	FolderInfoNode* pNext;
	FolderInfoNode()
	{
		strFullPath = "";
		pFolderInfo = NULL;
		pNext = NULL;
	}
};


////////////////////////////////// define constructor of string to avoid NULL point ////////////////////////////////////////
#define  MY_STRING_CONSTRUCTOR_LEN(strDest, pData, iLen){ \
	if (!pData){ \
		strDest = ""; \
	} else if(iLen < 0){ \
		strDest = ""; \
	} else { \
		strDest = string(pData, iLen); \
	} \
}

#define  MY_STRING_CONSTRUCTOR_STRING(strDest, strOrg, iStart, iLen){ \
	if (iStart >= strOrg.length()){	\
		strDest = ""; \
	} else { \
		strDest = string(strOrg, iStart, iLen); \
	}\
}

#define  MY_STRING_CONSTRUCTOR_POINT(strDest, pData){ \
	if (!pData){ \
		strDest = "";	\
	} else {\
		strDest = (pData); \
	} \
}


static char s_HexadecimalChar[] = 
{
	'0', '1', '2', '3', '4', 
	'5', '6', '7', '8', '9',
	'a', 'b', 'c', 'd', 'e',
	'f'
};

typedef list<string> STRING_LIST;

class CommonFunction
{
public:
	CommonFunction(void);
	virtual ~CommonFunction(void);

	/////////////////////////////////// string ///////////////////////////////////////
	static string WstringToString(const wstring& str, unsigned long ulCodePage);
	static wstring StringToWstring(const string& str, unsigned long ulCodePage);
	static bool IsGBK(unsigned long dwUnicode);
	static bool IsStringUTF8(const string& str);
	static string& StringTrimLeft(string& str, char cTrim = ' ');
	static string& StringTrimRight(string& str, char cTrim = ' ');
	static string& StringTrim(string& str, char cTrim = ' ');
	static string& StringReplace(string& str, const string& strOld, const string& strNew);
	static wstring GetUnicodeWstring(const  string& pSorce, ULONG ulCodePage);
	static wstring GetUnicodeWstring(wstring& pSorce);
	static string GetUnicodeString(string& pSorce);
	static string GetUnicodeString(const wstring& pSorce, ULONG ulCodePage);
	static string& StringMakeUpper(string& str);
	static string& StringMakeLower(string& str);
	static string SubStringRfindTag(const string& strOrg, const string& strTag, bool bOrgIfNoFound = true);
	static wstring& EncodeUtf16Chinese(wstring& strSource);
	static string& EncodeUtf16Chinese(string& strSource);
	static wstring& DecodeUtf16Chinese(wstring& strSource);
	static string& DecodeUtf16Chinese(string& strSource);


	////////////////////////////////// file ////////////////////////////////////////
	static bool ReAllocMem(char*& pBuffer, unsigned long dwOldLen, unsigned long dwNewLen);
	static bool ReadContentFromFile(const string& strFilePath, string& strContent);
	static bool ReadBigDataContentFromFile(const string& strFilePath, string& strContent);
	static bool ReadContentFromFileToBuffer(const string& strFilePath, char* DestBuffer, const unsigned& DestBufferSize);
	static bool WriteContentToFile(const string& strFilePath, const string& strContent, eWriteMode eMode);
	static bool WriteBufferToFile(const string& strFilePath, const char* pBuffer, unsigned long dwBuffersize, eWriteMode eMode, bool bShare = true);
	static bool CreatFlag(string strFlagPath);


	//////////////////////////////////// folder //////////////////////////////////////
	static bool IsPathExist(const string& strPath);
	static bool IsPathExist(const wstring& wstrPath);
	static bool CreatePath(const string& strPath);
	static bool CreatePath(const wstring& strPath);
	static bool IsPathDirectory(const char *str);
	static bool GetFileList(const string& strPath, STRING_LIST& folderList, STRING_LIST& fileList);
	static bool GetFileListRelative(const string& strPath, STRING_LIST& folderList, STRING_LIST& fileList);
	static void ClearSubFolderByFlagFile(const char* strFolderPath, const char* strFlagFileName);

	static void DeleteFileNodeList(MyFileNode*& pFolderInfo);
	static void DeleteFolderNodeList(FolderNode*& pFolderInfo);
	static void DeleteFolderInfoNodeList(FolderInfoNode*& pFolderNodeList, bool bDeleteFolderNode);
	static bool GetFolderInfoLoop(string strSource, FolderNode*& pFolderInfo, DWORD& dwFolderSize, DWORD& dwFileCount, DWORD& dwFolderCount);

	static bool DeleteDirectory(const string& strPath);
	static bool __DeleteDirectory(const string& strPath);

	static bool DeleteDirectory(const string& strPath, const STRING_LIST& listIgnoreFolder);
	static bool __DeleteDirectory(const string& strPath, const STRING_LIST& listIgnoreFolder);
	static bool GetFolderInfoLoop(string strSource, FolderNode*& pFolderInfo, DWORD& dwFolderSize, DWORD& dwFileCount, DWORD& dwFolderCount, const STRING_LIST& listIgnoreFolder);

	static bool DeleteFolderLoop(FolderNode* pFolderInfo);
	static bool CopyDirectory(string source, string target);
	static bool CopyFolderLoop(FolderNode* pFolderInfo, const string& strDestFolder);
	static bool ReplaceDirectory(string source, string target);
	static bool ReplaceFolderLoop(FolderNode* pFolderInfo, const string& strDestFolder);
	static bool CutReplaceDirectory(string source, string target);
	static bool CutReplaceFolderLoop(FolderNode* pFolderInfo, const string& strDestFolder);
	

	////////////////////////////////// exe ////////////////////////////////////////
	// app name: QQ, don't used QQ.exe!!!
	static bool RunExe(const string& strName, const string& strRunCmd);
	static string GetExePath();
	static bool RunBatCmd(const wstring& wstrCmd);
	static bool RunBatCmdFile(const string& strCmdFilePath);


	////////////////////////////////// regedit ////////////////////////////////////////
	static bool SearchSoftwareInstallInfoFromRegedit(string strSoftwareName, UNINSTALLSOFTWAREINFO& cSoftwareInstallInfo, bool bSearchInSys32 = true);


	///////////////////////////////// other /////////////////////////////////////////
	static void GetCurrentDate(string &strDateStr);
	static int GetTimeStamp();
	static int StrToTime(char* const date);
	static string GetPCName();
	static void XdPrint(char* format, ...);
	static bool IsDigit(const string& str);
	static bool ClearStrPrefix(
		const string& strSrc, 
		const string& strPrefix, 
		bool bRecursion, 
		string& strNew);
	
	// only merge string number and bool, not include array and object
	static void CjsonMergeDestToSrc(cJSON* pJsSrcJson, cJSON* pJsDestJson);
	static void CjsonMergePairedArrayItemToSrc(cJSON* pJsSrcJson, cJSON* pJsDestArrayJson, char* pPairChar);
	// only can compare number and bool
	static bool CjsonCompare(cJSON* pJsSrcJson, cJSON* pJsDestJson, char* pSrcCmpChar, char* pDestCmpChar);
	static bool CjsonCompareSrcValueAndDestKey(cJSON* pJsSrcJson, cJSON* pJsDestJson, char* pCmpChar);

private:
	
	static unsigned parse_hex4(const char *str);
	
};

#ifdef _DEBUG
#define MY_PRINTF(pchar){ \
	string strP = pchar; \
	CommonFunction::XdPrint("%s\n", strP.c_str());\
}
#else
#define MY_PRINTF(pchar){}
#endif
