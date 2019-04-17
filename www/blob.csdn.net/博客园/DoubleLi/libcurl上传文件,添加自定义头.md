# libcurl上传文件,添加自定义头 - DoubleLi - 博客园







原文  [http://www.cnblogs.com/meteoric_cry/p/4285881.html](http://www.cnblogs.com/meteoric_cry/p/4285881.html?utm_source=tuicool&utm_medium=referral)

主题 [curl](http://www.tuicool.com/topics/11200052)




libcurl参数很多，一不小心就容易遇到问题。曾经就遇到过一个很蛋疼的问题：libcurl断点下载>>

这里主要汇总一下，libcurl上传的二种方式：

1、直接上传文件，类似form表单<input type=”file” />，<form enctype=”multipart/form-data”…；

2、上传二进制流；

至于设定自定义头，都是使用一样的方法
struct curl_slist *headers=NULL;
headers = curl_slist_append(headers, "Content-Type: text/xml");
headers = curl_slist_append(headers, "Accept: text/html, */*;q=0.01");
//...

//set headers
curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);


//last free the header list
curl_slist_free_all(headers); /* free the header list */[前往官网查看CURLOPT_HTTPHEADER>>](http://curl.haxx.se/libcurl/c/CURLOPT_HTTPHEADER.html)
直接上传文件：
struct curl_httppost *formpost = NULL;
struct curl_httppost *lastptr = NULL;
 
curl_formadd(&formpost, &lastptr,
    CURLFORM_COPYNAME, "uploadfile",
    CURLFORM_FILE, "/",//imagepath
    CURLFORM_CONTENTTYPE, "Image/jpeg",
    CURLFORM_END);
 
curl_formadd(&formpost, &lastptr,
    CURLFORM_COPYNAME, "filename",
    CURLFORM_COPYCONTENTS, "test.jpg",
    CURLFORM_END);
 
curl_easy_setopt(m_curl,CURLOPT_HTTPPOST,formpost);
  
//last free post
curl_formfree(formpost);
如果上传其它文件类型，那就在curl_formadd中附加CURLFORM_CONTENTTYPE参数（ [Content-Type/Mime-Type](http://tool.oschina.net/commons) ）

二进制流上传：
//referer http://curl.haxx.se/mail/lib-2003-08/0190.html
curl_formadd(&post, &last, 
             CURLFORM_COPYNAME, "file", 
             CURLFORM_BUFFER, "unnamed.png", 
             CURLFORM_BUFFERPTR, memblock,     
             CURLFORM_BUFFERLENGTH, memblock_length, 
             CURLFORM_CONTENTTYPE, "image/png",
             CURLFORM_END); 

 
‘file' is the name of this part, 'unnamed.png' is the name set in the file name field.
上面的参数均不可缺省，如果缺省结果可能不太符合预期。其中Content-Type默认为“application/octet-stream”

![](http://img0.tuicool.com/y6VJzy.png!web)

CURLFORM_COPYNAME为上传的字段名，如上图所示(name之后)

CURLFORM_BUFFER  在没有使用CURLFORM_FILE上传自定义文件时使用，它用来告诉libcurl文件内容已经在缓存中了，在内容的头信息中它提供了filename字段。我之前没加这个参数发现一直上传不成功- -

*** 还有一个地方要注意的是：CURLFORM_BUFFERLENGTH它的类型必须为long

为了方便测试，我使用了node.js的 [formidable](https://github.com/felixge/node-formidable) 来调试

参考链接：

[http://curl.haxx.se/libcurl/c/curl_formadd.html](http://curl.haxx.se/libcurl/c/curl_formadd.html)

[http://curl.haxx.se/libcurl/c/libcurl-tutorial.html](http://curl.haxx.se/libcurl/c/libcurl-tutorial.html)

[http://curl.haxx.se/libcurl/c/postit2.html](http://curl.haxx.se/libcurl/c/postit2.html)

[http://stackoverflow.com/questions/14685196/c-libcurl-force-content-type](http://stackoverflow.com/questions/14685196/c-libcurl-force-content-type)

[http://zengrong.net/post/2088.htm](http://zengrong.net/post/2088.htm)

[http://stackoverflow.com/questions/25370991/libcurl-buffered-file-upload-not-working](http://stackoverflow.com/questions/25370991/libcurl-buffered-file-upload-not-working)

[通过HTTP协议上传文件](http://www.cnblogs.com/cswuyg/p/3185164.html)

[使用libcurl POST数据和上传文件](http://www.blogbus.com/gcoder-logs/191913436.html)











