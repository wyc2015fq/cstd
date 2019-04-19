# LinuxI2C驱动--从两个访问eeprom的例子开始 - maopig的专栏 - CSDN博客
2017年02月23日 21:57:59[maopig](https://me.csdn.net/maopig)阅读数：3114
本小节介绍两个在linux应用层访问eeprom的方法，并给出示例代码方便大家理解。第一个方法是通过sysfs文件系统对eeprom进行访问，第二个方法是通过eeprom的设备文件进行访问。这两个方法分别对应了i2c设备驱动的两个不同的实现，在后面的小结会详细的分析。
我们嵌入式系统中的E2PROM 是 24C02.先简单了解一下这款芯片：
AT24C02的存储容量为2Kb，内容分成32页，每页8B，共256B，操作时有两种寻址方式：芯片寻址和片内子地址寻址。 
　　（1）芯片寻址：AT24C02的芯片地址为1010，其地址控制字格式为 1010A2A1A0R/W。其中A2，A1，A0可编程地址选择位。A2，A1，A0引脚接高、
低电平后得到确定的三位编码，与1010形成7位编码， 即为该器件的地址码。R/W为芯片读写控制位，该位为0，表示芯片进行写操作。 
　　（2）片内子地址寻址：芯片寻址可对内部256B中的任一个进行读/写操作，其寻址范围为00~FF，共256个寻址单位。
## 1. 通过sysfs文件系统访问I2C设备
eeprom的设备驱动在/sys/bus/i2c/devices/0-0050/目录下把eeprom设备映射为一个二进制节点，文件名为eeprom。对这个eeprom文件的读写就是对eeprom进行读写。
我们可以先用cat命令来看下eeprom的内容。
```
[root@FORLINX210]# cat eeprom                                                                      
�����������X�����������������������������������������������
```
- 1
- 2
- 1
- 2
发现里面都是乱码，然后用echo命令把字符串“test”输入给eeprom文件，然后再cat出来。
```
[root@FORLINX210]# echo "test" > eeprom  
[root@FORLINX210]# cat eeprom                                                 
test                                                                            
�����������X�����������������������������������������������
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
就会发现字符串test已经存在eeprom里面了，我们知道sysfs文件系统断电后就没了，也无法对数据进行保存，为了验证确实把“test”字符串存储在了eeprom，可以把系统断电重启，然后cat eeprom，会发现test还是存在的，证明确实对eeprom进行了写入操作。
当然，因为eeprom已经映射为一个文件了，我们还可以通过文件I/O写应用程序对其进行简单的访问测试。比如以下程序对特定地址（0x40）写入特定数据（Hi,this is an eepromtest!），然后再把写入的数据在此地址上读出来。
```
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
int main(void){
    int fd, size, len, i;
    char buf[50]= {0};
    char *bufw="Hi,this is an eepromtest!";//要写入的数据
    len=strlen(bufw);//数据长度
    fd= open("/sys/bus/i2c/devices/0-0050/eeprom",O_RDWR);//打开文件
    if(fd< 0)
    {
        printf("####i2c test device open failed####/n");
        return(-1);
    }
    //写操作
    lseek(fd,0x40,SEEK_SET); //定位地址，地址是0x40
    if((size=write(fd,bufw, len))<0)//写入数据
    {
        printf("write error\n");
        return 1;
    }
    printf("writeok\n");
    //读操作
    lseek(fd,0x40, SEEK_SET);//准备读，首先定位地址，因为前面写入的时候更新了当前文件偏移量，所以这边需要重新定位到0x40.
    if((size=read(fd,buf,len))<0)//读数据
    {
        printf("readerror\n");
        return 1;
    }
    printf("readok\n");
    for(i=0; i< len; i++)
        printf("buff[%d]=%x\n",i, buf[i]);//打印数据
    close(fd);
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
## 2. 通过devfs访问I2C设备
linux的i2c驱动会针对每个i2c适配器在/dev/目录下生成一个主设备号为89的设备文件，简单的来说，对于本例的eeprom驱动，/dev/i2c/0就是它的设备文件，因此接下来的eeprom的访问就变为了对此设备文件的访问。
我们需要用到两个结构体i2c_msg和i2c_rdwr_ioctl_data。
```
struct i2c_msg { //i2c消息结构体，每个i2c消息对应一个结构体
     __u16 addr; /* 从设备地址，此处就是eeprom地址，即0x50 */
     __u16 flags;    /* 一些标志，比如i2c读等*/
     __u16 len;      /* i2c消息的长度 */
     __u8 *buf;      /* 指向i2c消息中的数据 */
 };
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 1
- 2
- 3
- 4
- 5
- 6
- 7
```
struct i2c_rdwr_ioctl_data {
     struct i2c_msg __user *msgs;    /* 指向一个i2c消息 */
     __u32 nmsgs;            /* i2c消息的数量 */
 };
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
对一个eeprom上的特定地址（0x10）写入特定数据（0x58）并在从此地址读出写入数据的示例程序如下所示。
```
#include <stdio.h>
#include <linux/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
int main()
{
    int fd,ret;
    struct i2c_rdwr_ioctl_data e2prom_data;
    fd=open("/dev/i2c/0",O_RDWR);//打开eeprom设备文件结点
    if(fd<0)
    {
        perror("open error");
    }
    e2prom_data.nmsgs=2; 
    e2prom_data.msgs=(struct i2c_msg*)malloc(e2prom_data.nmsgs*sizeof(struct i2c_msg));//分配空间
    if(!e2prom_data.msgs)
    {
        perror("malloc error");
        exit(1);
    }
    ioctl(fd,I2C_TIMEOUT,1);/*超时时间*/
    ioctl(fd,I2C_RETRIES,2);/*重复次数*/
    /*写eeprom*/
    e2prom_data.nmsgs=1;//由前面eeprom读写分析可知，写eeprom需要一条消息
    (e2prom_data.msgs[0]).len=2; //此消息的长度为2个字节，第一个字节是要写入数据的地址，第二个字节是要写入的数据
    (e2prom_data.msgs[0]).addr=0x50;//e2prom 设备地址
    (e2prom_data.msgs[0]).flags=0; //写
    (e2prom_data.msgs[0]).buf=(unsigned char*)malloc(2);
    (e2prom_data.msgs[0]).buf[0]=0x10;// e2prom 写入目标的地址
    (e2prom_data.msgs[0]).buf[1]=0x58;//写入的数据
    ret=ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);//通过ioctl进行实际写入操作，后面会详细分析
    if(ret<0)
    {
        perror("ioctl error1");
    }
    sleep(1);
    /*读eeprom*/
    e2prom_data.nmsgs=2;//读eeprom需要两条消息
    (e2prom_data.msgs[0]).len=1; //第一条消息实际是写eeprom，需要告诉eeprom需要读数据的地址，因此长度为1个字节
    (e2prom_data.msgs[0]).addr=0x50; // e2prom 设备地址
    (e2prom_data.msgs[0]).flags=0;//先是写
    (e2prom_data.msgs[0]).buf[0]=0x10;//e2prom上需要读的数据的地址
    (e2prom_data.msgs[1]).len=1;//第二条消息才是读eeprom，
    (e2prom_data.msgs[1]).addr=0x50;// e2prom 设备地址 
    (e2prom_data.msgs[1]).flags=I2C_M_RD;//然后是读
    (e2prom_data.msgs[1]).buf=(unsigned char*)malloc(1);//存放返回值的地址。
    (e2prom_data.msgs[1]).buf[0]=0;//初始化读缓冲，读到的数据放到此缓冲区
    ret=ioctl(fd,I2C_RDWR,(unsigned long)&e2prom_data);//通过ioctl进行实际的读操作
    if(ret<0)
    {
        perror("ioctl error2");
    }
    printf("buff[0]=%x\n",(e2prom_data.msgs[1]).buf[0]);
    /***打印读出的值，没错的话，就应该是前面写的0x58了***/
    close(fd);
    return 0;
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
## 3. 总结
本小节介绍了两种在linux应用层访问eeprom的方法，并且给出了示例程序，通过sysfs文件系统访问eeprom操作简单，无需了解eeprom的硬件特性以及访问时序，而通过devfs访问eeprom的方法则需要了解eeprom的读写时序。
后面分析后会发现，第一种通过sysfs文件系统的二进制结点访问eeprom的方法是由eeprom的设备驱动实现的，是一种专有的方法；而第二种通过devfs访问eeprom的方法是linux i2c提供的一种通用的方法，访问设备的能力有限。
