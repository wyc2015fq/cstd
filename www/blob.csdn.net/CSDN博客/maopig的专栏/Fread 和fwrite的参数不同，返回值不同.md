# Fread 和fwrite的参数不同，返回值不同 - maopig的专栏 - CSDN博客
2016年12月14日 13:38:17[maopig](https://me.csdn.net/maopig)阅读数：3187

# 函数fwrite
**功能**
C语言函数，向文件写入一个数据块  。
size_t fwrite(const void* buffer, size_t size, size_t count, FILE* stream);
注意：这个函数以二进制形式对文件进行操作，不局限于文本文件
返回值：返回实际写入的[数据块](http://baike.baidu.com.cn/view/702806.htm)数目
（1）buffer：是一个[指针](http://baike.baidu.com.cn/view/159417.htm)，对fwrite来说，是要输出数据的地址；
（2）size：要写入内容的单字节数；
（3）count:要进行写入size字节的[数据项](http://baike.baidu.com.cn/view/178581.htm)的个数；
（4）stream:目标[文件指针](http://baike.baidu.com.cn/view/5019859.htm)；
（5）返回实际写入的数据项个数count。

刚才编一个关于用C库函数实现的文件复制操作的代码时发生错误。错误的根本是想当然的以为fread函数的用法，对其理解不深刻。后来在网友帮助下才发现错误。
其实函数的用法可以通过Linux中的man来获得帮助。
比如fread.在终端键入
**[cpp]**[view plain](http://blog.csdn.net/abclixu123/article/details/8284680#)[copy](http://blog.csdn.net/abclixu123/article/details/8284680#)[print](http://blog.csdn.net/abclixu123/article/details/8284680#)[?](http://blog.csdn.net/abclixu123/article/details/8284680#)
- man 3 fread   
这是会出现下面的东西:
**[cpp]**[view plain](http://blog.csdn.net/abclixu123/article/details/8284680#)[copy](http://blog.csdn.net/abclixu123/article/details/8284680#)[print](http://blog.csdn.net/abclixu123/article/details/8284680#)[?](http://blog.csdn.net/abclixu123/article/details/8284680#)
- NAME  
-        fread, fwrite - binary stream input/output  
- 
- SYNOPSIS  
-        #include <stdio.h>
- 
- size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);  
- 
- size_t fwrite(constvoid *ptr, size_t size, size_t nmemb,  
- FILE *stream);  
- 
- DESCRIPTION  
-        The function fread() reads nmemb elements of data, each size bytes long, from the stream pointed to by stream, storing them at the location given by ptr.  
- 
-        The  function  fwrite()  writes  nmemb elements of data, each size bytes long, to the stream pointed to by stream, obtaining them from the location given by  
-        ptr.  
- 
-        For nonlocking counterparts, see unlocked_stdio(3).  
- 
- RETURN VALUE  
-        On success, fread() and fwrite() return the number of items read or written.  This number equals the number of bytes transferred only when size is 1.  If an  
-        error occurs, or the end of the file is reached, the return value is a short item count (or zero).  
- 
-        fread() does not distinguish between end-of-file and error, and callers must use feof(3) and ferror(3) to determine which occurred.  
重点放在对返回值。
可以看出，如果调用成功的话，函数会返回读到的元素个数。如果想返回实际读取的字节数的话只有当size=1，也就是第二个参数size为1时。如果发生读取错误的话，或者已经到达文件末尾，返回值是一个小的元素个数值或者是0.下面给出我的代码，正确实现文件拷贝的代码，以此分析。
**[cpp]**[view plain](http://blog.csdn.net/abclixu123/article/details/8284680#)[copy](http://blog.csdn.net/abclixu123/article/details/8284680#)[print](http://blog.csdn.net/abclixu123/article/details/8284680#)[?](http://blog.csdn.net/abclixu123/article/details/8284680#)
- #include <stdio.h>
- 
- #define BUFFER_SIZE  1024
- 
- int main(int argc, char *argv[])  
- {  
- FILE *from_fp, *to_fp;  
- int bytes_read, bytes_write;  
- char *ptr;  
- char buffer[BUFFER_SIZE];  
- 
- if(argc != 3)       //参数包括源文件名与目标文件名
-     {  
-         printf("Input failed!\n");  
- return 1;  
-     }  
- 
- if( (from_fp = fopen(argv[1],"r")) == NULL )    //以只读方式打开源文件名
-     {  
-         printf("File is not exist\n");  
- return 1;  
-     }  
- 
- if((to_fp = fopen(argv[2],"w+")) == NULL)      //打开第二个文件
-     {  
-         printf("Open file failed!\n");    
- return 1;  
-     }  
- 
- while(bytes_read = fread(buffer, 1, BUFFER_SIZE, from_fp))  //读取BUFFSIZE大小字节
-     {  
- if(bytes_read > 0)           //读取有效数据
-         {  
-             ptr = buffer;  
- while(bytes_write = fwrite(ptr, 1, bytes_read, to_fp))  //写数据到目标文件
-             {  
- if(bytes_write == bytes_read)           //写完    
- break;  
- elseif(bytes_write > 0)         //未写完
-                 {  
-                     ptr += bytes_write;  
-                     bytes_read -= bytes_write;  
-                 }  
-             }  
- if(bytes_write == 0)            //写错误
- break;  
-         }  
-     }  
- 
-     fclose(from_fp);  
-     fclose(to_fp);  
- 
- return 0;  
- }  
注意到我的fread和fwrite中的第二个参数size都是1，这样的话我返回值就是实际读取到的或写入的字节数。刚开始我写的程序不是这样的，我是
**[cpp]**[view plain](http://blog.csdn.net/abclixu123/article/details/8284680#)[copy](http://blog.csdn.net/abclixu123/article/details/8284680#)[print](http://blog.csdn.net/abclixu123/article/details/8284680#)[?](http://blog.csdn.net/abclixu123/article/details/8284680#)
- while(bytes_read = fread(buffer, BUFFER_SIZE, 1,from_fp))  
**[cpp]**[view plain](http://blog.csdn.net/abclixu123/article/details/8284680#)[copy](http://blog.csdn.net/abclixu123/article/details/8284680#)[print](http://blog.csdn.net/abclixu123/article/details/8284680#)[?](http://blog.csdn.net/abclixu123/article/details/8284680#)
- while(bytes_write = fwrite(ptr, bytes_read, 1, to_fp))  
这里第三个参数为1，换句话说，也就是说要读取1个元素（返回值为1），此元素中包含BUFFER_SIZE个字节，因为我的文件不满足这个条件，这样的元素值不存在。于是返回的值为0，这也是为什么我的文件数据没有复制到另一个文件的原因了，因为根本就没有执行这个循环中的代码。
另外一个需要注意到的问题是fread函数不能区分文件是否结尾和出错两种情况。所以必须使用ferror()和feof()函数来确定到底是哪种情况，所以关于文件复制还有下面另一种写法。代码如下：
**[cpp]**[view plain](http://blog.csdn.net/abclixu123/article/details/8284680#)[copy](http://blog.csdn.net/abclixu123/article/details/8284680#)[print](http://blog.csdn.net/abclixu123/article/details/8284680#)[?](http://blog.csdn.net/abclixu123/article/details/8284680#)
- #include <stdio.h>
- #include <string.h>
- 
- #define BUFFER_SIZE  1024
- 
- int main(int argc, char *argv[])  
- {  
- FILE *from_fp, *to_fp;  
- //int bytes_read, bytes_write;
- long file_len = 0;  
- char buffer[BUFFER_SIZE];  
- 
- if(argc != 3)       //参数包括源文件名与目标文件名
-     {  
-         printf("Input failed!\n");  
- return 1;  
-     }  
- 
- if( (from_fp = fopen(argv[1],"r")) == NULL )    //以只读方式打开源文件名
-     {  
-         printf("File is not exist\n");  
- return 1;  
-     }  
- 
- if((to_fp = fopen(argv[2],"w+")) == NULL)      //打开第二个文件
-     {  
-         printf("Open file failed!\n");    
- return 1;  
-     }  
- 
-     fseek(from_fp, 0L, SEEK_END);       //定位文件指针到尾部
-     file_len = ftell(from_fp);          //获得文件长度
-     fseek(from_fp, 0L, SEEK_SET);       //定位文件指针到开始处
- 
- while(!feof(from_fp))               //判断文件是否结束
-     {  
-         fread(buffer, BUFFER_SIZE, 1, from_fp);  
- if(BUFFER_SIZE > file_len)               //缓冲区长度大于文件长度
-             fwrite(buffer, file_len, 1, to_fp);  
- else
-         {  
-             fwrite(buffer, BUFFER_SIZE, 1, to_fp);  
-             file_len -= BUFFER_SIZE;  
-         }  
-         bzero(buffer,BUFFER_SIZE);      //清零缓冲区
-     }  
- 
-     fclose(from_fp);  
-     fclose(to_fp);  
- 
- return 0;  
- }  
这里面多了几个函数，其中feof就是用来检测文件是否结尾的函数。其返回值非0时表示文件结束。还有一个函数bzero是清零一段缓冲区的函数，使用需包含头文件string.h。注意fread和fwrite函数的书写就是采用第三个参数为1的方式来书写的了。
            
