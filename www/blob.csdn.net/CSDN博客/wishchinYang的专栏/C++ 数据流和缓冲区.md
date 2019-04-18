# C++:数据流和缓冲区 - wishchinYang的专栏 - CSDN博客
2013年10月22日 18:56:32[wishchin](https://me.csdn.net/wishchin)阅读数：1001
## （1）：C++之自定义的input缓冲区
   原文链接：[http://hi.baidu.com/nicker2010/item/d0c4cd2a7caf27c4ddf69aeb](http://hi.baidu.com/nicker2010/item/d0c4cd2a7caf27c4ddf69aeb)
input stream用三个指针维护一个read缓冲区，这些指针可以透过**eback(),gptr(),egptr()**获得
**    1.eback()(意思是 end back)是缓冲区的起始位置，或者回退区的尾端，如果不采取特殊措施，      字符最多只能被回退到这个位置    2.gptr()(意思是 get pointer)是当前的读取位置    3.egptr()(意思是 end get pointer)是input buffer的尾端**
    函数**sungetc()**(由input stream的unget()调用)或者
**sputbackc()**(由input  stream的putback()调用)
    可以用来存储stream缓冲区最后一次读取前的状态
    函数**sgetc()**和**sbumpc()**可以读取一个字符。二者的差别在于：后者会让读取指针前进，而前者不会
    如果缓冲区读取完毕(gptr() == egptr()),就不再有可用字符了，此时对缓冲区就必须重新获得补给
    这项工作可以由虚函数 underflow()完成，它会负责读取数据
    如果没有字符，函数sbumpc()会调用虚函数uflow(),而uflow()的缺省行为就是调用underflow()，
    并移动(前进)读取指针。基类basic_streambuf中对underflow()的缺省行为是返回EOF
    函数sgetn()用于一次读取多个字符，这个函数把任务交给xsgetn()，
    后者的缺省做法是多美歌字符调用sbumpc().我们可以改进xsgetn()来优化对多个字符的读取
**    对input缓冲区，要么就必须建立一个缓冲区，要么就至少实现underflow()和uflow()    任何一个具备字符读取功能的stream缓冲区，都必须实现underflow()**
    成员函数setg()可以建立一个read缓冲区，它有三个参数：
    1.一个指针，指向缓冲区头部(eback())
    2.一个指针，指向当前读取位置(gptr())
    3.一个指针，指向缓冲区尾部(egptr())
    下面就实现一个我们自定义的input stream buffer：
```cpp
extern "C"
{
    int read(int fd,char* buf,int num);
}
class InBuffer : public std::streambuf
{
protected:
    static const int bufferSize = 10; ///缓冲区大小
    char buffer[bufferSize];
public:
    /**建立read缓冲区，完全清空*/
    InBuffer()
    {
        setg(buffer+4,buffer+4,buffer+4);
    }
protected:
    virtual int_type underflow()
    {
        if(gptr() < egptr())
            return *gptr();
        int numPutback = gptr()-eback();
        if(numPutback > 4) numPutback=4;
        /**把缓冲区最后几个字符移到头部，便于回退*/
        std::memcpy(buffer+(4-numPutback), gptr()-numPutback, numPutback);
        int num = read(0,buffer+4,bufferSize-4); ///读取数据
        if(num <= 0) return EOF;
        setg(buffer+(4-numPutback),buffer+4,buffer+4+num);
        return *gptr();
    }
};
void testMyInputBuffer()
{
    InBuffer buf;
    std::istream in(&buf);
    char c;
    for(int i=0;i<20;++i)
    {
        in.get(c);
        cout<<c<<flush;
        if(i == 10)
        {
            in.unget();  ///放回2个字符，即第9,10个字符被打印两次
            in.unget();
        }
    }
    cout<<endl;
}
```
（2）：C++对缓冲区的理解
**摘自：[http://www.vckbase.com/document/viewdoc/?id=1897](http://www.vckbase.com/document/viewdoc/?id=1897)**
**什么是缓冲区**
缓冲区又称为缓存，它是内存空间的一部分。也就是说，在内存空间中预留了一定的存储空间，这些存储空间用来缓冲输入或输出的数据，这部分预留的空间就叫做缓冲区。
缓冲区根据其对应的是输入设备还是输出设备，分为输入缓冲区和输出缓冲区。
**为什么要引入缓冲区**
我们为什么要引入缓冲区呢？
比如我们从磁盘里取信息，我们先把读出的数据放在缓冲区，计算机再直接从缓冲区中取数据，等缓冲区的数据取完后再去磁盘中读取，这样就可以减少磁盘的读写次数，再加上计算机对缓冲区的操作大大快于对磁盘的操作，故应用缓冲区可大大提高计算机的运行速度。
又比如，我们使用打印机打印文档，由于打印机的打印速度相对较慢，我们先把文档输出到打印机相应的缓冲区，打印机再自行逐步打印，这时我们的CPU可以处理别的事情。
现在您基本明白了吧，缓冲区就是一块内存区，它用在输入输出设备和CPU之间，用来缓存数据。它使得低速的输入输出设备和高速的CPU能够协调工作，避免低速的输入输出设备占用CPU，解放出CPU，使其能够高效率工作。
**缓冲区的类型**
缓冲区 分为三种类型：全缓冲、行缓冲和不带缓冲。
1、全缓冲
在这种情况下，当填满标准I/O缓存后才进行实际I/O操作。全缓冲的典型代表是对磁盘文件的读写。
2、行缓冲
在这种情况下，当在输入和输出中遇到换行符时，执行真正的I/O操作。这时，我们输入的字符先存放在缓冲区，等按下回车键换行时才进行实际的I/O操作。典型代表是键盘输入数据。
3、不带缓冲
也就是不进行缓冲，标准出错情况stderr是典型代表，这使得出错信息可以直接尽快地显示出来。
缓冲区的刷新
下列情况会引发缓冲区的刷新:
1、缓冲区满时；
2、执行flush语句；
3、执行endl语句；
4、关闭文件。
可见，缓冲区满或关闭文件时都会刷新缓冲区，进行真正的I/O操作。另外，在C++中，我们可以使用flush函数来刷新缓冲区（执行I/O操作并清空缓冲区），如：
cout<<flush; //将显存的内容立即输出到显示器上进行显示
endl控制符的作用是将光标移动到输出设备中下一行开头处，并且清空缓冲区。
cout<<endl;
相当于
cout<<”\n” <<flush;
**通过实例演示说明**
**1、文件操作演示全缓冲**
创建一个控制台工程，输入如下代码：
#include <fstream>using namespace std;int main(){    //创建文件test.txt并打开ofstream outfile("test.txt");    //向test.txt文件中写入4096个字符’a’for(int n=0;n<4096;n++){outfile<<'a';}    //暂停，按任意键继续system("PAUSE");        //继续向test.txt文件中写入字符’b’，也就是说，第4097个字符是’b’outfile<<'b';    //暂停，按任意键继续system("PAUSE");return 0;}
上面这段代码很容易理解，已经在代码内部作了注释。
编写这段小代码的目的是验证WindowsXP下全缓冲的大小是4096个字节，并验证缓冲区满后会刷新缓冲区，执行真正的I/O操作。
编译并执行，运行结果如下：
![](http://www.vckbase.com/document/journal/vckbase55/images/buf002.jpg)
此时打开工程所在文件夹下的test.txt文件，您会发现该文件是空的，这说明4096个字符“a”还在缓冲区，并没有真正执行I/O操作。敲一下回车键，窗口变为如下：
![](http://www.vckbase.com/document/journal/vckbase55/images/buf004.jpg)
此时再打开test.txt文件，您就会发下该文件中已经有了4096个字符“a”。这说明全缓冲区的大小是4K（4096），缓冲区满后执行了I/O操作，而字符“b”还在缓冲区。
再次敲一下回车键，窗口变为如下：
![](http://www.vckbase.com/document/journal/vckbase55/images/buf006.jpg)
此时再打开test.txt文件，您就会发现字符“b”也在其中了。这一步验证了文件关闭时刷新了缓冲区。
**2、键盘操作演示行缓冲**
先介绍getchar()函数。
函数原型：int getchar(void);
说明：当程序调用getchar()函数时，程序就等着用户按键，用户输入的字符被存放在键盘缓冲区中，直到用户按回车为止（回车字符也放在缓冲区中）。当用户键入回车之后，getchar()函数才开始从键盘缓冲区中每次读入一个字符。也就是说，后续的getchar()函数调用不会等待用户按键，而直接读取缓冲区中的字符，直到缓冲区中的字符读完后，才重新等待用户按键。
不知道您明白了没有，再通俗一点讲，当程序调用getchar()函数时，程序就等着用户按键，并等用户按下回车键返回。期间按下的字符存放在缓冲区，第一个字符作为函数返回值。继续调用getchar()函数，将不再等用户按键，而是返回您刚才输入的第2个字符；继续调用，返回第3个字符，直到缓冲区中的字符读完后，才等待用户按键。
如果您还没有明白，只能怨我表达能力有限，您可以结合以下实例体会。
创建一个控制台工程，输入如下代码：
#include <iostream>using namespace std;int main(){char c;//第一次调用getchar()函数//程序执行时，您可以输入一串字符并按下回车键，按下回车键后该函数才返回c=getchar();    //显示getchar()函数的返回值cout<<c<<endl;    //暂停system("PAUSE"); //循环多次调用getchar()函数//将每次调用getchar()函数的返回值显示出来//直到遇到回车符才结束while((c=getchar())!='\n'){printf("%c",c);}    //暂停system("PAUSE");return 0;}
这段小代码也很简单，同样在代码内部都有注释。
getchar()函数的执行就是采用了行缓冲。第一次调用getchar()函数，会让程序使用者（用户）输入一行字符并直至按下回车键 函数才返回。此时用户输入的字符和回车符都存放在行缓冲区。
再次调用getchar()函数，会逐步输出行缓冲区的内容。
好了，本人表达能力有限，还是编译运行程序，通过运行结果自己领会吧。
编译运行程序，会提示您输入字符，您可以交替按下一些字符，如下：
![](http://www.vckbase.com/document/journal/vckbase55/images/buf008.jpg)
您一直按下去，您就会发现当您按到第4094个字符时，不允许您继续输入字符。这说明行缓冲区的大小也是4K。
此时您按下回车键，返回第一个字符’a’，如下图：
![](http://www.vckbase.com/document/journal/vckbase55/images/buf010.jpg)
继续敲一下回车键，将缓冲区的其它的字符全部输出，如下图：
![](http://www.vckbase.com/document/journal/vckbase55/images/buf012.jpg)
**3、标准错误输出不带缓冲**
如错误输出时使用：
cerr<<”错误，请检查输入的参数!”;
这条语句等效于：
fprintf(stderr, ”错误，请检查输入的参数!”);
好了，就说到这吧，祝您好运，希望能对您有所帮助。
（3）：内存日志系统
原文链接：[http://www.cnblogs.com/JefferyZhou/archive/2010/07/06/1771802.html](http://www.cnblogs.com/JefferyZhou/archive/2010/07/06/1771802.html)
在第一篇中主要由讨论日志的需求以及接口设计，这里阐述一下各个部分的实现过程。在设计过程中我们把整个系统分为三个部分：模块buffer, 线程buffer, 和内存日志系统接口。
首先我们需要明确的是我们的写入是正对线程来说，不同线程需要在不同的内存段进行写入操作，而线程内的写入操作是针对模块而来的。所以正在的写入操作在模块buffer里面，这里我们直接实现 一个前面设计的模块buffer：
```cpp
//模块buffer
template<int nMaxBuffLen = MEMLOGSYS_MAX_BUFFE_LEN>
classIModuleBufferImp
{
public:
    typedefunsigned int size_t;
 
    staticconst unsigned int m_snMaxBuffLen = nMaxBuffLen;
 
    voidWriteLog( constchar* szBufferCache)                                  //写日志
    {
        WriteLog_Imp(GetTimeSting().c_str());
        WriteLog_Imp(szBufferCache);
        WriteLog_Imp("\n");
    }
 
    constchar* GetMemLog()                                        //获取当前模块的日志信息
    {
        m_Buffer[m_snMaxBuffLen] = 0;
        returnm_Buffer;
    }
 
    constchar* GetModuleName()const
    {
        returnm_szModuleName;
    }
     
    staticIModuleBufferImp* CreateModuleBuffer(constchar* pszModuleName)
    {
        IModuleBufferImp* pModuleBuffer =new IModuleBufferImp(pszModuleName);
        returnpModuleBuffer;
    }
 
    staticvoid DestroyModuleBuffer(IModuleBufferImp* pModuleBuffer)
    {
        SAFE_DELETE(pModuleBuffer);
    }
 
    staticstd::string GetTimeSting()
    {
        tm* pTimeStruct = Time::GetLOCALTime();
 
        charszTempBuffer[128] = {0};
         
        sprintf_s(szTempBuffer,"%.2d.%.2d.%.2d:%.2d:%.2d:%.10d  ",pTimeStruct->tm_mon,pTimeStruct->tm_mday,pTimeStruct->tm_hour,pTimeStruct->tm_min,pTimeStruct->tm_sec, GetTickCount());
         
        returnstd::string(szTempBuffer);
    }
protected:
 
    explicitIModuleBufferImp(constchar* pszModuleName)
    {
        if(pszModuleName)
        {
            sprintf_s(m_szModuleName, MEMLOGSYS_MAX_MODULENAME_LEN-1,"%s", pszModuleName);
            m_szModuleName[MEMLOGSYS_MAX_MODULENAME_LEN-1] = 0;
        }else
        {
            memset(m_szModuleName, 0, MEMLOGSYS_MAX_MODULENAME_LEN);
            sprintf_s(m_szModuleName, MEMLOGSYS_MAX_MODULENAME_LEN-1,"%s", MEMLOGSYS_DEFAULT_MODULENAME);
        }
 
        m_WriteOffSet = 0; 
        memset(m_Buffer, 0, m_snMaxBuffLen);
    }
 
    ~IModuleBufferImp()
    {
        ;
    }
 
    boolWriteLog_Imp(constchar* pszLog)
    {
        if(pszLog)
        {
            intlen = strlen(pszLog);
 
            if(len >= m_snMaxBuffLen)   //超过最大值了
            {
                returnfalse;
            }  
 
            if(m_WriteOffSet + len > m_snMaxBuffLen)
            {  
                memcpy(m_Buffer+m_WriteOffSet, pszLog, m_snMaxBuffLen - m_WriteOffSet);
                memcpy(m_Buffer,pszLog + (m_snMaxBuffLen - m_WriteOffSet), len-m_snMaxBuffLen + m_WriteOffSet);
            }else
            {
                memcpy(m_Buffer+m_WriteOffSet, pszLog, len);
            }
 
            m_WriteOffSet += len;
 
            if(m_WriteOffSet >= m_snMaxBuffLen)
            {
                m_WriteOffSet -= m_snMaxBuffLen;
            }
 
            returntrue;
        }
        returnfalse;
    }
     
 
private:
    IModuleBufferImp(constIModuleBufferImp& rstIModuleBuffer){;};              //禁用
    IModuleBufferImp& operator = (constIModuleBufferImp& rstIModuleBuffer) {;};//禁用
 
 
    charm_szModuleName[MEMLOGSYS_MAX_MODULENAME_LEN];
    intm_WriteOffSet;
    charm_Buffer[m_snMaxBuffLen + 1];
};
```
```cpp
上面我们提供了一个简单的工厂函数来创建模块buffer， 并置拷贝构造函数和赋值构造函数为私有防止恶意拷贝。
```
在上面的实现过程中，有几个小细节需要注意:
　　1。 我们是需要一个线程安全的写入类，所以在类的实现中不能出现任何多线程共享的可写字段。
　　2。 线程Buffer 是一个循环日志，必须保证最后记录的完整性，以及记录的循环可读性，
            必须在写入记录的时候保证字符串结尾的0不屏蔽，数组中的有效内容。也就是在Write_Imp()里面用的是memcpy, 当然也可以使用 sprintf 系列，
            (读者也可以自行改为使用sprintf系列，如果使用sprintf系列需要仔细阅读相关文档, 比如 sprintf_s 保证最后给你加一个 0， 超过缓冲区会报错获设置错误标志位 : 这里有一个小tips，sprintf_s ,不仅在你的缓冲区里面输入内容，而且在后面加一个0，不仅加一个0，还会对0后面的有效缓冲区拿来做其他用途)
     3。 为了保证不破坏任何有效的完整记录，以及循环可读性，申请的缓冲区必须比可写缓冲区多一个字节用来存字符串结束符.
     4。 把构造函数定义为保护，提供一个简单工厂函数，这里把缓冲区定义为一个数组，而不是一个动态申请的空间，然后，这个时候工厂函数全部是new出的模块buff，所有你定义任何大小的缓冲区，只要不操作当前系统最大可申请内存数量就不会出现错误。
Sign Clown 2010.7.6 0:59 HDPY
大概实现就是上面了。后面另外两个部分，线程buff，系统管理接口的具体实现，会陆续展示。
[本文原创，转载请注明出处，在文章末尾提供原文链接[http://www.cnblogs.com/JefferyZhou/](http://www.cnblogs.com/JefferyZhou/)，否则一旦发现，将按字节每人民币收费，绝不论价]
