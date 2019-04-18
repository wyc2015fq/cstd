# [GNU/Linux] Linux系统调用-文件操作（二） - Pangda NoSilly - CSDN博客

2017年07月20日 17:19:30[hepangda](https://me.csdn.net/hepangda)阅读数：127


# 获得文件属性

函数功能：获得文件的属性。

### 函数原型及解释

```
int stat(const char *restrict path, struct stat *restrict buf);
int fstat(int fildes, struct *buf);
int lstat(const char *restrict path, struct stat *restrict buf);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `path`：要获得文件属性的文件的路径。既可以是绝对路径也可以是相对路径。
- `fildes`：要控制的文件描述符。
- `buf`：获得的文件属性将存在`buf`参数所指的内存中。

# 设置文件的归属

函数功能：设置文件所属的用户和用户组。

### 函数原型及解释

```
int chown（const char *path, uid_t owner, gid_t group);
int fchown(int fildes, uid_t owner, gid_t group);
int lchown(const char *path, uid_t owner, gid_t group);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `path`：要设置的文件的路径。既可以是绝对路径也可以是相对路径。
- `fildes`：要设置的文件的文件描述符。
- `owner`：文件所属的用户，若设置为`-1`则不改变。
- `group`：文件所属的用户组，若设置为`-1`则不改变。

# 改变文件长度

函数功能：截断或填充文件使其达到指定长度。

### 函数原型及解释

```
int truncate(const char *path, off_t length);
int ftruncate(int fildes, off_t length);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `path`：要改变长度的文件的路径。既可以是绝对路径也可以是相对路径。
- `fildes`：要改变长度的文件的文件描述符。
- `length`：新的文件长度。若比原来的长度短，则截断文件，若比原来的长度长，则以二进制`0`填充。

# 改变文件修改时间

函数功能：改变文件的访问时间和更改时间。

### 函数原型及解释

```
int utime(const char *path, const struct utimbuf *times);
int utimes(const char *path, const struct timeval times[2]);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `path`：要改变修改时间的文件的路径。既可以是绝对路径也可以是相对路径。
- `times`：新的访问时间和更改时间。若指定为`NULL`则不改变。
- `times[2]`：新的访问时间和更改时间。其中times[0]为访问时间，times[1]为更改时间。

# 设置屏蔽字

函数功能：设置文件的默认权限屏蔽字。

### 函数原型及解释
`int umask(mode_t mask);`- 返回值：返回`原来的屏蔽字`。
- `mask`：要设置的新的屏蔽字。

# 移动/删除文件

函数功能：删除/移动文件。

### 函数原型及解释

```
int rename(const char *old, const char *new);
int unlink(const char *path);
int remove(const char *path);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `old`：文件的新路径。
- `new`：文件的新路径。若新旧路径在同一目录下，可以认为是在重命名。
- `path`：要删除的文件路径。既可以是绝对路径也可以是相对路径。

# 创建/删除目录

函数功能：创建/删除目录。

### 函数原型及解释

```
int mkdir(const char *path, mode_t mode);
int rmdir(const char *path);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `old`：文件的新路径。
- `new`：文件的新路径。若新旧路径在同一目录下，可以认为是在重命名。
- `path`：要创建/删除的文件夹的路径。既可以是绝对路径也可以是相对路径。

# 获得当前目录

函数功能：获得当前工作目录。

### 函数原型及解释

```
char *getcwd(char *buf, size_t size);
 char *getwd(char *buf);
 char *get_current_dir_name(void);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `buf`：用于存储当前工作目录。
- `size`：`buf`参数的大小。若指定为`NULL`，将自动分配。

# 设置当前目录

函数功能：设置当前工作目录。

### 函数原型及解释

```
int chdir(const char *path);
int fchdir(int fildes);
```
- 返回值：若执行成功则返回`0`，失败则返回`-1`。
- `path`：要设置为工作目录的路径。既可以是绝对路径也可以是相对路径。
- `fildes`：要设置为工作目录的文件描述符

# 目录操作

函数功能：打开一个目录；关闭目录；获取目录中的文件信息。

### 函数原型及解释

```
DIR *opendir(const char *name);
DIR *fdopendir(int fd);

int closedir(DIR *dirp);

struct dirent *readdir(DIR *dirp);
```
- 返回值：若成功则分别返回`打开的目录流`、`0`和`目录下的文件信息`，失败则分别返回`NULL`、`0`和`NULL`。
- `name`：要打开的目录路径。既可以是绝对路径也可以是相对路径。
- `fd`：要打开的目录的文件描述符。
- `dirp`：要关闭/读取的目录流。

