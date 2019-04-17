# Java快速验证项目API服务是否发布成功 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月24日 15:02:55[boonya](https://me.csdn.net/boonya)阅读数：179








这里有三个知识点：a.HTTP的协议请求方式;b.Jackson的使用；c.Java的HttpClient模拟Http请求响应。最近通过.Net  MVC 实现了一套应用服务接口，因为Java同事始终无法正确的访问，我只好自己动手给写了个示例程序。Java也是我一直热爱从事开发的语言，因为工作需要通过.Net MVC提供了一套基于音视频服务的接口，所以在此记录下。

### Maven 依赖配置

加入两类配置HttpClient和Jackson。

```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.boonya.httpclient</groupId>
    <artifactId>HttpClientTest</artifactId>
    <version>1.0-SNAPSHOT</version>

    <properties>
        <jackjson.version>2.8.8</jackjson.version>
    </properties>

    <dependencies>
        <dependency>
            <groupId>org.apache.httpcomponents</groupId>
            <artifactId>httpclient</artifactId>
            <version>4.5.2</version>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-core</artifactId>
            <version>${jackjson.version}</version>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-annotations</artifactId>
            <version>${jackjson.version}</version>
        </dependency>
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
            <version>${jackjson.version}</version>
        </dependency>

    </dependencies>

</project>
```

### Jackson Json工具

最简单的就是提交JSON格式的数据，JSON的工具是很有必要的，可以实现对象的序列化和反序列化操作。

```java
package com.boonya.httpclient;

import com.fasterxml.jackson.core.JsonFactory;
import com.fasterxml.jackson.core.JsonGenerator;
import com.fasterxml.jackson.databind.ObjectMapper;

import java.io.IOException;
import java.io.StringWriter;

/**
 * @ClassName: JSONUtil
 * @Description: TODO(功能描述)
 * @author: pengjunlin
 * @company: ******科技有限公司
 * @date 2018-07-21
 */
public class JSONUtil {

    /**
     * 对象转json
     * @param obj object 对象
     * @return String
     * @throws IOException
     */
    public static String toJson(Object obj) throws IOException {

        ObjectMapper mapper = new ObjectMapper();
        StringWriter sw = new StringWriter();
        JsonGenerator gen = new JsonFactory().createJsonGenerator(sw);
        mapper.writeValue(gen, obj);
        gen.close();
        return sw.toString();
    }

    /**
     * json转对象
     * @param jsonStr json字符串
     * @param objClass 类名.class
     * @return 泛型
     * @throws Exception
     */
    public static <T> T toObject(String jsonStr, Class<T> objClass)
            throws Exception {

        ObjectMapper mapper = new ObjectMapper();

        return mapper.readValue(jsonStr, objClass);
    }
}
```

### HttpClient工具

这里提供两种请求方式：GET和POST。

```java
package com.boonya.httpclient;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.HttpStatus;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.util.EntityUtils;
import java.io.IOException;
import java.nio.charset.Charset;

/**
 * @ClassName: HttpClientUtil
 * @Description: TODO(HttpClientUtil工具类)
 * @author: pengjunlin
 * @company: ******科技有限公司
 * @date 2018-07-19
 */
public class HttpClientUtil{

    /**
     * get请求，参数拼接在地址上
     * @param url 请求地址加参数
     * @return 响应
     */
    public static String get(String url)
    {
        String result = null;
        CloseableHttpClient httpClient = HttpClients.createDefault();
        HttpGet get = new HttpGet(url);
        get.addHeader("Content-Type","application/json; charset=utf-8");
        get.setHeader("Accept", "application/json");
        CloseableHttpResponse response = null;
        try {
            response = httpClient.execute(get);
            if(response != null && response.getStatusLine().getStatusCode() == 200)
            {
                HttpEntity entity = response.getEntity();
                result =entity.getContent().toString();
            }
            return result;
        } catch (IOException e) {
            e.printStackTrace();
        }finally {
            try {
                httpClient.close();
                if(response != null)
                {
                    response.close();
                }
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    public static  String  post(String url,String jsonParams){
        HttpClient httpClient =HttpClients.createDefault();
        HttpPost httpPost=new HttpPost(url);
        httpPost.addHeader("Content-Type","application/json; charset=utf-8");
        httpPost.setHeader("Accept", "application/json");
        httpPost.setEntity(new StringEntity(jsonParams, Charset.forName("UTF-8")));
        try {
            HttpResponse response = httpClient.execute(httpPost);
            int statusCode = response.getStatusLine().getStatusCode();
            if (statusCode != HttpStatus.SC_OK) {
                System.err.println("Method failed:" + response.getStatusLine());
            }else{
                String result = EntityUtils.toString(response.getEntity());

                return result;
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

}
```

