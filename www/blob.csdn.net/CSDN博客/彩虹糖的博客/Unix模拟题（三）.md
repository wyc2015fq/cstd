# Unix模拟题（三） - 彩虹糖的博客 - CSDN博客





2018年06月25日 22:15:40[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：60标签：[Unix](https://so.csdn.net/so/search/s.do?q=Unix&t=blog)
个人分类：[Unix](https://blog.csdn.net/caozixuan98724/article/category/7630652)









全是问题的程序，别问我为什么，明天就考试了...

```cpp
#include <sys/types.h>
#include <errno.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
static
void search(int signo){
    DIR*dp;
    struct dirent* dirp;
    struct stat sb;
    if(signo==SIGQUIT){
        dp = opendir("/home/ubuntu/");
        while ((dirp = readdir(dp)) != NULL ){
           char file_name[50];
           sprintf(file_name,"%s%s","/home/ubuntu/",dirp->d_name);
           stat(file_name, &sb);

           if(S_ISREG(sb.st_mode))
               printf("yes, this is a regular file: ");
           printf("%s \n", dirp->d_name);
        }
    }
}
static
void move(int signo){
    if(signo==SIGINT){
        rename("test.c","test/test.c");
        printf("move file succcessfully!\n");
    }

}

int main(){
    signal(SIGQUIT,search);
    signal(SIGINT,move);
    int pid = fork();
    if(pid<0){
        printf("fork error\n");
    }
    else if(pid==0){
        printf("in child\n");
        printf("wait for signal\n");
        pause();
    }
    else{
        printf("in parent\n");
        printf("use kill\n");
        kill(pid,SIGINT);
        kill(pid,SIGQUIT);
    }
    return 0;
}
```




