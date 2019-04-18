# Java利用HttpURLConnection发送post请求 - z69183787的专栏 - CSDN博客
2012年10月29日 16:24:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：16112
```java
URL url = null;
HttpURLConnection http = null;
try {
	url = new URL(urls);
	http = (HttpURLConnection) url.openConnection();
	http.setDoInput(true);
	http.setDoOutput(true);
	http.setUseCaches(false);
	http.setConnectTimeout(50000);//设置连接超时
//如果在建立连接之前超时期满，则会引发一个 java.net.SocketTimeoutException。超时时间为零表示无穷大超时。
	http.setReadTimeout(50000);//设置读取超时
//如果在数据可读取之前超时期满，则会引发一个 java.net.SocketTimeoutException。超时时间为零表示无穷大超时。			
	http.setRequestMethod("POST");
	// http.setRequestProperty("Content-Type","text/xml; charset=UTF-8");
	http.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");
	http.connect();
	param = "&appName=" + appName 
			+ "&token=" + token 
			+ "&method=" + method 
			+ "&dataType=" + dataType 
			+ "&dataParams=" + dataParams 
			+ "&sign=" + sign;
	OutputStreamWriter osw = new OutputStreamWriter(http.getOutputStream(), "utf-8");
	osw.write(param);
	osw.flush();
	osw.close();
	if (http.getResponseCode() == 200) {
		BufferedReader in = new BufferedReader(new InputStreamReader(http.getInputStream(), "utf-8"));
		String inputLine;
		while ((inputLine = in.readLine()) != null) {
			result += inputLine;
		}
		in.close();
		//result = "["+result+"]";
	}
} catch (Exception e) {
	System.out.println("err");
} finally {
	if (http != null) http.disconnect();
	if (fis != null) fis.close();
}
```
