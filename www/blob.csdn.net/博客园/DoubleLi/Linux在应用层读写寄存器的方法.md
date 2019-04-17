# Linux在应用层读写寄存器的方法 - DoubleLi - 博客园






可以通过操作/dev/mem设备文件，以及mmap函数，将寄存器的地址映射到用户空间，直接在应用层对寄存器进行操作，示例如下：







**[cpp]**[view plain](http://blog.csdn.net/liukang325/article/details/26601811#)[copy](http://blog.csdn.net/liukang325/article/details/26601811#)

![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)![派生到我的代码片](https://code.csdn.net/assets/ico_fork.svg)

- #include <stdio.h>  
- #include <stdlib.h>  
- #include <time.h>  
- #include <unistd.h>  
- #include <fcntl.h>  
- #include <unistd.h>   
- #include <sys/mman.h>  
- #define AUDIO_REG_BASE   (0xB800E000)  
- #define MAP_SIZE        0xFF  
- 
- static int dev_fd;  
- int main(int argc, char **argv)  
- {  
- 
-     dev_fd = open("/dev/mem", O_RDWR | O_NDELAY);        
- 
- if (dev_fd < 0)    
-     {  
-         printf("open(/dev/mem) failed.");      
- return 0;  
-     }    
- 
-     unsigned char *map_base=(unsigned char * )mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, dev_fd, AUDIO_REG_BASE );  
- 
-     printf("%x \n", *(volatile unsigned int *)(map_base+0x38)); //打印该寄存器地址的value  
- 
- #if 1 // LINE IN  
-     printf("%x \n", *(volatile unsigned int *)(map_base+0x30));  
- 
-     *(volatile unsigned int *)(map_base + 0x30) = 0x208121bc; //修改该寄存器地址的value  
-     usleep(1000000);  
-     *(volatile unsigned int *)(map_base + 0x30) &= ~(0x1<<16); //修改该寄存器地址的value  
-     usleep(1000000);  
- 
-     printf("%x \n", *(volatile unsigned int *)(map_base+0x30));  
- #endif  
- 
- if(dev_fd)  
-         close(dev_fd);  
- 
-     munmap(map_base,MAP_SIZE);//解除映射关系  
- 
- return 0;  
- }  










