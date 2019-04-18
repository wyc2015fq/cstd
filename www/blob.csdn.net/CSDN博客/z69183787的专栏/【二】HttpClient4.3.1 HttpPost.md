# 【二】HttpClient4.3.1 HttpPost - z69183787的专栏 - CSDN博客
2015年09月11日 14:28:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：659
使用HttpClient Post提交数据，详细代码注释
**[java]**[view
 plain](http://blog.csdn.net/mr_tank_/article/details/17484761#)[copy](http://blog.csdn.net/mr_tank_/article/details/17484761#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/123228/fork)
- publicstaticvoid main(String args[]) {  
- //创建HttpClientBuilder
-        HttpClientBuilder httpClientBuilder = HttpClientBuilder.create();  
- //HttpClient
-        CloseableHttpClient closeableHttpClient = httpClientBuilder.build();  
- 
-        HttpPost httpPost = new HttpPost("http://tutor.sturgeon.mopaas.com/tutor/search");  
-        httpPost.setConfig(DEFAULT);  
- // 创建参数队列
-        List<NameValuePair> formparams = new ArrayList<NameValuePair>();  
-        formparams.add(new BasicNameValuePair("searchText", "英语"));  
- 
-        UrlEncodedFormEntity entity;  
- try {  
-            entity = new UrlEncodedFormEntity(formparams, "UTF-8");  
-            httpPost.setEntity(entity);  
- 
-            HttpResponse httpResponse;  
- //post请求
-            httpResponse = closeableHttpClient.execute(httpPost);  
- 
- //getEntity()
-            HttpEntity httpEntity = httpResponse.getEntity();  
- if (httpEntity != null) {  
- //打印响应内容
-                System.out.println("response:" + EntityUtils.toString(httpEntity, "UTF-8"));  
-            }  
- //释放资源
-            closeableHttpClient.close();  
-        } catch (Exception e) {  
-            e.printStackTrace();  
-        }  
- 
-    }  
- 
