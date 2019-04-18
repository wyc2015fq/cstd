# Java开发的WebService获取客户端IP地址 - z69183787的专栏 - CSDN博客
2014年05月09日 09:47:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3135
1.基于jax-ws开发的webservice获取客户端ip的方法【在代理类XXXDelegate添加如下代码】：
　　import javax.annotation.Resource;
　　import javax.servlet.http.HttpServletRequest;
　　import javax.xml.ws.WebServiceContext;
　　import javax.xml.ws.handler.MessageContext;
　　@Resource
　　private WebServiceContext wsContext;
　　private String getClientInfo(){
　　MessageContext mc = wsContext.getMessageContext();
　　HttpServletRequest request = (HttpServletRequest)(mc.get(MessageContext.SERVLET_REQUEST));
　　String remortAddress = request.getRemoteAddr();
　　return (remortAddress);
　　}
　　2.基于axis发布的webservice获取客户端ip的方法
　　/**
　　* 获取客户端IP地址，如调用方的IP，以便检查权限。
　　* 适用于axis发布的webservice
　　* @return
　　*/
　　public String getClientIpAxis() {
　　MessageContext mc = null;
　　HttpServletRequest request = null;
　　try {
　　mc = MessageContext.getCurrentMessageContext();
　　if (mc == null)
　　throw new Exception(“无法获取到MessageContext”);
　　request = (HttpServletRequest) mc
　　.getProperty(HTTPConstants.MC_HTTP_SERVLETREQUEST);
　　System.out.println(“remote  ip:  ” + request.getRemoteAddr());
　　} catch (Exception e) {
　　System.out.println(e.getMessage());
　　e.printStackTrace();
　　}
　　return request.getRemoteAddr();
　　}
　　3.基于xfire发布的webservice获取客户端ip的方法
　　/**
　　* 获取客户端IP地址
　　* 适用于xfire发布的webservice
　　* @return
　　*/
　　public String getClientIpXfire() {
　　String ip = “”;
　　try {
　　HttpServletRequest request = XFireServletController.getRequest();
　　ip = request.getRemoteAddr();
　　} catch (Exception e) {
　　System.out.println(“无法获取HttpServletRequest.”);
　　e.printStackTrace();
　　}
　　return ip;
　　}
