# CreateFileMapping的MSDN翻译和使用心得 - xqhrs232的专栏 - CSDN博客
2011年03月28日 10:22:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1491标签：[null																[file																[attributes																[access																[terminal																[fp](https://so.csdn.net/so/search/s.do?q=fp&t=blog)](https://so.csdn.net/so/search/s.do?q=terminal&t=blog)](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=attributes&t=blog)](https://so.csdn.net/so/search/s.do?q=file&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::
[http://fengqing888.blog.163.com/blog/static/330114162009815105052494/?fromdm&fromSearch&isFromSearchEngine=yes](http://fengqing888.blog.163.com/blog/static/330114162009815105052494/?fromdm&fromSearch&isFromSearchEngine=yes)
测试创建和打开文件映射的时候老是得到"句柄无效"的错误, 仔细看了MSDN以后才发觉是函数认识不透, 这里把相关的解释翻译出来 
HANDLE CreateFileMapping(
  HANDLE hFile,                       //物理文件句柄
  LPSECURITY_ATTRIBUTES lpAttributes, //安全设置
  DWORD flProtect,                    //保护设置
  DWORD dwMaximumSizeHigh,            //高位文件大小
  DWORD dwMaximumSizeLow,             //低位文件大小
  LPCTSTR lpName                      //共享内存名称
);
1) 物理文件句柄
   任何可以获得的物理文件句柄, 如果你需要创建一个物理文件无关的内存映射也无妨, 将它设置成为 0xFFFFFFFF(INVALID_HANDLE_VALUE)就可以了.
   如果需要和物理文件关联, 要确保你的物理文件创建的时候的访问模式和"保护设置"匹配, 比如: 物理文件只读, 内存映射需要读写就会发生错误. 推荐你的物理文件使用独占方式创建.
   如果使用 INVALID_HANDLE_VALUE, 也需要设置需要申请的内存空间的大小, 无论物理文件句柄参数是否有效, 这样 CreateFileMapping 就可以创建一个和物理文件大小无关的内存空间给你, 甚至超过实际文件大小, 如果你的物理文件有效, 而大小参数为0, 则返回给你的是一个和物理文件大小一样的内存空间地址范围.  返回给你的文件映射地址空间是可以通过复制, 集成或者命名得到, 初始内容为0.
2) 保护设置
   就是安全设置, 不过一般设置NULL就可以了, 使用默认的安全配置. 在win2k下如果需要进行限制, 这是针对那些将内存文件映射共享给整个网络上面的应用进程使用是, 可以考虑进行限制.
3) 高位文件大小
   弟兄们, 我想目前我们的机器都是32位的东东, 不可能得到超过32位进程所能寻址的私有32位地址空间, 一般还是设置0吧, 我没有也不想尝试将它设置超过0的情况.
4) 低位文件大小
   这个还是可以进行设置的, 不过为了让其他共享用户知道你申请的文件映射的相关信息, 我使用的时候是在获得的地址空间头部添加一个结构化描述信息, 记录内存映射的大小, 名称等, 这样实际申请的空间就比输入的增加了一个头信息结构大小了, 我认为这样类似BSTR的方式应该是比较合理的.
5) 共享内存名称
   这个就是我今天测试的时候碰壁的祸根, 因为为了对于内存进行互斥访问, 我设置了一个互斥句柄, 而名称我选择和命名共享内存同名, 之下就是因为他们使用共同的namespace导致了错误, 呵呵.
7) 调用CreateFileMapping的时候GetLastError的对应错误
   ERROR_FILE_INVALID     如果企图创建一个零长度的文件映射, 应有此报
   ERROR_INVALID_HANDLE   如果发现你的命名内存空间和现有的内存映射, 互斥量, 信号量, 临界区同名就麻烦了
   ERROR_ALREADY_EXISTS   表示内存空间命名已经存在
8) 相关服务或者平台的命名保留
   Terminal Services: 
   命名可以包含 "Global" 或者 "Local" 前缀在全局或者会话名空间初级文件映射. 其他部分可以包含任何除了()以外的字符, 可以参考 Kernel Object Name Spaces.
   Windows 2000 or later: 
   如果 Terminal Services 没有运行 "Global" 和 "Local" 前缀的特殊含义就被忽略了
