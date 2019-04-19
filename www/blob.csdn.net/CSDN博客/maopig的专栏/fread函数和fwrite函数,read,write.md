# fread函数和fwrite函数,read,write - maopig的专栏 - CSDN博客
2011年11月07日 19:00:04[maopig](https://me.csdn.net/maopig)阅读数：7721
fread函数和fwrite函数
 1.函数功能
用来读写一个数据块。
2.一般调用形式
fread(buffer,size,count,fp);
fwrite(buffer,size,count,fp);
3.说明
  （1）buffer：是一个指针，对fread来说，它是读入数据的存放地址。对fwrite来说，是要输出数据的地址。
  （2）size：要读写的字节数；
  （3）count:要进行读写多少个size字节的数据项；// 
1代表二进制
  （4）fp:文件型指针。
 注意：
          1 完成次写操(fwrite())作后必须关闭流(fclose());
           2 完成一次读操作(fread())后，如果没有关闭流(fclose()),则指针(FILE * fp)自动向后移动前一次读写的长度，不关闭流继续下一次读操作则接着上次的输出继续输出;
           3 fprintf() ： 按格式输入到流，其原型是int fprintf(FILE *stream, const char *format[, argument, ...]);其用法和printf()相同，不过不是写到控制台，而是写到流罢了。注意的是返回值为此次操作写入到文件的字节数。如int c = fprintf(fp, "%s %s %d %f", str1,str2, a, b) ;str1：10字节；str2： 10字节；a：2字节；b：8字节，c为33，因为写入时不同的数据间自动加入一个空格。
文件使用之后一定要关闭,否则将不能正确显示内容.fwrite:读入两个学生信息然后用fwrite存入文件
fread:用fread从文件中读出学生信息。
fwrite.c
#include <stdio.h>
#define SIZE 2
struct student_type
{
 char name[10];
 int num;
 int age;
 char addr[10];
}stud[SIZE];
void save()
{
 FILE *fp;
 int i;
 if((fp=fopen("stu_list","wb"))==NULL)
 {
  printf("cant open the file");
  exit(0);
 }
 for(i=0;i<SIZE;i++)
 {
   if(fwrite(&stud[i],sizeof(struct student_type),1,fp)!=1)
    printf("file write error\n");
 }
 fclose(fp);
}
main()
{
 int i;
 for(i=0;i<SIZE;i++)
 {
   scanf("%s%d%d%s",&stud[i].name,&stud[i].num,&stud[i].age,&stud[i].addr);
   save();
 }
 for(i=0;i<SIZE;i++)
 {
   printf("%s,%d,%d",stud[i].name,stud[i].num,stud[i].age,stud[i].addr);
 }
}
fread.c
#include <stdio.h>
#define SIZE 2
struct student_type
{
 char name[10];
 int num;
 int age;
 char addr[10];
}stud[SIZE];
void read()
{
 FILE *fp;
 int i;
 if((fp=fopen("stu_list","rb"))==NULL)
 {
  printf("cant open the file");
  exit(0);
 }
 for(i=0;i<SIZE;i++)
 {
   if(fread(&stud[i],sizeof(struct student_type),1,fp)!=1)
    printf("file write error\n");
 }
 fclose(fp);
}
main()
{
 int i;
 read();
 for(i=0;i<SIZE;i++)
 {
   printf("%s,%d,%d,%s",stud[i].name,stud[i].num,stud[i].age,stud[i].addr);
   printf("\n");
 }
}
在C语言中进行文件操作时，我们经常用到fread()和fwrite()，用它们来对文件进行读写操作。下面详细绍一下这两个函数的用法。
  我们在用C语言编写程序时，一般使用标准文件系统，即缓冲文件系统。系统在内存中为每个正在读写的文件开辟“文件缓冲区”，在对文件进行读写时数据都经过缓冲区。要对文件进行读写，系统首先开辟一块内存区来保存文件信息，保存这些信息用的是一个结构体，将这个结构体typedef为FILE类型。我们首先要定义一个指向这个结构体的指针，当程序打开一个文件时，我们获得指向FILE结构的指针，通过这个指针，我们就可以对文件进行操作。例如：
＃i nclude <stdio.h>
＃i nclude <string.h>
int main()
{
   FILE *fp;
   char buffer[100] = "This is a test";
   if((fp = fopen("c:\\example.txt", "w")) == 0)
    {
       printf("open failed!");
       exit(1);
    }
   fwrite(buffer, 1, strlen("This is a test"), fp);
   fclose(fp);
   return 0;
}
  通过以上代码，我们就在c盘的根目录下建立了一个名为example扩展名为.txt的文件，我们打开可以看到上面写上了This is a test。当我们对它将它读出时，用如下代码：
＃i nclude <stdio.h>
＃i nclude <mem.h>
int main()
{
   FILE *fp;   int len;
   char buffer[100];
   /*memset(buffer, 1, 100); */
   if((fp = fopen("c:\\example.txt", "r")) == 0)
    {
       printf("open failed!");
       exit(1);
    }
   fseek(fp, 0L, SEEK_END);
   len = ftell(fp);
   rewind(fp);
   fread(buffer, 1, len , fp);
   printf("%s",buffer);
   fclose(fp);
   getch();
   return 0;
}
 可以看到，当我们使用memset了以后，读出了一大堆乱码，这是为什么呢？原因是我们在fwrite函数时写入的字节数是用strlen求得的，也就是说字符串最后的'\0'并没有写到文件中去。所以我们从文件中读到buffer中时也自然没有'\0',因为buffer中的数是随机的，除非buffer中最后一个字符的下一个数恰好随机到0（可能性很小，这里用memset将它排除），否则以％s将buffer中的字符输出时遇不到0，所以乱码产生。解决的办法有很多，你可以在向文件写数据时多写入一个字节，系统会自动写入0，fwrite(buffer,
 1, strlen("This is a test")＋1, fp);这样读出时最后就有一个0了。或者读出操作完成后，在最后一个字符后面补上一个0：buffer[len] = 0;这样问题也可得到解决。
fread函数和fwrite函数 1.函数功能
  用来读写一个数据块。
2.一般调用形式
  fread(buffer,size,count,fp);
  fwrite(buffer,size,count,fp);
3.说明
  （1）buffer：是一个指针，对fread来说，它是读入数据的存放地址。对fwrite来说，是要输出数据的地址。
  （2）size：要读写的字节数；
  （3）count:要进行读写多少个size字节的数据项；
  （4）fp:文件型指针。
 注意：1 完成次写操(fwrite())作后必须关闭流(fclose());
           2 完成一次读操作(fread())后，如果没有关闭流(fclose()),则指针(FILE * fp)自动向后移动前一次读写的长度，不关闭流继续下一次读操作则接着上次的输出继续输出;
           3 fprintf() ： 按格式输入到流，其原型是int fprintf(FILE *stream, const char *format[, argument, ...]);其用法和printf()相同，不过不是写到控制台，而是写到流罢了。注意的是返回值为此次操作写入到文件的字节数。如int c = fprintf(fp, "%s %s %d %f", str1,str2, a, b) ;str1：10字节；str2： 10字节；a：2字节；b：8字节，c为33，因为写入时不同的数据间自动加入一个空格。
文件使用之后一定要关闭,否则将不能正确显示内容.fwrite:读入两个学生信息然后用fwrite存入文件
fread:用fread从文件中读出学生信息。
fwrite.c
#include <stdio.h>
#define SIZE 2
struct student_type
{
 char name[10];
 int num;
 int age;
 char addr[10];
}stud[SIZE];
void save()
{
 FILE *fp;
 int i;
 if((fp=fopen("stu_list","wb"))==NULL)
 {
  printf("cant open the file");
  exit(0);
 }
 for(i=0;i<SIZE;i++)
 {
   if(fwrite(&stud[i],sizeof(struct student_type),1,fp)!=1)
    printf("file write error\n");
 }
 fclose(fp);
}
main()
{
 int i;
 for(i=0;i<SIZE;i++)
 {
   scanf("%s%d%d%s",&stud[i].name,&stud[i].num,&stud[i].age,&stud[i].addr);
   save();
 }
 for(i=0;i<SIZE;i++)
 {
   printf("%s,%d,%d",stud[i].name,stud[i].num,stud[i].age,stud[i].addr);
 }
}
fread.c
#include <stdio.h>
#define SIZE 2
struct student_type
{
 char name[10];
 int num;
 int age;
 char addr[10];
}stud[SIZE];
void read()
{
 FILE *fp;
 int i;
 if((fp=fopen("stu_list","rb"))==NULL)
 {
  printf("cant open the file");
  exit(0);
 }
 for(i=0;i<SIZE;i++)
 {
   if(fread(&stud[i],sizeof(struct student_type),1,fp)!=1)
    printf("file write error\n");
 }
 fclose(fp);
}
main()
{
 int i;
 read();
 for(i=0;i<SIZE;i++)
 {
   printf("%s,%d,%d,%s",stud[i].name,stud[i].num,stud[i].age,stud[i].addr);
   printf("\n");
 }
}
在C语言中进行文件操作时，我们经常用到fread()和fwrite()，用它们来对文件进行读写操作。下面详细绍一下这两个函数的用法。
  我们在用C语言编写程序时，一般使用标准文件系统，即缓冲文件系统。系统在内存中为每个正在读写的文件开辟“文件缓冲区”，在对文件进行读写时数据都经过缓冲区。要对文件进行读写，系统首先开辟一块内存区来保存文件信息，保存这些信息用的是一个结构体，将这个结构体typedef为FILE类型。我们首先要定义一个指向这个结构体的指针，当程序打开一个文件时，我们获得指向FILE结构的指针，通过这个指针，我们就可以对文件进行操作。例如：
＃i nclude <stdio.h>
＃i nclude <string.h>
int main()
{
   FILE *fp;
   char buffer[100] = "This is a test";
   if((fp = fopen("c:\\example.txt", "w")) == 0)
    {
       printf("open failed!");
       exit(1);
    }
   fwrite(buffer, 1, strlen("This is a test"), fp);
   fclose(fp);
   return 0;
}
  通过以上代码，我们就在c盘的根目录下建立了一个名为example扩展名为.txt的文件，我们打开可以看到上面写上了This is a test。当我们对它将它读出时，用如下代码：
＃i nclude <stdio.h>
＃i nclude <mem.h>
int main()
{
   FILE *fp;   int len;
   char buffer[100];
   /*memset(buffer, 1, 100); */
   if((fp = fopen("c:\\example.txt", "r")) == 0)
    {
       printf("open failed!");
       exit(1);
    }
   fseek(fp, 0L, SEEK_END);
   len = ftell(fp);
   rewind(fp);
   fread(buffer, 1, len , fp);
   printf("%s",buffer);
   fclose(fp);
   getch();
   return 0;
}
 可以看到，当我们使用memset了以后，读出了一大堆乱码，这是为什么呢？原因是我们在fwrite函数时写入的字节数是用strlen求得的，也就是说字符串最后的'\0'并没有写到文件中去。所以我们从文件中读到buffer中时也自然没有'\0',因为buffer中的数是随机的，除非buffer中最后一个字符的下一个数恰好随机到0（可能性很小，这里用memset将它排除），否则以％s将buffer中的字符输出时遇不到0，所以乱码产生。解决的办法有很多，你可以在向文件写数据时多写入一个字节，系统会自动写入0，fwrite(buffer,
 1, strlen("This is a test")＋1, fp);这样读出时最后就有一个0了。或者读出操作完成后，在最后一个字符后面补上一个0：buffer[len] = 0;这样问题也可得到解决。
/***
*fwrite.c - read from a stream
*
*       Copyright (c) 1989-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Write to the specified stream from the user's buffer.
*
*******************************************************************************/
#include <cruntime.h>
#include <stdio.h>
#include <mtdll.h>
#include <io.h>
#include <string.h>
#include <file2.h>
#include <internal.h>
/***
*size_t fwrite(void *buffer, size_t size, size_t count, FILE *stream) -
*       write to the specified stream from the specified buffer.
*
*Purpose:
*       Write 'count' items of size 'size' to the specified stream from
*       the specified buffer. Return when 'count' items have been written
*       or no more items can be written to the stream.
*
*Entry:
*       buffer  - pointer to user's buffer
*       size    - size of the item to write
*       count   - number of items to write
*       stream  - stream to write to
*
*Exit:
*       Returns the number of (whole) items that were written to the stream.
*       This may be less than 'count' if an error or eof occurred. In this
*       case, ferror() or feof() should be used to distinguish between the
*       two conditions.
*
*Notes:
*       fwrite will attempt to buffer the stream (side effect of the _flsbuf
*       call) if necessary.
*
*       No more than 0xFFFE bytes may be written out at a time by a call to
*       write(). Further, write() does not handle huge buffers. Therefore,
*       in large data models, the write request is broken down into chunks
*       that do not violate these considerations. Each of these chunks is
*       processed much like an fwrite() call in a small data model (by a
*       call to _nfwrite()).
*
*       This code depends on _iob[] being a near array.
*
*       MTHREAD/DLL - Handled in just two layers since it is small data
*       model. The outer layer, fwrite(), handles stream locking/unlocking
*       and calls _fwrite_lk() to do the work. _fwrite_lk() is the same as
*       the single-thread, small data model version of fwrite().
*
*******************************************************************************/
#ifdef _MT
/* define locking/unlocking version */
size_t __cdecl fwrite (
        const void *buffer,
        size_t size,
        size_t count,
        FILE *stream
        )
{
        size_t retval;
        _lock_str(stream);                      /* lock stream */
        retval = _fwrite_lk(buffer, size, count, stream);  /* do the read */
        _unlock_str(stream);                    /* unlock stream */
        return retval;
}
#endif  /* _MT */
/* define the normal version */
#ifdef _MT
size_t __cdecl _fwrite_lk (
#else  /* _MT */
size_t __cdecl fwrite (
#endif  /* _MT */
const void *buffer,
        size_t size,
        size_t num,
        FILE *stream
        )
{
const char *data;               /* point to where data comes from next */
        unsigned total;                 /* total bytes to write */
        unsigned count;                 /* num bytes left to write */
        unsigned bufsize;               /* size of stream buffer */
        unsigned nbytes;                /* number of bytes to write now */
        unsigned nwritten;              /* number of bytes written */
        int c;                          /* a temp char */
        /* initialize local vars */
 data = buffer;
        count = total = size * num;
        if (0 == count)
            return 0;
        if (anybuf(stream))
                /* already has buffer, use its size */
                bufsize = stream->_bufsiz;
        else
#if defined (_M_M68K) || defined (_M_MPPC)
                /* assume will get BUFSIZ buffer */
                bufsize = BUFSIZ;
#else  /* defined (_M_M68K) || defined (_M_MPPC) */
                /* assume will get _INTERNAL_BUFSIZ buffer */
                bufsize = _INTERNAL_BUFSIZ;
#endif  /* defined (_M_M68K) || defined (_M_MPPC) */
        /* here is the main loop -- we go through here until we're done */
        while (count != 0) {
                /* if the buffer is big and has room, copy data to buffer */
                if (bigbuf(stream) && stream->_cnt != 0) {
                        /* how much do we want? */
                        nbytes = (count < (unsigned)stream->_cnt) ? count : stream->_cnt;
                        memcpy(stream->_ptr, data, nbytes);
                        /* update stream and amt of data written */
                        count -= nbytes;
                        stream->_cnt -= nbytes;
                        stream->_ptr += nbytes;
                        data += nbytes;
                }
                else if (count >= bufsize) {
                        /* If we have more than bufsize chars to write, write
                           data by calling write with an integral number of
                           bufsiz blocks.  If we reach here and we have a big
                           buffer, it must be full so _flush it. */
                        if (bigbuf(stream)) {
                                if (_flush(stream)) {
                                        /* error, stream flags set -- we're out
                                           of here */
                                        return (total - count) / size;
                                }
                        }
                        /* calc chars to read -- (count/bufsize) * bufsize */
                        nbytes = ( bufsize ? (count - count % bufsize) :
                                   count );
                        nwritten = _write(_fileno(stream), data, nbytes);
                        if (nwritten == (unsigned)EOF) {
                                /* error -- out of here */
                                stream->_flag |= _IOERR;
                                return (total - count) / size;
                        }
                        /* update count and data to reflect write */
                        count -= nwritten;
                        data += nwritten;
                        if (nwritten < nbytes) {
                                /* error -- out of here */
                                stream->_flag |= _IOERR;
                                return (total - count) / size;
                        }
                }
                else {
                        /* buffer full and not enough chars to do direct write,
                           so do a _flsbuf. */
                        c = *data;  /* _flsbuf write one char, this is it */
                        if (_flsbuf(c, stream) == EOF) {
                                /* error or eof, stream flags set by _flsbuf */
                                return (total - count) / size;
                        }
                        /* _flsbuf wrote a char -- update count */
                        ++data;
                        --count;
                        /* update buffer size */
                        bufsize = stream->_bufsiz > 0 ? stream->_bufsiz : 1;
                }
        }
        /* we finished successfully, so just return num */
        return num;
}
这是fread的源文件
/***
*fread.c - read from a stream
*
*       Copyright (c) 1989-1997, Microsoft Corporation. All rights reserved.
*
*Purpose:
*       Read from the specified stream into the user's buffer.
*
*******************************************************************************/
#include <cruntime.h>
#include <stdio.h>
#include <mtdll.h>
#include <io.h>
#include <string.h>
#include <file2.h>
/***
*size_t fread(void *buffer, size_t size, size_t count, FILE *stream) -
*       read from specified stream into the specified buffer.
*
*Purpose:
*       Read 'count' items of size 'size' from the specified stream into
*       the specified buffer. Return when 'count' items have been read in
*       or no more items can be read from the stream.
*
*Entry:
*       buffer  - pointer to user's buffer
*       size    - size of the item to read in
*       count   - number of items to read
*       stream  - stream to read from
*
*Exit:
*       Returns the number of (whole) items that were read into the buffer.
*       This may be less than 'count' if an error or eof occurred. In this
*       case, ferror() or feof() should be used to distinguish between the
*       two conditions.
*
*Notes:
*       fread will attempt to buffer the stream (side effect of the _filbuf
*       call) if necessary.
*
*       No more than 0xFFFE bytes may be read in at a time by a call to
*       read(). Further, read() does not handle huge buffers. Therefore,
*       in large data models, the read request is broken down into chunks
*       that do not violate these considerations. Each of these chunks is
*       processed much like an fread() call in a small data model (by a
*       call to _nfread()).
*
*       MTHREAD/DLL - Handled in three layers. fread() handles the locking
*       and DS saving/loading/restoring (if required) and calls _fread_lk()
*       to do the work. _fread_lk() is the same as the single-thread,
*       large data model version of fread(). It breaks up the read request
*       into digestible chunks and calls _nfread() to do the actual work.
*
*       386/MTHREAD/DLL - Handled in just the two layers since it is small
*       data model. The outer layer, fread(), takes care of the stream locking
*       and calls _fread_lk() to do the actual work. _fread_lk() is the same
*       as the single-thread version of fread().
*
*******************************************************************************/
#ifdef _MT
/* define locking/unlocking version */
size_t __cdecl fread (
        void *buffer,
        size_t size,
        size_t count,
        FILE *stream
        )
{
        size_t retval;
        _lock_str(stream);                                /* lock stream */
        retval = _fread_lk(buffer, size, count, stream);  /* do the read */
        _unlock_str(stream);                              /* unlock stream */
        return retval;
}
#endif  /* _MT */
/* define the normal version */
#ifdef _MT
size_t __cdecl _fread_lk (
#else  /* _MT */
size_t __cdecl fread (
#endif  /* _MT */
        void *buffer,
        size_t size,
        size_t num,
        FILE *stream
        )
{
        char *data;                     /* point to where should be read next */
        unsigned total;                 /* total bytes to read */
        unsigned count;                 /* num bytes left to read */
        unsigned bufsize;               /* size of stream buffer */
        unsigned nbytes;                /* how much to read now */
        unsigned nread;                 /* how much we did read */
        int c;                          /* a temp char */
        /* initialize local vars */
        data = buffer;
        if ( (count = total = size * num) == 0 )
                return 0;
        if (anybuf(stream))
                /* already has buffer, use its size */
                bufsize = stream->_bufsiz;
        else
#if defined (_M_M68K) || defined (_M_MPPC)
                /* assume will get BUFSIZ buffer */
                bufsize = BUFSIZ;
#else  /* defined (_M_M68K) || defined (_M_MPPC) */
                /* assume will get _INTERNAL_BUFSIZ buffer */
                bufsize = _INTERNAL_BUFSIZ;
#endif  /* defined (_M_M68K) || defined (_M_MPPC) */
        /* here is the main loop -- we go through here until we're done */
        while (count != 0) {
                /* if the buffer exists and has characters, copy them to user
                   buffer */
                if (anybuf(stream) && stream->_cnt != 0) {
                        /* how much do we want? */
                        nbytes = (count < (unsigned)stream->_cnt) ? count : stream->_cnt;
                        memcpy(data, stream->_ptr, nbytes);
                        /* update stream and amt of data read */
                        count -= nbytes;
                        stream->_cnt -= nbytes;
                        stream->_ptr += nbytes;
                        data += nbytes;
                }
                else if (count >= bufsize) {
                        /* If we have more than bufsize chars to read, get data
                           by calling read with an integral number of bufsiz
                           blocks.  Note that if the stream is text mode, read
                           will return less chars than we ordered. */
                        /* calc chars to read -- (count/bufsize) * bufsize */
                        nbytes = ( bufsize ? (count - count % bufsize) :
                                   count );
                        nread = _read(_fileno(stream), data, nbytes);
                        if (nread == 0) {
                                /* end of file -- out of here */
                                stream->_flag |= _IOEOF;
                                return (total - count) / size;
                        }
                        else if (nread == (unsigned)-1) {
                                /* error -- out of here */
                                stream->_flag |= _IOERR;
                                return (total - count) / size;
                        }
                        /* update count and data to reflect read */
                        count -= nread;
                        data += nread;
                }
                else {
                        /* less than bufsize chars to read, so call _filbuf to
                           fill buffer */
                        if ((c = _filbuf(stream)) == EOF) {
                                /* error or eof, stream flags set by _filbuf */
                                return (total - count) / size;
                        }
                        /* _filbuf returned a char -- store it */
                        *data++ = (char) c;
                        --count;
                        /* update buffer size */
                        bufsize = stream->_bufsiz;
                }
        }
        /* we finished successfully, so just return num */
        return num;
} 赞同0| 评论              2009-7-29 20:06 zawdd | 七级 
stdio.h
size_t fwrite( const void *buffer, size_t size, size_t count, FILE *stream );
把数据写入一个数据流
这个例子中实现了把从A到Z的26个字符写入文件sample.txt:     
FILE *Stream; 
int Count;  
char Buffer[30];   
Stream=fopen("sample.txt","w+t");   
Buffer[0]='A';  
for(I=1; I<=25;I++) {
  Buffer[I]=Buffer[I-1]+1; 
}
  Count= fwrite(Buffer,sizeof(char),26，Stream);  
  cout<<Count; 
  fclose(Stream);
fread
stdio.h
从一个数据流中读取数据
使用实例：请参看feof函数
feof
stdio.h
int feof( FILE *stream );
测试文件指针是否指向数据流的尾部，若是则返回0，否则返回-1
这个例子中打开文件sample.txt，每次读取10个字节，直到所以的字节被读完为止：
    FILE  *p;
    int count,Size=0;
    char Buffer[10];
    if((p = fopen("sample.txt","r"))==NULL) exit(1);
    while(!feof(p)) {
        count =fread (Buffer,sizeof(char),10,p);
        if(ferror(p)) {
            perror("出错！");
            break;
        }
        Size = Size + count;
    }
    fclose(p); 
read函数从打开的设备或文件中读取数据。
#include <unistd.h> ssize_t read(int fd, void *buf, size_t count); 
返回值：成功返回读取的字节数，出错返回-1并设置errno，如果在调read之前已到达文件末尾，则这次read返回0
参数count是请求读取的字节数，读上来的数据保存在缓冲区buf中，同时文件的当前读写位置向后移。注意这个读写位置和使用C标准I/O库时的读写位置有可能不同，这个读写位置是记在内核中的，而使用C标准I/O库时的读写位置是用户空间I/O缓冲区中的位置。比如用fgetc读一个字节，fgetc有可能从内核中预读1024个字节到I/O缓冲区中，再返回第一个字节，这时该文件在内核中记录的读写位置是1024，而在FILE结构体中记录的读写位置是1。注意返回值类型是ssize_t，表示有符号的size_t，这样既可以返回正的字节数、0（表示到达文件末尾）也可以返回负值-1（表示出错）。read函数返回时，返回值说明了buf中前多少个字节是刚读上来的。有些情况下，实际读到的字节数（返回值）会小于请求读的字节数count，例如：
读常规文件时，在读到count个字节之前已到达文件末尾。例如，距文件末尾还有30个字节而请求读100个字节，则read返回30，下次read将返回0。
从终端设备读，通常以行为单位，读到换行符就返回了。
从网络读，根据不同的传输层协议和内核缓存机制，返回值可能小于请求的字节数，后面socket编程部分会详细讲解。
write函数向打开的设备或文件中写数据。
#include <unistd.h> ssize_t write(int fd, const void *buf, size_t count); 
返回值：成功返回写入的字节数，出错返回-1并设置errno
写常规文件时，write的返回值通常等于请求写的字节数count，而向终端设备或网络写则不一定。
读常规文件是不会阻塞的，不管读多少字节，read一定会在有限的时间内返回。从终端设备或网络读则不一定，如果从终端输入的数据没有换行符，调用read读终端设备就会阻塞，如果网络上没有接收到数据包，调用read从网络读就会阻塞，至于会阻塞多长时间也是不确定的，如果一直没有数据到达就一直阻塞在那里。同样，写常规文件是不会阻塞的，而向终端设备或网络写则不一定。
现在明确一下阻塞（Block）这个概念。当进程调用一个阻塞的系统函数时，该进程被置于睡眠（Sleep）状态，这时内核调度其它进程运行，直到该进程等待的事件发生了（比如网络上接收到数据包，或者调用sleep指定的睡眠时间到了）它才有可能继续运行。与睡眠状态相对的是运行（Running）状态，在Linux内核中，处于运行状态的进程分为两种情况：
正在被调度执行。CPU处于该进程的上下文环境中，程序计数器（eip）里保存着该进程的指令地址，通用寄存器里保存着该进程运算过程的中间结果，正在执行该进程的指令，正在读写该进程的地址空间。
就绪状态。该进程不需要等待什么事件发生，随时都可以执行，但CPU暂时还在执行另一个进程，所以该进程在一个就绪队列中等待被内核调度。系统中可能同时有多个就绪的进程，那么该调度谁执行呢？内核的调度算法是基于优先级和时间片的，而且会根据每个进程的运行情况动态调整它的优先级和时间片，让每个进程都能比较公平地得到机会执行，同时要兼顾用户体验，不能让和用户交互的进程响应太慢。
下面这个小程序从终端读数据再写回终端。
例 28.2. 阻塞读终端
#include <unistd.h> 
#include <stdlib.h>
 int main(void) 
 {
 char buf[10]; 
 int n; 
 n = read(STDIN_FILENO, buf, 10);
 if (n < 0) 
 { 
 perror("read STDIN_FILENO"); 
 exit(1); 
 }
 write(STDOUT_FILENO, buf, n); 
 return 0; 
 }
执行结果如下：
$ ./a.out hello（回车） hello $ ./a.out hello world（回车） hello worl$ d bash: d: command not found
第一次执行a.out的结果很正常，而第二次执行的过程有点特殊，现在分析一下：
Shell进程创建a.out进程，a.out进程开始执行，而Shell进程睡眠等待a.out进程退出。
a.out调用read时睡眠等待，直到终端设备输入了换行符才从read返回，read只读走10个字符，剩下的字符仍然保存在内核的终端设备输入缓冲区中。
a.out进程打印并退出，这时Shell进程恢复运行，Shell继续从终端读取用户输入的命令，于是读走了终端设备输入缓冲区中剩下的字符d和换行符，把它当成一条命令解释执行，结果发现执行不了，没有d这个命令。
如果在open一个设备时指定了O_NONBLOCK标志，read/write就不会阻塞。以read为例，如果设备暂时没有数据可读就返回-1，同时置errno为EWOULDBLOCK（或者EAGAIN，这两个宏定义的值相同），表示本来应该阻塞在这里（would block，虚拟语气），事实上并没有阻塞而是直接返回错误，调用者应该试着再读一次（again）。这种行为方式称为轮询（Poll），调用者只是查询一下，而不是阻塞在这里死等，这样可以同时监视多个设备：
while(1) { 非阻塞read(设备1); if(设备1有数据到达) 处理数据; 非阻塞read(设备2); if(设备2有数据到达) 处理数据; ... }
如果read(设备1)是阻塞的，那么只要设备1没有数据到达就会一直阻塞在设备1的read调用上，即使设备2有数据到达也不能处理，使用非阻塞I/O就可以避免设备2得不到及时处理。
非阻塞I/O有一个缺点，如果所有设备都一直没有数据到达，调用者需要反复查询做无用功，如果阻塞在那里，操作系统可以调度别的进程执行，就不会做无用功了。在使用非阻塞I/O时，通常不会在一个while循环中一直不停地查询（这称为Tight Loop），而是每延迟等待一会儿来查询一下，以免做太多无用功，在延迟等待的时候可以调度其它进程执行。
while(1) { 非阻塞read(设备1); if(设备1有数据到达) 处理数据; 非阻塞read(设备2); if(设备2有数据到达) 处理数据; ... sleep(n); }
这样做的问题是，设备1有数据到达时可能不能及时处理，最长需延迟n秒才能处理，而且反复查询还是做了很多无用功。以后要学习的select(2)函数可以阻塞地同时监视多个设备，还可以设定阻塞等待的超时时间，从而圆满地解决了这个问题。
以下是一个非阻塞I/O的例子。目前我们学过的可能引起阻塞的设备只有终端，所以我们用终端来做这个实验。程序开始执行时在0、1、2文件描述符上自动打开的文件就是终端，但是没有O_NONBLOCK标志。所以就像例 28.2 “阻塞读终端”一样，读标准输入是阻塞的。我们可以重新打开一遍设备文件/dev/tty（表示当前终端），在打开时指定O_NONBLOCK标志。
例 28.3. 非阻塞读终端
#include <unistd.h> 
#include <fcntl.h>
 #include <errno.h>
 #include <string.h> 
 #include <stdlib.h> 
 #define MSG_TRY "try again\n" 
 int main(void)
 {
 char buf[10];
 int fd, n; 
 fd = open("/dev/tty", O_RDONLY|O_NONBLOCK); 
 if(fd<0) { perror("open /dev/tty"); exit(1); }
 tryagain: n = read(fd, buf, 10); if (n < 0) 
 { 
 if (errno == EAGAIN)
 { 
 sleep(1); 
 write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
 goto tryagain;
 } 
 perror("read /dev/tty"); 
 exit(1); 
 } 
 write(STDOUT_FILENO, buf, n); 
 close(fd);
 return 0; 
 }
以下是用非阻塞I/O实现等待超时的例子。既保证了超时退出的逻辑又保证了有数据到达时处理延迟较小。
例 28.4. 非阻塞读终端和等待超时
#include <unistd.h> 
#include <fcntl.h> 
#include <errno.h> 
#include <string.h>
 #include <stdlib.h>
 #define MSG_TRY "try again\n"
 #define MSG_TIMEOUT "timeout\n" 
 int main(void) 
 { 
 char buf[10]; 
 int fd, n, i; 
 fd = open("/dev/tty", O_RDONLY|O_NONBLOCK); 
 if(fd<0) { perror("open /dev/tty"); exit(1); }
 for(i=0; i<5; i++) 
 {
 n = read(fd, buf, 10); 
 if(n>=0) break; 
 if(errno!=EAGAIN) {
 perror("read /dev/tty"); exit(1); 
 } 
 sleep(1);
 write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY)); 
 } 
 if(i==5)
 write(STDOUT_FILENO, MSG_TIMEOUT, strlen(MSG_TIMEOUT)); 
 else write(STDOUT_FILENO, buf, n); close(fd); return 0; 
 }
