# Unix模拟题（二） - 彩虹糖的博客 - CSDN博客





2018年06月23日 10:01:37[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：75








题目：遍历命令行输入的目录，找到目标文件，并在目标文件中追加写入hello world!

 使用方法： ./a.out dir_name file_name

考点：DIR类和结构体dirent的使用，opendir,readdir函数的使用，学会遍历目录,openat函数的使用，还有O_APPEND追加至文件末尾的使用。

源码：

```cpp
#include <sys/types.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
int
main(int argc, char **argv) {

        DIR *dp;
        struct dirent *dirp;

        if (argc != 3) {
                fprintf(stderr, "usage: %s dir_name file_name\n", argv[0]);
                exit(1);
        }

        if ((dp = opendir(argv[1])) == NULL ) {
                fprintf(stderr, "can't open '%s'\n", argv[1]);
                exit(1);
        }
        int flag =0;
        while ((dirp = readdir(dp)) != NULL ){
                if(strcmp(dirp->d_name,argv[2])==0) {
                    int fd = openat(dirfd(dp),argv[2],O_WRONLY|O_APPEND);
                    char * s = "hello,world!\n";
                    write(fd,s,strlen(s));
                    close(fd);
                    flag = 1;
                }
        }

        closedir(dp);
        if(flag==1){
            printf("find and write successfully!\n");
        }
        return(0);
}
```

输出结果：

![](https://img-blog.csdn.net/20180623100059769?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180623100111350?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nhb3ppeHVhbjk4NzI0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







