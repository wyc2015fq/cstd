# fdopen和popen - Orisun - 博客园







# [fdopen和popen](https://www.cnblogs.com/zhangchaoyang/articles/2300853.html)





popen：让进程看起来像文件

```
#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    char buf[100];
    int i=0;
    fp=popen("who|sort","r");   //打开一个命令，因为是只读"r"，所以是要读取命令的输出
    while(fgets(buf,100,fp)!=NULL)
        printf("%3d %s",i++,buf);
    pclose(fp);
    return 0;
}
```

$ gcc popendemo.c -o popendemo
$ ./popendemo
 0 orisun pts/0 2011-12-25 09:16 (:0.0)
 1 orisun tty7 2011-12-25 09:15 (:0)

调用popen之后注意调用pclose，进程产生之后一定要等待进程退出，否则将成为僵局进程。而pclose中调用了wait函数来等待进程结束。

fdopen：让文件描述符像文件一样使用

fdopen和fopen类似，都返回一个FILE*，但fdopen使用文件描述符作为参数，而fopen使用文件名作为参数。在有些情况下确实不知道文件名，只知道文件描述符，这时可以用fdopen。在Linux上把设备和网络都当成文件来看待。在网络服务端会有这样的操作：

```
int sock_fd=accept(sock_id,NULL,NULL);　　　　//accept接收客户端的socket连接请求，并返回新的套接口描述符。与客户端收发数据时使用新的套接口描述符
int sock_fp=fdopen(sock_fd,"w");　　　　//根据套接口描述符，像文件一样打开套接口
fprintf(sock_fp,"message");　　　　//向套接口写入数据，等价于调用sendmsg向客户端发送数据
fclose(sock_fp);　　　　//关闭文件等价于关闭连接
```