### 编写接口业务实体

这里是一个简单的音视频资源查询实体类：

```java
package com.boonya.httpclient;

import java.io.Serializable;
import java.util.Date;

/**
 * @ClassName: FileSourceMultiRequest
 * @Description: TODO(功能描述)
 * @author: pengjunlin
 * @company: ******科技有限公司
 * @date 2018-07-19
 */
public class FileSourceMultiRequest implements Serializable
{

    /// <summary>
    /// SIM卡号【必填】
    /// </summary>
    public String[] Sims ;

    /// <summary>
    /// 通道号(Channel小于0通道不生效)
    /// </summary>
    public int Channel ;

    /// <summary>
    /// 报警标志(最小值为0，不能为负数)
    /// </summary>
    public long[] Alarms ;

    /// <summary>
    /// 存储器类型(0:主存储器或灾备存储器，1:主存储器，2:灾备存储器)【必填】
    /// </summary>
    public int StorageType ;

    /// <summary>
    /// 码流类型(0:主码流或子码流，1:主码流，2:子码流)【必填】
    /// </summary>
    public int StreamType ;

    /// <summary>
    /// 开始时间(yyyy-MM-dd HH:mm:ss)【必填】
    /// </summary>
    public Date StartTime ;

    /// <summary>
    /// 结束时间(yyyy-MM-dd HH:mm:ss)【必填】
    /// </summary>
    public Date EndTime ;

    /// <summary>
    /// 音视频资源类型(0:音视频，1:音频，2:视频，3:视频或音频)
    /// </summary>
    public int DataType;

    public String[] getSims() {
        return Sims;
    }

    public void setSims(String[] sims) {
        Sims = sims;
    }

    public int getChannel() {
        return Channel;
    }

    public void setChannel(int channel) {
        Channel = channel;
    }

    public long[] getAlarms() {
        return Alarms;
    }

    public void setAlarms(long[] alarms) {
        Alarms = alarms;
    }

    public int getStorageType() {
        return StorageType;
    }

    public void setStorageType(int storageType) {
        StorageType = storageType;
    }

    public int getStreamType() {
        return StreamType;
    }

    public void setStreamType(int streamType) {
        StreamType = streamType;
    }

    public Date getStartTime() {
        return StartTime;
    }

    public void setStartTime(Date startTime) {
        StartTime = startTime;
    }

    public Date getEndTime() {
        return EndTime;
    }

    public void setEndTime(Date endTime) {
        EndTime = endTime;
    }

    public int getDataType() {
        return DataType;
    }

    public void setDataType(int dataType) {
        DataType = dataType;
    }
}
```

### 测试访问接口

```java
package com.boonya.httpclient;
import java.util.Date;

/**
 * @ClassName: MainTest
 * @Description: TODO(功能描述)
 * @author: pengjunlin
 * @company: ******科技有限公司
 * @date 2018-07-19
 */
public class MainTest {

    public static void main(String[] args) throws Exception{

        String url="http://10.10.10.10:18100/api/WebService/MultiSIMAlarmForJsonParam";
        FileSourceMultiRequest request=new FileSourceMultiRequest();
        request.setSims(new String[]{"",""});
        request.setChannel(1);
        request.setAlarms(new long[]{1});
        Date start=new Date();
        start.setTime(start.getTime()-86400);
        request.setStartTime(start);
        Date end=new Date();
        end.setTime(end.getTime()+86400);
        request.setEndTime(end);
        request.setStorageType(0);
        request.setStreamType(0);
        request.setDataType(0);

        System.out.println(HttpClientUtil.post(url,JSONUtil.toJson(request)));

    }
}
```

运行此测试：

![](https://img-blog.csdn.net/20180724145947216?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

程序接口正常返回数据。



