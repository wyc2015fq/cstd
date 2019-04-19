# 一步一步走进Linux HOOK API(一) - xqhrs232的专栏 - CSDN博客
2017年01月18日 18:12:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：505
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://blog.csdn.net/yyttiao/article/details/7346287](http://blog.csdn.net/yyttiao/article/details/7346287)
相关文章
1、[一步一步走进Linux HOOK API(二)](http://blog.csdn.net/yyttiao/article/details/7350279)----[http://blog.csdn.net/yyttiao/article/details/7350279](http://blog.csdn.net/yyttiao/article/details/7350279)
2、[一步一步走进Linux HOOK API(三)](http://blog.csdn.net/yyttiao/article/details/7358578)----[http://blog.csdn.net/yyttiao/article/details/7358578](http://blog.csdn.net/yyttiao/article/details/7358578)
3、[一步一步走进Linux HOOK API(四)](http://blog.csdn.net/yyttiao/article/details/7365235)----[http://blog.csdn.net/yyttiao/article/details/7365235](http://blog.csdn.net/yyttiao/article/details/7365235)
4、[一步一步走进Linux HOOK API(五)](http://blog.csdn.net/yyttiao/article/details/7391831)----[http://blog.csdn.net/yyttiao/article/details/7391831](http://blog.csdn.net/yyttiao/article/details/7391831)
5、[一步一步走进Linux HOOK API(六)](http://blog.csdn.net/yyttiao/article/details/7423545)----[http://blog.csdn.net/yyttiao/article/details/7423545](http://blog.csdn.net/yyttiao/article/details/7423545)
6、[一步一步走进Linux HOOK API(七)](http://blog.csdn.net/yyttiao/article/details/7423549)----[http://blog.csdn.net/yyttiao/article/details/7423549](http://blog.csdn.net/yyttiao/article/details/7423549)
7、[一步一步走进Linux HOOK API(八)](http://blog.csdn.net/yyttiao/article/details/7777032)----[http://blog.csdn.net/yyttiao/article/details/7777032](http://blog.csdn.net/yyttiao/article/details/7777032)

最近我查阅很多参考资料.发现对于讲述[Linux](http://lib.csdn.net/base/linux) HOOK API的资料是很少,让我们这些新人难以去走进Linux HOOK大门.在这里我将全面的讲述Linux HOOK API的全部实现过程,这个过程中我也遇到很多坎坷,所以在这么写下这份教程.让大家都来进入HOOK的神秘世界.
不要认为HOOK API是windows的专利(PS.其实我以前就是这么认为的.哈哈....),其实在Linux中也有HOOK API这样的技术,只是实现起来相对比较麻烦,首先今天主要带大家认识的是ELF文件,在Linux中,ELF文件主要是应用在可执行文件,重定位文件,可执行文件动态连接库。首先来看一下ELF Head的定义:
      PS.我们这里主要针对的是32位平台.有关64位平台相关定义请参阅/usr/include/elf.h
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- 
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #define EI_NIDENT (16)
- 
- 
- typedefstruct
- 
- {  
- 
-   unsigned char e_ident[EI_NIDENT]; /* Magic number and other info */
- 
-   Elf32_Half e_type; /* Object file type */
- 
-   Elf32_Half e_machine; /* Architecture */
- 
-   Elf32_Word e_version; /* Object file version */
- 
-   Elf32_Addr e_entry; /* Entry point virtual address */
- 
-   Elf32_Off e_phoff; /* Program header table file offset */
- 
-   Elf32_Off e_shoff; /* Section header table file offset */
- 
-   Elf32_Word e_flags; /* Processor-specific flags */
- 
-   Elf32_Half e_ehsize; /* ELF header size in bytes */
- 
-   Elf32_Half e_phentsize; /* Program header table entry size */
- 
-   Elf32_Half e_phnum; /* Program header table entry count */
- 
-   Elf32_Half e_shentsize; /* Section header table entry size */
- 
-   Elf32_Half e_shnum; /* Section header table entry count */
- 
-   Elf32_Half e_shstrndx; /* Section header string table index */
- 
- } Elf32_Ehdr;  
**e_ident:** 这个成员,是ELF文件的第一个成员,该成员是个数字,根据上面的宏可以看出,这个程序是个16字节的数据.该成员的前4个字节依次是 0x7F,0x45,0x4c,0x46,也
 就是"\177ELF"。这是ELF文件的标志，任何一个ELF文件这四个字节都完全相同。
为了让我们更方便的使用ELF数据在elf.h中对上述数据进行了宏定义.如下:
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #define EI_MAG0 0 /* File identification byte 0 index */
- 
- #define ELFMAG0 0x7f /* Magic number byte 0 *
- 
- #define EI_MAG1 1 /* File identification byte 1 index */
- 
- #define ELFMAG1 'E' /* Magic number byte 1 */
- 
- #define EI_MAG2 2 /* File identification byte 2 index */
- 
- #define ELFMAG2 'L' /* Magic number byte 2 */
- 
- #define EI_MAG3 3 /* File identification byte 3 index */
- 
- #define ELFMAG3 'F' /* Magic number byte 3 */
- 
- /* Conglomeration of the identification bytes, for easy testing as a word.  */
- 
- #define ELFMAG "\177ELF"
- 
- #define SELFMAG 4
第四个字节表示ELF格式,1:32位2:64位
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #define EI_CLASS 4 /* File class byte index */
- 
- #define ELFCLASSNONE 0 /* Invalid class */
- 
- #define ELFCLASS32 1 /* 32-bit objects */
- 
- #define ELFCLASS64 2 /* 64-bit objects */
- 
- #define ELFCLASSNUM 3
第五个字节表示数据编码格式,1:小端模式 2:大端模式
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #define EI_DATA 5 /* Data encoding byte index */
- 
- #define ELFDATANONE 0 /* Invalid data encoding */
- 
- #define ELFDATA2LSB 1 /* 2's complement, little endian */
- 
- #define ELFDATA2MSB 2 /* 2's complement, big endian */
- 
- #define ELFDATANUM 3
第六个字节表示文件版本,该值目前必须为1
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #define EV_CURRENT 1 /* Current version */
第七个字节表示[操作系统](http://lib.csdn.net/base/operatingsystem)标识:
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #define EI_OSABI 7 /* OS ABI identification */
- 
- #define ELFOSABI_NONE 0 /* UNIX System V ABI */
- 
- #define ELFOSABI_SYSV 0 /* Alias.  */
- 
- #define ELFOSABI_HPUX 1 /* HP-UX */
- 
- #define ELFOSABI_NETBSD 2 /* NetBSD.  */
- 
- #define ELFOSABI_LINUX 3 /* Linux.  */
- 
- #define ELFOSABI_SOLARIS 6 /* Sun Solaris.  */
- 
- #define ELFOSABI_AIX 7 /* IBM AIX.  */
- 
- #define ELFOSABI_IRIX 8 /* SGI Irix.  */
- 
- #define ELFOSABI_FREEBSD 9 /* FreeBSD.  */
- 
- #define ELFOSABI_TRU64 10 /* Compaq TRU64 UNIX.  */
- 
- #define ELFOSABI_MODESTO 11 /* Novell Modesto.  */
- 
- #define ELFOSABI_OPENBSD 12 /* OpenBSD.  */
- 
- #define ELFOSABI_ARM 97 /* ARM */
- 
- #define ELFOSABI_STANDALONE 255 
- 
- /*Standalone(embedded) application */
第八个字节表示ABI版本
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #define EI_ABIVERSION 8 /* ABI version */
第九个字节表示e_ident中从哪开始之后未使用.
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #define EI_PAD 9 /* Byte index of padding bytes */
**e_type:**          这个成员是ELF文件的类型:
                         1:表示此文件是重定位文件.
                         2:表示可执行文件.
                         3:表示此文件是一个动态连接库。
**e_machine:**       这个成员表示机器版本.具体定义参与elf.h (篇幅问题,太长了)
**e_version:**          这个成员表示ELF文件版本,为 1
**e_entry:**              这个成员表示可执行文件的入口虚拟地址。此字段指出了该文件中第一条可执 行机器指令在进程被正确加载后的内存地址！ELF可执行文件只能被加载到固定位 置.
**e_phoff:**             这个成员表示程序头(Program Headers)在ELF文件中的偏移量。如果程序头 不存在此值为0。
**e_shoff:**             这个成员表示节头(Section Headers:)在ELF文件中的偏移量。如果节头不存 在此值为0。
**e_flags:**             这个成员表示处理器标志.
**e_ehsize:**          这个成员描述了“ELF头”自身占用的字节数。
**e_phentsize:**    该成员表示程序头中的每一个结构占用的字节数。程序头也叫程序头表，可以 被看做一个在文件中连续存储的结构数组，数组中每一项是一个结构，此字段 给出了这个结构占用的字节大小。
**e_phoff:**指出程序头在ELF文件中的起始偏移。
**e_phnum:**        此字段给出了程序头中保存了多少个结构。如果程序头中有3个结构则程序头 在文件中占用了3×e_phentsize个字节的大小。
**e_shentsize:**   节头中每个结构占用的字节大小。节头与程序头类似也是一个结构数组，关于 这两个结构的定义将分别在讲述程序头和节头的时候给出。
**e_shnum:**        节头中保存了多少个结构。
**e_shstrndx:**     这是一个整数索引值。节头可以看作是一个结构数组，用这个索引值做为此数 组的下标，它在节头中指定的一个结构进一步给出了一个“字符串表”的信息，而这 个字符串表保存着节头中描述的每一个节的名称，包括字符串表自己也是其中的一
 个节。
**示例代码:**
**[cpp]**[view
 plain](http://blog.csdn.net/yyttiao/article/details/7346287#)[copy](http://blog.csdn.net/yyttiao/article/details/7346287#)
- #include <unistd.h>
- #include <elf.h>
- #include <fcntl.h>
- #include <stdio.h>
- #include <string.h>
- #include <stdlib.h>
- #include <sys/mman.h>
- #include <sys/types.h>
- #include <sys/stat.h>
- 
- int     g_File      = 0;  
- void    *g_pData    = NULL;  
- 
- void * Map(char* szFileName)  
- {  
-     g_File = open(szFileName, O_RDWR);    
- if (g_File < 0)     
-     {     
-         g_File = 0;    
- return NULL;     
-     }    
- struct stat status;    
-     fstat(g_File, &status);    
- 
-     g_pData = mmap(0, status.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, g_File, 0);    
- if (MAP_FAILED != g_pData) {  
- return g_pData;  
-     }    
- 
-     close(g_File);    
-     g_pData = NULL;    
-     g_File = 0;    
- return NULL;    
- }  
- 
- void displayEhdr(Elf32_Ehdr *ehdr)  
- {  
-     printf("Magic:");  
- int i = 0;  
- for(i = 0; i < EI_NIDENT;i++){  
-         printf(" %02x",ehdr->e_ident[i]);  
-     }  
-     printf("\n");  
-     printf("Version:            0x%x\n",ehdr->e_version);  
-     printf("Entry point address:        0x%x\n",ehdr->e_entry);  
-     printf("Start of program headers:   %d (bytes into file)\n",ehdr->e_phoff);  
-     printf("Start of section headers:   %d (bytes into file)\n",ehdr->e_shoff);  
-     printf("Flags:              %d\n",ehdr->e_flags);  
-     printf("Size of this header:        %d (bytes)\n",ehdr->e_ehsize);  
-     printf("Size of program headers:    %d (bytes)\n",ehdr->e_phentsize);  
-     printf("Number of program headers:  %d\n",ehdr->e_phnum);  
-     printf("Size of section headers:    %d (bytes)\n",ehdr->e_shentsize);  
-     printf("Number of section headers:  %d\n",ehdr->e_shnum);  
-     printf("Section header string table index:  %d\n",ehdr->e_shstrndx);  
- }  
- 
- int main(int argc,char *argv[])  
- {  
- if(argc != 2){  
-         printf("parameter error\n");  
-         exit(0);  
-     }  
-     Elf32_Ehdr *ehdr = (Elf32_Ehdr *)Map(argv[1]);  
- if(ehdr == NULL){  
-         perror("Map Error\n");  
-         exit(0);  
-     }  
-     displayEhdr(ehdr);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
