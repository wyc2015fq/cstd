# PHP的CURLOPT_POSTFIELDS参数使用数组和字符串的区别 - weixin_33985507的博客 - CSDN博客
2015年03月11日 16:21:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**手册上解释：**
**CURLOPT_POSTFIELDS **
全部数据使用HTTP协议中的"POST"操作来发送。要发送文件，在文件名前面加上@前缀并使用完整路径。这个参数可以通过urlencoded后的字符串类似'para1=val1¶2=val2&...'或使用一个以字段名为键值，字段数据为值的数组。如果value是一个数组，Content-Type头将会设置成multipart/form-data。 
PHP的CURL组件是非常常用的HTTP请求模拟器。
**正确使用方法：**
通常要发送post数据时，我已经习惯于这样写：
*curl_setopt( $ch, CURLOPT_POSTFIELDS,$post_data);但是在向某一个服务器发送请求的时候，服务器返回500。而使用socket方式连接上去发送请求，返回正常的数据。*
尝试发送到一个仅有<?php print_r($_SERVER);?>的网页上，可以看到使用数组发送POST数据时，收到的CONTENT_TYPE如下：
[CONTENT_TYPE] => multipart/form-data; boundary=—————————-f924413ea122
而使用http_build_query($post_data)，收到的CONTENT_TYPE如下：
[CONTENT_TYPE] => application/x-www-form-urlencoded
可见，当CURLOPT_POSTFIELDS被设置为数组时，HTTP头会发送Content_type: application/x-www-form-urlencoded。这个是正常的网页<form>提交表单时，浏览器发送的头部。而multipart/form-data我们知道这是用于上传文件的表单。包括了boundary分界符，会多出很多字节。
手册上提到：
> 
The full data to post in a HTTP “POST” operation. To post a file, prepend a filename with @ and use the full path. This can either be passed as a urlencoded string like ‘para1=val1¶2=val2&…’ or as an array with the field name as key and field data as value. If value is an array, the Content-Type header will be set to multipart/form-data.
使用数组提供post数据时，CURL组件大概是为了兼容@filename这种上传文件的写法，默认把content_type设为了multipart/form-data。虽然对于大多数web服务器并没有影响，但是还是有少部分服务器不兼容。
本文得出的结论是，在没有需要上传文件的情况下，尽量对post提交的数据进行http_build_query，然后发送出去，能实现更好的兼容性，更小的请求数据包。
**备注：**
string **http_build_query** ( [mixed](http://www.cnblogs.com/)`$query_data` [, string `$numeric_prefix` [, string `$arg_separator` [, int `$enc_type` = **`PHP_QUERY_RFC1738`** ]]] )
使用给出的关联（或下标）数组生成一个经过URL-encode 的请求字符串。
curl发送post数据的时候，如果用http_build_query生成对应的post数据，则不用urlencode编码对应的字符串了
如果是手动拼接，则需要对中文手动用urlencode编码
