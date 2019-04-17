# Android使用HttpURLConnection和HttpClient请求服务器数据 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年06月25日 10:24:31[boonya](https://me.csdn.net/boonya)阅读数：882








    HTTP提交方式有多种，最常用的的就是POST和GET，另外还有PUT、DELETE、HEAD。好久没学习了，Android这边又生疏了，近三个月来毫无建树，整天都忙也没学到什么东西，打算继续学习Android！


### 1.0 HttpURLConnection



#### 1.1 HttpURLConnection:GET





```java
public boolean loginByGet(String path, String username , String password) throws Exception{  
          
        String url_path = path +"?username=" + URLEncoder.encode(username, "utf-8") + "&password="+password;  
          
        URL url = new URL(url_path);  
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();  
          
        conn.setRequestMethod("GET");  
        conn.setConnectTimeout(5000);  
        if(conn.getResponseCode() == 200){  
            return true;  
        }  
          
          
        return false;  
    }
```

#### 1.2 HttpURLConnection:POST



```java
public boolean loginByPost(String path,String username , String password) throws Exception{  
  
        System.out.println("LoginService的loginByPost()");  
        URL url = new URL(path);  
        HttpURLConnection conn = (HttpURLConnection) url.openConnection();  
          
        conn.setRequestMethod("POST");  
        conn.setConnectTimeout(5000);  
          
        String value = "username=" + username +"&" + "password=" +password;  
        byte[] entity = value.getBytes();  
           
        conn.setRequestProperty("Content-Type", "application/x-www-form-urlencoded");  
        conn.setRequestProperty("Content-Length", entity.length + "");  
          
        conn.setDoOutput(true);  
        OutputStream os = conn.getOutputStream();  
        os.write(entity);  
          
        if(conn.getResponseCode() == 200){  
            return true;  
        }  
          
        return false;  
    }
```

### 2.0 HttpClient

#### 2.1 HttpClient:GET



```java
public boolean loginByHttpClientGet(String path,String username , String password) throws Exception{  
          
          
        String value = path + "?username=" + username +"&password=" + password;   
        HttpClient httpClient =  new DefaultHttpClient();  
        HttpGet httpGet = new HttpGet(value);  
          
        HttpResponse httpResponse = httpClient.execute(httpGet);  
        if(httpResponse.getStatusLine().getStatusCode() == 200){  
            return true;  
        }  
          
        return false;  
    }
```

#### 2.2 HttpClient:POST



```java
public boolean loginByHttpClientPost(String path,String username , String password)throws Exception{  
          
        HttpClient httpClient = new DefaultHttpClient();  
        HttpPost httpPost = new HttpPost(path);  
          
        List<NameValuePair> parameters = new ArrayList<NameValuePair>();  
        parameters.add(new BasicNameValuePair("username", username));  
        parameters.add(new BasicNameValuePair("password", password));  
          
        UrlEncodedFormEntity entity = new UrlEncodedFormEntity(parameters,"utf-8");  
        httpPost.setEntity(entity);  
        HttpResponse httpResponse = httpClient.execute(httpPost);  
        if(httpResponse.getStatusLine().getStatusCode() == 200){  
            return true;  
        }  
          
        return false;  
    }
```




