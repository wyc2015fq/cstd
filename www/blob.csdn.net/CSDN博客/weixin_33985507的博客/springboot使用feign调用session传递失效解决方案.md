# springboot使用feign调用session传递失效解决方案 - weixin_33985507的博客 - CSDN博客
2019年01月22日 09:15:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：41
|标题|框架|version|
|----|----|----|
|1|springboot|1.5.9.release|
|2|springCloud|1.2.4.release|
|3|eureka|1.6.2|
|4|feign|9.5.0|
### 前言
使用场景：由于我们项目使用的是微服务框架，各个服务之间的调用，使用的是feign调用，我们项目调用各个模块直接调用，如果涉及到跨服务调用的话，开始发现从request里面的什么东西都获取不到，登录的request的session肯定是空的，但是数据调用能带过来没有什么问题。我们增加了各个服务的拦截器之后，需要从request获取当前请求的用户session。这个肯定就不行了。我们就开始寻找解决方案，最后通过踩坑终于解决session失效的问题了（解决方案如下）。
### 1.解决办法：
使用feign调用的时候通过使用requestInterceptor拦截器对请求进行拦截，代码如下：
```
package com.xxx.system.feign;
import java.util.Enumeration;
import javax.servlet.http.HttpServletRequest;
import org.springframework.web.context.request.RequestContextHolder;
import org.springframework.web.context.request.ServletRequestAttributes;
import feign.RequestInterceptor;
import feign.RequestTemplate;
import lombok.extern.slf4j.Slf4j;
@Slf4j
public class FeignBasicAuthRequestInterceptor implements RequestInterceptor {
      @Override
      public void apply(RequestTemplate requestTemplate) {
        ServletRequestAttributes attributes = (ServletRequestAttributes) RequestContextHolder
                .getRequestAttributes();
        HttpServletRequest request = attributes.getRequest();
        Enumeration<String> headerNames = request.getHeaderNames();
        if (headerNames != null) {
          while (headerNames.hasMoreElements()) {
            String name = headerNames.nextElement();
            String values = request.getHeader(name);
            requestTemplate.header(name, values);
          }
        }
        //下面注释掉的代码，如果你放开，默认是post请求（如果你的接口是get请求放开了此代码，回报错：405不支持post请求。）
        /*Enumeration<String> bodyNames = request.getParameterNames();
          StringBuffer body =new StringBuffer();
          if (bodyNames != null) {
              while (bodyNames.hasMoreElements()) {
                String name = bodyNames.nextElement();
                String values = request.getParameter(name);
                body.append(name).append("=").append(values).append("&");
              }
          }
         if(body.length()!=0) {
            body.deleteCharAt(body.length()-1);
            requestTemplate.body(body.toString());
            log.info("feign interceptor body:{}",body.toString());
        }*/
      }
}
```
注意：下面注释掉的代码，如果你放开，默认是post请求（如果你的接口是get请求放开了此代码，回报错：405不支持post请求。（踩的坑）；**这个拦截器配置加到什么位置呢？设计到模块调用的服务都需要加一个这个配置。**
#### yml配置：
```
hystrix:
  command:
    default:
      execution:
        timeout:
          enabled: false
        isolation:
          strategy: SEMAPHORE
```
### 备注：
**如果使用过程中遇到问题请留言，如果有什么不对的请高手们指正谢谢。**
