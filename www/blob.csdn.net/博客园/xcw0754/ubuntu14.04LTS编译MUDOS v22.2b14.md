# ubuntu14.04LTS编译MUDOS v22.2b14 - xcw0754 - 博客园
# [ubuntu14.04LTS编译MUDOS v22.2b14](https://www.cnblogs.com/xcw0754/p/5121206.html)
附：[MudOS v22.2b14下载连接](http://files.cnblogs.com/files/xcw0754/v22.2b14.tar.gz)
**简单的编译步骤：**
（1）运行./build.MudOS
（2）vim socket_efuns.c
       o 修改Ln:1198的代码为 
       addr_in = (local ? &lpc_socks[fd].l_addr : &lpc_socks[fd].r_addr);
（3）make
（4）第三步执行之后的最后一行会出现：
英文系统：make: *** No rule to make target `obj/malloc.o', needed by `driver'.  Stop.
中文系统：make: *** 没有规则可以创建“driver”需要的目标“obj/malloc.o”。 停止。
解决方法：
　　原封不动再次make即可。详细可参考连接：http://bluesock.org/~willg/blog/dev/mudos_under_ubuntu.html。
到此编译已经完成，下面进行安装。
（5）make install 不会出错。
-----------下面的还没有尝试，仅修改如上的内容就能编译了，即下面的选项是可选的------------
下面几个文件添加stdlib.h去掉一些警告(找不到exit（）之类的)
vi edit_source.c
       添加 #include <stdlib.h>
vi preprocess.c
       添加 #include <stdlib.h>
vi make_finc.y
       添加 #include <stdlib.h>
修改一下option.h文件兼容老的lib
vi options.h
         添加巫师的功能
         修改 Ln：225 #undef NO_WIZARDS
         采用老的lpc语法（关于nosave - static之类的）
         修改 Ln：266 #undef SENSIBLE_MODIFIERS
         启用driver 的权限机制
         修改 Ln：659 #define PACKAGE_UIDS
   创建三个文件、否则会找不到  malloc.c 目标啥的  
    touch malloc.c
    touch mallocwrapper.c
    touch applies_table.c
    * 需要新建的三个文件是在make过程生成的，由于ubuntu下的make工具似乎不能识别make过程中的
      文件，所以需要新建三个空白文件，这样在make开始的时候就会识别到，当然在make过程中会复写这三个文件
    数据库部分、拷自nt2的driver 、（特此声明）
    由于以前的数据库api太老connect换成real_connect之类的
    添加数据库部分
    option.h修改下列配置
    #define PACKAGE_DB
    #ifdef PACKAGE_DB
    #undef USE_MSQL        /* MiniSQL, it's small; it's free */
    #define USE_MYSQL 2        /* MySQL, bigger; it's free */
    #define DEFAULT_DB USE_MYSQL    /* default database */
    #endif
    修改 db.h 头文件：
    将 24 行至 37 行注释掉，紧接着写上你自己的 MySQL 头文件的位置，
    例如在 /usr/local/mysql/include/mysql.h
    就写上：
    #include "/usr/local/mysql/include/mysql.h"
    在源代码目录里的 system_libs 文件里加上 mysql 连接库的位置，
    例如在 /usr/local/mysql/lib
    就加上：
    -L/usr/local/mysql/lib -lmysqlclient
   编译好的diver可能会遇到count（*）返零的问题
    MySQL_fetch对数据类型进行判断时、不包含Count（*）的数据类型
    最后default赋值为零
[font=楷体_gb2312;]    count（*）返回的数据了类型为FIELD_TYPE_LONGLONG[font=楷体_gb2312;]
    在db.c 650行左右添加[font=楷体_gb2312;] case FIELD_TYPE_LONGLONG:如下
switch (field->type) {
        case FIELD_TYPE_TINY:
        case FIELD_TYPE_SHORT:
        case FIELD_TYPE_DECIMAL:
        case FIELD_TYPE_LONG:
       case FIELD_TYPE_LONGLONG:
            v->item*.type = T_NUMBER;*
            v->item*.u.number = atoi(target_row*);**
            break;
        case FIELD_TYPE_FLOAT:
        case FIELD_TYPE_DOUBLE:
            v->item*.type = T_REAL;*
            v->item*.u.real = atof(target_row*);**
            break;
        case FIELD_TYPE_TINY_BLOB:
        case FIELD_TYPE_MEDIUM_BLOB:
        case FIELD_TYPE_LONG_BLOB:
        case FIELD_TYPE_BLOB:
        case FIELD_TYPE_STRING:
        case FIELD_TYPE_VAR_STRING:
                    if (field->flags & BINARY_FLAG) {
#ifndef NO_BUFFER_TYPE
                v->item*.type = T_BUFFER;*
                v->item*.u.buf = allocate_buffer(field->length);*
                write_buffer(v->item*.u.buf, 0, target_row*, field->length);**
#else
                        v->item* = const0u;*
#endif
                    } else {
                v->item*.type = T_STRING;*
                if (target_row*) {*
                v->item*.subtype = STRING_MALLOC;*
                v->item*.u.string = string_copy(target_row*, "MySQL_fetch");**
                } else {
                v->item*.subtype = STRING_CONSTANT;*
                v->item*.u.string = "";*
                }
                    }
            break;
        default:
            v->item* = const0u;*
            break;
    }
    最后是time（）不跑的问题
    在driver里面，有个变量current_time标示时间
    每次心跳时、会current_time++
    心跳的触发要看backend.c 186行左右 heart_beat_flag的标志变量
    heart_beat_flag为1才会出发
    heart_beat_flag 置1、是通过
    backend.c-> call_haert_beat()
     391行左右
    signal(SIGALRM, sigalrm_handler);
    ualarm(HEARTBEAT_INTERVAL, 0);
    到时触发函数sigalrm_handler（）heart_beat_flag 置1
     在v22.2b14中ualarm没有调用成功、没有设置时钟、就不触发函数sigalrm_handler（）
   心跳会停掉
    原因是ualarm.c文件#include "std.h"放的位置不正确
    因为 #ifndef HAS_UALARM 且 std.h里定义了HAS_UALARM 所以放到 #ifndef HAS_UALARM前
    就不会编译ualarm.c文件、就掉不到ualarm（）
    应该将#include "std.h"放到 #ifndef HAS_UALARM 之后

