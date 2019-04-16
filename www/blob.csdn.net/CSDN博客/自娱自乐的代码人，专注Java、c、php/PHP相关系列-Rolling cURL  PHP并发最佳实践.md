# PHP相关系列 - Rolling cURL: PHP并发最佳实践 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年06月27日 15:50:14[initphp](https://me.csdn.net/initphp)阅读数：894








在实际项目或者自己编写小工具(比如新闻聚合,商品价格监控,比价)的过程中, 通常需要从第3方网站或者API接口获取数据, 在需要处理1个URL队列时, 为了提高性能, 可以采用cURL提供的curl_multi_*族函数实现简单的并发.

本文将探讨两种具体的实现方法, 并对不同的方法做简单的性能对比. 

## 1. 经典cURL并发机制及其存在的问题

经典的cURL实现机制在网上很容易找到, 比如参考[PHP在线手册](http://php.net/manual/en/function.curl-multi-init.php)的如下实现方式:



```
```php
**function**
```

```php
classic_curl(
```

```php
$urls
```

```php
,
```

```php
$delay
```

```php
)
 {
```



```php

```

```php
$queue
```

```php
= curl_multi_init();
```



```php

```

```php
$map
```

```php
=
```

```php
**array**
```

```php
();
```



```php

```



```php

```

```php
**foreach**
```

```php
(
```

```php
$urls
```

```php
**as**
```

```php
$url
```

```php
)
 {
```



```php

```

```php
// create cURL resources
```



```php

```

```php
$ch
```

```php
= curl_init();
```



```php

```



```php

```

```php
// set URL and other appropriate options
```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_URL,
```

```php
$url
```

```php
);
```



```php

```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_TIMEOUT, 1);
```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_RETURNTRANSFER, 1);
```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_HEADER, 0);
```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_NOSIGNAL, true);
```



```php

```



```php

```

```php
// add handle
```



```php

```

```php
curl_multi_add_handle(
```

```php
$queue
```

```php
,
```

```php
$ch
```

```php
);
```



```php

```

```php
$map
```

```php
[
```

```php
$url
```

```php
]
 =
```

```php
$ch
```

```php
;
```



```php

```

```php
}
```



```php

```



```php

```

```php
$active
```

```php
= null;
```



```php

```



```php

```

```php
// execute the handles
```



```php

```

```php
**do**
```

```php
{
```



```php

```

```php
$mrc
```

```php
= curl_multi_exec(
```

```php
$queue
```

```php
,
```

```php
$active
```

```php
);
```



```php

```

```php
}
```

```php
**while**
```

```php
(
```

```php
$mrc
```

```php
==
 CURLM_CALL_MULTI_PERFORM);
```



```php

```



```php

```

```php
**while**
```

```php
(
```

```php
$active
```

```php
>
 0 &&
```

```php
$mrc
```

```php
== CURLM_OK) {
```



```php

```

```php
**if**
```

```php
(curl_multi_select(
```

```php
$queue
```

```php
,
 0.5) != -1) {
```



```php

```

```php
**do**
```

```php
{
```



```php

```

```php
$mrc
```

```php
= curl_multi_exec(
```

```php
$queue
```

```php
,
```

```php
$active
```

```php
);
```



```php

```

```php
}
```

```php
**while**
```

```php
(
```

```php
$mrc
```

```php
==
 CURLM_CALL_MULTI_PERFORM);
```



```php

```

```php
}
```



```php

```

```php
}
```



```php

```



```php

```

```php
$responses
```

```php
=
```

```php
**array**
```

```php
();
```



```php

```

```php
**foreach**
```

```php
(
```

```php
$map
```

```php
**as**
```

```php
$url
```

```php
=>
```

```php
$ch
```

```php
)
 {
```



```php

```

```php
$responses
```

```php
[
```

```php
$url
```

```php
]
 = callback(curl_multi_getcontent(
```

```php
$ch
```

```php
),
```

```php
$delay
```

```php
);
```



```php

```

```php
curl_multi_remove_handle(
```

```php
$queue
```

```php
,
```

```php
$ch
```

```php
);
```



```php

```

```php
curl_close(
```

```php
$ch
```

```php
);
```



```php

```

```php
}
```



```php

```



```php

```

```php
curl_multi_close(
```

```php
$queue
```

```php
);
```



```php

```

```php
**return**
```

```php
$responses
```

```php
;
```



```php
}
```
```







首先将所有的URL压入并发队列, 然后执行并发过程, 等待所有请求接收完之后进行数据的解析等后续处理. 在实际的处理过程中, 受网络传输的影响, 部分URL的内容会优先于其他URL返回, 但是经典cURL并发必须等待最慢的那个URL返回之后才开始处理, 等待也就意味着CPU的空闲和浪费. 如果URL队列很短, 这种空闲和浪费还处在可接受的范围, 但如果队列很长, 这种等待和浪费将变得不可接受.

## 2. 改进的Rolling cURL并发方式

仔细分析不难发现经典cURL并发还存在优化的空间, 优化的方式时当某个URL请求完毕之后尽可能快的去处理它, 边处理边等待其他的URL返回, 而不是等待那个最慢的接口返回之后才开始处理等工作, 从而避免CPU的空闲和浪费. 闲话不多说, 下面贴上具体的实现:



```
```php
**function**
```

```php
rolling_curl(
```

```php
$urls
```

```php
,
```

```php
$delay
```

```php
)
 {
```



```php

```

```php
$queue
```

```php
= curl_multi_init();
```



```php

```

```php
$map
```

```php
=
```

```php
**array**
```

```php
();
```



```php

```



```php

```

```php
**foreach**
```

```php
(
```

```php
$urls
```

```php
**as**
```

```php
$url
```

```php
)
 {
```



```php

```

```php
$ch
```

```php
= curl_init();
```



```php

```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_URL,
```

```php
$url
```

```php
);
```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_TIMEOUT, 1);
```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_RETURNTRANSFER, 1);
```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_HEADER, 0);
```



