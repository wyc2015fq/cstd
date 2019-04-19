# 标准IO与文件IO - yixianfeng41的专栏 - CSDN博客
2015年12月29日 22:17:23[第2梦](https://me.csdn.net/yixianfeng41)阅读数：771
个人分类：[Linux/Unix](https://blog.csdn.net/yixianfeng41/article/category/6006179)
所属专栏：[Linux/Unix 网络编程](https://blog.csdn.net/column/details/14513.html)
## **标准ＩＯ**
标准I/O是ANSI C建立的一个标准I/O模型，是一个标准函数包和stdio.h头文件中的定义，具有一定的可移植性。标准IO库处理很多细节。例如缓存分配，以优化长度执行IO等。
标准的IO提供了三种类型的缓存:
1）全缓存：当填满标准IO缓存后才进行实际的IO操作。 
2）行缓存：当输入或输出中遇到新行符时，标准IO库执行IO操作。 
3）不带缓存：stderr就是了。
## **文件ＩＯ**
文件ＩＯ称之为不带缓存的IO（unbuffered I/O)。不带缓存指的是每个read，write都调用内核中的一个系统调用。也就是一般所说的低级I/O——操作系统提供的基本IO服务，与os绑定，特定于linix或unix平台。
## **区别**
首先：两者一个显著的不同点在于，标准I/O默认采用了缓冲机制，比如调用fopen函数，不仅打开一个文件，而且建立了一个缓冲区（读写模式下将建立两个缓冲区），还创建了一个包含文件和缓冲区相关数据的数据结构。低级I/O一般没有采用缓冲，需要自己创建缓冲区，不过其实在linix或unix系统中，都是有使用称为内核缓冲的技术用于提高效率，读写调用是在内核缓冲区和进程缓冲区之间进行的数据复制。
其次从操作的设备上来区分，文件I/O主要针对文件操作，读写硬盘等，它操作的是文件描述符，标准I/O针对的是控制台，打印输出到屏幕等，它操作的是字符流。对于不同设备得特性不一样，必须有不同api访问才最高效。
||标准ＩＯ|文件ＩＯ(低级IO)|
|----|----|----|
|打开|fopen,freopen,fdopen|open|
|关闭|fclose|close|
|读|getc,fgetc,getcharfgets,getsfread|read|
|写|putc,fputc,putcharfputs,puts,fwrite|write|
最后来看下他们使用的函数
### **1.fopen与open**
1) 标准I/O使用fopen函数打开一个文件：
```cpp
FILE fp=fopen(const char path,const char mod)
```
其中path是文件名，mod用于指定文件打开的模式的字符串，比如"r","w","w+","a"等等，可以加上字母b用以指定以二进制模式打开（对于 NULL,这里的文件指针并不是指向实际的文件，而是一个关于文件信息的数据包，其中包括文件使用的缓冲区信息。
2) 文件IO使用open函数用于打开一个文件：
```cpp
int fd=open(char name,int how);
```
 与fopen类似，name表示文件名字符串，而how指定打开的模式：O_RDONLY(只读),O_WRONLY(只写）,O_RDWR （可读可写),还有其他模式请man 2 open。成功返回一个正整数称为文件描述符，这与标准I/O显著不同，失败的话返回-1，与标准I/O返回NULL也是不同的。
### **2.fclose与close**
1) 与打开文件相对的，标准I/O使用fclose关闭文件，将文件指针传入即可，如果成功关闭，返回0，否则返回EOF比如：
```cpp
if(fclose(fp)!=0) printf("Error in closing file");
```
2) 而文件IO使用close用于关闭open打开的文件，与fclose类似，只不过当错误发生时返回的是-1，而不是EOF，成功关闭同样是返回0。C语言用error code来进行错误处理的传统做法。
**3. 读文件，getc,fscanf,fgets和read**
1) 标准I/O中进行文件读取可以使用getc，一个字符一个字符的读取，也可以使用gets（读取标准io读入的）、fgets以字符串单位进行读取（读到遇到的第一个换行字符的后面），gets（接受一个参数，文件指针）不判断目标数组是否能够容纳读入的字符，可能导致存储溢出(不建议使用），而fgets使用三个参数： char fgets(char s, int size, FILE stream);
第一个参数和gets一样，用于存储输入的地址，第二个参数为整数，表示输入字符串的最大长度，最后一个参数就是文件指针，指向要读取的文件。最 后是fscanf，与scanf类似，只不过增加了一个参数用于指定操作的文件，比如fscanf(fp,"%s",words)
2) 文件IO中使用read函数用于读取open函数打开的文件，函数原型如下：
```cpp
ssize_t numread=read(int fd,void buf,size_t qty);
```
其中fd就是open返回的文件描述符，buf用于存储数据的目的缓冲区，而qty指定要读取的字节数。如果成功读取，就返回读取的字节数目（小于等于qty）
### **4. 判断文件结尾**
如果尝试读取达到文件结尾，标准IO的getc会返回特殊值EOF，而fgets碰到EOF会返回NULL,而对于nix的read函数，情况有所不 同。read读取qty指定的字节数，最终读取的数据可能没有你所要求的那么多（qty），而当读到结尾再要读的话，read函数将返回0.
### **5. 写文件：putc,fputs,fprintf和write**
1) 与读文件相对应的，标准C语言I/O使用putc写入字符，比如：putc(ch,fp);
第一个参数是字符，第二个是文件指针。而fputs与此类似：fputs(buf,fp);仅仅是第一个参数换成了字符串地址。
    而fprintf与printf类似，增加了一个参数用于指定写入的文件，比如：fprintf(stdout,"Hello %s.\n","dennis");切记fscanf和fprintf将FILE指针作为第一个参数，而putc,fputs则是作为第二个参数。
2) 在文件IO中提供write函数用于写入文件，原型与read类似：ssize_t result=write(int fd,void buf ,size_t amt); fd是文件描述符，buf是将要写入的内存数据，amt是要写的字节数。如果写入成功返回写入的字节数，通过result与amt的比较可以判断是否写入正常，如果写入失败返回-1
### **6. 随机存取：fseek()、ftell()和lseek()**
1) 标准I/O使用fseek和ftell用于文件的随机存取，先看看fseek函数原型
```cpp
int fseek(FILE stream, long offset, int whence);
```
第一个参数是文件指针，第二个参数是一个long类型的偏移量（offset），表示从起始点开始移动的距离。第三个参数就是用于指定起始点的模式，stdio.h指定了下列模式常量：
seek_set 文件开始处 
SEEK_CUR 当前位置 
SEEK_END 文件结尾处
看几个调用例子： fseek(fp,0L,SEEK_SET); //找到文件的开始处
fseek(fp,0L,SEEK_END); //定位到文件结尾处 
fseek(fp,2L,SEEK_CUR); //文件当前位置向前移动2个字节数
而ftell函数用于返回文件的当前位置，返回类型是一个long类型，比如下面的调用：
fseek(fp,0L,SEEK_END);//定位到结尾    long last=ftell(fp); //返回当前位置   那么此时的last就是文件指针fp指向的文件的字节数。
2) 与标准I/O类似，nix系统提供了lseek来完成fseek的功能，原型如下：
```cpp
off_t lseek(int fildes, off_t offset, int whence);
```
fildes是文件描述符，而offset也是偏移量，whence同样是指定起始点模式，唯一的不同是lseek有返回值，如果成功就 返回指针变化前的位置，否则返回-1。whence的取值与fseek相同：SEEK_SET,SEEK_CUR,SEEK_END，但也可以用整数 0,1,2相应代替。“本文由华清远见http://www.embedu.org/index.htm提供”
