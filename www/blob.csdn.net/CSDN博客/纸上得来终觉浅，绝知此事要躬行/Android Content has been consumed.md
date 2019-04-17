# Android:Content has been consumed - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年01月15日 13:43:50[boonya](https://me.csdn.net/boonya)阅读数：4964








如题所示：这个问题是在http请求后操作response产生的。

#### **读取JSON**



```java
/**
	 * JSON返回常用String
	 * 
	 * @param response
	 * @return
	 * @throws IllegalStateException
	 * @throws IOException
	 * @throws JSONException
	 */
	public static String getStringByJson(String field,HttpResponse response) throws IllegalStateException, IOException, JSONException
	{
		StringBuilder builder = new StringBuilder();
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
		
		String s = null;
		while ((s = bufferedReader.readLine()) != null)
		{
			builder.append(s);
		}
		
		JSONObject jsonDataInfo = new JSONObject(builder.toString());
		
		return jsonDataInfo.getString(field);
	}
	
	/**
	 * JSON返回常用int
	 * 
	 * @param response
	 * @return
	 * @throws IllegalStateException
	 * @throws IOException
	 * @throws JSONException
	 */
	public static int getIntByJson(String field,HttpResponse response) throws IllegalStateException, IOException, JSONException
	{
		StringBuilder builder = new StringBuilder();
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));

		String s = null;
		while ((s = bufferedReader.readLine()) != null)
		{
			builder.append(s);
		}

		JSONObject jsonDataInfo = new JSONObject(builder.toString());

		return jsonDataInfo.getInt(field);
	}
	
	/**
	 * 获取钻井任务封装model对象集合
	 * 
	 * @param response
	 * @param clazz
	 * @return
	 * @throws IllegalStateException
	 * @throws IOException
	 * @throws JSONException  
	 */
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public static SchedulerModel getSchedulerModelByJson(String field,HttpResponse response, Class clazz) throws IllegalStateException, IOException, JSONException
	{
		String resultString=StringUtil.getStringByJson(field, response);
		ObjectMapper om = new ObjectMapper();
		SimpleDateFormat myDateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		om.getSerializationConfig().setDateFormat(myDateFormat);

		return om.readValue(resultString, clazz);
	}
```



#### **JSON转对象**



```java
try
		{
			// 设置请求参数项
			request.setEntity(new UrlEncodedFormEntity(params, HTTP.UTF_8));
			HttpClient client = HttpClientUtil.getInstance();
			// 执行请求返回相应
			HttpResponse response = client.execute(request);
			if (response.getStatusLine().getStatusCode() == 200)
			{
				schedulerModel=StringUtil.getSchedulerModelByJson("model", response, SchedulerModel.class);
				if (schedulerModel!=null)
				{
					Log.d("log", ">>>>执行 方法addToServer(Scheduler scheduler) ,id=" + schedulerModel.getId());
				}
			} else
			{
				Log.d("log", ">>>>connectionError");
			}
		} catch (Exception e)
		{
			Log.d("log", ">>>>connectionError" + e.getMessage());
			// e.printStackTrace();
			Log.d("log", ">>>>执行 方法addToServer(Scheduler scheduler) ,出错。" + e.getMessage());
		}
```



#### 异常分析

如果在调用

```java
StringUtil.getSchedulerModelByJson("model", response, SchedulerModel.class);
```


之前或者之后调用，有使用到response，只要调用了一次





```java
BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(response.getEntity().getContent()));
```
那么，都将会返回：Content has been consumed异常，因为content读取之后就被销毁了。
#### 代码提升

如果要实现多次读取就只有将content内容保存在一个临时对象上，如StringBuilder



```java
/**
	 * 获取返回值内容
	 * @param response
	 * @return
	 * @throws IOException
	 */
	public static StringBuilder getStringBuilder(HttpResponse response) throws IOException
	{
		StringBuilder builder = new StringBuilder();
		BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(response.getEntity().getContent(),HTTP.UTF_8));

		String s = null;
		while ((s = bufferedReader.readLine()) != null)
		{
			builder.append(s);
		}
		return builder;
	}
```


以后获取JSON数据的时候就方便多了，代码如下：



```java
/**
	 * JSON返回常用String
	 * 
	 * @param response
	 * @return
	 * @throws IllegalStateException
	 * @throws IOException
	 * @throws JSONException
	 */
	public static String getStringByJson(String field,StringBuilder builder) throws IllegalStateException, IOException, JSONException
	{		
		JSONObject jsonDataInfo = new JSONObject(builder.toString());
		
		return jsonDataInfo.getString(field);
	}
	
	/**
	 * JSON返回常用int
	 * 
	 * @param response
	 * @return
	 * @throws IllegalStateException
	 * @throws IOException
	 * @throws JSONException
	 */
	public static int getIntByJson(String field,StringBuilder builder) throws IllegalStateException, IOException, JSONException
	{
		JSONObject jsonDataInfo = new JSONObject(builder.toString());

		return jsonDataInfo.getInt(field);
	}
```





