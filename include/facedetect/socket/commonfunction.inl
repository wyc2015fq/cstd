#include "CommonFunction.h"
#include <algorithm>
#include "windows.h"
#include <time.h>
#include "MyLog.h"
#include <sstream>

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

const unsigned long READ_FILE_SIZE		= 1024 * 1;
const unsigned long FILE_BUFFER_ONCE	= 1024 * 2;

const unsigned long READ_FILE_SIZE_BIGDATA		= 1024 * 1024;
const unsigned long FILE_BUFFER_ONCE_BIGDATA	= 1024 * 1024 * 10;
static const unsigned char firstByteMark[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

CommonFunction::CommonFunction(void)
{
}

CommonFunction::~CommonFunction(void)
{
}


/////////////////////////////////// string ///////////////////////////////////////
string CommonFunction::WstringToString(const wstring& wstrOrg, unsigned long ulCodePage)
{
	// codepage: CP_UTF8/CP_ACP...
	int iStringBufferLen = WideCharToMultiByte( ulCodePage,	0, wstrOrg.c_str(), -1, NULL, 0, NULL, NULL );
	char* pStringBuffer = new char[iStringBufferLen + 1];
	if (pStringBuffer)
	{
		memset(pStringBuffer, 0, sizeof(char) * (iStringBufferLen + 1));
		WideCharToMultiByte( ulCodePage, 0, wstrOrg.c_str(), -1, pStringBuffer, iStringBufferLen, NULL, NULL );
		string strReturn = pStringBuffer;
		delete[] pStringBuffer;
		return strReturn;
	}
	else
	{
		return "";
	}
	
}

wstring CommonFunction::StringToWstring(const string& strOrg, unsigned long ulCodePage)
{
	// codepage: CP_UTF8/CP_ACP...
	int iWStringLen = ::MultiByteToWideChar( ulCodePage, 0, strOrg.c_str(), -1, NULL, 0 ); 
	wchar_t * pWStringBuffer = new wchar_t[iWStringLen+1]; 
	if (pWStringBuffer)
	{
		memset(pWStringBuffer,0,(iWStringLen+1)*sizeof(wchar_t)); 
		::MultiByteToWideChar( ulCodePage, 0, strOrg.c_str(), -1, (LPWSTR)pWStringBuffer, iWStringLen ); 
		wstring wstrReturn; 
		wstrReturn = ( wchar_t* )pWStringBuffer;
		delete []pWStringBuffer; 
		return wstrReturn; 
	}
	else
	{
		return L"";
	}
	
}

wstring& CommonFunction::EncodeUtf16Chinese(wstring &strSource)
{
	if (L"" == strSource){
		return strSource;
	}

	DWORD dwLen = strSource.length();
	wchar_t* pRet = new wchar_t[dwLen*6 + 1];
	if (!pRet){
		return strSource;
	}

	memset(pRet, 0x00, sizeof(wchar_t)* (dwLen*6 + 1));
	wchar_t* pStr = const_cast<wchar_t*>(strSource.c_str());
	DWORD dwIndex = 0;
	DWORD dwRetIndex = 0;
	while (pStr[dwIndex] != L'\0')
	{
		DWORD dwUnicodeValue = pStr[dwIndex];
		if (IsGBK(dwUnicodeValue))
		{
			// add "\u"
			pRet[dwRetIndex + 0] = L'\\';
			pRet[dwRetIndex + 1] = L'u';
			for (int indexTemp = 4; indexTemp > 0; --indexTemp){
				pRet[dwRetIndex + 1 + indexTemp] = s_HexadecimalChar[dwUnicodeValue%16];
				dwUnicodeValue /= 16;
			}
			dwRetIndex += 6;
		}
		else
		{
			pRet[dwRetIndex] = dwUnicodeValue;
			++dwRetIndex;
		}
		++dwIndex;
	}

	strSource = pRet;
	delete []pRet;

	return strSource;
}

string& CommonFunction::EncodeUtf16Chinese(string &strSource)
{
	wstring wstr = StringToWstring(strSource, CP_UTF8);
	EncodeUtf16Chinese(wstr);
	strSource = WstringToString(wstr, CP_UTF8);
	return strSource;
}

wstring& CommonFunction::DecodeUtf16Chinese(wstring& strSource)
{
	string str = WstringToString(strSource, CP_UTF8);
	DecodeUtf16Chinese(str);
	strSource = StringToWstring(str, CP_UTF8);
	return strSource;
}

string& CommonFunction::DecodeUtf16Chinese(string& strSource)
{
	unsigned long dwSourceLen = strSource.length();
	if (dwSourceLen < 6){
		return strSource;
	}

	char* pDest = new char[dwSourceLen + 1];
	if (!pDest){
		return strSource;
	}

	memset(pDest, 0x00, sizeof(char)*(dwSourceLen + 1));
	const char* pSource = strSource.c_str();
	unsigned long indexSour = 0;
	unsigned long indexDest = 0;

	while (indexSour < dwSourceLen)
	{
		if (pSource[indexSour] == '\\' && indexSour <= dwSourceLen - 6)
		{
			switch(pSource[indexSour+1])
			{
/*
			case 'b': 
				pDest[indexDest++]= '\b';	
				break;
			case 'f': 
				pDest[indexDest++]= '\f';	
				break;
			case 'n': 
				pDest[indexDest++]= '\n';	
				break;
			case 'r': 
				pDest[indexDest++]= '\r';	
				break;
			case 't': 
				pDest[indexDest++]= '\t';	
				break;*/
			case 'u':	 /* transcode utf16 to utf8. */
				{
					++indexSour;
					unsigned long dwUnicodeValuePartA, dwUnicodeValuePartB(0), dwUtf8WordLen;
					dwUnicodeValuePartA = parse_hex4(pSource + indexSour + 1);
					//indexSour += 4;	/* get the unicode char. */

					if ((dwUnicodeValuePartA>=0xDC00 && dwUnicodeValuePartA<=0xDFFF) || dwUnicodeValuePartA==0){/* check for invalid.	*/
						pDest[indexDest++]= '\\';
						pDest[indexDest++]= pSource[indexSour++];
						break;	
					}

					if (dwUnicodeValuePartA>=0xD800 && dwUnicodeValuePartA<=0xDBFF)	/* UTF16 surrogate pairs.	*/
					{
						 /* missing second-half of surrogate. */
						if (indexSour + 10 > dwSourceLen && (pSource[indexSour + 5] !='\\' || pSource[indexSour + 6] !='u')){
							pDest[indexDest++]= '\\';
							pDest[indexDest++]= pSource[indexSour++];
							break;	
						}

						dwUnicodeValuePartB = parse_hex4(pSource + indexSour + 7); 
						//ptr+=6;
						if (dwUnicodeValuePartB<0xDC00 || dwUnicodeValuePartB>0xDFFF){/* invalid second-half of surrogate.	*/
							pDest[indexDest++]= '\\';
							pDest[indexDest++]= pSource[indexSour++];
							break;	
						}

						dwUnicodeValuePartA = 0x10000 + (((dwUnicodeValuePartA&0x3FF)<<10) | (dwUnicodeValuePartB&0x3FF));
					}

					dwUtf8WordLen = 4;
					if (dwUnicodeValuePartA<0x80) {
						dwUtf8WordLen=1;
					}else if (dwUnicodeValuePartA<0x800){
						dwUtf8WordLen=2;
					}else if (dwUnicodeValuePartA<0x10000){
						dwUtf8WordLen=3;
					}

					//ptr2+=len;
					// DON'T BREAK HERE!!! 
					switch (dwUtf8WordLen) 
					{
					case 4: 
						pDest[indexDest + 3] = ((dwUnicodeValuePartA | 0x80) & 0xBF);
						dwUnicodeValuePartA >>= 6;
					case 3: 
						pDest[indexDest + 2] = ((dwUnicodeValuePartA | 0x80) & 0xBF);
						dwUnicodeValuePartA >>= 6;
					case 2: 
						pDest[indexDest + 1] = ((dwUnicodeValuePartA | 0x80) & 0xBF);
						dwUnicodeValuePartA >>= 6;
					case 1: 
						pDest[indexDest] = (dwUnicodeValuePartA | firstByteMark[dwUtf8WordLen]);
					}
					indexDest += dwUtf8WordLen;
					if (0 != dwUnicodeValuePartB){
						indexSour += 11;
					}else{
						indexSour += 5;
					}
				}
				break;
			default:
				pDest[indexDest++] = pSource[indexSour++];
				break;
			}

			continue;
		} // if

		pDest[indexDest++] = pSource[indexSour++];
	} // while

	strSource = string(pDest);
	delete []pDest;
	return strSource;
}

bool CommonFunction::IsStringUTF8(const string& str)
{
	unsigned long length = str.length();
	const char* pstr = str.c_str();
	int i;
	DWORD nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
	UCHAR chr;
	bool bAllAscii=true; //如果全部都是ASCII, 说明不是UTF-8
	for(i=0;i<length;i++)
	{
		chr= *(pstr+i);
		if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0, o0xxxxxxx
		{
			bAllAscii= false;
		}
		if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr>=0x80)
			{
				if(chr>=0xFC&&chr<=0xFD)
					nBytes=6;
				else if(chr>=0xF8)
					nBytes=5;
				else if(chr>=0xF0)
					nBytes=4;
				else if(chr>=0xE0)
					nBytes=3;
				else if(chr>=0xC0)
					nBytes=2;
				else
				{
					return false;
				}
				nBytes--;
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return false;
			}
			nBytes--;
		}
	}

	if( nBytes > 0 ) //违返规则
	{
		return false;
	}
	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
	{
		return false;
	}
	return true;
}

