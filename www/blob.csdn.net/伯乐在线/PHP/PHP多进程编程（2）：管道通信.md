# PHP多进程编程（2）：管道通信 - 文章 - 伯乐在线
原文出处： [暮夏](http://www.cnblogs.com/niniwzw/archive/2010/01/20/1652801.html)
一个进程如果是个人英雄主义，那么多进程就是集体主义。（不严格区分多进程 和 多线程的差别）
你不再是一个独行侠，而是一个指挥家。
独来独往，非常自由自在，但是，很多时候，不如众人拾柴火焰高。
这就是我对多进程的理解。多线程编程的主要问题是：通信 和 同步问题。
更多PHP 多线程编程的背景知识见：
> 
**PHP多进程编程（一）**
在PHP 中，如果光用pcntl ，实现比较简单的通信问题都是很困难的。
**下面介绍管道通信：**
1. 管道可以认为是一个队列，不同的线程都可以往里面写东西，也都可以从里面读东西。写就是
在队列末尾添加，读就是在队头删除。
2. 管道一般有大小，默认一般是4K，也就是内容超过4K了，你就只能读，不能往里面写了。
3. 默认情况下，管道写入以后，就会被阻止，直到读取他的程序读取把数据读完。而读取进程也会被阻止，
直到有进程向管道写入数据。当然，你可以改变这样的默认属性，用stream_set_block  函数，设置成非阻断模式。
下面是我分装的一个管道的类（这个类命名有问题，没有统一，没有时间改成统一的了，我一般先写测试代码，最后分装，所以命名上可能不统一）：

PHP
```
Code highlighting produced by Actipro CodeHighlighter (freeware)http://www.CodeHighlighter.com/--><?php
class Pipe
{
    public  $fifoPath;
    private $w_pipe;
    private $r_pipe;
    /**
     * 自动创建一个管道
     *
     * @param string $name 管道名字
     * @param int $mode  管道的权限，默认任何用户组可以读写
     */
    function __construct($name = 'pipe', $mode = 0666)
    {
        $fifoPath = "/tmp/$name." . posix_getpid();
        if (!file_exists($fifoPath)) {
            if (!posix_mkfifo($fifoPath, $mode)) {
                error("create new pipe ($name) error.");
                return false;
            }
        } else {
            error( "pipe ($name) has exit.");
            return false;
        }
        $this->fifoPath = $fifoPath;
    }
///////////////////////////////////////////////////
//  写管道函数开始
///////////////////////////////////////////////////
    function open_write()
    {
        $this->w_pipe = fopen($this->fifoPath, 'w');
        if ($this->w_pipe == NULL) {
            error("open pipe {$this->fifoPath} for write error.");
            return false;
        }
        return true;
    }
    function write($data)
    {
        return fwrite($this->w_pipe, $data);
    }
    function write_all($data)
    {
        $w_pipe = fopen($this->fifoPath, 'w');
        fwrite($w_pipe, $data);
        fclose($w_pipe);
    }
    function close_write()
    {
        return fclose($this->w_pipe);
    }
/////////////////////////////////////////////////////////
/// 读管道相关函数开始
////////////////////////////////////////////////////////
    function open_read()
    {
        $this->r_pipe = fopen($this->fifoPath, 'r');
        if ($this->r_pipe == NULL) {
            error("open pipe {$this->fifoPath} for read error.");
            return false;
        }
        return true;
    }
    function read($byte = 1024)
    {
        return fread($this->r_pipe, $byte);
    }
    function read_all()
    {
        $r_pipe = fopen($this->fifoPath, 'r');
        $data = '';
        while (!feof($r_pipe)) {
            //echo "read one K\n";
            $data .= fread($r_pipe, 1024);
        }
        fclose($r_pipe);
        return $data;
    }
    function close_read()
    {
        return fclose($this->r_pipe);
    }
////////////////////////////////////////////////////
    /**
     * 删除管道
     *
     * @return boolean is success
     */
    function rm_pipe()
    {
        return unlink($this->fifoPath);
    }
}
?>
```
有了这个类，就可以实现简单的管道通信了，因为这个教程是多进程编程系列教程的一个部分。
这个管道类的应用部分，将放到第三部分。
