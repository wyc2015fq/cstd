# 借助百度云中的百度人脸识别用java开发一个人脸识别 - z69183787的专栏 - CSDN博客
2017年08月17日 16:24:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3307
[http://m.blog.csdn.net/u010651369/article/details/64439090](http://m.blog.csdn.net/u010651369/article/details/64439090)
1.官网文档必须看
http://ai.baidu.com/docs
![](https://static.oschina.net/uploads/space/2017/0321/091109_hCT9_1444646.png)
2.在管理中心创建应用及查看相关参数 AppID APIKey SecretKey都需要保存起来
![](https://static.oschina.net/uploads/space/2017/0321/091302_f21B_1444646.png)
3.搬砖测试
项目需要的lib包，基本下载官网的SDK就都会包含。
![](https://static.oschina.net/uploads/space/2017/0321/094040_e1J9_1444646.png)
```java
```java
package com.xs.demo.common;
//相关的必要参数可以保存到常量
public class APIConstants {
	//百度人脸识别应用id
	public static final String APPID = "管理中心获得";
	//百度人脸识别应用apikey
	public static final String API_KEY = "管理中心获得";
	//百度人脸识别应用sercetkey
	public static final String SERCET_KEY = "管理中心获得";
	//百度人脸识别token 有效期一个月
	public static final String TOKEN = "为了测试，将生成的key先存一下";
}
```
```
4.获取AccessToken
官网文档说明http://ai.baidu.com/docs#Beginner-Auth
向授权服务地址`https://aip.baidubce.com/oauth/2.0/token`发送请求（推荐使用POST），并在URL中带上以下参数：
- grant_type: 必须参数，固定为“client_credentials”；
- client_id: 必须参数，应用的API Key；
- client_secret: 必须参数，应用的Secret Key；
```java
//获取AccessToken 有效期一个月
public class AITest {
	public static void main(String[] args) throws Exception {;
		String access_token_url = "https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials" +
				"&client_id="+APIContants.API_KEY
			   +"&client_secret="+ APIContants.SERCET_KEY;
		HttpResponse response = HttpUtils.doPostBD(access_token_url,new HashMap<String, String>(),new HashMap<String, String>());
		System.out.println(EntityUtils.toString(response.getEntity()));
		
	}
}
```
```
//返回的JSON数据。只需要access_token即可
{"access_token":"24.39b6fc97e87058ed6edb92ad68782c29.2592000.1492651482.11111-11111","session_key":"9mzdCP1XKXZ5yipkl7ubftudl2reGQybIG25NqWi9vqeSFFjxigLFl+WpfDW0PyUt7EP\/Uyn74r31lj3d4kVr4xkbkSk","scope":"public vis-faceverify_faceverify vis-faceattribute_faceattribute wise_adapt lebo_resource_base lightservice_public hetu_basic lightcms_map_poi kaidian_kaidian wangrantest_test wangrantest_test1 bnstest_test1 bnstest_test2 ApsMisTest_Test\u6743\u9650 vis-classify_flower","refresh_token":"25.3c8370eaeb8af4f8367a38b6dbefcef9.315360000.1805419482.11111-11111","session_secret":"ff06850552b385cff19de958e435e6a4","expires_in":2592000}
```
5.HttpUtils工具类代码
```java
```java
package com.xs.demo.util;
import java.io.UnsupportedEncodingException;
import java.net.URLEncoder;
import java.security.KeyManagementException;
import java.security.NoSuchAlgorithmException;
import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import javax.net.ssl.SSLContext;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;
import org.apache.commons.lang.StringUtils;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpDelete;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.conn.ClientConnectionManager;
import org.apache.http.conn.scheme.Scheme;
import org.apache.http.conn.scheme.SchemeRegistry;
import org.apache.http.conn.ssl.SSLSocketFactory;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.message.BasicNameValuePair;
/**
 * httpClient工具类
 * @author 小帅帅丶
 * @date  2017-3-21上午09:28:21
 */
public class HttpUtils {
	/**
	 * 
	 * @Title doPostBD
	 * @param url 接口地址
	 * @param method 请求方式
	 * @param headers 
	 * @param bodys
	 * @return response
	 * @throws Exception
	 * @author 小帅帅丶
	 * @date 2017-3-20
	 *
	 */
	public static HttpResponse doPostBD(String url, 
			Map<String, String> headers, 
			Map<String, String> bodys)
            throws Exception {    	
    	HttpClient httpClient = wrapClient(url);
    	HttpPost request = new HttpPost(url);
        for (Map.Entry<String, String> e : headers.entrySet()) {
        	request.addHeader(e.getKey(), e.getValue());
        }
        if (bodys != null) {
            List<NameValuePair> nameValuePairList = new ArrayList<NameValuePair>();
            for (String key : bodys.keySet()) {
                nameValuePairList.add(new BasicNameValuePair(key, bodys.get(key)));
            }
            UrlEncodedFormEntity formEntity = new UrlEncodedFormEntity(nameValuePairList, "utf-8");
            formEntity.setContentType("application/x-www-form-urlencoded; charset=UTF-8");
            request.setEntity(formEntity);
        }
        return httpClient.execute(request);
    }
	private static HttpClient wrapClient(String host) {
		HttpClient httpClient = new DefaultHttpClient();
		if (host.startsWith("https://")) {
			sslClient(httpClient);
		}
		return httpClient;
	}
	private static void sslClient(HttpClient httpClient) {
        try {
            SSLContext ctx = SSLContext.getInstance("TLS");
            X509TrustManager tm = new X509TrustManager() {
                public X509Certificate[] getAcceptedIssuers() {
                    return null;
                }
                public void checkClientTrusted(X509Certificate[] xcs, String str) {
                	
                }
                public void checkServerTrusted(X509Certificate[] xcs, String str) {
                	
                }
            };
            ctx.init(null, new TrustManager[] { tm }, null);
            SSLSocketFactory ssf = new SSLSocketFactory(ctx);
            ssf.setHostnameVerifier(SSLSocketFactory.ALLOW_ALL_HOSTNAME_VERIFIER);
            ClientConnectionManager ccm = httpClient.getConnectionManager();
            SchemeRegistry registry = ccm.getSchemeRegistry();
            registry.register(new Scheme("https", 443, ssf));
        } catch (KeyManagementException ex) {
            throw new RuntimeException(ex);
        } catch (NoSuchAlgorithmException ex) {
        	throw new RuntimeException(ex);
        }
    }
}
```
```
6.检测人脸美丑及年龄
http://ai.baidu.com/docs#FACE官网文档
请求参数
||是否必选|类型|说明|
|----|----|----|----|
|image|是|string|base64编码后的图片数据，图片大小不超过2M。|
|max_face_num|否|uint32|最多处理人脸数目，默认值1|
|face_fields|否|string|包括age,beauty,expression,faceshape,gender,glasses,landmark,race,qualities信息，逗号分隔，默认只返回人脸框、概率和旋转角度。|
- 
URL参数：
|参数|值|
|----|----|
|access_token|通过API Key和Secret Key获取的access_token,参考“[Access Token获取](http://ai.baidu.com/docs#Beginner-Auth)”|
- Header如下：
|参数|值|
|----|----|
|Content-Type|application/x-www-form-urlencoded|
- Body中数据如下：
|参数|值|
|----|----|
|image|图像base64编码|
|max_face_num|非必填，默认1|
|face_fields|非必填|
```java
```java
package com.xs.demo.test;
import java.util.HashMap;
import java.util.Map;
import org.apache.http.HttpResponse;
import org.apache.http.util.EntityUtils;
import com.xs.demo.common.APIConstants;
import com.xs.demo.util.HttpUtils;
import com.xs.demo.util.Image4Base64;
//人脸识别 百度一张如花的图片
public class Test {
	public static void main(String[] args) {
		/****************/
		String Filepath = "F:/ruhua.jpg";
		String image = Image4Base64.GetImageStr(Filepath);
		String url = "https://aip.baidubce.com/rest/2.0/face/v1/detect?access_token="+APIConstants.TOKEN;
		Map<String, String> headers = new HashMap<String, String>();
		Map<String, String> bodys = new HashMap<String, String>();
		headers.put("Content-Type", "application/x-www-form-urlencoded");
		bodys.put("image", image);
		bodys.put("face_fields", "age,beauty,expression,gender,glasses,race,qualities");
		try {
			HttpResponse response = HttpUtils.doPostBD(url,headers,bodys);
			System.out.println(EntityUtils.toString(response.getEntity()));
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
```
```
7.返回JSON字符串内容
age 年龄  （目前官网还是没有说明年龄的误差范围）
beauty 美丑分数 0-100 越大值越美（之前官网写的是0-1在我发现错误时，提交官网，官网及时做了修改）
如花识别年龄38  美丑评分21  表示不太合理
```
{
  "result": [{
    "expression": 0,
    "face_probability": 1,
    "glasses": 0,
    "location": {
      "height": 155,
      "width": 136,
      "left": 103,
      "top": 49
    },
    "beauty": 21.201513290405,
    "race": "yellow",
    "expression_probablity": 0.77510279417038,
    "rotation_angle": 9,
    "yaw": 2.6519072055817,
    "roll": 10.0813331604,
    "qualities": {
      "completeness": 0,
      "occlusion": {
        "left_eye": 0,
        "chin": 0,
        "mouth": 0,
        "right_cheek": 0,
        "left_cheek": 0,
        "nose": 0,
        "right_eye": 0
      },
      "blur": 0,
      "type": {
        "cartoon": 0.0060238856822252,
        "human": 0.99397611618042
      },
      "illumination": 0
    },
    "gender_probability": 0.93728905916214,
    "age": 37.866649627686,
    "gender": "male",
    "glasses_probability": 0.99999260902405,
    "race_probability": 0.99436098337173,
    "pitch": -17.080112457275
  }],
  "result_num": 1,
  "log_id": 2045651588
}
```
8.SDK调用
```cpp
```java
/**
 * 基于百度SDK提供的方法进行调用
 * @author 小帅帅丶
 * @date  2017-3-20下午06:00:05
 */
public class Sample {
	public static void main(String[] args) {
		//初始化一个FaceClient
		AipFace face = new AipFace(APIConstants.APPID,APIConstants.API_KEY,APIConstants.SERCET_KEY);
		//可选：设置网络连接参数
		face.setConnectionTimeoutInMillis(60000);
		face.setSocketTimeoutInMillis(60000);
		//调用API
		HashMap map = new HashMap();
		//
		map.put("face_fields", "age,beauty,expression,gender,glasses,race,qualities");
		String path = "F:/ruhua.jpg";
		AipRequest aipRequest = new AipRequest();
		aipRequest.setBody(map);
		JSONObject result = face.detect(Image4Base64.GetImageStrByte(path),map);
		System.out.println(result.toString(2));
	}
}
```
```
以上就是百度人脸识别API的调用测试流程及demo