```php

```

```php
curl_setopt(
```

```php
$ch
```

```php
, CURLOPT_NOSIGNAL, true);
```



```php

```



```php

```

```php
curl_multi_add_handle(
```

```php
$queue
```

```php
,
```

```php
$ch
```

```php
);
```



```php

```

```php
$map
```

```php
[(string)
```

```php
$ch
```

```php
]
 =
```

```php
$url
```

```php
;
```



```php

```

```php
}
```



```php

```



```php

```

```php
$responses
```

```php
=
```

```php
**array**
```

```php
();
```



```php

```

```php
**do**
```

```php
{
```



```php

```

```php
**while**
```

```php
((
```

```php
$code
```

```php
=
 curl_multi_exec(
```

```php
$queue
```

```php
,
```

```php
$active
```

```php
)) == CURLM_CALL_MULTI_PERFORM)
 ;
```



```php

```



```php

```

```php
**if**
```

```php
(
```

```php
$code
```

```php
!=
 CURLM_OK) {
```

```php
**break**
```

```php
; }
```



```php

```



```php

```

```php
// a request was just completed -- find out which one
```



```php

```

```php
**while**
```

```php
(
```

```php
$done
```

```php
=
 curl_multi_info_read(
```

```php
$queue
```

```php
)) {
```



```php

```



```php

```

```php
// get the info and content returned on the request
```



```php

```

```php
$info
```

```php
= curl_getinfo(
```

```php
$done
```

```php
[
```

```php
'handle'
```

```php
]);
```



```php

```

```php
$error
```

```php
= curl_error(
```

```php
$done
```

```php
[
```

```php
'handle'
```

```php
]);
```



```php

```

```php
$results
```

```php
= callback(curl_multi_getcontent(
```

```php
$done
```

```php
[
```

```php
'handle'
```

```php
]),
```

```php
$delay
```

```php
);
```



```php

```

```php
$responses
```

```php
[
```

```php
$map
```

