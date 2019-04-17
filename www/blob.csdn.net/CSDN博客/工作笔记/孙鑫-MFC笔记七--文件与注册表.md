# 孙鑫-MFC笔记七--文件与注册表 - 工作笔记 - CSDN博客





2012年05月08日 20:49:59[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：6335








#### c 语言对文件的操作是通过 FILE 结构体来完成的

   1 定义一个文件结构体的指针，获取文件结构体指针并付给


   如何打开一个文件

   FILE *fopen(const char*filename,const char *mode);   //文件名，访问方式

   FILE *_wfopen(const wchar_t *filename,const wchar_t *mode);


   r 读取文件，文件必须存在，后面加b，就是以二进制的方式读

   w 打开一个空的文件，文件存在则内容被销毁，不存在就创建该文件，后面加b，就是以二进制的方式写

   a 打开文件，在文件的末尾写入数据

   r+ 为了读和写打开文件，文件必须存在

   w+ 打开一个空文件，

   a+ 为读取和添加打开一个文件

   如何写入一个文件

   size_t fwrite(const void *puffor,size_t size,size_t count,FILE *strcam);

                 指向将要被写入的数据  数据单位   单位数量    文件结构体指针

   FILE *pFile=fopen("1.txt","w");

   fwrite("[http://www.sunxin.org",1,strlen("http://www.sunxin.org"),pFile]()); //此时数据已经写入内存，退出程序就能看到内容

   //fclose(pFile);  //添加关闭文件语句，就不必等到退出程序才能在磁盘文件中看到内容

                   //但是要再次操作文件就又必须要再次打开文件，不方便，可以用另一个函数 fflush()；

   fflush(pFile);  //将缓冲区中的内容写入磁盘文件，这样就不必关闭文件才能看到文件数据了，也方便多次操作文件

移动文件内容指针到指定位置
   int fseek(FILE *stream,long offset,int origin);  //文件结构体指针，相对起始位置的偏移量，起始位置

   起始位置取值有三个：

   SEEK_CUR 当前位置 

   SEEK_END 文件末

   SEEK_SET 文件头

   FILE *pFile=fopen("1.txt","w");

   fwrite("[http://www.sunxin.org",1,strlen("http://www.sunxin.org"),pFile]());

   fseek(pFile,0,SEEK_SET);

   fwrite("ftp:",1,strlen("ftp:"),pFile);  //此处写入的ftp:会把原来字串的开头http给覆盖掉

读取文件
   size_t fread(void *buffor,size_t size,size_t count,FILE *stream);

   FILE *pFile=fopen("1.txt","r");

   char ch[100];

   fread(ch,1,100,pFile);

   MessageBox(ch);        //此处显示的除了文件中的字符串，还有ch[100]中未占用，其他空间的乱码

   fclose(pFile);

   解决显示未占用空间的乱码问题有两个办法：

   a 在写入数据的时候多写一个空字符作为结尾

     fwrite("[http://www.sunxin.org",1,strlen("http://www.sunxin.org")+1]() ,pFile);

   b 用memset()把多余的空间内容全部清零

     void *memset(void *dcst,int c,size_t count);  //用指定的字符填充一块内存区域

                  内存        字符      要填充的数目

     memset(ch,0,100);

 c获取文件内容指针的位置

   long ftell(FILE *stream);  //配合fseek()把指针移到文件末尾，就能够得到文件的长度了

   把文件指针放置到文件开始处

   void rewind(FILE *strcam);

   FILE *pFile=fopen("1.txt","r");

   char *pBuf;

   fseek(pFile,0,SEEK_END);     //把文件指针移动到文件末尾，记得显示文件内容时要把指针移动回到文件头部

   int len=ftell(pFile);        //获取文件指针的位置，相当于得到文件的长度了

   pBuf=new char[len+1];        //根据文件长度申请内存空间，使读取文件长度获得一定的灵活性，注意后面加一的意义

   rewind(pFile);               //把文件指针移动到文件开头，当然你也可以用fseek()完成这个动作

   fread(pBuf,1,len,pFile);     //读取文件

   pBuf[len]=0;                 //把文件末尾置零，作为结束标志

   MessageBox(pBuf);            //输出

   fclose(pFile);

