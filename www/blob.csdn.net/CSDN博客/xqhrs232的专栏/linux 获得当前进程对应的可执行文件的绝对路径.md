# linux 获得当前进程对应的可执行文件的绝对路径 - xqhrs232的专栏 - CSDN博客
2016年05月23日 11:36:08[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：733
原文地址::[http://www.cnblogs.com/xuxm2007/archive/2012/04/08/2437954.html](http://www.cnblogs.com/xuxm2007/archive/2012/04/08/2437954.html)
先说在应用层怎么搞这个事，很简单：
 #include <stdio.h>
```
#include <unistd.h>
int main() {
        char link[100], path[100];
        sprintf(link, "/proc/%d/exe", getpid());/////////////
        readlink(link, path, sizeof(path));//////////////
        printf("%s/n", path);
        return 0;
}
```
或者 
 #include <stdio.h>
```
#include <unistd.h>
char * get_exe_path( char * buf, int count)
{
    int i;
    int rslt = readlink("/proc/self/exe", buf, count - 1);/////注意这里使用的是self
    if (rslt < 0 || (rslt >= count - 1))
    {
        return NULL;
    }
    buf[rslt] = '\0';
    for (i = rslt; i >= 0; i--)
    {
        printf("buf[%d] %c\n", i, buf[i]);
        if (buf[i] == '/')
        {
            buf[i + 1] = '\0';
            break;
        }
    }
    return buf;
}
int main(int argc, char ** argv)
{
    char path[1024];
    printf("%s\n", get_exe_path(path, 1024));
    return 0;
}
```
在内核层，当前进程的所有信息都包含在current中。current里边有几个相关的变量：
1、current->comm:是一个16字节大小的char数组，记录的是当前进程的段路径。就是说，如果当前进程的可执行文件的全路径是/home/yaog/test,那么comm的内容就是“test”，但是由comm是不能得到全路径的。
2、current->mm/current->active_mm:这两个变量时mm_struct结构的，mm_struct用于描述虚拟内存。而current->mm
/current->active_mm就是用于描述当前进程所在的页面信息的。这个看上去有点儿靠谱。果然顺藤摸瓜：
#if LINUX_VERSION_CODE >= 0x020616
```
p->mm->mmap->vm_file->f_path.dentry->d_name.name
#else
p->mm->mmap->vm_file->f_dentry->d_name.name
#endif
```
#if LINUX_VERSION_CODE >=0x020616
```
p->mm->mmap->vm_file->f_path.dentry->d_parent
#else
p->mm->mmap->vm_file->f_dentry->d_parent
#endif
```
得到父亲节点，就是“/home/yaog/test”中的“yaog”，这样依次往上找。一直找到“/”为止。
这还不算完：这个可执行文件可能是被挂载上去的，还要得到它的挂载点信息：
```
```
filp = filp_open("/proc/(current->pid)/exe",0,0)
```
```
得到这个filp之后，再按照2中的方法找一遍，就OK了
//===================================================================================================================================
备注::
1>可以输入ps  -ax来查到所有程序的PID，然后按上面的方法来找到各个应用程序的绝对路径！！！
