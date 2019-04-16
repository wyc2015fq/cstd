# Nginx的命令行控制 - youbingchen的博客 - CSDN博客





2016年06月07日 23:47:28[youbingchen](https://me.csdn.net/youbingchen)阅读数：347标签：[nginx](https://so.csdn.net/so/search/s.do?q=nginx&t=blog)
个人分类：[nginx](https://blog.csdn.net/youbingchen/article/category/6263213)









#### 默认方式启动

```
/usr/local/nginx/sbin/nginx
#这时会读取默认路径下的配置文件:/usr/local/nginx/conf/nginx.conf
```

#### 另行指定配置文件

```
# 使用-c参数指定配置文件
/usr/local/nginx/sbin/nginx -c /tmp/nginx.conf
```

#### 另行指定安装目录的启动方式
`/usr/local/nginx/sbin/nginx -p /usr/local/nginx`
#### 另行指定全局配置项的启动方式

```
/usr/local/nginx/sbin/nginx -g "pid /var/nginx/test.pid"
# 上面的命令行意味着会将pid文件写到/var/nginx/test.pid
```

-g参数的约束条件是指定的配置项不能与默认路径下的nginx.conf中的配置相冲突，否则无法启动，就像上例那样，类似这样的配置项:pid logs/nginx.pid,是不能存在于默认的nginx.conf中的。另外一个约束是，-g方式启动的Nginx服务执行其他命令时，需要把-g带上，否则可能出现配置项不匹配的情形,如果要停止Nginx服务
`/usr/local/nginx/sbin/nginx -g "pid /var/nginx/test.pid" -s stop`
#### 仅仅测试配置信息是否有错误

在不启动Nginx的情况下，使用-t参数仅测试配置文件是否有错误

```
/usr/local/nginx/sbin/nginx -t 
# 在测试的时候使用-q参数可以不把error级别以下的信息输出到屏幕
/usr/local/nginx/sbin/nginx -t -q
```

#### 显示版本信息

使用-v参数显示Nginx的版本信息

```
/usr/local/nginx/sbin/nginx -v
# 用-V显示编译阶段的信息，比如Gcc的版本、操作系统版本，执行configure时的参数
/usr/local/nginx/sbin/nginx -V
```

#### 快速地停止服务

使用-s stop可以强制停止Nginx服务，-s参数其实是告诉Nginx程序正在运行的Nginx的服务发送信号量，Nginx通过nginx.pid文件得到master进程的进程ID，再向运行中的master进程发送TERM信号来快速地关闭Nginx服务。

```bash
/usr/local/nginx/sbin/nginx -s stop
# 等价于
kill -s SIGTERM <id>
# 或是
kill -s SIGINT <id>
```

实际上，如果通过kill命令直接向Nginx master进程发送TEAM或者是INT信号，效果是一样的。

#### 优雅的停止服务

如果希望Nginx服务可以正常地处理完当前所有的请求再停止服务

```bash
/usr/local/nginx/sbin/nginx -s quit
# 等价于
kill -s SIGQUIT <id>
```

#### 使运行中的Nginx重读配置项并生效

```bash
/usr/local/nginx/sbin/nginx -s reload
# 等价于
kill -s SIGHUP <id>
```

#### 日志文件回滚

```bash
/usr/local/nginx/sbin/nginx -s reopen
# 等价于
kill -s SIGUSER1 <id>
```

#### 平滑升级Nginx

当Nginx服务升级新的版本时，必须要将旧的二进制文件Nginx替换掉，通常要重启服务，但Nginx支持不重启服务来完成新版本的平滑升级

> 
通知正在运行的旧版本Nginx准备升级,通过master进程发送USER2信号可达到目的


```bash
kill -s  SIGUSER2 <id>
```

启动新版本的Nginx，可以使用以上介绍过的任意一种启动方法，这时通过ps命令发现新旧版本的Nginx在同时进行 

优雅关掉旧版本的Nginx



