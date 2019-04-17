# Linux tar解压缩命令详解 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月11日 15:37:54[boonya](https://me.csdn.net/boonya)阅读数：156








tar命令Windows和Linux操作都一样 ，但是zip不一定都支持，压缩命令用得最多的还是tar。

**目录**

[tar命令帮助](#tar%E5%91%BD%E4%BB%A4%E5%B8%AE%E5%8A%A9%E5%A6%82%E4%B8%8B)

[tar测试](#tar%E6%B5%8B%E8%AF%95)

[压缩](#%E5%8E%8B%E7%BC%A9%EF%BC%9A)

[解压](#%E8%A7%A3%E5%8E%8B%EF%BC%9A)

[查看内部文件](#%E6%9F%A5%E7%9C%8B%E5%86%85%E9%83%A8%E6%96%87%E4%BB%B6%EF%BC%9A)

## tar命令帮助

[root @ localhost /] #tar --help

用法：tar [选项...] [文件] ...

GNU`tar'将许多文件一起保存到单个磁带或磁盘存档中，并且可以从归档中恢复单个文件。

例子：

  tar -cf archive.tar foo bar＃从文件foo和bar创建archive.tar。

  tar -tvf archive.tar＃详细列出archive.tar中的所有文件。

  tar -xf archive.tar＃从archive.tar中提取所有文件。

 主要操作方式：

  -A， --catenate， --concatenate将tar文件附加到存档

  -c， --create创建一个新的存档

  -d， --dif， --compare存档和文件系统之间的差异

       --delete存档中的删除（不在磁带上！）

  -r， --append将文件追加到存档的末尾

  -t， --list列出存档的内容

      --test-label测试存档卷标并退出

  -u， --update仅附加比归档中的副本更新的文件

  -x， --extract， --get从存档中提取文件

 操作修饰符：

      --check-device在创建增量时检查设备号

                             档案（默认）

  -g， --list-incremental = FILE处理新的GNU格式增量备份

  -G， --incremental 增量处理旧的GNU格式增量备份

      --ignore-failed-read不会在不可读的文件上以非零退出

      --level =已创建的列出的增量存档的NUMBER转储级别

  -n， - 搜索档案是可以搜索的

      --no-check-device在创建时不检查设备号

                             增量档案

       - 不寻求存档是不可寻求的

      --occurrence [= NUM​​BER]仅处理每个文件的第NUMBER次出现

                             在档案中;此选项仅在

                             与其中一个子命令--delete结合使用，

                             --diff， - extract或--list以及列表时

                             文件在命令行或通过

                             -T选项; NUMBER默认为1

      --sparse-version= MAJOR [.MINOR]

                             设置要使用的稀疏格式的版本（暗示

                              - 疏）

  -S， --sparse有效处理稀疏文件

 覆盖控制：

  -k， --keep-old-files在提取时不替换现有文件，

                             把它们视为错误

      --keep-directory-symlink将现有符号链接保留到目录时

                             提取

      --keep-newer-files不会替换比new更新的现有文件

                             他们的档案副本

      --no-overwrite-dir保留现有目录的元数据

      --overwrite在提取时覆盖现有文件

      --overwrite-dir覆盖现有目录的元数据时

                             提取（默认）

       --recursive-unlink --recursive-unlink空层次结构

      --remove-files将文件添加到存档后删除文件

      --skip-old-files在提取时不替换现有文件，

                             默默地跳过它们

  -U，--unlink-first首先删除每个文件，然后再提取它

  -W，--verify 在写入后验证存档的尝试

选择输出流：

      --ignore-command-error忽略子代的退出代码

      --no-ignore-command-error将子项的非零退出代码视为

                             错误

  -O， --to-stdout  将文件提取到标准输出

      --to-command = COMMAND管道提取文件到另一个程序

 处理文件属性：

      --atime-preserve [= METHOD]保留转储文件的访问时间

                             通过阅读后恢复时间

                             （METHOD ='replace';默认）或不设置

                             首先是时间（METHOD ='system'）

      --delay-directory-restore延迟设置修改时间和

                             提取目录的权限直到结束

                             提取

      --group = NAME强制NAME作为添加文件的组

      --mode = CHANGES强制（符号）模式更改添加的文件

      --mtime = DATE-OR-FILE为来自DATE-OR-FILE的添加文件设置mtime

  -m， - touch不提取文件修改时间

      --no-delay-directory-restore

                             取消--delay-directory-restore的效果

                             选项

      --no-same-owner将文件提取为您自己（默认为普通文件）

                             用户）

      --no-same-permissions在提取权限时应用用户的umask

                             来自存档（普通用户的默认值）

      --numeric-owner始终使用用户/组名称的数字

      --owner = NAME强制NAME为添加文件的所有者

  -p， --preserve-permissions， --same-permissions

                             提取有关文件权限的信息

                             （超级用户默认）

      --preserve  保持与-p和-s相同

      --same-owner尝试提取具有相同所有权的文件

                             存档中存在（超级用户的默认值）

  -s， --preserve-order， --same-order

                             成员参数的列出顺序与

                             存档中的文件

 处理扩展文件属性：

      --acls启用POSIX ACL支持

      --no-acls禁用POSIX ACL支持

      --no-selinux禁用SELinux上下文支持

      --no-xattrs禁用扩展属性支持

      --selinux启用SELinux上下文支持

      --xattrs启用扩展属性支持

      --xattrs-exclude = MASK指定xattr键的排除模式

      --xattrs-include = MASK指定xattr键的包含模式

 设备选择和切换：

  -f， --file = ARCHIVE 使用存档文件或设备存档

      --force-local存档文件即使有冒号也是本地文件

  -F， --info-script = NAME， --new-volume-script = NAME

                             在每个磁带的末尾运行脚本（暗示-M）

  写入NUMBER x 1024字节后，-L，--tape-length = NUM​​BER更改磁带

  -M， --multi-volume  多卷创建/列表/提取多卷存档

      --rmt-command =COMMAND 使用给定rmt COMMAND而不是rmt

      --rsh-command = COMMAND 使用远程COMMAND而不是rsh

      --volno-file = FILE使用/更新FILE中的卷号

 设备阻止：

  -b，--blocking-factor = BLOCKS BLOCKS每个记录512个字节

  -B， --read-full-records当我们读取时重新记录（对于4.2BSD管道）

  -i， --ignore-zeros 忽略归档中的归零块（表示EOF）

       --record-size=NUMBER 每条记录的NUMBER个字节数，512的倍数

 存档格式选择：

  -H， --format = FORMAT 创建给定格式的存档

FORMAT是以下之一：

    gnu GNU tar 1.13.x格式

    oldgnu GNU格式，根据tar <= 1.12

    pax POSIX 1003.1-2001（pax）格式

    posix与pax相同

    ustar POSIX 1003.1-1988（ustar）格式

    v7旧的V7 tar格式

      --old-archive， - 可移植性

                             与--format = v7相同

      --pax选项=关键字[[：] =值] [，关键字[[：] =值]] ...

                             控制pax关键字

      --posix与--format = posix相同

  -V， --label = TEXT使用卷名TEXT创建存档;在

                             列表/提取时间，使用TEXT作为通配模式

                             用于卷名

 压缩选项：

  -a， --auto-compress 使用archive后缀来确定压缩

                             程序

  -I， --use-compress-program = PROG

                             通过PROG过滤（必须接受-d）

  -j, --bzip2  通过bzip2过滤存档

  -J， -- xz 通过xz过滤存档

      --lzip 通过lzip过滤存档

      --lzma通过lzma过滤存档

      --lzop

      --no-auto-compress不使用归档后缀来确定

                             压缩程序

  -z， - gzip， - gunzip， - ungzip通过gzip过滤存档

  -Z， - compress， - uncompress通过compress过滤归档

 本地文件选择：

      --add-file = FILE 将给定的FILE添加到存档（如果名称有用则有用）

                             以破折号开头）

      --backup[=CONTROL] 删除前备份[= CONTROL]备份，选择版本CONTROL

  -C，--directory=DIR 更改到目录DIR

      --exclude = PATTERN排除文件，以PATTERN形式给出

      --exclude-backups排除备份和锁定文件

      --exclude-caches排除包含目录的内容

                             CACHEDIR.TAG，标记文件本身除外

      --exclude-caches-all排除包含CACHEDIR.TAG的目录

      --exclude-caches-under排除包含目录的所有内容

                             CACHEDIR.TAG

      --exclude-tag = FILE排除包含FILE的目录的内容，

                             除了FILE本身

      --exclude-tag-all = FILE排除包含FILE的目录

      --exclude-tag-under = FILE排除目录下的所有内容

                             包含文件

      --exclude-vcs排除版本控制系统目录

  -h，--deference遵循符号链接;存档并转储它们的文件

                             指向

      --hard-dereference 硬取消引用硬链接;存档并转储它们的文件

                             参考

  -K， --starting-file=MEMBER-NAME

                             阅读时，请在会员MEMBER-NAME开头

                             档案

      --newer-mtime = DATE仅比较数据更改的日期和时间

      --no-null禁用前一个--null选项的效果

      --no-recursion避免在目录中自动降序

      --no-unquote不要取消引用-T读取的文件名

      --null -T读取以null结尾的名称，禁用-C

  -N, --newer=DATE-OR-FILE, --after-date=DATE-OR-FILE

                             仅存储比DATE-OR-FILE更新的文件

       --one-file-system 创建存档时，one-file-system保留在本地文件系统中

  -P，--absolute-names  不会从文件名中删除前导`/

      --recursion递归到目录（默认）

      --suffix = 删除之前的STRING备份，覆盖通常的后缀（'〜'

                             除非被环境变量覆盖

                             SIMPLE_BACKUP_SUFFIX）

  -T, --files-from=FILE 获取要从FILE中提取或创建的名称

       --unquote  使用-T（默认）读取unquote unquote文件名

  -X， --exclude-from=FILE  排除FILE中列出的模式

文件名转换：

      --strip-components = NUM​​BER条带来自文件的NUMBER个主要组件

                             提取名称

      --transform = EXPRESSION， -- xform = EXPRESSION

                             使用sed替换EXPRESSION来转换文件

                             名

 文件名匹配选项（同时影响排除和包含模式）：

       --anchored 匹配文件名称开始

      --ignore-case忽略大小写

      --no-anchored 在任何`/'后都没有锚定模式匹配（默认为排除）

      --no-ignore-case区分大小写匹配（默认）

      --no-wildcards逐字字符串匹配

      --no-wildcards-match-slash通配符与`/'不匹配

      --wildcards使用通配符（默认）

      --wildcards-match-slash通配符匹配`/'（默认为排除）

 信息输出：

      --checkpoint [= NUM​​BER]  显示每NUMBERth条记录的进度消息                         （默认10）

      --checkpoint-action = ACTION 在每个检查点上执行ACTION

      --full-time  全时打印文件时间到其全分辨率

      --index-file = FILE  将详细输出发送到FILE

  -l， - check-links  打印消息，如果不是所有链接都被转储

      --no-quote-chars = STRING 禁用STRING中字符的引用

      --quote-chars = STRING 另外引用STRING中的字符

      --quoting-style = STYLE 设置名称引用样式;请参阅下面的有效STYLE值

  -R， - block-number显示每个消息的归档中的块编号


      --show-defaults 显示tar默认值

      --show-omitted-dirs 列出或解压缩时显示--dirs -dirs，列出每个目录与搜索条件不匹配

      --show-transformed-names， - show-stored-names                 转换后显示文件或存档名称

      --totals [= SIGNAL]在处理存档后打印总字节数;

                             带参数 - 打印总字节数

                             SIGNAL已交付;允许的信号是：SIGHUP，

                             SIGQUIT，SIGINT，SIGUSR1和SIGUSR2;名字

                             也不接受SIG前缀

      --utc   以UTC格式打印文件修改时间

  -v， - verbose  详细列出已处理的文件

      --warning = KEYWORD警告控制

  -w， - interactive， --confirmation            要求确认每项行动

 兼容性选项：

  -o  创建时，与--old-archive相同;什么时候

                             提取，与--no-same-owner相同

 其他选择：

   - ？， - help给出这个帮助列表

       -- restrict 严格禁用某些可能有害的选项

      --usage给出一个简短的使用信息

      --version打印程序版本

长选项的必需或可选参数也是必需的或可选的

对于任何相应的短期权。

备份后缀为“〜”，除非使用--suffix或SIMPLE_BACKUP_SUFFIX进行设置。

可以使用--backup或VERSION_CONTROL设置版本控制，值为：

  none，off  永远不会备份

  t，numbered 为编号备份

  nil，existing 现有编号的编号备份存在，否则简单

  never，simple 总是做简单的备份

--quoting-style选项的有效参数是：

  literal

  shell

  shell-always

  c

  c-maybe

  escape

  locale

  clocale

*此* tar默认为：

--format=gnu -f- -b20 --quoting-style=escape --rmt-command=/etc/rmt

--rsh-command=/usr/bin/ssh

## tar测试

### 压缩

```bash
tar -zcvf /rtvsweb-dev-installer.tar.gz ./*
```

### 解压

```bash
tar -zxvf /rtvsweb-dev-installer.tar.gz
```

### 查看内部文件

![](https://img-blog.csdn.net/2018101115323999?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





