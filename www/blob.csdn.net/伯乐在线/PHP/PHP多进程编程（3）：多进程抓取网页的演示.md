# PHP多进程编程（3）：多进程抓取网页的演示 - 文章 - 伯乐在线
原文出处： [暮夏](http://www.cnblogs.com/niniwzw/archive/2010/01/29/1659474.html)
要理解这个部分的代码，请阅读：
[PHP多进程编程（一）](http://blog.jobbole.com/?p=99254)
[PHP多进程编程（二）管道通信](http://blog.jobbole.com/?p=99264)
我们知道，从父进程到子经常的数据传递相对比较容易一些，但是从子进程传递到父进程就比较的困难。
有很多办法实现进程交互，在php中比较方便的是 管道通信。当然，还可以通过 socket_pair 进行通信。
首先是服务器为了应对每一个请求要做的事情（发送一个url 序列，url序列用t 分割。而结束标记是 n）

PHP
```
function clientHandle($msgsock, $obj)
{
    $nbuf = '';
    socket_set_block($msgsock);
    do {
        if (false === ($buf = @socket_read($msgsock, 2048, PHP_NORMAL_READ))) {
            $obj->error("socket_read() failed: reason: " . socket_strerror(socket_last_error($msgsock)));
            break;
        }
        $nbuf .= $buf;
        if (substr($nbuf, -1) != "\n") {
            continue;
        }
        $nbuf = trim($nbuf);
        if ($nbuf == 'quit') {
            break;
        }
        if ($nbuf == 'shutdown') {
            break;
        }
        $url = explode("\t", $nbuf);
        $nbuf = '';
        $talkback = serialize(read_ntitle($url));
        socket_write($msgsock, $talkback, strlen($talkback));
        debug("write to the client\n");
        break;
    } while (true);
}
```
上面代码比较关键的一个部分是 read_ntitle，这个函数实现多线程的读取标题。
代码如下：（为每一个url fork 一个线程，然后打开管道 ，读取到的标题写入到管道里面去，主线程一直的在读取管道数据，直到所有的数据读取完毕，最后删除管道）

PHP
```
function read_ntitle($arr)
{
    $pipe = new Pipe("multi-read");
    foreach ($arr as $k => $item)
    {
        $pids[$k] = pcntl_fork();
        if(!$pids[$k])
        {
             $pipe->open_write();
             $pid = posix_getpid();
             $content = base64_encode(read_title($item));
             $pipe->write("$k,$content\n");
             $pipe->close_write();
             debug("$k: write success!\n");
             exit;
        }
    }
    debug("read begin!\n");
    $data = $pipe->read_all();
    debug("read end!\n");
$pipe->rm_pipe();
return parse_data($data);
}
parse_data 代码如下，非常的简单，就不说了。
parse_data  代码如下，非常的简单，就不说了。
function parse_data($data)
{
    $data = explode("\n", $data);
    $new = array();
    foreach ($data as $value)
    {
        $value = explode(",", $value);
        if (count($value) == 2) {
            $value[1] = base64_decode($value[1]);
            $new[intval($value[0])] = $value[1];
        }
    }
    ksort($new, SORT_NUMERIC);
    return $new;
}
```
上面代码中，还有一个函数read_title 比较有技巧。为了兼容性，我没有采用curl，而是直接采用socket 通信。
在下载到 title 标签后，就停止读取内容，以节省时间。代码如下：

PHP
```
function read_title($url)
{
    $url_info = parse_url($url);
    if (!isset($url_info['host']) || !isset($url_info['scheme'])) {
     return false;
    }
    $host = $url_info['host'];
 $port = isset($url_info['port']) ? $url_info['port'] : null;
 $path = isset($url_info['path']) ? $url_info['path']  : "/";
 if(isset($url_info['query'])) $path .= "?".$url_info['query'];
 if(empty($port)){
  $port = 80;
 }
 if ($url_info['scheme'] == 'https'){
  $port = 443;
 }
 if ($url_info['scheme'] == 'http') {
  $port = 80;
 }
    $out = "GET $path HTTP/1.1\r\n";
    $out .= "Host: $host\r\n";
    $out .= "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.1.7)\r\n";
    $out .= "Connection: Close\r\n\r\n";
    $fp = fsockopen($host, $port, $errno, $errstr, 5);
    if ($fp == NULL) {
     error("get title from $url, error. $errno: $errstr \n");
     return false;
    }
    fwrite($fp, $out);
    $content = '';
    while (!feof($fp)) {
        $content .= fgets($fp, 1024);
        if (preg_match("/<title>(.*?)<\/title>/is", $content, $matches)) {
             fclose($fp);
            return encode_to_utf8($matches[1]);
        }
    }
    fclose($fp);
    return false;
}
function encode_to_utf8($string) 
{
     return mb_convert_encoding($string, "UTF-8", mb_detect_encoding($string, "UTF-8, GB2312, ISO-8859-1", true));
}
```
这里，我只是检测了 三种最常见的编码。其他的代码都很简单，这些代码都是测试用的，如果你要做这样一个服务器，一定要进行优化处理。特别是，要防止一次打开太多的进程，你要做更多的处理。
很多时候，我们抱怨php 不支持多进程，实际上，php是支持多进程的。当然，没有那么多的进程通信的选项，而多进程的核心就在于进程的通信与同步。在web开发中，这样的多线程基本上是不会使用的，因为有很严重的性能问题。要实现比较简单的多进程，高负载，必须借助其扩展。
