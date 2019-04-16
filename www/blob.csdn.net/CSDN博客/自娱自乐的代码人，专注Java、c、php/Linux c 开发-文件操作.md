# Linux c 开发 - 文件操作 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月23日 13:39:01[initphp](https://me.csdn.net/initphp)阅读数：1260标签：[语言																[io																[c																[include																[file](https://so.csdn.net/so/search/s.do?q=file&t=blog)
个人分类：[Linux c 开发系列](https://blog.csdn.net/initphp/article/category/2609039)

所属专栏：[Linux c 开发系列](https://blog.csdn.net/column/details/mylinuxc.html)](https://so.csdn.net/so/search/s.do?q=include&t=blog)




介绍文件操作的基本用法。

### 常用API

#### 1. 打开文件fopen()



```cpp
FILE *fopen(const char *filename,const char *mode);
```
打开文件。参数filename指向要打开的文件名，mode表示打开状态的字符串。



mode表：



```
"r"
打开一个用于读取的文本文件
"w"
创建一个用于写入的文本文件
"a"
附加到一个文本文件
"rb"
打开一个用于读取的二进制文件
"wb"
创建一个用于写入的二进制文件
"ab"
附加到一个二进制文件
"r+"
打开一个用于读/写的文本文件
"w+"
创建一个用于读/写的文本文件
"a+"
打开一个用于读/写的文本文件
“rt”
只读打开一个文本文件，只允许读数据
“wt”
只写打开或建立一个文本文件，只允许写数据
“at”
追加打开一个文本文件，并在文件末尾写数据
“rb”
只读打开一个二进制文件，只允许读数据
“wb”
只写打开或建立一个二进制文件，只允许写数据
“ab”
追加打开一个二进制文件，并在文件末尾写数据
“rt+”
读写打开一个文本文件，允许读和写
“wt+”
读写打开或建立一个文本文件，允许读写
“at+”
读写打开一个文本文件，允许读，或在文件末追加数据
“rb+”
读写打开一个二进制文件，允许读和写
“wb+”
读写打开或建立一个二进制文件，允许读和写
“ab+”
读写打开一个二进制文件，允许读，或在文件末追加数据
```
例子：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {  
    FILE *file = fopen("test.vm", "r+"); //打开文件
    if (file) {
        puts("打开成功");
    } else {
        puts("打开失败");
    }
    fclose(file); //关闭文件
}
```




#### 2. 关闭文件fclose() 



```cpp
int fclose(FILE *fp);
```
fclose()的功能就是关闭用fopen()打开的文件。如果成功，返回0,失败返回EOF。在程序结束时一定要记得关闭打开的文件。



#### 3. 向流写一个字符fputc()

```cpp
int fputc(int c, FILE *stream);
```


成功返回这个字符,失败返回EOF。



#### 4. 向流读取一个字符fgetc()

```cpp
int fputc(FILE *stream);
```




#### 5. 位置移动 fseek()



```cpp
int fseek(FILE *stream, long offset, int whence);
```
如果成功返回0，参数offset是移动的字符数，whence是移动的基准取值是

符号常量 值 基准位置

SEEK_SET 0 文件开头

SEEK_CUR 1 当前读写的位置

SEEK_END 2 文件尾部 

例：fseek(fp,1234L,SEEK_CUR);//把读写位置从当前位置向后移动1234字节(L后缀表示长整数)

fseek(fp,0L,2);//把读写位置移动到文件尾






#### 6. 写一个字符串到流中fputs()



```cpp
int fputs(const char *s, FILE *stream);
```




#### 7. 读取一个流到字符串fgets()



```cpp
char *fgets(char *s, int n, FILE *stream);
```
从流中读取n-1个字符，除非读完一行，参数s是来接收字符串，如果成功则返回s的指针，否则返回NULL。

例：如果一个文件的当前位置的文本如下

Love ,I Have

But ........

如果用

fgets(str1,4,file1);

则执行后str1="Lov"，读取了4-1=3个字符，而如果用

fgets(str1,23,file1);

则执行str="Love ,I Have"，读取了一行(不包括行尾的'n')。






#### 8. 按格式输出fprintf()



```cpp
int fprintf(FILE *stream, const char *format[, argument, ...]);
```




#### 9. 按格式读取fscanf()



```cpp
int fscanf(FILE *stream, const char *format[, address, ...]);
```


#### 10. 判断文件结尾feof()



```cpp
int feof(FILE *stream);
```
检测是否已到文件尾，是返回真，否则返回0。例：if(feof(fp))printf("已到文件尾");





#### 11. 返回流最新的错误信息ferror()



```cpp
int ferror(FILE *stream);
```




#### 12. 文件指针恢复到文件开始 rewind()



```cpp
void rewind(FILE *stream);
```

把当前的读写位置回到文件开始。






#### 13. 删除文件remove() 



```cpp
int remove(const char *filename);
```
参数就是要删除的文件名，成功返回0。例：remove("c:io.sys");





#### 14. 读取数据fread()



```cpp
size_t fread(void *ptr, size_t size, size_t n, FILE *stream);
```
从流中读指定个数的字符，原型是size_t fread(void *ptr, size_t size, size_t n, FILE *stream);参数ptr是保存读取的数据，void*的指针可用任何类型的指针来替换，如char*、int *等等来替换；size是每块的字节数；n是读取的块数，如果成功，返回实际读取的块数(不是字节数)，本函数一般用于二进制模式打开的文件中。

例：

char x[4230];

FILE *file1=fopen("c:msdos.sys","r");

fread(x,200,12 ,file1);//共读取200*12=2400个字节





#### 15. 写入数据fwrite()



```cpp
size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream);
```
与fread对应，向流中写指定的数据，原型是size_t fwrite(const void *ptr, size_t size, size_t n, FILE *stream);参数ptr是要写入的数据指针，void*的指针可用任何类型的指针来替换，如char*、int *等等来替换；size是每块的字节数；n是要写的块数，如果成功，返回实际写入的块数(不是字节数)，本函数一般用于二进制模式打开的文件中。

例：

char x[]="I Love You";

fwire(x, 6,12,fp);//写入6*12=72字节

将把"I Love"写到流fp中12次，共72字节





#### 16. 生成临时文件 tmpfile()



```cpp
FILE *tmpfile(void);
```
生成一个临时文件，以"w+b"的模式打开，并返回这个临时流的指针，如果失败返回NULL。在程序结束时，这个文件会被自动删除。

例：FILE *fp=tmpfile();





#### 17. 生成唯一文件名 tmpnam();



```cpp
char *tmpnam(char *s);
```
生成一个唯一的文件名，其实tmpfile()就调用了此函数，参数s用来保存得到的文件名，并返回这个指针，如果失败，返回NULL。

例：tmpnam(str1);






### 例子



```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {  
    FILE *file = fopen("test.vm", "r+"); //打开文件
    if (file) {
        puts("打开成功");
    } else {
        puts("打开失败");
    }
    fputc('x', file); //写入一个字符
    fseek(file,3L, SEEK_SET); //移动到文件头部
    char x = fgetc(file); //读取一个字符


    if (x == EOF) {
        puts("读取失败");
    } else {
        puts("读取成功");
    }
    putc(x, stdout); //输出单个字符

    fseek(file,0L, SEEK_SET); //移动到文件头部
    //读取一行
    char *s = (char *) malloc(sizeof(char) * 1024);
    while (fgets(s, 1024, file) != NULL) {
        puts(s);
    }
    //写入一行
    char *z = "woshishen\n";
    fputs(z, file);

    if (feof(file) == 0) {
        puts("已经读取到文件EOF位置");
    }
    
    fclose(file); //关闭文件
}
```](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=io&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)




