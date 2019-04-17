# 问题：内存对齐、sizeof计算的字节数不对 - V__KING__的专栏 - CSDN博客





2017年12月26日 20:01:53[v__king__](https://me.csdn.net/V__KING__)阅读数：203








### 1. 脑补内存对齐：

百度[https://baike.baidu.com/item/%E5%86%85%E5%AD%98%E5%AF%B9%E9%BD%90/9537460?fr=aladdin](https://baike.baidu.com/item/%E5%86%85%E5%AD%98%E5%AF%B9%E9%BD%90/9537460?fr=aladdin)

### 2. 实验一下：

编译如下的c文件：使用命令`gcc main.c`

```cpp
#include <sys/types.h>

#pragma pack(1)
typedef struct _CmdRfidReset {
    //unsigned char a;
    unsigned short int b;
    unsigned char c;
    unsigned char d;
    unsigned char e;
    unsigned char f;
    unsigned char g;
    unsigned char reserved[8];
    unsigned short int crc;
} CmdRfidReset;
typedef CmdRfidReset tSCmdRfidReset;


int main(){

    struct _CmdRfidResetUpload{
        unsigned char a;             //this is different to tSCmdRfidReset
        unsigned short int b;
        unsigned char c;
        unsigned char d;
        //unsigned char e;   //this is different to tSCmdRfidReset
        unsigned char f;
        unsigned char g;
        unsigned char reserved[8];
        unsigned short int crc;
    } CmdRfidResetUpload;



    printf("sizeof CmdRfidResetUpload = %d\n", sizeof(CmdRfidResetUpload));

    printf("sizeof RfidReset= %d\n", sizeof(tSCmdRfidReset));

    return 0;
}
```



