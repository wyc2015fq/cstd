# PHP多进程(4) ：内部多进程 - 文章 - 伯乐在线
原文出处： [暮夏](http://www.cnblogs.com/niniwzw/archive/2010/01/30/1659589.html)
上面一个系列的教程：
[PHP多进程编程（一）](http://blog.jobbole.com/?p=99254)
[PHP多进程编程（二）管道通信](http://blog.jobbole.com/?p=99264)
[PHP多进程编程（三）多进程抓取网页的演示](http://blog.jobbole.com/?p=99272)
说的都是只兼容unix 服务器的多进程，下面来讲讲在window 和 unix 都兼容的多进程(这里是泛指，下面的curl实际上是通过IO复用实现的)。
通过扩展实现多线程的典型例子是CURL，CURL 支持多线程的抓取网页的功能。
这部分过于抽象，所以，我先给出一个CURL并行抓取多个网页内容的一个分装类。这个类实际上很实用，
详细分析这些函数的内部实现将在下一个教程里面描述。
你可能不能很好的理解这个类，而且，php curl 官方主页上都有很多错误的例子，在讲述了其内部机制
后，你就能够明白了。
先看代码：

PHP
```
class Http_MultiRequest
{
    //要并行抓取的url 列表
    private $urls = array();
    //curl 的选项
    private $options;
    //构造函数
    function __construct($options = array())
    {
        $this->setOptions($options);
    }
    //设置url 列表
    function setUrls($urls)
    {
        $this->urls = $urls;
        return $this;
    }
    //设置选项
    function setOptions($options)
    {
        $options[CURLOPT_RETURNTRANSFER] = 1;
        if (isset($options['HTTP_POST'])) 
        {
            curl_setopt($ch, CURLOPT_POST, 1);
            curl_setopt($ch, CURLOPT_POSTFIELDS, $options['HTTP_POST']);
            unset($options['HTTP_POST']);
        }
        if (!isset($options[CURLOPT_USERAGENT])) 
        {
            $options[CURLOPT_USERAGENT] = 'Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1;)';
        }
        if (!isset($options[CURLOPT_FOLLOWLOCATION])) 
        {
            $options[CURLOPT_FOLLOWLOCATION] = 1;
        }
        if (!isset($options[CURLOPT_HEADER]))
        {
            $options[CURLOPT_HEADER] = 0;
        }
        $this->options = $options;
    }
    //并行抓取所有的内容
    function exec()
    {
        if(empty($this->urls) || !is_array($this->urls))
        {
            return false;
        }
        $curl = $data = array();
        $mh = curl_multi_init();
        foreach($this->urls as $k => $v)
        {
            $curl[$k] = $this->addHandle($mh, $v);
        }
        $this->execMulitHandle($mh);
        foreach($this->urls as $k => $v)
        {
            $data[$k] = curl_multi_getcontent($curl[$k]);
            curl_multi_remove_handle($mh, $curl[$k]);
        }
        curl_multi_close($mh);
        return $data;
    }
    //只抓取一个网页的内容。
    function execOne($url)
    {
        if (empty($url)) {
            return false;
        }
        $ch = curl_init($url);
        $this->setOneOption($ch);
        $content = curl_exec($ch);
        curl_close($ch);
        return $content;
    }
    //内部函数，设置某个handle 的选项
    private function setOneOption($ch)
    {
        curl_setopt_array($ch, $this->options);
    }
    //添加一个新的并行抓取 handle
    private function addHandle($mh, $url)
    {
        $ch = curl_init($url);
        $this->setOneOption($ch);
        curl_multi_add_handle($mh, $ch);
        return $ch;
    }
    //并行执行(这样的写法是一个常见的错误，我这里还是采用这样的写法，这个写法
    //下载一个小文件都可能导致cup占用100%, 并且，这个循环会运行10万次以上
    //这是一个典型的不懂原理产生的错误。这个错误在PHP官方的文档上都相当的常见。）
    private function execMulitHandle2($mh)
    {
        $i = 0;
        $running = null;
        do {
            curl_multi_exec($mh, $running);
            $i++;
        } while ($running > 0);
        //var_dump($i);
    }
    //应该用这样的写法
    private function execMulitHandle($mh)
    {
        $i = 0;
        do {$mrc = curl_multi_exec($mh,$active); $i++;} while ($mrc == CURLM_CALL_MULTI_PERFORM);
        while ($active && $mrc == CURLM_OK) 
        {
            if (curl_multi_select($mh) != -1)
            {
                do {$mrc = curl_multi_exec($mh, $active); $i++;} while ($mrc == CURLM_CALL_MULTI_PERFORM);
            }
            $i++;
        }
        //var_dump($i);
    }
}
```
看最后一个注释最多的函数，这个错误在平时调试的时候可能不太容易发现，因为程序完全正常，但是，在生产服务器下，马上会引起崩溃效果。
解释为什么不能这样，必须从C 语言内部实现的角度来分析。这个部分将放到下一个教程（PHP高级编程之–单线程实现并行抓取网页 ）。不过不是通过C语言来表述原理，而是通过PHP
这个类，实际上也就很简单的实现了前面我们费了4个教程的篇幅，并且是九牛二虎之力才实现的多线程的抓取网页的功能。在纯PHP的实现下，我们只能用一个后台服务的方式来比较好的实现，但是当你使用 操作系统接口语言 C 语言时候，这个实现当然就更加的简单，灵活，高效。
就同时抓取几个网页这样一件简单的事情，实际上在底层涉及到了很多东西，对很多半路出家的PHP程序员，可能不喜欢谈多线程这个东西，深入了就涉及到操作系统，浅点说就是并行运行好几个“程序”。但是，很多时候，多线程必不可少，比如要写个快点的爬虫，往往就会浪费九牛二虎之力。不过，PHP的程序员现在应该感谢CURL 这个扩展，这样，你完全不需要用你不太精通的 python 去写爬虫了，对于一个中型大小的爬虫，有这个内部多线程，就已经足够了。
最后是上面的类的一个测试的例子：

PHP
```
$urls = array("http://baidu.com", "http://baidu.com", "http://baidu.com", "http://baidu.com", "http://baidu.com", "http://baidu.com", "http://www.google.com", "http://www.sina.com.cn", );
$m = new Http_MultiRequest();
$t = microtime(true);
$m->setUrls($urls);
//parallel fetch（并行抓取）:
$data = $m->exec();
$parallel_time = microtime(true) - $t;
echo $parallel_time . "\n";
$t = microtime(true);
//serial fetch（串行抓取）:
foreach ($urls as $url)
{
    $data[] = $m->execOne($url);
}
$serial_time = microtime(true) - $t;
echo $serial_time . "\n";
```
