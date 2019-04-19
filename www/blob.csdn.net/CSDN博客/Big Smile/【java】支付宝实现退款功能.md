# 【java】支付宝实现退款功能 - Big Smile - CSDN博客
2018年05月06日 22:58:48[王啸tr1912](https://me.csdn.net/tr1912)阅读数：4880
所属专栏：[java学习](https://blog.csdn.net/column/details/26775.html)
        最近完成的差不多的项目突然需要加退款的流程需求了，所以来小小的实现以下。
其实对比其他的支付和退款来说，支付宝算是特别专业，也是特别简单的一个了，所以，先拿他来开刀了。
# 一、引入支付宝SDK
        在这里说一下，其实每种支付都会有一个官方带的SDK，我们可以把这个sdk引入到我们的项目中去，然后我们就可以调用里面的方法和对象，给里面传值，进行我们相应的请求的访问和介入。
![](https://img-blog.csdn.net/20180506234647148?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3RyMTkxMg==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
首先引入如上图所示的jar包，然后我们就算是成功了一半了。下载地址：
[https://docs.open.alipay.com/54/103419/](https://docs.open.alipay.com/54/103419/)
         然后我们需要把这个jar包引入到所需要的地方，这里不做过多的描述。
# 二、找到需要的资源
        做支付，最最基本的还是各种appid，还有公钥，私钥，以及请求的网关等等内容，支付宝需要的包括：
APP_ID：开放平台账号申请的appid
requestUrl：实现功能所需要请求的网关地址
APP_PRIVATE_KEY：用户自己生成的私钥
ALIPAY_PUBLIC_KEY：支付宝生成的公钥
CHARSET：我们需要的字符编码，一般为utf-8的
# 三、编写请求代码
```java
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import com.alipay.api.AlipayClient;
import com.alipay.api.DefaultAlipayClient;
import com.alipay.api.domain.AlipayTradeRefundModel;
import com.alipay.api.request.AlipayTradeRefundRequest;
import com.alipay.api.response.AlipayTradeRefundResponse;
public class AlipayUtil {
	
    private static final Logger LOGGER = LoggerFactory.getLogger(AlipayUtil.class);   
    private static String APP_ID = "234789720984098";
	
    //支付宝退款请求的网关
    private static String requestUrl = "https://openapi.alipay.com/gateway.do";
    //用户自己生成的私钥
    private static String APP_PRIVATE_KEY = "随机字符，见文档";
    //支付宝公钥
    private static String ALIPAY_PUBLIC_KEY = "从支付宝中获得";
    
    //编码级别
    private static String CHARSET = "UTF-8";
    
    public static String refundOrder(AliRefundTO aliRefundTO){
  	System.out.println("开始调用支付宝加密******************************************************");
  	//实例化客户端
    	AlipayClient alipayClient = new DefaultAlipayClient(requestUrl, APP_ID, APP_PRIVATE_KEY, "json", CHARSET, ALIPAY_PUBLIC_KEY, "RSA2");
    	//SDK已经封装掉了公共参数，这里只需要传入业务参数。以下方法为sdk的model入参方式(model和biz_content同时存在的情况下取biz_content)。
    	AlipayTradeRefundModel refundModel = new AlipayTradeRefundModel();
    	refundModel.setTradeNo(aliRefundTO.getTrade_no());
    	refundModel.setRefundAmount(aliRefundTO.getRefund_amount());
    	refundModel.setRefundReason("商品退款");
    	//实例化具体API对应的request类,类名称和接口名称对应,当前调用接口名称：alipay.trade.app.pay
    	AlipayTradeRefundRequest request = new AlipayTradeRefundRequest();
    	request.setBizModel(refundModel);
    	try{
    		AlipayTradeRefundResponse response = alipayClient.execute(request);
    		System.out.println(response.getMsg()+"\n");
    		System.out.println(response.getBody());
    	}catch(Exception e){
    		e.printStackTrace();
    		LOGGER.error("支付宝退款错误！",e.getMessage());    		
    	}    
    	return "";    	
  	}
  
  	public static void main(String[] arg){
  		AliRefundTO aliRefundTO=new AliRefundTO();
  		aliRefundTO.setRefund_amount("0.01");
  		aliRefundTO.setTrade_no("2018050421001004310518105890");
  		String res=refundOrder(aliRefundTO);
  	}
}
```
这里的AliRefundTO 是封装的一个实体，里面都是退款所需要的数据，包括流水单号，还有需要退款的金额等等。
