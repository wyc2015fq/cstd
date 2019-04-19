# Linux C 读取文件夹下所有文件（包括子文件夹）的文件名 - maopig的专栏 - CSDN博客
2017年06月16日 14:43:41[maopig](https://me.csdn.net/maopig)阅读数：5769
## 本文：[http://www.cnblogs.com/xudong-bupt/p/3504442.html](http://www.cnblogs.com/xudong-bupt/p/3504442.html)
Linux C  下面读取文件夹要用到结构体struct dirent，在头#include <dirent.h>中，如下：
```
#include <dirent.h>
struct dirent
{
   long d_ino; /* inode number 索引节点号 */
   off_t d_off; /* offset to this dirent 在目录文件中的偏移 */
   unsigned short d_reclen; /* length of this d_name 文件名长 */
   unsigned char d_type; /* the type of d_name 文件类型 */
   char d_name [NAME_MAX+1]; /* file name (null-terminated) 文件名，最长255字符 */
}
```
其中d_type表明该文件的类型：文件(8)、目录(4)、链接文件(10)等。
下面程序，递归读取某文件夹及其子文件夹下所有文件名：
```
1 #include <stdio.h>
 2 #include <stdlib.h>
 3 #include <string.h>
 4 #include <dirent.h>
 5 #include <unistd.h>
 6 int readFileList(char *basePath)
 7 {
 8     DIR *dir;
 9     struct dirent *ptr;
10     char base[1000];
11 
12     if ((dir=opendir(basePath)) == NULL)
13     {
14         perror("Open dir error...");
15         exit(1);
16     }
17 
18     while ((ptr=readdir(dir)) != NULL)
19     {
20         if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)    ///current dir OR parrent dir
21             continue;
22         else if(ptr->d_type == 8)    ///file
23             printf("d_name:%s/%s\n",basePath,ptr->d_name);
24         else if(ptr->d_type == 10)    ///link file
25             printf("d_name:%s/%s\n",basePath,ptr->d_name);
26         else if(ptr->d_type == 4)    ///dir
27         {
28             memset(base,'\0',sizeof(base));
29             strcpy(base,basePath);
30             strcat(base,"/");
31             strcat(base,ptr->d_name);
32             readFileList(base);
33         }
34     }
35     closedir(dir);
36     return 1;
37 }
38 
39 int main(void)
40 {
41     DIR *dir;
42     char basePath[1000];
43 
44     ///get the current absoulte path
45     memset(basePath,'\0',sizeof(basePath));
46     getcwd(basePath, 999);
47     printf("the current dir is : %s\n",basePath);
48 
49     ///get the file list
50     memset(basePath,'\0',sizeof(basePath));
51     strcpy(basePath,"./XL");
52     readFileList(basePath);
53     return 0;
54 }
```
执行输出 ：
![](http://images.cnitblog.com/blog/466768/201401/04131222-fcabec1304f64f188bf2f945b41e1a7e.png)

