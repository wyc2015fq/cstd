# WINDOWS核心编程笔记(1-2) - 深之JohnChen的专栏 - CSDN博客

2005年12月10日 12:24:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：3229


介绍
本书是讲解Windows 操作系统内部机制的专著，作者从基本概念入手，全面系统地介绍了Windows的各种基本构件，如进程、线程、DLL和内存管理等，并列举了大量应用程序，精辟地分析了构件的使用方法，为掌握Windows 编程技巧提供了一条有效的捷径。对于不同水平的Windows 编程人员来说，本书都具有极好的参考价值。

第1章 对程序错误的处理

    在开始介绍Microsoft windows的特性之前。必须先了解windows的各个函数是如何进行销误处理的；
    当调用一个W1ndows函数时，它首先要检验传递给它的的各个参数的有效性。然后再设法执行仔务。如果传递了一个无效参数，或者由于某种原因无法执行这项操作，那么操作系统就会返回一个值．指明该函数在某种程度上运行失败了。下表列出了大多数windows的函数使用的返回值的数据类型。

     数据类型                                  表示失败的值
      VOID           该函数的运行不可能失败。windows函数的返回值类型很少是VOID
      BOOL          如果函数运行失败，那么返回值是0，否则返回的是非0值。
      HANDLE       如果函数运行失败，则返回值是NULL，否则返回值为HANDLE，用于标识你可以操作的一个对象。注意，有些函数会返回一个句柄值INVALID_HANDLE_VALUE，它被定义为-1.
      PVOID          如果函数运行失败，则返回值是NULL，否则返回值为PVOID，以标识数据块的内存地址。
      LONG/DWORD  这是个难以处理的值。返回数量的函数通常返回LONG或DWORD，如果由于某种原因，函数无法对想要进行计数的对象进行计数，那么该函数通常返回0或-1，如果调用的函数返回了LONG/DWORD，那么请认真阅读SDK文档，以确保能正确检查潜在的错误。

    一个WIndows函数返回的错误代码对了解该函数为什么会运行失败常常很有用。微软公司编译了一个所有可能的错误代码的列表．并且为每个错误代码分配了一个32位的号码。
    从系统内部来讲，当一个windows函数检测到一个错误时．它会使用一个称为线程本地存储器的机制，将相应的错误代码号码与调用的线程关联起来。这将使线程能够互相独立地运行．而不会影响各自的错误代码。当函数返回时．它的返回值就能指明—个错误已经发生。若要确定这是个什么错误。请调用GetLastError函数；
    GetLastError能返回线程产生的最后一个错误。如果该线程调用的windows函数运行成功，那么最后一个错误代码就不会被改写，并且不指明运行成功。有少数windows函数并不遵循这一规则，它会更改最后的错误代码;但是platform SDK文档通常指明，当函数运行成功时，该函数会更改最后的错误代码。
    Windows 98 许多Windows 98的函数实际上是用Microsoft公司的16位Windows 3.1产品产生的1 6位代码来实现的。这种比较老的代码并不通过GetLastError之类的函数来报告错误，而且Microsoft公司并没有在Windows 98中修改1 6位代码，以支持这种错误处理方式。对于我们来说，这意味着Windows 98中的许多Wi n 3 2函数在运行失败时不能设置最后的错误代码。该函数将返回一个值，指明运行失败，这样你就能够发现该函数确实已经运行失败，但是你无法确定运行失败的原因。
    在进行调试的时候，监控线程的最后错误代码是非常有用的。在VC6.0中微软的调试程序支持一个非常有用的特性，即可以配置Watch窗口，以便始终都能显示线程的最后错误代码的号码和该错误的英文描述。通过选定watch窗口中的一行，并键入"@err,hr",就能够做到这一点。另外，Visual studio还配有一个小的实用程序，称为Error Lookup.可以用它来将错误代码的号码转换成相应文本描述。
    最后要说的是你也可以自己定义自己的错误代码。若要指明函数运行失败，只需要设定线程的最后错误代码，然后让你的函数返回FALSE、INVALID_HANDLE_VALUE、NULL或者返回任何合适的信息。若要设定线程的最后错误代码，只需要调用WinError.h中已经存在的代码。