二进制文件和文本文件

   文本文件(也称为ASCII文件)：它的每一个字节存放的是可表示为一个字符的ASCII代码的文件。因此，给你一个整数，例如：98341，将这个整数保存到文件中，要求在以记事本程序打开文件时，显示的是98341，则编程时应该将该整数转换为字符串。如果直接存入整数，最终显示的将是98341对应的ASCII字符。


   当我们按照文本方式往文件中写入数据时，一旦遇到换行字符(ASCII为10)，则会转换为回车－换行(ASCII为13、10)。在读取文件时，一旦遇到回车－换行的组合(即连续的ASCII 13、10)，则会转换为换行字符(ASCII为10)。

   当我们按照二进制方式往文件中写入数据，则将数据在内存中的存储形式原样输出到文件中。

   由于二进制文件和文本文件操作上的差异，我们在操作文件的时候必须保持读写一致性，用什么方式写的，就用什么方式读


#### c++ 的方式文件操作



   写入文件

   ofstream::ofstream  //操作文件类，要包含头文件 fstream.h

   ofstream();

   ofstream(const char* szName,int nMode=ios::out,int nProt=filebut::openprot); //文件名，打开方式，文件保护说明

   ofstream(filedesc fd);

   ofstream(filedesc fd,char *pch,int nLength);

   ofstream ofs("4.txt");   //构造类对象

   ofs.write("[http://www.sunxin.org",strlen("http://www.sunxin.org]()"));

   ofs.close();

   读取文件

   ifstream::ifstream  //操作文件类，要包含头文件 fstream.h

   ifstream();

   ifstream(const char* szName,int nMode=ios::out,int nProt=filebut::openprot); //文件名，打开方式，文件保护说明

   ifstream(filedesc fd);

   ifstream(filedesc fd,char *pch,int nLength);

   ifstream ifs("4.txt");

   char ch[100];

   memset(ch,0,100);

   ifs.read(ch,100);

   ifs.close();

   MessageBox(ch);



#### win32 api 操作文件

   创建文件

   HANDLE CreateFile(      //该函数可以创建或者打开控制台,通信资源,目录,磁盘设备,文件,邮槽,管道,并返回相应句柄

      LPCTSTR lpFileName,                           //文件名

      DWORD dwDesiredAccess,                        //访问方式,读或写

      DWORD dwShareMode,                            //共享方式,为零则不共享

      LPSECURITY_ATTRIBUTES lpSecurityAttributes,   //创建的访问权限,确定返回的句柄是否能够被子进程继承,用于服务器端软件

      DWORD dwCreationDisposition,                  //如何创建

      DWORD dwFlagsAndAttributes,                   //设置文件属性和标记

      HANDLE hTemplateFile                          //模板文件句柄,如果在此传入文件句柄,则忽略上一个参数,如果打开一个

   );                                               //现有文件,则该参数被忽略

   写入数据

   BOOL WriteFile(

     HANDLE hFile,                    //文件的句柄

     LPCVOID lpBuffer,                //内存指针

     DWORD nNumberOfBytesToWrite,     //写多少个字节

     LPDWORD lpNumberOfBytesWritten,  //返回,实际写入到文件的字节数,传入一个变量接收就行了

     LPOVERLAPPED lpOverlapped        //告诉系统我们需要异步访问数据,否则用同步io访问数据,此参数要起作用,必须

   );                                 //在创建文件时添加FILE_FLAG_OVERLAPPED标记,否则用NULL即可



   HANDLE hFile;

   hFile=CreateFile("5.txt",GENERIC_WRITE,0,NULL,CREATE_NEW,FILE_ATTRIBUTE_NORMAL,NULL); //注意第二个参数,创建方式为读取

   DWORD dwWrites;

   WriteFile(hFile,"[http://www.sunxin.org",strlen("http://www.sunxin.org"),&dwWrites,NULL]());

   CloseHandle(hFile);  //关闭该句柄

   读取文件

   BOOL ReadFile(

     HANDLE hFile,                 //文件句柄

     LPVOID lpBuffer,              //用来接收从文件中读取的数据

     DWORD nNumberOfBytesToRead,   //读取的字节数

     LPDWORD lpNumberOfBytesRead,  //接收实际读取的字节数的变量,一般用取址方式作参数

     LPOVERLAPPED lpOverlapped     //

   );


   HANDLE hFile;

   hFile=CreateFile("5.txt",GENERIC_READ,0,NULL,OPEN_EXISTING,  //注意第二个参数,创建方式为读取

  FILE_ATTRIBUTE_NORMAL,NULL);

   char ch[100];

   DWORD dwReads;

   ReadFile(hFile,ch,100,&dwReads,NULL);

   ch[dwReads]=0;        //文件数据末尾

   CloseHandle(hFile);

   MessageBox(ch);

