# httpclient SocketTimeout ConnectTimeout ConnectionRequestTimeout - z69183787的专栏 - CSDN博客
2018年01月09日 11:28:40[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1061
[http://m.blog.csdn.net/btlas/article/details/53710854](http://m.blog.csdn.net/btlas/article/details/53710854)
# httpclient超时时间
# 说明
### ConnectionRequestTimeout
httpclient使用连接池来管理连接，这个时间就是从连接池获取连接的超时时间，可以想象下数据库连接池
### ConnectTimeout
连接建立时间，三次握手完成时间
### SocketTimeout
数据传输过程中数据包之间间隔的最大时间
下面重点说下SocketTimeout，比如有如下图所示的http请求
![这里写图片描述](https://img-blog.csdn.net/20161217181959985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnRsYXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
虽然报文(“abc”)返回总共用了6秒，如果`SocketTimeout`设置成4秒，实际程序执行的时候是不会抛出
```
java.net.SocketTimeoutException:
 Read timed out
```
异常的。
因为`SocketTimeout`的值表示的是“a”、”b”、”c”这三个报文，每两个相邻的报文的间隔时间不能超过`SocketTimeout`。
## 程序验证：
### server端(python3)：每隔3秒返回a、b、c
```python
__author__ = 'yanglikun'
from flask import Flask
from flask import Response
import time
app = Flask(__name__)
@app.route("/")
def hello():
    def generate():
        for row in ["a","b","c"]:
            time.sleep(3)
            yield row;
    return Response(generate());
if __name__ == "__main__":
    app.run(host='0.0.0.0',port=8897)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
### java客户端(httpclient 4.3.6)
```java
CloseableHttpClient client = HttpClientBuilder.create().build();
        RequestConfig config = RequestConfig.custom().setConnectionRequestTimeout(5)
                .setConnectTimeout(5)
                .setSocketTimeout(6000).build();
        HttpGet httpGet = new HttpGet("http://192.168.147.90:8897/");
        httpGet.setConfig(config);
        long begin = System.currentTimeMillis();
        String respStr = null;
        try {
            CloseableHttpResponse resp = client.execute(httpGet);
            respStr = EntityUtils.toString(resp.getEntity());
        } catch (IOException e) {
            e.printStackTrace();
        }
        System.err.println("respStr:" + respStr);
        System.err.println("end:" + (System.currentTimeMillis() - begin));
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
### 客户端执行结果
虽然设置SocketTimeout为6000(6秒)，但是程序执行了9秒，也没有抛出`java.net.SocketTimeoutException: Read timed out`异常 
![这里写图片描述](https://img-blog.csdn.net/20161217183155786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnRsYXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### wireshark抓包结果
可能会想实际的报文是不是每经过3秒返回一个报文呢，下面是通过wireshark抓包的结果 
其中 
1：返回字母a的TCP报文 
2：返回字母b的TCP报文 
3：返回字母c的TCP报文 
根据time列可以看出TCP数据传输报文之间的间隔就是python程序里面写的 3秒。BTW，可以复习下TCP的另一个知识，每个报文后还有一个ACK的报文，就是tcp客户端的回执报文
[点击查看大图](http://yanglikun.github.io/share/httpclient-timeout.jpg)
![这里写图片描述](https://img-blog.csdn.net/20161217184208417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnRsYXM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