第2章 Unicode

    Unicode是开发任何应用程序时要采用的基本步骤。所以放在前面来讲这个问题。
    有些文字和书写规则（比如日文中的汉字就是个典型的例子）的字符集中的符号太多了，因此单字节（它提供的符号最多不能超过2 5 6个）是根本不敷使用的。为此出现了双字节字符集（D B C S），以支持这些文字和书写规则。但是对双字节字符集的操作必须通过windows提供的三个函数CharNext 和Char Prev 、IsDBCSLeadByte来完成。还是有点复杂。为了更使操作更容易，从而产生了Unicode（宽字节字符集）。
U n i c o d e是A p p l e和X e r o x公司于1 9 8 8年建立的一个技术标准。1 9 9 1年，成立了一个集团机构负责U n i c o d e的开发和推广应用。
     U n i c o d e提供了一种简单而又一致的表示字符串的方法。U n i c o d e字符串中的所有字符都是1 6位的（两个字节）。它没有专门的字节来指明下一个字节是属于同一个字符的组成部分，还是一个新字符。这意味着你只需要对指针进行递增或递减，就可以遍历字符串中的各个字符，不再需要调用C h a r N e x t、C h a r P r e v和I s D B C S L e a d B y t e之类的函数。
    由于U n i c o d e用一个1 6位的值来表示每个字符，因此总共可以得到65 000个字符，这样，它就能够对世界各国的书面文字中的所有字符进行编码，远远超过了单字节字符集的2 5 6个字符的数目。
    U n i c o d e具备下列功能：
• 可以很容易地在不同语言之间进行数据交换。
• 使你能够分配支持所有语言的单个二进制. e x e文件或D L L文件。
• 提高应用程序的运行效率
    Windows 2000是使用U n i c o d e从头进行开发的，用于创建窗口、显示文本、进行字符串操作等的所有核心函数都需要U n i c o d e字符串。如果调用任何一个Wi n d o w s函数并给它传递一个A N S I字符串，那么系统首先要将字符串转换成U n i c o d e，然后将U n i c o d e字符串传递给操作系统。如果希望函数返回A N S I字符串，系统就会首先将U n i c o d e字符串转换成A N S I字符串，然后将结果返回给你的应用程序。所有这些转换操作都是在你看不见的情况下发生的。当然，进行这些字符串的转换需要占用系统的时间和内存。所以通过从头开始用U n i c o d e来开发应用程序，就能够使你的应用程序更加有效地运行。
    另外Windows 98只支持A N S I，只能为A N S I开发应用程序。Windows CE只支持U n i c o d e，只能为U n i c o d e开发应用程序。
    这里强调一下COM。 当M i c r o s o f t公司将C O M从1 6位Wi n d o w s转换成Wi n 3 2时，公司作出了一个决定，即需要字符串的所有C O M接口方法都只能接受U n i c o d e字符串。这是个了不起的决定，因为C O M通常用于使不同的组件能够互相进行通信，而U n i c o d e则是传递字符串的最佳手段。
    请注意，所有的U n i c o d e函数均以w c s开头，w c s是宽字符串的英文缩写。若要调用U n i c o d e函数，只需用前缀w c s来取代A N S I字符串函数的前缀s t r即可。
    注意 大多数软件开发人员可能已经不记得这样一个非常重要的问题了，那就是M i c r o s o f t公司提供的C运行期库与A N S I的标准C运行期库是一致的。ANSI C规定，C运行期库支持U n i c o d e字符和字符串。这意味着始终都可以调用C运行期函数，以便对U n i c o d e字符和字符串进行操作，即使是在Windows 98上运行，也可以调用这些函数。换句话说， w c s c a t、w c s l e n和w c s t o k等函数都能够在Windows 98上很好地运行，这些都是必须关心的操作系统函数。
