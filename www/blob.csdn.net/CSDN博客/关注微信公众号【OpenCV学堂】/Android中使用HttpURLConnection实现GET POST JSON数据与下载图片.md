# Android中使用HttpURLConnection实现GET POST JSON数据与下载图片 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年01月04日 17:48:22[gloomyfish](https://me.csdn.net/jia20003)阅读数：22214








Android中使用HttpURLConnection实现GET POST JSON数据与下载图片


Android6.0中把Apache HTTP Client所有的包与类都标记为deprecated不再建议使用

所有跟HTTP相关的数据请求与提交操作都通过HttpURLConnection类实现，现实是

很多Android开发者一直都Apache HTTP Client来做andoird客户端与后台HTTP接口数

据交互，本人刚刚用HttpURLConnection做了一个android的APP，不小心踩到了几个

坑，总结下最常用的就通过HttpURLConnection来POST提交JSON数据与GET请求

JSON数据。此外就是下载图片，下载图片分为显示进度与不显示进度两种。其中提交

数据的时候涉及中文一定要先把中文转码成utf-8之后在POST提交，否则就会一直遇到

HTTP 400的错误。



**一：GET请求JSON数据的例子**

```java
public UserDto execute(String... params) {
	InputStream inputStream = null;
	HttpURLConnection urlConnection = null;

	try {
		// read responseURLEncoder.encode(para, "GBK");
		String urlWithParams = DOMAIN_ADDRESS + MEMBER_REQUEST_TOKEN_URL + "?userName=" + java.net.URLEncoder.encode(params[0],"utf-8") + "&password=" + params[1];
		URL url = new URL(urlWithParams);
		urlConnection = (HttpURLConnection) url.openConnection();

		/* optional request header */
		urlConnection.setRequestProperty("Content-Type", "application/json; charset=UTF-8");

		/* optional request header */
		urlConnection.setRequestProperty("Accept", "application/json");

		/* for Get request */
		urlConnection.setRequestMethod("GET");
		int statusCode = urlConnection.getResponseCode();

		/* 200 represents HTTP OK */
		if (statusCode == 200) {
			inputStream = new BufferedInputStream(urlConnection.getInputStream());
			String response = HttpUtil.convertInputStreamToString(inputStream);
			Gson gson = new Gson();
			UserDto dto = gson.fromJson(response, UserDto.class);
			if (dto != null && dto.getToken() != null) {
				Log.i("token", "find the token = " + dto.getToken());
			}
			return dto;
		}

	} catch (Exception e) {
		e.printStackTrace();
	} finally {
		if (inputStream != null) {
			try {
				inputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		if (urlConnection != null) {
			urlConnection.disconnect();
		}
	}
	return null;
}
```
**二：POST提交JSON数据**


```java
public Map<String, String> execute(NotificationDto dto) {
	InputStream inputStream = null;
	HttpURLConnection urlConnection = null;
	try {
		URL url = new URL(getUrl);
		urlConnection = (HttpURLConnection) url.openConnection();

		/* optional request header */
		urlConnection.setRequestProperty("Content-Type", "application/json; charset=UTF-8");

		/* optional request header */
		urlConnection.setRequestProperty("Accept", "application/json");
		dto.setCreator(java.net.URLEncoder.encode(dto.getCreator(), "utf-8"));
		
		// read response
		/* for Get request */
		urlConnection.setRequestMethod("POST");
		urlConnection.setDoOutput(true);
		DataOutputStream wr = new DataOutputStream(urlConnection.getOutputStream());
		Gson gson = new Gson();
		String jsonString = gson.toJson(dto);
		wr.writeBytes(jsonString);
		wr.flush();
		wr.close();
		// try to get response
		int statusCode = urlConnection.getResponseCode();
		if (statusCode == 200) {
			inputStream = new BufferedInputStream(urlConnection.getInputStream());
			String response = HttpUtil.convertInputStreamToString(inputStream);
			Map<String, String> resultMap = gson.fromJson(response, Map.class);
			if (resultMap != null && resultMap.size() > 0) {
				Log.i("applyDesigner", "please check the map with key");
			}
			return resultMap;
		}
	}
	catch(Exception e)
	{
		e.printStackTrace();
	}
	finally
	{
		if (inputStream != null) {
			try {
				inputStream.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		if (urlConnection != null) {
			urlConnection.disconnect();
		}
	}
	return null;
}
```





**三：下载图片显示下载进度**

```java
package com.example.demo;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.AsyncTask;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class ImageLoadTask extends AsyncTask<String, Void, Bitmap> {
	private Handler handler;

	public ImageLoadTask(Handler handler) {
		this.handler = handler;
	}

	protected void onPostExecute(Bitmap result) {
		Message msg = new Message();
		msg.obj = result;
		handler.sendMessage(msg);
	}

	protected Bitmap doInBackground(String... getUrls) {
		InputStream inputStream = null;
		HttpURLConnection urlConnection = null;

		try {
			// open connection
			URL url = new URL(getUrls[0]);
			urlConnection = (HttpURLConnection) url.openConnection();
			/* for Get request */
			urlConnection.setRequestMethod("GET");
			int fileLength = urlConnection.getContentLength();
			int statusCode = urlConnection.getResponseCode();
			if (statusCode == 200) {
				inputStream = urlConnection.getInputStream();
				byte data[] = new byte[4096];
				long total = 0;
				int count;
				ByteArrayOutputStream output = new ByteArrayOutputStream();
				while ((count = inputStream.read(data)) != -1) {
					total += count;
					// publishing the progress....
					if (fileLength > 0 && handler != null) {
						handler.sendEmptyMessage(((int) (total * 100 / fileLength)) - 1);
					}
					output.write(data, 0, count);
				}
				ByteArrayInputStream bufferInput = new ByteArrayInputStream(output.toByteArray());
				Bitmap bitmap = BitmapFactory.decodeStream(bufferInput);
				inputStream.close();
				bufferInput.close();
				output.close();
				Log.i("image", "already get the image by uuid : " + getUrls[0]);
				handler.sendEmptyMessage(100);
				return bitmap;
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			if (inputStream != null) {
				try {
					inputStream.close();
				} catch (IOException e) {
					e.printStackTrace();
				}
			}
			if (urlConnection != null) {
				urlConnection.disconnect();
			}
		}
		return null;
	}

}
```
**总结：**使用HttpURLConnection提交JSON数据的时候编码方式为UTF-8
所有中文字符请一定要预先转码为UTF-8，然后在后台服务器对应的API

中解码为UTF-8，不然就会报错HTTP 400。