inline bool CommonFunction::IsGBK(DWORD dwUnicode)
{
	// http://blog.csdn.net/shuilan0066/article/details/7839189
	// U+4E00..U+9FA5, 2E80－A4CF, F900-FAFF, FE30-FE4F
	if ((dwUnicode >= 0x4E00 && dwUnicode <= 0x9FA5)
		||(dwUnicode >= 0x2E80 && dwUnicode <= 0xA4CF)
		||(dwUnicode >= 0xF900 && dwUnicode <= 0xFAFF)
		||(dwUnicode >= 0xFE30 && dwUnicode <= 0xFE4F)
		)
	{
		return true;
	}

	return false;
}

string& CommonFunction::StringTrimLeft(string& str, char cTrim /* = ' ' */)
{
	long dwStrLen = str.length();
	if (0 == dwStrLen){
		return str;
	}

	const char* pStr = str.c_str();
	long dwIndex = 0;
	while(dwIndex < dwStrLen && pStr[dwIndex] == cTrim){
		++dwIndex;
	}

	str =  string(pStr + dwIndex, dwStrLen - dwIndex);
	return str;
}

string& CommonFunction::StringTrimRight(string& str, char cTrim /* = ' ' */)
{
	long dwStrLen = str.length();
	if (0 == dwStrLen){
		return str;
	}

	const char* pStr = str.c_str();
	long dwIndex = dwStrLen;
	while(dwIndex > 0 && pStr[dwIndex - 1] == cTrim){
		--dwIndex;
	}

	str = string(pStr, dwIndex);
	return str;
}

string& CommonFunction::StringTrim(string& str, char cTrim /* = ' ' */)
{
	StringTrimLeft(str, cTrim);
	StringTrimRight(str, cTrim);
	return str;
}

string& CommonFunction::StringReplace(string& str, const string& strOld, const string& strNew)
{
	if (str.find(strOld, 0) == -1 || 0 == strOld.length()){
		return str;
	}

	long strLen = str.length();
	if (strNew.length() > strOld.length()){
		strLen = strLen * strNew.length() / strOld.length();
	}

	char* pNew = new char[strLen + 1];
	if (!pNew){
		return str;
	}

	memset(pNew, 0x00, strLen + 1);
	int startFindIndex = 0;
	int currentNewIndex = 0;
	int findIndex = str.find(strOld, startFindIndex);
	
	while(findIndex != -1)
	{
/*
		if (strOld == "\\")
		{
			if (findIndex != str.length() - 1 &&
				(str.c_str()[findIndex + 1] == 'a' // 
				||str.c_str()[findIndex + 1] == 'b' // 
				||str.c_str()[findIndex + 1] == 'f' // 
				||str.c_str()[findIndex + 1] == 'n' // 
				||str.c_str()[findIndex + 1] == 'r' // 
				||str.c_str()[findIndex + 1] == 't' // 
				||str.c_str()[findIndex + 1] == 'v' // 
				||str.c_str()[findIndex + 1] == '\"' // 
				||str.c_str()[findIndex + 1] == '\\' // 
				))
			{
				startFindIndex = findIndex + 2;
				findIndex = str.find(strOld, startFindIndex);
				continue;
			}
		}*/

		memcpy(pNew + currentNewIndex, str.c_str() + startFindIndex, findIndex - startFindIndex);
		currentNewIndex += findIndex - startFindIndex;
		memcpy(pNew + currentNewIndex, strNew.c_str(), strNew.length());
		currentNewIndex += strNew.length();
		startFindIndex = findIndex + strOld.length();
		findIndex = str.find(strOld, startFindIndex);
	} 

	memcpy(pNew + currentNewIndex, str.c_str() + startFindIndex, str.length() - startFindIndex);

	str = pNew;
	delete []pNew;
	return str;
}

wstring CommonFunction::GetUnicodeWstring(const string& pSorce, ULONG ulCodePage)
{
	int unicodeLen = ::MultiByteToWideChar( ulCodePage,//CP_ACP,
		0,
		pSorce.c_str(),
		-1,
		NULL,
		0 ); 
	wchar_t * pUnicode; 
	pUnicode = new wchar_t[unicodeLen+1]; 
	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t)); 
	::MultiByteToWideChar( ulCodePage, //CP_ACP,
		0,
		pSorce.c_str(),
		-1,
		(LPWSTR)pUnicode,
		unicodeLen ); 

	unsigned long dwResultLen = unicodeLen * 6 + 1;
	wchar_t* pResult = new wchar_t[dwResultLen];
	unsigned long dwRetSize = 0;
	for (int index = 0; index < unicodeLen; ++index)
	{
		unsigned long dwUnicodeValue = pUnicode[index];
		if (0 == dwUnicodeValue){
			break;
		}
		// add "\u"
		pResult[index * 6 + 0] = L'\\';
		pResult[index * 6 + 1] = L'u';
		// get unicode string
		for (int indexTemp = 4; indexTemp > 0; --indexTemp){
			pResult[index * 6 + 1 + indexTemp] = s_HexadecimalChar[dwUnicodeValue%16];
			dwUnicodeValue /= 16;
		}

		dwRetSize += 6;
	}
	pResult[dwRetSize] = 0;

	wstring ret = wstring((wchar_t*)pResult, dwRetSize);
	delete []pUnicode;
	delete []pResult;
	return ret;
}

wstring CommonFunction::GetUnicodeWstring(wstring& pSorce)
{
	unsigned long dwSourceLen = pSorce.length();
	const wchar_t* pUnicode = pSorce.c_str();

	unsigned long dwResultLen = dwSourceLen * 6 + 1;
	wchar_t* pResult = new wchar_t[dwResultLen];
	unsigned long dwRetSize = 0;
	for (int index = 0; index < dwSourceLen; ++index)
	{
		unsigned long dwUnicodeValue = pUnicode[index];
		if (0 == dwUnicodeValue){
			break;
		}
		// add "\u"
		pResult[index * 6 + 0] = L'\\';
		pResult[index * 6 + 1] = L'u';
		// get unicode string
		for (int indexTemp = 4; indexTemp > 0; --indexTemp){
			pResult[index * 6 + 1 + indexTemp] = s_HexadecimalChar[dwUnicodeValue%16];
			dwUnicodeValue /= 16;
		}

		dwRetSize += 6;
	}
	pResult[dwRetSize] = 0;

	pSorce = wstring((wchar_t*)pResult, dwRetSize);
	delete []pResult;
	return pSorce;
}

string CommonFunction::GetUnicodeString(string& pSorce)
{
	wstring ret = GetUnicodeWstring(pSorce, CP_UTF8);
	pSorce = WstringToString(ret, CP_UTF8);
	return pSorce;
}

string CommonFunction::GetUnicodeString(const wstring& pSorce, ULONG ulCodePage)
{
	wstring wstrSource = pSorce;
	wstrSource = GetUnicodeWstring(wstrSource);
	return WstringToString(wstrSource, ulCodePage);
}

string& CommonFunction::StringMakeUpper(string& str)
{
	transform(str.begin(), str.end(), str.begin(), ::toupper); 
	return str;
}

string& CommonFunction::StringMakeLower(string& str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower); 
	return str;
}

string CommonFunction::SubStringRfindTag(const string& strOrg, const string& strTag, bool bOrgIfNoFound /* = true */)
{
	int iTemp = strOrg.rfind(strTag);
	if (iTemp >= 0){
		return string(strOrg.c_str(), iTemp);
	}else{
		if (bOrgIfNoFound){
			return strOrg;
		}else{
			return "";
		}
	}
}


unsigned CommonFunction::parse_hex4(const char *str)
{
	unsigned h= 0;
	if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
	h=h<<4;str++;
	if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
	h=h<<4;str++;
	if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
	h=h<<4;str++;
	if (*str>='0' && *str<='9') h+=(*str)-'0'; else if (*str>='A' && *str<='F') h+=10+(*str)-'A'; else if (*str>='a' && *str<='f') h+=10+(*str)-'a'; else return 0;
	return h;
}


////////////////////////////////// file ////////////////////////////////////////
inline bool CommonFunction::ReAllocMem(char*& pBuffer, unsigned long dwOldLen, unsigned long dwNewLen)
{
	if (!pBuffer){
		return false;
	}

	char* pBufferResult = new char[dwNewLen];
	if (pBufferResult)
	{
		memset(pBufferResult, 0x00, dwNewLen);
		memcpy(pBufferResult, pBuffer, dwOldLen);
		delete []pBuffer;
		pBuffer = pBufferResult;
	}
	else
	{
		return false;
	}

	return true;
}