#### mfc 类进行文件操作


   CFile : CObject   提供了没有缓存的，二进制输入输出的磁盘服务，通过派生类间接的支持文本文件和内存文件


   CFile::CFile

   CFile(int hFile);

   CFile(LPCTSTR lpszFileName,UINT nOpenFlags);  //文件名，共享和访问的方式

   读取文件

   CFile file("6.txt",CFile::modeCreate | CFile::modeWrite);

   file.Write("[http://www.sunxin.org",strlen("http://www.sunxin.org]()"));

   file.Close();

   写入文件

   CFile file("6.txt",CFile::modeRead);

   char *pBuf;

   DWORD dwFileLen;

   dwFileLen=file.GetLength();    //获取文件长度

   pBuf=new char[dwFileLen+1];    //根据文件长度申请内存空间

   pBuf[dwFileLen]=0;             //末尾置零

   file.Read(pBuf,dwFileLen);     //读取文件到内存

   file.Close();                  //关闭文件

   MessageBox(pBuf);

11、增加打开文件对话框和保存文件对话框，这都可以用一个类来完成

   CFileDialog : CCommonDialog : CDialog : CWnd : CCmdTarget : CObject


   CFileDialog(BOOL bOpenFileDialog,LPCTSTR lpszDefExt=NULL,LPCTSTR lpszFileName=NULL,DWORD dwFlags=OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,LPCTSTR lpszFilter=NULL,CWnd*pParentWnd=NULL);

   对话框类型  true 打开    false 保存为

   默认扩展名  

   初始文件名

   标志

   文件过滤器 

   父窗口指针

   以上数据也可以在创建对象后,通过CFileDialog的一个成员变量m_ofn来改变,m_ofn实际上是一个OPENFILENAME类型,通过改变其中的lpstrTitle、lpstrFilter、lpstrDefExt等变量，可以改变标题栏，文件类型过滤器、缺省扩展名等等



   获取用户选择的文件名

   CFileDialog::GetPathName     //获取的字串包含了文件的路径和文件名

   CString GetPathName()const;


   CFileDialog::GetFileName     //获取的字串只有文件名

   CString GetFileName()const;

   读取文件

    CFileDialog fileDlg(FALSE);

 fileDlg.m_ofn.lpstrTitle="我的文件保存对话框";

 fileDlg.m_ofn.lpstrFilter="Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";  //字串末必须以两个空结尾

 fileDlg.m_ofn.lpstrDefExt="txt";        //加\0才有文件过滤功能

 if(IDOK==fileDlg.DoModal())

 {

  CFile file(fileDlg.GetFileName(),CFile::modeCreate | CFile::modeWrite);

  file.Write("[http://www.sunxin.org",strlen("http://www.sunxin.org]()"));

  file.Close();

 }

   打开文件

    CFileDialog fileDlg(TRUE);

 fileDlg.m_ofn.lpstrTitle="我的文件打开对话框";

 fileDlg.m_ofn.lpstrFilter="Text Files(*.txt)\0*.txt\0All Files(*.*)\0*.*\0\0";


 if(IDOK==fileDlg.DoModal())

 {

  CFile file(fileDlg.GetFileName(),CFile::modeRead);

  char *pBuf;

  DWORD dwFileLen;

  dwFileLen=file.GetLength();

  pBuf=new char[dwFileLen+1];

  pBuf[dwFileLen]=0;

  file.Read(pBuf,dwFileLen);

  file.Close();

  MessageBox(pBuf);

 }



#### win.ini文件访问

      修改win.ini文件的函数，这个函数是为了兼容16位系统而保留的，在32位系统中用注册表来保存信息

    BOOL WriteProfileString(

      LPCTSTR lpAppName,     //段名

      LPCTSTR lpKeyName,     //键名

      LPCTSTR lpString       //值

    );

     读取win.ini文件的函数

    DWORD GetProfileString(

      LPCTSTR lpAppName,          //段名

      LPCTSTR lpKeyName,          //键名

      LPCTSTR lpDefault,          //缺省的字符串，如果键名在文件中找不到，该值被铐到下面一个参数中

      LPTSTR  lpReturnedString,   //注意此处，需要的是一个内存指针，如果用CString接收，则可以用GetBuffer()获取LPTSTR

      DWORD nSize                 //内存大小

    );

    CString::GetBuffer

    LPTSTR GetBuffer(int nMinBufLength);

    可以通过该函数获取的指针修改内存中字符串的内容，但必须用ReleaseBuffer()释放指针，如果不修改，则不用释放

     //::WriteProfileString("[http://www.sunxin.org","admin","zhangsan](/)");

