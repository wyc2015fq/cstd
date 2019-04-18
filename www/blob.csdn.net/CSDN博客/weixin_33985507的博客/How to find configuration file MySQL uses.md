# How to find configuration file MySQL uses? - weixin_33985507的博客 - CSDN博客
2013年10月30日 10:23:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
[http://www.dbasquare.com/2012/04/01/how-to-find-mysql-configuration-file/](http://www.dbasquare.com/2012/04/01/how-to-find-mysql-configuration-file/)
A customer called me today asking for help with locating the configuration file used by one of their production MySQL instances. From the description I was given it appeared that their server had at least six different copies of `my.cnf` file in different locations on disk. And all were similar enough that each could actually be the one. All superfluous files were the result of a bit negligent system administration. So what turned to be the quickest and the least destructive way to find the correct one?
Initially suspecting the server was simply running more than just one MySQL instance, I logged in to take a deeper look. But I found only one `mysqld` process and, indeed, several configuration files.
All of them seemed good candidates:
/etc/my.cnf
/etc/mysql/my.cnf
/var/lib/mysql/my.cnf
...
In many cases you could simply check system process list using `ps`:
server ~ # ps ax | grep '[m]ysqld'
10801 ?        Ssl    0:27 /usr/sbin/mysqld --defaults-file=/etc/mysql/my.cnf --basedir=/usr --datadir=/var/lib/mysql --pid-file=/var/run/mysqld/mysqld.pid --socket=/var/run/mysqld/mysqld.sock
In many cases, because it doesn’t really have to work every time. If configuration file was not specified explicitly by an init script starting the MySQL instance, then database would used the compiled-in default and such information would not be visible in the `ps` output. This could happen if for example the database instance was launched “by hand” from shell. The file information would also not be visible if the process line was truncated for any.
An alternative method could be examining information in `/proc`. `/proc` is the place where Linux kernel exposes a lot of internal information about itself, hardware and running processes through a bunch of virtual files and directories. Specifically each process has its own directory there that takes the name after the process id (or `PID`). Learning MySQL`PID` is as easy as running `pidof mysqld`.
One of the files we need is called `cmdline`. It contains the full command that started certain process.
server ~ # cat /proc/$(pidof mysqld)/cmdline | tr '\0' '\n'
/usr/sbin/mysqld
--defaults-file=/etc/mysql/my.cnf
The configuration information is clearly visible. The `tr` command simply converts any `\0` characters into line breaks and is there just for readability.
Yet another approach could be browsing the process environment information. It can also be found in `/proc` in a file called `environ`. Sometimes a startup script may leave some information there:
server ~ # tr '\0' '\n' < /proc/$(pidof mysqld)/environ | grep -i cnf
MY_CNF=/etc/mysql/my.cnf
Finally you can try figuring out the compiled-in defaults, but it won’t necessarily tell you which configuration was actually used. This method is also not 100% safe as it means attempting to start another MySQL instance, even if only to print help message, because MySQL does not seem to handle this very well and it may produce some conflicts:
server ~ # /usr/sbin/mysqld --help --verbose --skip-networking --pid-file=$(tempfile) 2> /dev/null | grep -A1 'Default options are read'
Default options are read from the following files in the given order:
/etc/my.cnf /etc/mysql/my.cnf ~/.my.cnf
Specifying `--pid-file` here is essential as otherwise the new `mysqld` may overwrite the PID file of the running instance.
All in all I was able to help my customer. But there is no foolproof way. It might happen that in certain circumstances figuring out the true `my.cnf` location may not be possible.
