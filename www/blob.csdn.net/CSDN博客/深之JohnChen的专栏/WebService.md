# WebService - 深之JohnChen的专栏 - CSDN博客

2013年08月03日 15:24:56[byxdaz](https://me.csdn.net/byxdaz)阅读数：4499


WebService是一种基于SOAP协议的远程调用标准，通过webservice可以将不同操作系统平台、不同语言、不同技术整合到一块。在Android
 SDK中并没有提供调用WebService的库，因此，需要使用第三方的SDK来调用WebService。PC版本的WEbservice客户端库非常丰富，例如Axis2，CXF等，但这些开发包对于Android系统过于庞大，也未必很容易移植到Android系统中。因此，这些开发包并不是在我们的考虑范围内。适合手机的WebService客户端的SDK有一些，比较常用的有Ksoap2，可以从http://code.google.com/p/ksoap2-android/downloads/list进行下载；将下载的ksoap2-android-assembly-2.4-jar-with-dependencies.jar包复制到Eclipse工程的lib目录中，当然也可以放在其他的目录里。同时在Eclipse工程中引用这个jar包。

具体调用调用webservice的方法为：

（1）指定webservice的命名空间和调用的方法名，如：

SoapObject request =new SoapObject(http://service,”getName”);

SoapObject类的第一个参数表示WebService的命名空间，可以从WSDL文档中找到WebService的命名空间。第二个参数表示要调用的WebService方法名。

（2）设置调用方法的参数值，如果没有参数，可以省略，设置方法的参数值的代码如下：

Request.addProperty(“param1”,”value”);

Request.addProperty(“param2”,”value”);

要注意的是，addProperty方法的第1个参数虽然表示调用方法的参数名，但该参数值并不一定与服务端的WebService类中的方法参数名一致，只要设置参数的顺序一致即可。

（3）生成调用Webservice方法的SOAP请求信息。该信息由SoapSerializationEnvelope对象描述，代码为：

SoapSerializationEnvelope envelope=new  SoapSerializationEnvelope(SoapEnvelope.VER11);

Envelope.bodyOut = request;

创建SoapSerializationEnvelope对象时需要通过SoapSerializationEnvelope类的构造方法设置SOAP协议的版本号。该版本号需要根据服务端WebService的版本号设置。在创建SoapSerializationEnvelope对象后，不要忘了设置SOAPSoapSerializationEnvelope类的bodyOut属性，该属性的值就是在第一步创建的SoapObject对象。

（4）创建HttpTransportsSE对象。通过HttpTransportsSE类的构造方法可以指定WebService的WSDL文档的URL：

HttpTransportSE ht=new HttpTransportSE(“http://192.168.18.17:80

/axis2/service/SearchNewsService?wsdl”);

(5)使用call方法调用WebService方法，代码：

ht.call（null，envelope）；

Call方法的第一个参数一般为null，第2个参数就是在第3步创建的SoapSerializationEnvelope对象。

（6）使用getResponse方法获得WebService方法的返回结果，代码：

SoapObject soapObject =( SoapObject) envelope.getResponse();

以下为简单的实现一个天气查看功能的例子：

publicclass WebService extends Activity { 

privatestaticfinal String NAMESPACE ="http://WebXml.com.cn/"; 

// WebService地址

privatestatic String URL ="http://www.webxml.com.cn/

webservices/weatherwebservice.asmx"; 

privatestaticfinal String METHOD_NAME ="getWeatherbyCityName"; 

privatestatic String SOAP_ACTION ="http://WebXml.com.cn/

getWeatherbyCityName"; 

private String weatherToday; 

private Button okButton; 

private SoapObject detail; 

@Override  

publicvoid onCreate(Bundle savedInstanceState) { 

  super.onCreate(savedInstanceState); 

  setContentView(R.layout.main); 

  okButton = (Button) findViewById(R.id.ok); 

  okButton.setOnClickListener(new Button.OnClickListener() { 

      publicvoid onClick(View v) { 

         showWeather(); 

      }  

  });  

}  

privatevoid showWeather() { 

  String city ="武汉"; 

  getWeather(city); 

}  

@SuppressWarnings("deprecation") 

publicvoid getWeather(String cityName) { 

try {  

 System.out.println("rpc------"); 

  SoapObject rpc =new SoapObject(NAMESPACE, METHOD_NAME); 

  System.out.println("rpc"+ rpc); 

  System.out.println("cityName is "+ cityName); 

 rpc.addProperty("theCityName", cityName); 

  AndroidHttpTransport ht =new AndroidHttpTransport(URL);  //AndroidHttpTransport
是一个要过期的类，可以使用HttpTransportsSE对象

  ht.debug =true;  

SoapSerializationEnvelope envelope =new SoapSerializationEnvelope( 

          SoapEnvelope.VER11); 

  envelope.bodyOut = rpc; 

  envelope.dotNet =true; 

 envelope.setOutputSoapObject(rpc); 

  ht.call(SOAP_ACTION, envelope); 

 SoapObject result = (SoapObject) envelope.bodyIn;  

  detail = (SoapObject) result 

          .getProperty("getWeatherbyCityNameResult"); 

  System.out.println("result"+ result); 

  System.out.println("detail"+ detail); 

  Toast.makeText(WebService.this, detail.toString(), 

         Toast.LENGTH_LONG).show(); 

  parseWeather(detail); 

  return;  

} catch (Exception e) { 

  e.printStackTrace(); 

  }  

}  

privatevoid parseWeather(SoapObject detail) 

      throws UnsupportedEncodingException { 

  String date = detail.getProperty(6).toString();  

  weatherToday ="今天："+ date.split("")[0]; 

  weatherToday = weatherToday +"\n天气："+ date.split("")[1]; 

  weatherToday = weatherToday +"\n气温：" 

          + detail.getProperty(5).toString(); 

 weatherToday = weatherToday +"\n风力：" 

          + detail.getProperty(7).toString() +"\n"; 

  System.out.println("weatherToday is "+ weatherToday); 

  Toast.makeText(WebService.this, weatherToday,

  Toast.LENGTH_LONG).show(); 

 }  

}

项目遇到的bug如下：当手机使用wifi或者net访问webservice的时候一切正常，但是当手机网络为wap的情况下，就连接失败。解决办法：ksoap2-android最新版本是2.5.4。在2.5.2之前源码都是不支持代理访问的，在2.5.4之后，源码增加了对网络有代理的支持。

//判断当前网络是否是net，true为net，false为wap

       public static boolean isGprsNet(){

              String proxyHost = android.net.Proxy.getDefaultHost();

              return proxyHost==null;

       }

       //根据网络类型返回相应的HttpTransportSE  webservice用

       public static HttpTransportSE getHttpTransportSE(Context context){

              HttpTransportSE ht;

              if(!HttpUtil.isGprsNet()){

                  java.net.Proxy p = new java.net.Proxy(java.net.Proxy.Type.HTTP,new InetSocketAddress(android.net.Proxy.getDefaultHost(),android.net.Proxy.getDefaultPort()));

                  ht=new HttpTransportSE(p,context.getString(R.string.wsurl));

           }else{

                  ht = new HttpTransportSE(context.getString(R.string.wsurl));

           }

              return ht;

       }

       /*

        * 登录接口  1成功  0失败

        */

       public String login(Context context,String phoneNumber,String pwd){

              SoapObject request = new SoapObject(context.getString(R.string.namespace), "login");

              request.addProperty("usrPhone", phoneNumber);

              request.addProperty("passWord", pwd);

              SoapSerializationEnvelope envelope = new SoapSerializationEnvelope( SoapEnvelope.VER11 );

           envelope.bodyOut=request;

           HttpTransportSE ht = HttpUtil.getHttpTransportSE(context);

           try {

                     ht.call(null, envelope);

              } catch (Exception e) {}

              SoapObject so = null;

              so = (SoapObject) envelope.bodyIn;

              String result= so.getProperty("return").toString();

              return result;

       }

androi下net和wap自适应代码：

private HttpURLConnection getURLConnection(String url) throws Exception {    

          String proxyHost = android.net.Proxy.getDefaultHost();    

          if (proxyHost != null) {    

              java.net.Proxy p = new java.net.Proxy(java.net.Proxy.Type.HTTP,    

                      new InetSocketAddress(android.net.Proxy.getDefaultHost(),    

                              android.net.Proxy.getDefaultPort()));    

              return (HttpURLConnection) new URL(url).openConnection(p);    

         } else {    

             return (HttpURLConnection) new URL(url).openConnection();    

         }     

     }    

