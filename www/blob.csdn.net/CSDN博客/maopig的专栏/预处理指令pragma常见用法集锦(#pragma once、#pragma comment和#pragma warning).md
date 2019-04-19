# 预处理指令pragma常见用法集锦(#pragma once、#pragma comment和#pragma warning) - maopig的专栏 - CSDN博客
2011年09月01日 19:42:54[maopig](https://me.csdn.net/maopig)阅读数：1206标签：[linker																[compiler																[编译器																[mfc																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)](https://so.csdn.net/so/search/s.do?q=compiler&t=blog)](https://so.csdn.net/so/search/s.do?q=linker&t=blog)
个人分类：[c/C++](https://blog.csdn.net/maopig/article/category/847675)
**#pragma once：**这是一个比较常用的指令,只要在头文件的最开始加入这条指令就能够保证头文件被编译一次，避免文件被重复包含。
*********************************** 例如 *****************************************
头文件中的 
#if _MSC_VER > 1000 //MFC版本 
#pragma once //编译一次 
#endif // _MSC_VER > 1000 
是什么意思？ 
以上意思就是：如果MFC的版本大于4。2的话，本文件就只编译一次。
************************************************************************************
**#pragma comment：**该指令将一个注释记录放入一个对象文件或可执行文件中。
该指令的格式为： 
        #pragma comment( "comment-type" [, commentstring] ) 
comment-type(注释类型)：可以指定为以下五种预定义的标识符的其中一种。
1、compiler
    将编译器的版本号和名称放入目标文件中,本条注释记录将被编译器忽略。
    如果你为该记录类型提供了commentstring参数,编译器将会产生一个警告。 
    例如:#pragma comment( compiler ) 
2、exestr
   将commentstring参数放入目标文件中,在链接的时候这个字符串将被放入到可执行文件中。
   当操作系统加载可执行文件的时候,该参数字符串不会被加载到内存中.但是,该字符串可以被dumpbin之类的程序查找出并打印出来,你可以用这个标识符将版本号码之类的信息嵌入到可执行文件中!
3、lib
   这是一个非常常用的关键字,用来将一个库文件链接到目标文件中。
   例如: #pragma comment(lib, "user32.lib") 
            该指令用来将user32.lib库文件加入到本工程中。
4、linker
   将一个链接选项放入目标文件中,你可以使用这个指令来代替由命令行传入的或者在开发环境中设置的链接选项。
你可以指定/include选项来强制包含某个对象。
   #pragma comment(linker,"/include:__mySymbol")
你可以在程序中设置下列连接选项：
/DEFAULTLIB
/EXPORT
/INCLUDE
/MANIFESTDEPENDENCY 
/MERGE
/SECTION
**/NODEFAULTLIB**
又如：
#pragma comment(linker,"/nodefaultlib:LIBC.lib")
该与指令是用来忽略特定库的，解决诸如LINK : fatal error LNK1104: 无法打开文件“LIBC.lib”的编译错误。
**#pragma warning**( disable : 4507 34; once : 4385; error : 164 )
等价于：
#pragma warning(disable:4507 34) // 不显示4507和34号警告信息
#pragma warning(once:4385)        // 4385号警告信息仅报告一次
#pragma warning(error:164)          // 把164号警告信息作为一个错误。