对于包含了对s t r函数或w c s函数进行显式调用的代码来说，无法非常容易地同时为A N S I和
U n i c o d e对这些代码进行编译。本章前面说过，可以创建同时为A N S I和U n i c o d e进行编译的单个源代码文件。若要建立双重功能，必须包含T C h a r. h文件，而不是包含S t r i n g . h文件。
T C h a r. h文件的唯一作用是帮助创建A N S I / U n i c o d e通用源代码文件。它包含你应该用在源代码中的一组宏，而不应该直接调用s t r函数或者w c s函数。如果在编译源代码文件时定义了_ U N I C O D E，这些宏就会引用w c s这组函数。如果没有定义_ U N I C O D E，那么这些宏将引用s t r这组宏。
若要生成一个U n i c o d e字符串而不是A N S I字符串，必须使用如下代码：
例：TCHAR *szERROR L”ERROR”.
字符串前面的大写字母L，用于告诉编译器该字符串应该作为U n i c o d e字符串来编译
Wi n d o w s定义的U n i c o d e数据类型
数据类型         说明
W C H A R     U n i c o d e字符
P W S T R     指向U n i c o d e字符串的指针
P C W S T R   指向一个恒定的U n i c o d e字符串的指针

将你的应用程序转换成符合U n i c o d e的应用程序。下面是应该遵循的一些基本原则：
• 将文本串视为字符数组，而不是c h a r s数组或字节数组。
• 将通用数据类型（如T C H A R和P T S T R）用于文本字符和字符串。
• 将显式数据类型（如B Y T E和P B Y T E）用于字节、字节指针和数据缓存。
• 将T E X T宏用于原义字符和字符串。
• 执行全局性替换（例如用P T S T R替换P S T R）。
• 修改字符串运算问题。例如函数通常希望你在字符中传递一个缓存的大小，而不是字节。这意味着你不应该传递s i z e o f ( s z B u ff e r ) ,而应该传递（ s i z e o f ( s z B u ff e r ) / s i z e o f ( T C H A R )。另外，如果需要为字符串分配一个内存块，并且拥有该字符串中的字符数目，那么请记住要按字节来分配内存。这就是说，应该调用malloc(nCharacters *sizeof(TCHAR)), 而不是调用m a l l o c( n C h a r a c t e r s )。在上面所说的所有原则中，这是最难记住的一条原则，如果操作错误，编译器将不发出任何警告。

★★★

0.C 运行期库对Unicode的支持 

为了利用U n i c o d e 字符串，定义了一些数据类型。标准的C 头文件S t r i n g . h 已经作了修改，以便定义一个名字为w c h a r _ t 的数据类型，它是一个U n i c o d e 字符的数据类型：

typedef unsigned short wchar_t;

例如，如果想要创建一个缓存，用于存放最多为9 9 个字符的U n i c o d e 字符串和一个结尾为零的字符，可以使用下面这个语句：

wchar_t szBuffer[100];

该语句创建了一个由1 0 0 个1 6 位值组成的数组。当然，标准的C 运行期字符串函数，如s t r c p y 、s t r c h r 和s t r c a t 等，只能对A N S I 字符串进行操作，不能正确地处理U n i c o d e 字符串。因此，ANSI C 也拥有一组补充函数。清单2 - 1 显示了一些标准的ANSI C 字符串函数，后面是它们的等价U n i c o d e 函数。

char * strcat(char *,const char *);

wchar_t * wcscat(wchar_t *,const wchar_t *);

清单2-1 标准的ANSI C 字符串函数和它们的等价U n i c o d e 函数 

char * strchr(const char *,int);

wchar_t * wcschr(const wchar_t *,wchar_t);


int strcmp(const char *,const char *);

int wcscmp(const wchar_t *,const wchar_t *);


char * strcpy(char *,const char *);

wchar_t * wcscpy(wchar_t *,const wchar_t *);


size_t strlen(const char *);

size_t wcslen(const wchar_t *);

请注意，所有的U n i c o d e 函数均以w c s 开头，w c s 是宽字符串的英文缩写。若要调用U n i c o d e函数，只需用前缀w c s 来取代A N S I 字符串函数的前缀s t r 即可。

注意大多数软件开发人员可能已经不记得这样一个非常重要的问题了，那就是M i c r o s o f t 公司提供的C 运行期库与A N S I 的标准C 运行期库是一致的。ANSI C 规定，C运行期库支持U n i c o d e 字符和字符串。这意味着始终都可以调用C 运行期函数，以便对U n i c o d e 字符和字符串进行操作，即使是在Windows 98 上运行，也可以调用这些函数。换句话说，w c s c a t 、w c s l e n 和w c s t o k 等函数都能够在Windows 98 上很好地运行，这些都是必须关心的操作系统函数。

