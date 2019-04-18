# 内存映射文件详解-----C++实现（即一块内存和一个文件相映射对应） - 文章 - 伯乐在线
原文出处： [iross，2015-07-14](http://blog.csdn.net/haiross/article/details/46875211)
先不说内存映射文件（即一块内存和一个文件相映射对应）是什么。贴个代码先，。

```
#include <iostream>
#include <fcntl.h>
#include <io.h>
#include <afxwin.h>
using namespace std;
int main()
{
    //开始
    //获得文件句柄
    HANDLE hFile=CreateFile(
        "c:\\test.dat",   //文件名
        GENERIC_READ|GENERIC_WRITE, //对文件进行读写操作
        FILE_SHARE_READ|FILE_SHARE_WRITE,
        NULL,     
        OPEN_EXISTING,  //打开已存在文件
        FILE_ATTRIBUTE_NORMAL,   
        0);  
    //返回值size_high,size_low分别表示文件大小的高32位/低32位
    DWORD size_low,size_high;
    size_low= GetFileSize(hFile,&size_high); 
    //创建文件的内存映射文件。   
    HANDLE hMapFile=CreateFileMapping(  
        hFile,     
        NULL,   
        PAGE_READWRITE,  //对映射文件进行读写
        size_high,    
        size_low,   //这两个参数共64位，所以支持的最大文件长度为16EB
        NULL);   
    if(hMapFile==INVALID_HANDLE_VALUE)   
    {   
        AfxMessageBox("Can't create file mapping.Error%d:\n",   GetLastError());   
        CloseHandle(hFile);
        return 0;   
    }  
    //把文件数据映射到进程的地址空间
    void* pvFile=MapViewOfFile(
        hMapFile, 
        FILE_MAP_READ|FILE_MAP_WRITE, 
        0,
        0,
        0);  
    unsigned char *p=(unsigned char*)pvFile; 
    //至此，就获得了外部文件test.dat在内存地址空间的映射，
    //下面就可以用指针p"折磨"这个文件了
    CString s;
    p[size_low-1]='!'; 
    p[size_low-2]='X'; //修改该文件的最后两个字节(文件大小<4GB高32位为0)
    s.Format("%s",p);
    //读文件的最后3个字节
    AfxMessageBox(s);
    //结束
    //UnmapViewOfFile(pvFile); //撤销映射
    //CloseHandle(hFile); //关闭文件
    return 0;
}
```
忘小了说，只要你把这几个API函数搞定了，一般的内存映射问题就可以解决了。。但是内存映射文件到底是干嘛的呢？让我们先来思考一个
如果您想读的内容大于系统分配的内存块怎么办？如果您想搜索的字符串刚好超过内存块的边界又该如何处理？对于第一个问题，您也许会说，只要不断地读就不解决了吗。至于第二个问题，您又会说在内存块的边界处做一些特别的处理，譬如放上一些标志位就可以了。原理上确实是行得通，但是这随问题复杂程度加深而显得非常难以处理。其中的第二个问题是有名的边界判断问题，程序中许许多多的错误都是由此引起。想一想，如果我们能够分配一个能够容纳整个文件的大内存块该多好啊，这样这两个问题不都迎刃而解了吗？是的，WIN32的内存映射文件确实允许我们分配一个装得下现实中可能存在的足够大的文件的内存。
利用内存映射文件您可以认为操作系统已经为您把文件全部装入了内存，然后您只要移动文件指针进行读写即可了。这样您甚至不需要调用那些分配、释放内存块和文件输入/输出的API函数，另外您可以把这用作不同的进程之间共享数据的一种办法。运用内存映射文件实际上没有涉及实际的文件操作，它更象为每个进程保留一个看得见的内存空间。至于把内存映射文件当成进程间共享数据的办法来用，则要加倍小心，因为您不得不处理数据的同步问题，否则您的应用程序也许很可能得到过时或错误的数据甚至崩溃。本课中我们将主要讲述内存映射文件，将不涉及进程间的同步。WIN32中的内存映射文件应用非常广泛，譬如：即使是系统的核心模块—PE格式文件装载器也用到了内存映射文件，因为PE格式的文件并不是一次性加载到内存中来的，譬如他它在首次加载时只加载必需加载的部分，而其他部分在用到时再加载，这正好可以利用到内存映射文件的长处。实际中的大多数文件存取都和PE加载器类似，所以您在处理该类问题时也应该充分利用内存映射文件。
内存映射文件本身还是有一些局限性的，譬如一旦您生成了一个内存映射文件，那么您在那个会话期间是不能够改变它的大小的。所以内存映射文件对于只读文件和不会影响其大小的文件操作是非常有用的。当然这并不意味着对于会引起改变其大小的文件操作就一定不能用内存影射文件的方法，您可以事先估计操作后的文件的可能大小，然后生成这么大小一块的内存映射文件，然后文件的长度就可以增长到这么一个大小。
参考：
内存文件映射 c++ 谷歌
以下介绍如何使用CreateFileMapping,MapViewOfFile创建内存映射文件，如何向内存映射文件中写入数据，读取数据。


```
#include <iostream>
#include <assert.h>
 
#include <Windows.h>
#include <WinBase.h>
 
#define BAD_POS 0xFFFFFFFF // returned by SetFilePointer and GetFileSize
#define SUCCESS 0
using namespace std;
 
 
typedef DWORD mmf_share_mode;
typedef DWORD mmf_access_mode;
typedef DWORD mmf_flags;
 
 
int main(){
    cout<<"create memorymapfile..."<<endl;
    const char* shared_name = "testMmf";
    const char* file_name = "d:\\testMmf.mmf";
    const DWORD mmf_size = 512*1024;
    //存取模式
    mmf_access_mode access_mode = (GENERIC_READ|GENERIC_WRITE);
    //共享模式
    mmf_share_mode share_mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
    //文件属性
    mmf_flags flags = FILE_FLAG_SEQUENTIAL_SCAN;//|FILE_FLAG_WRITE_THROUGH|FILE_FLAG_NO_BUFFERING;
    DWORD error_code;
     
    //创建文件
    HANDLE mmHandle =
        CreateFile(file_name,
             access_mode, 
             share_mode, 
             NULL, 
             OPEN_ALWAYS,
             flags,
             NULL);
 
    if (mmHandle == INVALID_HANDLE_VALUE) {
        error_code = GetLastError();
        cout<<"创建mmf失败:"<<error_code<<endl;
    }else{
        DWORD high_size;
        DWORD file_size = GetFileSize(mmHandle, &high_size);
        if (file_size == BAD_POS && (error_code = GetLastError()) != SUCCESS) {
            CloseHandle(mmHandle);            
            cout<<"error："<<error_code<<endl;
        }
        cout<<"create mmf sucessfully"<<endl;
 
        //assert(file_size == 0);
 
        DWORD size_high = 0;
        //创建文件映射，如果要创建内存页面文件的映射，第一个参数设置为INVALID_HANDLE_VALUE
        HANDLE mmfm = CreateFileMapping(mmHandle,
            NULL,
            PAGE_READWRITE,
            size_high, 
            mmf_size, 
            shared_name);
 
        error_code = GetLastError();
        if(SUCCESS != error_code){
            cout<<"createFileMapping error"<<error_code<<endl;
        }else{
            if(mmfm == NULL){
                if(mmHandle != INVALID_HANDLE_VALUE){
                    CloseHandle(mmHandle);
                }
            }else{
                //char write_chars[] = "hello chars";
                //size_t position = 0;
                //DWORD written = 0;
                //const size_t write_chars_size = sizeof(write_chars);
                //WriteFile(mmHandle,write_chars,write_chars_size,&written,NULL);
                size_t view_size = 1024*256;
                DWORD view_access = FILE_MAP_ALL_ACCESS;
 
                //获得映射视图
                char* mmfm_base_address = (char*)MapViewOfFile(mmfm,view_access,0,0,view_size);
                if(mmfm_base_address == NULL){
                    error_code = GetLastError();
                    if(error_code != SUCCESS){
                        cout<<"error code "<<error_code<<endl;
                    }
                }else{
                    char write_chars[] = "hello chars";
                    const size_t write_chars_size = sizeof(write_chars);
                    //向内存映射视图中写数据
                    CopyMemory((PVOID)mmfm_base_address, write_chars, write_chars_size);
 
                    //memcpy(mmfm_base_address,write_chars,write_chars_size);
                     
                    size_t position = 0;
                    char read_chars[write_chars_size];
 
                    //读数据
                    memcpy(read_chars,mmfm_base_address,write_chars_size);
                    cout<<"read chars "<<read_chars<<endl;
                     
                    //卸载映射
                    UnmapViewOfFile(mmfm_base_address);
                    //关闭内存映射文件
                    CloseHandle(mmfm);
                    //关闭文件
                    CloseHandle(mmHandle);
                }
            }
        }
    }
 
     
 
    system("pause");
    exit(0);
    return EXIT_SUCCESS;
}
```
