# [GNU/Linux] Linux系统调用-文件操作（一） - Pangda NoSilly - CSDN博客

2017年07月20日 11:43:42[hepangda](https://me.csdn.net/hepangda)阅读数：145


> 
**POSIX 错误处理**

  POSIX函数若执行时发生错误，大部分函数都将设置全局变量errno，以下内容若未特别提及，均在执行失败时设置了errno。 

  各个errno变量可以取的常数值可以使用`man 3 errno`查看man手册得知。

# 打开文件

函数功能： 打开文件并返回文件描述符，其他功能视参数而定。

### 函数原型及解释

```
int open(const char *path, int oflag, ...);

//一般使用的两种形式：
int open(const char *path, int oflag);
int open(const char *path, int oflag, mode_t mode);
```
- 返回值：若执行成功则返回`该文件描述符`，失败则返回`-1`。
- `path`：要打开的文件位置，既可以是绝对路径也可以是相对路径。
- `oflag`：打开文件的方式。
- `mode`：仅当`oflag`设置了`O_CREAT`时有效，要创建的文件的权限。

### 参数

其中参数`oflag`可选项有：
|选项（以下五选一）|说明|
|----|----|
|`O_RDONLY`|只读打开|
|`O_WRONLY`|只写打开|
|`O_RDWR`|读写打开|
|`O_EXEC`|只执行打开|
|`O_SEARCH`|(仅对目录有意义)只搜索打开|
|选项（以下任选）|说明|
|----|----|
|`O_APPEND`|每次写文件时都追加到文件的尾部|
|`O_CLOEXEC`|将`FD_CLOEXEC`设置为文件描述符标志|
|`O_CREAT`|若该文件不存在则创建|
|`O_EXCL`|(指定了`O_CREAT`)若文件已存在则报错|
|`O_NOFOLLOW`|若文件是一个符号链接则报错|
|`O_SYNC`|对文件的修改将阻塞直到物理磁盘上同步为止|
|`O_DIRECTORY`|若文件不是一个目录则失败|
|`O_NONBLOCK`|以非阻塞的方式打开文件|
|`O_NDELAY`|以非阻塞方式打开文件|
|`O_TRUNC`|当文件存在并以可写方式打开则截断文件至0|

# 创建文件

函数功能：创建一个文件。

### 函数原型及解释
`int creat(const char *path, mode_t mode);`- 返回值：若执行成功则返回`该文件描述符`，失败则返回`-1`。
- `path`：要创建的文件名，这里可以使用绝对路径也可以使用相对路径。
- `mode`：要创建的文件的权限、类型。

# 关闭文件

函数功能：关闭文件描述符，同时将释放进程施加在该文件上的所有记录锁。

### 函数原型及解释
`int close(int fildes);`- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `fildes`：要关闭的文件描述符。

# 设置文件的权限

函数功能：设置文件的权限。

### 函数原型及解释

```
int chmod(const char *path, mode_t mode);
int fchmod(int fd, mode_t mode);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `path`：要设置权限的文件位置，既可以使用绝对路径也可以使用相对路径。
- `fd`：要设置权限的文件的文件描述符。
- `mode`：要设置的权限。

# 读文件

函数功能：从文件中读取一定的字节。

### 函数原型及解释
`ssize_t read(int fildes, void *buf, size_t nbyte);`- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `fildes`：要读取的文件的文件描述符。
- `buf`：存放读取出的内容的缓冲区。
- `nbyte`：要读取多少字节。

### 额外的说明

`read`和`write`是无缓冲的IO函数，使用时务必保证`buf`参数中的空间足够，而且与`nbyte`参数指定的字节数相匹配。另外`read`和`write`都是直接操作二进制内容，并不能“智能”的识别`buf`参数中的类型并转换，使用时务必注意。

# 写文件

函数功能：将指定的内容写入文件。

### 函数原型及解释
`ssize_t write(int fildes, const void *buf, size_t nbyte);`- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `fildes`：要写入的文件描述符。
- `buf`：从该位置中读取要写入的内容。
- `nbyte`：要写入多少字节。

# 读写指针的移动

函数功能：移动读写指针

### 函数原型及解释
`off_t lseek(int fildes, off_t offset, int whence);`- 返回值：若执行成功则返回`移动后距离文件首的字节数`，失败则返回`-1`。
- `fildes`：要移动读写指针的文件描述符。
- `offset`：将指针移动到对于参数`whence`表示的位置的偏移量。
- `whence`：表示要将指针移动到的位置的参考位置。

### 参数

`whence`参数的可选项有：
|选项|表示|
|----|----|
|`SEEK_SET`|表示`offset`参数是相对于**文件头**的偏移|
|`SEEK_CUR`|表示`offset`参数是相对于**当前位置**的偏移|
|`SEEK_END`|表示`offset`参数是相对于**文件尾**的偏移|

当选择`SEEK_END`并将`offset`取得正数值时，中间内容将以二进制`0`填充。

# 复制文件描述符

函数功能：复制一个文件描述符

### 函数原型及解释

```
int dup(int fildes);
int dup2(int fildes, int fildes2);
```
- 返回值：若执行成功则返回`产生的新文件描述符`，失败则返回`-1`。
- `fildes`：要复制的文件描述符。
- `fildes2`：指定复制后的新的文件描述符。若`fildes`等于`fildes2`，函数`dup2`将什么也不做。若`fildes2`已经被其他文件使用，则会先释放这个文件描述符。

# 文件控制

函数功能：视参数而定。

### 函数原型

```
int fcntl(int fildes, int cmd, ...);

//常见的使用形式：
int fcntl(int fildes, int cmd, long arg);
int fcntl(int fildes, int cmd, struct flock *lock);
```

### 各个功能的解释

`fcntl`函数根据`cmd`参数的不同有不同的功能，但都对`fildes`表示的文件描述符进行操作。 

 1.  `F_DUPFD`：此时`fcntl`函数的功能与`dup`函数的功能相同，返回值也相同。 

 2. `F_GETFD`：获取`fildes`表示的文件的`close_on_exec`标志值。若执行成功则返回`该标志值`，失败则返回`-1`。 

 3. `F_SETFD`：此时需要额外提供`arg`参数，设置`fildes`表示的文件的`close_on_exec`标志值，设为`arg`参数的最后一位。若执行成功则返回`0`，失败则返回`-1`。 

 4. `F_GETFL`：获得`fildes`表示的文件的打开方式（即`open`函数的`oflag`参数）。若执行成功则返回`0`，失败则返回`-1`。 

 5. `F_SETFL`：设置`fildes`表示的文件的打开方式，这里只可以设置`O_APPEND`、`O_NONBLOCK`和`O_ASYNC`三种属性。 

 6. `F_SETLK`：以下三个功能需要额外提供`lock`参数。对`fildes`表示的文件加锁或释放锁。若执行成功则返回`0`，失败则返回`-1`。 

 7. `F_SETLKW`：与`F_SETLK`功能基本相同，区别是若设置了这个标志，在文件被使用时将等待文件被释放。 

 8. `F_GETLK`：试探`fildes`表示的文件是否可以设置为`lock`，但不会真的设置为该值。

# 设备控制

函数功能：控制流设备。

### 函数原型及解释
`int ioctl(int fildes, int request, ...);`- `fildes`：要控制的文件描述符。
- `request`：要进行的操作，视参数的不同还需要在这里添加其他参数。