bool CommonFunction::ReadContentFromFile(const string& strFilePath, string& strContent)
{
	strContent.clear();

	FILE* pFile = _fsopen(strFilePath.c_str(), "r", _SH_DENYNO);
	if (!pFile){
		return false;
	}

	bool bResult = false;
	char* pbufferTemp = NULL;
	char* pbufferRet = NULL;
	do 
	{
		// read content
		pbufferTemp = new char[READ_FILE_SIZE];
		if (!pbufferTemp){
			break;
		}

		unsigned long dwBufferRetSize = FILE_BUFFER_ONCE;
		pbufferRet = new char[dwBufferRetSize];
		if (!pbufferRet){
			break;
		}
		memset(pbufferRet,0x00, dwBufferRetSize);

		unsigned long dwReadSize = 0;
		unsigned long dwTemp = 0;
		do 
		{
			// CAUTION!!!!! Since used the return value to check if read finished, the second param MUST be 1!
			dwTemp = fread(pbufferTemp, 1, READ_FILE_SIZE, pFile);
			if (dwTemp + dwReadSize > dwBufferRetSize)
			{
				if(ReAllocMem(pbufferRet, dwBufferRetSize, dwBufferRetSize + FILE_BUFFER_ONCE)){
					dwBufferRetSize += FILE_BUFFER_ONCE;
				}else{
					break;
				}
			}

			if (0 == dwReadSize // first time copy
				&& 0xef == (BYTE)pbufferTemp[0] && 0xbb == (BYTE)pbufferTemp[1] && 0xbf == (BYTE)pbufferTemp[2] // UTF-8-BOM
			)
			{
				memcpy(pbufferRet + dwReadSize, pbufferTemp + 3, dwTemp - 3);
				dwReadSize += (dwTemp - 3);
			}
			else
			{
				memcpy(pbufferRet + dwReadSize, pbufferTemp, dwTemp);
				dwReadSize += dwTemp;
			}
		} while(dwTemp == READ_FILE_SIZE);

		strContent = string(pbufferRet, dwReadSize);
		bResult = true;
	} while(0);

	if (pbufferRet){
		delete []pbufferRet;
	}

	if (pbufferTemp){
		delete []pbufferTemp;
	}

	fclose(pFile);

	CommonFunction::DecodeUtf16Chinese(strContent);
	
	return bResult;
}

bool CommonFunction::ReadBigDataContentFromFile(const string& strFilePath, string& strContent)
{
	strContent.clear();

	FILE* pFile = _fsopen(strFilePath.c_str(), "r", _SH_DENYNO);
	if (!pFile){
		return false;
	}

	bool bResult = false;
	char* pbufferTemp = NULL;
	char* pbufferRet = NULL;
	do 
	{
		// read content
		pbufferTemp = new char[READ_FILE_SIZE_BIGDATA];
		if (!pbufferTemp){
			break;
		}

		unsigned long dwBufferRetSize = FILE_BUFFER_ONCE_BIGDATA;
		pbufferRet = new char[dwBufferRetSize];
		if (!pbufferRet){
			break;
		}
		memset(pbufferRet,0x00, dwBufferRetSize);

		unsigned long dwReadSize = 0;
		unsigned long dwTemp = 0;
		do 
		{
			// CAUTION!!!!! Since used the return value to check if read finished, the second param MUST be 1!
			dwTemp = fread(pbufferTemp, 1, READ_FILE_SIZE_BIGDATA, pFile);
			if (dwTemp + dwReadSize > dwBufferRetSize)
			{
				if(ReAllocMem(pbufferRet, dwBufferRetSize, dwBufferRetSize + FILE_BUFFER_ONCE_BIGDATA)){
					dwBufferRetSize += FILE_BUFFER_ONCE_BIGDATA;
				}else{
					break;
				}
			}

			if (0 == dwReadSize // first time copy
				&& 0xef == (BYTE)pbufferTemp[0] && 0xbb == (BYTE)pbufferTemp[1] && 0xbf == (BYTE)pbufferTemp[2] // UTF-8-BOM
			)
			{
				memcpy(pbufferRet + dwReadSize, pbufferTemp + 3, dwTemp - 3);
				dwReadSize += (dwTemp - 3);
			}
			else
			{
				memcpy(pbufferRet + dwReadSize, pbufferTemp, dwTemp);
				dwReadSize += dwTemp;
			}
		} while(dwTemp == READ_FILE_SIZE_BIGDATA);

		strContent = string(pbufferRet, dwReadSize);
		bResult = true;
	} while(0);

	if (pbufferRet){
		delete []pbufferRet;
	}

	if (pbufferTemp){
		delete []pbufferTemp;
	}

	fclose(pFile);

	return bResult;
}

bool CommonFunction::ReadContentFromFileToBuffer(const string& strFilePath, char* DestBuffer, const unsigned& DestBufferSize)
{
	//strContent.clear();
	memset(DestBuffer, 0x00, DestBufferSize);

	FILE* pFile = _fsopen(strFilePath.c_str(), "rb", _SH_DENYNO);
	if (!pFile){
		return false;
	}

	bool bResult = false;
	char* pbufferTemp = NULL;
	char* pbufferRet = NULL;
	do 
	{
		// read content
		pbufferTemp = new char[READ_FILE_SIZE];
		if (!pbufferTemp){
			//LOGOUT_FATAL("ReadContentFromFile:: New Err- %d", GetLastError());
			break;
		}

		unsigned long dwBufferRetSize = FILE_BUFFER_ONCE;
		pbufferRet = new char[dwBufferRetSize];
		if (!pbufferRet){
			//LOGOUT_FATAL("ReadContentFromFile:: New Err- %d", GetLastError());
			break;
		}
		memset(pbufferRet,0x00, dwBufferRetSize);

		unsigned long dwReadSize = 0;
		unsigned long dwTemp = 0;
		do 
		{
			dwTemp = fread(pbufferTemp, 1, READ_FILE_SIZE, pFile);
			if (dwTemp + dwReadSize > dwBufferRetSize)
			{
				if(ReAllocMem(pbufferRet, dwBufferRetSize, dwBufferRetSize + FILE_BUFFER_ONCE)){
					dwBufferRetSize += FILE_BUFFER_ONCE;
				}else{
					break;
				}
			}

			if (0 == dwReadSize // first time copy
				&& 0xef == (BYTE)pbufferTemp[0] && 0xbb == (BYTE)pbufferTemp[1] && 0xbf == (BYTE)pbufferTemp[2] // UTF-8-BOM
			)
			{
				memcpy(pbufferRet + dwReadSize, pbufferTemp + 3, dwTemp - 3);
				dwReadSize += (dwTemp - 3);
			}
			else
			{
				memcpy(pbufferRet + dwReadSize, pbufferTemp, dwTemp);
				dwReadSize += dwTemp;
			}
		} while(dwTemp == READ_FILE_SIZE);

		//strContent = string(pbufferRet, dwReadSize);
		memcpy(DestBuffer, pbufferRet,(DestBufferSize < dwReadSize ? DestBufferSize : dwReadSize));

		bResult = true;
	} while(0);

	if (pbufferRet){
		delete []pbufferRet;
	}

	if (pbufferTemp){
		delete []pbufferTemp;
	}

	fclose(pFile);

	return bResult;
}

bool CommonFunction::WriteContentToFile(const string& strFilePath, const string& strContentOrg, eWriteMode eMode)
{
	string strOpenMode;
	if (eWriteMode_OverWrite == eMode){
		strOpenMode = "w+";
	}else if(eWriteMode_Apend == eMode){
		strOpenMode = "a+";
	} else {
		strOpenMode = "w+";
	}

	FILE* pFile = NULL;
	pFile = _fsopen(strFilePath.c_str(), strOpenMode.c_str(), _SH_DENYNO);
	if (!pFile){
		return false;
	}

	string strContent = strContentOrg;
	int iTemp = strFilePath.rfind(".");
	if (iTemp >= 0 && string(strFilePath.c_str() + iTemp, strFilePath.length() - iTemp) == ".json"){
		CommonFunction::EncodeUtf16Chinese(strContent);
	}
	
	fseek(pFile, 0, SEEK_END);
	unsigned long dwWriteSize = fwrite(strContent.c_str(), 1, strContent.length(), pFile);
	fclose(pFile);

	if (dwWriteSize == strContent.length()){
		return true;
	} else {
		return false;
	}
}

bool CommonFunction::WriteBufferToFile(const string& strFilePath, const char* pBuffer, unsigned long dwBuffersize, eWriteMode eMode, bool bShare/* = true*/)
{
	if (!pBuffer){
		return false;
	}

	string strOpenMode;
	if (eWriteMode_OverWrite == eMode){
		strOpenMode = "wb+";
	}else if(eWriteMode_Apend == eMode){
		strOpenMode = "ab+";
	} else {
		strOpenMode = "wb+";
	}

	int iOpenFlag = _SH_SECURE;
	if (bShare){
		iOpenFlag = _SH_DENYNO;
	}

	FILE* pFile = NULL;
	pFile = _fsopen(strFilePath.c_str(), strOpenMode.c_str(), iOpenFlag);
	if (!pFile){
		return false;
	}

	fseek(pFile, 0, SEEK_END);
	unsigned long dwWriteSize = fwrite(pBuffer, 1, dwBuffersize, pFile);
	fclose(pFile);

	if (dwWriteSize == dwBuffersize){
		return true;
	} else {
		return false;
	}
}

bool CommonFunction::CreatFlag(string strFlagPath)
{
	if (IsPathExist(strFlagPath)){
		return true;
	}

	FILE* pFile = _fsopen(strFlagPath.c_str(), "w+", _SH_DENYNO);
	if (!pFile){
		return false;
	}

	fclose(pFile);
	return true;
}


//////////////////////////////////// folder //////////////////////////////////////
bool CommonFunction::IsPathExist(const string& strPath)
{
	wstring wstrPath = CommonFunction::StringToWstring(strPath, CP_UTF8);
	string strPathANSI = CommonFunction::WstringToString(wstrPath, CP_ACP);
	if (0 == _access(strPathANSI.c_str(), 0)){
		return true;
	}

	return false;
}

bool CommonFunction::IsPathExist(const wstring& wstrPath)
{
	string strPathANSI = CommonFunction::WstringToString(wstrPath, CP_ACP);
	if (0 == _access(strPathANSI.c_str(), 0)){
		return true;
	}

	return false;
}

