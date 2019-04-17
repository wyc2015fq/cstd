# 嵌入式linux串口编程（二） - DoubleLi - 博客园






/*com_writer.c*/
#include "uart_api.h"

int main(void)
{
 int fd;
 char buff[BUFFER_SIZE];
 if((fd=open_port(HOST_COM_PORT))<0)  /*打开串口*/
 {
  perror("open serial error");
  return 1;
 }
 if(set_com_config(fd,115200,8,'N',1)<0) /*配置串口*/
 {
  perror("set_com_config error");
  return 1;
 }
 do
 {
  printf("Input some words(enter 'quit' to exit):");
  memset(buff,0,BUFFER_SIZE);
  if(fgets(buff,BUFFER_SIZE,stdin)==NULL)
  {
   perror("fgets");
   break;
  }
  write(fd,buff,strlen(buff));
 }while(strncmp(buff,"quit",4));
 close(fd);
 return 0;

}



/*com_reader.c*/
#include "uart_api.h"
#define COM_TYPE GNR_COM
int main(void)
{
 int fd;
 char buff[BUFFER_SIZE];
 if((fd=open_port(TARGET_COM_PORT))<0)
 {
  perror("open serial error");
  return 1;
 }
 if(set_com_config(fd,115200,8,'N',1)<0) /*配置串口*/
 {
  perror("set_com_config error");
  return 1;
 }
 do
 {
  memset(buff,0,BUFFER_SIZE);
  if(read(fd,buff,BUFFER_SIZE)>0)
  {
   printf("the receive words are:%s",buff);
  }
 }while(strncmp(buff,"quit",4));
 close(fd);
 return 0;
}

/*uart_api.h*/


#include<errno.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <termios.h>
#include <fcntl.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#define HOST_COM_PORT 1
#define BUFFER_SIZE  4096
#define MAX_COM_NUM  5
#define TARGET_COM_PORT 1
extern int open_port(int com_port);
extern int set_com_config(int fd,int baud_rate, int data_bits,char parity,int stop_bits);









