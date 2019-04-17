# Linux c 目录操作函数scandir - DoubleLi - 博客园






头文件
#include <dirent.h> 　　
函数定义：
int scandir(const char *dir,struct dirent **namelist,int (*filter)(const void *b),
                       int ( * compare )( const struct dirent **, const struct dirent ** ) );

int alphasort(const void *a, const void *b);
int versionsort(const void *a, const void *b);

函数scandir扫描dir目录下(不包括子目录)满足filter过滤模式的文件，返回的结果是compare函数经过排序的，并保存在 namelist中。注意namelist是通过malloc动态分配内存的,所以在使用时要注意释放内存。alphasort和versionsort 是使用到的两种排序的函数。 　　
当函数成功执行时返回找到匹配模式文件的个数，如果失败将返回-1。
eg:
#include <dirent.h> 　
int main() 　
{ 　　
     struct dirent **namelist;
 　　int n;
 　　n = scandir(".", &namelist, 0, alphasort);
 　　if (n < 0) 　
       　{ 　　
            perror("not found\n");
     　　} 　
   　else 　
    　{
    　　while(n--)
     　　{
        　　printf("%s\n", namelist[n]->d_name);
        　　free(namelist[n]);
     　　}
 　  　free(namelist);
  　　}
 }