bool CommonFunction::CreatePath(const string& strPath)
{
	wstring wstrPath = CommonFunction::StringToWstring(strPath, CP_UTF8);
	string strPathANSI = CommonFunction::WstringToString(wstrPath, CP_ACP);
	int iStartIndex = 0;
	int iTempIndex = -1;
	do 
	{
		iTempIndex = strPathANSI.find(L'\\', iStartIndex);
		if (-1 != iTempIndex){
			iStartIndex = iTempIndex + 1;
			string strTempPath(strPathANSI.c_str(), iTempIndex);
			if (0 != _access(strTempPath.c_str(), 0)){
				CreateDirectoryA(strTempPath.c_str(), NULL);
			}
			if (0 != _access(strTempPath.c_str(), 0)){
				return false;
			}
		}
	} while(iTempIndex != -1);

	if (0 != _access(strPathANSI.c_str(), 0)){
		CreateDirectoryA(strPathANSI.c_str(), NULL);
	}
	if (0 != _access(strPathANSI.c_str(), 0)){
		return false;
	}

	return true;
}

bool CommonFunction::CreatePath(const wstring& strPath)
{
	string strPathANSI = CommonFunction::WstringToString(strPath, CP_ACP);
	int iStartIndex = 0;
	int iTempIndex = -1;
	do 
	{
		iTempIndex = strPathANSI.find(L'\\', iStartIndex);
		if (-1 != iTempIndex){
			iStartIndex = iTempIndex + 1;
			string strTempPath(strPathANSI.c_str(), iTempIndex);
			if (0 != _access(strTempPath.c_str(), 0)){
				CreateDirectoryA(strTempPath.c_str(), NULL);
			}
			if (0 != _access(strTempPath.c_str(), 0)){
				return false;
			}
		}
	} while(iTempIndex != -1);

	if (0 != _access(strPathANSI.c_str(), 0)){
		CreateDirectoryA(strPathANSI.c_str(), NULL);
	}
	if (0 != _access(strPathANSI.c_str(), 0)){
		return false;
	}

	return true;
}

bool CommonFunction::IsPathDirectory(const char *str)
{
	if (FILE_ATTRIBUTE_DIRECTORY == PathIsDirectoryA(str)){
		return true;
	}

	return false;
}

//sample code
/*
{
	string strPath = L"D:\\Temp";
	STRING_LIST folderList;
	STRING_LIST fileList;
	bool bret = GetFileList(strPath, folderList, fileList);
}*/
bool CommonFunction::GetFileList(const string& strPath, STRING_LIST& folderList, STRING_LIST& fileList)
{
	folderList.clear();
	fileList.clear();

	// check if folder exist
	if (FILE_ATTRIBUTE_DIRECTORY != PathIsDirectoryA(strPath.c_str())){
		return false;
	}

	bool bRet = false;
	_finddata_t dir_info;
	_finddata_t file_info;
	intptr_t f_handle;
	string strPathTemp = strPath + "\\*";
	if((f_handle = _findfirst(strPathTemp.c_str(), &dir_info)) != -1)
	{
		while(0 == _findnext(f_handle, &file_info))
		{
			// special dir
			if(0 == strcmp(file_info.name, "..") || 0 == strcmp(file_info.name, ".")){
				continue;
			}

			if (file_info.attrib & _A_SUBDIR ) // dir
			{
				string strNewPath = strPath + "\\" + file_info.name;
				folderList.push_back(strNewPath);
			}
			else // file
			{
				string strFullPath = strPath + "\\" + file_info.name;
				fileList.push_back(strFullPath);
			}
		}

		_findclose(f_handle);

		bRet = true;
	}

	return bRet;
}

//sample code
/*
{
	string strPath = L"D:\\Temp";
	STRING_LIST folderList;
	STRING_LIST fileList;
	bool bret = GetFileList(strPath, folderList, fileList);
}*/
bool CommonFunction::GetFileListRelative(const string& strPath, STRING_LIST& folderList, STRING_LIST& fileList)
{
	folderList.clear();
	fileList.clear();

	// check if folder exist
	if (FILE_ATTRIBUTE_DIRECTORY != PathIsDirectoryA(strPath.c_str())){
		return false;
	}

	bool bRet = false;
	_finddata_t dir_info;
	_finddata_t file_info;
	intptr_t f_handle;
	string strPathTemp = strPath + "\\*";
	if((f_handle = _findfirst(strPathTemp.c_str(), &dir_info)) != -1)
	{
		while(0 == _findnext(f_handle, &file_info))
		{
			// special dir
			if(0 == strcmp(file_info.name, "..") || 0 == strcmp(file_info.name, ".")){
				continue;
			}

			if (file_info.attrib & _A_SUBDIR ) // dir
			{
				string strNewPath = file_info.name;
				folderList.push_back(strNewPath);
			}
			else // file
			{
				string strFullPath = file_info.name;
				fileList.push_back(strFullPath);
			}
		}

		_findclose(f_handle);

		bRet = true;
	}

	return bRet;
}

void CommonFunction::ClearSubFolderByFlagFile(const char* strFolderPath, const char* strFlagFileName)
{
	// check if folder exist
	if (!IsPathDirectory(strFolderPath)){
		return ;
	}

	_finddata_t dir_info;
	_finddata_t file_info;
	intptr_t f_handle;
	string strPathTemp = string(strFolderPath) + "\\*";
	if((f_handle = _findfirst(strPathTemp.c_str(), &dir_info)) != -1)
	{
		while(_findnext(f_handle, &file_info) == 0)
		{
			// special dir
			if(strcmp(file_info.name, "..") == 0 
				|| strcmp(file_info.name, ".") == 0){
					continue;
			}

			if (file_info.attrib & _A_SUBDIR ) // dir
			{
				// delete dir
				string strNewPath = string(strFolderPath) + "\\" + file_info.name;
				string strFlagFullPath = strNewPath + "\\" + strFlagFileName;
				if (IsPathExist(strFlagFullPath)){
					DeleteDirectory(strNewPath);
				}
			}
		}
		// close file before delete empty dir
		_findclose(f_handle);
	}
}

void CommonFunction::DeleteFileNodeList(MyFileNode*& pFolderInfo)
{
	while (pFolderInfo)
	{
		MyFileNode* pTemp = pFolderInfo->pNext;
		delete pFolderInfo;
		pFolderInfo = pTemp;
	}
}

void CommonFunction::DeleteFolderNodeList(FolderNode*& pFolderInfo)
{
	if (!pFolderInfo){
		return;
	}

	struct FolderNodeDelete
	{
		FolderNode* pFolderNode;
		FolderNodeDelete* pNext;
		FolderNodeDelete(){
			pFolderNode = NULL;
			pNext = NULL;
		}
	};

	FolderNode* pFolderInfoTemp = pFolderInfo;
	FolderNodeDelete* pDeleteList = new FolderNodeDelete;
	if (!pDeleteList){
		return;
	}

	pDeleteList->pFolderNode = pFolderInfo;
	while (pDeleteList){
		if (pDeleteList->pFolderNode && pDeleteList->pFolderNode->pSubFolderList){
			FolderNodeDelete* pDeleteNodeTemp = new FolderNodeDelete;
			if (!pDeleteNodeTemp){
				return;
			}
			pDeleteNodeTemp->pFolderNode = pDeleteList->pFolderNode->pSubFolderList;
			pDeleteNodeTemp->pNext = pDeleteList->pNext;
			pDeleteList->pNext = pDeleteNodeTemp;
		}

		if (pDeleteList->pFolderNode && pDeleteList->pFolderNode->pNext){
			FolderNodeDelete* pDeleteNodeTemp = new FolderNodeDelete;
			if (!pDeleteNodeTemp){
				return;
			}
			pDeleteNodeTemp->pFolderNode = pDeleteList->pFolderNode->pNext;
			pDeleteNodeTemp->pNext = pDeleteList->pNext;
			pDeleteList->pNext = pDeleteNodeTemp;
		}

		FolderNodeDelete* pDeleteNodeTemp = pDeleteList->pNext;
		DeleteFileNodeList(pDeleteList->pFolderNode->pFileList);
		delete pDeleteList->pFolderNode;
		pDeleteList->pFolderNode = NULL;
		delete pDeleteList;
		pDeleteList = pDeleteNodeTemp;
	}

	pFolderInfo = NULL;
}

void CommonFunction::DeleteFolderInfoNodeList(FolderInfoNode*& pFolderNodeList, bool bDeleteFolderNode)
{
	while(pFolderNodeList)
	{
		FolderInfoNode* pTemp = pFolderNodeList->pNext;
		if (bDeleteFolderNode){
			DeleteFolderNodeList(pFolderNodeList->pFolderInfo);
		}

		delete pFolderNodeList;
		pFolderNodeList = pTemp;
	}
}

