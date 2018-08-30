/* 
        a.c 
*/  
#include <stdio.h>  
#include <stdlib.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <fcntl.h>  
#include <string.h>  
#include <io.h>  
#include <winsock2.h>  
  
int test_pipe_c(int argc,char* argv[])  
{  
    if(argc!=3)  
    {  
        printf("args error\n");  
        return -1;  
    }  
    int fdr,fdw;  
    fdr=open(argv[1],O_RDONLY);  
    if(-1==fdr)  
    {  
        perror("openr");  
        close(fdr);  
        return -1;  
    }  
    fdw=open(argv[2],O_WRONLY);  
    if(-1==fdw)  
    {  
        perror("openw");  
        close(fdw);  
        return -1;  
    }  
    int ret,retr,retw;  
    char buf[128];  
    fd_set readset;  
    while(1)  
    {  
        FD_ZERO(&readset);  
        FD_SET(fdr,&readset);  
        FD_SET(STDIN_FILENO,&readset);  
        ret=select(fdr+1,&readset,NULL,NULL,NULL);  
        if(ret>0)  
        {  
            if(FD_ISSET(fdr,&readset))  
            {  
                bzero(buf,sizeof(buf));  
                retr=read(fdr,buf,sizeof(buf));  
                if(-1==retr)  
                {  
                    perror("read");  
                    close(fdr);close(fdw);  
                    return -1;  
                }  
                printf("B:%s\n",buf);  
                if(!strcmp(buf,"bye")) break;  
            }  
            if(FD_ISSET(STDIN_FILENO,&readset))  
            {  
                bzero(buf,sizeof(buf));  
                if((retw=read(STDIN_FILENO,buf,sizeof(buf)))>0)  
                {  
                    write(fdw,buf,retw-1);  
                }  
                else  
                {  
                    write(fdw,"bye",3);  
                    break;  
                }  
            }  
        }  
    }  
    close(fdr);  
    close(fdw);  
    return 0;  
}  