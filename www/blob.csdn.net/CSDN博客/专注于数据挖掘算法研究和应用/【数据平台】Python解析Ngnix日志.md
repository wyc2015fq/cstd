# 【数据平台】Python解析Ngnix日志 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月04日 15:30:01[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：466
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









场景：Ngnix格式具有一定格式，通过python的正则表达式读取日志中每行的字段。

Python正则表达式参考：https://docs.python.org/2/library/re.html

代码参考：



```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月4日

@author: Jason.F
@summary: Ngnix log parse
'''
import time
import re
class NgnixLogParse(object):
    
    def __init__(self,logline):
        self.logline=logline
     
    '''
    log_format  main  '$remote_addr - $remote_user [$time_local] "$request" '
                      '$status $body_bytes_sent "$http_referer" '
                      '"$http_user_agent" "$http_x_forwarded_for" "$upstream_addr" "$request_time" "$upstream_response_time" $host';
    '''
    def logparse(self):
        #正则表达式
        ip = r"?P<ip>[\d.]*"
     #timelocal = r"?P<timelocal>\[.*\]" #拆解成date、month、year、log_time
     user =r"?P<user>\S+"
     date = r"?P<date>\d+"
     month = r"?P<month>\w+"
    year = r"?P<year>\d+"
    log_time = r"?P<time>\S+"
    #request = r"?P<request>[^\"]*" #拆解成method、request、portocal
    method = r"?P<method>\S+"
    request = r"?P<request>\S+"
    protocal=r"?P<protocal>\S+"
    status = r"?P<status>\d+"
    bodyBytesSent = r"?P<bodyBytesSent>\d+"
    referer=r"?P<referer>\S+"
    user_agent = r"?P<user_agent>[^\"]*"
    forwardedfor=r"?P<forwardedfor>\S+"
    upstream_addr = r"?P<upstream_addr>.*"
    requesttime=r"?P<requesttime>.*"
    responsetime=r"?P<responsetime>.*"
    host = r"?P<host>\S+"
    p=re.compile(r"(%s)\s-\s(%s)\s\[(%s)/(%s)/(%s)\:(%s)\s[\S]+\]\s\"(%s)\s(%s)\s(%s)\"\s(%s)\s(%s)\s\"(%s)\"\s\"(%s)\"\s\"(%s)\"\s\"(%s)\"\s\"(%s)\"\s\"(%s)\"\s(%s) " \
                     %(ip,user,date,month,year,log_time,method,request,protocal,status,bodyBytesSent,referer,user_agent,forwardedfor,upstream_addr,requesttime,responsetime,host),re.VERBOSE)
    m = re.findall(p, self.logline)
        return m
        
if __name__ == "__main__":    
    start = time.clock() 
    
    logline='11.11.7.21 - - [22/Nov/2017:00:28:46 +0800] "POST /xyz/qn_cb HTTP/1.1" 200 218 "-" "qiniu-callback/1.0" "-" "1.25.69.11:8080" "0.008" "0.008" x.163.com'
    nlp=NgnixLogParse(logline)
    m=nlp.logparse()
    print (m)
    print (m[0][6])
    
    end = time.clock()    
    print('finish all in %s' % str(end - start))
```


执行结果：





```
[('11.11.7.21', '-','22', 'Nov', '2017', '00:28:46', 'POST', '/xyz/qn_cb', 'HTTP/1.1', '200', '218', 'qiniu-callback/1.0', '-','1.25.69.11:8080', '0.008', '0.008', 'x.163.com')]
/xyz/qn_cb
finish all in 0.00115241167476
```










