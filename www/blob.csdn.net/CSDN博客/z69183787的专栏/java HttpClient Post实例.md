# java HttpClient Post实例 - z69183787的专栏 - CSDN博客
2013年04月03日 09:00:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：909
个人分类：[Httpclient-简介](https://blog.csdn.net/z69183787/article/category/2185975)

private void postMethod(String url) throws IOException
 {     
  url = "[http://www.newsmth.net/bbslogin2.php](http://www.newsmth.net/bbslogin2.php)";
  PostMethod postMethod = new PostMethod(url);
  // 填入各个表单域的值
  NameValuePair[] data = { new NameValuePair("id", "herrapfel"),new NameValuePair("passwd", "") };
  // 将表单的值放入postMethod中
  postMethod.setRequestBody(data);
  // 执行postMethod
  int statusCode = httpClient.executeMethod(postMethod);
  System.out.println(" status code:" + statusCode);
  // HttpClient对于要求接受后继服务的请求，象POST和PUT等不能自动处理转发
if(statusCode == HttpStatus.SC_OK)
  {
   StringBuffer contentBuffer = new StringBuffer();
   InputStream in = postMethod.getResponseBodyAsStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(in,postMethod.getResponseCharSet()));
            String inputLine = null;
            while((inputLine = reader.readLine()) != null)
            {
             contentBuffer.append(inputLine);
             System.out.println("input line:"+ inputLine);
             contentBuffer.append("/n");
            }
            in.close();
  }
  else if (statusCode == HttpStatus.SC_MOVED_PERMANENTLY || statusCode == HttpStatus.SC_MOVED_TEMPORARILY) 
  {
      // 从头中取出转向的地址
      Header locationHeader = postMethod.getResponseHeader("location");
      String location = null;
      if (locationHeader != null) 
      {
       location = locationHeader.getValue();
       System.out.println("The page was redirected to:" + location);
      } 
      else 
      {
       System.err.println("Location field value is null.");
      }
  }
