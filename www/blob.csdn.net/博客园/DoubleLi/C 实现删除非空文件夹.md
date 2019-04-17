# C 实现删除非空文件夹 - DoubleLi - 博客园







**[cpp]**[view plain](http://blog.csdn.net/blueln/article/details/8700299)[copy](http://blog.csdn.net/blueln/article/details/8700299)

[print](http://blog.csdn.net/blueln/article/details/8700299)[?](http://blog.csdn.net/blueln/article/details/8700299)

- /* 
- 文件名：   rd.c 
- 
- ---------------------------------------------------- 
-     c中提供的对文件夹操作的函数，只能对空文件夹进行 
- 删除，这使很多初学者在编码过程中产生许多困扰，我也 
- 很不爽这件事情，所以编写这个对非空文件夹进行删除的 
- 函数，仅供参考。 
- 
- 注意：本函数编写以VC6为依据，其中关于文件夹的操作函数 
-       与标准c有所区别。如VC6中的findclose可能需要用c 
-       中的closedir（）来代替。 
- ---------------------------------------------------- 
- 日期         程序员                       变更记录 
- 
- 2010.4.28    海总（掌门人号）           创建文件，编写函数 
- 
- 
- ---------------------------------------------------- 
- */  
- 
- 
- 
- 
- #include <stdio.h>  
- #include <io.h>  
- #include <string.h>  
- #include <direct.h>  
- 
- /* 
- 函数入口：文件夹的绝对路径 
-           const char*  dirPath 
- 
- 函数功能：删除该文件夹，包括其中所有的文件和文件夹 
- 
- 返回值：  0  删除  
-          -1  路径不对，或其它情况，没有执行删除操作 
- */  
- int  removeDir(const char*  dirPath)  
- {  
- 
- struct _finddata_t fb;   //查找相同属性文件的存储结构体  
- char  path[250];            
- long    handle;  
- int  resultone;  
- int   noFile;            //对系统隐藏文件的处理标记  
- 
-     noFile = 0;  
-     handle = 0;  
- 
- 
- //制作路径  
-     strcpy(path,dirPath);  
-     strcat (path,"/*");  
- 
-     handle = _findfirst(path,&fb);  
- //找到第一个匹配的文件  
- if (handle != 0)  
-     {  
- //当可以继续找到匹配的文件，继续执行  
- while (0 == _findnext(handle,&fb))  
-         {  
- //windows下，常有个系统文件，名为“..”,对它不做处理  
-             noFile = strcmp(fb.name,"..");  
- 
- if (0 != noFile)  
-             {  
- //制作完整路径  
-                 memset(path,0,sizeof(path));  
-                 strcpy(path,dirPath);  
-                 strcat(path,"/");  
-                 strcat (path,fb.name);  
- //属性值为16，则说明是文件夹，迭代  
- if (fb.attrib == 16)  
-                 {  
-                      removeDir(path);     
-                 }  
- //非文件夹的文件，直接删除。对文件属性值的情况没做详细调查，可能还有其他情况。  
- else  
-                 {  
-                     remove(path);  
-                 }  
-             }     
-         }  
- //关闭文件夹，只有关闭了才能删除。找这个函数找了很久，标准c中用的是closedir  
- //经验介绍：一般产生Handle的函数执行后，都要进行关闭的动作。  
-         _findclose(handle);  
-     }  
- //移除文件夹  
-         resultone = rmdir(dirPath);  
- return  resultone;  
- }  









