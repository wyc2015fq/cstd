# C/C++文件操作函数使用总结 - fanyun的博客 - CSDN博客
2017年09月01日 15:13:07[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：407
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)

fopen()函数：
1.作用: 在C语言中fopen()函数用于打开指定路径的文件，获取指向该文件的指针。
2.函数原型：
```cpp
FILE * fopen(const char * path,const char * mode);
    -- path: 文件路径，如："F:\Visual Stdio 2012\test.txt"
    -- mode: 文件打开方式，例如：
             "r" 以只读方式打开文件，该文件必须存在。
             "w" 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。若文件不存在则建立该文件。
            "w+" 打开可读写文件，若文件存在则文件长度清为零，即该文件内容会消失。若文件不存在则建立该文件。
             "a" 以附加的方式打开只写文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾，即文件原先的内容会被保留。（EOF符保留）
             "a+" 以附加方式打开可读写的文件。若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后，即文件原先的内容会被保留。（原来的EOF符不保留）
            "wb" 只写打开或新建一个二进制文件，只允许写数据。
            "wb+" 读写打开或建立一个二进制文件，允许读和写。
             "ab" 追加打开一个二进制文件，并在文件末尾写数据。
             "ab+"读写打开一个二进制文件，允许读，或在文件末追加数据。   
    --返回值: 文件顺利打开后，指向该流的文件指针就会被返回。如果文件打开失败则返回NULL，并把错误代码存在errno中。
```
2.函数原型：
```cpp
size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
    -- buffer:指向数据块的指针
    -- size:每个数据的大小，单位为Byte(例如：sizeof(int)就是4)
    -- count:数据个数
    -- stream:文件指针
```
(1) 调用格式：fwrite(buf,sizeof(buf),1,fp);
    成功写入返回值为1(即count)
(2)调用格式：fwrite(buf,1,sizeof(buf),fp);
    成功写入则返回实际写入的数据个数(单位为Byte)
3. 注意事项:
    写完数据后要调用fclose()关闭流，不关闭流的情况下，每次读或写数据后，文件指针都会指向下一个待写或者读数据位置的指针。
示例说明:
代码1：下面代码可将1024个字(int)写入到文本文件，fwrite的调用中，size是sizeof(int)，count是DATA_SIZE
```cpp
#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define DATA_SIZE 1024
int main()
{
    unsigned int *dataPtr = NULL;
    dataPtr = (unsigned int *)malloc(sizeof(int)*DATA_SIZE);
    for(unsigned int i=0;i<DATA_SIZE;i++)
    {
        dataPtr[i] = i; //初始化缓存区
    }
    FILE *fp = fopen("F:\\Labwindows cvi\\test.txt","w");
    fwrite(dataPtr,sizeof(int),DATA_SIZE,fp);
       fclose(fp);
       free(dataPtr);
    system("pause");
    return 0;
}
```
代码2：下面代码也可将1024个字写到文本中，虽然fwrite函数中，size是1，但count是DATA_SIZE*sizeof(int)。与代码1实现的结果一样。
```cpp
// datasave.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define DATA_SIZE 1024
int main()
{
    unsigned int *dataPtr = NULL;
    dataPtr = (unsigned int *)malloc(sizeof(int)*DATA_SIZE);
    for(unsigned int i=0;i<DATA_SIZE;i++)
    {
        dataPtr[i] = i; //初始化缓存区
    }
    FILE *fp = fopen("F:\\Labwindows cvi\\test.txt","ab+");
    fwrite(dataPtr,1,DATA_SIZE*sizeof(unsigned int),fp);
<pre name="code" class="cpp">       fclose(fp);
<pre name="code" class="cpp">       free(dataPtr)
```
代码3：下面的代码将4096个char数据写到文本，写入的数据中，最大值为255，与上面代码1、2有区别，因为缓存区数据类型不同
```cpp
// datasave.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define DATA_SIZE 1024
int main()
{
    unsigned char *dataPtr = NULL;
    dataPtr = (unsigned char *)malloc(sizeof(int)*DATA_SIZE); //申请的区域是4096个char，即1024个字的区域
    for(unsigned int i=0;i<DATA_SIZE;i++)
    {
        dataPtr[i] = i; //初始化缓存区
    }
    FILE *fp = fopen("F:\\Labwindows cvi\\test.txt","ab+");
    fwrite(dataPtr,sizeof(char),DATA_SIZE*sizeof(int),fp);
    fclose(fp);      
       free(dataPtr);
    system("pause");
    return 0;
}
```
代码4：用malloc函数申请区域时是申请的一片char*区域，通过强制类型转换后可装unsigned int 数据。
```cpp
// datasave.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define DATA_SIZE 1024
int main()
{
    unsigned char *dataPtr = NULL;
    unsigned int *Ptr = NULL;
    dataPtr = (unsigned char *)malloc(sizeof(int)*DATA_SIZE);
    Ptr = (unsigned int *) dataPtr;
    for(unsigned int i=0;i<DATA_SIZE;i++)
    {
        Ptr[i] = i; 
    }
    FILE *fp = fopen("F:\\Labwindows cvi\\test.txt","ab+");
    fwrite(Ptr,sizeof(unsigned int),DATA_SIZE,fp);
    fclose(fp);
    free(dataPtr);
   system("pause");
    return 0;
}
```
fread()函数：
1. 作用：从一个文件流中读取数据
2. 函数原型如下：
```cpp
size_t fread(void *buffer, size_t size, size_t count, FILE *stream);
	  -- buffer:指向数据块的指针
	  -- size:每个数据的大小，单位为Byte(例如：sizeof(int)就是4)
	  -- count:数据个数
	  -- stream:文件指针
```
注意：返回值随着调用格式的不同而不同：
(1) 调用格式：fread(buf,sizeof(buf),1,fp);
读取成功时：当读取的数据量正好是sizeof(buf)个Byte时，返回值为1(即count)
                       否则返回值为0(读取数据量小于sizeof(buf))
(2)调用格式：fread(buf,1,sizeof(buf),fp);
读取成功返回值为实际读回的数据个数(单位为Byte)
代码参考：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
	FILE *filp = NULL;
	char fileDir[] = "/home/yangzhiyuan/Documents/test.txt"; 
	char dataPtr[] = "Helloworld";
	printf("sizeof(dataPtr) = %ld\n",sizeof(dataPtr));
	filp = fopen(fileDir,"w+");  /* 可读可写，不存在则创建 */
	int writeCnt = fwrite(dataPtr,sizeof(dataPtr),1,filp);  /* 返回值为1 */
	//int writeCnt = fwrite(dataPtr,1,sizeof(dataPtr),filp);  /* 返回值为11 */
	printf("writeCnt = %d\n",writeCnt);
	fclose(filp);
	FILE *fp = NULL;
	fp = fopen(fileDir,"r");
	char buffer[256];
	int readCnt = fread(buffer,sizeof(buffer),1,fp);  /* 返回值为0 */
	//int readCnt = fread(buffer,1,sizeof(buffer),fp);  /* 返回值为11 */	
	printf("readCnt = %d\n",readCnt);
	fclose(fp);
	
	printf("%s\n",buffer);
	exit(0);
}
```
本例代码中定义了两个FILE变量，一个用于write，一个用于read，写完后要close掉，然后再打开，然后读。最后使用完都需要关闭。
