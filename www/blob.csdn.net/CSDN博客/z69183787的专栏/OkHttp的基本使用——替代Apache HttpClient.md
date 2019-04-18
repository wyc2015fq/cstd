# OkHttp的基本使用——替代Apache HttpClient - z69183787的专栏 - CSDN博客
2018年01月09日 11:15:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2026

http是现在主流应用使用的网络请求方式, 用来交换数据和内容, 有效的使用HTTP可以使你的APP 变的更快和减少流量的使用
OkHttp 是一个很棒HTTP客户端:
- 支持SPDY, 可以合并多个到同一个主机的请求
-  使用连接池技术减少请求的延迟(如果SPDY是可用的话)
-  使用GZIP压缩减少传输的数据量
-  缓存响应避免重复的网络请求
OkHttp可以替换Apache的HttpClient
OkHttp支持2.3和以上版本，对于java，需要jdk1.7 ，OkHttp需要依赖Okio包
下面上demo
1：使用get方式请求，获取响应
```java
import java.io.IOException;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.Response;
public class GetExample {
  OkHttpClient client = new OkHttpClient();
  String run(String url) throws IOException {
    Request request = new Request.Builder()
        .url(url)
        .build();
    try (Response response = client.newCall(request).execute()) {
      return response.body().string();
    }
  }
  public static void main(String[] args) throws IOException {
    GetExample example = new GetExample();
    String response = example.run("https://raw.github.com/square/okhttp/master/README.md");
    System.out.println(response);
  }
}
```
2：使用post向服务器发送请求
```java
import java.io.IOException;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;
public class PostExample {
  public static final MediaType JSON
      = MediaType.parse("application/json; charset=utf-8");
  OkHttpClient client = new OkHttpClient();
  String post(String url, String json) throws IOException {
    RequestBody body = RequestBody.create(JSON, json);
    Request request = new Request.Builder()
        .url(url)
        .post(body)
        .build();
    try (Response response = client.newCall(request).execute()) {
      return response.body().string();
    }
  }
  String bowlingJson(String player1, String player2) {
    return "{'winCondition':'HIGH_SCORE',"
        + "'name':'Bowling',"
        + "'round':4,"
        + "'lastSaved':1367702411696,"
        + "'dateStarted':1367702378785,"
        + "'players':["
        + "{'name':'" + player1 + "','history':[10,8,6,7,8],'color':-13388315,'total':39},"
        + "{'name':'" + player2 + "','history':[6,10,5,10,10],'color':-48060,'total':41}"
        + "]}";
  }
  public static void main(String[] args) throws IOException {
    PostExample example = new PostExample();
    String json = example.bowlingJson("Jesse", "Jake");
    String response = example.post("http://www.roundsapp.com/post", json);
    System.out.println(response);
  }
}
```