对于包含了对s t r 函数或w c s 函数进行显式调用的代码来说，无法非常容易地同时为A N S I 和U n i c o d e 对这些代码进行编译。本章前面说过，可以创建同时为A N S I 和U n i c o d e 进行编译的单个源代码文件。若要建立双重功能，必须包含T C h a r. h 文件，而不是包含S t r i n g . h 文件。

T C h a r. h 文件的唯一作用是帮助创建A N S I / U n i c o d e 通用源代码文件。它包含你应该用在源代码中的一组宏，而不应该直接调用s t r 函数或者w c s 函数。如果在编译源代码文件时定义了U N I C O D E ，这些宏就会引用w c s 这组函数。如果没有定义_ U N I C O D E ，那么这些宏将引用s t r这组宏。

例如，在T C h a r. h 中有一个宏称为_ t c s c p y 。如果在包含该头文件时没有定义_ U N I C O D E ,那么_ t c s c p y 就会扩展为A N S I 的s t r c p y 函数。但是如果定义了_UNICODE, _tcscpy 将扩展为U n i c o d e的w c s c p y 函数。拥有字符串参数的所有C 运行期函数都在T C h a r. h 文件中定义了一个通用宏。如果使用通用宏，而不是A N S I / U n i c o d e 的特定函数名，就能够顺利地创建可以为A N S I 或U n i c o d e进行编译的源代码。

但是，除了使用这些宏之外，还有一些操作是必须进行的。T C h a r. h 文件包含了另外一些宏.若要定义一个A N S I / U n i c o d e 通用的字符串数组，请使用下面的T C H A R 数据类型。如果定义了_ U N I C O D E ，T C H A R 将声明为下面的形式：

typedef wchar_t TCHAR;

如果没有定义_ U N I C O D E ，则T C H A R 将声明为下面的形式：

typedef char TCHAR;

使用该数据类型，可以像下面这样分配一个字符串：

TCHAR szString[100];

也可以创建对字符串的指针：

TCHAR *szError="Error";

不过上面这行代码存在一个问题。按照默认设置，M i c r o s o f t 公司的C + +编译器能够编译所有的字符串，就像它们是A N S I 字符串，而不是U n i c o d e 字符串。因此，如果没有定义_ U N I C O D E ，该编译器将能正确地编译这一行代码。但是，如果定义了_ U N I C O D E ，就会产生一个错误。若要生成一个U n i c o d e 字符串而不是A N S I 字符串，必须将该代码行改写为下面的样子：

TCHAR *szError=L"Error";

字符串（literal string ）前面的大写字母L ，用于告诉编译器该字符串应该作为U n i c o d e 字符串来编译。当编译器将字符串置于程序的数据部分中时，它在每个字符之间分散插入零字节。这种变更带来的问题是，现在只有当定义了_ U N I C O D E 时，程序才能成功地进行编译。我们需要另一个宏，以便有选择地在字符串的前面加上大写字母L 。这项工作由_ T E X T 宏来完成，_ T E X T 宏也在T C h a r. h 文件中做了定义。如果定义了_ U N I C O D E ，那么_ T E X T 定义为下面的形式：

#define _TEXT(x) L ## x

如果没有定义_ U N I C O D E ，_ T E X T 将定义为

#define _TEXT(x) x

使用该宏，可以改写上面这行代码，这样，无论是否定义了_ U N I C O D E 宏，它都能够正确地进行编译。如下所示：

TCHAR *szError=_TEXT("Error");

_ T E X T 宏也可以用于字符串。例如，若要检查一个字符串的第一个字符是否是大写字母J ，只需编写下面的代码即可：

if(szError[0]==_TEXT('J')){

//First character is a 'J'

...

}

else{

//First character is not a 'J'

...

}


1.成为符合ANSI和Unicode的应用程序


即使你不打算立即使用U n i c o d e ，最好也应该着手将你的应用程序转换成符合U n i c o d e 的应用程序。下面是应该遵循的一些基本原则：


• 将文本串视为字符数组，而不是c h a r s 数组或字节数组。

• 将通用数据类型（如T C H A R 和P T S T R ）用于文本字符和字符串。

• 将显式数据类型（如B Y T E 和P B Y T E ）用于字节、字节指针和数据缓存。

• 将T E X T 宏用于原义字符和字符串。

• 执行全局性替换（例如用P T S T R 替换P S T R ）。

