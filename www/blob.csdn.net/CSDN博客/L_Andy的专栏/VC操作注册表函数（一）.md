# VC操作注册表函数（一） - L_Andy的专栏 - CSDN博客

2012年11月28日 16:17:56[卡哥](https://me.csdn.net/L_Andy)阅读数：4729


VC操作注册表函数收藏

张明奇（卡哥）
最近在做一个项目的时候使用到一些操作注册表的函数，现将这些函数记录以下，以方便以后使用

    1. 打开注册表键

   LONG RegOpenKeyEx(

  HKEY hKey,                            // handle to open key主键

  LPCTSTR lpSubKey,           // subkey name子键

  DWORD ulOptions,   // reserved。必须是0

  REGSAM samDesired,   // security access mask读写标识

  PHKEY phkResult    // handle to open key返回的HKEY类型的指针。以后，读写，关闭用这个指针

  );
如：

// 打开HKEY_LOCAL_MACHINE主键下的SoftWare\\Cleaner\\Cleaner子键

 HKEY hKEY;

 HKEY  hKeyRoot = HKEY_LOCAL_MACHINE;

 long ret0=(::RegOpenKeyEx(hKeyRoot,"SoftWare\\Cleaner\\Cleaner",0,KEY_READ,&hKEY));

 if(ret0!=ERROR_SUCCESS)//如果无法打开hKEY,则中止程序的执行

 {

  AfxMessageBox("错误：无法打开有关的hKEY");

  return;

 }

    2. 读取注册表

 LONG RegQueryValueEx(

  HKEY hKey,            // handle to key打开注册表指针

  LPCTSTR lpValueName,  // value name要读取的键名称

  LPDWORD lpReserved,   // reserved  must be NULL. 必须是NULL

  LPDWORD lpType,       // type buffer，键类型。我最常用REG_SZ,REG_DWORD

  LPBYTE lpData,        // data buffer。保存查询结果的缓冲区

  LPDWORD lpcbData      // size of data buffer。缓冲区大小

);
如：

// hKEY是上面打开时得到的指针。

 LPBYTE getValue = new BYTE[80];//得到的键值

 DWORD keyType = REG_SZ;//定义数据类型

 DWORD DataLen = 80;//定义数据长度

 CString strUser = _T("Version");//要查询的键名称

 long ret1=::RegQueryValueEx(hKEY,strUser,NULL,&keyType,getValue,&DataLen);

 if(ret1!=ERROR_SUCCESS)

 {

  AfxMessageBox("错误：无法查询有关的注册表信息");

  return;

 }

    3. 写注册表

  LONG RegSetValueEx(

  HKEY hKey,           // handle to key。打开注册表的指针

  LPCTSTR lpValueName, // value name 要写入的键

  DWORD Reserved,      // reserved  必须是0

  DWORD dwType,        // value type 写入值类型

  CONST BYTE *lpData,  // value data 要写入的数据

  DWORD cbData         // size of value data 。数据SIZE

);
如：

// 写注册表。修改Version为1.0.12

// 写入CString类型的数据

CString strVersion = _T("Version");//要写入的键名称

LPCTSTR strVersionValue = "1.0.12";

long ret = ::RegSetValueEx(hKEY, strVersion, 0, REG_SZ, (const BYTE *) strVersionValue, strlen(strVersionValue)+1);

if(ret!=ERROR_SUCCESS)

{

 AfxMessageBox("错误：无法查询有关的注册表信息");

 return;

}

[/code]

    4. 创建一个新键

LONG RegCreateKeyEx(

  HKEY hKey,                                  // handle to open key。打开的注册表指针

  LPCTSTR lpSubKey,                           // subkey name。子键名称

  DWORD Reserved,                             // reserved。必须为0

  LPTSTR lpClass,                             // class string。已经存在时用，一般为NULL

  DWORD dwOptions,                            // special options

               //默认值REG_OPTION_VOLATILE，保存在注册表，下次开机仍然存在

               //REG_OPTION_VOLATILE，保存在内存

               //REG_OPTION_BACKUP_RESTORE

  REGSAM samDesired,                          // desired security access。操作权限。一般KEY_ALL_ACCESS，除非有特殊需要，请查阅MSDN

  LPSECURITY_ATTRIBUTES lpSecurityAttributes, // inheritance。继承性。一般为NULL

  PHKEY phkResult,                            // key handle 。返回该键值镇。

  LPDWORD lpdwDisposition                     // disposition value buffer

             //REG_CREATED_NEW_KEY The key did not exist and was created.

           //REG_OPENED_EXISTING_KEY The key existed and was simply opened without being changed.

);

    5. 删除一个键

LONG RegDeleteKey(

  HKEY hKey,         // handle to open key

  LPCTSTR lpSubKey   // subkey name

);

    6. 删除一个键值

LONG RegDeleteValue(

  HKEY hKey,            // handle to key

  LPCTSTR lpValueName   // value name。值名称，不是打开的那个指针，是查询到的指针，如果为空RegSetValueEx创建的值将被删除

);

    7. 刷新注册表

LONG RegFlushKey(

  HKEY hKey   // handle to key to write。写入所有的值，在给定的指针

);

//这个函数是将改变数据直接写到硬盘上，不要频繁使用，会影响性能

    8.  导入一个注册表文件到指定的键下

LONG RegLoadKey(

  HKEY hKey,        // handle to open key

  LPCTSTR lpSubKey, // subkey name

  LPCTSTR lpFile    // registry file name

);

//没有用

    9. 关闭打开的注册表

LONG RegCloseKey(

  HKEY hKey   // handle to key to close

);

//打开的注册表的指针需要注意关闭释放

总结

//注册表的删除一个键的操作

最先想到的函数是RegDeleteKey，RegDeleteKey函数不能删除带有子项的注册表项，果然问题在这里。果断上MSDN，MSDN上是这么说的The subkey to be deleted must not have subkeys. To delete akey and all its subkeys, you need to enumerate the subkeys
 and delete themindividually. To delete keys recursively, use the [**RegDeleteTree**]()or**SHDeleteKey** function.大意是RegDeleteKey函数不能删除一个含有子项的注册表项，如果想要删除此类的项，必须先枚举再删除。你也可以使用RegDeleteTree或者SHDeleteKey函数。（本人英语不好，大概说一下大意）。到这里才知道原来RegDeleteKey函数不能删除带子项的项，难怪一直没有被删除。而RegDeleteKey函数又只能在VISTA系统以上的平台上编译，我就只能一个个遍历删除了。

//这个程序是在网上搜的，不知道具体能否实现

附:

删除带子项的注册表项（不使用RegDeleteKey函数）

BOOL NosafeModle(void)     

{

HKEYhKey;

DWORDcount;

LPTSTRdata1=_T("SYSTEM//CurrentControlSet//Control//SafeBoot//Minimal");

LPTSTRdata2=_T("SYSTEM//CurrentControlSet//Control//SafeBoot//Network");

/*if(RegCreateKey(HKEY_CURRENT_USER,data,&hKey)==ERROR_SUCCESS)

{

RegDeleteKey(hKey,_T("Minimal"));

RegDeleteKey(hKey,_T("Network"));

}*/

if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,data1,0,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)

{   if(RegQueryInfoKey(hKey,NULL,NULL,NULL,&count,NULL,NULL,NULL,NULL,NULL,NULL,NULL)==ERROR_SUCCESS)

{

DWORDdwIndex=0;

char szName[256]={0};

for(;dwIndex<count;dwIndex++)

{

if(RegEnumKey(hKey,dwIndex,szName,257)==ERROR_SUCCESS)

{

RegDeleteKey(hKey,(constchar *)szName);

}

}

}

}

if(RegOpenKeyEx(HKEY_LOCAL_MACHINE,data2,0,KEY_ALL_ACCESS,&hKey)==ERROR_SUCCESS)

//如下本人写的例子

voidCProjectManagedlg::OnBnClickedButtonClearList()

{

// TODO: 在此添加控件通知处理程序代码

CString
keyValue;       

HKEY
hkOpen;       

RegOpenKey(HKEY_CURRENT_USER,"SOFTWARE\\LvComp\\RadioPlaningD\\RecentList\\",&hkOpen);

if(hkOpen ==NULL)return;

int
nIndexValue=0;       

for(;;)       

    {      

char
ValueName[256];           

byte
DataValue[256];           

DWORD
cbValueName=256;           

DWORD
cbDataValue=256;           

DWORD
dwType;           

if(ERROR_NO_MORE_ITEMS==      

RegEnumValue(       

hkOpen,       

nIndexValue,       

ValueName,           

           &cbValueName,          

NULL,           

           &dwType,           

DataValue,           

           &cbDataValue)       

           )       

        {       

break;       

       }      

//if(nIndexValue  0) 

       {

RegDeleteValue(hkOpen,ValueName);

       }    

       //nIndexValue++;

    }      

m_lstRecentProject.DeleteAllItems();

}