bool CommonFunction::GetFolderInfoLoop(string strSource, FolderNode*& pFolderInfo, DWORD& dwFolderSize, DWORD& dwFileCount, DWORD& dwFolderCount)
{
	if (!IsPathExist(strSource)){
		//LOGOUT_ERR("no source[%s]", strSource.c_str());
		return false;
	}

	if (pFolderInfo){
		return false;
	}
	pFolderInfo = new FolderNode;
	if (!pFolderInfo){
		return false;
	}
	pFolderInfo->strFullPath = strSource;

	dwFolderSize = 0;
	dwFileCount = 0;
	dwFolderCount = 0;
	bool bRet = true;

	FolderInfoNode* pNodeList = new FolderInfoNode;
	if (!pNodeList){
		return false;
	}
	pNodeList->strFullPath = strSource;
	pNodeList->pFolderInfo = pFolderInfo;
	while (pNodeList)
	{
		_finddata_t dir_info;
		_finddata_t file_info;
		intptr_t f_handle;
		if((f_handle = _findfirst((pNodeList->strFullPath + "\\*").c_str(), &dir_info)) != -1)
		{
			while(0 == _findnext(f_handle, &file_info))
			{
				if(0 == strcmp(file_info.name, "..") || 0 == strcmp(file_info.name, ".")){
					continue;
				}

				if (file_info.attrib & _A_SUBDIR ) // dir
				{
					FolderNode* pFolderInfoTemp = new FolderNode;
					if (!pFolderInfoTemp){
						bRet = false;
					}else{
						pFolderInfoTemp->strFolderName = file_info.name;
						pFolderInfoTemp->strFullPath =  pNodeList->strFullPath + "\\"+ file_info.name;
						pFolderInfoTemp->pNext = pNodeList->pFolderInfo->pSubFolderList;
						pNodeList->pFolderInfo->pSubFolderList = pFolderInfoTemp;

						FolderInfoNode* pNodeTemp = new FolderInfoNode;
						if (!pNodeTemp)	{
							bRet = false;
						}else{
							pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pSubFolderList;
							pNodeTemp->strFullPath = pNodeList->strFullPath + "\\"+ file_info.name;
							pNodeTemp->pNext = pNodeList->pNext;
							pNodeList->pNext = pNodeTemp;
						}
						++dwFolderCount;
					}
				}
				else // file
				{
					MyFileNode* pFile = new MyFileNode;
					if (!pFile){
						bRet = false;
					}else{
						pFile->strFileName = file_info.name;
						pFile->pNext = pNodeList->pFolderInfo->pFileList;
						pNodeList->pFolderInfo->pFileList = pFile;

						dwFolderSize += file_info.size;
						++dwFileCount;
					}
				}

				if (!bRet){
					break;
				}
			}

			_findclose(f_handle);
		}

		if (!bRet){
			break;
		}

		FolderInfoNode* pNodeTemp = pNodeList->pNext;
		delete pNodeList;
		pNodeList = pNodeTemp;
	}

	if (!bRet){
		DeleteFolderNodeList(pFolderInfo);
		DeleteFolderInfoNodeList(pNodeList, true);
	}

	return bRet;
}

bool CommonFunction::DeleteDirectory(const string& strPath)
{
	if (strPath == ""){
		LOGOUT_ERR("path NULL");
		return false;
	}

	bool bRet = false;
	int iRetryTime = 3;
	do 
	{
		__DeleteDirectory(strPath);
		if (!IsPathExist(strPath)){
			bRet = true;
			break;
		}
	} while(--iRetryTime > 0);

	return bRet;
}

bool CommonFunction::__DeleteDirectory(const string& strPath)
{
	if (!IsPathExist(strPath)){
		return false;
	}

	bool bRet = false;
	FolderNode* pFolderInfo = NULL;
	do
	{
		DWORD dwFolderCount(0);
		DWORD dwFileCount(0);
		DWORD dwFileSize(0);
		bRet = GetFolderInfoLoop(strPath, pFolderInfo, dwFileSize, dwFileCount, dwFolderCount);
		if (!bRet || !pFolderInfo){
			break;
		}

		bRet = DeleteFolderLoop(pFolderInfo);
	}while(0);

	DeleteFolderNodeList(pFolderInfo);

	// delete by other
	if (!bRet && !CommonFunction::IsPathExist(strPath)){
		bRet = true;
	}

	return bRet;
}

bool CommonFunction::DeleteDirectory(const string& strPath, const STRING_LIST& listIgnoreFolder)
{
	if (strPath == ""){
		LOGOUT_ERR("path NULL");
		return false;
	}

	bool bRet = false;
	int iRetryTime = 3;
	do 
	{
		__DeleteDirectory(strPath, listIgnoreFolder);
		if (!IsPathExist(strPath)){
			bRet = true;
			break;
		}
	} while(--iRetryTime > 0);

	return bRet;
}

bool CommonFunction::__DeleteDirectory(const string& strPath, const STRING_LIST& listIgnoreFolder)
{
	if (!IsPathExist(strPath)){
		return false;
	}

	bool bRet = false;
	FolderNode* pFolderInfo = NULL;
	do
	{
		DWORD dwFolderCount(0);
		DWORD dwFileCount(0);
		DWORD dwFileSize(0);
		bRet = GetFolderInfoLoop(strPath, pFolderInfo, dwFileSize, dwFileCount, dwFolderCount);
		if (!bRet || !pFolderInfo){
			break;
		}

		bRet = DeleteFolderLoop(pFolderInfo);
	}while(0);

	DeleteFolderNodeList(pFolderInfo);

	// delete by other
	if (!bRet && !CommonFunction::IsPathExist(strPath)){
		bRet = true;
	}

	return bRet;
}

bool CommonFunction::GetFolderInfoLoop(string strSource, FolderNode*& pFolderInfo, DWORD& dwFolderSize, DWORD& dwFileCount, DWORD& dwFolderCount, const STRING_LIST& listIgnoreFolder)
{
	if (!IsPathExist(strSource)){
		//LOGOUT_ERR("no source[%s]", strSource.c_str());
		return false;
	}

	if (pFolderInfo){
		return false;
	}
	pFolderInfo = new FolderNode;
	if (!pFolderInfo){
		return false;
	}
	pFolderInfo->strFullPath = strSource;

	dwFolderSize = 0;
	dwFileCount = 0;
	dwFolderCount = 0;
	bool bRet = true;

	FolderInfoNode* pNodeList = new FolderInfoNode;
	if (!pNodeList){
		return false;
	}
	pNodeList->strFullPath = strSource;
	pNodeList->pFolderInfo = pFolderInfo;
	while (pNodeList)
	{
		_finddata_t dir_info;
		_finddata_t file_info;
		intptr_t f_handle;
		if((f_handle = _findfirst((pNodeList->strFullPath + "\\*").c_str(), &dir_info)) != -1)
		{
			while(0 == _findnext(f_handle, &file_info))
			{
				if(0 == strcmp(file_info.name, "..") || 0 == strcmp(file_info.name, ".")){
					continue;
				}

				if (file_info.attrib & _A_SUBDIR ) // dir
				{
					// check ignore folder // 
					//////////////////////////////////////////////////////////////////////////
					{
						bool bCheckIgnoreRet = false;
						string strCheckedPath = pNodeList->strFullPath + "\\"+ file_info.name;
						STRING_LIST list = listIgnoreFolder;
						for (STRING_LIST::iterator itr = list.begin(); itr != list.end(); ++itr)
						{
							string strIgnore = *itr;
							if (strCheckedPath.length() >= strIgnore.length()
								&& string(strCheckedPath.c_str() + strCheckedPath.length() - strIgnore.length(), strIgnore.length()) == strIgnore)
							{
								bCheckIgnoreRet = true;
								break;
							}
						}

						if (bCheckIgnoreRet){
							continue;
						}
					}

					//////////////////////////////////////////////////////////////////////////

					FolderNode* pFolderInfoTemp = new FolderNode;
					if (!pFolderInfoTemp){
						bRet = false;
					}else{
						pFolderInfoTemp->strFolderName = file_info.name;
						pFolderInfoTemp->strFullPath =  pNodeList->strFullPath + "\\"+ file_info.name;
						pFolderInfoTemp->pNext = pNodeList->pFolderInfo->pSubFolderList;
						pNodeList->pFolderInfo->pSubFolderList = pFolderInfoTemp;

						FolderInfoNode* pNodeTemp = new FolderInfoNode;
						if (!pNodeTemp)	{
							bRet = false;
						}else{
							pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pSubFolderList;
							pNodeTemp->strFullPath = pNodeList->strFullPath + "\\"+ file_info.name;
							pNodeTemp->pNext = pNodeList->pNext;
							pNodeList->pNext = pNodeTemp;
						}
						++dwFolderCount;
					}
				}
				else // file
				{
					MyFileNode* pFile = new MyFileNode;
					if (!pFile){
						bRet = false;
					}else{
						pFile->strFileName = file_info.name;
						pFile->pNext = pNodeList->pFolderInfo->pFileList;
						pNodeList->pFolderInfo->pFileList = pFile;

						dwFolderSize += file_info.size;
						++dwFileCount;
					}
				}

				if (!bRet){
					break;
				}
			}

			_findclose(f_handle);
		}

		if (!bRet){
			break;
		}

		FolderInfoNode* pNodeTemp = pNodeList->pNext;
		delete pNodeList;
		pNodeList = pNodeTemp;
	}

	if (!bRet){
		DeleteFolderNodeList(pFolderInfo);
		DeleteFolderInfoNodeList(pNodeList, true);
	}

	return bRet;
}