• 修改字符串运算问题。例如函数通常希望你在字符中传递一个缓存的大小，而不是字节。


这意味着你不应该传递s i z e o f ( s z B u ff e r ) ,而应该传递（s i z e o f ( s z B u ff e r ) / s i z e o f ( T C H A R )。另外，如果需要为字符串分配一个内存块，并且拥有该字符串中的字符数目，那么请记住要按字节来分配内存。这就是说，应该调用malloc(nCharacters *sizeof(TCHAR)),而不是调用m a l l o c( n C h a r a c t e r s )。在上面所说的所有原则中，这是最难记住的一条原则，如果操作错误，编译器将不发出任何警告。


2. Windows字符串函数

Wi n d o w s 也提供了一组用于对U n i c o d e 字符串进行操作的函数，表2 - 4 对它们进行了描述。

如下是对U n i c o d e 字符串进行操作的函数 

函数描述 

l s t r c a t 将一个字符串置于另一个字符串的结尾处 

l s t r c m p 对两个字符串进行区分大小写的比较 

l s t r c m p i 对两个字符串进行不区分大小写的比较 

l s t r c p y 将一个字符串拷贝到内存中的另一个位置 

l s t r l e n 返回字符串的长度（按字符数来计量） 


这些函数是作为宏来实现的，这些宏既可以调用函数的U n i c o d e 版本，也可以调用函数的A N S I 版本，这要根据编译源代码模块时是否已经定义了U N I C O D E 而定。例如，如果没有定义U N I C O D E ，l s t r c a t 函数将扩展为l s t r c a t A 。如果定义了U N I C O D E ，l s t r c a t 将扩展为l s t r c a t W 。

3. 其他C 运行期函数没有为U n i c o d e 字符串的操作提供很好的支持。例如，t o l o w e r 和t o u p p e r函数无法正确地转换带有重音符号的字符。为了弥补C 运行期库中的这些不足，必须调用下面这些Wi n d o w s 函数，以便转换U n i c o d e 字符串的大小写字母。这些函数也可以正确地用于A N S I字符串。

头两个函数：

PTSTR CharLower(PTSTR pszString);

PTSTR CharUpper(PTSTR pszString);

既可以转换单个字符，也可以转换以0 结尾的整个字符串。若要转换整个字符串，只需要传递字符串的地址即可。若要转换单个字符，必须像下面这样传递各个字符：
将单个字符转换成一个P T S T R ，便可调用该函数，将一个值传递给它，在这个值中，较低的1 6 位包含了该字符，较高的1 6 位包含0 。当该函数看到较高位是0 时，该函数就知道你想要转换单个字符，而不是整个字符串。返回的值是个3 2 位值，较低的1 6 位中是已经转换的字符。

