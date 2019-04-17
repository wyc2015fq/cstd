# IP地址与无符号整数值相互转换 - DoubleLi - 博客园






方法1：
IP地址转换为整数：比如"192.168.1.254"，以点为分割，将192 168 1 254保存在一个int a[4]数组里，
然后通过unsigned int nResult = (a[3] << 24) + (a[2] << 16) + (a[1] << 8) + a[0]得到一个无符号32位整数。

整数转换为IP地址：把这个整数转换成一个无符号32位二进制数。从左到右，每八位进行一下分割，
得到4段8位的二进制数，把这些二进制数转换成整数

#include <iostream>
#include <string>

using namespace std;

unsigned int  IPToValue(const string& strIP)//IP转换为整数
{
	//IP转化为数值
	//没有格式检查
	//返回值就是结果

	int a[4];
	string IP = strIP;
	string strTemp;
	size_t pos;
	size_t i = 3;

	do
	{
		pos = IP.find(".");

		if (pos != string::npos)
		{
			strTemp = IP.substr(0, pos);
			a[i] = atoi(strTemp.c_str());//int atoi(const char *str);
			i--;
			IP.erase(0, pos + 1);//从位置0开始删除pos+1个字符
		}
		else
		{
			strTemp = IP;
			a[i] = atoi(strTemp.c_str());
			break;
		}

	} while (1);

	unsigned int nResult = (a[3] << 24) + (a[2] << 16) + (a[1] << 8) + a[0];
	return nResult;
}

string ValueToIP(const int& nValue)//整数转换为ip
{
	//数值转化为IP
	//没有格式检查
	//返回值就是结果

	char strTemp[20];
	sprintf(strTemp, "%d.%d.%d.%d",
		(nValue & 0xff000000) >> 24,
		(nValue & 0x00ff0000) >> 16,
		(nValue & 0x0000ff00) >> 8,
		(nValue & 0x000000ff));

	return string(strTemp);
}

int main()
{
	string strIP = "192.168.1.254";
	cout  << IPToValue(strIP) << endl;

	cout << ValueToIP(3232236030) << endl;

	return 0;
}

方法2：使用win32 api
#include <iostream>
#include <winsock2.h>

#pragma comment(lib,"ws2_32.lib")
using namespace std;

int main(void)
{
	cout << (unsigned int)htonl(inet_addr("192.168.1.254")) << endl;

	struct in_addr addr;
	addr.S_un.S_addr = ntohl(3232236030);
	cout << inet_ntoa(addr) << endl;
	return 0;