bool CommonFunction::DeleteFolderLoop(FolderNode* pFolderInfo)
{
	if (!pFolderInfo){
		return true;
	}

	FolderInfoNode* pNodeList = new FolderInfoNode;
	if (!pNodeList){
		return false;
	}
	pNodeList->pFolderInfo = pFolderInfo;

	FolderInfoNode* pEmptyFolderList = new FolderInfoNode;
	if (!pEmptyFolderList){
		return false;
	}

	bool bRet = true;
	while (pNodeList)
	{
		// get folder path
		string strDestPath = pNodeList->pFolderInfo->strFullPath;

		// delete files
		MyFileNode* pFileTemp = pNodeList->pFolderInfo->pFileList;
		while (pFileTemp){
			string strS = pNodeList->pFolderInfo->strFullPath + "\\" + pFileTemp->strFileName;
			if(!DeleteFileA(strS.c_str())){
				bRet = false;
				break;
			}

			pFileTemp = pFileTemp->pNext;
		}
		if (!bRet){
			break;
		}

		// add sub folder
		if (pNodeList->pFolderInfo->pSubFolderList)	{
			FolderInfoNode* pNodeTemp = new FolderInfoNode;
			if (!pNodeTemp){
				bRet = false;
				break;
			}
			pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pSubFolderList;
			pNodeTemp->pNext = pNodeList->pNext;
			pNodeList->pNext = pNodeTemp;
		}

		// add next folder
		if (pNodeList->pFolderInfo->pNext)	{
			FolderInfoNode* pNodeTemp = new FolderInfoNode;
			if (!pNodeTemp){
				bRet = false;
				break;
			}
			pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pNext;
			pNodeTemp->pNext = pNodeList->pNext;
			pNodeList->pNext = pNodeTemp;
		}

		// add empty folder list
		FolderInfoNode* pEmptyFolderTemp = new FolderInfoNode;
		if (!pEmptyFolderTemp){
			bRet = false;
			break;
		}
		pEmptyFolderTemp->strFullPath = strDestPath;
		pEmptyFolderTemp->pNext = pEmptyFolderList->pNext;
		pEmptyFolderList->pNext = pEmptyFolderTemp;

		// delete the finished folder
		FolderInfoNode* pNodeTemp = pNodeList->pNext;
		delete pNodeList;
		pNodeList = pNodeTemp;
	}

	if (!bRet){
		DeleteFolderInfoNodeList(pNodeList, false);
		DeleteFolderInfoNodeList(pEmptyFolderList, false);
	}else{
		while (pEmptyFolderList)
		{
			if ("" != pEmptyFolderList->strFullPath)
			{
				int iRetryTime = 3;
				bool bDeleteTemp = true;
				do 
				{
					bDeleteTemp = RemoveDirectoryA(pEmptyFolderList->strFullPath.c_str());
				} while(!bDeleteTemp && --iRetryTime > 0);

				if (!bDeleteTemp){
					bRet = false;
				}
			}

			FolderInfoNode* pNodeTemp = pEmptyFolderList->pNext;
			delete pEmptyFolderList;
			pEmptyFolderList = pNodeTemp;
		}
	}

	return bRet;
}

bool CommonFunction::CopyDirectory(string strSourcePath, string strDestPath)
{
	if (IsPathExist(strDestPath)){
		return false;
	}

	bool bRet = false;
	FolderNode* pFolderInfo = NULL;
	do
	{
		DWORD dwFolderCount(0);
		DWORD dwFileCount(0);
		DWORD dwFileSize(0);
		bRet = CommonFunction::GetFolderInfoLoop(strSourcePath, pFolderInfo, dwFileSize, dwFileCount, dwFolderCount);
		if (!bRet || !pFolderInfo){
			break;
		}

		bRet = CopyFolderLoop(pFolderInfo, strDestPath);
	}while(0);

	DeleteFolderNodeList(pFolderInfo);

	return bRet;
}

bool CommonFunction::CopyFolderLoop(FolderNode* pFolderInfo, const string& strDestFolder)
{
	if (!pFolderInfo){
		return true;
	}

	FolderInfoNode* pNodeList = new FolderInfoNode;
	if (!pNodeList){
		return false;
	}
	pNodeList->strFullPath = strDestFolder;
	pNodeList->pFolderInfo = pFolderInfo;

	bool bRet = true;
	while (pNodeList)
	{
		// create folder
		string strDestPath = ("" !=  pNodeList->pFolderInfo->strFolderName) ? (pNodeList->strFullPath + "\\" + pNodeList->pFolderInfo->strFolderName) : (pNodeList->strFullPath);
		CreateDirectoryA(strDestPath.c_str(), NULL);
		if (!IsPathExist(strDestPath)){
			bRet = false;
			break;
		}

		// copy files
		MyFileNode* pFileTemp = pNodeList->pFolderInfo->pFileList;
		while (pFileTemp){
			string strS = pNodeList->pFolderInfo->strFullPath + "\\" + pFileTemp->strFileName;
			string strD = strDestPath + "\\" + pFileTemp->strFileName;
			if(!CopyFileA(strS.c_str(), strD.c_str(), true)){
				bRet = false;
				break;
			}

			pFileTemp = pFileTemp->pNext;
		}
		if (!bRet){
			break;
		}

		// add sub folder
		if (pNodeList->pFolderInfo->pSubFolderList)	{
			FolderInfoNode* pNodeTemp = new FolderInfoNode;
			if (!pNodeTemp){
				bRet = false;
				break;
			}
			pNodeTemp->strFullPath = strDestPath ;
			pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pSubFolderList;
			pNodeTemp->pNext = pNodeList->pNext;
			pNodeList->pNext = pNodeTemp;
		}

		// add next folder
		if (pNodeList->pFolderInfo->pNext)	{
			FolderInfoNode* pNodeTemp = new FolderInfoNode;
			if (!pNodeTemp){
				bRet = false;
				break;
			}
			pNodeTemp->strFullPath = pNodeList->strFullPath;
			pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pNext;
			pNodeTemp->pNext = pNodeList->pNext;
			pNodeList->pNext = pNodeTemp;
		}

		// delete the finished folder
		FolderInfoNode* pNodeTemp = pNodeList->pNext;
		delete pNodeList;
		pNodeList = pNodeTemp;
	}

	if (!bRet){
		DeleteFolderInfoNodeList(pNodeList, false);
	}

	return bRet;
}

bool CommonFunction::ReplaceDirectory(string source, string target)
{
	bool bRet = false;
	FolderNode* pFolderInfo = NULL;
	do
	{
		DWORD dwFolderCount(0);
		DWORD dwFileCount(0);
		DWORD dwFileSize(0);
		bRet = CommonFunction::GetFolderInfoLoop(source, pFolderInfo, dwFileSize, dwFileCount, dwFolderCount);
		if (!bRet || !pFolderInfo){
			break;
		}

		bRet = ReplaceFolderLoop(pFolderInfo, target);
	}while(0);

	DeleteFolderNodeList(pFolderInfo);

	return bRet;
}

bool CommonFunction::ReplaceFolderLoop(FolderNode* pFolderInfo, const string& strDestFolder)
{
	if (!pFolderInfo){
		return true;
	}

	FolderInfoNode* pNodeList = new FolderInfoNode;
	if (!pNodeList){
		return false;
	}
	pNodeList->strFullPath = strDestFolder;
	pNodeList->pFolderInfo = pFolderInfo;

	bool bRet = true;
	while (pNodeList)
	{
		// create folder
		string strDestPath = ("" !=  pNodeList->pFolderInfo->strFolderName) ? (pNodeList->strFullPath + "\\" + pNodeList->pFolderInfo->strFolderName) : (pNodeList->strFullPath);
		CreateDirectoryA(strDestPath.c_str(), NULL);
		if (!IsPathExist(strDestPath)){
			bRet = false;
			break;
		}

		// copy files
		MyFileNode* pFileTemp = pNodeList->pFolderInfo->pFileList;
		while (pFileTemp){
			string strS = pNodeList->pFolderInfo->strFullPath + "\\" + pFileTemp->strFileName;
			string strD = strDestPath + "\\" + pFileTemp->strFileName;
			if(!CopyFileA(strS.c_str(), strD.c_str(), false)){
				bRet = false;
				break;
			}

			pFileTemp = pFileTemp->pNext;
		}
		if (!bRet){
			break;
		}

		// add sub folder
		if (pNodeList->pFolderInfo->pSubFolderList)	{
			FolderInfoNode* pNodeTemp = new FolderInfoNode;
			if (!pNodeTemp){
				bRet = false;
				break;
			}
			pNodeTemp->strFullPath = strDestPath ;
			pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pSubFolderList;
			pNodeTemp->pNext = pNodeList->pNext;
			pNodeList->pNext = pNodeTemp;
		}

		// add next folder
		if (pNodeList->pFolderInfo->pNext)	{
			FolderInfoNode* pNodeTemp = new FolderInfoNode;
			if (!pNodeTemp){
				bRet = false;
				break;
			}
			pNodeTemp->strFullPath = pNodeList->strFullPath;
			pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pNext;
			pNodeTemp->pNext = pNodeList->pNext;
			pNodeList->pNext = pNodeTemp;
		}

		// delete the finished folder
		FolderInfoNode* pNodeTemp = pNodeList->pNext;
		delete pNodeList;
		pNodeList = pNodeTemp;
	}

	if (!bRet){
		DeleteFolderInfoNodeList(pNodeList, false);
	}

	return bRet;
}

bool CommonFunction::CutReplaceDirectory(string source, string target)
{
	bool bRet = false;
	FolderNode* pFolderInfo = NULL;
	do
	{
		DWORD dwFolderCount(0);
		DWORD dwFileCount(0);
		DWORD dwFileSize(0);
		bRet = GetFolderInfoLoop(source, pFolderInfo, dwFileSize, dwFileCount, dwFolderCount);
		if (!bRet || !pFolderInfo){
			break;
		}

		bRet = CutReplaceFolderLoop(pFolderInfo, target);
	}while(0);

	DeleteFolderNodeList(pFolderInfo);

	if (bRet){
		DeleteDirectory(source);
	}

	return bRet;
}