TCHAR cLowerCase=CharLower((PTSTR szString[0]);

将单个字符转换成一个P T S T R ，便可调用该函数，将一个值传递给它，在这个值中，较低的1 6 位包含了该字符，较高的1 6 位包含0 。当该函数看到较高位是0 时，该函数就知道你想要转换单个字符，而不是整个字符串。返回的值是个3 2 位值，较低的1 6 位中是已经转换的字符。

下面两个函数与前面两个函数很相似，差别在于它们用于转换缓存中包含的字符（该缓存不必以0 结尾）：

DWORD CharLowerBuff(

/* pointer to buffer containing characters to process */

PTSTR pszString, 

/* number of bytes or characters to process */

DWORD cchLength

);

DWORD CharUpperBuff(

/* pointer to buffer containing characters to process */

LPTSTR lpsz,

/* number of characters to process */

DWORD cchLength

);

其他的C 运行期函数，如i s a l p h a 、i s l o w e r 和i s u p p e r ，返回一个值，指明某个字符是字母字符、小写字母还是大写字母。Windows API 提供了一些函数，也能返回这些信息，但是Wi n d o w s 函数也要考虑用户在控制面板中指定的语言：

BOOL IsCharAlpha(TCHAR ch);

BOOL IsCharAlphaNumeric(TCHAR ch);

BOOL IsCharLower(TCHAR ch);

BOOL IsCharUpper(TCHAR ch);


4 .在Unicode与ANSI之间转换字符串 

Wi n d o w s 函数M u l t i B y t e To Wi d e C h a r 用于将多字节字符串转换成宽字符串。下面显示了M u l t i B y t e To Wi d e C h a r 函数。

int MultiByteToWideChar(

UINT CodePage,//code page

DWORD dwFlags,//character-type options

LPCSTR lpMultiByteStr,//address of string to map

int cchMultiByte,//number of bytes in string

LPWSTR lpWideCharStr,//address of wide-character buffer

int cchWideChar//size of buffer

);

u C o d e P a g e 参数用于标识一个与多字节字符串相关的代码页号。d w F l a g s 参数用于设定另一个控件，它可以用重音符号之类的区分标记来影响字符。这些标志通常并不使用，在d w F l a g s参数中传递0 。p M u l t i B y t e S t r 参数用于设定要转换的字符串，c c h M u l t i B y t e 参数用于指明该字符串的长度（按字节计算）。如果为c c h M u l t i B y t e 参数传递- 1 ，那么该函数用于确定源字符串的长度。

转换后产生的U n i c o d e 版本字符串将被写入内存中的缓存，其地址由p Wi d e C h a r S t r 参数指定。必须在c c h Wi d e C h a r 参数中设定该缓存的最大值（以字符为计量单位）。如果调用M u l t i B y t e To Wi d e C h a r ，给c c h Wi d e C h a r 参数传递0 ，那么该参数将不执行字符串的转换，而是返回为使转换取得成功所需要的缓存的值。一般来说，可以通过下列步骤将多字节字符串转换成U n i c o d e 等价字符串：

1) 调用M u l t i B y t e To Wi d e C h a r 函数，为p Wi d e C h a r S t r 参数传递N U L L ，为c c h Wi d e C h a r 参数传递0 。
2) 分配足够的内存块，用于存放转换后的U n i c o d e 字符串。该内存块的大小由前面对M u l t B y t e To Wi d e C h a r 的调用返回。
3) 再次调用M u l t i B y t e To Wi d e C h a r ，这次将缓存的地址作为p Wi d e C h a r S t r 参数来传递，并传递第一次调用M u l t i B y t e To Wi d e C h a r 时返回的缓存大小，作为c c h Wi d e c h a r 参数。
4. 使用转换后的字符串。
5) 释放U n i c o d e 字符串占用的内存块。
函数Wi d e C h a r To M u l t i B y t e 将宽字符串转换成等价的多字节字符串，如下所示：

int WideCharToMultiByte(

UINT CodePage,// code page

DWORD dwFlags,// performance and mapping flags

LPCWSTR lpWideCharStr, // address of wide-character string

int cchWideChar,// number of characters in string

LPSTR lpMultiByteStr,// address of buffer for new string

int cchMultiByte,// size of buffer

LPCSTR lpDefaultChar,// address of default for unmappable 

// characters

LPBOOL lpUsedDefaultChar// address of flag set when default 

// char. used

);

该函数与M u l t i B i t e To Wi d e C h a r 函数相似。同样，u C o d e P a g e 参数用于标识与新转换的字符串相关的代码页。d w F l a g s 则设定用于转换的其他控件。这些标志能够作用于带有区分符号的字符和系统不能转换的字符。通常不需要为字符串的转换而拥有这种程度的控制手段，你将为d w F l a g s 参数传递0 。

p Wi d e C h a r S t r 参数用于设定要转换的字符串的内存地址，c c h Wi d e C h a r 参数用于指明该字符串的长度（用字符数来计量）。如果你为c c h Wi d e C h a r 参数传递- 1 ，那么该函数用于确定源字符串的长度。

转换产生的多字节版本的字符串被写入由p M u l t i B y t e S t r 参数指明的缓存。必须在c c h M u l t i B y t e参数中设定该缓存的最大值（用字节来计量）。如果传递0 作为Wi d e C h a r To M u l t i B y t e 函数的c c h M u l t i B y t e 参数，那么该函数将返回目标缓存需要的大小值。通常可以使用将多字节字符串转换成宽字节字符串时介绍的一系列类似的事件，将宽字节字符串转换成多字节字符串。

