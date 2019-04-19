# stderr和stdout详细解说 - 数据之美的博客 - CSDN博客
2017年04月07日 14:56:04[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：13494
<cstdio>
object
stderr
FILE * stderr;
Standard error stream
The standard error stream is the defaultdestination for error messages and other diagnostic warnings. Like stdout, itis usually also directed by default to the text console (generally, on thescreen).
stderr can be used as an argument for anyfunction that takes an argument of type FILE* expecting an output stream, likefputs or fprintf.
Although in many cases both stdout andstderr are associated with the same output device (like the console),applications may differentiate between what is sent to stdout and what tostderr for the case that one of them is redirected.
 For example, it is frequentto redirect the regular output of a console program (stdout) to a file whileexpecting the error messages to keep appearing in the console.
It is also possible to redirect stderr tosome other destination from within a program using the freopen function.
stderr is is never fully buffered onstartup. It is library-dependent whether the stream is line buffered or notbuffered by default (see setvbuf).
**stderr和stdout详细解说**
今天又查了一下fprintf，其中对第一个参数stderr特别感兴趣。
int fprintf(FILE *stream,char*format,[argument])；
在此之前先区分一下：printf，sprintf，fprintf。
1，printf就是标准输出，在屏幕上打印出一段字符串来。
2，sprintf就是把格式化的数据写入到某个字符串中。返回值字符串的长度。
3，fprintf是用于文件操作。
     原型：int fprintf(FILE *stream,char *format,[argument])；      
     功能：fprintf()函数根据指定的format(格式)发送信息(参数)到由stream(流)指定的文件.因此fprintf()可以使得信息输出到指定的文件。
例子:
#include<iostream>
#include<stdio.h>
usingnamespace std;
int main()
{
char name[] = "lucy";
    FILE* out;
    out = fopen("output.txt", "w");
if(NULL != out)
    {
int rel =fprintf(out,"Hello %s\n", name);
        cout<< "写入了" << rel << "个字符" << endl;
    }
    system("pause");
return 0;
}
返回值：若成功则返回输出字符数，若输出出错则返回负值。
好了，以上到此为止。
然后深挖stdout，stderr。
stdout, stdin, stderr的中文名字分别是标准输出，标准输入和标准错误。
当一个用户进程被创建的时候，系统会自动为该进程创建三个数据流，也就是题目中所提到的这三个。那么什么是数据流呢（stream）？我们知道，一个程序要运行，需要有输入、输出，如果出错，还要能表现出自身的错误。这是就要从某个地方读入数据、将数据输出到某个地方，这就够成了数据流。
因此，一个进程初期所拥有的这么三个数据流，就分别是标准输出、标准输入和标准错误，分别用stdout, stdin, stderr来表示。。这3个文件分别为标准输入（stdin）、标准输出（stdout）、标准错误（stderr）。它们在<stdio.h>中声明，大多数环境中，stdin指向键盘，stdout、stderr指向显示器。之所以使用stderr，若因某种原因造成其中一个文件无法访问，相应的诊断信息要在该链接的输出的末尾才能打印出来。当输出到屏幕时，这种处理方法尚可接受，但如果输出到一个文件或通过管道输出到另一个程序时，就无法接受了。若有stderr存在，即使对标准输出进行了重定向，写到stderr中的输出通常也会显示在屏幕上。比如我们在c++中使用fprintf:
fprintf(stdout,"hello world!\n");
屏幕上将打印出"helloworld!"来。
同样，我们使用：fread(ptr,1,10,stdin);
上面的代码会接收用户输入在终端里的字符，并存在ptr中。
那么标准输入输出和错误是不是只能反应在终端里呢？答案是不是的！我们可以将标准输入和输出重定位到文件中：
1，我们知道，标准输出和标准错误默认都是将信息输出到终端上，那么他们有什么区别呢？让我们来看个题目：
问题：下面程序的输出是什么？（intel笔试2011）
int main(){
fprintf(stdout,"Hello ");
fprintf(stderr,"World!");
return0;
}
解答：这段代码的输出是什么呢？，然后发现输出是:
World!Hello
这是为什么呢？在默认情况下，stdout是行缓冲的，他的输出会放在一个buffer里面，只有到换行的时候，才会输出到屏幕。而stderr是无缓冲的，会直接输出，举例来说就是fprintf(stdout, "xxxx") 和 fprintf(stdout,"xxxx\n")，前者会缓存，直到遇到新行才会一起输出。而fprintf(stderr, "xxxxx")，不管有么有\n，都输出。
2，fprintf(stderr,"Can't open it!\n");
fprintf(stdout, "Can't open it!\n");
printf("Can't open it!\n");
这3句效果不是一样啊，有什么区别吗？
有区别。
stdout -- 标准输出设备
stderr -- 标准错误输出设备
两者默认向屏幕输出。
但如果用转向标准输出到磁盘文件，则可看出两者区别。stdout输出到磁盘文件，stderr在屏幕。
**[C++](http://lib.csdn.net/base/cplusplus)stderr/stdout 重定向到文件**
通常，stderr和stdout被用来输出内容显示到屏幕，但是，有时候我们需要把这些信息写到指定的文件，方便随时查阅。最简单的实现方式就是，把 stderr/stdout 的输出重定向到文件。
stderr/stdout重定向到文件
这里以stderr代码说明。
#include<stdio.h>
#include<stdlib.h>
intmain(  )
{
 FILE *stream = freopen("freopen.out", "w", stderr );
 if( stream == NULL )
    fprintf( stdout, "error onfreopen\n" );
 else
{
    fprintf( stdout, "successfullyreassigned\n" ); fflush( stdout );
    fprintf(stream, "This will [Go](http://lib.csdn.net/base/go) to the file 'freopen.out'\n" );
    fprintf( stderr, "Also you can do it likethis!\n" );
    fclose(stream );
 }
 // windwos下读取文件 freopen.out
 system( "type freopen.out" );
 getchar();
 return 0;
}
执行结果如下，
stderr与stdout的区别
stdout（标准输出），输出方式是行缓冲。输出的字符会先存放在缓冲区，等按下回车键时才进行实际的I/O操作。
stderr（标准出错），是不带缓冲的，这使得出错信息可以直接尽快地显示出来。
关于缓冲的说明：
关于缓冲的说明：
|类型|说明|输出情况|
|----|----|----|
|满缓冲|I/O操作只有在缓冲区被填满之后才会进行|1.缓冲区满2.刷出数据 （fflush）3.关闭文件 （fclose）|
|行缓冲|通常只有遇到换行符时，才会执行实际的I/O操作；但缓冲区满也会强制执行|1.遇到换行符2.缓冲区满3.刷出数据 （fflush）4.关闭文件 （fclose）|
|无缓冲|不缓存，直接进行I/O操作|直接输出|
然而就缓冲来说，stdout与stderr没有绝对的区别，因为缓冲类型可以设定。这里要借助setvbuf() 或setbuf() 函数。
#include<stdio.h> 
#include<stdlib.h>  
intmain() 
{ 
 char buf[512] = {0};
 setbuf(stderr, buf);
 fprintf(stderr, "It is error 1\n");
 printf("echo 1\n");
 fprintf(stderr, "It is error 2\n");
 printf("echo 2\n");
 fprintf(stderr, "It is error 3\n");
 fflush(stderr);
 getchar();
 return 0;
}
运行结果如下：
这样，我们就可以定义缓冲区大小。缓冲区默认大小由stdio.h 头文件中的宏 BUFSIZ定义，是512字节。另外，查阅一些资料说最小不能低于256字节，但[测试](http://lib.csdn.net/base/softwaretest)例子没有这个问题（暂时没有深究）。
setvbuf()与setbuf()
setvbuf()函数原型如下：
intsetvbuf ( FILE * stream, char * buffer, int mode, size_t size );
setbuf()可以当作是调用setvbuf(stream,buf, buf ? _IOFBF : _IONBF, BUFSIZE)；
其中， mode是声明缓冲的类型，如下几个：
|_IOFBF|满缓冲|
|----|----|
|_IOLBF|行缓冲|
|_IONBF|无缓冲|
size是缓冲区大小，单位字节。
/*setvbuf example */
#include<stdio.h>
intmain ()
{
  FILE *pFile=fopen("myfile.txt","w");
  setvbuf ( pFile , NULL , _IOFBF , 1024 );
  // File operations here
  fclose (pFile);
  return 0;
}