/* CString str;

 ::GetProfileString("[http://www.sunxin.org","admin","lisi](/)",

  str.GetBuffer(100),100);

 AfxMessageBox(str);*/



    在CWinApp当中也有WriteProfileString(）这个函数

    CWinApp::WriteProfileString

    BOOL WriteProfileString(LPCTSTR lpszSection,LPCTSTR lpszEntry,LPCTSTR lpszValue);  //段名,键名,值

    在winnt系统中,该函数修改的不是win.int文件,而是注册表

    HKEY_CURRENT_USER\Software\Local AppWizard-Generated Applications\File\ 中

    为什么是在Local AppWizard-Generated Applications中呢,因为在InitInstance()中有一个SetRegistryKey()设定了

      SetRegistryKey(_T("Local AppWizard-Generated Applications"));

    读取注册表中相应的信息

    CWinApp::GetProfileString

    CString GetProfileString(LPCTSTR lpszSection,LPCTSTR lpszEntry,LPCTSTR lpszDefault=NULL); //段名,键名,缺省，返回相应值


// WriteProfileString("[http://www.sunxin.org","admin","zhangsan](/)");

 /*CString str;

 str=GetProfileString("[http://www.sunxin.org","admin](/)");

 AfxMessageBox(str);*/

12、注册表操作


    打开注册表项

    LONG RegCreateKey(HKEY hKey, LPCTSTR lpSubKey, PHKEY phkResult);  //主键，子健，用于接收所创建或打开的表项句柄的变量

    如果已有表项则打开，没有则创建

    写入键值，字符串类型的

    LONG RegSetValue((HKEY hKey, LPCTSTR lpSubKey,DWORD dwType,LPCTSTR lpData,DWORD cbData);//2 NULL，设置缺省或没有的数据

                                                      主键         子健             类型         值             值大小，字节为单位

        HKEY hKey;

 RegCreateKey(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",&hKey); //admin是小目录

 RegSetValue(hKey,NULL,REG_SZ,"zhangsan",strlen("zhangsan"));  //zhangsan写入到了名称为“默认”的值名中

 RegCloseKey(hKey);//关闭句柄

    读取数据，字符串类型的

    LONG RegQueryValue(HKEY hKey, LPCTSTR lpSubKey,LPTSTR lpValue,PLONG lpcbValue);

    LONG lValue;

    RegQueryValue(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",NULL,&lValue); //第一次，得到值的大小

    char *pBuf=new char[lValue];   //lValue长度包含了空结束符长度，所以此处不必加一了

    RegQueryValue(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",pBuf,&lValue); //第二次，将值写入内存pBuf

    MessageBox(pBuf);



    写入数据，数字类型

    LONG RegSetValueEx(

       HKEY hKey,

       LPCTSTR lpValueName,  //

       DWORD Reserved,       //保留参数，设为 0 即可

       DWORD dwType,         //类型

       CONST BYTE *lpData,

       DWORD cbData          //数据大小

    );

 HKEY hKey;

 DWORD dwAge=30;

 RegCreateKey(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",&hKey);

 RegSetValue(hKey,NULL,REG_SZ,"zhangsan",strlen("zhangsan"));

 RegSetValueEx(hKey,"age",0,REG_DWORD,(CONST BYTE*)&dwAge,4);

 RegCloseKey(hKey);

    读取数据，数字类型

    LONG RegOpenKey(HKEY hKey, LPCTSTR lpSubKey,PHKEY phkResult);

    LONG RegQueryValueEx(

       HKEY hKey,

       LPCTSTR lpValueName,

       LPDWORD lpReserved,

       LPDWORD lpType,       //接受数据类型的变量，要自己定义一个变量

       LPBYTE lpdata,

       LPDWORD lpcbdata

    );

     HKEY hKey;

 RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\http://www.sunxin.org\\admin",&hKey);

 DWORD dwType;

 DWORD dwValue;

 DWORD dwAge;

 RegQueryValueEx(hKey,"age",0,&dwType,(LPBYTE)&dwAge,&dwValue);

 CString str;

 str.Format("age=%d",dwAge);

 MessageBox(str);

   删除表项

   LONG RegDeleteKey(HKEY hKey, LPCTSTR lpSubKey); 



