# Linux 文件查找工具之find“大宝剑” - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [JamesXin](http://www.jobbole.com/members/JamesXin) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
**一、文件查找工具常用软件**
**locate:**
locate命令其实是find -name的另一种写法，但是要比后者快得多，原因在于它不搜索具体目录，而是搜索一个数据库/var/lib/locatedb，这个数据库中含有本地所有文件信息。Linux系统自动创建这个数据库，并且每天自动更新一次，所以使用locate命令查不到最新变动过的文件。为了避免这种情况，可以在使用locate之前，先使用updatedb命令，手动更新数据库。
**find:**
find命令用来在指定目录下查找文件，通过遍历指定的目标目录，实时查找符合指定属性的文件。可以结合正则表达式（REGular EXPressio）来匹配模式。
**二、软件常用参数及使用方式**
**locate：**
使用格式：locate [OPTION]… PATTERN…
常用参数：
-i:忽略大小写
![wKiom1bgPWGTjPsyAABDnEiqYOI752.png](http://jbcdn2.b0.upaiyun.com/2016/05/83f4de3f98267c51ddd44027cbe4e683.png)
实例：
过滤/etc/下以sh开头的文件

Shell
```
# locate /etc/sh
/etc/shadow
/etc/shadow-
/etc/shells
```
**find：**    find [OPTIONS] [查找路径] [查找条件] [处理动作]
查找路径：默认为当前路径；
查找条件：默认为指定路径下的所有文件；
处理动作：默认为打印至屏幕；
**1、查找路径**
默认为当前路径，可以指定系统内的任意路径，但不建议使用/
**2、查找条件**
（1）基本查找条件
a、用户、用户组及文件名查找
**-name “文件名”:文件名支持使用globbing**
glob：文件名通配；快速引用多个文件；文件名整体匹配度检测；
元字符：基于元字符可编写匹配模式(pattern)；
*：匹配任意长度的任意字符；
?：匹配任意单个字符；
[ ]：匹配指定集合内的任意单个字符；
[a-z], [A-Z]：不区分字符大小写；
[0-9]
[a-z0-9]
[[:upper:]]：所有大写字母；
[[:lower:]]：所有小写字母；
[[:digit:]]：所有的数字；
[[:alpha:]]：所有字母；
[[:alnum:]]：所有字母和数字；
[[:space:]]：空白字符；
[[:punct:]]：标点符号；
[^ ]：匹配指定集合外的任意单个字符；
实例：


```
# find /etc -name "passwd*"
```
**-iname “文件名” 不区分大小写**
实例：


```
# find /tmp -iname "*yum*"
/tmp/yum.log
/tmp/YUM.LOG
# find /tmp -name "*yum*"
/tmp/yum.log
```
**-user USERNAME：根据属主查找**
**-group USERNAME：根据属组查找**
实例：

Shell
```
# find /var -user root -o -group mail
```
**-uid UID:根据指定UID查找**
**-gid GID：根据指定GID查找**
实例：

Shell
```
# find /tmp -uid 3323 -ls
2346806    0 -rw-rw-r--   1 mageedu  mageedu         0 Mar 10 09:36 /tmp/mageedu
2346812    0 -rw-rw-r--   1 mageedu  mageedu         0 Mar 10 09:36 /tmp/testfile
```
**-nouser:查找没有属主的文件**
**-nogroup：查找没有属组的文件需**
-nouser与-nogroup参数主要用在当公司员工离职后，虽然账户删除，但是他所有权限的文件还存在，并且以UID的形式来标识属主与属组，既当有uid有这个用户相同时就会可以查看此文件，所以需要删除此类文件。
实例：

Shell
```
# find /tmp -nouser -nogroup -ls
2346806    0 -rw-rw-r--   1 3323     3323            0 Mar 10 09:36 /tmp/mageedu
2346812    0 -rw-rw-r--   1 3323     3323            0 Mar 10 09:36 /tmp/testfile
```
b、文件类型查找
-type TYPE: 根据文件类型查找
f: 普通文件
d: 目录文件
l: 符号链接文件
b: 块设备
c: 字符设备文件
p: 命令名管道文件
s: 套接字文
实例：

Shell
```
# find /tmp -type d -ls
```
c、文件大小
-size [+|-]#UNIT：
常用单位有：k, M, G
#UNIT: #-1
2k指1k-2k
-#UNIT：x
-2k：指0k-1k
+#UNIT: x > #
+2k：指2k到正无穷
实例：

Shell
```
# find /etc/ -type f -size +1
```
d、时间戳查找
以“天”为单位
-atime [+|-]#
-mtime [+|-]#
-ctime [+|-]#
#: 例如：今天为3月10日，3就是3月7日到3月8日（注意天是24小时制）
-#: 例如：今天为3月10日，-3就是三天内指，3月7日的现在时间到现在的时间。
+#: 例如：今天为3月10日，+3就是三天前之前指,3月7日前
以“分钟”为单位
-amin [+|-]#
-mmin
-cmin
实例：

Shell
```
# find /etc -not -user root -not -user hadoop -mtime -7
# find / -nouser -nogroup -atime -3
```
e、权限查找
根据权限查找：
-perm [+|-]MODE
MODE: 与MODE精确匹配
+MODE：任何一类用户的权限只要能包含对其指定的任何一位权限即可；
-MODE：每一类用户的权限都包含对其指定的所有权限；
#这里是指包含，例如：-444 指在属主、属组、其他用户中只要包含读权限就可以。
实例：

Shell
```
# find /etc/init.d/ -perm -113  #此处指所有用户都有执行权限，其他用户有写权限的文件或目录
# find /etc/ -type f -not -perm +222 #此处指所有用户都没有写权限
```
（2）组合查找条件
组合查找条件：
与：-a, 查找条件1 -a 查找条件2 -a …
所有条件必须同时满足
或：-o, 查找条件1 -o 查找条件2 -o …
满足其中一个条件即可
非：-not, !
-not 查找条件
实例：

Shell
```
# find / ( -nouser -o -nogroup ) -a -atime -3
# find /etc/ -not ( -user root -o -user hadoop ) -a -mtime -7
```
**3、处理动作**
-print: 默认动作，打印至屏幕；
-ls: 显示找到的文件的详细属性；
-exec COMMAND {} ;
-ok COMMAND {} ;
#exec与ok的区别：ok会提供交互式，让你确认。而exec则不需要；
实例：

Shell
```
# find /tmp -iname "*yum*" -type f -ls
   134    0 -rw-r--r--   1 root     root            0 Mar 10 09:32 /tmp/yum.log
   137    0 -rw-r--r--   1 root     root            0 Mar 10 09:32 /tmp/YUM.LOG
# find /tmp -iname "*yum*" -type f -exec ls -l {} ;
-rw-r--r-- 1 root root 0 Mar 10 09:32 /tmp/yum.log
-rw-r--r-- 1 root root 0 Mar 10 09:32 /tmp/YUM.LOG
# find /tmp -iname "*yum*" -type f -exec echo {} >>/tmp/test.file ;
# find /tmp -iname "*yum*" -type f -ok echo {} >>/tmp/test.file \;
 ? y
 ? y
```