```php
[(string)
```

```php
$done
```

```php
[
```

```php
'handle'
```

```php
]]]
 = compact(
```

```php
'info'
```

```php
,
```

```php
'error'
```

```php
,
```

```php
'results'
```

```php
);
```



```php

```



```php

```

```php
// remove the curl handle that just completed
```



```php

```

```php
curl_multi_remove_handle(
```

```php
$queue
```

```php
,
```

```php
$done
```

```php
[
```

```php
'handle'
```

```php
]);
```



```php

```

```php
curl_close(
```

```php
$done
```

```php
[
```

```php
'handle'
```

```php
]);
```



```php

```

```php
}
```



```php

```



```php

```

```php
// Block for data in / output; error handling is done by curl_multi_exec
```



```php

```

```php
**if**
```

```php
(
```

```php
$active
```

```php
>
 0) {
```



```php

```

```php
curl_multi_select(
```

```php
$queue
```

```php
, 0.5);
```



```php

```

```php
}
```



```php

```



```php

```

```php
}
```

```php
**while**
```

```php
(
```

```php
$active
```

```php
);
```



```php

```



```php

```

```php
curl_multi_close(
```

```php
$queue
```

```php
);
```



```php

```

```php
**return**
```

```php
$responses
```

```php
;
```



```php
}
```
```



## 3. 两种并发实现的性能对比

改进前后的性能对比试验在LINUX主机上进行, 测试时使用的并发队列如下:
- [http://item.taobao.com/item.htm?id=14392877692](http://item.taobao.com/item.htm?id=14392877692)
- [http://item.taobao.com/item.htm?id=16231676302](http://item.taobao.com/item.htm?id=16231676302)
- [http://item.taobao.com/item.htm?id=17037160462](http://item.taobao.com/item.htm?id=17037160462)
- [http://item.taobao.com/item.htm?id=5522416710](http://item.taobao.com/item.htm?id=5522416710)
- [http://item.taobao.com/item.htm?id=16551116403](http://item.taobao.com/item.htm?id=16551116403)
- [http://item.taobao.com/item.htm?id=14088310973](http://item.taobao.com/item.htm?id=14088310973)

简要说明下实验设计的原则和性能测试结果的格式: 为保证结果的可靠, 每组实验重复20次, 在单次实验中, 给定相同的接口URL集合, 分别测量Classic(指经典的并发机制)和Rolling(指改进后的并发机制)两种并发机制的耗时(秒为单位), 耗时短者胜出(Winner), 并计算节省的时间(Excellence, 秒为单位)以及性能提升比例(Excel. %). 为了尽量贴近真实的请求而又保持实验的简单, 在对返回结果的处理上只是做了简单的正则表达式匹配, 而没有进行其他复杂的操作. 另外, 为了确定结果处理回调对性能对比测试结果的影响,
 可以使用usleep模拟现实中比较负责的数据处理逻辑(如提取, 分词, 写入文件或数据库等). 

性能测试中用到的回调函数为:



```
```php
**function**
```

```php
callback(
```

```php
$data
```

```php
,
```

```php
$delay
```

```php
)
 {
```



```php

```

```php
preg_match_all(
```

```php
'/<h3>(.+)<\/h3>/iU'
```

```php
,
```

```php
$data
```

```php
,
```

```php
$matches
```

```php
);
```



```php

```

```php
usleep(
```

```php
$delay
```

```php
);
```



```php

```

```php
**return**
```

```php
compact(
```

```php
'data'
```

```php
,
```

```php
'matches'
```

```php
);
```



