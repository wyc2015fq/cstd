# Java SpringMVC实现PC端网页微信扫码支付完整版 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2016年10月29日 00:47:33[gloomyfish](https://me.csdn.net/jia20003)阅读数：5309








一：前期微信支付扫盲知识

前提条件是已经有申请了微信支付功能的公众号，然后我们需要得到公众号APPID和微信商户号，这个分别在微信公众号和微信支付商家平台上面可以发现。其实在你申请成功支付功能之后，微信会通过邮件把Mail转给你的，有了这些信息之后，我们就可以去微信支付服务支持页面：https://pay.weixin.qq.com/service_provider/index.shtml

打开这个页面，点击右上方的链接【开发文档】会进入到API文档说明页面，看起来如下

![](https://img-blog.csdn.net/20161029002951479)




选择红色圆圈的扫码支付就是我们要做接入方式，鼠标移动到上面会提示你去查看开发文档，如果这个都不知道怎么查看，可以洗洗睡了，你真的不合适做程序员，地址如下：

https://pay.weixin.qq.com/wiki/doc/api/native.php?chapter=6_1在浏览器中打开之后会看到
![](https://img-blog.csdn.net/20161029003122449)



我们重点要关注和阅读的内容我已经用红色椭圆标注好了，首先阅读【接口规则】里面的协议规范，开玩笑这个都不读你就想做微信支付，这个就好比你要去泡妞，得先收集点基本背景信息，了解对方特点，不然下面还怎么沟通。事实证明只有会泡妞得程序员才是好销售。跑题了我们接下来要看一下【场景介绍】中的案例与规范，只看一下记得一定要微信支付的LOGO下载下来，是为了最后放到我们自己的扫码支付网页上，这样看上去比较专业一点。之后重点关注【模式二】

我们这里就是要采用模式二的方式实现PC端页面扫码支付功能。

    微信官方对模式二的解释是这样的“商户后台系统先调用微信支付的统一下单接口，微信后台系统返回链接参数code_url，商户后台系统将code_url值生成二维码图片，用户使用微信客户端扫码后发起支付。注意：code_url有效期为2小时，过期后扫码不能再发起支付”。看明白了吧就是我们首先要调用微信提供**统一下单接口**，得到一个关键信息code_url（至于这个code_url是什么鬼，我也不知道），然后我们通过自己的程序把这个URL生成一个二维码，生成二维码我这里用了Google的zxing库。然后把这个二维码显示在你的PC端网页上就行啦。这样终端用户一扫码就支付啦，支付就完成啦，看到这里你肯定很激动，发现微信支付如此简单，等等还有个事情我们还不知道，客户知道付钱了，我们服务器端还不知道呢，以微信开发人员的智商他们早就想到这个问题了，所以让你在调用统一下单接口的时候其中有个必填的参数就是回调URL，就是如果客户端付款成功之后微信会通过这个URL向我们自己的服务器提交一些数据，然后我们后台解析这些数据，完成我们自己操作。这样我们才知道客户是否真的已经通过微信付款了。这样整个流程才结束，这个就是模式二。微信用一个时序图示这样表示这个过程的。
![](https://img-blog.csdn.net/20161029003209864)



表达起来比较复杂，看上去比较吃力，总结一下其实我们服务器该做的事情就如下件：

1. 通过统一下单接口传入正确的参数(当然要包括我们的回调URL)与签名验证，从返回数据中得到code_url的对应数据

2. 根据code_url的数据我们自己生成一个二维码图片，显示在浏览器网页上

3. 在回调的URL中添加我们自己业务逻辑处理。



至此扫盲结束了，你终于知道扫码支付什么个什么样的流程了，下面我们就一起来扒扒它的相关API使用，做好每步处理。

**二：开发过程**

**在开发代码之前，请先准备几件事情。**

**1. 添加ZXing的maven依赖**

**2. 添加jdom的maven依赖**

**3.下载Java版本SDK演示程序，地址在这里**

**https://pay.weixin.qq.com/wiki/doc/api/native.php?chapter=11_1**

**我们需要MD5Util.java和XMLUtil.java两个文件**

**4. 我们使用HttpClient版本是4.5.1，记得添加Maven依赖**

**上面准备工作做好以后，继续往下看：**

首先我们要调用微信的统一下单接口，我们点击【API列表】中的统一下单会看到这样页面：
![](https://img-blog.csdn.net/20161029003305356)



以本人调用实际情况为例，如下的参数是必须要有的,为了大家的方便我已经把它变成一个POJO的对象， 代码如下：

```java
public class UnifiedorderDto implements WeiXinConstants {

	private String appid;
	private String body;
	private String device_info;
	private String mch_id;
	private String nonce_str;
	private String notify_url;
	private String openId;
	private String out_trade_no;
	private String spbill_create_ip;
	private int total_fee;
	private String trade_type;
	private String product_id;
	private String sign;
	
	public UnifiedorderDto() {
		this.appid = APPID;
		this.mch_id = WXPAYMENTACCOUNT;
		this.device_info = DEVICE_INFO_WEB;
		this.notify_url = CALLBACK_URL;
		this.trade_type = TRADE_TYPE_NATIVE;
	}

	public String getAppid() {
		return appid;
	}

	public void setAppid(String appid) {
		this.appid = appid;
	}

	public String getBody() {
		return body;
	}

	public void setBody(String body) {
		this.body = body;
	}

	public String getDevice_info() {
		return device_info;
	}

	public void setDevice_info(String device_info) {
		this.device_info = device_info;
	}

	public String getMch_id() {
		return mch_id;
	}

	public void setMch_id(String mch_id) {
		this.mch_id = mch_id;
	}

	public String getNonce_str() {
		return nonce_str;
	}

	public void setNonce_str(String nonce_str) {
		this.nonce_str = nonce_str;
	}

	public String getNotify_url() {
		return notify_url;
	}

	public void setNotify_url(String notify_url) {
		this.notify_url = notify_url;
	}

	public String getOpenId() {
		return openId;
	}

	public void setOpenId(String openId) {
		this.openId = openId;
	}

	public String getOut_trade_no() {
		return out_trade_no;
	}

	public void setOut_trade_no(String out_trade_no) {
		this.out_trade_no = out_trade_no;
	}

	public String getSpbill_create_ip() {
		return spbill_create_ip;
	}

	public void setSpbill_create_ip(String spbill_create_ip) {
		this.spbill_create_ip = spbill_create_ip;
	}

	public int getTotal_fee() {
		return total_fee;
	}

	public void setTotal_fee(int total_fee) {
		this.total_fee = total_fee;
	}

	public String getTrade_type() {
		return trade_type;
	}

	public void setTrade_type(String trade_type) {
		this.trade_type = trade_type;
	}

	public String getSign() {
		return sign;
	}

	public void setSign(String sign) {
		this.sign = sign;
	}

	public String getProduct_id() {
		return product_id;
	}

	public void setProduct_id(String product_id) {
		this.product_id = product_id;
	}
	public String generateXMLContent() {
		String xml = "<xml>" +
		   "<appid>" + this.appid + "</appid>" + 
		   "<body>" + this.body + "</body>" + 
		   "<device_info>WEB</device_info>" + 
		   "<mch_id>" + this.mch_id + "</mch_id>" + 
		   "<nonce_str>" + this.nonce_str + "</nonce_str>" +
		   "<notify_url>" + this.notify_url + "</notify_url>" + 
		   "<out_trade_no>" + this.out_trade_no + "</out_trade_no>" + 
		   "<product_id>" + this.product_id + "</product_id>" +
		   "<spbill_create_ip>" + this.spbill_create_ip+ "</spbill_create_ip>" +
		   "<total_fee>" + String.valueOf(this.total_fee) + "</total_fee>" + 
		   "<trade_type>" + this.trade_type + "</trade_type>" + 
		   "<sign>" + this.sign + "</sign>" + 
		"</xml>";
		return xml;
	}
	
	public String makeSign() {
		String content ="appid=" + this.appid + 
				   "&body=" + this.body + 
				   "&device_info=WEB" + 
				   "&mch_id=" + this.mch_id + 
				   "&nonce_str=" + this.nonce_str + 
				   "¬ify_url=" + this.notify_url +
				   "&out_trade_no=" + this.out_trade_no + 
				   "&product_id=" + this.product_id + 
				   "&spbill_create_ip=" + this.spbill_create_ip+
				   "&total_fee=" + String.valueOf(this.total_fee) +
				   "&trade_type=" + this.trade_type;
		content = content + "&key=" + WeiXinConstants.MD5_API_KEY;
		String esignature = WeiXinPaymentUtil.MD5Encode(content, "utf-8");
		return esignature.toUpperCase();
	}
	
}
```

其中各个成员变量的解释可以参见【统一下单接口】的说明即可。

有这个之后我们就要要设置的内容填写进去，去调用该接口得到返回数据，从中拿到code_url的数据然后据此生成一个二维图片，把图片的地址返回给PC端网页，然后它就会显示出来，这里要特别说明一下，我们自己PC端网页在点击微信支付的时候就会通过ajax方式调用我们自己后台的SpringMVC Controller然后在Controller的对应方法中通过HTTPClient完成对微信统一下单接口调用解析返回的XML数据得到code_url的值，生成二维码之后返回给前台网页。Controller中实现的代码如下：



```java
Map<String,Object> result=new HashMap<String,Object>();
		UnifiedorderDto dto = new UnifiedorderDto();
		if(cash == null || "".equals(cash)) {
			result.put("error", "cash could not be zero");
			return result;
		}
		int totalfee = 100*Integer.parseInt(cash);
		logger.info("total recharge cash : " + totalfee);
		dto.setProduct_id(String.valueOf(System.currentTimeMillis()));
		dto.setBody("repair");
		dto.setNonce_str(String.valueOf(System.nanoTime()));
		LoginInfo loginInfo = LoginInfoUtil.getLoginInfo();
		// 通过我们后台订单号+UUID为身份识别标志
		dto.setOut_trade_no("你的订单号+关键信息，微信回调之后传回，你可以验证");
		dto.setTotal_fee(totalfee);
		dto.setSpbill_create_ip("127.0.0.1");
		// generate signature
		dto.setSign(dto.makeSign());
		logger.info("sign : " + dto.makeSign());
		logger.info("xml content : " + dto.generateXMLContent());
		try {
			HttpClient httpClient = HttpClientBuilder.create().build(); 
			HttpPost post = new HttpPost(WeiXinConstants.UNIFIEDORDER_URL);
			post.addHeader("Content-Type", "text/xml; charset=UTF-8");
			StringEntity xmlEntity = new StringEntity(dto.generateXMLContent(), ContentType.TEXT_XML);
			post.setEntity(xmlEntity);
			HttpResponse httpResponse = httpClient.execute(post);
			String responseXML = EntityUtils.toString(httpResponse.getEntity(), "UTF-8");
			logger.info("response xml content : " + responseXML);
			// parse CODE_URL CONTENT
			Map<String, String> resultMap = (Map<String, String>)XMLUtil.doXMLParse(responseXML);
			logger.info("response code_url : " + resultMap.get("code_url"));
			String codeurl = resultMap.get("code_url");
			if(codeurl != null && !"".equals(codeurl)) {
				String imageurl = generateQrcode(codeurl);
				result.put("QRIMAGE", imageurl);
			}
			post.releaseConnection();
		} catch(Exception e) {
			e.printStackTrace();
		}
		result.put("success", "1");
		return result;
```



生成二维码的代码如下：

```java
private String generateQrcode(String codeurl) {
		File foldler = new File(basePath + "qrcode");
		if(!foldler.exists()) {
			foldler.mkdirs();
		}
		
		String f_name = UUIDUtil.uuid() + ".png";
        try {
        	File f = new File(basePath + "qrcode", f_name);
        	FileOutputStream fio = new FileOutputStream(f);
        	MultiFormatWriter multiFormatWriter = new MultiFormatWriter();
        	Map hints = new HashMap();
        	hints.put(EncodeHintType.CHARACTER_SET, "UTF-8"); //设置字符集编码类型
        	BitMatrix bitMatrix = null;
            bitMatrix = multiFormatWriter.encode(codeurl, BarcodeFormat.QR_CODE, 300, 300,hints);
            BufferedImage image = toBufferedImage(bitMatrix);
            //输出二维码图片流
            ImageIO.write(image, "png", fio);
            return ("qrcode/" + f_name);
        } catch (Exception e1) {
            e1.printStackTrace();
            return null;
        }     
	}
```
此时如何客户端微信扫码之后，微信就会通过回调我们制定URL返回数据给我们。在回调方法中完成我们自己的处理，这里要特别注意的是你的回调接口必须通过HTTP POST方法实现，否则无法接受到XML数据。回调处理的代码如下：


```java
@RequestMapping(value = "/your_callback_url", method = RequestMethod.POST)
	@ResponseBody
	public void finishPayment(HttpServletRequest request, HttpServletResponse response) {
		try {
			logger.info("start to callback from weixin server: " + request.getRemoteHost());
			Map<String, String> resultMap = new HashMap<String, String>();
			InputStream inputStream = request.getInputStream();
		    // 读取输入流
			SAXBuilder saxBuilder= new SAXBuilder();
		    Document document = saxBuilder.build(inputStream);
		    // 得到xml根元素
		    Element root = document.getRootElement();
		    // 得到根元素的所有子节点
		    List list = root.getChildren();
			Iterator it = list.iterator();
			while(it.hasNext()) {
				Element e = (Element) it.next();
				String k = e.getName();
				String v = "";
				List children = e.getChildren();
				if(children.isEmpty()) {
					v = e.getTextNormalize();
				} else {
					v = XMLUtil.getChildrenText(children);
				}
				resultMap.put(k, v);
			}
			
			// 验证签名！！！
			/*
			String[] keys = resultMap.keySet().toArray(new String[0]);
			Arrays.sort(keys);
			String kvparams = "";
			for(int i=0; i<keys.length; i++) {
				if(keys[i].equals("esign")) {
					continue;
				}
				// 签名算法
				if(i == 0) {
					kvparams += (keys[i] + "=" + resultMap.get(keys[i]));
				} else {
					kvparams += ("&" + keys[i] + "=" + resultMap.get(keys[i]));
				}
			}
			String esign = kvparams + "&key=" + WeiXinConstants.MD5_API_KEY;
			String md5esign = WeiXinPaymentUtil.MD5Encode(esign, "UTF-8");
			if(!md5esign.equals(resultMap.get("sign"))) {
				return;
			}*/
			
			//关闭流
		    // 释放资源
		    inputStream.close();
		    inputStream = null;
		    String returnCode = resultMap.get("return_code");
		    String outtradeno = resultMap.get("out_trade_no");
		    // 以分为单位
		    int nfee = Integer.parseInt(resultMap.get("total_fee"));
		    logger.info("out trade no : " + outtradeno);
		    logger.info("total_fee : " + nfee);
		    // 业务处理流程
		    if("SUCCESS".equals(returnCode)) {		    	
		    	// TODO: your business process add here
		    	response.getWriter().print(XMLUtil.getRetResultXML(resultMap.get("return_code"), resultMap.get("return_code")));
		    } else {
		    	response.getWriter().print(XMLUtil.getRetResultXML(resultMap.get("return_code"), resultMap.get("return_msg")));
		    }
		}
		catch(IOException ioe) {
			ioe.printStackTrace();
		} catch (JDOMException e1) {
			e1.printStackTrace();
		}
	}
```



微信官方Java版Demo用到的XMLUtil和MD5Util的两个类记得拿过来改一下，演示代码可以在它的官方演示页面找到，相关maven依赖如下：



```
<dependency>
			<groupId>jdom</groupId>
			<artifactId>jdom</artifactId>
			<version>1.1</version>
		</dependency>
		<dependency>
			<groupId>com.google.zxing</groupId>
			<artifactId>core</artifactId>
			<version>3.3.0</version>
		</dependency>
```



最后要特别注意的是关于签名，签名生成MD5的类我是从微信官网直接下载Java版Demo程序获取的，建议你也是，因为这个是确保MD5签名是一致的最佳选择。具体的生成签名的算法可以查看微信官方文档，这里也强烈建议大家一定要官方API说明，你开发中所遇到各种问题90%都是因为不看官方文档，而是轻信某人博客！这个才是我写这篇文章的真正目的和用意，根据官方文档，用我的Java代码实现，微信PC端网页扫码支付必定在你的WEB应用中飞起来。



