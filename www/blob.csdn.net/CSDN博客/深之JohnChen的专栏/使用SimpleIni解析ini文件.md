# 使用SimpleIni解析ini文件 - 深之JohnChen的专栏 - CSDN博客

2019年03月04日 23:04:05[byxdaz](https://me.csdn.net/byxdaz)阅读数：652


SimpleIni 是一个跨平台的 C++ 库，支持Windows, WinCE and Linux系统，提供一个简单的API用于操作 ini 配置文件。

simpleini项目地址：
[https://github.com/brofield/simpleini](https://github.com/brofield/simpleini)

一、Simpleini Api讲解

void Reset();    //释放内存中存储的对象

void SetUnicode(bool a_bIsUtf8 = true);//设置INI数据的存储格式，参数为true时保存为UTF-8格式，否则为本地编码格式

bool IsUnicode() const;//获取INI数据的存储格式(是否为Unicode (UTF-8))

void SetMultiKey(bool a_bAllowMultiKey = true);//是否允许一个关键字对应多个值，默认为允许；若不允许，则将最后一个值作为此关键字关联的值

bool IsMultiKey() const;//获取INI数据的存储格式(是否为一键多值)

void SetMultiLine(bool a_bAllowMultiLine = true);//设置数值能否跨越多行

bool IsMultiLine() const;//查询多行数据的状态

SI_Error LoadFile( const char * a_pszFile);//从磁盘加载INI文件到内存中，参数为INI文件的有效路径，返回错误定义

SI_Error LoadFile(const SI_WCHAR_T * a_pwszFile);//从磁盘加载INI文件(UTF-16)到内存中，参数为INI文件的有效路径，返回错误定义

SI_Error LoadFile(FILE * a_fpFile);//从一个文件指针加载INI文件，返回错误定义

SI_Error Load(std::istream & a_istream);//从输入流中加载INI文件，返回错误定义

SI_Error Load(const std::string & a_strData);//从std::string中加载INI文件，返回错误定义

SI_Error Load(const char *      a_pData,size_t        a_uDataLen);//从内存中加载INI文件，a_pData为要加载的数据,a_uDataLen为要加载的数据的长度，返回错误定义

SI_Error SaveFile(const char *          a_pszFile,bool a_bAddSignature = true) const;//从内存INI文件保存到磁盘，a_pszFile为保存的有效路径，不是UTF-8时a_bAddSignature将忽略

SI_Error SaveFile(const SI_WCHAR_T *  a_pwszFile,bool     a_bAddSignature = true) const;//从内存INI文件(UTF-16)保存到磁盘，a_pszFile为保存的有效路径，不是UTF-8时a_bAddSignature将忽略

SI_Error SaveFile(FILE *  a_pFile,bool    a_bAddSignature = false) const;//将INI数据保存至一个文件中，a_pFile为二进制打开的文件描述符

SI_Error Save(OutputWriter &  a_oOutput,bool   a_bAddSignature = false) const;//保存INI数据。这些数据将被写入到输出设备

SI_Error Save(std::ostream &  a_ostream,bool   a_bAddSignature = false) const;//将INI数据保存到一个ostream

SI_Error Save(std::string &   a_sBuffer,bool        a_bAddSignature = false) const;//追加到一个字符串的INI数据

void GetAllSections(TNamesDepend & a_names) const;//检索所有的section名。这份名单是返回一个STL的list，a_names(list)接收所有的节名称

bool GetAllKeys(const SI_CHAR * a_pSection,TNamesDepend &  a_names) const;//获取一个section下的所有关键字，结果保存在a_names中。返回true表示section找到，false表示未发现

bool GetAllValues(const SI_CHAR * a_pSection,const SI_CHAR * a_pKey,TNamesDepend & a_values) const;//获取一个指定section和关键字key下的所有值，保存在a_values。返回true表示找到，false表示未发现section/key

int GetSectionSize(const SI_CHAR * a_pSection) const;//获取指定section的key的个数，-1表示section不存在于INI文件中，其他表示个数

const TKeyVal * GetSection(const SI_CHAR * a_pSection) const;//返回一个section的键值对，用map保存

const SI_CHAR * GetValue(const SI_CHAR * a_pSection,const SI_CHAR * a_pKey,const SI_CHAR * a_pDefault     = NULL,bool *         a_pHasMultiple = NULL ) const;//获取一个特定的key值。如果启用了多个键(见SetMultiKey)，那么只有第一个与该键关联的值返回；  参数a_pDefault是如果没有找到将要返回的值，参数a_pHasMultiple可选收到通知，如果有这个key有多个条目

long GetLongValue(const SI_CHAR * a_pSection,const SI_CHAR * a_pKey,long            a_nDefault          = 0, bool *         a_pHasMultiple = NULL) const;//获取一个特定的key值。如果启用了多个键(见SetMultiKey)，那么只有第一个与该键关联的值返回；  参数a_nDefault是如果没有找到将要返回的值，参数a_pHasMultiple可选收到通知，如果有这个key有多个条目

bool GetBoolValue(const SI_CHAR * a_pSection,const SI_CHAR * a_pKey,bool            a_bDefault = false, bool *         a_pHasMultiple = NULL) const;//检索某个特定键的布尔值。如果启用了多个键(见SetMultiKey)，那么只有第一个与该键关联的值返回。参数a_bDefault是如果没有找到将要返回的值，参数a_pHasMultiple可选收到通知，如果有这个key有多个条目

SI_Error SetValue(const SI_CHAR * a_pSection,const SI_CHAR * a_pKey,const SI_CHAR * a_pValue,const SI_CHAR * a_pComment = NULL);//添加或更新一个section或者key 

        @参数a_pSection组中添加或更新

        @参数a_pKey键添加或更新。设置为NULL创建一个空的section

        @参数a_pValue值来设置。设置为NULL，创建一个空的section

        @参数a_pComment为注释

返回SI_Error错误定义

返回SI_UPDATED值更新

返回SI_INSERTED值是插入

SI_Error SetLongValue(const SI_CHAR * a_pSection,const SI_CHAR * a_pKey,long            a_nValue,const SI_CHAR * a_pComment = NULL,bool       a_bUseHex = false);//添加或更新一个section或者key ，最后一个参数a_bUseHex为true表示写入的值为十六进制，为false为十进制

SI_Error SetBoolValue(const SI_CHAR * a_pSection,const SI_CHAR * a_pKey,bool            a_nValue,const SI_CHAR * a_pComment = NULL);//添加或更新一个bool值

bool Delete(const SI_CHAR * a_pSection,const SI_CHAR * a_pKey,bool            a_bRemoveEmpty = false);//删除了整个section，或者一个section中的key。 注意：启用多个键时，这将删除该名称的所有键

        @参数a_pSection        删除section中的键，如果 a_pKey是NULL，section整个删除

        @参数a_pKey     从指定的section中删除一个key。设置为NULL删除整个section

        @参数a_bRemoveEmpty      删除一个section中的key之后，如果section是空的了，此section是否可以删除     

        @返回true  key或section被删除

        @返回false key或section没有找到

二、实例

创建test.ini文件，内容如下：

[server]

root = 10.1.1.1

gc = 10.1.1.2

game = 10.1.1.3

```
#include <iostream>
#include <SimpleIni.h>

using namespace std;

int main()
{
    CSimpleIniA ini;
    ini.SetUnicode();
    ini.LoadFile("test.ini");
    const char * pVal = ini.GetValue("server", "game", "default");
    ini.SetValue("server", "game", "10.1.1.5");
    const char * xVal = ini.GetValue("server", "game", "default");
    cout << pVal << endl;
    cout << xVal << endl;

    // save the data back to the file
    int rc = ini.SaveFile("test.ini");
    if (rc < 0) return false;
}
```


