# HttpClient +JSOUP 设置代理 爬虫 - z69183787的专栏 - CSDN博客
2017年12月06日 15:23:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：541
```java
package com.fh.job.util;
import com.alibaba.fastjson.JSONArray;
import com.alibaba.fastjson.JSONObject;
import com.fh.job.repository.entity.waybill.ProxyVO;
import com.google.common.base.Joiner;
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.NameValuePair;
import org.apache.http.ParseException;
import org.apache.http.auth.AuthScope;
import org.apache.http.auth.UsernamePasswordCredentials;
import org.apache.http.client.CredentialsProvider;
import org.apache.http.client.HttpClient;
import org.apache.http.client.entity.UrlEncodedFormEntity;
import org.apache.http.client.methods.CloseableHttpResponse;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.BasicCredentialsProvider;
import org.apache.http.impl.client.CloseableHttpClient;
import org.apache.http.impl.client.DefaultHttpClient;
import org.apache.http.impl.client.HttpClients;
import org.apache.http.message.BasicNameValuePair;
import org.apache.http.util.EntityUtils;
import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.nodes.Element;
import org.jsoup.select.Elements;
import org.springframework.beans.factory.annotation.Autowired;
import java.beans.BeanInfo;
import java.beans.Introspector;
import java.beans.PropertyDescriptor;
import java.io.IOException;
import java.io.UnsupportedEncodingException;
import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
public class CrawlYTOTrace {
	private static final Log logger = LogFactory.getLog(CrawlYTOTrace.class);
	public static String delHTMLTag(String htmlStr){
		String regEx_script="<script[^>]*?>[\\s\\S]*?<\\/script>"; //定义script的正则表达式
		String regEx_style="<style[^>]*?>[\\s\\S]*?<\\/style>"; //定义style的正则表达式
		String regEx_html="<[^>]+>"; //定义HTML标签的正则表达式
		Pattern p_script= Pattern.compile(regEx_script,Pattern.CASE_INSENSITIVE);
		Matcher m_script=p_script.matcher(htmlStr);
		htmlStr=m_script.replaceAll(""); //过滤script标签
		Pattern p_style=Pattern.compile(regEx_style,Pattern.CASE_INSENSITIVE);
		Matcher m_style=p_style.matcher(htmlStr);
		htmlStr=m_style.replaceAll(""); //过滤style标签
		Pattern p_html=Pattern.compile(regEx_html,Pattern.CASE_INSENSITIVE);
		Matcher m_html=p_html.matcher(htmlStr);
		htmlStr=m_html.replaceAll(""); //过滤html标签
		return htmlStr.trim(); //返回文本字符串
	}
	public static String CrawlWaybillTrace(List<String> listWaybillNos,int count,ProxyVO proxyVO) throws InterruptedException {
		String result = null;
		if (listWaybillNos.size() <= 0) {
			logger.warn(" CrawlWaybillTrace input waybill nos null");
			return result;
		}
		Map<String, Object> params = new HashMap<>();
		String requestWaybillNos = Joiner.on(",").skipNulls().join(listWaybillNos);
		params.put("waybillNo", requestWaybillNos);
		params.put("validateCode", null);
		params.put("jsessionId", null);
		result = CrawlYTOTrace.creatPostReqeust(params,count,proxyVO);
		Document document = Jsoup.parse(result);
		Elements divsTraceBoxElements = document.select("div[class=cont]").select("div[class=trace-box]");
		Elements lastStatusElements = document.select("div[class~=^trace-ico]").select("span");
		String lastStatusStr = "";
		List statusList = new ArrayList();
//		for (Element lastStatusElement : lastStatusElements) {
//			lastStatusStr = lastStatusElement.text();
//			statusList.add(lastStatusStr);
//		}
		JSONObject export = new JSONObject();
		export.put("success", "false");
		JSONArray traceListArray = new JSONArray();
		String lastTime="";
		int num = -1;
		for (Element traceBox : divsTraceBoxElements) {
			num++;
			JSONObject waybillNoBox = new JSONObject();
			JSONArray traceArray = new JSONArray();
			String waybillNo = traceBox.select("h1[class=box-hd]").select("strong").text();
			String status = traceBox.select("div[class=trace-ico]").select("span").text();
			Elements dataRows = traceBox.select("div[class=trace-table]").select("table").select("tr");
			lastStatusStr = lastStatusElements.get(num).text();
			for (Element row : dataRows) {
				JSONObject trace = new JSONObject();
				Elements tds = row.select("td");
				String data = null;
				Elements netColor = tds.select("td[class=data]");
				if (netColor.size() > 0) {
					data = String.valueOf(tds.select("td[class=data]"));
					data = delHTMLTag(data);
					data = data.replaceAll(" "," ");
				}
				String yttime = null;
				Elements timeColor = tds.select("td[class=time]");
				if (timeColor.size() > 0) {
					yttime = String.valueOf(tds.select("td[class=time]"));
					yttime = delHTMLTag(yttime);
					yttime = yttime.replaceAll(" "," ");
					if("8".equals(String.valueOf(yttime.length()))){
						String toptime = lastTime.substring(0,10);
						yttime= toptime+" "+yttime;
					}
				}
				if("19".equals(String.valueOf(yttime.length()))){
					lastTime=yttime;
				}
//				delHTMLTag();
//				if (netColor.size() > 0) {
//					data = tds.select("td[class=data]").select("span[class=netColor]").text().trim();
//				} else {
//					Elements stationColor = tds.select("td[class=data]").select("span[class=stationColor]");
//					if (stationColor.size() > 0) {
//						data = tds.select("td[class=data]").select("span[class=stationColor]").text().trim();
//					} else {
//						data = tds.select("td[class=data]").text().trim();
//					}
//
//				}
//				String ytwl=yttime +" "+ data;
				trace.put("orderInfo", data);
				String type = "";
				if(!Tools.isEmpty(data)){
//					data.indexOf(DictionariesUtil.);
				}
				trace.put("type", "");
				trace.put("uploadTime", yttime);
				trace.put("waybillNo", waybillNo);
				traceArray.add(trace);
			}
			waybillNoBox.put("waybillNo", waybillNo);
			waybillNoBox.put("traces", traceArray);
			waybillNoBox.put("lastStatus", lastStatusStr);
			traceListArray.add(waybillNoBox);
		}
		export.put("success", "true");
		JSONObject exportData = new JSONObject();
		exportData.put("tracesList", traceListArray);
		export.put("data", exportData);
		return export.toJSONString();
	}
	private static String creatPostReqeust(Map<String, Object> params,int count,ProxyVO proxyVO) throws InterruptedException {
		String result = null;
		// 创建HttpClient实例
		CloseableHttpClient httpClient = null;
//		if(count==1){
//			httpClient = CrawlYTOTrace.getHttpClient();
//		} else if(count==2){
//			httpClient = CrawlYTOTrace.getHttpClientSecond();
//		} else {
//			httpClient = CrawlYTOTrace.getHttpClientThird();
//		}
		httpClient = CrawlYTOTrace.getHttpClientProxy(proxyVO);
//		CloseableHttpClient httpClient = CrawlYTOTrace.getHttpClient();
		HttpPost httppost = new HttpPost("");
		// 设置请求头信息
		httppost.setHeader("User-Agent",
				"Mozilla/5.0 (Windows NT 6.1; Win64; x64; rv:53.0) Gecko/20100101 Firefox/53.0");
		httppost.setHeader("Accept-Encoding", "gzip, deflate");
		httppost.setHeader("Content-Type", "application/x-www-form-urlencoded");
		httppost.setHeader("Referer", "http://www.yto.net.cn/gw/index/index.html");
		boolean ret = CrawlYTOTrace.setPostParams(httppost, params);
		if (!ret) {
			return result;
		}
		CloseableHttpResponse response = null;
		try {
			response = httpClient.execute(httppost);
			Thread.sleep(3000);
			// 获取返回实体
			HttpEntity entity = response.getEntity();
			result = EntityUtils.toString(entity, "utf-8");
			logger.debug(" Crawl YTO Trace return : " + result);
			EntityUtils.consume(entity);
		} catch (ParseException | IOException e) {
			logger.error("creatPostReqeust Error Message: " + e.getMessage() + " \n Stack Trace: " + e.getStackTrace());
			e.printStackTrace();
		} finally {
			try {
				if (response != null)
					response.close();
				if (httpClient != null) {
					httpClient.close();
				}
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return result;
	}
	private static boolean setPostParams(HttpPost httpost, Map<String, Object> params) {
		List<NameValuePair> nvps = new ArrayList<NameValuePair>();
		Set<String> keySet = params.keySet();
		for (String key : keySet) {
			if (null == params.get(key)) {
				nvps.add(new BasicNameValuePair(key, null));
			} else {
				nvps.add(new BasicNameValuePair(key, params.get(key).toString()));
			}
		}
		try {
			httpost.setEntity(new UrlEncodedFormEntity(nvps, "UTF-8"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
			logger.error("setPostParams Error Message: " + e.getMessage() + " \n Stack Trace: " + e.getStackTrace());
			return false;
		}
		return true;
	}
//	 //设置代理
//	public static CloseableHttpClient getHttpClient() {
//		String host1 = ResourceBundleUtils.getValue("zh_CN", "host4");
//		int port1 = Integer.valueOf(ResourceBundleUtils.getValue("zh_CN", "port4"));
//		String username1 = ResourceBundleUtils.getValue("zh_CN", "username4");
//		String password1 = ResourceBundleUtils.getValue("zh_CN", "password4");
//		//创建认证，并设置认证范围
//		CredentialsProvider credsProvider = new BasicCredentialsProvider();
//		credsProvider.setCredentials(new AuthScope(host1, port1),//可以访问的范围
//				new UsernamePasswordCredentials(username1 , password1));//用户名和密码
//		CloseableHttpClient client = HttpClients.custom().setDefaultCredentialsProvider(credsProvider).build();
//
//		return client;
//	}
	//设置代理
	public static CloseableHttpClient getHttpClient() {
		String host1 = ResourceBundleUtils.getValue("zh_CN", "host1");
		int port1 = Integer.valueOf(ResourceBundleUtils.getValue("zh_CN", "port1"));
		String username1 = ResourceBundleUtils.getValue("zh_CN", "username1");
		String password1 = ResourceBundleUtils.getValue("zh_CN", "password1");
		//创建认证，并设置认证范围
		CredentialsProvider credsProvider = new BasicCredentialsProvider();
		credsProvider.setCredentials(new AuthScope(host1, port1),//可以访问的范围
				new UsernamePasswordCredentials(username1 , password1));//用户名和密码
		CloseableHttpClient client = HttpClients.custom().setDefaultCredentialsProvider(credsProvider).build();
		return client;
	}
	 //设置代理Proxy
	public static CloseableHttpClient getHttpClientProxy(ProxyVO proxyVO) {
		String host1 = proxyVO.getHost();
		int port1 = proxyVO.getPort();
		//创建认证，并设置认证范围
		CredentialsProvider credsProvider = new BasicCredentialsProvider();
		credsProvider.setCredentials(new AuthScope(host1, port1),//可以访问的范围
				new UsernamePasswordCredentials("",""));//用户名和密码
		CloseableHttpClient client = HttpClients.custom().setDefaultCredentialsProvider(credsProvider).build();
		return client;
	}
	// 设置代理2
	public static CloseableHttpClient getHttpClientSecond() {
		String host2 = ResourceBundleUtils.getValue("zh_CN", "host2");
		int port2 = Integer.valueOf(ResourceBundleUtils.getValue("zh_CN", "port32"));
		String username2 = ResourceBundleUtils.getValue("zh_CN", "username2");
		String password2 = ResourceBundleUtils.getValue("zh_CN", "password2");
		//创建认证，并设置认证范围22
		CredentialsProvider credsProvider = new BasicCredentialsProvider();
		credsProvider.setCredentials(new AuthScope(host2, port2),//可以访问的范围
				new UsernamePasswordCredentials(username2 , password2));//用户名和密码
		CloseableHttpClient client = HttpClients.custom().setDefaultCredentialsProvider(credsProvider).build();
		return client;
	}
	// 设置代理3
	public static CloseableHttpClient getHttpClientThird() {
		String host3 = ResourceBundleUtils.getValue("zh_CN", "host3");
		int port3 = Integer.valueOf(ResourceBundleUtils.getValue("zh_CN", "port3"));
		String username3 = ResourceBundleUtils.getValue("zh_CN", "username3");
		String password3 = ResourceBundleUtils.getValue("zh_CN", "password3");
		//创建认证，并设置认证范围
		CredentialsProvider credsProvider = new BasicCredentialsProvider();
		credsProvider.setCredentials(new AuthScope(host3, port3),//可以访问的范围
				new UsernamePasswordCredentials(username3 , password3));//用户名和密码
		CloseableHttpClient client = HttpClients.custom().setDefaultCredentialsProvider(credsProvider).build();
		return client;
	}
	public List<ProxyVO> getProxy() throws IOException {
		HttpClient httpClient = new DefaultHttpClient();
		String url="http://http-api.taiyangruanjian.com/getip?num=1&type=2&pro=0&city=0&yys=0&port=1&pack=7435&ts=0&ys=0&cs=0&lb=1&sb=0&pb=4&mr=2";
		HttpGet httpGet = new HttpGet(url);
		// 在请求消息头中指定语言，保证服务器会返回中文数据
		httpGet.addHeader("Accept-Language", "zh-CN");
		HttpResponse httpResponse = httpClient.execute(httpGet);
		ProxyVO proxys = null;
		List<ProxyVO> proxysList = new ArrayList();
		try {
			if (httpResponse.getStatusLine().getStatusCode() == 200) {
				HttpEntity entity = httpResponse.getEntity();
				String response = EntityUtils.toString(entity, "utf-8");
				org.json.JSONObject jsonObject = new org.json.JSONObject(response);
				if (jsonObject.get("success").toString().equals("true")) {
					org.json.JSONArray jsonArray =jsonObject.getJSONArray("data");
					for(int i=0;i<jsonArray.length();i++) {
						proxys = new ProxyVO();
//						proxys=new Proxy());
//						jsonArray.getJSONObject(i).getString("ip"),jsonArray.getJSONObject(i).getInt("port"
						proxys.setHost(jsonArray.getJSONObject(i).getString("ip"));
						proxys.setPort(jsonArray.getJSONObject(i).getInt("port"));
						proxysList.add(proxys);
					}
				}
			}
		} catch (Exception e) {
			throw e;
		}
		return proxysList;
	}
	public static void main(String[] args) {
//		List<String> listWaybillNos = new ArrayList<>();
//		listWaybillNos.add("886833535763837710");
//		listWaybillNos.add("886846605929772997");
//
//		String result = CrawlYTOTrace.CrawlWaybillTrace(listWaybillNos,1);
//		System.out.println(111);
//		LanjianFollowRespEntity entity = JSONObject.parseObject(result,LanjianFollowRespEntity.class );
//
//
//		LanjianFollowFstRespVO lanjianFollowFstRespVO = entity.getData();
//
//		List<LanjianFollowRespSecVO> tracesList =  lanjianFollowFstRespVO.getTracesList();
//
//		List insertVoList = new ArrayList();
//
//		for(int m=0;m<tracesList.size();m++){
//
//			LanjianFollowRespSecVO lanjianFollowRespSecVO = tracesList.get(m);
//			String lastStatus = lanjianFollowRespSecVO.getLastStatus();
//
//			List<LanjianFollowThdRespVO> traces = lanjianFollowRespSecVO.getTraces();
//
//			LanjianFollowThdRespVO lanjianFollowThdRespVO = traces.get(traces.size()-1);
//
//			lanjianFollowThdRespVO.setType(lastStatus);
//			insertVoList.add(lanjianFollowThdRespVO);
////			for(int n=0;n<traces.size();n++){
//
////				LanjianFollowThdRespVO lanjianFollowThdRespVO = traces.get(n);
////
////				String orderInfo = lanjianFollowThdRespVO.getOrderInfo();
////				String uploadTime = lanjianFollowThdRespVO.getUploadTime();
////				String waybillNo = lanjianFollowThdRespVO.getWaybillNo();
////				String type = lanjianFollowThdRespVO.getType();
////
////				System.out.print(orderInfo);
////			}
//		}
//
//		System.out.println(insertVoList);
//		//定时
//		Timer timer = new Timer();
//		timer.scheduleAtFixedRate(new TimerTask() {
//			public void run() {
//
//
//				System.out.println("abc");
//			}
//		}, 1000 , 12000);
	}
	public static void copyBean(Object source, Object dest) throws Exception {
		// 获取属性
		BeanInfo sourceBean = Introspector.getBeanInfo(source.getClass(),
				Object.class);
		PropertyDescriptor[] sourceProperty = sourceBean
				.getPropertyDescriptors();
		BeanInfo destBean = Introspector.getBeanInfo(dest.getClass(),
				Object.class);
		PropertyDescriptor[] destProperty = destBean.getPropertyDescriptors();
		try {
			for (int i = 0; i < sourceProperty.length; i++) {
				for (int j = 0; j < destProperty.length; j++) {
					if (sourceProperty[i].getName().equals(
							destProperty[j].getName())) {
						// 调用source的getter方法和dest的setter方法
						destProperty[j].getWriteMethod().invoke(
								dest,
								sourceProperty[i].getReadMethod()
										.invoke(source));
						break;
					}
				}
			}
		} catch (Exception e) {
			throw new Exception("属性复制失败:" + e.getMessage());
		}
	}
}
```