如何用CreateFileMapping实现进程之间共享数据
HANDLE hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE,NULL,PAGE_READWRITE,0,4*1024,"XQ");
if (hMapFile == NULL) 
{ 
    MessageBox(NULL,"程序异常！","信息",MB_OK);
}
LPVOID lpMapAddress = MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,0);
if (lpMapAddress == NULL)
{
    MessageBox(NULL,"程序异常！","信息",MB_OK); 
}
else
{
   char szPath[MAX_PATH];
   GetCurrentDirectory(MAX_PATH,szPath);
   (char *)lpMapAddress = szPath;
   UnmapViewOfFile(lpMapAddress);
}
HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS,false,"XQ");
if (hMapFile == NULL)
{
MessageBox(NULL,"程序异常","信息",MB_OK);
}
LPVOID lpMapAddress = MapViewOfFile(hMapFile,FILE_MAP_ALL_ACCESS,0,0,0);
if (lpMapAddress == NULL)
{
MessageBox(NULL,"程序异常","信息",MB_OK);
}
else
{
Form2->Caption = AnsiString((char *)lpMapAddress); //这边得到的不是上面的值
UnmapViewOfFile(lpMapAddress);
}
内存映射API函数CreateFileMapping创建一个有名的共享内存：
HANDLE CreateFileMapping(
HANDLE hFile,                                                                        // 映射文件的句柄，
                                                                                                   //设为0xFFFFFFFF以创建一个进程间共享的对象
LPSECURITY_ATTRIBUTES lpFileMappingAttributes,   // 安全属性
DWORD flProtect,                                                                   // 保护方式
DWORD dwMaximumSizeHigh,                                           //对象的大小 
DWORD dwMaximumSizeLow, 
LPCTSTR lpName                                                                 // 必须为映射文件命名
);
与虚拟内存类似，保护方式可以是PAGE_READONLY或是PAGE_READWRITE。如果多进程都对同一共享内存进行写访问，则必须保持相互间同步。映射文件还可以指定PAGE_WRITECOPY标志，可以保证其原始数据不会遭到破坏，同时允许其他进程在必要时自由的操作数据的拷贝。
在创建文件映射对象后使用可以调用MapViewOfFile函数映射到本进程的地址空间内。
下面说明创建一个名为MySharedMem的长度为4096字节的有名映射文件：
HANDLE hMySharedMapFile=CreateFileMapping((HANDLE)0xFFFFFFFF)，
NULL，PAGE_READWRITE，0，0x1000，"MySharedMem")；
并映射缓存区视图：
LPSTR pszMySharedMapView=(LPSTR)MapViewOfFile(hMySharedMapFile，
FILE_MAP_READ|FILE_MAP_WRITE，0，0，0);
其他进程访问共享对象，需要获得对象名并调用OpenFileMapping函数。
HANDLE hMySharedMapFile=OpenFileMapping(FILE_MAP_WRITE，
FALSE，"MySharedMem");
一旦其他进程获得映射对象的句柄，可以象创建进程那样调用MapViewOfFile函数来映射对象视图。用户可以使用该对象视图来进行数据读写操作，以达到数据通讯的目的。
当用户进程结束使用共享内存后，调用UnmapViewOfFile函数以取消其地址空间内的视图：
if (!UnmapViewOfFile(pszMySharedMapView))
{ 
         AfxMessageBox("could not unmap view of file");
 }
void CreateFileMappingEx() 
{ 
DWORD timebegin = ::timeGetTime(); 
HANDLE fp = CreateFile(TEXT("E://jyzhj2.rar"),//这里输入需要复制的文件 src 
GENERIC_READ | GENERIC_WRITE, 
FILE_SHARE_READ, 
NULL, 
OPEN_EXISTING, 
FILE_FLAG_SEQUENTIAL_SCAN, 
NULL); 
if(fp == NULL) 
{ 
cout<<"错误"<<endl; 
return; 
} 
DWORD dwBytesInBlock = GetFileSize(fp,NULL); //文件长度
// 创建文件映射内核对象，句柄保存于hFileMapping 
HANDLE hFileMapping = CreateFileMapping(fp, 
NULL, 
PAGE_READWRITE, 
0,//(DWORD)(dwBytesInBlock >> 16), 
dwBytesInBlock,//(DWORD)(dwBytesInBlock & 0x0000FFFF), 
NULL); 
int dwError = GetLastError(); 
// 释放文件内核对象 
CloseHandle(fp); 
// 偏移地址 
__int64 qwFileOffset = 0; 
// 将文件数据映射到进程的地址空间 
LPVOID pbFile = (LPVOID)MapViewOfFile( hFileMapping, 
FILE_MAP_ALL_ACCESS, 
(DWORD)(qwFileOffset >> 32), 
(DWORD)(qwFileOffset & 0xFFFFFFFF), 
dwBytesInBlock); 
HANDLE wp = CreateFile( TEXT("E://仙剑5.rar"),//这里输入 需要粘贴的文件 dst
GENERIC_READ | GENERIC_WRITE, 
FILE_SHARE_WRITE, 
NULL, 
CREATE_ALWAYS, 
FILE_FLAG_SEQUENTIAL_SCAN | FILE_FLAG_WRITE_THROUGH, 
NULL); 
HANDLE hFileMapping2 = CreateFileMapping( wp, 
NULL, 
PAGE_READWRITE, 
0,//(DWORD)(dwBytesInBlock >> 16), 
dwBytesInBlock,//(DWORD)(dwBytesInBlock & 0x0000FFFF), 
NULL); 
CloseHandle(wp); 
LPVOID pbFile2 = (LPVOID)MapViewOfFile( hFileMapping2,
FILE_MAP_ALL_ACCESS, 
(DWORD)(qwFileOffset >> 32), 
(DWORD)(qwFileOffset & 0xFFFFFFFF), 
dwBytesInBlock); 
memcpy(pbFile2,pbFile,dwBytesInBlock); 
UnmapViewOfFile(pbFile2); 
UnmapViewOfFile(pbFile); 
CloseHandle(hFileMapping2); 
CloseHandle(hFileMapping); 
DWORD timeend = ::timeGetTime(); 
cout<<"CreateFileMapping和MapViewOfFile程序运行时间为"<<timeend - timebegin<<endl; 
}

