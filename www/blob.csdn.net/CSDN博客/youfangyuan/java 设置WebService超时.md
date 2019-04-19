# java 设置WebService超时 - youfangyuan - CSDN博客
2013年05月06日 23:14:40[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：21047
```java
Service model = new ObjectServiceFactory().create(TaskService.class);
XFireProxyFactory factory = new XFireProxyFactory(XFireFactory.newInstance().getXFire());
TaskService service = (TaskService) factory.create(model, url);
Client client =Client.getInstance(service);
client.setTimeout(10000);
client.setProperty(CommonsHttpMessageSender.HTTP_TIMEOUT, String.valueOf(10000));
client.setProperty(CommonsHttpMessageSender.DISABLE_KEEP_ALIVE, "true");
client.setProperty(CommonsHttpMessageSender.DISABLE_EXPECT_CONTINUE, "true");
```
```java
ShipInterfaceWebService  ws = new ShipInterfaceWebService(url,new QName("http://shipInterface.sni.hpb.gov.sg/", "ShipInterfaceWebService"));
IShipWebService ishipws = ws.getIShipWebServicePort();
BindingProvider bp = (BindingProvider)ishipws;
Map requestContext = bp.getRequestContext();
requestContext.put(JAXWSProperties.CONNECT_TIMEOUT,1000*60); 
requestContext.put(BindingProviderProperties.REQUEST_TIMEOUT,1000*60);
```
```java
//1,在CXF生成的stub的客户端,方式如下(CXF2.1.1):
        SimpleServiceService srv = new SimpleServiceService();
        SimpleService servicePort = srv.getSimpleServicePort();
    
        //timeout setting
        Client cl = ClientProxy.getClient(servicePort);
        HTTPConduit http = (HTTPConduit) cl.getConduit();
        HTTPClientPolicy httpClientPolicy = new HTTPClientPolicy();
        httpClientPolicy.setConnectionTimeout(10000);
        httpClientPolicy.setReceiveTimeout(1000);
        http.setClient(httpClientPolicy);
        
        servicePort.concatenate("111", "2222");
//其中SimpleServiceService是自动生成的stub类.
 
//2,在Xfire生成的stub的客户端,方式如下(Xfire1.26):
        SimpleServiceServiceClient serviceClient= new SimpleServiceServiceClient();
        SimpleService  servicePort = serviceClient.getSimpleServicePort();
        
        HttpClientParams params = new HttpClientParams();
        params.setParameter(HttpClientParams.USE_EXPECT_CONTINUE, Boolean.FALSE);
        // set connection timeout
        params.setParameter(HttpClientParams.CONNECTION_MANAGER_TIMEOUT, 2L);
        //set response timeout 
        params.setIntParameter(HttpClientParams.SO_TIMEOUT,2);
        
        Client client = Client.getInstance(servicePort);
        client.setProperty(CommonsHttpMessageSender.HTTP_CLIENT_PARAMS, params);
        
        System.out.println(servicePort.concatenate("xfire1", "xfire2"));
//其中SimpleServiceServiceClient 是自动生成的stub类.
 
//上面两种方式都是通过控制底层的http连接来达到timeout控制,如果超时会抛出 java.net.SocketTimeoutException: Read timed out.
 
//3,jax-ws的客户端:同样利用CXF的ClientProxy控制http连接:
        URL wsdlURL = new URL("file:///D:/JAVAWorkspace/Repository/prjCXFWS/src/wsdl/prjCXFWS.wsdl");
        
        QName serviceQName = new QName("http://test.cxfws.com/", "SimpleServiceService");
        
        QName portQName = new QName("http://test.cxfws.com/", "SimpleServicePort");
        
        Service service = Service.create(wsdlURL, serviceQName);
        
        
        //set handler
        service.setHandlerResolver(new RequestOrderHandlerResolver());
        
        SimpleService port = (SimpleService) service.getPort(portQName, SimpleService.class);
        //timeout setting
        Client cl = ClientProxy.getClient(port);
        HTTPConduit http = (HTTPConduit) cl.getConduit();
        HTTPClientPolicy httpClientPolicy = new HTTPClientPolicy();
        httpClientPolicy.setConnectionTimeout(10000);
        httpClientPolicy.setReceiveTimeout(1000);
        http.setClient(httpClientPolicy);
        System.out.println(port.concatenate("srt1", "srt2"));
 
//4,jax-Dispatch,在网上搜索到有人说用以下方式:试了一下不好用.
//dispatch.getRequestContext().put("com.sun.xml.ws.request.timeout", 1000);
```
