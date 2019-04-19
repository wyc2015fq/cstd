# 虚拟内存、CreateFileMapping使用及在WINCE失败的原因 - xqhrs232的专栏 - CSDN博客
2011年03月21日 17:02:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1334
原文地址::[http://fpcfjf.blog.163.com/blog/static/554697932009112411205181/](http://fpcfjf.blog.163.com/blog/static/554697932009112411205181/)
CreateFileMapping在WINCE下总是失败的原因
**声明：代码基本来自网上，虽然自己有过改动和增减，但整体的框架仍然是网上所得，谢谢网上共享的朋友**。
这周一直在测试在CE上使用虚拟内存和内存映射，基本上证明在CE上如网上据说，分配超过30M的大虚拟内存，无法提交到物理内存。
 //1 .虚拟内存的使用测试
 g_Address1 = VirtualAlloc(0, 32 * 1024 * 1024, MEM_RESERVE, PAGE_NOACCESS);
 g_Address2 = VirtualAlloc(g_Address1, 32 * 1024 * 1024, MEM_COMMIT, PAGE_READWRITE);
 UINT nNewSize = (UINT)(2048*2048*200); // 1024.0) * 1024;
 double * pNewBuffer =(double*)  VirtualAlloc(NULL,nNewSize,MEM_COMMIT,PAGE_READWRITE);
 if (pNewBuffer)
 {
  //测试虚拟内存。
  double Block;
  for(int i =0;i<100;i++)
  {
   Block=10.11*i;
   memcpy(&pNewBuffer[i],&Block,sizeof(Block));
  }
  double tt;
  tt=(pNewBuffer[99]);
  CString s;
  s.Format(_T("The total is %f"),tt);
  AfxMessageBox(s) ; 
  //删除分配的内存。
  VirtualFree(pNewBuffer,0,MEM_RELEASE);
 }
这个没有遇到什么问题，紧接着测试内存映射文件，当第一个参数HFILE为INVALID_HANDLE_VALUE时，发现当程序的分配大小超过256M时，返回错误，当然我没有测量256-512M之间的数，只测了这两个，代码如下：
 HANDLE  hFile1;
 hFile1 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, MAXLEN, NULL);
 if(hFile1 == NULL)
 {
  //创建文件映射对象失败
  return;
 }
 LPVOID  lpAddress;
 lpAddress = MapViewOfFile(hFile1, FILE_MAP_WRITE|FILE_MAP_READ, 0, 0, MAXLEN);
 if(lpAddress == NULL)
 {
  //创建文件视图失败
  return;
 }
 UnmapViewOfFile(lpAddress); 
 CloseHandle(hFile1);
记得在内存里建立映射，就不必CREATEFILE了。
问题出现在下面的读取文件时在内存映射里，我的程序在CreateFileMapping死活返回0，而同样的代码移植到PC机上，结果非常正确，数据都同步显示了出来。一直在网上找到一个朋友说，**在CE里必须CreateFileForMapping,否则不成功，**于是改用这个，果然好了。
**注意：注释的部分是测试过程，为了大家理解没有去掉，其在PC上是可用的。要认真看MSDN的使用。比如共享写在CE上是被忽略的。**
 //2共享文件内存映射
 CString pathName = _T("//Program Files//data.txt");
 //HANDLE hFile = CreateFile(pathName,GENERIC_READ,FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
 //HANDLE hFile = CreateFile(pathName,GENERIC_READ,0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
 //HANDLE hFile = CreateFile(pathName,GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    HANDLE hFile = CreateFileForMapping(pathName,**GENERIC_READ**,NULL,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
 char cBuf[512];
 DWORD len;
 ReadFile(hFile,cBuf,512,&len,NULL);
 if (hFile == INVALID_HANDLE_VALUE)
 {
  DWORD it = GetLastError() ;
  TRACE(_T("创建文件对象失败,错误代码:%d/r/n"), GetLastError());
  return;
 }
 // 创建文件映射对象
 HANDLE hFileMap = CreateFileMapping(hFile, NULL, **PAGE_READONLY**, 0, 0, NULL);
 //HANDLE hFileMap = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
 if (hFileMap == NULL)
 {
  DWORD it = GetLastError() ;//ERROR_ALREADY_EXISTS
  TRACE(_T("创建文件映射对象失败,错误代码:%d/r/n"), GetLastError());  
  return;
 }
中间有一个小插曲，The file must be opened with an access mode compatible with the protection flags specified by the flProtect parameter. 这是MSDN上说的，也就是说你打开文件和创建内存文件映射的访问模式必须相同，否则也不会成功，我就遇到了。（上面红色部分）可是后来在MSDN又找到一句，
Windows Mobile Remarks
This function is deprecated for Windows Mobile-based devices. To create or open a file that can be used for memory mapping,
call CreateFile.
意思是说不建议在WM上使用这个函数，最好还是使用CreateFile.至于CE与MOBILE有什么区别，大家可以去网上找。
另外，内存映射的访问方式也略有不同，可以使用已创建的内存映射的名字来访问：
HANDLE hFile = NULL, hFileMap = NULL;
PBYTE pFileData;
       hFile = CreateFileForMapping(m_fileName, GENERIC_READ, 
              FILE_SHARE_READ, NULL, 
              OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL |
              FILE_FLAG_RANDOM_ACCESS,0);
hFileMap = CreateFileMapping (hFile, NULL, PAGE_READONLY, 0, 0, 0);//文件映射对象的大小等于文件的大小。
pFileData = (PBYTE)MapViewOfFile (hFileMap, FILE_MAP_READ, 0, 0, 0);//全部映射
HANDLE hMemMap=NULL;
PBYTE pMemData;
hMemMap = CreateFileMapping (INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE,
                            0, memSize, 0);//创建基于系统页文件的内存映射对象。
//这也是进程间共享数据的方法。
**pMemData = (PBYTE)MapViewOfFile (hMemMap, FILE_MAP_ALL_ACCESS, 0, 0, 0);**
**先说到这里，有时间接着写。**
