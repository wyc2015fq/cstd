# Swoole笔记（四） - ggabcda的博客 - CSDN博客





2017年08月20日 19:21:06[ggabcda](https://me.csdn.net/ggabcda)阅读数：117










## Process

Process是swoole内置的进程管理模块，用来替代PHP的pcntl扩展。

swoole_process支持重定向标准输入和输出，在子进程内echo不会打印屏幕，而是写入管道，读键盘输入可以重定向为管道读取数据。

配合swoole_event模块，创建的PHP子进程可以异步的事件驱动模式。

### 创建子进程

函数原型：


```php
```php
int swoole_process::__construct(mixed $function, $redirect_stdin_stdout = false, $create_pipe = true);
```
```

- `$function`，子进程创建成功后要执行的函数，底层会自动将函数保存到对象的callback属性上。 
- 如果希望更改执行的函数，可赋值新的函数到对象的callback属性。 
- `$redirect_stdin_stdout`，重定向子进程的标准输入和输出。启用此选项后，在进程内echo将不是打印屏幕，而是写入到管道。读取键盘输入将变为从管道中读取数据。默认为阻塞读取。

- `$create_pipe`，是否创建管道，启用`$redirect_stdin_stdout`后，此选项将忽略用户参数，强制为true 如果子进程内没有进程间通信，可以设置为false。

- 1.7.22或更高版本允许设置管道的类型，默认为`SOCK_STREAM`流式

参数`$create_pipe`为2时，管道类型将设置为`SOCK_DGRAM`。 


```php
```php
<?php
$process = new swoole_process(function(swoole_process $worker){
    while (true){
        $cmd = $worker->read();
        passthru($cmd);
    }   
}, true, 2);
$process->start();

$process->write('ls -l');//将参数传入子进程内
echo $data = $process->read();//获取执行结果
```
```


实例：web版本的shell

https://github.com/52fhy/swoole_demo/blob/master/swoole_shell_server.php

## EventLoop

swoole还提供了直接操作底层epoll/kqueue事件循环的接口。可将其他扩展创建的socket，PHP代码中stream/socket扩展创建的socket等加入到Swoole的EventLoop中。

swoole_event_add函数用于将一个socket加入到swoole的reactor事件监听中。 函数原型：
`bool swoole_event_add(int $sock, mixed $read_callback, mixed $write_callback = null, int `$flags` = null);`- `$sock`支持文件描述符、stream资源、sockets资源。 
- `$read_callback`为可读回调函数。 
- `$write_callback`为可写事件回调。 
- `$flags`为事件类型的掩码，可选择关闭/开启可读可写事件，如`SWOOLE_EVENT_READ`，`SWOOLE_EVENT_WRITE`，或者`SWOOLE_EVENT_READ | SWOOLE_EVENT_WRITE`。


在Server程序中使用，可以理解为在worker/taskworker进程中将此socket注册到epoll事件中。

在Client程序中使用，可以理解为在客户端进程中将此socket注册到epoll事件中。

示例：


```php
```php
<?php
$fp = stream_socket_client("tcp://www.52fhy.com:80", $errno, $errstr, 30);
fwrite($fp, "GET / HTTP/1.1\r\nHost: www.52fhy.com\r\n\r\n");

swoole_event_add($fp, function($fp) {

    echo $resp = fread($fp, 1024);
    //socket处理完成后，从epoll事件中移除socket
    swoole_event_del($fp);
    fclose($fp);
});
echo "Finish\n";  //swoole_event_add不会阻塞进程，这行代码会顺序执行
```
```


对比一下，下面这个会阻塞进程：

```
<?php
$fp  =  stream_socket_client ( "tcp://www.52fhy.com:80" ,  $errno ,  $errstr ,  30 );
if (! $fp ) {
    echo  " $errstr  ( $errno )<br />\n" ;
} else {
     fwrite ( $fp ,  "GET / HTTP/1.1\r\nHost: www.52fhy.com\r\n\r\n" );
    while (! feof ( $fp )) {
        echo  fgets ( $fp ,  1024 );
    }
     fclose ( $fp );
}

echo "Finish\n";
```



