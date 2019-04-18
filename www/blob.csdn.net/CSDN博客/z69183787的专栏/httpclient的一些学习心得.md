# httpclient的一些学习心得 - z69183787的专栏 - CSDN博客
2014年02月13日 10:18:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1968标签：[HTTPCLIENT](https://so.csdn.net/so/search/s.do?q=HTTPCLIENT&t=blog)
个人分类：[Httpclient-简介](https://blog.csdn.net/z69183787/article/category/2185975)

最近忙于一个项目，了解下httpclient，在这里总结出来，和大家一起学习分享，希望各位朋友提出宝贵的意见。 
首先介绍一下项目的背景： 
  目标：把国内一家保险公司的“ＷＥＢ一账通”改成“ＷＡＰ一账通”。 
  资源：客户不提供任何的webservice接口。 
本项目中用到的第三方组件是apache的httpclient，一个非常强大的网页抓取工具（抓这个字用得可能不太好），　这里和大家 
一起讨论下httpclient的一些常用用法和要注意的地方。 
本文引用的资源列表： 
  httpclient入门：  http://www.ibm.com/developerworks/cn/opensource/os-httpclient/ 
  httpclient证书导入：http://www.blogjava.net/happytian/archive/2006/12/22/89447.html 
  httpclient高级认识：http://laohuang.iteye.com/blog/55613 
  httpclient官方文档：http://hc.apache.org/httpcomponents-client/index.html 
  httpclient资源关闭：http://www.iteye.com/topic/234759 
上面的文章写得很好，看完之后也就知道怎么用httpclient这个很好的工具了，但是在这里还是补充一些比较重要的东西，也是项目中经 
常碰到的问题。 
首先要注意的有以下几点： 
1、httpclient连接后资源释放问题很重要，就跟我们用database connection要释放资源一样。 
2、https网站采用ssl加密传输，证书导入要注意。 
3、做这样的项目最好先了解下http协义，比如302,301,200,404返回代码的含义（这是最基本的）,cookie,session的机制。 
4、httpclient的redirect状态默认是自动的，这在很大程度上给开发者很大的方便（如一些授权获得cookie），但是有时要手动管理下，比如 
　　有时会遇到CircularRedirectException异常，出现这样的情况是因为返回的头文件中location值指向之前重复(端口号可以不同)地址，导致可能会出现死 
  循环递归重定向，这时可以手动关闭:method.setFollowRedirects(false) 
5、有的网站会先判别用户的请求是否是来自浏览器，如不是，则返回不正确的文本，所以用httpclient抓取信息时在头部加入如下信息： 
  header.put("User-Agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; QQDownload 1.7; .NET CLR 1.1.4322; CIBA; .NET CLR 2.0.50727)"); 
6、当post请求提交数据时要改变默认编码，不然的话提交上去的数据会出现乱码。重写postMethod的setContentCharSet()方法就可以了： 
下面写一个通用类来处理request请求返回的文本: 
Java代码  ![收藏代码](http://wallimn.iteye.com/images/icon_star.png)
- /*
-  * HttpRequestProxy.java
-  *
-  * Created on November 3, 2008, 9:53 AM
-  */
- 
- package cn.com.mozat.net;  
- 
- import java.io.BufferedReader;  
- import java.io.IOException;  
- import java.io.InputStream;  
- import java.io.InputStreamReader;  
- import java.util.HashMap;  
- import java.util.Iterator;  
- import java.util.Map;  
- import java.util.Set;  
- 
- import org.apache.commons.httpclient.Header;  
- import org.apache.commons.httpclient.HttpClient;  
- import org.apache.commons.httpclient.HttpException;  
- import org.apache.commons.httpclient.HttpMethod;  
- import org.apache.commons.httpclient.NameValuePair;  
- import org.apache.commons.httpclient.SimpleHttpConnectionManager;  
- import org.apache.commons.httpclient.methods.GetMethod;  
- import org.apache.commons.httpclient.methods.PostMethod;  
- 
- import cn.com.mozat.exception.CustomException;  
- 
- /**
-  * 
-  * @author bird  email:lihongfu-84@163.com
-  *
-  * 2008-11-4  09:49:48
-  */
- publicclass HttpRequestProxy{  
- //超时间隔
- privatestaticint connectTimeOut = 60000;  
- //让connectionmanager管理httpclientconnection时是否关闭连接
- privatestaticboolean alwaysClose = false;  
- //返回数据编码格式
- private String encoding = "UTF-8";  
- 
- privatefinal HttpClient client = new HttpClient(new SimpleHttpConnectionManager(alwaysClose));  
- 
- public HttpClient getHttpClient(){  
- return client;  
-     }  
- 
- /**
-      * 用法：
-      * HttpRequestProxy hrp = new HttpRequestProxy();
-      * hrp.doRequest("http://www.163.com",null,null,"gbk");
-      * 
-      * @param url  请求的资源ＵＲＬ
-      * @param postData  POST请求时form表单封装的数据 没有时传null
-      * @param header   request请求时附带的头信息(header) 没有时传null
-      * @param encoding response返回的信息编码格式 没有时传null
-      * @return  response返回的文本数据
-      * @throws CustomException 
-      */
- public String doRequest(String url,Map postData,Map header,String encoding) throws CustomException{  
-      String responseString = null;  
- //头部请求信息
-      Header[] headers = null;  
- if(header != null){  
-       Set entrySet = header.entrySet();  
- int dataLength = entrySet.size();  
-           headers= new Header[dataLength];  
- int i = 0;  
- for(Iterator itor = entrySet.iterator();itor.hasNext();){  
-           Map.Entry entry = (Map.Entry)itor.next();  
-           headers[i++] = new Header(entry.getKey().toString(),entry.getValue().toString());  
-          }  
-      }  
- //post方式
- if(postData!=null){  
-          PostMethod postRequest = new PostMethod(url.trim());  
- if(headers != null){  
- for(int i = 0;i < headers.length;i++){  
-            postRequest.setRequestHeader(headers[i]);  
-           }  
-          }  
-          Set entrySet = postData.entrySet();  
- int dataLength = entrySet.size();  
-          NameValuePair[] params = new NameValuePair[dataLength];  
- int i = 0;  
- for(Iterator itor = entrySet.iterator();itor.hasNext();){  
-           Map.Entry entry = (Map.Entry)itor.next();  
-           params[i++] = new NameValuePair(entry.getKey().toString(),entry.getValue().toString());  
-          }  
-          postRequest.setRequestBody(params);  
- try {  
-     responseString = this.executeMethod(postRequest,encoding);  
-    } catch (CustomException e) {  
- throw e;  
-    } finally{  
-     postRequest.releaseConnection();  
-    }  
-         }  
- //get方式
- if(postData == null){  
-          GetMethod getRequest = new GetMethod(url.trim());  
- if(headers != null){  
- for(int i = 0;i < headers.length;i++){  
-            getRequest.setRequestHeader(headers[i]);  
-           }  
-          }  
- try {  
-     responseString = this.executeMethod(getRequest,encoding);  
-    } catch (CustomException e) {  
-                 e.printStackTrace();  
- throw e;  
-    }finally{  
-     getRequest.releaseConnection();  
-    }  
-         }  
- 
- return responseString;  
-     }  
- 
- private String executeMethod(HttpMethod request, String encoding) throws CustomException{  
-   String responseContent = null;  
-   InputStream responseStream = null;  
-   BufferedReader rd = null;  
- try {  
- this.getHttpClient().executeMethod(request);  
- if(encoding != null){  
-     responseStream = request.getResponseBodyAsStream();  
-      rd = new BufferedReader(new InputStreamReader(responseStream,  
-                       encoding));  
-               String tempLine = rd.readLine();  
-               StringBuffer tempStr = new StringBuffer();  
-               String crlf=System.getProperty("line.separator");  
- while (tempLine != null)  
-               {  
-                   tempStr.append(tempLine);  
-                   tempStr.append(crlf);  
-                   tempLine = rd.readLine();  
-               }  
-               responseContent = tempStr.toString();  
-    }else
-     responseContent = request.getResponseBodyAsString();  
- 
-    Header locationHeader = request.getResponseHeader("location");  
- //返回代码为302,301时，表示页面己经重定向，则重新请求location的url，这在
- //一些登录授权取cookie时很重要
- if (locationHeader != null) {  
-              String redirectUrl = locationHeader.getValue();  
- this.doRequest(redirectUrl, null, null,null);  
-          }  
-   } catch (HttpException e) {  
- thrownew CustomException(e.getMessage());  
-   } catch (IOException e) {  
- thrownew CustomException(e.getMessage());  
- 
-   } finally{  
- if(rd != null)  
- try {  
-      rd.close();  
-     } catch (IOException e) {  
- thrownew CustomException(e.getMessage());  
-     }  
- if(responseStream != null)  
- try {  
-       responseStream.close();  
-      } catch (IOException e) {  
- thrownew CustomException(e.getMessage());  
- 
-      }  
-   }  
- return responseContent;  
-  }  
- 
- 
- /**
-   * 特殊请求数据,这样的请求往往会出现redirect本身而出现递归死循环重定向
-   * 所以单独写成一个请求方法
-   * 比如现在请求的url为：http://localhost:8080/demo/index.jsp
-   * 返回代码为302 头部信息中location值为:http://localhost:8083/demo/index.jsp
-   * 这时httpclient认为进入递归死循环重定向，抛出CircularRedirectException异常
-   * @param url
-   * @return
-   * @throws CustomException 
-   */
- public String doSpecialRequest(String url,int count,String encoding) throws CustomException{  
-   String str = null;  
-   InputStream responseStream = null;  
-   BufferedReader rd = null;  
-   GetMethod getRequest = new GetMethod(url);  
- //关闭httpclient自动重定向动能
-   getRequest.setFollowRedirects(false);  
- try {  
- 
- this.client.executeMethod(getRequest);  
-    Header header = getRequest.getResponseHeader("location");  
- if(header!= null){  
- //请求重定向后的ＵＲＬ，count同时加1
- this.doSpecialRequest(header.getValue(),count+1, encoding);  
-    }  
- //这里用count作为标志位，当count为0时才返回请求的ＵＲＬ文本,
- //这样就可以忽略所有的递归重定向时返回文本流操作，提高性能
- if(count == 0){  
-     getRequest = new GetMethod(url);  
-     getRequest.setFollowRedirects(false);  
- this.client.executeMethod(getRequest);  
-     responseStream = getRequest.getResponseBodyAsStream();  
-     rd = new BufferedReader(new InputStreamReader(responseStream,  
-                       encoding));  
-              String tempLine = rd.readLine();  
-              StringBuffer tempStr = new StringBuffer();  
-              String crlf=System.getProperty("line.separator");  
- while (tempLine != null)  
-              {  
-                  tempStr.append(tempLine);  
-                  tempStr.append(crlf);  
-                  tempLine = rd.readLine();  
-              }  
-              str = tempStr.toString();  
-    }  
- 
-   } catch (HttpException e) {  
- thrownew CustomException(e.getMessage());  
-   } catch (IOException e) {  
- thrownew CustomException(e.getMessage());  
-   } finally{  
-    getRequest.releaseConnection();  
- if(rd !=null)  
- try {  
-      rd.close();  
-     } catch (IOException e) {  
- thrownew CustomException(e.getMessage());  
-     }  
- if(responseStream !=null)  
- try {  
-       responseStream.close();  
-      } catch (IOException e) {  
- thrownew CustomException(e.getMessage());  
-      }  
-   }  
- return str;  
-  }  
- 
- 
- 
- 
- publicstaticvoid main(String[] args) throws Exception{  
-   HttpRequestProxy hrp = new HttpRequestProxy();  
-    Map header = new HashMap();  
-          header.put("User-Agent", "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1; QQDownload 1.7; .NET CLR 1.1.4322; CIBA; .NET CLR 2.0.50727)");  
-   String str = hrp.doRequest(  
- "http://www.cma-cgm.com/en/eBusiness/Tracking/Default.aspx?BolNumber=GZ2108827",  
- null, header,null);  
-   System.out.println(str.contains("row_CRXU1587647"));  
- //  System.out.println(str);
-  }  
- 
- }  
- 
