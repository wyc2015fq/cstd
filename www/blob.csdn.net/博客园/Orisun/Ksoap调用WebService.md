# Ksoap调用WebService - Orisun - 博客园







# [Ksoap调用WebService](https://www.cnblogs.com/zhangchaoyang/articles/1955834.html)





Ksoap是为J2ME调用/制作WebService量身定做的

```
package zcy.org;

import java.io.IOException;

import org.ksoap2.SoapEnvelope;
import org.ksoap2.serialization.SoapObject;
import org.ksoap2.serialization.SoapSerializationEnvelope;
import org.ksoap2.transport.AndroidHttpTransport;
import org.xmlpull.v1.XmlPullParserException;

import android.app.Activity;
import android.os.Bundle;
import android.widget.TextView;

publicclass QQ extends Activity {
    privatestaticfinal String NAMESPACE ="http://WebXml.com.cn/";
    privatestatic String URL ="http://www.webxml.com.cn/webservices/qqOnlineWebService.asmx";
    privatestaticfinal String METHOD_NAME ="qqCheckOnline";
    privatestatic String SOAP_ACTION ="http://WebXml.com.cn/qqCheckOnline";
    
    /** Called when the activity is first created. */
    @Override
    publicvoid onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        TextView tv=(TextView)findViewById(R.id.tv);
        tv.setText(getStat());
    }
    
    private String getStat(){
        /*
         * SoapObject request = new SoapObject(serviceNamespace, methodName);
         * SoapObject构造函数的两个参数含义为：serviceNamespace – 你的webservice的命名空间，既可以是
         * http://localhost:8088/flickrBuddy/services/Buddycast这样的，也可以是
         * urn:PI/DevCentral/SoapService这样的；methodName – 你要调用方法的名字。
         */
        SoapObject rpc =new SoapObject(NAMESPACE, METHOD_NAME);
        /*
         * 对于webservice方法返回String类型的情况，还用不着开发者做序列化(Serialization)定制工作。 要点： kSOAP
         * 1.X/2.0可以自动把四种SOAP类型映射为Java类型 SOAP type Java type xsd:int
         * java.lang.Integer xsd:long java.lang.Long xsd:string java.lang.String
         * xsd:boolean java.lang.Boolean 除此之外，都需要开发者自己做类型映射。
         */
        rpc.addProperty("qqCode", "253640957");
        /*
         * HttpTransport tx = new HttpTransport(serviceURL); ht.debug = true;
         * HttpTransport构造函数的参数含义为： serviceURL – 要投递SOAP数据的目标地址，譬如说
         * http://soap.amazon.com/onca/soap3 。
         * HttpTransport是一个强大的辅助类，来完成Http-call transport
         * process，它封装了网络请求的一切，你完全不用考虑序列化消息。我们通过设置它的debug属性为true来打开调试信息。
         * 方法HttpTransport.call()自己就能够发送请求给服务器、接收服务器响应并序列化SOAP消息，如下所示：
         * ht.call(null, envelope); HttpTransport的call方法的两个参数含义为： soapAction –
         * SOAP 规范定义了一个名为 SOAPAction 的新 HTTP 标头，所有 SOAP HTTP 请求（即使是空的）都必须包含该标头。
         * SOAPAction
         * 标头旨在表明该消息的意图。通常可以置此参数为null，这样HttpTransport就会设置HTTP标头SOAPAction为空字符串。
         * Envelope – 就是前面我们构造好的SoapSerializationEnvelope或SoapEnvelope对象。 注意：
         * 对于HttpTransport的处理上，kSOAP2和kSOAP1.2的写法不一样。 对于kSOAP
         * 1.2，HttpTransport的构造函数是HttpTransport (String url, String
         * soapAction)，第二个参数soapAction可以是要调用的webservice方法名。 而kSOAP 2，构造函数是
         * HttpTransport(String
         * url)。kSOAP2相当于把webservice方法名分离出去，完全交给SoapObject去封装
         * ，而HttpTransport仅仅负责把SoapEnvelope发送出去并接收响应
         */
        AndroidHttpTransport ht =new AndroidHttpTransport(URL);
        /*
         * 是否测试模式
         */
        ht.debug =false;
        /*
         * SoapSerializationEnvelope envelope =new
         * SoapSerializationEnvelope(SoapEnvelope.VER11); envelope.bodyOut =
         * request; 要点：
         * 你可以通过SoapSerializationEnvelope或者SoapEnvelope的构造函数来指明你要用SOAP的哪一个规范
         * ，可以是以下几种之一： 常量SoapEnvelope.VER10：对应于SOAP 1.0规范
         * 常量SoapEnvelope.VER11：对应于SOAP 1.1规范 常量SoapEnvelope.VER12：对应于SOAP 1.2规范
         * 这样，无论要调用的webservice采用了哪一个SOAP规范，你都可以轻松应对。
         */
        SoapSerializationEnvelope envelope =new SoapSerializationEnvelope(
                SoapEnvelope.VER11);
        /*
         * 发送请求
         */
        envelope.bodyOut = rpc;
        envelope.dotNet =true;
        envelope.setOutputSoapObject(rpc);
        /*
         * ht.call(SOAP_ACTION, envelope);
         */
        try {
            ht.call(SOAP_ACTION, envelope);
        } catch (IOException e) {
            return"IOException:"+e.getMessage();
        } catch (XmlPullParserException e) {
            return"XmlPullParserException:"+e.getMessage();
        }
        SoapObject result = (SoapObject) envelope.bodyIn;
        // result.getProperty(1);
        // SoapObject detail = (SoapObject)result.getProperty("qqCheckOnlineResult");
return (String) result.getProperty("qqCheckOnlineResult");
    }
}
```