你会发现，Wi d e C h a r To M u l t i B y t e 函数接受的参数比M u l t i B y t e To Wi d e C h a r 函数要多2 个，即p D e f a u l t C h a r 和p f U s e d D e f a u l t C h a r 。只有当Wi d e C h a r To M u l t i B y t e 函数遇到一个宽字节字符，而该字符在u C o d e P a g e 参数标识的代码页中并没有它的表示法时，Wi d e C h a r To M u l t i B y t e 函数才使用这两个参数。如果宽字节字符不能被转换，该函数便使用p D e f a u l t C h a r 参数指向的字符。如果该参数是N U L L （这是大多数情况下的参数值），那么该函数使用系统的默认字符。该默认字符通常是个问号。这对于文件名来说是危险的，因为问号是个通配符。

p f U s e d D e f a u l t C h a r 参数指向一个布尔变量，如果宽字符串中至少有一个字符不能转换成等价多字节字符，那么函数就将该变量置为T R U E 。如果所有字符均被成功地转换，那么该函数就将该变量置为FA L S E 。当函数返回以便检查宽字节字符串是否被成功地转换后，可以测试该变量。同样，通常为该测试传递N U L L 。

关于如何使用这些函数的详细说明，请参见Platform SDK 文档。

如果使用这两个函数，就可以很容易创建这些函数的U n i c o d e 版本和A N S I 版本。例如，你可能有一个动态链接库，它包含一个函数，能够转换字符串中的所有字符。可以像下面这样编写该函数的U n i c o d e 版本：

BOOL StringReverseW(PWSTR pWideCharStr)

{

//Get a pointer to the last character in the string.

PWSTR pEndOfStr=pWideCharStr+wcslen(pWideCharStr)-1;

wchar_t cCharT;


//Repeat until we reach the center character

//in the string.

while (pWideCharStr < pEndOfStr)

{

//Save a character in a temporary variable.

cCharT=*pWideCharStr;


//Put the last character in the first character.

*pWideCharStr =*pEndOfStr;


//Put the temporary character in the last character.

*pEndOfStr=cCharT;


//Move in one character from the left.

pWideCharStr++;


//Move in one character from the right.

pEndOfStr--;

}

//The string is reversed; return success.

return(TRUE);

}

你可以编写该函数的A N S I 版本以便该函数根本不执行转换字符串的实际操作。你也可以编写该函数的A N S I 版本，以便该函数它将A N S I 字符串转换成U n i c o d e 字符串，将U n i c o d e 字符串传递给S t r i n g R e v e r s e W 函数，然后将转换后的字符串重新转换成A N S I 字符串。该函数类似下面的样子：

BOOL StringReverseA(PSTR pMultiByteStr) 

{

PWSTR pWideCharStr;

int nLenOfWideCharStr;

BOOL fOk = FALSE;


//Calculate the number of characters needed to hold

//the wide_character version of string.

nLenOfWideCharStr = MultiRyteToWideChar(CP_ACP, 0,

pMultiByteStr, -1, NULL, 0);


//Allocate memory from the process's default heap to 

//accommodate the size of the wide-character string.

//Don't forget that MultiByteToWideChar returns the 

//number of characters,not the number of bytes,so

//you must multiply by the size of wide character.

pWideCharStr = HeapAlloc(GetProcessHeap(), 0, 

nLenOfWideCharStr * sizeof(WCHAR));


if (pWideCharStr == NULL)

return(fOk);


//Convert the multibyte string to a wide_character string.

MultiByteToWideChar(CP_ACP, 0, pMulti8yteStr, -1, 

pWideCharStr, nLenOfWideCharStr);


//Call the wide-character version of this 

//function to do the actual work 

fOk = StnngReverseW(pWideCharStr);

if (fOk)

{

//Convert the wide-character string back 

//to a multibyte string.

WideCharToMultiByte(CP_ACP, 0, pWideCharStr, -1, 

pMultiByteStr, strlen(pMultiByteStr), NULL, NULL);

}


//Free the momory containing the wide-character string.

HeapFree(GetProcessHeap(), 0, pWideCharStr);


return(fOk),

}

最后，在用动态链接库分配的头文件中，可以像下面这样建立这两个函数的原型：

BOOL StringReverseW (PWSTR pWideCharStr);

BOOL StringReverseA (PSTR pMultiByteStr);


#ifdef UNICODE

#define StnngReverse StringReverseW

#else

#define StringRevcrsc StringReverseA

#endif // UNICODE





