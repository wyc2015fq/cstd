# VC 注册表编程(二) - L_Andy的专栏 - CSDN博客

2012年11月28日 17:44:28[卡哥](https://me.csdn.net/L_Andy)阅读数：1148


转自 [http://www.cnblogs.com/lartely/archive/2011/04/10/2011770.html](http://www.cnblogs.com/lartely/archive/2011/04/10/2011770.html)

转自 [http://wenku.baidu.com/view/f369e14acf84b9d528ea7a77.html](http://wenku.baidu.com/view/f369e14acf84b9d528ea7a77.html)

注册表的组织方式跟文件目录比较相似，主要分为根键、子键和键值项三部分，与文件目录对应的话就是根目录、子目录和文件。分别介绍一下这三部分： 

（1）根键。分为5个，分别为HKEY_CLASSES_ROOT， HKEY_CURRENT_USER，HKEY_LOCAL_MACHINE，HKEY_USERS和HKEY_CURRENT_CONFIG，把它们理解成磁盘的五个分区可以了。

（2）子键。可以有多个子键和键值项，就像一个目录中可以有多个子目录和多个文件一样。 

（3）键值项。可以理解为文件，它由三部分组成，分别为 ：名称、类型、数据。 

类型又分为多种主要包括如下： 

REG_BINARY 二进制数据 

REG_DWORD 32位双字节数据 

REG_SZ 以0结尾的字符串 

REG_DWORD_BIG_ENDIAN 高位排在底位的双字 

REG_EXPAND_SZ 扩展字符串，可以加入变量如%PATH% 

REG_LINK UNICODE 符号链接 

REG_RESOURCE_LIST 设备驱动程序资源列表 

REG_MULTI_SZ 多字符串 

注册表数据项的数据类型有8种，但最常用的主要是前3种。 

有了这些基础下面我们讨论如何编程实现对注册表的操作。 

### 2.打开/关闭注册表句柄

在对注册表操作前应该先打开指定的键，然后通过键的句柄进行操作，打开键句柄可以用API RegOpenKeyEx来实现，其原形如下： 

RegOpenKeyEx( 

hKey， //父键句柄 

lpSubKey， //子键句柄 

dwOptions， //系统保留，指定为0 

samDesired， //打开权限 

phkResult， //返回打开句柄 

)； 

其中打开权限有多种， 想方便的话可以指定为KEY_ALL_ACCESS ，这样什么权限都有了，当函数执行成功时返回ERROR_SUCCESS。 

其实例代码如下： 

HKEY key; 

LPCTSTR data="SOFTWARE\Microsoft\Windows\CurrentVersion\Run"; 

if(RegOpenKeyEx(HKEY_LOCAL_MACHINE，data，0，KEY_ALL_ACCESS，&key)==ERROR_SUCCESS)

...{ 

/**//*需要执行的操作...*/ 

} 

：：RegCloseKey(key); 

要注意的是，在使用后应该调用RegCloseKey()函数关闭句柄. 

### 3.获取子键/键值信息

在现实的编程操作中我们常常需要获取子键/键值的信息比如：子键/键值的数量，长度，以及数据的最大长度等等，这些信息可以通过RegQueryInfoKey函数来获取。

它的原型如下： 

RegQueryInfoKey( 

hkey， //要获取信息的句柄 

lpClass， //接受创建健时的Class字符串 

lpcbClass， //lpClass的长度 

lpReserved， //系统保留，指定为0 

lpcSubKeys， //子键数量 

lpcbMaxSubKeyLen， //子键中最长名称的长度 

lpcbMaxClassLen， //子键中最长Class字符串长度 

lpcVlaues， //键值数量 

lpcbMaxValueNameLen， //键值项中最长名称的长度 

lpcbMaxValueLen， //键值项数据最大长度 

lpcbSecurityDescriptor， //安全描述符长度 

lpftLastWriteTime， //FILETIME结构，最后修改时间 

)； 

这个函数的参数很多，实际使用时，只填写自己需要的就行了，不需要的可以放个NULL就OK了，还有一点需要注意就是它所返回的长度都不包括结尾的0字符，所以在使用时应该用长度+1。

其实例代码如下： 

DWORD dwIndex=0，NameSize，NameCnt，NameMaxLen，Type; 

DWORD KeySize，KeyCnt，KeyMaxLen，DateSize，MaxDateLen; 

if(RegQueryInfoKey(key，NULL，NULL，NULL，&KeyCnt，&KeyMaxLen，NULL，&NameCnt，&NameMaxLen，&MaxDateLen，NULL，NULL)!=ERROR_SUCCESS)

...{ 

printf("RegQueryInfoKey错误"); 

return; 

} 

用的时候套用格式就成了。有了这些信息我们就可以枚举子键和键值的信息了。 

### 4.枚举子键信息

枚举子键可以用API函数 RegEnumKeyEx来实现，调用RegEnumKeyEx时将返回子键的名称、长度和一些相关数据。如果想得到一个键下的全部子键的话应该循环调用，直到返回ERROR_NO_MORE_ITEMS为至，就说明已经枚举完了所有数据。

其函数原型如下： 

RegEnumKeyEx( 

hkey， //被枚举的键句柄 

dwIndex， //子键索引编号 

lpName， //子键名称 

lpcbName， //子键名称长度 

lpReserved， //系统保留，指定为0 

lpClass， //子键类名 

lpcbClass， //子键类名长度 

lpftLastWriteTime//最后写入时间 

)； 

因为在之前我们已经通过RegQueryInfoKey函数获取了键的有关数据，所以在这里不再跟据ERROR_NO_MORE_ITEMS来实现了。 

其实现代码如下： 

for(dwIndex=0;dwIndex<KeyCnt;dwIndex++) //枚举子键 

...{ 

KeySize=KeyMaxLen+1; //因为RegQueryInfoKey得到的长度不包括0结束字符，所以应加1 

szKeyName=(char*)malloc(KeySize); 

/**//*参数定义请参照获取子键/键值信息部分...*/ 

RegEnumKeyEx(hKey，dwIndex，szKeyName，&KeySize，NULL，NULL，NULL，NULL);//枚举子键 

printf(szKeyName); 

} 

最后需要注意的是在每次调用RegEnumKeyEx前必须重新将KeySize的值设为KeyMaxLen缓冲区的大小，因为每次函数返回时KeySize的值会变成返回的键值的名称长度，随着循环次数这个值会变小，而可能出现无法枚举所有键值项的情况。

### 5.枚举键值信息

枚举键值信息的方法与枚举子键信息极为相似，可以用RegEnumValue函数实现，其函数原型如下： 

RegEnumValue( 

hkey， //被枚举的键句柄 

dwIndex， //子键索引编号 

lpValueName， //键值名称 

lpcbValueName， //键值名称长度 

lpReserved， //系统保留，指定为0 

lpType， //键值数据类型 

lpDate， //键值数据 

lpcbDate //键值数据长度 

)； 

其实现代码如下： 

for(dwIndex=0;dwIndex<NameCnt;dwIndex++) //枚举键值 

...{ 

DateSize=MaxDateLen+1; 

NameSize=NameMaxLen+1; 

szValueName=(char *)malloc(NameSize); 

szValueDate=(LPBYTE)malloc(DateSize); 

/**//*参数定义请参照获取子键/键值信息部分...*/ 

RegEnumValue(hKey，dwIndex，szValueName，&NameSize，NULL，&Type，szValueDate，&DateSize);//读取键值

if(Type==REG_SZ) 

...{ 

/**//*判断键值项类型并做其它操作......*/ 

} 

if(Type==REG_DWORD) 

...{ 

} 

} 

与枚举子键相似，在每次循环中应该重新设置 数据长度DateSize=MaxDateLen+1，键值名称长度NameSize=NameMaxLen+1。 

### 6.创建/删除子键

创建子键跟打开子键差不多，可以用RegCreateKeyEx函数来实现， 

其原型如下： 

RegCreateKeyEx( 

hkey， //父键句柄 

lpSubKey， //子键句柄 

Reserved， //系统保留，指定为0 

lpClass， //定义子键类名，通常设为NULL 

dwOptions， //创建子键时的选项 

samDesired， //创建后操作权限 

lpSecurityAttributes， //指向SECURITY_ATTRIBUTES结构，指定键句柄的继//承性 

phkResult， //返回创建句柄 

lpdwDisposition //通常设为NULL 

)； 

创建子键也可以用16位下的API函数RegCreateKey来实现。 

其实例代码如下： 

HKEY KEY; 

if (ERROR_SUCCESS!=RegCreateKey(HKEY_LOCAL_MACHINE，"SOFTWARE\Microsoft\Windows\MyKey"，&KEY))

...{ 

MessageBox("创建失败!"); 

}else 

...{ 

MessageBox("创建成功!"); 

}； 

删除一个键可以用RegDeleteKey()实现，它有两个参数原型如下： 

RegDeleteKey( 

hkey， //主键句柄 

lpSubKey， //子键名称字符串 

)； 

如果想删除上面创建的MyKey子键可以用下面的代码实现： 

if(ERROR_SUCCESS==RegDeleteKey(HKEY_LOCAL_MACHINE，"SOFTWARE\Microsoft\Windows\MyKey"))

...{ 

AfxMessageBox("删除成功!"); 

}else 

...{ 

AfxMessageBox("删除失败!"); 

} 

需要注意的是， 在创建子键时可以创建多级子键，比如： 

RegCreateKey(HKEY_LOCAL_MACHINE，"SOFTWARE\Microsoft\Windows\MyKey1\MyKey2"，&KEY)；

如果MyKey1不存在的话，那么它将先创建MyKey1，再创建MyKey2，这一点与文件系统中创建目录是不同的。但是删除的时候却不能删除多级子键。比如想删除MyKey1，那么必须先删除MyKey2才可以。不过一个子键下面的多个键值可以一次删除。

### 7.创建/删除键值项

创建键值可以用RegSetValueEx函数来实现，它的原型如下： 

RegSetValueEx( 

hkey， //键句柄，键值项将保存在此键下 

lpValueName， //键值项名称 

Reserved， //系统保留，指定为0 

dwType， //键值项类型 

lpDate， //键值项数据 

cbDate //键值项长度 

)； 

使用这个函数的时个有一点需要注意，其中参数lpDate和cbDate的值要跟据dwType的值来设定，按常用设置我们分三种情况 

（1）当dwType为REG_SZ时，这时跟通常一样，lpDate为要设置的数据， cbDate为数据的长度。 

（2）当dwType为REG_DWORD 时，cbDate必须设为4。 

（3）当dwType为REG_BINARY 时，cbDate也必须设为4。 

如果调用时，键值项名称已经存在，则会覆盖原有键值项。如果没有就新建一个。 

实现功能的实例代码如下： 

void CreateValue：：OnCreate() 

...{ 

HKEY key; 

UpdateData(true); 

if(m_type=="REG_SZ") 

...{ 

if(RegOpenKeyEx(MKEY，SubKey，0，KEY_ALL_ACCESS，&key)==ERROR_SUCCESS) 

...{ 

if(：：RegSetValueEx(key，m_name，0，REG_SZ，(const unsigned char *)m_date，MAX_PATH)==ERROR_SUCCESS)

...{ 

MessageBox("创建成功!"); 

} 

} 

} 

if(m_type=="REG_DWORD") 

...{ 

if(RegOpenKeyEx(MKEY，SubKey，0，KEY_ALL_ACCESS，&key)==ERROR_SUCCESS) 

...{ 

if(：：RegSetValueEx(key，m_name，0，REG_DWORD，(const unsigned char *)m_date，4)==ERROR_SUCCESS)//注意数据长度应该设为4

...{ 

MessageBox("创建成功!"); 

} 

} 

} 

/**//*其它类型的设置......*/ 

} 

删除键值可以用RegDeleteValue来实现，它的函数原型如下： 

RegDeleteValue( 

hkey， //父键句柄 

lpValueName //要删除的键值项名称 

)； 

其实例代码如下： 

HKEY key; 

char value[MAX_PATH]="LengFeng" //键值 

LPCTSTR data="SOFTWARE\Microsoft\Windows\CurrentVersion\Run";//路径 

RegOpenKeyEx(HKEY_LOCAL_MACHINE，data，0，KEY_WRITE，&key); //打开 

if(ERROR_SUCCESS==：：RegDeleteValue(key，value)) //删除 

...{ 

MessageBox("删除成功!"); 

} 

### 8.备份/恢复注册表

备份和恢复注册表相对来说用的不是太多，仅用一个运行在CONSOLE32下的小程序来讨论一下它们的实现。 

备份注册表可以用RegSaveKey函来实现， 它的原形如下： 

RegSaveKey( 

hkey， //要备份的键句柄 

lpFile， //保存信息的文件名称 

lpSecurityAttributes //文件安全属性 

）； 

hkey为要备份的键句柄，可以是系统预定义的，也可以是用RegOpenKey()打开或是RegCreateKeyEx()创建的。 

lpFile为保存信息的文件名称，注意这个文件必须是不存在的，而且也不能有扩展名（否则RegRestoreKey()函无法读取）。 

lpSecurityAttributes：在NT系统中用来设置新文件的安全属性，通常设置为NULL。 

在使用这个函数时需要有SE_BACKUP_NAME权限，而这个权限是不可以在RegOpenKey()或是RegCreateKeyEx()中指定的。 

要做到这一点就必须提升程序的权限，其具体实现如下代码如示： 

#include <windows.h> 

#include <stdio.h> 

#include <stdlib.h> 

void main() 

...{ 

char strKey[]="Software\Microsoft\Internet Explorer"; 

LPTSTR szSaveFileName; 

HKEY key; 

// 申请备份权限 

HANDLE hToken; 

TOKEN_PRIVILEGES tkp; 

if(!OpenProcessToken(GetCurrentProcess()，TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY，&hToken))

return; 

LookupPrivilegeValue(NULL，SE_BACKUP_NAME，&tkp.Privileges[0].Luid);//申请SE_BACKUP_NAME权限

tkp.PrivilegeCount=1; 

tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED; 

AdjustTokenPrivileges(hToken，FALSE，&tkp，0，(PTOKEN_PRIVILEGES)NULL，0); 

//开始备份工作 

szSaveFileName=LPTSTR("D：\KeyDate"); //注意文件不可存在否则无法成功 

RegOpenKeyEx( 

HKEY_CURRENT_USER， 

(LPCTSTR)strKey， 

0， 

KEY_ALL_ACCESS， 

&key); 

RegSaveKey(key，szSaveFileName， NULL); 

RegCloseKey(key); 

} 

上面用到了提升权限的代码，以前杂志上面有很多可以参考一下来看，这样备份就完成了。 

恢复注册表可以用函数RegRestoreKey来实现，它的原形如下： 

RegRestoreKey( 

hkey， //要恢复的键句柄 

lpFile， //保存信息的文件名称 

dwFlage //标志是否易失 

）； 

此函数的前两个参数可以与RegSaveKey相同，而参数dwFlage为TRUE的话，是暂时恢复注册表，如果为FALSE则是永久修改注册表值。同样需要注意的是使用这个函数需要有SE_RESTORE_NAME权限。

[?](http://www.cnblogs.com/lartely/archive/2011/04/10/2011770.html#)

```
```cpp
// reg.cpp : Defines the entry point for the console application.
```

```cpp
//
```

```cpp

```

```cpp
#include "stdafx.h"
```

```cpp
#include "windows.h"
```

```cpp
#include "malloc.h"
```

```cpp

```

```cpp
int
```

```cpp
main(
```

```cpp
int
```

```cpp
argc,
```

```cpp
char
```

```cpp
* argv[])
```

```cpp
{
```

```cpp

```

```cpp
DWORD
```

```cpp
dwIndex=0,NameSize,NameCnt,NameMaxLen,Type;
```

```cpp

```

```cpp
DWORD
```

```cpp
KeySize,KeyCnt,KeyMaxLen,DateSize,MaxDateLen;
```

```cpp

```

```cpp
HKEY
```

```cpp
hKey;
```

```cpp

```

```cpp
char
```

```cpp
*szKeyName;
```

```cpp

```

```cpp
char
```

```cpp
*szValueName;
```

```cpp

```

```cpp
LPBYTE
```

```cpp
szValueDate;
```

```cpp

```

```cpp
//打开关闭注册表---------------------------------------------------------------
```

```cpp

```

```cpp
LPCTSTR
```

```cpp
SubKey=
```

```cpp
"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"
```

```cpp
;
```

```cpp

```

```cpp
if
```

```cpp
(RegOpenKeyEx(HKEY_LOCAL_MACHINE,SubKey,0,KEY_ALL_ACCESS,&hKey)!=
```

```cpp

```

```cpp
ERROR_SUCCESS)
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"RegOpenKeyEx错误"
```

```cpp
);
```

```cpp

```

```cpp
return
```

```cpp
0;
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
//获取子键信息---------------------------------------------------------------
```

```cpp

```

```cpp
if
```

```cpp
(RegQueryInfoKey(hKey,NULL,NULL,NULL,&KeyCnt,&KeyMaxLen,NULL,&NameCnt,&NameMaxLen,&MaxDateLen,NULL,NULL)!=ERROR_SUCCESS)
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"RegQueryInfoKey错误"
```

```cpp
);
```

```cpp

```

```cpp
::RegCloseKey(hKey);
```

```cpp

```

```cpp
return
```

```cpp
0;
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
//枚举子键信息---------------------------------------------------------------
```

```cpp

```

```cpp
for
```

```cpp
(dwIndex=0;dwIndex<KeyCnt;dwIndex++)
```

```cpp
//枚举子键
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
KeySize=KeyMaxLen+1;
```

```cpp
//因为RegQueryInfoKey得到的长度不包括0结束字符,所以应加1
```

```cpp

```

```cpp
szKeyName=(
```

```cpp
char
```

```cpp
*)
```

```cpp
malloc
```

```cpp
(KeySize);
```

```cpp

```

```cpp
RegEnumKeyEx(hKey,dwIndex,szKeyName,&KeySize,NULL,NULL,NULL,NULL);
```

```cpp
//枚举子键
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"%s\n"
```

```cpp
,szKeyName);
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
//枚举键值信息---------------------------------------------------------------
```

```cpp

```

```cpp
for
```

```cpp
(dwIndex=0;dwIndex<NameCnt;dwIndex++)
```

```cpp
//枚举键值
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
DateSize=MaxDateLen+1;
```

```cpp

```

```cpp
NameSize=NameMaxLen+1;
```

```cpp

```

```cpp
szValueName=(
```

```cpp
char
```

```cpp
*)
```

```cpp
malloc
```

```cpp
(NameSize);
```

```cpp

```

```cpp
szValueDate=(
```

```cpp
LPBYTE
```

```cpp
)
```

```cpp
malloc
```

```cpp
(DateSize);
```

```cpp

```

```cpp
RegEnumValue(hKey,dwIndex,szValueName,&NameSize,NULL,&Type,szValueDate,&DateSize);
```

```cpp
//读取键值
```

```cpp

```

```cpp

```

```cpp
if
```

```cpp
(Type==REG_SZ)
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
///*判断键值项类型并做其它操作......*/
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"%s\n"
```

```cpp
,szValueName);
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
if
```

```cpp
(Type==REG_DWORD)
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
RegCloseKey(hKey);
```

```cpp

```

```cpp
//创建删除子键---------------------------------------------------------------
```

```cpp

```

```cpp
if
```

```cpp
(ERROR_SUCCESS!=RegCreateKey(HKEY_LOCAL_MACHINE,
```

```cpp
"SOFTWARE\\Microsoft\\Windows\\MyKey"
```

```cpp
,&hKey))
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"创建子键失败!\n"
```

```cpp
);
```

```cpp

```

```cpp
return
```

```cpp
0;
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
else
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"创建子键成功!\n"
```

```cpp
);
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
if
```

```cpp
(ERROR_SUCCESS==RegDeleteKey(HKEY_LOCAL_MACHINE,
```

```cpp
"SOFTWARE\\Microsoft\\Windows\\MyKey"
```

```cpp
))
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"删除子键成功!\n"
```

```cpp
);
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
else
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"删除子键失败!\n"
```

```cpp
);
```

```cpp

