
# Hive中UDF函数包含HttpClient依赖冲突 - 北京小辉的博客 - CSDN博客
原
# Hive中UDF函数包含HttpClient依赖冲突
2019年04月11日 20:05:32[silentwolfyh](https://me.csdn.net/silentwolfyh)阅读数：16标签：[Hive																](https://so.csdn.net/so/search/s.do?q=Hive&t=blog)[HttpClient																](https://so.csdn.net/so/search/s.do?q=HttpClient&t=blog)[Hive中UDF																](https://so.csdn.net/so/search/s.do?q=Hive中UDF&t=blog)[							](https://so.csdn.net/so/search/s.do?q=HttpClient&t=blog)[																					](https://so.csdn.net/so/search/s.do?q=Hive&t=blog)个人分类：[【大数据】Hive																](https://yuhui.blog.csdn.net/article/category/5778623)
[																								](https://so.csdn.net/so/search/s.do?q=Hive&t=blog)


版权声明：本文为博主原创文章，出处为 http://blog.csdn.net/silentwolfyh					https://yuhui.blog.csdn.net/article/details/89218818
## 目录
### 一、错误信息
### 二、问题分析及解决
**第一：定位代码如下**
**第二：查看CDH5.14.4中httpclient的版本**
**第三：替换pom中的依赖包**
**第四：有jar包冲突了需要排除，如下所示**
### 三、httpclient-4.2.5版本Get，Post方法
### 四、httpclient-4.5.3版本Get，Post方法
### 一、错误信息：
```
Caused by: org.apache.hadoop.hive.ql.metadata.HiveException: Unable to execute method public java.lang.String aks.AksDecryptUDF.evaluate(java.lang.String)  on object aks.AksDecryptUDF@4eb30d44 of class aks.AksDecryptUDF with arguments {CCnQwueHtw7LQmTYPWBkrg==:java.lang.String} of size 1
	at org.apache.hadoop.hive.ql.exec.FunctionRegistry.invoke(FunctionRegistry.java:993)
	at org.apache.hadoop.hive.ql.udf.generic.GenericUDFBridge.evaluate(GenericUDFBridge.java:182)
	at org.apache.hadoop.hive.ql.exec.ExprNodeGenericFuncEvaluator._evaluate(ExprNodeGenericFuncEvaluator.java:186)
	at org.apache.hadoop.hive.ql.exec.ExprNodeEvaluator.evaluate(ExprNodeEvaluator.java:77)
	at org.apache.hadoop.hive.ql.exec.ExprNodeEvaluator.evaluate(ExprNodeEvaluator.java:65)
	at org.apache.hadoop.hive.ql.exec.SelectOperator.processOp(SelectOperator.java:77)
	at org.apache.hadoop.hive.ql.exec.Operator.forward(Operator.java:815)
	at org.apache.hadoop.hive.ql.exec.TableScanOperator.processOp(TableScanOperator.java:98)
	at org.apache.hadoop.hive.ql.exec.MapOperator\$MapOpCtx.forward(MapOperator.java:157)
	at org.apache.hadoop.hive.ql.exec.MapOperator.process(MapOperator.java:497)
	... 9 more
Caused by: java.lang.reflect.InvocationTargetException
	at sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
	at sun.reflect.NativeMethodAccessorImpl.invoke(NativeMethodAccessorImpl.java:62)
	at sun.reflect.DelegatingMethodAccessorImpl.invoke(DelegatingMethodAccessorImpl.java:43)
	at java.lang.reflect.Method.invoke(Method.java:498)
	at org.apache.hadoop.hive.ql.exec.FunctionRegistry.invoke(FunctionRegistry.java:969)
	... 18 more
Caused by: java.lang.NoSuchFieldError: INSTANCE
	at org.apache.http.conn.ssl.SSLConnectionSocketFactory.<clinit>(SSLConnectionSocketFactory.java:146)
	at org.apache.http.impl.client.HttpClientBuilder.build(HttpClientBuilder.java:964)
	at org.apache.http.impl.client.HttpClients.createDefault(HttpClients.java:56)
	at aks.AksDecryptUDF.doPost(AksDecryptUDF.java:53)
	at aks.AksDecryptUDF.evaluate(AksDecryptUDF.java:37)
	... 23 more
```
### 二、问题分析及解决
**第一：定位代码如下**
```
at aks.AksDecryptUDF.doPost(AksDecryptUDF.java:53)
```
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190411195842188.png)
**第二：查看CDH5.14.4中httpclient的版本**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190411195947101.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly95dWh1aS5ibG9nLmNzZG4ubmV0,size_16,color_FFFFFF,t_70)
**第三：替换pom中的依赖包**
```
<dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
            <version>4.2.5</version>
        </dependency>
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
            <version>4.5.3</version>
        </dependency>
```
**第四：有jar包冲突了需要排除，如下所示**
```
<dependency>
            <groupId>org.apache.hadoop</groupId>
            <artifactId>hadoop-common</artifactId>
            <version>2.6.0</version>
            <exclusions>
                <exclusion>
                    <groupId>commons-httpclient</groupId>
                    <artifactId>commons-httpclient</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
        <dependency>
            <groupId>org.apache.hive</groupId>
            <artifactId>hive-exec</artifactId>
            <version>1.1.0</version>
            <exclusions>
                <exclusion>
                    <groupId>commons-httpclient</groupId>
                    <artifactId>commons-httpclient</artifactId>
                </exclusion>
            </exclusions>
        </dependency>
```
### 三、httpclient-4.2.5版本Get，Post方法
```
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.ParseException;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.List;
public class HttpUtils {
    /**
     * 发送 post请求访问本地应用并根据传递参数不同返回不同结果
     */
    public static String doPost(String url, String reqEncoding, String respEncoding, List<NameValuePair> param) {
        org.apache.http.client.HttpClient httpclient = new DefaultHttpClient();
        String resStr = "";
        // 创建httppost
        HttpPost httppost = new HttpPost(url);
        // 创建参数队列
        List<NameValuePair> formparams = param;
        UrlEncodedFormEntity uefEntity;
        try {
            uefEntity = new UrlEncodedFormEntity(formparams, reqEncoding);
            httppost.setEntity(uefEntity);
            HttpResponse response;
            response = httpclient.execute(httppost);
            HttpEntity entity = response.getEntity();
            if (entity != null) {
                resStr = EntityUtils.toString(entity, respEncoding);
            }
        } catch (ClientProtocolException e) {
            e.printStackTrace();
        } catch (UnsupportedEncodingException e1) {
            e1.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            // 关闭连接,释放资源
            httpclient.getConnectionManager().shutdown();
        }
        return resStr;
    }
    /**
     * 发送 get请求
     */
    public String get(String url) {
        org.apache.http.client.HttpClient httpclient = new DefaultHttpClient();
        String resStr = "";
        try {
            // 创建httpget.
            HttpGet httpget = new HttpGet(url);
            // 执行get请求.
            HttpResponse response = httpclient.execute(httpget);
            // 获取响应实体
            HttpEntity entity = response.getEntity();
            // 打印响应状态
            System.out.println(response.getStatusLine());
            if (entity != null) {
                // 打印响应内容长度
                System.out.println("Response content length: "
                        + entity.getContentLength());
                // 打印响应内容
                System.out.println("Response content: "
                        + EntityUtils.toString(entity));
                resStr = EntityUtils.toString(entity);
            }
        } catch (ClientProtocolException e) {
            e.printStackTrace();
        } catch (ParseException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            // 关闭连接,释放资源
            //httpclient.getConnectionManager().shutdown();
        }
        return resStr;
    }
    public static void main(String[] args) {
        List<NameValuePair> list = new ArrayList<NameValuePair>();
        list.add(new BasicNameValuePair("key01", "value01"));//登录参数
        list.add(new BasicNameValuePair("key02","value02" ));//登录参数
        String url = "http://IP:8002/xiaohui";
        System.out.println(HttpUtils.doPost(url, "UTF-8", "UTF-8", list));//先post登录也卖弄
    }
}
```
### 四、httpclient-4.5.3版本Get，Post方法
```
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.*;
import java.util.Map.Entry;
import org.apache.http.HttpEntity;
import org.apache.http.NameValuePair;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
public class HttpClient4 {
    public static String doGet(String url) {
        CloseableHttpClient httpClient = null;
        CloseableHttpResponse response = null;
        String result = "";
        try {
            // 通过址默认配置创建一个httpClient实例
            httpClient = HttpClients.createDefault();
            // 创建httpGet远程连接实例
            HttpGet httpGet = new HttpGet(url);
            // 设置请求头信息，鉴权
            httpGet.setHeader("Authorization", "Bearer da3efcbf-0845-4fe3-8aba-ee040be542c0");
            // 设置配置请求参数
            RequestConfig requestConfig = RequestConfig.custom().setConnectTimeout(35000)// 连接主机服务超时时间
                    .setConnectionRequestTimeout(35000)// 请求超时时间
                    .setSocketTimeout(60000)// 数据读取超时时间
                    .build();
            // 为httpGet实例设置配置
            httpGet.setConfig(requestConfig);
            // 执行get请求得到返回对象
            response = httpClient.execute(httpGet);
            // 通过返回对象获取返回数据
            HttpEntity entity = response.getEntity();
            // 通过EntityUtils中的toString方法将结果转换为字符串
            result = EntityUtils.toString(entity);
        } catch (ClientProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            // 关闭资源
            if (null != response) {
                try {
                    response.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (null != httpClient) {
                try {
                    httpClient.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }
    public static String doPost(String url, Map<String, Object> paramMap) {
        CloseableHttpClient httpClient = null;
        CloseableHttpResponse httpResponse = null;
        String result = "";
        // 创建httpClient实例
        httpClient = HttpClients.createDefault();
        // 创建httpPost远程连接实例
        HttpPost httpPost = new HttpPost(url);
        // 配置请求参数实例
        RequestConfig requestConfig = RequestConfig.custom().setConnectTimeout(35000)// 设置连接主机服务超时时间
                .setConnectionRequestTimeout(35000)// 设置连接请求超时时间
                .setSocketTimeout(60000)// 设置读取数据连接超时时间
                .build();
        // 为httpPost实例设置配置
        httpPost.setConfig(requestConfig);
        // 设置请求头
        httpPost.addHeader("Content-Type", "application/x-www-form-urlencoded");
        // 封装post请求参数
        if (null != paramMap && paramMap.size() > 0) {
            List<NameValuePair> nvps = new ArrayList<NameValuePair>();
            // 通过map集成entrySet方法获取entity
            Set<Entry<String, Object>> entrySet = paramMap.entrySet();
            // 循环遍历，获取迭代器
            Iterator<Entry<String, Object>> iterator = entrySet.iterator();
            while (iterator.hasNext()) {
                Entry<String, Object> mapEntry = iterator.next();
                nvps.add(new BasicNameValuePair(mapEntry.getKey(), mapEntry.getValue().toString()));
            }
            // 为httpPost设置封装好的请求参数
            try {
                httpPost.setEntity(new UrlEncodedFormEntity(nvps, "UTF-8"));
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }
        try {
            // httpClient对象执行post请求,并返回响应参数对象
            httpResponse = httpClient.execute(httpPost);
            // 从响应对象中获取响应内容
            HttpEntity entity = httpResponse.getEntity();
            result = EntityUtils.toString(entity);
        } catch (ClientProtocolException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            // 关闭资源
            if (null != httpResponse) {
                try {
                    httpResponse.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
            if (null != httpClient) {
                try {
                    httpClient.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
        return result;
    }
    public static void main(String[] args) {
        HashMap<String, Object> map = new HashMap<>();
        String url = "";
        map.put("key01", "value01");
        map.put("key02", "value01");
        HttpClient4.doPost(url, map);
    }
}
```