```php
}
```
```



数据处理回调无延迟时: Rolling Curl略优, 但性能提升效果不明显.
------------------------------------------------------------------------------------------------
Delay: 0 micro seconds, equals to 0 milli seconds
------------------------------------------------------------------------------------------------
Counter         Classic         Rolling         Winner          Excellence      Excel. %
------------------------------------------------------------------------------------------------
1               0.1193          0.0390          Rolling         0.0803          67.31%
2               0.0556          0.0477          Rolling         0.0079          14.21%
3               0.0461          0.0588          Classic         -0.0127         -21.6%
4               0.0464          0.0385          Rolling         0.0079          17.03%
5               0.0534          0.0448          Rolling         0.0086          16.1%
6               0.0540          0.0714          Classic         -0.0174         -24.37%
7               0.0386          0.0416          Classic         -0.0030         -7.21%
8               0.0357          0.0398          Classic         -0.0041         -10.3%
9               0.0437          0.0442          Classic         -0.0005         -1.13%
10              0.0319          0.0348          Classic         -0.0029         -8.33%
11              0.0529          0.0430          Rolling         0.0099          18.71%
12              0.0503          0.0581          Classic         -0.0078         -13.43%
13              0.0344          0.0225          Rolling         0.0119          34.59%
14              0.0397          0.0643          Classic         -0.0246         -38.26%
15              0.0368          0.0489          Classic         -0.0121         -24.74%
16              0.0502          0.0394          Rolling         0.0108          21.51%
17              0.0592          0.0383          Rolling         0.0209          35.3%
18              0.0302          0.0285          Rolling         0.0017          5.63%
19              0.0248          0.0553          Classic         -0.0305         -55.15%
20              0.0137          0.0131          Rolling         0.0006          4.38%
------------------------------------------------------------------------------------------------
Average         0.0458          0.0436          Rolling         0.0022          4.8%
------------------------------------------------------------------------------------------------
Summary: Classic wins 10 times, while Rolling wins 10 times

数据处理回调延迟5毫秒: Rolling Curl完胜, 性能提升40%左右.
------------------------------------------------------------------------------------------------
Delay: 5000 micro seconds, equals to 5 milli seconds
------------------------------------------------------------------------------------------------
Counter         Classic         Rolling         Winner          Excellence      Excel. %
------------------------------------------------------------------------------------------------
1               0.0658          0.0352          Rolling         0.0306          46.5%
2               0.0728          0.0367          Rolling         0.0361          49.59%
3               0.0732          0.0387          Rolling         0.0345          47.13%
4               0.0783          0.0347          Rolling         0.0436          55.68%
5               0.0658          0.0286          Rolling         0.0372          56.53%
6               0.0687          0.0362          Rolling         0.0325          47.31%
7               0.0787          0.0337          Rolling         0.0450          57.18%
8               0.0676          0.0391          Rolling         0.0285          42.16%
9               0.0668          0.0351          Rolling         0.0317          47.46%
10              0.0603          0.0317          Rolling         0.0286          47.43%
11              0.0714          0.0350          Rolling         0.0364          50.98%
12              0.0627          0.0215          Rolling         0.0412          65.71%
13              0.0617          0.0401          Rolling         0.0216          35.01%
14              0.0721          0.0226          Rolling         0.0495          68.65%
15              0.0701          0.0428          Rolling         0.0273          38.94%
16              0.0674          0.0352          Rolling         0.0322          47.77%
17              0.0452          0.0425          Rolling         0.0027          5.97%
18              0.0596          0.0366          Rolling         0.0230          38.59%
19              0.0679          0.0480          Rolling         0.0199          29.31%
20              0.0657          0.0338          Rolling         0.0319          48.55%
------------------------------------------------------------------------------------------------
Average         0.0671          0.0354          Rolling         0.0317          47.24%
------------------------------------------------------------------------------------------------
Summary: Classic wins 0 times, while Rolling wins 20 times

通过上面的性能对比, 在处理URL队列并发的应用场景中Rolling cURL应该是更加的选择, 并发量非常大(1000+)时, 可以控制并发队列的最大长度, 比如20, 每当1个URL返回并处理完毕之后立即加入1个尚未请求的URL到队列中, 这样写出来的代码会更加健壮, 不至于并发数太大而卡死或崩溃. 详细的实现请参考:[http://code.google.com/p/rolling-curl/](http://code.google.com/p/rolling-curl/)



