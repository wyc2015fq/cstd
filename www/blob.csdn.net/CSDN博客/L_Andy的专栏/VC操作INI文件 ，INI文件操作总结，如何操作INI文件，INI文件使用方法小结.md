# VC操作INI文件 ，INI文件操作总结，如何操作INI文件，INI文件使用方法小结 - L_Andy的专栏 - CSDN博客

2015年12月02日 11:05:52[卡哥](https://me.csdn.net/L_Andy)阅读数：2644


**INI文件简介**

在我们写程序时,总有一些配置信息需要保存下来,以便在下一次启动程序完成初始化，这实际上是一种类持久化。将一些信息写入INI文件（initialization file）中,可完成简单的持久化支持。

Windows提供了API接口用于操作INI文件，其支持的INI文件格式一般如下：

===============================

[Section1]

Key11=value11

Key12=value12

[Section2]

Key21=value21

Key22=value22

...

[SectionN]

KeyN1=valueN1

KeyN2=valueN2

===============================

一般一个INI文件可有N个节，每节可有n个键名及值对应，每个键名及其值以等式形式占一行。

一般键的名称可任取，不过建议用有意义的字符及词构成。**值一般可为整数和字符串**，其它类型要进行转换。

常见的系统配置文件：

C:/boot.ini

C:/WINDOWS/win.ini

C:/WINDOWS/system.ini

C:/WINDOWS/**desktop.ini**

C:/WINDOWS/Resources/Themes/Windows Classic.theme

注意，字符串存贮在INI文件中时没有引号；key和value之间的等号前后不容空格；注释以分号“;”开头。

**VC中操作INI文件的API**

(1)操作系统配置文件Win.ini的函数：

|函数名|功能|
|----|----|
|*Get*ProfileSection|读取win.ini中指定节lpAppName中所有键名及其值。*lpReturnedString*字符串形式如下：Key1=Value1/0Key2=Value2/0…KeyN=ValueN/0/0|
|*Get*ProfileString|读取win.ini中指定节lpAppName中键名为lpKeyName对应变量的字符串值。|
|*Get*ProfileInt|读取win.ini中指定节lpAppName中键名为lpKeyName对应变量的整数值。|
|||
|*Write*ProfileSection|写（替换）win.ini中指定节lpAppName中的键值。*lpString*字符串形式同GetProfileSection中的*lpReturnedString*。|
|*Write*ProfileString|写（替换）win.ini中指定节lpAppName中键名为lpKeyName对应变量的字符串值。|

 (2)操作用户自定义配置文件（PrivateProfile.ini）的函数：

|函数名|功能|
|----|----|
|*Get*PrivateProfileSectionNames|读取*lpFileName*指定的配置文件中所有的节名。*lpszReturnBuffer*字符串形式如下：Section1/0Section2/0…SectionN/0/0|
|*Get*PrivateProfileSection|同*Get*ProfileSection。|
|*Get*PrivateProfileString|同*Get*ProfileString。|
|*Get*PrivateProfileInt|同*Get*ProfileInt|
|*Get*PrivateProfileStruct|须同*Write*PrivateProfileStruct配套使用。|
|||
|*Write*PrivateProfileSection|同*Write*ProfileSection|
|*Write*PrivateProfileString|同*Write*ProfileString|
|*Write*PrivateProfileStruct|不常用。|

注意：

（1）使用得最频繁的是 GetPrivateProfileString 和 WritePrivateProfileString，没有WriteProfileInt/WritePrivateProfileInt函数。

（2）Get系列读取节键值，如果文件路径有误或节键名不对则返回设定的默认值。

（3）访存自定义配置文件时，**文件路径lpFileName必须完整,文件名前面的各级目录必须存在**。如果lpFileName文件路径不存在，则函数返回FALSE，GetLastError() = ERROR_PATH_NOT_FOUND。如果路径正确，但是文件不存在，则该函数将先创建该文件。如果路径及文件存在，则在现有ini文件基础上进行读写。

如果 lpFileName 只指定文件名而没有路径的话，调用API将会去 Windows 的安装目录去查找而不会在当前目录查找。

（4）要对调用API的模块(exe)所在目录下进行配置文件操作，可使用形如“.//config.ini”的相对路径，注意转义符。

（5）调用WritePrivateProfileSection，若参数三* lpString为*NULL，则可将对应section的全部内容清空；调用WritePrivateProfileString，若参数三* lpString为*NULL，则可将对应key删除。

**跨平台配置文件**

INI文件本质是对文件和字符串的处理，因此在跨平台项目中的配置文件可以基于<stdio.h>中的标C文件FILE，然后实现像类似以上对节（[Section]）、键（Key）和值（Value）的字符串读写功能。

鉴于XML的树形描述层次结构性清晰，现在很多软件都大面积使用XML文件进行配置，如QQ的全局配置文件C:/Program Files/Tencent/QQ/gf-config.xml。java程序的配置文件基本都使用XML格式，C++中并没有操作XML文件的标准库。

在C/C++程序中要使用XML做为配置文件，涉及到XML的解析。Windows平台可使用MsXml对XML进行解析，参考《[MsXml创建和解析XML示例](http://blog.csdn.net/phunxm/archive/2009/12/27/5087102.aspx)》，跨平台可以考虑自己实现，或使用C++ BOOST正则表达式，或选择[Free
 C or C++ XML Parser Libraries](http://lars.ruoff.free.fr/xmlcpp/)，如[XmlParser](http://www.applied-mathematics.net/tools/xmlParser.html)、[TinyXML](http://www.grinninglizard.com/tinyxml/)、[CMarkup](http://www.firstobject.com/)、[libxml](http://xmlsoft.org/)等。

**CIniFile类**

以下提供对Windows操作INI文件的API的简单封装类CIniFile。

**[cpp]**[view
 plain](http://blog.csdn.net/lewutian/article/details/6787048#)[copy](http://blog.csdn.net/lewutian/article/details/6787048#)

- // IniFile.h   
- #ifndef __INIFILE_H__   
- #define __INIFILE_H__   
- 
- class CIniFile    
- {    
- public:    
-     CIniFile();    
-     CIniFile(LPCTSTR szFileName);    
- virtual ~CIniFile();    
- 
- public:    
- // Attributes      
- void SetFileName(LPCTSTR szFileName);    
- 
- public:    
- // Operations   
- BOOL SetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int nKeyValue);    
- BOOL SetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszKeyValue);    
- 
- DWORD GetProfileSectionNames(CStringArray& strArray); // 返回section数量   
- 
- int GetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName);    
- DWORD GetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, CString& szKeyValue);    
- 
- BOOL DeleteSection(LPCTSTR lpszSectionName);    
- BOOL DeleteKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName);    
- 
- private:    
-     CString  m_szFileName; // .//Config.ini, 如果该文件不存在，则exe第一次试图Write时将创建该文件   
- 
- UINT m_unMaxSection; // 最多支持的section数(256)   
- UINT m_unSectionNameMaxSize; // section名称长度，这里设为32(Null-terminated)   
- 
- void Init();    
- };    
- 
- #endif   
- 
- // IniFile.cpp   
- #include "IniFile.h"   
- 
- void CIniFile::Init()    
- {    
-     m_unMaxSection = 512;    
-     m_unSectionNameMaxSize = 33; // 32位UID串   
- }    
- 
- CIniFile::CIniFile()    
- {    
-     Init();    
- }    
- 
- CIniFile::CIniFile(LPCTSTR szFileName)    
- {    
- // (1) 绝对路径，需检验路径是否存在   
- // (2) 以"./"开头，则需检验后续路径是否存在   
- // (3) 以"../"开头，则涉及相对路径的解析   
- 
-     Init();    
- 
- // 相对路径   
-     m_szFileName.Format(".//%s", szFileName);    
- }    
- 
- CIniFile::~CIniFile()      
- {    
- 
- }    
- 
- void CIniFile::SetFileName(LPCTSTR szFileName)    
- {    
-     m_szFileName.Format(".//%s", szFileName);    
- }    
- 
- DWORD CIniFile::GetProfileSectionNames(CStringArray &strArray)    
- {    
- int nAllSectionNamesMaxSize = m_unMaxSection*m_unSectionNameMaxSize+1;    
- char *pszSectionNames = newchar[nAllSectionNamesMaxSize];    
- DWORD dwCopied = 0;    
-     dwCopied = ::GetPrivateProfileSectionNames(pszSectionNames, nAllSectionNamesMaxSize, m_szFileName);    
- 
-     strArray.RemoveAll();    
- 
- char *pSection = pszSectionNames;    
- do
-     {    
-         CString szSection(pSection);    
- if (szSection.GetLength() < 1)    
-         {    
- delete[] pszSectionNames;    
- return dwCopied;    
-         }    
-         strArray.Add(szSection);    
- 
-         pSection = pSection + szSection.GetLength() + 1; // next section name   
-     } while (pSection && pSection<pszSectionNames+nAllSectionNamesMaxSize);    
- 
- delete[] pszSectionNames;    
- return dwCopied;    
- }    
- 
- DWORD CIniFile::GetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, CString& szKeyValue)    
- {    
- DWORD dwCopied = 0;    
-     dwCopied = ::GetPrivateProfileString(lpszSectionName, lpszKeyName, "",     
-         szKeyValue.GetBuffer(MAX_PATH), MAX_PATH, m_szFileName);    
-     szKeyValue.ReleaseBuffer();    
- 
- return dwCopied;    
- }    
- 
- int CIniFile::GetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName)    
- {    
- int nKeyValue = ::GetPrivateProfileInt(lpszSectionName, lpszKeyName, 0, m_szFileName);    
- 
- return nKeyValue;    
- }    
- 
- BOOL CIniFile::SetProfileString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszKeyValue)    
- {    
- return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, lpszKeyValue, m_szFileName);    
- }    
- 
- BOOL CIniFile::SetProfileInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int nKeyValue)    
- {    
-     CString szKeyValue;    
-     szKeyValue.Format("%d", nKeyValue);    
- 
- return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, szKeyValue, m_szFileName);    
- }    
- 
- BOOL CIniFile::DeleteSection(LPCTSTR lpszSectionName)    
- {    
- return ::WritePrivateProfileSection(lpszSectionName, NULL, m_szFileName);    
- }    
- 
- BOOL CIniFile::DeleteKey(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName)    
- {    
- return ::WritePrivateProfileString(lpszSectionName, lpszKeyName, NULL, m_szFileName);    
- }    


在VC++中读写INI文件

　　在我们写的程序当中,总有一些配置信息需要保存下来,以便完成程序的功能,最简单的办法就是将这些信息写入INI文件中,程序初始化时再读入.具体应用如下:

　　一.将信息写入.INI文件中.

　　1.所用的WINAPI函数原型为: 

BOOL WritePrivateProfileString(

LPCTSTR lpAppName,

LPCTSTR lpKeyName,

LPCTSTR lpString,

LPCTSTR lpFileName

);

　　其中各参数的意义:

　　　LPCTSTR lpAppName 是INI文件中的一个字段名.

　　　LPCTSTR lpKeyName 是lpAppName下的一个键名,通俗讲就是变量名.

　　　LPCTSTR lpString 是键值,也就是变量的值,不过必须为LPCTSTR型或CString型的.

　　　LPCTSTR lpFileName 是完整的INI文件名.

　　2.具体使用方法:设现有一名学生,需把他的姓名和年龄写入 c:\stud\student.ini 文件中. 

CString strName,strTemp;

int nAge;

strName="张三";

nAge=12;

::WritePrivateProfileString("StudentInfo","Name",strName,"c:\\stud\\student.ini");

　　此时c:\stud\student.ini文件中的内容如下:

　　　[StudentInfo]

　　　Name=张三

　　3.要将学生的年龄保存下来,只需将整型的值变为字符型即可:

strTemp.Format("%d",nAge);

::WritePrivateProfileString("StudentInfo","Age",strTemp,"c:\\stud\\student.ini");

　二.将信息从INI文件中读入程序中的变量.

　　1.所用的WINAPI函数原型为:

DWORD GetPrivateProfileString(

LPCTSTR lpAppName, 

LPCTSTR lpKeyName, 

LPCTSTR lpDefault, 

LPTSTR lpReturnedString, 

DWORD nSize, 

LPCTSTR lpFileName 

);

　　其中各参数的意义: 

　　　前二个参数与 WritePrivateProfileString中的意义一样.

　　　lpDefault : 如果INI文件中没有前两个参数指定的字段名或键名,则将此值赋给变量. 

　　　lpReturnedString : 接收INI文件中的值的CString对象,即目的缓存器.

　　　nSize : 目的缓存器的大小.

　　　lpFileName : 是完整的INI文件名.

　　2.具体使用方法:现要将上一步中写入的学生的信息读入程序中.

CString strStudName;

int nStudAge; 

GetPrivateProfileString("StudentInfo","Name","默认姓名",strStudName.GetBuffer(MAX_PATH),MAX_PATH,"c:\\stud\\student.ini");

　　执行后 strStudName 的值为:"张三",若前两个参数有误,其值为:"默认姓名".

　　3.读入整型值要用另一个WINAPI函数: 

UINT GetPrivateProfileInt(

LPCTSTR lpAppName, 

LPCTSTR lpKeyName, 

INT nDefault, 

LPCTSTR lpFileName 

);

　　这里的参数意义与上相同.使用方法如下:

nStudAge=GetPrivateProfileInt("StudentInfo","Age",10,"c:\\stud\\student.ini");

　三.循环写入多个值,设现有一程序,要将最近使用的几个文件名保存下来,具体程序如下:

　　1.写入:

CString strTemp,strTempA;

int i;

int nCount=6;

file://共有6个文件名需要保存

for(i=0;i {strTemp.Format("%d",i);

strTempA=文件名;

file://文件名可以从数组,列表框等处取得.

::WritePrivateProfileString("UseFileName","FileName"+strTemp,strTempA,

"c:\\usefile\\usefile.ini");

}

strTemp.Format("%d",nCount);

::WritePrivateProfileString("FileCount","Count",strTemp,"c:\\usefile\\usefile.ini");

file://将文件总数写入,以便读出.

　　2.读出:

nCount=::GetPrivateProfileInt("FileCount","Count",0,"c:\\usefile\\usefile.ini");

for(i=0;i {strTemp.Format("%d",i);

strTemp="FileName"+strTemp;

::GetPrivateProfileString("CurrentIni",strTemp,"default.fil", strTempA.GetBuffer(MAX_PATH),MAX_PATH,"c:\\usefile\\usefile.ini");

file://使用strTempA中的内容.

}

　　补充四点:

　　　1.INI文件的路径必须完整,文件名前面的各级目录必须存在,否则写入不成功,该函数返回 FALSE 值.

　　　2.文件名的路径中必须为 \\ ,因为在VC++中, \\ 才表示一个 \ .

　　　3.也可将INI文件放在程序所在目录,此时 lpFileName 参数为: ".\\student.ini".

　　　4.从网页中粘贴源代码时,最好先粘贴至记事本中,再往VC中粘贴,否则易造成编译错误,开始时我也十分不解,好好的代码怎么就不对呢?后来才找到这个方法.还有一些代码中使用了全角字符如:＜，＼等,也会

造成编译错误.




============

VC中用函数读写ini文件的方法

         ini文件(即Initialization file)，这种类型的文件中通常存放的是一个程序的初始化信息。ini文件由若干个节(Section)组成，每个Section由若干键(Key)组成，每个Key可以赋相应的值。读写ini文件实际上就是读写某个的Section中相应的Key的值，而这只要借助几个函数即可完成。

**一、向ini文件中写入信息的函数**
1. 把信息写入系统的win.ini文件

BOOL WriteProfileString(

      LPCTSTR lpAppName, // 节的名字，是一个以0结束的字符串

      LPCTSTR lpKeyName, // 键的名字，是一个以0结束的字符串。若为NULL，则删除整个节

      LPCTSTR lpString       // 键的值，是一个以0结束的字符串。若为NULL，则删除对应的键

)

2. 把信息写入自己定义的.ini文件

BOOL WritePrivateProfileString(

      LPCTSTR lpAppName,      // 同上

      LPCTSTR lpKeyName,      // 同上

      LPCTSTR lpString,       // 同上

      LPCTSTR lpFileName      // 要写入的文件的文件名。若该ini文件与程序在同一个目录下，也可使用相对

            //路径,否则需要给出绝度路径。

)

如：

::WriteProfileString("Test","id","xym"); 

//在win.ini中创建一个Test节，并在该节中创建一个键id,其值为xym

::WritePrivateProfileString("Test","id","xym","d://vc//Ex1//ex1.ini");

//在Ex1目录下的ex1.ini中创建一个Test节，并在该节中创建一个键id,其值为xym

//若Ex1.ini文件与读写该文件的程序在同一个目录下，则上面语句也可写为：

::WritePrivateProfileString("Test","id","xym",".//ex1.ini");

需要注意的是，C系列的语言中，转义字符'//'表示反斜线'/'。另外，当使用相对路径时，//前的.号不能丢掉了。

**二、从ini文件中读取数据的函数**
1、从系统的win.ini文件中读取信息

(1) 读取字符串

DWORD GetProfileString(

      LPCTSTR lpAppName,            // 节名

      LPCTSTR lpKeyName,            // 键名，读取该键的值

      LPCTSTR lpDefault,            // 若指定的键不存在，该值作为读取的默认值

      LPTSTR lpReturnedString,      // 一个指向缓冲区的指针，接收读取的字符串

      DWORD nSize                   // 指定lpReturnedString指向的缓冲区的大小

)

如：

CString str;

::GetProfileString("Test","id","Error",str.GetBuffer(20),20);

(2) 读取整数

UINT GetProfileInt(

      LPCTSTR lpAppName,      // 同上

      LPCTSTR lpKeyName,      // 同上

      INT nDefault            // 若指定的键名不存在，该值作为读取的默认值

)

如使用以下语句写入了年龄信息：

::WriteProfileString("Test","age","25"); 

//在win.ini中创建一个Test节，并在该节中创建一个键age,其值为25

则可用以下语句读取age键的值：

int age;

age=::GetProfileInt("Test","age",0);

2、从自己的ini文件中读取信息

(1) 读取字符串

DWORD GetPrivateProfileString(

      LPCTSTR lpAppName,            // 同1(1)

      LPCTSTR lpKeyName,            // 同1(1)

      LPCTSTR lpDefault,            // 同1(1)

      LPTSTR lpReturnedString,      // 同1(1)

      DWORD nSize,                  // 同1(1)

      LPCTSTR lpFileName            // 读取信息的文件名。若该ini文件与程序在同一个目录下，也可使用相      

            //对路径,否则需要给出绝度路径。

)

如：

CString str;

::GetPrivateProfileString("Test","id","Error",str.GetBuffer(20),20，".//ex1.ini");

或：

::GetPrivateProfileString("Test","id","Error",str.GetBuffer(20),20，"d://vc//Ex1//ex1.ini");

(2) 读取整数

UINT GetPrivateProfileInt(

      LPCTSTR lpAppName,      // 同上

      LPCTSTR lpKeyName,      // 同上

      INT nDefault,           // 若指定的键名不存在，该值作为读取的默认值

      LPCTSTR lpFileName      // 同上

)

如使用以下语句写入了年龄信息：

::WritePrivateProfileString("Test","age","25",".//ex1.ini"); 

//在ex1.ini中创建一个Test节，并在该节中创建一个键age,其值为25

则可用以下语句读取age键的值：

int age;

age=::GetPrivateProfileInt("Test","age",0,".//ex1.ini");

**三、 删除键值或节**

       回顾一下WriteProfileString函数的说明

BOOL WriteProfileString(

      LPCTSTR lpAppName, // 节的名字，是一个以0结束的字符串

      LPCTSTR lpKeyName, // 键的名字，是一个以0结束的字符串。若为NULL，则删除整个节

      LPCTSTR lpString       // 键的值，是一个以0结束的字符串。若为NULL，则删除对应的键

)

       由此可见，要删除某个节，只需要将WriteProfileString第二个参数设为NULL即可。而要删除某个键，则只需要将该函数的第三个参数设为 NULL即可。这是删除系统的win.ini中的节或键，类似的，要删除自己定义的ini文件中的节或键，也可做相同的操作。

       如：

::WriteProfileString("Test",NULL,NULL);      //删除win.ini中的Test节

::WriteProfileString("Test","id",NULL);      //删除win.ini中的id键

::WritePrivateProfileString("Test",NULL,NULL,".//ex1.ini");      //删除ex1.ini中的Test节

::WritePrivateProfileString("Test","id",NULL,".//ex1.ini");      //删除ex1.ini中的id键

**四、如何判断一个ini文件中有多少个节**

       要判断一个ini文件中有多少个节，最简单的办法就是将所有的节名都找出来，然后统计节名的个数。而要将所有的节名找出来，使用GetPrivateProfileSectionNames函数就可以了，其原型如下：

DWORD GetPrivateProfileSectionNames(

      LPTSTR lpszReturnBuffer,      // 指向一个缓冲区，用来保存返回的所有节名

      DWORD nSize,                  // 参数lpszReturnBuffer的大小

      LPCTSTR lpFileName            // 文件名，若该ini文件与程序在同一个目录下，

                                                //也可使用相对路径,否则需要给出绝度路径

)

下面的是用来统计一个ini文件中共有多少个节的函数，当然，如果需要同时找到每个节中的各个键及其值，根据找到节名就可以很容易的得到了。

/*统计共有多少个节

节名的分离方法：若chSectionNames数组的第一字符是'/0'字符，则表明

有0个节。否则，从chSectionNames数组的第一个字符开始，顺序往后找，

直到找到一个'/0'字符，若该字符的后继字符不是 '/0'字符，则表明前

面的字符组成一个节名。若连续找到两个'/0'字符，则统计结束*/

int CTestDlg::CalcCount(void)

{

TCHAR       chSectionNames[2048]={0};       //所有节名组成的字符数组

char * pSectionName; //保存找到的某个节名字符串的首地址

int i;       //i指向数组chSectionNames的某个位置，从0开始，顺序后移

int j=0;      //j用来保存下一个节名字符串的首地址相对于当前i的位置偏移量

int count=0;      //统计节的个数

//CString name;

//char id[20];

::GetPrivateProfileSectionNames(chSectionNames,2048,".//ex1.ini");   

for(i=0;i<2048;i++,j++)

{

      if(chSectionNames[0]=='/0')

       break;       //如果第一个字符就是0，则说明ini中一个节也没有

      if(chSectionNames[i]=='/0')

      {

       pSectionName=&chSectionNames[i-j]; //找到一个0，则说明从这个字符往前，减掉j个偏移量，

            //就是一个节名的首地址

       j=-1;         //找到一个节名后，j的值要还原，以统计下一个节名地址的偏移量

            //赋成-1是因为节名字符串的最后一个字符0是终止符，不能作为节名

            //的一部分

       /*::GetPrivateProfileString(pSectionName,"id","Error",id,20,".//ex1.ini");

       name.Format("%s",id);*/   

       //在获取节名的时候可以获取该节中键的值，前提是我们知道该节中有哪些键。

       AfxMessageBox(pSectionName);      //把找到的显示出来

       if(chSectionNames[i+1]==0)

       {

         break;      //当两个相邻的字符都是0时，则所有的节名都已找到，循环终止

       }

      }   

}

return count;

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////

在VC程序中利用系统提供的GetPrivateProfileString及WritePrivateProfileString函数直接读写系统配置ini文件(指定目录下的Ini文件)

假设在当前目录下有一个文件名为Tets.ini的文件

用于保存用户名和密码

文件格式如下：

[Section1]

Item1=huzhifeng

Item2=1234565

1.写INI文件

void CINI_File_TestDlg::OnButtonWrite() 

{

 // TODO: Add your control notification handler code here

 CString strSection       = "Section1";

  CString strSectionKey    = "Item1";

 char strBuff[256];

  CString strValue       = _T("");

 CString strFilePath;

 strFilePath=GetCurrentDirectory(256,strBuff);  //获取当前路径

 strFilePath.Format("%s//Test.ini",strBuff);

 GetDlgItemText(IDC_EDIT_NAME,strValue);     //获取文本框内容：即姓名

 WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);  //写入ini文件中相应字段

 strSectionKey="Item2";

 GetDlgItemText(IDC_EDIT_PASSWORD,strValue);   //获取文本框内容：即密码

 WritePrivateProfileString(strSection,strSectionKey,strValue,strFilePath);

}

2.读INI文件内容

void CINI_File_TestDlg::OnButtonRead() 

{

 // TODO: Add your control notification handler code here

 CString strSection       = "Section1";

  CString strSectionKey    = "Item1";

 char strBuff[256];

 CString strValue       = _T("");

 CString strFilePath;

 strFilePath=GetCurrentDirectory(256,strBuff);  //获取当前路径

 strFilePath.Format("%s//Test.ini",strBuff);

 GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath); //读取ini文件中相应字段的内容

 strValue=strBuff;

 SetDlgItemText(IDC_EDIT_NAME,strValue);

 strSectionKey="Item2";

 GetPrivateProfileString(strSection,strSectionKey,NULL,strBuff,80,strFilePath);

 strValue=strBuff;

 SetDlgItemText(IDC_EDIT_PASSWORD,strValue);

 UpdateData(FALSE);

}

