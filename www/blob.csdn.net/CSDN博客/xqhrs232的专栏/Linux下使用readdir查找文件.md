# Linux下使用readdir查找文件 - xqhrs232的专栏 - CSDN博客
2011年05月25日 15:50:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1004
原文地址::[http://xiaoxia.org/?p=1853](http://xiaoxia.org/?p=1853)
CSDN相关网帖::[http://topic.csdn.net/t/20020819/19/953531.html](http://topic.csdn.net/t/20020819/19/953531.html)(Linux下怎样遍历整个目录文件)
在Windows上一直使用_findfirst查找文件，以为是标准库里的，可以在Linux下直接使用。今晚试了一下失败了。
Linux下使用readdir来扫描一个目录下的项目。首先，需要包含头文件dirent.h。然后定义下面搜索需要的结构。
DIR* dir_info; //目录指针
struct dirent* dir_entry; //目录项信息指针
//打开一个待扫描的目录
dir_info = opendir("./plugins");
if( dir_info )
{ 
        //打开目录成功
while ( (dir_entry = readdir(dir_info)) != NULL)
{
             //忽略这两个特殊项目
             if(strcmp(dir_entry->d_name, "..")==0 || strcmp(dir_entry->d_name, ".")==0)
continue;
             //具体操作。。。
plugin_create( srv, dir_entry->d_name );
} // while
        //使用完毕，关闭目录指针。
        closedir(dir_info);
}
可惜，我没有找到有能够在windows和linux通用的文件扫描函数。所以只好用宏定义不同平台的处理方案。
#ifdef __WIN32__
...
#else
...
#endif
