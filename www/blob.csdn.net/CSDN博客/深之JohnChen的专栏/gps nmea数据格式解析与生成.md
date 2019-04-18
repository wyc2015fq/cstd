# gps nmea数据格式解析与生成 - 深之JohnChen的专栏 - CSDN博客

2018年12月11日 17:29:05[byxdaz](https://me.csdn.net/byxdaz)阅读数：485


一、nmea数据格式介绍

nmea数据如下： 

$GPGGA,025620.00,2602.33721,N,11911.49176,E,2,04,1.63,13.5,M,9.9,M,,0000*5D

$GPRMC,121252.000,A,3958.3032,N,11629.6046,E,15.15,359.95,070306,,,A*54 

$GNRMC,230134.00,A,2237.12422,N,11408.08310,E,9.291,215.59,310518,,,A*75

$GPVTG,359.95,T,,M,15.15,N,28.0,K,A*04 

$GPGSA,A,3,14,15,05,22,18,26,,,,,,,2.1,1.2,1.7*3D 

$GPGSV,3,1,10,18,84,067,23,09,67,067,27,22,49,312,28,15,47,231,30*70 

$GPGSV,3,2,10,21,32,199,23,14,25,272,24,05,21,140,32,26,14,070,20*7E 

$GPGSV,3,3,10,29,07,074,,30,07,163,28*7D

说明：NMEA0183格式以“$”开始，主要语句有GPGGA，GPVTG，GPRMC等

1、 GPS DOP and Active Satellites（GSA）当前卫星信息

$GPGSA,<1>,<2>,<3>,<3>,,,,,<3>,<3>,<3>,<4>,<5>,<6>,<7><CR><LF>

<1>模式 ：M = 手动， A = 自动。 

<2>定位型式 1 = 未定位， 2 = 二维定位， 3 = 三维定位。 

<3>PRN 数字：01 至 32 表天空使用中的卫星编号，最多可接收12颗卫星信息。 

<4> PDOP位置精度因子（0.5~99.9） 

<5> HDOP水平精度因子（0.5~99.9） 

<6> VDOP垂直精度因子（0.5~99.9） 

<7> Checksum.(检查位).

2、 GPS Satellites in View（GSV）可见卫星信息 

$GPGSV, <1>,<2>,<3>,<4>,<5>,<6>,<7>,?<4>,<5>,<6>,<7>,<8><CR><LF>

<1> GSV语句的总数 

<2> 本句GSV的编号 

<3> 可见卫星的总数，00 至 12。 

<4> 卫星编号， 01 至 32。 

<5>卫星仰角， 00 至 90 度。 

<6>卫星方位角， 000 至 359 度。实际值。 

<7>讯号噪声比（C/No）， 00 至 99 dB；无表未接收到讯号。 

<8>Checksum.(检查位).

第<4>,<5>,<6>,<7>项个别卫星会重复出现，每行最多有四颗卫星。其余卫星信息会于次一行出现，若未使用，这些字段会空白。

3、Global Positioning System Fix Data（GGA）GPS定位信息

$GPGGA,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,M,<10>,M,<11>,<12>*hh<CR><LF>

<1> UTC时间，hhmmss（时分秒）格式 

<2> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输） 

<3> 纬度半球N（北半球）或S（南半球） 

<4> 经度dddmm.mmmm（度分）格式（前面的0也将被传输） 

<5> 经度半球E（东经）或W（西经） 

<6> GPS状态：0=未定位，1=非差分定位，2=差分定位，6=正在估算 

<7> 正在使用解算位置的卫星数量（00~12）（前面的0也将被传输） 

<8> HDOP水平精度因子（0.5~99.9） 

<9> 海拔高度（-9999.9~99999.9） 

<10> 地球椭球面相对大地水准面的高度 

<11> 差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空） 

<12> 差分站ID号0000~1023（前面的0也将被传输，如果不是差分定位将为空）

4、Recommended Minimum Specific GPS/TRANSIT Data（RMC）推荐定位信息

$GPRMC,<1>,<2>,<3>,<4>,<5>,<6>,<7>,<8>,<9>,<10>,<11>,<12>*hh<CR><LF>

<1> UTC时间，hhmmss（时分秒）格式 

<2> 定位状态，A=有效定位，V=无效定位 

<3> 纬度ddmm.mmmm（度分）格式（前面的0也将被传输） 

<4> 纬度半球N（北半球）或S（南半球） 

<5> 经度dddmm.mmmm（度分）格式（前面的0也将被传输） 

<6> 经度半球E（东经）或W（西经） 

<7> 地面速率（000.0~999.9节，前面的0也将被传输） 

<8> 地面航向（000.0~359.9度，以真北为参考基准，前面的0也将被传输） 

<9> UTC日期，ddmmyy（日月年）格式 

<10> 磁偏角（000.0~180.0度，前面的0也将被传输） 

<11> 磁偏角方向，E（东）或W（西） 

<12> 模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）

5、 Track Made Good and Ground Speed（VTG）地面速度信息 

$GPVTG,<1>,T,<2>,M,<3>,N,<4>,K,<5>*hh<CR><LF> 

<1> 以真北为参考基准的地面航向（000~359度，前面的0也将被传输） 

<2> 以磁北为参考基准的地面航向（000~359度，前面的0也将被传输） 

<3> 地面速率（000.0~999.9节，前面的0也将被传输） 

<4> 地面速率（0000.0~1851.8公里/小时，前面的0也将被传输） 

<5> 模式指示（仅NMEA0183 3.00版本输出，A=自主定位，D=差分，E=估算，N=数据无效）

二、gps数据解析与生成类

```
//CommonFunction.h
#pragma once

#include <string>
#include <vector>

// 字符串分割
int StringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator);

// 去掉前后空格
std::string& StringTrim(std::string &str);
```

```
//CommonFunction.cpp
#include "stdafx.h"
#include "CommonFunction.h"

// 字符串分割
int StringSplit(std::vector<std::string>& dst, const std::string& src, const std::string& separator)
{
	if (src.empty() || separator.empty())
		return 0;

	int nCount = 0;
	std::string temp;
	size_t pos = 0, offset = 0;

	// 分割第1~n-1个
	while ((pos = src.find_first_of(separator, offset)) != std::string::npos)
	{
		temp = src.substr(offset, pos - offset);
		if (temp.length() >= 0) {
			dst.push_back(temp);
			nCount++;
		}
		offset = pos + 1;
	}

	// 分割第n个
	temp = src.substr(offset, src.length() - offset);
	if (temp.length() >= 0) {
		dst.push_back(temp);
		nCount++;
	}

	return nCount;
}

//去前后空格
std::string& StringTrim(std::string &str)
{
	if (str.empty()) {
		return str;
	}
	str.erase(0, str.find_first_not_of(" "));
	str.erase(str.find_last_not_of(" ") + 1);
	return str;
}
```

```
//GpsNmeaData.h
//gps nmea 数据解析与生成

#pragma once
#include "CommonFunction.h"

//gpsdata格式
enum GpsDataFormat
{
	GpsDataFormat_None		=		0,			//none
	GpsDataFormat_GNRMC		=		1,			//GNRMC
	GpsDataFormat_GPRMC		=		2,			//GPRMC
	GpsDataFormat_GPGGA		=		3,			//GPGGA
};

//Status
enum GpsStatus
{
	GpsStatus_Valid				=		'A',			//数据有效(GPRMC/GNRMC)
	GpsStatus_Invalid			=		'V',			//数据无效(GPRMC/GNRMC)
	GpsStatus_NoLocation		=		'0',			//未定位(GPGGA)
	GpsStatus_NoDiffLocation	=		'1',			//非差分定位(GPGGA)
	GpsStatus_DiffLocation		=		'2',			//差分定位(GPGGA)
	GpsStatus_Estimating		=		'6',			//正在估算(GPGGA)
};

//纬度方向
enum GpsLatDirect
{
	GpsLatDirect_N		=		'N',			//北纬
	GpsLatDirect_S		=		'S',			//南纬
};

//经度方向
enum GpsLngDirect
{
	GpsLngDirect_E		=		'E',			//东经
	GpsLngDirect_W		=		'W',			//西经
};

//磁偏角方向，E=东，W=西
enum GpsDeclinationDirect
{
	GpsDeclinationDirect_Invalid=		'0',
	GpsDeclinationDirect_E		=		'E',
	GpsDeclinationDirect_W		=		'W',
};

//模式，A=自动，D=差分，E=估测，N=数据无效
enum GpsMode
{
	GpsMode_A			=		'A',
	GpsMode_D			=		'D',
	GpsMode_E			=		'E',
	GpsMode_N			=		'N',
};

class GpsNmeaData
{
public:
	GpsNmeaData();
	virtual ~GpsNmeaData();

	//设置初始gps数据格式
	void SetInitGpsDataFormat(unsigned char ucInitGpsDataFormat);

	//加载GPS字符串
	bool LoadGPSString(char *pInputString, bool bIsCompareChecksum = true);
	//生成GPS字符串
	bool CreateGPSString(char *pOutputString, int & nOutputStringSize, bool bIsHasEndFlag);

	//加载GPRMC字符串
	bool LoadGPRMCString(char *pInputString,std::vector<std::string> vecValue,bool bIsCompareChecksum=true);
	//生成GPRMC字符串
	bool CreateGPRMCString(char *pOutputString, int & nOutputStringSize,bool bIsHasEndFlag);

	//加载GNRMC字符串
	bool LoadGNRMCString(char *pInputString,std::vector<std::string> vecValue, bool bIsCompareChecksum = true);
	//生成GNRMC字符串
	bool CreateGNRMCString(char *pOutputString, int & nOutputStringSize, bool bIsHasEndFlag);

	//加载GPGGA字符串
	bool LoadGPGGAString(char *pInputString, std::vector<std::string> vecValue, bool bIsCompareChecksum = true);
	//生成GPGGA字符串
	bool CreateGPGGAString(char *pOutputString, int & nOutputStringSize, bool bIsHasEndFlag);

	//计算经纬度（根据速度和方向计算下一经纬度）
	void CalcLngLat(float fDirect, float fSpeed, double dOldLng, double dOldLat, double & dNewLng, double & dNewLat);
	void CalcLngLatEx(float fDirect, float fSpeed, double dOldLng, double dOldLat, double & dNewLng, double & dNewLat, int nKeepSeconds=1);
	//计算经纬度（根据距离和方向计算下一经纬度）
	void CalcLngLatByDistanceAndDirect(float fDirect, double fDistance, double dOldLng, double dOldLat, double & dNewLng, double & dNewLat);

	//获取与设置参数
	void GetLng(double & fLng,unsigned char & ucDirect);
	void SetLng(double fLng, unsigned char ucDirect);

	void GetLat(double & fLat,unsigned char & ucDirect);
	void SetLat(double fLat, unsigned char ucDirect);

	float GetSpeed();
	void SetSpeed(float fSpeed);

	float GetDirect();
	void SetDirect(float fDirect);

	unsigned char GetStatus();
	void SetStatus(unsigned char ucStatus);

	unsigned char GetDeclination();
	void SetDeclination(unsigned char ucDeclination);

	unsigned char GetDeclinationDirect();
	void SetDeclinationDirect(unsigned char ucDeclinationDirect);

	unsigned char GetMode();
	void SetMode(unsigned char ucMode);

	unsigned char GetSatelliteNum();
	void SetSatelliteNum(unsigned char ucSatelliteNum);

	float GetHDOP();
	void  SetHDOP(float fHDOP);

	float GetHigh();
	void  SetHigh(float fHigh);

	float GetEarthHigh();
	void  SetEarthHigh(float fHigh);

	unsigned int GetDiffTime();
	void SetDiffTime(unsigned int uiDiffTime);

	unsigned short GetDiffSiteId();
	void SetDiffSiteId(unsigned short usDiffSiteId);

	unsigned char GetChecksum();
	void SetChecksum(unsigned char ucChecksum);

	void GetTime(unsigned short & usYear, unsigned short & usMonth, unsigned short & usDay, unsigned short & usHour, 
		unsigned short & usMiniute, unsigned short & usSecond, unsigned short & usMillsSecond);
	void SetTime(unsigned short usYear, unsigned short usMonth, unsigned short usDay, unsigned short usHour,
		unsigned short usMiniute, unsigned short usSecond, unsigned short usMillsSecond);
	void SetNowTime();

public:
	//度分转换度
	static double dm_to_degree(double dm);

	//度转换度分
	static double degree_to_dm(double ddd_dddd);

	//计算两点经纬度之间的距离
	static double GpsNmeaDataDistance(double latitude1, double longitude1, double latitude2, double longitude2);

protected:
	//计算校验和
	unsigned char CalcChecksum(unsigned char *pInput, int nInputSize);

protected:
	unsigned char m_ucGpsDataFormat;	//GPS数据格式
	double m_fLng;						//经度
	unsigned char m_ucGpsLngDirect;
	double m_fLat;						//纬度
	unsigned char m_ucGpsLatDirect;
	float m_fSpeed;						//速度,千米／小时 (一节也是1.852千米／小时)
	float m_fDirect;					//方向
	unsigned char  m_ucStatus;			//GPS状态 A=数据有效；V=数据无效
	unsigned char  m_ucDeclination;		 //磁偏角,（000 - 180）度（前导位数不足则补0）
	unsigned char  m_ucDeclinationDirect;//磁偏角方向，E=东，W=西
	unsigned char  m_ucMode;			//模式，A=自动，D=差分，E=估测，N=数据无效
	unsigned char  m_ucChecksum;		//校验和

	unsigned char  m_ucSatelliteNum;	//卫星数
	float m_fHDOP;						//HDOP水平精度因子（0.5~99.9） 
	float m_fHigh;						//海拔高度（-9999.9~99999.9）
	float m_fEarthHigh;					//地球椭球面相对大地水准面的高度 
	unsigned int m_uiDiffTime;			//差分时间（从最近一次接收到差分信号开始的秒数，如果不是差分定位将为空） 
	unsigned short m_usDiffSiteId;		//差分站ID号0000~1023（前面的0也将被传输，如果不是差分定位将为空）
	unsigned short m_usYear;
	unsigned short m_usMonth;
	unsigned short m_usDay;
	unsigned short m_usHour;
	unsigned short m_usMinute;
	unsigned short m_usSecond;
	unsigned short m_usMillsSecond;
};
```

```cpp
//GpsNmeaData.cpp
#include "stdafx.h"
#include "GpsNmeaData.h"

#define	 GpsNmeaData_StringSplitMinCount			8							//Gprmc字符串分解子字符串最小个数
#define	 GpsNmeaData_Gnrmc_Head		"$GNRMC"									//Gps nmea帧头，$GNRMC
#define  GpsNmeaData_Gprmc_Head		"$GPRMC"									//Gps nmea帧头，$GPRMC
#define  GpsNmeaData_Gpgga_Head		"$GPGGA"									//Gps nmea帧头，$GPGGA

const double PI = 3.14159265358979323846;
const double Ea = 6378137;     //   赤道半径  
const double Eb = 6356725;     //   极半径 

GpsNmeaData::GpsNmeaData()
{
	m_ucGpsDataFormat = GpsDataFormat_None;
	m_fLng = 0.0f;
	m_ucGpsLngDirect = GpsLngDirect_E;
	m_fLat = 0.0;
	m_ucGpsLatDirect = GpsLatDirect_N;
	m_fSpeed = 0;
	m_fDirect = 0;
	m_ucStatus = GpsStatus_Valid;
	m_ucDeclination = 0;
	m_ucDeclinationDirect = GpsDeclinationDirect_E;
	m_ucMode = GpsMode_A;
	m_ucChecksum = 0;
	m_usYear = 0;
	m_usMonth = 0;
	m_usDay = 0;
	m_usHour = 0;
	m_usMinute = 0;
	m_usSecond = 0;
	m_usMillsSecond = 0;
	m_ucSatelliteNum = 0;
	m_fHDOP = 0.5;			
	m_fHigh = 0;			
	m_fEarthHigh = 0;		
	m_uiDiffTime = 0;		
	m_usDiffSiteId = 0;
}

GpsNmeaData::~GpsNmeaData()
{
}

//设置初始gps数据格式
void GpsNmeaData::SetInitGpsDataFormat(unsigned char ucInitGpsDataFormat)
{
	m_ucGpsDataFormat = ucInitGpsDataFormat;
}

//获取与设置参数
void  GpsNmeaData::GetLng(double & fLng, unsigned char & ucDirect)
{
	fLng = m_fLng;
	ucDirect = m_ucGpsLngDirect;
}
void GpsNmeaData::SetLng(double fLng, unsigned char ucDirect)
{
	m_fLng = fLng;
	m_ucGpsLngDirect = ucDirect;
}

void GpsNmeaData::GetLat(double & fLat, unsigned char & ucDirect)
{
	fLat = m_fLat;
	ucDirect = m_ucGpsLatDirect;
}
void GpsNmeaData::SetLat(double fLat, unsigned char ucDirect)
{
	m_fLat = fLat;
	m_ucGpsLatDirect = ucDirect;
}

float GpsNmeaData::GetSpeed()
{
	return m_fSpeed;
}
void GpsNmeaData::SetSpeed(float fSpeed)
{
	m_fSpeed = fSpeed;
}

float GpsNmeaData::GetDirect()
{
	return m_fDirect;
}
void GpsNmeaData::SetDirect(float fDirect)
{
	m_fDirect = fDirect;
}

unsigned char GpsNmeaData::GetStatus()
{
	return m_ucStatus;
}
void GpsNmeaData::SetStatus(unsigned char ucStatus)
{
	m_ucStatus = ucStatus;
}

unsigned char GpsNmeaData::GetDeclination()
{
	return m_ucDeclination;
}
void GpsNmeaData::SetDeclination(unsigned char ucDeclination)
{
	m_ucDeclination = ucDeclination;
}

unsigned char GpsNmeaData::GetDeclinationDirect()
{
	return m_ucDeclinationDirect;
}
void GpsNmeaData::SetDeclinationDirect(unsigned char ucDeclinationDirect)
{
	m_ucDeclinationDirect = ucDeclinationDirect;
}

unsigned char GpsNmeaData::GetMode()
{
	return m_ucMode;
}
void GpsNmeaData::SetMode(unsigned char ucMode)
{
	m_ucMode = ucMode;
}

unsigned char GpsNmeaData::GetSatelliteNum()
{
	return m_ucSatelliteNum;
}
void GpsNmeaData::SetSatelliteNum(unsigned char ucSatelliteNum)
{
	m_ucSatelliteNum = ucSatelliteNum;
}

float GpsNmeaData::GetHDOP()
{
	return m_fHDOP;
}
void  GpsNmeaData::SetHDOP(float fHDOP)
{
	m_fHDOP = fHDOP;
}

float GpsNmeaData::GetHigh()
{
	return m_fHigh;
}
void  GpsNmeaData::SetHigh(float fHigh)
{
	m_fHigh = fHigh;
}

float GpsNmeaData::GetEarthHigh()
{
	return m_fEarthHigh;
}
void  GpsNmeaData::SetEarthHigh(float fHigh)
{
	m_fEarthHigh = fHigh;
}

unsigned int GpsNmeaData::GetDiffTime()
{
	return m_uiDiffTime;
}
void GpsNmeaData::SetDiffTime(unsigned int uiDiffTime)
{
	m_uiDiffTime = uiDiffTime;
}

unsigned short GpsNmeaData::GetDiffSiteId()
{
	return m_usDiffSiteId;
}
void GpsNmeaData::SetDiffSiteId(unsigned short usDiffSiteId)
{
	m_usDiffSiteId = usDiffSiteId;
}

unsigned char GpsNmeaData::GetChecksum()
{
	return m_ucChecksum;
}
void GpsNmeaData::SetChecksum(unsigned char ucChecksum)
{
	m_ucChecksum = ucChecksum;
}

void GpsNmeaData::GetTime(unsigned short & usYear, unsigned short & usMonth, unsigned short & usDay, unsigned short & usHour,
	unsigned short & usMiniute, unsigned short & usSecond, unsigned short & usMillsSecond)
{
	usYear = m_usYear;
	usMonth = m_usMonth;
	usDay = m_usDay;
	usHour = m_usHour;
	usMiniute = m_usMinute;
	usSecond = m_usSecond;
	usMillsSecond = m_usMillsSecond;
}
void GpsNmeaData::SetTime(unsigned short usYear, unsigned short usMonth, unsigned short usDay, unsigned short usHour,
	unsigned short usMiniute, unsigned short usSecond, unsigned short usMillsSecond)
{
	m_usYear = usYear;
	m_usMonth = usMonth;
	m_usDay = usDay;
	m_usHour = usHour;
	m_usMinute = usMiniute;
	m_usSecond = usSecond;
	m_usMillsSecond = usMillsSecond;
}
void GpsNmeaData::SetNowTime()
{
	//格林威治时间
	time_t timep;
	time(&timep);
	struct tm* zeroTM =  gmtime(&timep);
	SetTime(zeroTM->tm_year + 1900,zeroTM->tm_mon+1,zeroTM->tm_mday,zeroTM->tm_hour,zeroTM->tm_min,zeroTM->tm_sec,0);
}

//加载GPS字符串
bool GpsNmeaData::LoadGPSString(char *pInputString, bool bIsCompareChecksum)
{
	bool bRet = false;
	std::vector<std::string> vecValue;
	int nStringSplitRet = StringSplit(vecValue, pInputString, ",");
	if (nStringSplitRet >= GpsNmeaData_StringSplitMinCount)
	{
		if (strcmp(vecValue[0].c_str(), GpsNmeaData_Gprmc_Head) == 0)
		{
			//gprmc
			m_ucGpsDataFormat = GpsDataFormat_GPRMC;
			bRet = LoadGPRMCString(pInputString,vecValue, bIsCompareChecksum);
		}
		else if (strcmp(vecValue[0].c_str(), GpsNmeaData_Gnrmc_Head) == 0)
		{
			//gnrmc
			m_ucGpsDataFormat = GpsDataFormat_GNRMC;
			bRet = LoadGNRMCString(pInputString,vecValue, bIsCompareChecksum);
		}
		else if (strcmp(vecValue[0].c_str(), GpsNmeaData_Gpgga_Head) == 0)
		{
			//gpgga
			m_ucGpsDataFormat = GpsDataFormat_GPGGA;
			bRet = LoadGPGGAString(pInputString, vecValue, bIsCompareChecksum);
		}
		else
		{
			//非法数据格式
			return false;
		}
	}

	return bRet;
}

//生成GPS字符串
bool GpsNmeaData::CreateGPSString(char *pOutputString, int & nOutputStringSize, bool bIsHasEndFlag)
{
	bool bRet = false;
	if (m_ucGpsDataFormat == GpsDataFormat_GNRMC)
	{
		//GNRMC
		bRet = CreateGNRMCString(pOutputString, nOutputStringSize, bIsHasEndFlag);
	}
	else if (m_ucGpsDataFormat == GpsDataFormat_GPRMC)
	{
		//GPRMC
		bRet = CreateGPRMCString(pOutputString, nOutputStringSize, bIsHasEndFlag);
	}
	else if (m_ucGpsDataFormat == GpsDataFormat_GPGGA)
	{
		//GPGGA
		bRet = CreateGPGGAString(pOutputString, nOutputStringSize, bIsHasEndFlag);
	}
	else
	{
		bRet = CreateGNRMCString(pOutputString, nOutputStringSize, bIsHasEndFlag);
	}
	return bRet;
}

//加载GPRMC字符串
bool GpsNmeaData::LoadGPRMCString(char *pInputString,std::vector<std::string> vecValue, bool bIsCompareChecksum)
{
	//$GPRMC,212744.000,A,3721.3623,N,12706.8308,E,5.40,161.91,050317,,,A*65
	bool bRet = false;
	//utc时间
	sscanf(vecValue[1].c_str(), "%2d%2d%2d.%d", &m_usHour, &m_usMinute, &m_usSecond, &m_usMillsSecond);
	//状态
	if (vecValue[2].size() > 0)
	{
		m_ucStatus = vecValue[2].c_str()[0];
	}
	else
	{
		m_ucStatus = GpsStatus_Invalid;
	}
	//纬度
	double fLatMD = atof(vecValue[3].c_str());
	m_fLat = dm_to_degree(fLatMD);
	//北纬/南纬
	if (vecValue[4].size() > 0)
	{
		m_ucGpsLatDirect = vecValue[4].c_str()[0];
	}
	//经度
	double fLngMD = atof(vecValue[5].c_str());
	m_fLng = dm_to_degree(fLngMD);
	//东经/西经
	if (vecValue[6].size() > 0)
	{
		m_ucGpsLngDirect = vecValue[6].c_str()[0];
	}
	//速度，单位节，一节也是1.852千米／小时
	float fSpeedByNodeUnit = atof(vecValue[7].c_str());
	m_fSpeed = fSpeedByNodeUnit * 1.852;
	//方位角
	m_fDirect = atof(vecValue[8].c_str());
	//UTC日期
	char szDate[12] = { 0 };
	strcpy(szDate, vecValue[9].c_str());
	int usYear = 0;
	int usMonth = 0;
	int usDay = 0;
	sscanf(szDate, "%02d%02d%02d", &usDay, &usMonth, &usYear);
	m_usYear = usYear;
	m_usMonth = usMonth;
	m_usDay = usDay;
	m_usYear += 2000;
	//磁偏角
	if (vecValue[10].size() > 0)
	{
		m_ucDeclination = vecValue[10].c_str()[0];
	}
	//磁偏角方向
	if (vecValue[11].size() > 0)
	{
		m_ucDeclinationDirect = vecValue[11].c_str()[0];
	}
	else
	{
		m_ucDeclinationDirect = GpsDeclinationDirect_Invalid;
	}
	//模式指示、校验
	sscanf(vecValue[12].c_str(), "%c*%X", &m_ucMode, &m_ucChecksum);
	//计算校验和比对
	if (bIsCompareChecksum)
	{
		char *pFindStarChar = strrchr(pInputString, '*');
		if (pFindStarChar != NULL)
		{
			unsigned char ucTempChecksum = 0;
			ucTempChecksum = CalcChecksum((unsigned char *)pInputString, pFindStarChar - pInputString + 1);
			if (ucTempChecksum == m_ucChecksum)
			{
				bRet = true;
			}
		}
	}
	else
	{
		bRet = true;
	}

	return bRet;
}
//生成GPRMC字符串
bool GpsNmeaData::CreateGPRMCString(char *pOutputString, int & nOutputStringSize, bool bIsHasEndFlag)
{
	//$GPRMC,212744.000,A,3721.3623,N,12706.8308,E,5.40,161.91,050317,,,A*65
	bool bRet = true;
	//memset(pOutputString, 0, nOutputStringSize);
	//纬度
	double fLatMD = degree_to_dm(m_fLat);
	//经度
	double fLngMD = degree_to_dm(m_fLng);
	//速度(节)
	float fSpeedByNodeUnit = m_fSpeed/1.852;
	if (m_ucStatus == GpsStatus_Invalid)
	{
		//数据无效
		sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%c,,,,,,,%02d%02d%02d,,,N*",
			GpsNmeaData_Gprmc_Head,m_usHour,m_usMinute,m_usSecond,m_usMillsSecond,m_ucStatus,m_usDay,m_usMonth,m_usYear-2000);
		unsigned char ucChecksum = CalcChecksum((unsigned char *)pOutputString, strlen(pOutputString));
		char szChecksum[4] = { 0 };
		sprintf_s(szChecksum, "%02X", ucChecksum);
		strcat(pOutputString, szChecksum);
		if (bIsHasEndFlag)
		{
			//是否带结束标记符号，0x0d0x0a
			char szEndFlag[4] = { 0 };
			sprintf_s(szEndFlag, "%c%c", 0x0d, 0x0a);
			strcat(pOutputString, szEndFlag);
		}
		nOutputStringSize = strlen(pOutputString);
	}
	else
	{
		//数据有效
		if (m_ucDeclinationDirect == GpsDeclinationDirect_Invalid)
		{
			sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%c,%.5f,%c,%.5f,%c,%.2f,%.2f,%02d%02d%02d,,,%c*",
				GpsNmeaData_Gprmc_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond, m_ucStatus,
				fLatMD, m_ucGpsLatDirect, fLngMD, m_ucGpsLngDirect, fSpeedByNodeUnit, m_fDirect,
				m_usDay, m_usMonth, m_usYear - 2000, m_ucMode);
		}
		else
		{
			sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%c,%.5f,%c,%.5f,%c,%.2f,%.2f,%02d%02d%02d,%d,%c,%c*",
				GpsNmeaData_Gprmc_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond, m_ucStatus,
				fLatMD, m_ucGpsLatDirect, fLngMD, m_ucGpsLngDirect, fSpeedByNodeUnit, m_fDirect,
				m_usDay, m_usMonth, m_usYear - 2000, m_ucDeclination, m_ucDeclinationDirect, m_ucMode);
		}

		unsigned char ucChecksum = CalcChecksum((unsigned char *)pOutputString, strlen(pOutputString));
		char szChecksum[4] = { 0 };
		sprintf_s(szChecksum, "%02X", ucChecksum);
		strcat(pOutputString, szChecksum);
		if (bIsHasEndFlag)
		{
			//是否带结束标记符号，0x0d0x0a
			char szEndFlag[4] = { 0 };
			sprintf_s(szEndFlag, "%c%c", 0x0d, 0x0a);
			strcat(pOutputString, szEndFlag);
		}
		nOutputStringSize = strlen(pOutputString);
	}
	return bRet;
}

//加载GNRMC字符串
bool GpsNmeaData::LoadGNRMCString(char *pInputString,std::vector<std::string> vecValue,bool bIsCompareChecksum)
{
	//$GNRMC,230134.00,A,2237.12422,N,11408.08310,E,9.291,215.59,310518,,,A*75
	bool bRet = false;
	//utc时间
	sscanf(vecValue[1].c_str(), "%2d%2d%2d.%d", &m_usHour, &m_usMinute, &m_usSecond, &m_usMillsSecond);
	//状态
	if (vecValue[2].size() > 0)
	{
		m_ucStatus = vecValue[2].c_str()[0];
	}
	else
	{
		m_ucStatus = GpsStatus_Invalid;
	}
	//纬度
	double fLatMD = atof(vecValue[3].c_str());
	m_fLat = dm_to_degree(fLatMD);
	//北纬/南纬
	if (vecValue[4].size() > 0)
	{
		m_ucGpsLatDirect = vecValue[4].c_str()[0];
	}
	//经度
	double fLngMD = atof(vecValue[5].c_str());
	m_fLng = dm_to_degree(fLngMD);
	//东经/西经
	if (vecValue[6].size() > 0)
	{
		m_ucGpsLngDirect = vecValue[6].c_str()[0];
	}
	//速度，单位节，一节也是1.852千米／小时
	float fSpeedByNodeUnit = atof(vecValue[7].c_str());
	m_fSpeed = fSpeedByNodeUnit * 1.852;
	//方位角
	m_fDirect = atof(vecValue[8].c_str());
	//UTC日期
	char szDate[12] = { 0 };
	strcpy(szDate, vecValue[9].c_str());
	int usYear = 0;
	int usMonth = 0;
	int usDay = 0;
	sscanf(szDate, "%02d%02d%02d", &usDay, &usMonth, &usYear);
	m_usYear = usYear;
	m_usMonth = usMonth;
	m_usDay = usDay;
	m_usYear += 2000;
	//磁偏角
	if (vecValue[10].size() > 0)
	{
		m_ucDeclination = vecValue[10].c_str()[0];
	}
	//磁偏角方向
	if (vecValue[11].size() > 0)
	{
		m_ucDeclinationDirect = vecValue[11].c_str()[0];
	}
	else
	{
		m_ucDeclinationDirect = GpsDeclinationDirect_Invalid;
	}
	//模式指示、校验
	std::vector<std::string> vecChecksum;
	StringSplit(vecChecksum, vecValue[12].c_str(), "*");
	if (vecChecksum.size() >= 2)
	{
		if (vecChecksum[0].c_str() > 0)
		{
			sscanf(vecChecksum[0].c_str(), "%c", &m_ucMode);
		}
		char *stop = NULL;
		m_ucChecksum = strtol(vecChecksum[1].c_str(), &stop, 16);
	}
	//计算校验和比对
	if (bIsCompareChecksum)
	{
		char *pFindStarChar = strrchr(pInputString, '*');
		if (pFindStarChar != NULL)
		{
			unsigned char ucTempChecksum = 0;
			ucTempChecksum = CalcChecksum((unsigned char *)pInputString, pFindStarChar - pInputString + 1);
			if (ucTempChecksum == m_ucChecksum)
			{
				bRet = true;
			}
		}
	}
	else
	{
		bRet = true;
	}

	return bRet;	
}
//生成GNRMC字符串
bool GpsNmeaData::CreateGNRMCString(char *pOutputString, int & nOutputStringSize, bool bIsHasEndFlag)
{
//$GNRMC,230134.00,A,2237.12422,N,11408.08310,E,9.291,215.59,310518,,,A*75
	bool bRet = true;
	//memset(pOutputString, 0, nOutputStringSize);
	//纬度
	double fLatMD = degree_to_dm(m_fLat);
	//经度
	double fLngMD = degree_to_dm(m_fLng);
	//速度(节)
	float fSpeedByNodeUnit = m_fSpeed / 1.852;
	if (m_ucStatus == GpsStatus_Invalid)
	{
		//数据无效
		sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%c,,,,,,,%02d%02d%02d,,,N*",
			GpsNmeaData_Gnrmc_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond, m_ucStatus, m_usDay, m_usMonth, m_usYear - 2000);
		unsigned char ucChecksum = CalcChecksum((unsigned char *)pOutputString, strlen(pOutputString));
		char szChecksum[4] = { 0 };
		sprintf_s(szChecksum, "%02X", ucChecksum);
		strcat(pOutputString, szChecksum);
		if (bIsHasEndFlag)
		{
			//是否带结束标记符号，0x0d0x0a
			char szEndFlag[4] = { 0 };
			sprintf_s(szEndFlag, "%c%c", 0x0d, 0x0a);
			strcat(pOutputString, szEndFlag);
		}
		nOutputStringSize = strlen(pOutputString);
	}
	else
	{
		//数据有效
		if (m_ucDeclinationDirect == GpsDeclinationDirect_Invalid)
		{
			sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%c,%.5f,%c,%.5f,%c,%.3f,%.2f,%02d%02d%02d,,,%c*",
				GpsNmeaData_Gnrmc_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond, m_ucStatus,
				fLatMD, m_ucGpsLatDirect, fLngMD, m_ucGpsLngDirect, fSpeedByNodeUnit, m_fDirect,
				m_usDay, m_usMonth, m_usYear - 2000, m_ucMode);
		}
		else
		{
			sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%c,%.5f,%c,%.5f,%c,%.3f,%.2f,%02d%02d%02d,%d,%c,%c*",
				GpsNmeaData_Gnrmc_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond, m_ucStatus,
				fLatMD, m_ucGpsLatDirect, fLngMD, m_ucGpsLngDirect, fSpeedByNodeUnit, m_fDirect,
				m_usDay, m_usMonth, m_usYear - 2000, m_ucDeclination, m_ucDeclinationDirect, m_ucMode);
		}

		unsigned char ucChecksum = CalcChecksum((unsigned char *)pOutputString, strlen(pOutputString));
		char szChecksum[4] = { 0 };
		sprintf_s(szChecksum, "%02X", ucChecksum);
		strcat(pOutputString, szChecksum);
		if (bIsHasEndFlag)
		{
			//是否带结束标记符号，0x0d0x0a
			char szEndFlag[4] = { 0 };
			sprintf_s(szEndFlag, "%c%c", 0x0d, 0x0a);
			strcat(pOutputString, szEndFlag);
		}
		nOutputStringSize = strlen(pOutputString);
	}
	return bRet;
}

//加载GPGGA字符串
bool GpsNmeaData::LoadGPGGAString(char *pInputString, std::vector<std::string> vecValue,bool bIsCompareChecksum)
{
	//$GPGGA,025620.00,2602.33721,N,11911.49176,E,2,04,1.63,13.5,M,9.9,M,,0000*5D
	bool bRet = false;
	//utc时间
	sscanf(vecValue[1].c_str(), "%2d%2d%2d.%d", &m_usHour, &m_usMinute, &m_usSecond, &m_usMillsSecond);
	//纬度
	double fLatMD = atof(vecValue[2].c_str());
	m_fLat = dm_to_degree(fLatMD);
	//北纬/南纬
	if (vecValue[3].size() > 0)
	{
		m_ucGpsLatDirect = vecValue[3].c_str()[0];
	}
	//经度
	double fLngMD = atof(vecValue[4].c_str());
	m_fLng = dm_to_degree(fLngMD);
	//东经/西经
	if (vecValue[5].size() > 0)
	{
		m_ucGpsLngDirect = vecValue[5].c_str()[0];
	}
	//状态
	if (vecValue[6].size() > 0)
	{
		m_ucStatus = vecValue[6].c_str()[0];
	}
	else
	{
		m_ucStatus = '0';
	}
	//卫星数
	m_ucSatelliteNum = atoi(vecValue[7].c_str());
	//HDOP水平精度因子（0.5~99.9）
	m_fHDOP = atof(vecValue[8].c_str());
	//海拔高度（ - 9999.9~99999.9）
	m_fHigh = atof(vecValue[9].c_str());
	//M
	//地球椭球面相对大地水准面的高度
	m_fEarthHigh = atof(vecValue[11].c_str());
	//M
	//差分时间
	m_uiDiffTime = atol(vecValue[13].c_str());
	//差分站ID号、校验
	std::vector<std::string> vecDiffIno;
	StringSplit(vecDiffIno, vecValue[14].c_str(), "*");
	if (vecDiffIno.size() >= 2)
	{
		m_usDiffSiteId = atoi(vecDiffIno[0].c_str());
		char *stop = NULL;
		m_ucChecksum  = strtol(vecDiffIno[1].c_str(), &stop, 16);
	}
	//计算校验和比对
	if (bIsCompareChecksum)
	{
		char *pFindStarChar = strrchr(pInputString, '*');
		if (pFindStarChar != NULL)
		{
			unsigned char ucTempChecksum = 0;
			ucTempChecksum = CalcChecksum((unsigned char *)pInputString, pFindStarChar - pInputString + 1);
			if (ucTempChecksum == m_ucChecksum)
			{
				bRet = true;
			}
		}
	}
	else
	{
		bRet = true;
	}

	return bRet;
}
//生成GPGGA字符串
bool GpsNmeaData::CreateGPGGAString(char *pOutputString, int & nOutputStringSize, bool bIsHasEndFlag)
{
	//$GPGGA,025620.00,2602.33721,N,11911.49176,E,2,04,1.63,13.5,M,9.9,M,,0000*5D 
	bool bRet = true;
	//memset(pOutputString, 0, nOutputStringSize);
	//纬度
	double fLatMD = degree_to_dm(m_fLat);
	//经度
	double fLngMD = degree_to_dm(m_fLng);
	//m_ucStatus 0=未定位，1=非差分定位，2=差分定位，6=正在估算
	if (m_ucStatus == '0')
	{
		//数据无效
		sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,,,,,%c,,,,M,,M,,0000*",
			GpsNmeaData_Gpgga_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond, m_ucStatus);
		unsigned char ucChecksum = CalcChecksum((unsigned char *)pOutputString, strlen(pOutputString));
		char szChecksum[4] = { 0 };
		sprintf_s(szChecksum, "%02X", ucChecksum);
		strcat(pOutputString, szChecksum);
		if (bIsHasEndFlag)
		{
			//是否带结束标记符号，0x0d0x0a
			char szEndFlag[4] = { 0 };
			sprintf_s(szEndFlag, "%c%c", 0x0d, 0x0a);
			strcat(pOutputString, szEndFlag);
		}
		nOutputStringSize = strlen(pOutputString);
	}
	else
	{
		//数据有效
		if (m_ucStatus == '2')
		{
			//差分定位
			sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%.4f,%c,%.4f,%c,%c,%02d,%.1f,%.1f,M,%.1f,M,%u,%04d*",
				GpsNmeaData_Gpgga_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond,
				fLatMD, m_ucGpsLatDirect, fLngMD, m_ucGpsLngDirect, m_ucStatus,
				m_ucSatelliteNum,m_fHDOP,m_fHigh,m_fEarthHigh,m_uiDiffTime,m_usDiffSiteId);
		}
		else if (m_ucStatus == '1')
		{
			//非差分定位
			sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%.4f,%c,%.4f,%c,%c,%02d,%.1f,%.1f,M,%.1f,M,,0000d*",
				GpsNmeaData_Gpgga_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond,
				fLatMD, m_ucGpsLatDirect, fLngMD, m_ucGpsLngDirect, m_ucStatus,
				m_ucSatelliteNum, m_fHDOP, m_fHigh, m_fEarthHigh);
		}
		else
		{
			sprintf_s(pOutputString, nOutputStringSize - 1, "%s,%02d%02d%02d.%03d,%.4f,%c,%.4f,%c,%c,%02d,%.1f,%.1f,M,%.1f,M,,0000*",
				GpsNmeaData_Gpgga_Head, m_usHour, m_usMinute, m_usSecond, m_usMillsSecond,
				fLatMD, m_ucGpsLatDirect, fLngMD, m_ucGpsLngDirect, m_ucStatus,
				m_ucSatelliteNum, m_fHDOP, m_fHigh, m_fEarthHigh);
		}
		unsigned char ucChecksum = CalcChecksum((unsigned char *)pOutputString, strlen(pOutputString));
		char szChecksum[4] = { 0 };
		sprintf_s(szChecksum, "%02X", ucChecksum);
		strcat(pOutputString, szChecksum);
		if (bIsHasEndFlag)
		{
			//是否带结束标记符号，0x0d0x0a
			char szEndFlag[4] = { 0 };
			sprintf_s(szEndFlag, "%c%c", 0x0d, 0x0a);
			strcat(pOutputString, szEndFlag);
		}
		nOutputStringSize = strlen(pOutputString);
	}
	return bRet;
}

/*
功能：计算经纬度（根据速度和方向计算下一经纬度）
参数：fDirect：方向；fSpeed:速度；km/h；dOldLng：旧经度；dOldLat：旧纬度；dNewLng：新经度；dNewLat：新纬度
返回：无
*/
void GpsNmeaData::CalcLngLat(float fDirect, float fSpeed, double dOldLng, double dOldLat, double & dNewLng, double & dNewLat)
{
	CalcLngLatEx(fDirect, fSpeed, dOldLng, dOldLat, dNewLng, dNewLat, 1);
}
/*
功能：计算经纬度（根据速度和方向计算下一经纬度）
参数：fDirect：方向；fSpeed:速度；km/h；dOldLng：旧经度；dOldLat：旧纬度；dNewLng：新经度；dNewLat：新纬度；nKeepSeconds：保持多少秒
返回：无
*/
void GpsNmeaData::CalcLngLatEx(float fDirect, float fSpeed, double dOldLng, double dOldLat, double & dNewLng, double & dNewLat, int nKeepSeconds)
{
	if (nKeepSeconds < 1)
	{
		nKeepSeconds = 1;
	}
	float declie = fDirect;// 方位角
	float speed = fSpeed;// 速度
	float distance = speed * nKeepSeconds / 3600.0;//速度计算单位时间内车辆行驶距离,这里单位为千米。换算成每秒多少km，外部接口一秒发送一次gps数据
	float degree = distance / 111;//km
	float tempLat = degree * cos(declie);
	float tempLong = degree * sin(declie);
	dNewLat = dOldLat + tempLat;
	dNewLng = dOldLng + tempLong;
}

/*
功能：计算经纬度（根据距离和方向计算下一经纬度）
参数：fDirect：方向；fDistance:距离；km；dOldLng：旧经度；dOldLat：旧纬度；dNewLng：新经度；dNewLat：新纬度
返回：无
*/
void GpsNmeaData::CalcLngLatByDistanceAndDirect(float fDirect, double fDistance, double dOldLng, double dOldLat, double & dNewLng, double & dNewLat)
{
	double dx = fDistance * 1000 * sin(fDirect*PI / 180.0);
	double dy = fDistance * 1000 * cos(fDirect*PI / 180.0);
	double ec = Eb + (Ea - Eb) * (90 - dOldLat) / 90.0;
	double ed = ec * cos(dOldLat*PI / 180);
	dNewLng = (dx / ed + dOldLng*PI / 180.0) * 180.0 / PI;
	dNewLat = (dy / ec + dOldLat*PI / 180.0) * 180.0 / PI;
}

//计算校验和
unsigned char GpsNmeaData::CalcChecksum(unsigned char *pInput, int nInputSize)
{
	//$与*符号之间的字符进行异或
	unsigned char ucChecksum = 0;
	int i = 0;
	for (i = 1; i < nInputSize - 1; i++)//排除$与*符号
	{
		ucChecksum ^= pInput[i];
	}
	return ucChecksum;
}

//度分转换度
double GpsNmeaData::dm_to_degree(double dm)
{
	//计算度
	int d = (int)(dm / 100);
	//计算分
	double m = dm - d * 100;
	double s = d * 3600 + m * 60;
	double dd_dddd = s / 3600.0;

	return	dd_dddd;
}

//度转换度分
double GpsNmeaData::degree_to_dm(double ddd_dddd)
{
	int d = (int)(ddd_dddd);
	double m = (ddd_dddd - d) * 60;
	double dddmm_mmmm = d * 100 + m;
	return	dddmm_mmmm;
}

/*
功能：计算两点经纬度之间的距离
参数：latitude1：点1纬度值；longitude1：点1经度值；latitude2：点2纬度值；longitude2：点2经度值
返回：double，距离，单位为m
*/
double GpsNmeaData::GpsNmeaDataDistance(double latitude1, double longitude1, double latitude2, double longitude2)
{
	double lon1 = longitude1 * PI / 180;
	double lat1 = latitude1 * PI / 180;
	double lon2 = longitude2 * PI / 180;
	double lat2 = latitude2 * PI / 180;
	double deltaLat = lat1 - lat2;
	double deltaLon = lon1 - lon2;
	double s = 2 * asin(sqrt(pow(sin(deltaLat / 2), 2) + cos(lat1)*cos(lat2)*pow(sin(deltaLon / 2), 2)));
	return (s * 1000 * 6378.137);
}
```

```
//调用实例
GpsNmeaData gndGpsNmeaData;
bool bLoadGpsNmeaDataString = gndGpsNmeaData.LoadGPSString("$GNRMC,230142.00,A,2237.10770,N,11408.06877,E,11.002,218.24,310518,,,A,V*3A");
if(bLoadGpsNmeaDataString)
{
			char szOutputGPRMC[128] = { 0 };
			int nOutputGPRMCSize = 128;
			bool bCreateGPRMCString = false;
			float fDirect = 0;
			double fLngPrev = 0, fLatPrev = 0;
			unsigned char ucLngPrevDirect = 0, ucLatPrevDirect = 0;
			double fNewLng = 0, fNewLat = 0;
			//获取经纬度等信息
			gndGpsNmeaData.GetLng(fLngPrev, ucLngPrevDirect);
			gndGpsNmeaData.GetLat(fLatPrev, ucLatPrevDirect);
			//计算
			gndGpsNmeaData.CalcLngLatByDistanceAndDirect(fDirect, 30, fLngPrev, fLatPrev, fNewLng, fNewLat);
			//保存经纬度等信息
			gndGpsNmeaData.SetLng(fNewLng, ucLngPrevDirect);
			gndGpsNmeaData.SetLat(fNewLat, ucLatPrevDirect);
			gndGpsNmeaData.SetNowTime();
			nOutputGPRMCSize = 128;
			memset(szOutputGPRMC, 0, sizeof(szOutputGPRMC));
			bCreateGPRMCString = m_ggdGpsGprmcData.CreateGPSString(szOutputGPRMC, nOutputGPRMCSize, true);
}
```