bool CommonFunction::CutReplaceFolderLoop(FolderNode* pFolderInfo, const string& strDestFolder)
{
	if (!pFolderInfo){
		return true;
	}

	FolderInfoNode* pNodeList = new FolderInfoNode;
	if (!pNodeList){
		return false;
	}
	pNodeList->strFullPath = strDestFolder;
	pNodeList->pFolderInfo = pFolderInfo;

	bool bRet = true;
	while (pNodeList)
	{
		// create folder
		string strDestPath = ("" !=  pNodeList->pFolderInfo->strFolderName) ? (pNodeList->strFullPath + "\\" + pNodeList->pFolderInfo->strFolderName) : (pNodeList->strFullPath);
		CreateDirectoryA(strDestPath.c_str(), NULL);
		if (!IsPathExist(strDestPath)){
			bRet = false;
			break;
		}

		// copy files
		MyFileNode* pFileTemp = pNodeList->pFolderInfo->pFileList;
		while (pFileTemp){
			string strSourceFilePath = pNodeList->pFolderInfo->strFullPath + "\\" + pFileTemp->strFileName;
			string strTargetFilePath = strDestPath + "\\" + pFileTemp->strFileName;

			// delete old file, try to delete again if failure
			DeleteFileA(strTargetFilePath.c_str());
			if (IsPathExist(strTargetFilePath)){
				//LOGOUT_ERR("delete err[%s]", strTargetFilePath.c_str());
				DeleteFileA(strTargetFilePath.c_str());
			}

			if(0 != ::rename(strSourceFilePath.c_str(), strTargetFilePath.c_str())){
				//LOGOUT_ERR("[%s][%s][%d]", strSourceFilePath.c_str(),strTargetFilePath.c_str(), GetLastError());
				bRet = false;
				break;
			}

			pFileTemp = pFileTemp->pNext;
		}
		if (!bRet){
			break;
		}

		// add sub folder
		if (pNodeList->pFolderInfo->pSubFolderList)	{
			FolderInfoNode* pNodeTemp = new FolderInfoNode;
			if (!pNodeTemp){
				bRet = false;
				break;
			}
			pNodeTemp->strFullPath = strDestPath ;
			pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pSubFolderList;
			pNodeTemp->pNext = pNodeList->pNext;
			pNodeList->pNext = pNodeTemp;
		}

		// add next folder
		if (pNodeList->pFolderInfo->pNext)	{
			FolderInfoNode* pNodeTemp = new FolderInfoNode;
			if (!pNodeTemp){
				bRet = false;
				break;
			}
			pNodeTemp->strFullPath = pNodeList->strFullPath;
			pNodeTemp->pFolderInfo = pNodeList->pFolderInfo->pNext;
			pNodeTemp->pNext = pNodeList->pNext;
			pNodeList->pNext = pNodeTemp;
		}

		// delete the finished folder
		FolderInfoNode* pNodeTemp = pNodeList->pNext;
		delete pNodeList;
		pNodeList = pNodeTemp;
	}

	if (!bRet){
		DeleteFolderInfoNodeList(pNodeList, false);
	}

	return bRet;
}


////////////////////////////////// exe ////////////////////////////////////////
string CommonFunction::GetExePath() 
{ 
	// get EXE full path
	char  strExepath[MAX_PATH];
	memset(strExepath,0,MAX_PATH); 
	GetModuleFileNameA(NULL,strExepath,MAX_PATH); 

	string strdir = strExepath; 
	int iTemp = strdir.rfind('\\');
	if (iTemp >= 0 )
	{
		return string(strdir.c_str(), iTemp);
	}
	else
	{
		return strdir;
	}
}

bool CommonFunction::RunBatCmd(const wstring& wstrCmd)
{
	if (wstrCmd != L"")
	{
		DWORD dwThreadID = GetCurrentThreadId();
		DWORD dwTickout = GetTickCount();
		char batName[50] = {0};
		sprintf_s(batName,"%d_%d.bat",dwThreadID,dwTickout);
		string strBatFullPath = GetExePath() + batName;
		WriteContentToFile(strBatFullPath, WstringToString(wstrCmd, CP_ACP), eWriteMode_OverWrite);

		STARTUPINFOA  si; 
		si.cb = sizeof(STARTUPINFOA);
		si.lpReserved = NULL;
		si.lpDesktop = NULL;
		si.lpTitle = NULL;
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		si.cbReserved2 = NULL;
		si.lpReserved2 = NULL;

		PROCESS_INFORMATION  pi; 
		ZeroMemory(&pi, sizeof(pi)); 
		// Start the child process.   
		if( !CreateProcessA(
			NULL, // No module name (use command line).   
			const_cast<char*>(strBatFullPath.c_str()), // Command line.   
			NULL,         // Process handle not inheritable.   
			NULL,         // Thread handle not inheritable.   
			FALSE,         // Set handle inheritance to FALSE.   
			0,           // No creation flags.   
			NULL,         // Use parent's environment block.   
			NULL,         // Use parent's starting directory.   
			&si,           // Pointer to STARTUPINFO structure. 
			&pi )         // Pointer to PROCESS_INFORMATION structure. 
			)   
		{ 
			DeleteFileA(strBatFullPath.c_str());
			return false;
		}  

		// Wait until child process exits. 
		WaitForSingleObject( pi.hProcess, INFINITE ); 
		// Close process and thread handles.   
		CloseHandle( pi.hProcess ); 
		CloseHandle( pi.hThread ); 

		DeleteFileA(strBatFullPath.c_str());
		return true;
	}

	return false;
}

bool CommonFunction::RunBatCmdFile(const string& strCmdFilePath)
{
	if (strCmdFilePath != "")
	{
		STARTUPINFOA  si; 
		si.cb = sizeof(STARTUPINFOA);
		si.lpReserved = NULL;
		si.lpDesktop = NULL;
		si.lpTitle = NULL;
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		si.cbReserved2 = NULL;
		si.lpReserved2 = NULL;

		PROCESS_INFORMATION  pi; 
		ZeroMemory(&pi, sizeof(pi)); 
		// Start the child process.   
		if( !CreateProcessA(
			NULL, // No module name (use command line).   
			const_cast<char*>(strCmdFilePath.c_str()), // Command line.   
			NULL,         // Process handle not inheritable.   
			NULL,         // Thread handle not inheritable.   
			FALSE,         // Set handle inheritance to FALSE.   
			0,           // No creation flags.   
			NULL,         // Use parent's environment block.   
			NULL,         // Use parent's starting directory.   
			&si,           // Pointer to STARTUPINFO structure. 
			&pi )         // Pointer to PROCESS_INFORMATION structure. 
			)   
		{ 
			return false;
		}  

		// Wait until child process exits. 
		WaitForSingleObject( pi.hProcess, INFINITE ); 

		// Close process and thread handles.   
		CloseHandle( pi.hProcess ); 
		CloseHandle( pi.hThread ); 

		return true;
	}

	return false;
}

bool CommonFunction::RunExe(const string& strName, const string& strRunCmd)
{
	STARTUPINFO  si; 
	PROCESS_INFORMATION  pi; 
	ZeroMemory(&si, sizeof(si)); 
	si.cb = sizeof(si); 
	ZeroMemory(&pi, sizeof(pi)); 
	// Start the child process.   
	if( !CreateProcessA(
		strName.c_str(), // No module name (use command line).   
		const_cast<char*>(strRunCmd.c_str()), // Command line.   
		NULL,         // Process handle not inheritable.   
		NULL,         // Thread handle not inheritable.   
		FALSE,         // Set handle inheritance to FALSE.   
		0,           // No creation flags.   
		NULL,         // Use parent's environment block.   
		NULL,         // Use parent's starting directory.   
		&si,           // Pointer to STARTUPINFO structure. 
		&pi )         // Pointer to PROCESS_INFORMATION structure. 
		)   
	{
		return false;
	} 

	// Wait until child process exits. 
	WaitForSingleObject( pi.hProcess, INFINITE ); 

	// Close process and thread handles.   
	CloseHandle( pi.hProcess ); 
	CloseHandle( pi.hThread ); 
	return true;
}


////////////////////////////////// regedit ////////////////////////////////////////
bool CommonFunction::SearchSoftwareInstallInfoFromRegedit(string strSoftwareName, UNINSTALLSOFTWAREINFO& cUninstallInfo, bool bSearchInSys32 /* = true */)
{
	bool bRet = false;
	HKEY hKey = NULL;
	DWORD dwSubKeyIndex = 0;
	char subKeyName[MAX_PATH] = { 0 };
	DWORD charLength = REG_READ_LENGTH_MAX;
	DWORD dwReadRegType = KEY_READ;
	string strSearchKey = REG_SOFTWARE_UNINSTALL_KEY_32;
	if (!bSearchInSys32) {
		strSearchKey = REG_SOFTWARE_UNINSTALL_KEY_64;
		dwReadRegType |= KEY_WOW64_64KEY;
	}

	long lErr = ::RegOpenKeyExA(HKEY_LOCAL_MACHINE, (LPCSTR)strSearchKey.c_str(), 0, dwReadRegType, &hKey);
	if (ERROR_SUCCESS == lErr)
	{	
		while (ERROR_SUCCESS == ::RegEnumKeyEx(hKey, dwSubKeyIndex, subKeyName, &charLength, NULL, NULL, NULL, NULL))
		{
			HKEY hKeySub = NULL;
			string strSubKeyPath = strSearchKey + "\\" + subKeyName;

			if (ERROR_SUCCESS == ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, (LPCSTR)strSubKeyPath.c_str(), 0, dwReadRegType, &hKeySub))
			{
				char strValue[MAX_PATH];
				DWORD dwLength = MAX_PATH;
				DWORD dwType = REG_SZ;

				bool bMatch = false;
				if (string(subKeyName) == strSoftwareName){
					bMatch = true;
				}else if (ERROR_SUCCESS == ::RegQueryValueEx(hKeySub, (LPCSTR)s_strInstallInfoKeyList[0].c_str(), 0, &dwType, (LPBYTE)strValue, &dwLength)
					&& string(strValue) == strSoftwareName){
						bMatch = true;
				}

				if (bMatch)
				{
					bRet = true;
					for (DWORD dwIndex = 0; dwIndex < INSTALL_INFO_KEY_COUNT; ++dwIndex)
					{
						dwLength = MAX_PATH;
						DWORD dwTypeSZ = REG_SZ;
						DWORD dwTypeSZEX = REG_EXPAND_SZ;
						if (ERROR_SUCCESS == ::RegQueryValueEx(hKeySub, (LPCSTR)s_strInstallInfoKeyList[dwIndex].c_str(), 0, &dwTypeSZ, (LPBYTE)strValue, &dwLength)) {
							cUninstallInfo.SetValueByKey(s_strInstallInfoKeyList[dwIndex], strValue);
						} else if (ERROR_SUCCESS == ::RegQueryValueEx(hKeySub, (LPCSTR)s_strInstallInfoKeyList[dwIndex].c_str(), 0, &dwTypeSZEX, (LPBYTE)strValue, &dwLength)) {
							cUninstallInfo.SetValueByKey(s_strInstallInfoKeyList[dwIndex], strValue);
						} else {
							cUninstallInfo.SetValueByKey(s_strInstallInfoKeyList[dwIndex], "");
						}
					}
					break;
				}
			}

			if (hKeySub != NULL) {
				::RegCloseKey(hKeySub);
			}

			++dwSubKeyIndex;
			charLength = REG_READ_LENGTH_MAX;
		}
	}
	else
	{
		LOGOUT_ERR("[%s][%d]",strSoftwareName.c_str(), GetLastError());
	}

	if (hKey != NULL) {
		::RegCloseKey(hKey);
	}

	return bRet;
}



