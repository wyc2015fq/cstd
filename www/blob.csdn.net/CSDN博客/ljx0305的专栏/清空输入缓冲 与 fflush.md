# 清空输入缓冲 与 fflush - ljx0305的专栏 - CSDN博客
2008年12月19日 12:51:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：641
**1.**为什么**fflush(stdin)**是错的
首先请看以下程序：
#include ***<***stdio.h***>***
int main( void )
{
int i;
for (;;) {
fputs("Please input an integer: ", stdout);<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
scanf("%d", &i);
printf("%d/n", i);
}
return 0;
}
这个程序首先会提示用户输入一个整数，然后等待用户输入，如果用户输入的是整数，程序会输出刚才输入的整数，并且再次提示用户输入一个整数，然后等待用户输入。但是一旦用户输入的不是整数（如小数或者字母），假设 **scanf **函数最后一次得到的整数是2 ，那么程序会不停地输出“Please input an integer: 2”。这是因为 **scanf("%d", &i); **只能接受整数，如果用户输入了字母，则这个字母会遗留在“**输入缓冲区**”中。因为**缓冲**中有数据，故而 **scanf **函数不会等待用户输入，直接就去缓冲中读取，可是缓冲中的却是字母，这个字母再次被遗留在缓冲中，如此反复，从而导致不停地输出“Please input an integer: 2”。
也许有人会说：“居然这样，那么在 scanf 函数后面加上‘fflush(**stdin**);’，把**输入缓冲**清空掉不就行了？”然而这是错的！**C**和**C++**的**标准**里从来没有定义过 fflush(stdin)。也许有人会说：“可是我用 fflush(stdin) 解决了这个问题，你怎么能说是错的呢？”的确，**某些**编译器（如**VC6**）支持用 fflush(stdin) 来清空输入缓冲，但是并非所有编译器都要支持这个功能（**gcc3.2**不支持），因为标准中根本没有定义 fflush(stdin)。**MSDN **文档里也清楚地写着fflush on input stream is an **extension** to the C standard（fflush 操作输入流是对 C 标准的**扩充**）。当然，如果你毫不在乎程序的**移植性**，用 
fflush(stdin) 也没什么大问题。以下是 **C99 **对 **fflush **函数的定义：
int fflush(**FILE** ***stream**);
如果**stream**指向**输出流**或者**更新流**（update stream），并且这个更新流
最近执行的操作不是输入，那么fflush函数将把任何未被写入的数据写入**stream**指向的文件（如标准输出文件**stdout**）。否则，fflush函数的行为是**不确定**的。
fflush（**NULL**）清空所有输出流和上面提到的更新流。如果发生**写错误**，fflush
函数会给那些流打上错误标记，并且返回EOF，否则返回0。
由此可知，如果 stream 指向输入流（如 stdin），那么 fflush 函数的行为是不确定的。故而使用
**fflush(stdin) **是不正确的，至少是**移植性不好**的。
**2.**清空**输入缓冲区**的方法
 虽然不可以用 fflush(stdin)，但是我们可以自己写代码来清空**输入缓冲区**。只需要在 scanf 函数后面加上几句简单的代码就可以了。
/* **C**版本 */
#include ***<***stdio.h***>***
int main( void )
{
int i, c;
for (;;) {
fputs("Please input an integer: ", stdout);
        if ( scanf("%d", &i) != EOF ) { /* 如果用户输入的不是 EOF */
/* **while**循环会把**输入缓冲**中的残留字符清空 */
/* 读者可以根据需要把它改成**宏**或者**内联函数** */
/* 注：**C99**中也定义了内联函数，gcc3.2支持 */
while ( (c=getchar()) != '/n' && c != EOF ) {
;
} /* end of while */
        }
        printf("%d/n", i);
}
return 0;
}
/* **C++**版本 */
#include ***<***iostream***>***
#include ***<***limits***>***// 为了使用**numeric_limits**
using std::cout;
using std::endl;
using std::cin;
int main( )
{
int value;
for (;;) {
cout << "Enter an integer: ";
cin >> value;
/* 读到**非法字符**后，输入流将处于出错状态，
* 为了继续获取输入，首先要调用**clear**函数
* 来清除输入流的错误标记，然后才能调用
* **ignore**函数来清除**输入缓冲区**中的数据。*/
cin.clear( );
/* **numeric_limits**<**streamsize**>::**max( ) **返回缓冲区的大小。
* **ignore **函数在此将把**输入缓冲区**中的数据清空。
* 这两个函数的具体用法请读者自行查询。 */
cin.ignore( std::numeric_limits***<***std::streamsize***>***::max( ), '/n' );
cout << value << '/n';
}
return 0;
}
**参考资料**：
ISO/IEC 9899:1999 (E) Programming languages—**C****7.19.5.2 The fflush function**
The C Programming Language 2nd Edition By Kernighan & Ritchie
ISO/IEC 14882(1998-9-01)Programming languages —**C++**
**原文地址**[http://blog.chinaunix.net/u/9866/showart_124168.html](http://blog.chinaunix.net/u/9866/showart_124168.html)
