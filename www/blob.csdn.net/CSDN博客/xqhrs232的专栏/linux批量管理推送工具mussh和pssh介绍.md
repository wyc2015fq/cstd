# linux批量管理推送工具mussh和pssh介绍 - xqhrs232的专栏 - CSDN博客
2017年08月14日 15:45:10[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1014
原文地址::[https://www.kafan.cn/edu/84299254.html](https://www.kafan.cn/edu/84299254.html)
相关文章
1、**[使用mussh巡检多台linux服务器](http://bbs.51cto.com/thread-1021328-1.html)----[http://bbs.51cto.com/thread-1021328-1.html](http://bbs.51cto.com/thread-1021328-1.html)**
2、[Linux]管理多主机的ssh软件---mussh小工具----[http://rtxbc.blog.163.com/blog/static/1228936592010825105416832/](http://rtxbc.blog.163.com/blog/static/1228936592010825105416832/)
mussh 是shell开发的一个小工具，pssh是python开发的一个用来批量管理linux主机的工具，现在我们来介绍mussh和pssh的使用方法。
先说下mussh，mussh 是shell开发的一个小工具，刚学习运维和shell的朋友可以拿来锻炼下，做个工具的二次开发。
[root@devops-ruifengyun ~ ]$ apt-get install mussh
正在读取软件包列表... 完成
正在分析软件包的依赖关系树 
正在读取状态信息... 完成 
下列软件包是自动安装的并且现在不需要了：
kde-l10n-engb kde-l10n-zhcn libffi6:i386 libglib2.0-0:i386
libsystemd-daemon0:i386 libudev1:i386 python-async python-git python-gitdb
python-smmap
Use 'apt-get autoremove' to remove them.
下列【新】软件包将被安装：
mussh
升级了 0 个软件包，新安装了 1 个软件包，要卸载 0 个软件包，有 205 个软件包未被升级。
需要下载 14.4 kB 的软件包。
解压缩后会消耗掉 71.7 kB 的额外空间。
获取：1 http://mirrors.oschina.net/ubuntu/ saucy/universe mussh all 1.0-1 [14.4 kB]
下载 14.4 kB，耗时 0秒 (30.3 kB/s)
Selecting previously unselected package mussh.
(正在读取数据库 ... 系统当前共安装有 234998 个文件和目录。)
正在解压缩 mussh (从 .../archives/mussh_1.0-1_all.deb) ...
正在处理用于 man-db 的触发器...
正在设置 mussh (1.0-1) ...
[root@devops-ruifengyun ~ ]$
![](http://kfwimg.ikafan.com/upload/9c/0a/9c0a7f1276bd61d7df2d6c1db9808104_thumb.jpg)
mussh的帮助，会看到他的帮助相当的简单。
|代码如下||
|----|----|
|[root@devops-ruifengyun ~ ]$ mussh --helpUsage: mussh [OPTIONS] <-h host.. | -H hostfile> [-c cmd] [-C scriptfile]mussh --help for full help textSend a command or list of commands to multiple hosts.OPTIONS:--help This text.-d [n] Verbose debug. Prints each action, all hostsand commands to be executed to STDERR. 'n' canbe from 0 to 2.-v [n] Ssh debug levels. Can be from 0 to 3.-m [n] Run concurrently on 'n' hosts at a time (asynchronous).Use '0' (zero) for infinite. (default if -m)-q No output unless necessary. -i <identity> [identity ..]Load an identity file. May be usedmore than once.-o <ssh-args> Args to pass to ssh with -o option.-a Force loading ssh-agent.-A Do NOT load ssh-agent.-b Print each hosts' output in a block without minglingwith other hosts' output.-B Allow hosts' output to mingle. (default)-u Unique. Eliminate duplicate hosts. (default)-U Do NOT make host list unique.-P Do NOT fall back to passwords on any host. This willskip hosts where keys fail.-l <login> Use 'login' when no other is specified with hostname.-L <login> Force use of 'login' name on all hosts.-s <shell> Path to shell on remote host. (Default: bash)-t <secs> Timeout setting for each session.(requires openssh 3.8 or newer)-V Print version info and exit.PROXY ARGS:-p [user@]<host>Host to use as proxy. (Must have mussh installed)-po <ssh-args> Args to pass to ssh on proxy with -o option.HOST ARGS:-h [user@]<host> [[user@]<host> ..]Add a host to list of hosts. May beused more than once.-H <file> [file ..]Add contents of file(s) to list of hosts.Files should have one host per line. Use"#" for comments.COMMAND ARGS:If neither is specified, commands will be read from standard input.-c <command> Add a command or quoted list of commands andargs to list of commands to be executed oneach host. May be used more than once.-C <file> [file ..]Add file contents to list of commands to beexecuted on each host. May be used morethan once.At least one host is required. Arguments are in no particular order.EXAMPLES:mussh -H ./linuxhosts -C spfiles/testscript.shmussh -c "cat /etc/hosts" -h myhost.mydomain.comComments and Bug Reports: doughnut@doughnut.net| |
咱们直接跑一个例子，首先需要把对端的主机放到一个文件里面。然后mussh 调用-H 识别主机的list列表。
[root@devops-ruifengyun ~ ]$
http://rfyiamcool.blog.51cto.com/
[root@devops-ruifengyun ~ ]$ cat mu.list
10.1.25.46
10.1.25.47
10.1.25.48
10.1.25.49
[root@devops-ruifengyun ~ ]$
[root@devops-ruifengyun ~ ]$
[root@devops-ruifengyun ~ ]$ mussh -H ./mu.list -c 'dir'
10.154.252.46: a.py install.log keepalived-1.2.12
10.154.252.46: epel-release-6-8.noarch.rpm install.log.syslog keepalived-1.2.12.tar.gz
10.154.252.47: install.log install.log.syslog keepalived-1.2.12 keepalived-1.2.12.tar.gz
10.154.252.48: install.log install.log.syslog rs.sh
10.154.252.49: install.log install.log.syslog rs.sh
[root@devops-ruifengyun ~ ]$
[root@devops-ruifengyun ~ ]$
![](http://kfwimg.ikafan.com/upload/56/1a/561a2b394e21e7a9d9d101fe523cfb9d_thumb.jpg)
测试他有没有并发的特性。
[root@devops-ruifengyun ~ ]$ time mussh -H ./mu.list -c 'sleep 3;dir'
10.154.252.46: a.py install.log keepalived-1.2.12
10.154.252.46: epel-release-6-8.noarch.rpm install.log.syslog keepalived-1.2.12.tar.gz
10.154.252.47: install.log install.log.syslog keepalived-1.2.12 keepalived-1.2.12.tar.gz
10.154.252.48: install.log install.log.syslog rs.sh
10.154.252.49: install.log install.log.syslog rs.sh
mussh -H ./mu.list -c 'sleep 3;dir' 0.04s user 0.16s system 1% cpu 13.206 total
[root@devops-ruifengyun ~ ]$
果然没有并发执行。。。这也太不咋低了。
mussh支持发送脚本并执行的，在mussh的cli命令行 直接 -C 跟着脚本就可以了。
[root@devops-ruifengyun ~ ]$ time mussh -H ./mu.list -C c.sh
10.154.252.46: a.py install.log keepalived-1.2.12
10.154.252.46: epel-release-6-8.noarch.rpm install.log.syslog keepalived-1.2.12.tar.gz
10.154.252.47: install.log install.log.syslog keepalived-1.2.12 keepalived-1.2.12.tar.gz
10.154.252.48: install.log install.log.syslog rs.sh
10.154.252.49: install.log install.log.syslog rs.sh
mussh -H ./mu.list -C c.sh 0.10s user 0.17s system 42% cpu 0.629 total
[root@devops-ruifengyun ~ ]$
[root@devops-ruifengyun ~ ]$
[root@devops-ruifengyun ~ ]$
咱们再来测试下pssh这个比mussh要高端的东西，pssh是python开发的一个用来批量管理linux主机的工具。
pssh相关参数
pssh在多个主机上并行地运行命令
-h 执行命令的远程主机列表,文件内容格式[user@]host[:port]
如 test@172.16.10.10:229
-H 执行命令主机，主机格式 user@ip:port
-l 远程机器的用户名
-p 一次最大允许多少连接
-P 执行时输出执行信息
-o 输出内容重定向到一个文件
-e 执行错误重定向到一个文件
-t 设置命令执行超时时间
-A 提示输入密码并且把密码传递给ssh(如果私钥也有密码也用这个参数)
-O 设置ssh一些选项
-x 设置ssh额外的一些参数，可以多个，不同参数间空格分开
-X 同-x,但是只能设置一个参数
-i 显示标准输出和标准错误在每台host执行完毕后
附加工具
pscp 传输文件到多个hosts，类似scp
pscp -h hosts.txt -l irb2 foo.txt /home/irb2/foo.txt
pslurp 从多台远程机器拷贝文件到本地
pnuke 并行在远程主机杀进程
pnuke -h hosts.txt -l irb2 java
prsync 使用rsync协议从本地计算机同步到远程主机
prsync -r -h hosts.txt -l irb2 foo /home/irb2/foo
[root@vm-10-154-252-82 ~]$
[root@vm-10-154-252-82 ~]$cat list
10.154.252.46
10.154.252.47
10.154.252.48
10.154.252.49
[root@vm-10-154-252-82 ~]$
[root@vm-10-154-252-82 ~]$
[root@vm-10-154-252-82 ~]$pssh -i -h list 'uptime'
[1] 10:10:14 [SUCCESS] 10.154.252.46
10:10:14 up 62 days, 19:26, 2 users, load average: 0.08, 0.02, 0.03
[2] 10:10:14 [SUCCESS] 10.154.252.49
10:10:14 up 62 days, 19:30, 1 user, load average: 0.00, 0.00, 0.00
[3] 10:10:14 [SUCCESS] 10.154.252.48
10:10:14 up 62 days, 20:49, 1 user, load average: 0.13, 0.03, 0.01
[4] 10:10:14 [SUCCESS] 10.154.252.47
10:10:14 up 62 days, 19:59, 0 users, load average: 0.28, 0.08, 0.02
[root@vm-10-154-252-82 ~]$
[root@vm-10-154-252-82 ~]$
pssh 是python行的，容易实现程序调度的并发。
|代码如下|复制代码|
|----|----|
|[root@vm-10-154-252-82 ~]$time pssh -i -h list 'sleep 3;uptime'[1] 10:13:10 [SUCCESS] 10.154.252.46 10:13:10 up 62 days, 19:29, 2 users, load average: 0.00, 0.00, 0.01 [2] 10:13:10 [SUCCESS] 10.154.252.47 10:13:10 up 62 days, 20:02, 0 users, load average: 0.01, 0.04, 0.00 [3] 10:13:10 [SUCCESS] 10.154.252.49 10:13:10 up 62 days, 19:33, 1 user, load average: 0.00, 0.00, 0.00 [4] 10:13:10 [SUCCESS] 10.154.252.48 10:13:10 up 62 days, 20:52, 1 user, load average: 0.06, 0.04, 0.00 real 0m3.175s user 0m0.101s sys 0m0.038s| |
![](http://kfwimg.ikafan.com/upload/5a/7c/5a7c186b629ba049c3f3c85eb0addeeb_thumb.jpg)
pssh的源码是在 /usr/lib/python2.6/site-packages/psshlib
我们可以看看他的主调度的模块，managepy
import select 
import signal 
import sys 
import threading
里面含有这四个模块。 select用来做调度，signal用来做超时的判断，threading 用来做多任务的并发执行。 pssh 远程的ssh执行，没有调用paramiko或者是fabric这样现成的ssh库，而是直接用subprocess调用系统的ssh进程。
|代码如下||
|----|----|
|def clear_sigchld_handler(self): signal.signal(signal.SIGCHLD, signal.SIG_DFL)def set_sigchld_handler(self): # TODO: find out whether set_wakeup_fd still works if the default # signal handler is used (I'm pretty sure it doesn't work if the # signal is ignored). signal.signal(signal.SIGCHLD, self.handle_sigchld) # This should keep reads and writes from getting EINTR. if hasattr(signal, 'siginterrupt'): signal.siginterrupt(signal.SIGCHLD, False)def handle_sigchld(self, number, frame): """Apparently we need a sigchld handler to make set_wakeup_fd work.""" # Write to the signal pipe (only for Python <2.5, where the # set_wakeup_fd method doesn't exist). if self.iomap.wakeup_writefd: os.write(self.iomap.wakeup_writefd, '') for task in self.running: if task.proc: task.proc.poll() # Apparently some UNIX systems automatically resent the SIGCHLD # handler to SIG_DFL. Reset it just in case. self.set_sigchld_handler() def check_timeout(self): """Kills timed-out processes and returns the lowest time left.""" if self.timeout <= 0: return Nonemin_timeleft = None for task in self.running: timeleft = self.timeout - task.elapsed() if timeleft <= 0: task.timedout() continue if min_timeleft is None or timeleft < min_timeleft: min_timeleft = timeleftif min_timeleft is None: return 0 else: return max(0, min_timeleft)| |
解析主机host文件的逻辑
|代码如下||
|----|----|
|def read_host_files(paths, default_user=None, default_port=None): """Reads the given host files.Returns a list of (host, port, user) triples. """ hosts = [] if paths: for path in paths: hosts.extend(read_host_file(path, default_user=default_user)) return hostsdef read_host_file(path, default_user=None, default_port=None): """Reads the given host file.Lines are of the form: host[:port] [login]. Returns a list of (host, port, user) triples. """ lines = [] f = open(path) for line in f: lines.append(line.strip()) f.close()hosts = [] for line in lines: # Skip blank lines or lines starting with # line = line.strip() if not line or line.startswith('#'): continue host, port, user = parse_host_entry(line, default_user, default_port) if host: hosts.append((host, port, user)) return hosts| |
再来看看执行命令的逻辑。
|代码如下||
|----|----|
|def start(self, nodenum, iomap, writer, askpass_socket=None): """Starts the process and registers files with the IOMap.""" self.writer = writerif writer: self.outfile, self.errfile = writer.open_files(self.pretty_host)# Set up the environment. environ = dict(os.environ) environ['PSSH_NODENUM'] = str(nodenum) environ['PSSH_HOST'] = self.host # Disable the GNOME pop-up password dialog and allow ssh to use # askpass.py to get a provided password. If the module file is # askpass.pyc, we replace the extension. environ['SSH_ASKPASS'] = askpass_client.executable_path() if askpass_socket: environ['PSSH_ASKPASS_SOCKET'] = askpass_socket if self.verbose: environ['PSSH_ASKPASS_VERBOSE'] = '1' # Work around a mis-feature in ssh where it won't call SSH_ASKPASS # if DISPLAY is unset. if 'DISPLAY' not in environ: environ['DISPLAY'] = 'pssh-gibberish'# Create the subprocess. Since we carefully call set_cloexec() on # all open files, we specify close_fds=False. self.proc = Popen(self.cmd, stdin=PIPE, stdout=PIPE, stderr=PIPE, close_fds=False, preexec_fn=os.setsid, env=environ) self.timestamp = time.time() if self.inputbuffer: self.stdin = self.proc.stdin iomap.register_write(self.stdin.fileno(), self.handle_stdin) else: self.proc.stdin.close() self.stdout = self.proc.stdout iomap.register_read(self.stdout.fileno(), self.handle_stdout) self.stderr = self.proc.stderr iomap.register_read(self.stderr.fileno(), self.handle_stderr)| |
其实这两个工具都不太满意，还是推荐用ansible和saltstack这样的扩展性比较强的工具。
当然还是要看你的环境，怎么爽怎么来 ！！！