///////////////////////////////// other /////////////////////////////////////////
void CommonFunction::GetCurrentDate(string &strDateStr)
{
	strDateStr = "";
	time_t now;
	struct tm* pTimeInfo;
	time ( &now );
	pTimeInfo = localtime( &now );
	if (pTimeInfo)
	{
		pTimeInfo->tm_year = pTimeInfo->tm_year + 1900;
		pTimeInfo->tm_mon = pTimeInfo->tm_mon + 1;

		char szTmp[50] = {0};
		_snprintf_s(szTmp, sizeof(szTmp), "%04d-%02d-%02d %02d:%02d:%02d",
			pTimeInfo->tm_year, pTimeInfo->tm_mon, pTimeInfo->tm_mday,pTimeInfo->tm_hour,pTimeInfo->tm_min,pTimeInfo->tm_sec);
		strDateStr = szTmp;
	}
}

int CommonFunction::GetTimeStamp()
{
	time_t now;
	struct tm* timeinfo;
	time ( &now );
	timeinfo = localtime( &now );
	if (timeinfo)
	{
		return mktime(timeinfo);
	}
	else
	{
		return 0;
	}
}

int CommonFunction::StrToTime(char* const date)
{
	struct tm tm;
	int year = 0, month = 0, day = 0, hour = 0, minute = 0, second = 0;
	sscanf(date,"%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
	tm.tm_year  = year-1900;
	tm.tm_mon   = month-1;
	tm.tm_mday  = day;
	tm.tm_hour  = hour;
	tm.tm_min   = minute;
	tm.tm_sec   = second;
	tm.tm_isdst = 0;
	time_t ft=mktime(&tm);
	return ft;
}

string CommonFunction::GetPCName()
{
	DWORD dwSize = 0;
	string strName("");
	GetComputerNameA(NULL, &dwSize);
	if (dwSize > 0 && dwSize < MAX_PACKAGE_NAME)
	{
		char *pBuffer = new char[dwSize + 1];
		if (!pBuffer){
			return "";
		}

		memset(pBuffer, 0x00, dwSize + 1);
		if(GetComputerNameA(pBuffer,&dwSize)){
			strName = pBuffer;
		}
		delete []pBuffer;
		return strName;
	}
	else
	{
		return "";
	}
}

#ifdef _DEBUG
void CommonFunction::XdPrint(char* format, ...) 
{ 
	va_list args; 
	va_start(args, format); 
	char buf[1024]; 
	vsnprintf(buf, 1024, format, args);
	//vsntprintf(buf, format, args);
	va_end(args); 
	string str(buf);
	wstring wstr = StringToWstring(str, CP_UTF8);
	str = WstringToString(wstr, CP_ACP);
	OutputDebugString(str.c_str()); 
}
#else
void CommonFunction::XdPrint(char* format, ...) {}
#endif

bool CommonFunction::IsDigit(const string& str)
{
	string::const_iterator it = str.begin();
	if (it == str.end())
	{
		return false;
	}

	if ('0' == *it && str.length() > 1)
	{
		return false;
	}

	for (; it != str.end(); ++it)
	{
		if (*it < '0' || *it > '9')
		{
			return false;
		}
	}

	return true;
}

bool CommonFunction::ClearStrPrefix(
	const string& strSrc, 
	const string& strPrefix, 
	bool bRecursion, 
	string& strNew)
{
	if (0 == strSrc.length())
	{
		return false;
	}

	string temp = strSrc;

	while ((temp.length() > 0) 
		&& (string(temp, 0, 1) == strPrefix))
	{
		temp = string(temp, 1, temp.length());

		if (!bRecursion)
		{
			break;
		}
	}

	strNew = temp;
	return true;
}

void CommonFunction::CjsonMergeDestToSrc(cJSON* pJsSrcJson, cJSON* pJsDestJson)
{
	if (!pJsSrcJson)
	{
		return;
	}

	if (!pJsDestJson)
	{
		return;
	}

	cJSON* pJsChild = pJsDestJson->child;
	while (pJsChild)
	{
		cJSON_DeleteItemFromObject(pJsSrcJson, pJsChild->string);

		if (cJSON_String == pJsChild->type)
		{
			cJSON_AddStringToObject(pJsSrcJson, pJsChild->string, pJsChild->valuestring);
		}
		else if (cJSON_Number == pJsChild->type)
		{
			cJSON_AddNumberToObject(pJsSrcJson, pJsChild->string, pJsChild->valueint);
		}
		else if (cJSON_True == pJsChild->type)
		{
			cJSON_AddTrueToObject(pJsSrcJson, pJsChild->string);
		}
		else if (cJSON_False == pJsChild->type)
		{
			cJSON_AddFalseToObject(pJsSrcJson, pJsChild->string);
		}

		pJsChild = pJsChild->next;
	}
}

void CommonFunction::CjsonMergePairedArrayItemToSrc(cJSON* pJsSrcJson, cJSON* pJsDestArrayJson, char* pPairChar)
{
	if (!pJsSrcJson || !pJsDestArrayJson || (cJSON_Array != pJsDestArrayJson->type) || !pPairChar)
	{
		return;
	}

	cJSON* pJsChild = pJsDestArrayJson->child;
	while (pJsChild)
	{
		if (cJSON_Object == pJsChild->type)
		{
			if (CjsonCompare(pJsSrcJson, pJsChild, pPairChar, pPairChar))
			{
				CjsonMergeDestToSrc(pJsSrcJson, pJsChild);
				break;
			}
		}

		pJsChild = pJsChild->next;
	}
}

bool CommonFunction::CjsonCompare(cJSON* pJsSrcJson, cJSON* pJsDestJson, char* pSrcCmpChar, char* pDestCmpChar)
{
	if (!pJsSrcJson || !pJsDestJson || !pSrcCmpChar || !pDestCmpChar)
	{
		return false;
	}

	cJSON* pJsSrcChild = pJsSrcJson->child;
	while (pJsSrcChild)
	{
		if (0 == strcmp(pSrcCmpChar, pJsSrcChild->string))
		{
			cJSON* pJs = cJSON_GetObjectItem(pJsDestJson, pDestCmpChar);
			if (!pJs)
			{
				return false;
			}

			if (cJSON_String == pJsSrcChild->type)
			{
				if (cJSON_String == pJs->type)
				{
					if (0 == strcmp(pJsSrcChild->valuestring, pJs->valuestring))
					{
						return true;
					}
				}
				
			}
			else if (cJSON_Number == pJsSrcChild->type)
			{
				if (cJSON_Number == pJs->type)
				{
					if (pJsSrcChild->valueint == pJs->valueint)
					{
						return true;
					}
				}
			}
			else if (cJSON_True == pJsSrcChild->type)
			{
				if (cJSON_True == pJs->type)
				{
					return true;
				}
			}
			else if (cJSON_False == pJsSrcChild->type)
			{
				if (cJSON_False == pJs->type)
				{
					return true;
				}
			}

			break;
		}

		pJsSrcChild = pJsSrcChild->next;
	}

	return false;
}

bool CommonFunction::CjsonCompareSrcValueAndDestKey(cJSON* pJsSrcJson, cJSON* pJsDestJson, char* pCmpChar)
{
	if (!pJsSrcJson || !pJsDestJson || !pCmpChar)
	{
		return false;
	}

	cJSON* pJsSrcChild = pJsSrcJson->child;
	while (pJsSrcChild)
	{
		if (0 == strcmp(pCmpChar, pJsSrcChild->string))
		{
			if (cJSON_String == pJsSrcChild->type)
			{
				cJSON* pJsDestChilde = pJsDestJson->child;
				while (pJsDestChilde)
				{
					if (0 == strcmp(pJsSrcChild->valuestring, pJsDestChilde->string))
					{
						return true;
					}

					pJsDestChilde = pJsDestChilde->next;
				}
			}
			else if (cJSON_Number == pJsSrcChild->type)
			{
				stringstream stream;
				stream << pJsSrcChild->valueint;
				string s = stream.str();

				cJSON* pJsDestChilde = pJsDestJson->child;
				while (pJsDestChilde)
				{
					if (0 == strcmp(s.c_str(), pJsDestChilde->string))
					{
						return true;
					}

					pJsDestChilde = pJsDestChilde->next;
				}
			}

			break;
		}

		pJsSrcChild = pJsSrcChild->next;
	}

	return false;
}
