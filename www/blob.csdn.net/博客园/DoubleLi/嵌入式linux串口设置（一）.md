# 嵌入式linux串口设置（一） - DoubleLi - 博客园






在linux中，所有的设备文件一般都位于“/dev”下，串口1和串口2对应的设备名依次为“/dev/ttyS0”, “/dev/ttyS1”,而且USB转串口的设备名通常为“/dev/ttyUSB0”，在linux下对设备的操作方法与对文件的操作方法一样。

串口开发步骤：

串口的设置主要是设置struct termios结构体的各成员值

#include<termios>

struct termios

{

       Unsigned short c_iflag;            /*输入模式标志*/

       Unsigned short c_oflag;           /*输出模式标志*/

       Unsigned short c_cflag;           /*控制模式标志*/

       Unsigned short c_lflag;            /*本地模式标志*/

       Unsigned char c_line ;             /*线路规则*/

       Unsigned char c_cc[NCC];              /*控制特性*/

      Speed_t        c_ispeed;             /*输入速度*/

     Speed_t        c_ospeed;            /*输出速度*/

};

串口设置包括波特率设置，检验位和停止位设置，主要设置的是c_cflag结构体成员，注意，不能直接对c_cflag成员初始化，而要将其通过与或操作。





int set_com_config(int fd,int baud_rate, int data_bits,char parity,int stop_bits)

{

       struct termios new_cfg,old_cfg;

       int speed;



       /*保存并测试现有串口参数设置，在这里如果串口号等出错，会有相关出错信息*/

       if(tcgetattr(fd,&old_cfg) != 0)        /*该函数得到fd指向的终端配置参数，并将它们保存到old_cfg变量中，成功返回0，否则-1*/

       {

              perror("tcgetttr");

              return -1;

       }



       /*设置字符大小*/

       new_cfg=old_cfg;

       cfmakeraw(&new_cfg);    /*配置为原始模式*/

       new_cfg.c_cflag &= ~CSIZE;/*用位掩码清空数据位的设置*/



       /*设置波特率*/

       switch(baud_rate)

       {

              case 2400:

                            speed=B2400;

                            break;

           case 4800:

                            speed=B4800;

                            break;

              case 9600:

                            speed=B9600;

                            break;   

              case 19200:

                            speed=B19200;

                            break;                        

              case 38400:

                            speed=B38400;

                            break;

              default:

              case 115200:

                            speed=B115200;

                            break;

       }

       cfsetispeed(&new_cfg,speed);

       cfsetospeed(&new_cfg,speed);



       /*设置停止位*/

       switch(data_bits)

       {

              case 7:

                            new_cfg.c_cflag |= CS7;

                            break;

              default:

              case 8:

                            new_cfg.c_cflag |= CS8;

                            break;

       }



       /*设置奇偶校验位*/

       switch(parity)

       {

              default:

              case 'n':

              case 'N':

              {

                     new_cfg.c_cflag &= ~PARENB;

                     new_cfg.c_iflag &= ~INPCK;

              }

              break;

              case 'o':

              case 'O':

              {

                     new_cfg.c_cflag |= (PARODD | PARENB);

                     new_cfg.c_iflag |= INPCK;

              }

              break;

              case 'e':

              case 'E':

              {

                     new_cfg.c_cflag |=  PARENB;

                     new_cfg.c_cflag &= ~PARODD;

                     new_cfg.c_iflag |= INPCK;

              }

              break;



       }



       /*设置停止位*/

       switch(stopbits)

       {

              default:

              case 1:

                            new_cfg.c_cflag &= ~CSTOPB;

                            break;

              case 2:

                            new_cfg.c_cflag |= CSTOPB;

                            break;

       }



       /*设置等待时间和最小接收字符*/

       new_cfg.c_cc[VTIME]=0;

       new_cfg.c_cc[VMIN]=1;



       /*处理未接收字符*/

       tcflush(fd,TCIFLUSH);



       /*激活新配置*/

       if((tcsetattr(fd,TCSANOW,&new_cfg))!=0)

       {

              perror("tcsetattr");

              return -1;

       }

       return 0;

}









