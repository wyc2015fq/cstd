# C++中system函数的用法，有效配合控制台程序 - 战斗蜗牛的专栏 - CSDN博客





2012年11月03日 10:15:37[vbskj](https://me.csdn.net/vbskj)阅读数：3178








## C++ system函数用法（C）


int system( const char *command ); 

int _wsystem( const wchar_t *command ); 

command: Command to be executed 

sample: 

//system.c 

#include <process.h> 

void main( void ) 

{ 

system( "type system.c" ); 

} 

system函数 是可以调用一些DOS命令,比如 

system("cls");//清屏,等于在DOS上使用cls命令 

下面列出常用的DOS命令,都可以用system函数调用: 

ASSOC 显示或修改文件扩展名关联。 

AT 计划在计算机上运行的命令和程序。 

ATTRIB 显示或更改文件属性。 

BREAK 设置或清除扩展式 CTRL+C 检查。 

CACLS 显示或修改文件的访问控制列表(ACLs)。 

CALL 从另一个批处理程序调用这一个。 

CD 显示当前目录的名称或将其更改。 

CHCP 显示或设置活动代码页数。 

CHDIR 显示当前目录的名称或将其更改。 

CHKDSK 检查磁盘并显示状态报告。 

CHKNTFS 显示或修改启动时间磁盘检查。 

CLS 清除屏幕。 

CMD 打开另一个 Windows 命令解释程序窗口。 

COLOR 设置默认控制台前景和背景颜色。 

COMP 比较两个或两套文件的内容。 

COMPACT 显示或更改 NTFS 分区上文件的压缩。 

CONVERT 将 FAT 卷转换成 NTFS。您不能转换当前驱动器。 

COPY 将至少一个文件复制到另一个位置。 

DATE 显示或设置日期。 

DEL 删除至少一个文件。 

DIR 显示一个目录中的文件和子目录。 

DISKCOMP 比较两个软盘的内容。 

DISKCOPY 将一个软盘的内容复制到另一个软盘。 

DOSKEY 编辑命令行、调用 Windows 命令并创建宏。 

ECHO 显示消息，或将命令回显打开或关上。 

ENDLOCAL 结束批文件中环境更改的本地化。 

ERASE 删除至少一个文件。 

EXIT 退出 CMD.EXE 程序(命令解释程序)。 

FC 比较两个或两套文件，并显示不同处。 

FIND 在文件中搜索文字字符串。 

FINDSTR 在文件中搜索字符串。 

FOR 为一套文件中的每个文件运行一个指定的命令 

FORMAT 格式化磁盘，以便跟 Windows 使用。 

FTYPE 显示或修改用于文件扩展名关联的文件类型。 

GOTO 将 Windows 命令解释程序指向批处理程序中某个标明的行。 

GRAFTABL 启用 Windows 来以图像模式显示扩展字符集。 

HELP 提供 Windows 命令的帮助信息。 

IF 执行批处理程序中的条件性处理。 

LABEL 创建、更改或删除磁盘的卷标。 

MD 创建目录。 

MKDIR 创建目录。 

MODE 配置系统设备。 

MORE 一次显示一个结果屏幕。 

MOVE 将文件从一个目录移到另一个目录。 

PATH 显示或设置可执行文件的搜索路径。 

PAUSE 暂停批文件的处理并显示消息。 

POPD 还原 PUSHD 保存的当前目录的上一个值。 

PRINT 打印文本文件。 

PROMPT 更改 Windows 命令提示符。 

PUSHD 保存当前目录，然后对其进行更改。 

RD 删除目录。 

RECOVER 从有问题的磁盘恢复可读信息。 

REM 记录批文件或 CONFIG.SYS 中的注释。 

REN 重命名文件。 

RENAME 重命名文件。 

REPLACE 替换文件。 

RMDIR 删除目录。 

SET 显示、设置或删除 Windows 环境变量。 

SETLOCAL 开始批文件中环境更改的本地化。 

SHIFT 更换批文件中可替换参数的位置。 

SORT 对输入进行分类。 

START 启动另一个窗口来运行指定的程序或命令。 

SUBST 将路径跟一个驱动器号关联。 

TIME 显示或设置系统时间。 

TITLE 设置 CMD.EXE 会话的窗口标题。 

TREE 以图形模式显示驱动器或路径的目录结构。 

TYPE 显示文本文件的内容。 

VER 显示 Windows 版本。 

VERIFY 告诉 Windows 是否验证文件是否已正确写入磁盘。 

VOL 显示磁盘卷标和序列号。 

XCOPY 复制文件和目录树。



system（执行shell 命令） 

相关函数 fork，execve，waitpid，popen

表头文件 #include<stdlib.h>

定义函数 int system(const char * string);

函数说明 system()会调用fork()产生子进程，由子进程来调用/bin/sh-c string来执行参数string字符串所代表的命令，此命令执行完后随即返回原调用的进程。在调用system()期间SIGCHLD 信号会被暂时搁置，SIGINT和SIGQUIT 信号则会被忽略。

返回值 如果system()在调用/bin/sh时失败则返回127，其他失败原因返回-1。若参数string为空指针(NULL)，则返回非零值。如果system()调用成功则最后会返回执行shell命令后的返回值，但是此返回值也有可能为system()调用/bin/sh失败所返回的127，因此最好能再检查errno 来确认执行成功。

附加说明 在编写具有SUID/SGID权限的程序时请勿使用system()，system()会继承环境变量，通过环境变量可能会造成系统安全的问题。

范例

#include<stdlib.h>

main()

{

system(“ls -al /etc/passwd /etc/shadow”);

}




