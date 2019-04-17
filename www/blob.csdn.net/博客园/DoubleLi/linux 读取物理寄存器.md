# linux 读取物理寄存器 - DoubleLi - 博客园








#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, void** argv) {
    void* base;
    int fd, reg;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        printf("ERR: device open failed.\n");
        return -1; 
    }   

    base = mmap(NULL, 0x1000, PROT_READ, MAP_SHARED, fd, 0x20E0000);
    if (base == 0) {
        printf("ERR: mmap failed.\n");
        close(fd);
        return -1; 
    }   

    if (argc > 1) {
        reg = strtol(argv[1], NULL, 16);
        printf(">>> %08X is %08X\n", 0x20E0000 + reg,  *((unsigned int*)(base + reg)));
    }   
    munmap(base, 0x1000);
    close(fd);
    return 0;
}











