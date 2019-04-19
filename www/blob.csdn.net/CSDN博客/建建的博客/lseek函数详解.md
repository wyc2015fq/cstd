# lseek函数详解 - 建建的博客 - CSDN博客
2017年08月12日 18:13:00[纪建](https://me.csdn.net/u013898698)阅读数：171
    所有打开的文件都有一个当前文件偏移量（current file offset），以下简称为 cfo。cfo 通常是一个非负整数，用于表明文件开始处到文件当前位置的字节数。读写操作通常开始于 cfo，并且使 cfo 增大，增量为读写的字节数。文件被打开时，cfo 会被初始化为 0，除非使用了 O_APPEND 。
    使用 lseek 函数可以改变文件的 cfo 。
        #include <unistd.h>
        off_t lseek(int filedes, off_t offset, int whence);
        返回值：新的偏移量（成功），-1（失败）
参数 offset 的含义取决于参数 whence：
    1. 如果 whence 是 SEEK_SET，文件偏移量将被设置为 offset。
    2. 如果 whence 是 SEEK_CUR，文件偏移量将被设置为 cfo 加上 offset，
       offset 可以为正也可以为负。
    3. 如果 whence 是 SEEK_END，文件偏移量将被设置为文件长度加上 offset，
       offset 可以为正也可以为负。
SEEK_SET、SEEK_CUR 和 SEEK_END 是 System V 引入的，在这之前使用的是 0、1 和 2。
    lseek 的以下用法返回当前的偏移量：
        off_t    currpos;
        currpos = lseek(fd, 0, SEEK_CUR);
这个技巧也可用于判断我们是否可以改变某个文件的偏移量。如果参数 fd（文件描述符）指定的是 pipe（管道）、FIFO 或者 socket，lseek 返回 -1 并且置 errno 为 ESPIPE。
    对于普通文件（regular file），cfo 是一个非负整数。但对于特殊设备，cfo 有可能是负数。因此，我们不能简单地测试 lseek 的返回值是否小于 0 来判断 lseek 成功与否，而应该测试 lseek 的返回值是否等于 -1 来判断 lseek 成功与否。
    lseek 仅将 cfo 保存于内核中，不会导致任何 I/O 操作。这个 cfo 将被用于之后的读写操作。
    如果 offset 比文件的当前长度更大，下一个写操作就会把文件“撑大（extend）”。这就是所谓的在文件里创造“空洞（hole）”。没有被实际写入文件的所有字节由重复的 0 表示。空洞是否占用硬盘空间是由文件系统（file system）决定的。
    以下程序创建一个有空洞的文件：
        /* Standard C header */
        #include <stdio.h>
        /* Unix header */
        #include <fcntl.h>
        #include <unistd.h>
        #include <sys/stat.h>
        char    buf1[] = "abcdefghij";
        char    buf2[] = "ABCDEFGHIJ";
        int main(void)
        {
            int     fd, size;
            if ((fd = creat("file.hole", S_IRUSR|S_IWUSR)) < 0)
            {
                printf("creat error\n");
                return -1;
            }
            size = sizeof buf1 - 1;
            if (write(fd, buf1, size) != size)
            {
                printf("buf1 write error\n");
                return -1;
            }
            /* offset now = 10 */
            if (lseek(fd, 16384, SEEK_SET) == -1)
            {
                printf("lseek error\n");
                return -1;
            }
            /* offset now = 16384 */
            size = sizeof buf2 - 1;
            if (write(fd, buf2, size) != size)
            {
                printf("buf2 write error\n");
                return -1;
            }
            /* offset now = 16394 */
            return 0;
        }
