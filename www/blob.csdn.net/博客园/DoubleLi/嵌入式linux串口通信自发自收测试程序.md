# 嵌入式linux串口通信自发自收测试程序 - DoubleLi - 博客园






 /*串口自收自发程序主函数*/
#include"uart_api.h"
int main()
{
 int fd;
 char buff[BUFFER_SIZE];
 char buff2[]="Hello NUAA2440!\n";
 int nread,nwrite;
 if((fd=open_port(TARGET_COM_PORT))<0)
 {
  perror("open serial error");
  return 1;
 }
 printf("open ok!\n");
 if(set_com_config(fd,115200,8,'N',1)<0) /*配置串口*/
 {
  perror("set_com_config error");
  return 1;
 }
 printf(" set ok!\n");

 /*将缓冲区buff2中的数据写入到串口1中去*/
 nwrite=write(fd,buff2,sizeof(buff2));
 printf("nwrite=%d\n",nwrite);   /*打印写入的数据*/
 /*接下来就是读数据*/
 while(1)
 {
  if((nread = read(fd,buff,1024))>0)
  {
   buff[nread] = '\0';
   printf("\nrecv:%d\n",nread);
   printf("%s",buff);
  }
 }
 close(fd);
 return 0;

}