```

```cpp
RegCloseKey(hKey);
```

```cpp

```

```cpp
return
```

```cpp
0;
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
RegCloseKey(hKey);
```

```cpp

```

```cpp
//创建删除键值---------------------------------------------------------------
```

```cpp

```

```cpp
if
```

```cpp
(RegOpenKeyEx(HKEY_LOCAL_MACHINE,SubKey,0,KEY_ALL_ACCESS,&hKey)!=ERROR_SUCCESS)
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"创建HKEY失败!\n"
```

```cpp
);
```

```cpp

```

```cpp
return
```

```cpp
0;
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
char
```

```cpp
*szValueName1=
```

```cpp
"QQ"
```

```cpp
;
```

```cpp

```

```cpp
char
```

```cpp
*szValueDate1=
```

```cpp
"This is QQ"
```

```cpp
;
```

```cpp

```

```cpp
UINT
```

```cpp
cbLen=
```

```cpp
strlen
```

```cpp
(szValueDate1);
```

```cpp

```

```cpp
char
```

```cpp
*szValueName2=
```

```cpp
"TT"
```

```cpp
;
```

```cpp

```

```cpp
UINT
```

```cpp
tmp=16;
```

```cpp

```

```cpp
UINT
```

```cpp
*szValueDate2=&tmp;
```

```cpp

