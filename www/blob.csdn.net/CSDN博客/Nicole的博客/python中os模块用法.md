# python中os模块用法 - Nicole的博客 - CSDN博客
2018年04月17日 11:10:17[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：7196

一、os模块概述
Python os模块包含普遍的操作系统功能。如果你希望你的程序能够与平台无关的话，这个模块是尤为重要的。(一语中的)
二、常用方法
1、os.name
输出字符串指示正在使用的平台。如果是window 则用'nt'表示，对于Linux/Unix用户，它是'posix'。
2、os.getcwd()
**函数得到当前工作目录**，即当前Python脚本工作的目录路径。
3、os.listdir()
**返回指定目录下的所有文件和目录名。**
```
>>> os.listdir(os.getcwd())
['Django', 'DLLs', 'Doc', 'include', 'Lib', 'libs', 'LICENSE.txt', 'MySQL-python-wininst.log', 'NEWS.txt', 'PIL-wininst.log', 'python.exe', 'pythonw.exe', 'README.txt', 'RemoveMySQL-python.exe', 'RemovePIL.exe', 'Removesetuptools.exe', 'Scripts', 'setuptools-wininst.log', 'tcl', 'Tools', 'w9xpopen.exe']
>>>
```
4、os.remove()
删除一个文件。
5、os.system()
运行shell命令。
```
>>> os.system('dir')
0
>>> os.system('cmd') #启动dos
```
6、os.sep 可以取代操作系统特定的路径分割符。
7、os.linesep字符串给出当前平台使用的行终止符
```
>>> os.linesep
'\r\n'            #Windows使用'\r\n'，Linux使用'\n'而Mac使用'\r'。
>>> os.sep
'\\'              #Windows
>>>
```
8、os.path.split()
函数返回一个路径的目录名和文件名
```
>>> os.path.split('C:\\Python25\\abc.txt')
('C:\\Python25', 'abc.txt')
```
9、os.path.isfile()和os.path.isdir()函数分别检验给出的路径是一个文件还是目录。
```
>>> os.path.isdir(os.getcwd())
True
>>> os.path.isfile('a.txt')
False
```
10、**os.path.exists()函数用来检验给出的路径是否真的存在**
```
>>> os.path.exists('C:\\Python25\\abc.txt')
False
>>> os.path.exists('C:\\Python25')
True
>>>
```
11、os.path.abspath(name):获得绝对路径
12、os.path.normpath(path):规范path字符串形式
13、os.path.getsize(name):**获得文件大小，如果name是目录返回0L，如果那么为文件，则返回文件的字节数**
14、os.path.splitext():分离文件名与扩展名
```
>>> os.path.splitext('a.txt')
('a', '.txt')
```
15、os.path.join(path,name):连接目录与文件名或目录
```
>>> os.path.join('c:\\Python','a.txt')
'c:\\Python\\a.txt'
>>> os.path.join('c:\\Python','f1')
'c:\\Python\\f1'
>>>
```
16、os.path.basename(path):**返回文件名**
```
>>> os.path.basename('a.txt')
'a.txt'
>>> os.path.basename('c:\\Python\\a.txt')
'a.txt'
>>>
```
17、os.path.dirname(path):**返回文件路径**
```
>>> os.path.dirname('c:\\Python\\a.txt')
'c:\\Python'
```
II
os.error 
内建OSError exception的别名。 
os.name 
导入依赖操作系统模块的名字。下面是目前被注册的名字：'posix', 'nt', 'mac', 'os2', 'ce', 'java', 'riscos'. 
下面的function和data项是和当前的进程和用户有关 
```
>>> os.path.dirname('c:\\Python\\a.txt')
'c:\\Python'
```
os.environ   
一个mapping对象表示环境。例如，environ['HOME'] ，表示的你自己home文件夹的路径(某些平台支持，windows不支持) 
，它与C中的getenv("HOME")一致。 
这个mapping对象在os模块第一次导入时被创建，一般在python启动时，作为site.py处理过程的一部分。在这一次之后改变environment不 
影响os.environ，除非直接修改os.environ. 
注：putenv()不会直接改变os.environ，所以最好是修改os.environ 
注：在一些平台上，包括FreeBSD和Mac OS X，修改environ会导致内存泄露。参考 putenv()的系统文档。 
如果没有提供putenv()，mapping的修改版本传递给合适的创建过程函数，将导致子过程使用一个修改的environment。 
如果这个平台支持unsetenv()函数，你可以删除mapping中的项目。当从os.environ使用pop()或clear()删除一个项目时，unsetenv()会自动被调用（版本2.6）。 
os.chdir(path) 
os.fchdir(fd) 
os.getcwd() 
这些函数在 Files和 Directories中。 
os.ctermid() 
返回进程控制终端的文件名。在unix中有效，请查看相关文档.。 
os.getegid() 
返回当前进程有效的group的id。对应于当前进程的可执行文件的"set id "的bit位。在unix中有效，请查看相关文档.。 
os.geteuid() 
返回当前进程有效的user的id。在unix中有效，请查看相关文档.。 
os.getgid() 
返回当前进程当前group的id。在unix中有效，请查看相关文档.。 
os.getgroups() 
返回当前进程支持的groups的id列表。在unix中有效，请查看相关文档.。 
os.getlogin() 
返回进程控制终端登陆用户的名字。在大多情况下它比使用environment变量LOGNAME来得到用户名，或使用pwd.getpwuid(os.getuid())[0] 得到当前有效用户id的登陆名更为有效。在unix中有效，请查看相关文档.。 
os.getpgid(pid) 
返回pid进程的group id.如果pid为0,返回当前进程的group id。在unix中有效，请查看相关文档.。 
os.getpgrp() 
返回当前进程组的id.在unix中有效，请查看相关文档.。 
os.getpid() 
返回当前进程的id.在unix，Windows中有效。 
os.getppid() 
返回当前父进程的id.在unix中有效，请查看相关文档.。 
os.getuid() 
返回当前当前进程用户的id.在unix中有效，请查看相关文档.。 
os.getenv(varname[, value]) 
返回environment变量varname的值，如果value不存在，默认为None.在大多版本的unix，Windows中有效。 
os.putenv(varname, value) 
设置varname环境变量为value值。此改变影响以os.system(), popen() 或 fork()和execv()启动的子进程。在大多版本的unix，Windows中有效。 
当支持putenv()时，在os.environ分配项目时，自动调用合适的putenv()。然而，调用putenv() 不会更新os.environ，所以直接设置os.environ的项。 
os.setegid(egid) 
设置当前进程有效组的id.在unix中有效，请查看相关文档.。 
os.seteuid(euid) 
设置当前进程有效用户的id.在unix中有效，请查看相关文档.。 
os.setgid(gid) 
设置当前进程组的id.在unix中有效，请查看相关文档.。 
os.setgroups(groups) 
设置当前进程支持的groups id列表。groups必须是个列表，每个元素必须是个整数，这个操作只对超级用户有效，在unix中有效，请查看相关文档.。 
os.setpgrp() 
调用system的setpgrp()或setpgrp(0, 0)() ，依赖于使用的是哪个版本的system. 请查看 Unix手册. 在unix中有效，请查看相关文档.。 
os.setpgid(pid, pgrp) 
调用system的setpgid()设置pid进程group的id为pgrp.请查看 Unix手册. 在unix中有效，请查看相关文档.。 
os.setreuid(ruid, euid) 
设置当前process当前 和有效的用户id. 在unix中有效，请查看相关文档.。 
os.setregid(rgid, egid) 
设置当前process当前 和有效的组id. 在unix中有效，请查看相关文档.。 
os.getsid(pid) 
调用system的getsid(). 请查看 Unix手册. 在unix中有效，请查看相关文档.。 
os.setsid() 
调用system的setsid().请查看 Unix手册. 在unix中有效，请查看相关文档.。 
os.setuid(uid) 
设置当前user id. 在unix中有效，请查看相关文档.。 
os.strerror(code) 
返回程序中错误code的错误信息。在某些平台上，当给一个未知的code,strerror()返回NULL,将抛出ValueError。在unix，Windows中有效。 
os.umask(mask) 
设置当前权限掩码，同时返回先前的权限掩码。在unix，Windows中有效。 
os.fdopen(fd[, mode[, bufsize]]) 
返回一个文件描述符号为fd的打开的文件对象。mode和bufsize参数，和内建的open()函数是同一个意思。在unix，Windows中有效。 
mode必须以'r', 'w',或者'a'开头, 否则抛出ValueError. 
以'a'开头的mode, 文件描述符中O_APPEND位已设置. 
os.popen(command[, mode[, bufsize]]) 
给或从一个command打开一个管理。返回一个打开的连接到管道文件对象，文件对象可以读或写，在于模式是'r'(默认) 或'w'，bufsize参数，和内建的open()函数是同一个意思。 command返回的状态 (在wait()函数中编码) 和调用文件对象的close()返回值一样, 除非返回值是0(无错误终止), 返回None . 在unix，Windows中有效。 
在 2.6版本中已抛弃. 使用subprocess模块. 
os.tmpfile() 
返回一个打开的模式为(w+b)的文件对象 .这文件对象没有文件夹入口，没有文件描述符，将会自动删除. 在unix，Windows中有效。 
从 version 2.6起: 所有的popen*()函数已抛弃. 使用subprocess模块. 
os.popen2(cmd[, mode[, bufsize]]) 
os.popen3(cmd[, mode[, bufsize]]) 
os.popen4(cmd[, mode[, bufsize]]) 
16.1.3. 文件描述符操作 
这些函数操作使用文件描述符引用的I/O stream。 
文件描述符是与当前进程打开的文件相对应的一些小整数. 例如标准输入的通常文件描述符中 0, 标准输出是1,标准错误是 2. 进程打开的更多文件将被分配为3, 4, 5,等. 这“文件描述符”有一点迷惑性;在Unix平台上, socket和pipe 通常也使用文件描述符引用. 
os.close(fd) 
关闭文件描述符fd. 在unix，Windows中有效。 
这函数是为低层的I/O服务的，应用在os.open()或 pipe()返回的文件描述符上。关闭一个由内建函数open()或 popen()或fdopen()打开的文件对象，使用close()方法。 
os.closerange(fd_low, fd_high) 
关闭从fd_low（包含）到fd_high（不包含）所有的文件描述符，忽略错误。在unix，Windows中有效。 
等同于： 
for fd in xrange(fd_low, fd_high): 
    try: 
        os.close(fd) 
    except OSError: 
        pass 
os.dup(fd) 
返回文件描述符fd的cope. 在unix，Windows中有效。 
os.dup2(fd, fd2) 
复制文件描述符fd到fd2, 如果有需要首先关闭fd2. 在unix，Windows中有效。 
os.fchmod(fd, mode) 
改变文件描述符为fd的文件’mode‘为mode. 查看chmod()文档 中mode的值. 在unix中有效，请查看相关文档.。 
version 2.6中新增. 
os.fchown(fd, uid, gid) 
改变文件描述符为fd的文件的所有者和group的id为uid和gid. 如果不想它们中的一个, 就设置为-1. 在unix中有效，请查看相关文档.。 
version 2.6中新增. 
os.fdatasync(fd) 
强制将文件描述符为fd的文件写入硬盘. 不强制更新metadata. 在unix中有效，请查看相关文档.。 
注：在MacOS中无效. 
os.fpathconf(fd, name) 
返回一个打开的文件的系统配置信息。name为检索的系统配置的值，它也许是一个定义系统值的字符串，这些名字在很多标准中指定（POSIX.1, Unix 95, Unix 98, 和其它）。一些平台也定义了一些额外的名字。这些名字在主操作系统上pathconf_names的字典中。对于不在pathconf_names中的配置变量，传递一个数字作为名字，也是可以接受的。 在unix中有效，请查看相关文档.。 
如果name是一个字符串或者未知的，将抛出ValueError。如果name是一个特别的值，在系统上不支持，即使它包含在pathconf_names中，将会抛出错误数字为errno.EINVAL的OSError。 
os.fstat(fd) 
返回文件描述符fd的状态，像stat()。在unix，Windows中有效。 
os.fstatvfs(fd) 
返回包含文件描述符fd的文件的文件系统的信息，像 statvfs(). 在unix中有效，请查看相关文档.。 
os.fsync(fd) 
强制将文件描述符为fd的文件写入硬盘.在Unix, 将调用fsync()函数;在Windows, 调用 _commit()函数. 
如果你准备操作一个Python文件对象f, 首先f.flush(),然后os.fsync(f.fileno()), 确保与f相关的所有内存都写入了硬盘.在unix，Windows中有效。 
os.ftruncate(fd, length) 
裁剪文件描述符fd对应的文件, 所以它最大不能超过文件大小. 在unix中有效，请查看相关文档.。 
os.isatty(fd) 
如果文件描述符fd是打开的，同时与tty(-like)设备相连，则返回true, 否则False. 在unix中有效，请查看相关文档.。
os.lseek(fd, pos, how) 
设置文件描述符 fd当前位置为pos, how方式修改: SEEK_SET 或者 0 设置从文件开始的计算的pos; SEEK_CUR或者 1 则从当前位置计算; os.SEEK_END或者2则从文件尾部开始. 在unix，Windows中有效。 
os.open(file, flags[, mode]) 
打开file同时根据flags设置变量flags ，如果有mode，则设置它的mode. 默认的mode是 0777 (八进制), 当前掩码值是first masked out. 返回刚打开的文件描述符. 在unix，Windows中有效。 
flag和mode值, 请查看C运行时文档; flag常数(像O_RDONLY and O_WRONLY)在这个模块中也定义了（在下面）.
注:这函数是打算为低层 I/O服务的.正常的使用，使用内建函数open(),返回read()和 write() 等方法创建的文件对象.包装文件描述符为“文件对象”, 使用fdopen(). 
os.openpty() 
在一些Unix平台上有效，请查看相关文档. 
os.pipe() 
创建一个管道. 返回一对文件描述符(r, w) 分别为读和写. 在unix，Windows中有效。 
os.read(fd, n) 
从文件描述符 fd中读取最多n个字节. 返回包含读取字节的string. 文件描述符 fd对应文件已达到结尾, 返回一个空string. 在unix，Windows中有效。 
注:这函数是打算为低层 I/O服务的 ，同时必须应用在os.open()或者pipe()函数返回的文件描述符. 读取内建函数open()或者by popen()或者fdopen(),或者sys.stdin返回的一个“文件对象” , 使用它的read()或者readline()方法. 
os.tcgetpgrp(fd) 
在unix中有效，请查看相关文档.。 
os.tcsetpgrp(fd, pg) 
在unix中有效，请查看相关文档.。 
os.ttyname(fd) 
在unix中有效，请查看相关文档.。 
os.write(fd, str) 
写入字符串到文件描述符 fd中. 返回实际写入的字符串长度. 在unix，Windows中有效。 
注:这函数是打算为低层 I/O服务的 ，同时必须应用在os.open()或者pipe()函数返回的文件描述符. 读取内建函数open()或者by popen()或者fdopen(),或者sys.stdin返回的一个“文件对象” , 使用它的read()或者readline()方法. 
下面的常数是open()函数的 flags参数选项. 它们可以使用 bitwise合并或者operator |. 它们中的一些常数并不是在所有平台都有效. 它们更多使用请查看相关资料，在unix上参考open(2)手册页面，windows上http://msdn.microsoft.com/en-us/library/z0kc8e3z.aspx. 
os.O_RDONLY 
os.O_WRONLY 
os.O_RDWR 
os.O_APPEND 
os.O_CREAT 
os.O_EXCL 
os.O_TRUNC 
这些常数在Unix and Windows上有效. 
os.O_DSYNC 
os.O_RSYNC 
os.O_SYNC 
os.O_NDELAY 
os.O_NONBLOCK 
os.O_NOCTTY 
os.O_SHLOCK 
os.O_EXLOCK 
这些常数仅在Unix上有效. 
os.O_BINARY 
os.O_NOINHERIT 
os.O_SHORT_LIVED 
os.O_TEMPORARY 
os.O_RANDOM 
os.O_SEQUENTIAL 
os.O_TEXT 
这些常数仅在 Windows上有效. 
os.O_ASYNC 
os.O_DIRECT 
os.O_DIRECTORY 
os.O_NOFOLLOW 
os.O_NOATIME 
这些常数是 GNU扩展，如果没有在C库声明刚没有. 
os.SEEK_SET 
os.SEEK_CUR 
os.SEEK_END 
lseek()函数的参数. 它们的值分别是 0, 1,和 2. 在Unix and Windows上有效. 
版本2.5新增. 
16.1.4. 文件和文件夹 
os.access(path, mode) 
使用现在的uid/gid尝试访问path. 注大部分操作使用有效的uid/gid, 因此运行环境可以在 suid/sgid环境尝试，如果用户有权访问path. mode为F_OK，测试存在的path,或者它可以是包含R_OK, W_OK和X_OK或者R_OK, W_OK和X_OK其中之一或者更多.如果允许访问返回 True , 否则返回False. 查看Unix手册access(2) 获取更多信息. 在unix，Windows中有效。 
注:使用access()去测试用户是否授权. 在实际使用open()打开一个文件前测试会创建一个安全漏洞前 , 因为用户会利用这短暂时间在检测和打开这个文件去修改它. 
注:即使access()表明它将succeed，但I/O 操作也可能会失败, 如网络文件系统. 
os.F_OK 
作为access()的mode参数，测试path是否存在. 
os.R_OK 
包含在access()的mode参数中 ， 测试path是否可读. 
os.W_OK 
包含在access()的mode参数中 ，测试path是否可写. 
os.X_OK 
包含在access()的mode参数中 ，测试path是否可执行.. 
os.chdir(path) 
改变当前工作目录. 在unix，Windows中有效。 
os.fchdir(fd) 
在unix中有效，请查看相关文档.。 
os.getcwd() 
返回当前工作目录的字符串， 在unix，Windows中有效。 
os.getcwdu() 
返回一个当前工作目录的Unicode对象. 在unix，Windows中有效。 
os.chflags(path, flags) 
在unix中有效，请查看相关文档.。 
os.chroot(path) 
在unix中有效，请查看相关文档.。 
os.chmod(path, mode) 
改变path的mode到数字mode. mode为下面中的一个 (在stat模块中定义)或者bitwise或者它们的组合: 
?stat.S_ISUID 
?stat.S_ISGID 
?stat.S_ENFMT 
?stat.S_ISVTX 
?stat.S_IREAD 
?stat.S_IWRITE 
?stat.S_IEXEC 
?stat.S_IRWXU 
?stat.S_IRUSR 
?stat.S_IWUSR 
?stat.S_IXUSR 
?stat.S_IRWXG 
?stat.S_IRGRP 
?stat.S_IWGRP 
?stat.S_IXGRP 
?stat.S_IRWXO 
?stat.S_IROTH 
?stat.S_IWOTH 
?stat.S_IXOTH 
在unix，Windows中有效。 
注:尽管Windows支持chmod(), 你只可以使用它设置只读 flag (通过stat.S_IWRITE和stat.S_IREAD 常数或者一个相对应的整数)。所有其它的 bits都忽略了. 
os.chown(path, uid, gid) 
在unix中有效，请查看相关文档.。 
os.lchflags(path, flags) 
在unix中有效，请查看相关文档.。 
新增version 2.6. 
os.lchmod(path, mode) 
在unix中有效，请查看相关文档.。 
新增version 2.6. 
os.lchown(path, uid, gid) 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.link(source, link_name) 
在unix中有效，请查看相关文档.。 
os.listdir(path) 
返回path指定的文件夹包含的文件或文件夹的名字的列表. 这个列表以字母顺序. 它不包括 '.' 和'..' 即使它在文件夹中. 在unix，Windows中有效。 
Changed in version 2.3:在Windows NT/2k/XP 和Unix, 如果文件夹是一个Unicode object, 结果将是 Unicode objects列表. 不能解码的文件名将仍然作为string objects返回. 
os.lstat(path) 
像stat(),但是没有符号链接. 这是stat()的别名 在某些平台上,例如Windows. 
os.mkfifo(path[, mode]) 
在unix中有效，请查看相关文档.。 
os.mknod(filename[, mode=0600, device]) 
创建一个名为filename文件系统节点（文件，设备特别文件或者命名pipe）。 mode指定创建或使用节点的权限, 组合 (或者bitwise) stat.S_IFREG, stat.S_IFCHR, stat.S_IFBLK, 和stat.S_IFIFO (这些常数在stat模块). 对于 stat.S_IFCHR和stat.S_IFBLK, 设备定义了 最新创建的设备特殊文件 (可能使用 os.makedev()),其它都将忽略. 
新增version 2.3. 
os.major(device) 
从原始的设备号中提取设备major号码 (使用stat中的st_dev或者st_rdev field). 
新增version 2.3. 
os.minor(device) 
从原始的设备号中提取设备minor号码 (使用stat中的st_dev或者st_rdev field ). 
新增version 2.3. 
os.makedev(major, minor) 
以major和minor设备号组成一个原始设备号. 
新增version 2.3. 
os.mkdir(path[, mode])   
以数字mode的mode创建一个名为path的文件夹.默认的 mode 是 0777 (八进制). 在有些平台上, mode是忽略的. 当使用时。这当前的掩码值是first masked out. 在unix，Windows中有效。 
os.mkdir('sunck')    #在当前目录下创建新目录
os.mkdir(r"C:\\Users\\Administrator\\.spyder-py3\\kaige")    #在指定目录下创建新目录
也可以创建临时文件夹; 查看tempfile模块tempfile.mkdtemp()函数. 
os.makedirs(path[, mode]) 
递归文件夹创建函数。像mkdir(), 但创建的所有intermediate-level文件夹需要包含子文件夹. 抛出一个error exception如果子文件夹已经存在或者不能创建. 默认的 mode 是 0777 (八进制). 在有些平台上, mode是忽略的.当使用时。这当前的掩码值是first masked out。 
注: 
makedirs()变得迷惑 如果路径元素包含os.pardir. 
现在可以正确处理 UNC路径. 
os.pathconf(path, name) 
在unix中有效，请查看相关文档.。 
os.pathconf_names 
在unix中有效，请查看相关文档.。 
os.readlink(path) 
在unix中有效，请查看相关文档.。 
os.remove(path) 
删除路径为path的文件.如果path 是一个文件夹，将抛出OSError; 查看下面的rmdir()删除一个 directory. 这和下面的unlink()函数文档是一样的.在Windows,尝试删除一个正在使用的文件将抛出一个exception;在Unix,directory入口会被删除，但分配给文件的存储是无效的，直到原来的文件不再使用. 在unix，Windows中有效。 
os.removedirs(path) 
递归删除directorie. 像rmdir(), 如果子文件夹成功删除, removedirs()才尝试它们的父文件夹,直到抛出一个error(它基本上被忽略,因为它一般意味着你文件夹不为空).例如, os.removedirs('foo/bar/baz') 将首先删除 'foo/bar/baz', 然后删除 'foo/bar' 和 'foo' 如果它们是空的. 如果子文件夹不能被成功删除，将抛出OSError . 
新增version 1.5.2. 
os.rename(src, dst)     
重命名file或者directory从 src 到dst.**如果dst是一个存在的directory, 将抛出OSError.** 在Unix, 如果dst在存且是一个file,如果用户有权限的话，它将被安静的替换. 操作将会失败在某些Unix 中如果src和dst在不同的文件系统中. 如果成功, 这命名操作将会是一个原子操作 (这是POSIX 需要). 在 Windows上, 如果dst已经存在, 将抛出OSError，即使它是一个文件. 在unix，Windows中有效。 如os.rename("sunck","kaige")  将sunck文件重命名为kaige
os.renames(old, new) 
递归重命名文件夹或者文件。像rename() 
新增version 1.5.2. 
os.rmdir(path) 
删除path文件夹. 仅当这文件夹是空的才可以, 否则, 抛出OSError. 要删除整个文件夹树, 可以使用shutil.rmtree(). 在unix，Windows中有效。 
os.stat(path)    #获取文件属性
执行一个stat()系统调用在给定的path上. 返回值是一个对象，属性与stat结构成员有关: st_mode (保护位), st_ino (inode number), st_dev (device), st_nlink (number of hard links), st_uid (所有用户的id), st_gid (所有者group id), st_size (文件大小, 以位为单位), st_atime (最近访问的时间), st_mtime (最近修改的时间), st_ctime (依赖于平台;在Unix上是metadata最近改变的时间,或者在 Windows上是创建时间): 
>>> import os 
>>> statinfo = os.stat('somefile.txt') 
>>> statinfo 
(33188, 422511L, 769L, 1, 1032, 100, 926L, 1105022698,1105022732, 1105022732) 
>>> statinfo.st_size 
926L 
>>> 
如果stat_float_times()返回True,time值是floats,以second进行计算. 一秒的小数部分也会显示出来， 如果系统支持. 在Mac OS, 时间常常是 floats.查看 stat_float_times() 获取更多信息. 
在一些Unix系统上(例如 Linux), 下面的属性也许是有效的: st_blocks (为文件分配了多少块), st_blksize (文件系统blocksize), st_rdev (设备型号如果是一个inode设备). st_flags (用户为文件定义的flags). 
在unix，Windows中有效。 
os.stat_float_times([newvalue]) 
决定stat_result是否以float对象显示时间戳. 
os.statvfs(path) 
在unix中有效，请查看相关文档.。 
os.symlink(source, link_name) 
在unix中有效，请查看相关文档.。 
os.tempnam([dir[, prefix]]) 
为创建一个临时文件返回一个唯一的path。在Windows使用TMP . 依赖于使用的C库; 
警告: 
使用tempnam() 对于symlink攻击是一个漏洞; 考虑使用tmpfile()代替. 
在unix，Windows中有效。 
os.tmpnam() 
为创建一个临时文件返回一个唯一的path. 
Warning: 
使用tempnam() 对于symlink攻击是一个漏洞; 考虑使用tmpfile()代替. 
在unix，Windows中有效。 
os.TMP_MAX 
tmpnam() 将产生唯一名字的最大数值. 
os.unlink(path) 
删除 file路径. 与remove()相同; 在unix，Windows中有效。 
os.utime(path, times) 
返回指定的path文件的访问和修改的时间。如果时间是 None, 则文件的访问和修改设为当前时间 。 否则, 时间是一个 2-tuple数字, (atime, mtime) 用来分别作为访问和修改的时间. 
在unix，Windows中有效。 
os.walk(top[, topdown=True[, onerror=None[, followlinks=False]]]) 
输出在文件夹中的文件名通过在树中游走，向上或者向下.在根目录下的每一个文件夹(包含它自己), 产生3-tuple (dirpath, dirnames, filenames)
【文件夹路径, 文件夹名字, *该路径下的所有*文件名】
dirpath是一个字符串, directory的路径. dirnames在dirpath中子文件夹的列表 (不包括 '.' '..'). filenames 文件是在dirpath不包含子文件夹的文件名的列表. 注： 列表中的names不包含path.为获得dirpath中的一个文件或者文件夹的完整路径 (以顶目录开始)或者, 操作 os.path.join(dirpath, name). 
如果optional参数topdown为True或者not指定, 一个directory的3-tuple将比它的任何子文件夹的3-tuple先产生 (directories 自上而下).如果topdown为 False, 一个directory的3-tuple将比它的任何子文件夹的3-tuple后产生 (directories 自下而上)。 
当topdown为True,调用者可以修改列表中列出的文件夹名字(也可以使用del或者slice), walk() 仅仅递归每一个包含在dirnames中的子文件夹; 可以减少查询, 利用访问的特殊顺序,或者甚至 告诉 walk() 关于文件夹的创建者或者重命名在它重新walk()之前.修改文件名当topdown 为False时是无效的, 因为在bottom-up模式中在dirnames中的directories比dirpath 它自己先产生 . 
默认listdir()的errors将被忽略. 如果optional参数onerror被指定,它应该是一个函数; 它调用时有一个参数, 一个OSError实例. 报告这错误后，继续walk,或者抛出exception终止walk.注意filename是可用的的 ，exception对象的filename属性. 
默认, walk() 不会进入符号链接 . 
新增version 2.6: 
注:如果你传入一个相对的pathname, 不要在walk()执行过程中改变当前文件夹. walk()不会改变改前文件夹, 同时确保它的调用者也不会改变. 
import os 
from os.path import join, getsize 
for root, dirs, files in os.walk('python/Lib/email'): 
    print root, "consumes", 
    print sum(getsize(join(root, name)) for name in files), 
    print "bytes in", len(files), "non-directory files" 
    if 'CVS' in dirs: 
        dirs.remove('CVS') # don't visit CVS directories 
# Delete everything reachable from the directory named in "top", 
# assuming there are no symbolic links. 
# CAUTION: This is dangerous! For example, if top == '/', it 
# could delete all your disk files. 
import os 
for root, dirs, files in os.walk(top, topdown=False): 
    for name in files: 
        os.remove(os.path.join(root, name)) 
    for name in dirs: 
        os.rmdir(os.path.join(root, name)) 
新增version 2.3. 
使用函数来创建和管理进程。 
使用 exec*() 函数使用arguments列表来载入新程序. 在每个例子, 一个用户敲入一个命令行中的第一个参数传递给程序作为它自己的名字而不是作为参数. 对于C 程序员来说, 它是传递给main()的argv[0]. 例如, os.execv('/bin/echo', ['foo', 'bar']) 将仅仅在标准输出上打印bar; foo 将被忽略. 
os.abort() 
产生一个SIGABRT标识到当前的进程.在Unix,这默认的行为是产生一个主要的dump;在Windows,这进程立即返回退出以一个状态码为3. 程序使用signal.signal()来注册一个SIGABRT 将导致不同的行为. 在unix，Windows中有效。 
os.execl(path, arg0, arg1, ...) 
os.execle(path, arg0, arg1, ..., env) 
os.execlp(file, arg0, arg1, ...) 
os.execlpe(file, arg0, arg1, ..., env) 
os.execv(path, args) 
os.execve(path, args, env) 
os.execvp(file, args) 
os.execvpe(file, args, env) 
这些函数将执行一个新程序, 替换当前进程; 他们没有返回.在Unix,新的执行体载入到当前的进程, 同时将和当前的调用者有相同的id. 将报告Errors 当抛出 OSError时. 
当前的进程立即被替代. 打开文件对象和描述符不会被刷新, 如果在这些打开的文件中有数据缓冲区， 应该在调用exec*() 函数之前，使用sys.stdout.flush()或者os.fsync()flush它们 . 
在unix，Windows中有效。 
os._exit(n) 
使用状态n退出系统,没有调用清理函数，刷新缓冲区。 在unix，Windows中有效。 
注:标准退出的方法是sys.exit(n). _exit()一般使用于fork()产生的子进程中. 
os.EX_OK 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_USAGE 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_DATAERR 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_NOINPUT 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_NOUSER 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_NOHOST 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_UNAVAILABLE 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_SOFTWARE 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_OSERR 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_OSFILE 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_CANTCREAT 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_IOERR 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_TEMPFAIL 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_PROTOCOL 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_NOPERM 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_CONFIG 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.EX_NOTFOUND 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.fork() 
在unix中有效，请查看相关文档.。 
os.forkpty() 
在一些unix中有效，请查看相关文档 
os.kill(pid, sig) 
在unix中有效，请查看相关文档.。 
os.killpg(pgid, sig) 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.nice(increment) 
在unix中有效，请查看相关文档.。 
os.plock(op) 
在unix中有效，请查看相关文档.。 
os.popen(...) 
os.popen2(...) 
os.popen3(...) 
os.popen4(...) 
运行子进程, 返回交流的打开的管道.这些函数在前面创建文件对象时介绍过. 
os.spawnl(mode, path, ...) 
os.spawnle(mode, path, ..., env) 
os.spawnlp(mode, file, ...) 
os.spawnlpe(mode, file, ..., env) 
os.spawnv(mode, path, args) 
os.spawnve(mode, path, args, env) 
os.spawnvp(mode, file, args) 
os.spawnvpe(mode, file, args, env) 
在新进程中执行程序path 
(请使用subprocess模块) 
如果模式是 P_NOWAIT, 返回新进程的id;如果模式是P_WAIT,返回进程退出时的状态码。 如果正常退出,或者-signal, 当 signal 是killed.在 Windows, 进程id实际上是process的handle,所它可以使用于waitpid()函数. 
import os 
os.spawnlp(os.P_WAIT, 'cp', 'cp', 'index.html', '/dev/null') 
L = ['cp', 'index.html', '/dev/null'] 
os.spawnvpe(os.P_WAIT, 'cp', L, os.environ) 
在unix，Windows中有效。 
新增version 1.6. 
os.P_NOWAIT 
os.P_NOWAITO 
spawn*()族函数参数mode的可选值. 如果给出其中任一个值, 新进程一创建完成，spawn*()函数将立即返回 ,返回进程id的值. 在unix，Windows中有效。 
新增version 1.6. 
os.P_WAIT 
spawn*()族函数参数mode的可能值. 如果将它赋值给mode, spawn*() 函数不返回，直接运行结束 以及如果运行成功 ，将返回进程的退出码，或者如果 signal杀掉了这个进程，将返回-signal. 在unix，Windows中有效。 
新增version 1.6. 
os.P_DETACH 
os.P_OVERLAY 
spawn*()族函数参数mode的可选值。P_DETACH和P_NOWAIT很相似，但是新进程依附在调用进程的console上。如果使用了P_OVERLAY，当前进程将被替换，spawn*()函数将无返回 。在Windows上有效。 
新增version 1.6. 
os.startfile(path[, operation]) 
以相关的程序打开文件. 
当operation没有指定或者'open', 这操作就像在Windows Explorer双击文件,或者将这个文件作为交互命令行中start命令的参数:与文件扩展相关的程序打开文件。 
当指定另外操作时, 它必须是“command verb” 它指定应该对文件做什么.像Microsoft的'print' 'edit' (作用在文件上) 'explore' and 'find' (作用在文件夹上). 
startfile()只要相关的应该程序一启动就返回。 没有选项等待应用程序关闭, 没有方法接收应用程序退出的状态. path参数与当前directory相关. 如果你想使用一个绝对路径,确保第一个字符不是 ('/')。 使用os.path.normpath() 函数确保路径对于win32来说是正确的. 在Windows上有效. 
新增version 2.0. 
新增version 2.5: The operation parameter. 
os.system(command) 
在shell中执行string命令. 这是使用标准C函数system(), 有同样的限制. 例如改变 sys.stdin, 不影响命令执行环境. 
在Unix, 请查看相当文档. 
在Windows, 返回值是在 shell运行命令的返回值。 
在unix，Windows中有效。 
subprocess模块提供了一个更强大的功能产生新进程和接收它们的结果; 
os.times() 
返回一个5-tuple的浮点数字， 表示(处理器或者其它)累积时间, 以秒为单位. items为:用户时间, 系统time, 子用户time, 子系统time, 和从过去一个固定的点真实流逝的时间. 在unix，Windows中有效。在Windows, 仅仅填充开始两项, 其它都为0. 
os.wait() 
在unix中有效，请查看相关文档.。 
os.waitpid(pid, options) 
Unix:等待一个指定的pid的子进程完成, 返回一个tuple返回它的进程id和退出状态 . 一般情况下option设为0. 
更强细请查看相关文档 
在Windows: 等待一个指定的pid的进程完成, 返回一个tuple返回它的进程id和退出状态向左移动了8位 。 如果pid小于或等于0没有特别的意思,将抛出exception. integer options 没有任何影响. pid 可以指向任何进程的id,不一定是子进程的id. 
os.wait3([options]) 
在unix中有效，请查看相关文档.。 
新增version 2.5. 
os.wait4(pid, options) 
在unix中有效，请查看相关文档.。 
新增version 2.5. 
os.WNOHANG 
在unix中有效，请查看相关文档.。 
os.WCONTINUED 
在某些unix中有效，请查看相关文档.。 
新增version 2.3. 
os.WUNTRACED 
在unix中有效，请查看相关文档.。 
os.WCOREDUMP(status) 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.WIFCONTINUED(status) 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.WIFSTOPPED(status) 
在unix中有效，请查看相关文档.。 
os.WIFSIGNALED(status) 
在unix中有效，请查看相关文档.。 
os.WIFEXITED(status) 
在unix中有效，请查看相关文档.。 
os.WEXITSTATUS(status) 
在unix中有效，请查看相关文档.。 
os.WSTOPSIG(status) 
在unix中有效，请查看相关文档.。 
os.WTERMSIG(status) 
在unix中有效，请查看相关文档.。 
16.1.6. Miscellaneous System Information 
os.confstr(name) 
在unix中有效，请查看相关文档.。 
os.confstr_names 
在unix中有效，请查看相关文档.。 
os.getloadavg() 
在unix中有效，请查看相关文档.。 
新增version 2.3. 
os.sysconf(name) 
在unix中有效，请查看相关文档.。 
os.curdir 
操作系统用此常数字符串作为当前文件夹的引用,获取当前目录。 
os.pardir 
操作系统用此常数字符串作为父文件夹的引用。 
os.sep 
系统使用此字符来分割路径。 
os.altsep 
系统使用另外一个字符来分割路径，如果只有一个分割字符存在，则是None. 
os.extsep 
分割基本文件名和扩展名的字符。 
新增version 2.2. 
os.pathsep 
系统使用此字符来分割搜索路径（像PATH），例如POSIX上':',Windows上的';'，也可以通过os.path 
os.defpath 
默认的搜索路径用作 exec*p*()和spawn*p*()如果环境没有 'PATH'. 也可以通过os.path. 
os.linesep 
当前平台上的换行符字符串. 在POSIX上是'\n',或者 在Windows上是'\r\n' . 不要使用 os.linesep作为换行符，当写入文本文件时 (默认); 使用'\n'代替, 在所有平台上. 
os.devnull 
空设备的文件路径.例如:POSIX上 '/dev/null' . 也可以通过os.path. 
新增version 2.4. 
16.1.7. 其它 函数 
os.urandom(n) 
返回n个随机byte值的string，作为加密使用
```
>>> os.path.dirname('c:\\Python\\a.txt')
'c:\\Python'
```
