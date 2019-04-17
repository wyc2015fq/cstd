# Android客户端使用HttpClient发起web数据访问 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年09月24日 12:32:45[boonya](https://me.csdn.net/boonya)阅读数：5103








  HttpClient与服务器数据交互方式：HttpPost和HttpGet 分别对应Post和Get提交。因为要做Android客户端的缘故，所以就必须实现客户端与服务器实现数据交互，以保证数据链条的通畅，实现数据闭环。由于之前对Android客户端访问web数据没有设置权限，所以可以很好的访问系统资源，但是如果是开发应用这种方式就存在极大的安全隐患，一个Host或Get提交过去数据就获取到了，web端系统完全就是裸奔。所以web端权限管理很有必要，也就不用过于担心Android客户端带来的安全隐患。Android新手学习一般都会遇到问题，前两天就遇到web端设置了权限而Android客户端发起访问获取不到数据的问题。

      获取JSON遇到权限问题：[http://my.oschina.net/boonya/blog/317057 这篇文章和本文要讲的内容有点关系。](http://my.oschina.net/boonya/blog/317057)

#### 1.HttpPost与HttpGet实现用户登录和列表展现

编写一个测试Activity，MainActivity.




```java
package com.boonya.httpclienttest;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.protocol.HTTP;
import org.json.JSONArray;
import org.json.JSONObject;
import com.boonya.httpclienttest.utils.HtttpClientUtil;
import android.app.Activity;
import android.os.Bundle;
import android.util.Log;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Toast;

public class MainActivity extends Activity
{

	private static final String TAG = "MainActivity";

	private List<HashMap<String, Object>> videos = null;

	private HashMap<String, Object> video = null;

	private ListView listView = null;
	
	private static String loginurl = "http://192.168.1.147:8090/wtms/androidservice/login";
	
	private static String getdataurl = "http://192.168.1.147:8090/wtms/androidservice/videos.avd";

	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		listView = (ListView) findViewById(R.id.videos);
		
		postMethod();

		getMethod();
	}

	/**
	 * HttpGet获取服务器数据显示
	 * 
	 * @param url
	 */
	protected void getMethod()
	{
	    HttpGet request = new HttpGet(getdataurl);  
	    //request.setHeader("Cookie", HtttpClientUtil.getCookie());//设置cookie 
		try
		{
			//设置请求参数项  
            //request.setEntity(new UrlEncodedFormEntity(params, HTTP.UTF_8));  
            HttpClient client = HtttpClientUtil.getInstance();  
            //执行请求返回相应  
            HttpResponse response = client.execute(request);
			// 判断请求是否成功
			if (response.getStatusLine().getStatusCode() == 200)
			{ // 200表示请求成功

				StringBuilder builder = new StringBuilder();
				BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));

				String s = null;
				while ((s = bufferedReader.readLine()) != null)
				{
					builder.append(s);
				}

				// String out = EntityUtils.toString(entity, "UTF-8");
				String msg = builder.toString();
				
				Log.d("log", ">>>>执行 方法 getMethod()获取到范围值："+msg);

				JSONArray jsonArray = new JSONArray(msg);

				videos = new ArrayList<HashMap<String, Object>>();
				for (int i = 0; i < jsonArray.length(); i++)
				{
					JSONObject jsonObject = (JSONObject) jsonArray.get(i);
					int id = jsonObject.getInt("id");
					String name = jsonObject.getString("name");
					int timelength = jsonObject.getInt("time");

					video = new HashMap<String, Object>();
					video.put("id", id);
					video.put("name", name);
					video.put("timelength", "时长为:" + timelength);

					videos.add(video);
				}

				SimpleAdapter adapter = new SimpleAdapter(this, videos, R.layout.item, new String[]
				{ "name", "timelength" }, new int[]
				{ R.id.title, R.id.timelength });
				listView.setAdapter(adapter);

			}
		} catch (Exception e)
		{
			e.printStackTrace();
			Log.e(TAG, e.toString());
			Toast.makeText(MainActivity.this, "获取数据失败", Toast.LENGTH_LONG).show();
		}
	}

	/**
	 * HttpPost提交数据
	 */
	@SuppressWarnings(
	{ "unchecked", "rawtypes" })
	protected void postMethod()
	{

		try
		{
			// 使用ApacheHttp客户端进行连接(重要方法)
			HttpClient client = HtttpClientUtil.getInstance();

			// 如果是Get提交则创建HttpGet对象，否则创建HttpPost对象
			// POST提交的方式
			HttpPost request = new HttpPost(loginurl);
			// 如果是Post提交可以将参数封装到集合中传递
			List params = new ArrayList();
			params.add(new BasicNameValuePair("username", "test"));
			params.add(new BasicNameValuePair("password", "test"));
			// UrlEncodedFormEntity用于将集合转换为Entity对象
			request.setEntity(new UrlEncodedFormEntity(params,HTTP.UTF_8));
			try
			{
				// 获取相应消息
				HttpResponse response = client.execute(request);
				StringBuilder builder = new StringBuilder(); 
                BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(response.getEntity().getContent())); 
                // 操作cookie
               /* List<Cookie> cookies = ((AbstractHttpClient) client).getCookieStore().getCookies();
                if(cookies!=null&&cookies.size()>0)
                {
                	for (int i = 0; i < cookies.size(); i++)
					{
						HtttpClientUtil.setCookie(cookies.get(i).getValue());
					}
                }*/
                String s =null;
                while((s = bufferedReader.readLine())!=null){
                	builder.append(s); 
                }
                
                String string=builder.toString();
            	Log.d("log", ">>>>执行 方法 postMethod()获取到范围值："+string);
            	
			} catch (ClientProtocolException e)
			{
				e.printStackTrace();
			} catch (IOException e)
			{
				e.printStackTrace();
			}

		} catch (UnsupportedEncodingException e)
		{
			e.printStackTrace();
		}

	}
	
}
```



#### 2.实现客户端与服务器cookie共享

      这里采用单例模式实现HttpClient对象在Android客户端的唯一性，用户在登录系统后，这个对象会记录用户的Cookie，一旦建立客户端与服务器的访问认证，以后就可以任意发送HTTP请求到服务器请求或操作资源了。



```java
package com.boonya.httpclienttest.utils;

import org.apache.http.client.HttpClient;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;

public class HtttpClientUtil
{
	/** 设置请求超时10秒钟 */
	private static final int REQUEST_TIMEOUT = 10 * 1000;
	
	/** 设置等待数据超时时间10秒钟 */
	private static final int SO_TIMEOUT = 10 * 1000;

	private static HttpClient instance;
	
	/**  记住cookie字符串*/
	private static String cookie;

	/**
	 * 自定义方法：初始化HttpClient，并设置超时
	 * 
	 * @return 返回：HttpClient 对象
	 */
	private HtttpClientUtil()
	{

	}
	
	public static String getCookie()
	{
		return cookie;
	}



	public static void setCookie(String cookie)
	{
		HtttpClientUtil.cookie = cookie;
	}



	public static HttpClient getInstance()
	{
		if (instance == null)
		{
			BasicHttpParams httpParams = new BasicHttpParams();

			HttpConnectionParams.setConnectionTimeout(httpParams, REQUEST_TIMEOUT);

			HttpConnectionParams.setSoTimeout(httpParams, SO_TIMEOUT);

			instance = new DefaultHttpClient(httpParams);
		}
		return instance;
	}

}
```
上面cookie字段并非必须的，如果HttpClient不是单例的，那么用户需要在静态变量来记住登录后的cookie。


优化HttpClientUtil的cookie获取方法：



```java
package com.boonya.httpclienttest.utils;

import java.util.List;

import org.apache.http.client.HttpClient;
import org.apache.http.cookie.Cookie;
import org.apache.http.impl.client.AbstractHttpClient;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.params.BasicHttpParams;
import org.apache.http.params.HttpConnectionParams;

public class HtttpClientUtil
{
	/** 设置请求超时10秒钟 */
	private static final int REQUEST_TIMEOUT = 10 * 1000;

	/** 设置等待数据超时时间10秒钟 */
	private static final int SO_TIMEOUT = 10 * 1000;

	private static HttpClient instance;

	/** 记住cookie字符串 */
	private static String cookie = null;

	/**
	 * 自定义方法：初始化HttpClient，并设置超时
	 * 
	 * @return 返回：HttpClient 对象
	 */
	private HtttpClientUtil()
	{

	}

	public static String getCookie()
	{
		// 确保实例存在
		HtttpClientUtil.getInstance();
		// 获取cookie
		List<Cookie> cookies = ((AbstractHttpClient) instance).getCookieStore().getCookies();
		if (cookies != null && cookies.size() > 0)
		{
			for (int i = 0; i < cookies.size(); i++)
			{
				cookie = cookies.get(i).getValue();
			}
		}
		return cookie;
	}

	public static HttpClient getInstance()
	{
		if (instance == null)
		{
			BasicHttpParams httpParams = new BasicHttpParams();

			HttpConnectionParams.setConnectionTimeout(httpParams, REQUEST_TIMEOUT);

			HttpConnectionParams.setSoTimeout(httpParams, SO_TIMEOUT);

			instance = new DefaultHttpClient(httpParams);
		}
		return instance;
	}

}
```







