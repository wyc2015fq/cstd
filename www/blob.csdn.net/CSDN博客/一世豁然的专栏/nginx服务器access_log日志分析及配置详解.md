# nginx服务器access_log日志分析及配置详解 - 一世豁然的专栏 - CSDN博客





2016年08月17日 14:50:03[一世豁然](https://me.csdn.net/Explorer_day)阅读数：43883








本博客转载于：[http://www.pythontab.com/html/2013/linuxkaiyuan_0104/96.html](http://www.pythontab.com/html/2013/linuxkaiyuan_0104/96.html)





nginx的log日志分为access log 和 error log





其中access log 记录了哪些用户，哪些页面以及用户浏览器、ip和其他的访问信息




error log 则是记录服务器错误日志




错误日志的形式如下：




201.158.69.116 - - [03/Jan/2013:21:17:20 -0600] fwf[-] tip[-] 127.0.0.1:9000 0.007 0.007 MX pythontab.com GET /html/test.html HTTP/1.1 "200" 2426 "http://a.com" "es-ES,es;q=0.8" "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.97
 Safari/537.11"

187.171.69.177 - - [03/Jan/2013:21:17:20 -0600] fwf[-] tip[-] 127.0.0.1:9000 0.006 0.006 MX pythontab.com GET /html/test2.html HTTP/1.1 "200" 2426 "http://a.com" "es-ES,es;q=0.8" "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.97
 Safari/537.11"




从上面我们可以看出几部分信息：




1.客户端（用户）IP地址。如：上例中的 201.158.69.116

2.访问时间。如：上例中的 [03/Jan/2013:21:17:20 -0600]

3.访问端口。如：上例中的 127.0.0.1:9000

4.响应时间。如：上例中的 0.007 

5.请求时间。如：上例中的 0.007 

6.用户地理位置代码（国家代码）。如：上例中的 MX（墨西哥）

7.请求的url地址（目标url地址）的host。如：上例中的 pythontab.com

8.请求方式（GET或者POST等）。如：上例中的 GET

9.请求url地址（去除host部分）。如：上例中的 /html/test.html

10.请求状态（状态码，200表示成功，404表示页面不存在，301表示永久重定向等，具体状态码可以在网上找相关文章，不再赘述）。如：上例中的 "200"

11.请求页面大小，默认为B（byte）。如：上例中的 2426

12.来源页面，即从哪个页面转到本页，专业名称叫做“referer”。如：上例中的 "http://a.com"

13.用户浏览器语言。如：上例中的 "es-ES,es;q=0.8"

14.用户浏览器其他信息，浏览器版本、浏览器类型等。如：上例中的  "Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.11 (KHTML, like Gecko) Chrome/23.0.1271.97 Safari/537.11"







其实nginx access日志的格式不是一成不变的，是可以自定义的。




在nginx的nginx.conf配置文件找到：log_format 这里就是日志的格式




看一下和上述日志匹配的log格式设置：




#access日志格式配置，具体参数不再细说，上面都已经说过了，自己对应一下即可

log_format main '$remote_addr - $remote_user [$time_local] '

                     'fwf[$http_x_forwarded_for] tip[$http_true_client_ip] '

                     '$upstream_addr $upstream_response_time $request_time '

                     '$geoip_country_code '

                     '$http_host $request '

                     '"$status" $body_bytes_sent "$http_referer" '

                     '"$http_accept_language" "$http_user_agent" ';




#配置access log日志的存储位置及文件，注意：access.log文件是可以按日期进行分割的，方便查看及处理

access_log  /home/serversoft/nginx/log/access.log  main;