```

```cpp
if
```

```cpp
(RegSetValueEx(hKey,szValueName1,0,REG_SZ,(
```

```cpp
const
```

```cpp
unsigned
```

```cpp
char
```

```cpp
*)szValueDate1,cbLen)==ERROR_SUCCESS)
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"创建REG_SZ键值成功!\n"
```

```cpp
);
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
else
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"创建REG_SZ键值失败!\n"
```

```cpp
);
```

```cpp

```

```cpp
return
```

```cpp
0;
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
if
```

```cpp
(RegSetValueEx(hKey,szValueName2,0,REG_DWORD,(
```

```cpp
const
```

```cpp
unsigned
```

```cpp
char
```

```cpp
*)szValueDate2,4)==ERROR_SUCCESS)
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"创建REG_DWORD键值成功!\n"
```

```cpp
);
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
else
```

```cpp

```

```cpp
{
```

```cpp

```

```cpp
printf
```

```cpp
(
```

```cpp
"创建REG_DWORD键值失败!\n"
```

```cpp
);
```

```cpp

```

```cpp
RegCloseKey(hKey);
```

```cpp

```

```cpp
return
```

```cpp
0;
```

```cpp

```

```cpp
}
```

```cpp

```

```cpp
RegCloseKey(hKey);
```

```cpp

```

```cpp
return
```

```cpp
0;
```

```cpp
}
```
```



