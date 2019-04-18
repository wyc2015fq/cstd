# GetBuffer 认识 - writeeee的专栏 - CSDN博客
2016年01月23日 10:13:48[writeeee](https://me.csdn.net/writeeee)阅读数：194
个人分类：[c++](https://blog.csdn.net/writeeee/article/category/5634855)
转载：
http://blog.pfan.cn/xman/43212.html
GetBuffer()主要作用是将字符串的缓冲区长度锁定，releaseBuffer则是解除锁定，使得CString对象在以后的代码中继续可以**实现长度自适应增长**的功能。
CString ::GetBuffer有两个重载版本:
LPTSTR GetBuffer( );LPTSTR GetBuffer(int nMinBufferLength);
在第二个版本中，当设定的长度小于原字符串长度时，nMinBufLength = nOldLen，该参数会被忽
略，不分配内存，指向原CString；当设定的长度大于原字符串本身的长度时就要重新分配（reallocate）一块比较大的空间出来。而调用第一个版本时，应如通过传入0来调用第二个版本一样。
是否需要在GetBufer后面调用ReleaseBuffer()，是根据你的后面的程序是否需要继续使用该字符串变量，并且是否动态改变其长度而定的。如果你GetBuffer以后程序自函数就退出，局部变量都不存在了，调用不调用ReleaseBuffer没什么意义了。
这是一个非常容易被用错的函数，主要可能是由于大家对它的功能不太了解。其实点破的话，也不是那么深奥。
    GetBuffer(int size)是用来返回一个你所指定大小可写内存的成员方法。它和被重载的操作符LPCTSTR还是有点本质区别的，LPCTSTR是直接返回一个只读内存的指针，而GetBuffer则是返回一个可以供调用者写入的内存，并且，你可以给定大小。下面是个简单的，但也是非常典型的例子：
    int readFile(CString& str, const CString& strPathName)
    {
        FILE* fp = fopen(strPathName, "r"); // 打开文件
        fseek(fp, 0, SEEK_END);
        int nLen = ftell(fp); // 获得文件长度
        fseek(fp, 0, SEEK_SET); // 重置读指针
        char* psz = str.GetBuffer(nLen);
        fread(psz, sizeof(char), nLen, fp); //读文件内容
        str.ReleaseBuffer(); //千万不能缺少
        fclose(fp);
    }
    上面的函数是GetBuffer函数最典型的用法了，其实它就相当于申请一块nLen大小的内存，只不过，这块内存是被引用在CString对象的内部而已，这是非常有效的一种用法，如果不直接用GetBuffer函数来申请的话，那么你必须用new操作符（或者malloc()函数）在CString的外部申请，然后再将申请的内存拷贝到CString对象中，显然这是一个非常冗余的操作，会使你函数的效率大大下降。
    ReleaseBuffer函数是用来告诉CString对象，你的GetBuffer所引用的内存已经使用完毕，现在必须对它进行封口，否则 CString将不会知道它现在所包含的字符串的长度，所以在使用完GetBuffer之后，必须立即调用ReleaseBuffer函数重置 CString的内部属性，其实也就是头部信息。
补充一下：
GetBuffer说白了就两个功能：
1：就是将CString里面的内存交到外部一个来处理，外部可以直接修改它的内容。
2：重新修改CString的内存大小，这个数值不包含null结尾符。
另一个典型的用法：就是将CString里面的内容变为int或long型，需要先获取里面的内存指针。这样就可以先GetBuffer（内存大小）方便直接转换。
如果在外部修改了CString里面的内容，在重新使用CString之前，需调用ReleaseBuffer()也就是说，ReleaseBuffer不需要每次都调用。
MSDN原文：
If you use the pointer returned by **GetBuffer** to change the string contents, you must call [ReleaseBuffer](http://www.cnblogs.com/jamesmile/admin/ms-help:/ms.vscc.v80/ff0a3476-a7c4-4dc4-89fc-713fcfcdab16.htm) before
 using any other **CSimpleStringT** member methods.
The buffer memory is automatically freed when the **CSimpleStringT** object is destroyed.
If you keep track of the string length yourself, you should not append the terminating null character. You must, however, specify the final string length when you release the buffer with **ReleaseBuffer**. If you do append a terminating null character,
 you should pass –1 (the default) for the length to **ReleaseBuffer**, and **ReleaseBuffer** will perform a **strlen** on the buffer to determine its length.
