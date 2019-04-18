# 用httpPost对JSON发送和接收的例子 - z69183787的专栏 - CSDN博客
2016年04月06日 11:00:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6461
HTTPPost发送JSON：
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)privatestaticfinal String APPLICATION_JSON = "application/json";
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)privatestaticfinal String CONTENT_TYPE_TEXT_JSON = "text/json";
![](http://www.blogjava.net/Images/OutliningIndicators/None.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)publicstaticvoid httpPostWithJSON(String url, String json) throws Exception {
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)// 将JSON进行UTF-8编码,以便传输中文
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        String encoderJson = URLEncoder.encode(json, HTTP.UTF_8);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        DefaultHttpClient httpClient = new DefaultHttpClient();
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        HttpPost httpPost = new HttpPost(url);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        httpPost.addHeader(HTTP.CONTENT_TYPE, APPLICATION_JSON);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        StringEntity se = new StringEntity(encoderJson);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        se.setContentType(CONTENT_TYPE_TEXT_JSON);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        se.setContentEncoding(new BasicHeader(HTTP.CONTENT_TYPE, APPLICATION_JSON));
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        httpPost.setEntity(se);
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        httpClient.execute(httpPost);
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
接收HTTPPost中的JSON:
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockStart.gif)publicstatic String receivePost(HttpServletRequest request) throws IOException, UnsupportedEncodingException {
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)// 读取请求内容
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        BufferedReader br = new BufferedReader(new InputStreamReader(request.getInputStream()));
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        String line = null;
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        StringBuilder sb = new StringBuilder();
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockStart.gif)while((line = br.readLine())!=null){
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)            sb.append(line);
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)// 将资料解码
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)        String reqBody = sb.toString();
![](http://www.blogjava.net/Images/OutliningIndicators/InBlock.gif)return URLDecoder.decode(reqBody, HTTP.UTF_8);
![](http://www.blogjava.net/Images/OutliningIndicators/ExpandedBlockEnd.gif)    }
