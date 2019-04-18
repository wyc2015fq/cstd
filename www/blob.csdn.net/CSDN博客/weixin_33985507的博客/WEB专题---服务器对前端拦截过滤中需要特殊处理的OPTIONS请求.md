# WEB专题---服务器对前端拦截过滤中需要特殊处理的OPTIONS请求 - weixin_33985507的博客 - CSDN博客
2018年04月26日 20:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：114
是否是跨域请求呢？
在网上找到了相关资料([http://www.cnblogs.com/sniper...](https://link.jianshu.com?t=https%3A%2F%2Flink.segmentfault.com%2F%3Fr%3Dhttp%253A%252F%252Fwww.cnblogs.com%252Fsniper007%252Fp%252F3357018.html%2529)：
OPTIONS请求方法的主要用途有两个：
1、获取服务器支持的HTTP请求方法；也是黑客经常使用的方法。
2、用来检查服务器的性能。例如：AJAX进行跨域请求时的预检，需要向另外一个域名的资源发送一个HTTP OPTIONS请求头，用以判断实际发送的请求是否安全。
fetch post修改了请求头,导致fetch第一发送一个options请求，询问服务器是否支持修改的请求头，如过服务器支持，那么将会再次发送真正的请求。
![2717693-cefb0371b8286ad6.png](https://upload-images.jianshu.io/upload_images/2717693-cefb0371b8286ad6.png)
image.png
![2717693-acc6474ff303267d.png](https://upload-images.jianshu.io/upload_images/2717693-acc6474ff303267d.png)
image.png
处理方式 JAVA
```
package com.yt.design.web;
import com.yt.design.entity.JsonResponseEntity;
import com.yt.design.entity.JsonResponseStatus;
import com.yt.design.utils.JsonConverter;
import com.yt.design.utils.LogUtil;
import org.springframework.util.StringUtils;
import org.springframework.web.servlet.HandlerInterceptor;
import org.springframework.web.servlet.ModelAndView;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import java.io.IOException;
import java.io.PrintWriter;
/**
 * @Description a
 * @Author
 * @Create 2018-04-23 下午3:01
 **/
public class WebDefaultHandlerInterceptor implements HandlerInterceptor {
    private static final String accesstokenHeader = "access-token";
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        LogUtil.log(LogUtil.LogType.info, " preHandle :" + request.getMethod() + "   " + request.getRequestURI());
        //跨域请求会自动发送一次OPTIONS请求以用来检测请求是否安全可靠,需要做特殊过滤 https://segmentfault.com/q/1010000008693779
        System.out.println();
        if (request.getMethod().equals("OPTIONS")) {
            return true;
        }
        String token = request.getHeader(accesstokenHeader);
        if (!StringUtils.isEmpty(token)) {
            return true;
        } else {
            buildGuestResponseBody(response, JsonResponseStatus.CODE_SERER_HANDLE_FAILED, "sessionId is invalid,please login again!");
            return false;
        }
    }
    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView) throws Exception {
    }
    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) throws Exception {
    }
    private void buildGuestResponseBody(HttpServletResponse response, int code, String msg) {//return a guest token when token check failed.
        try {
            response.setContentType("application/json;charset=UTF-8");
            response.setHeader("Access-Control-Allow-Origin", "*");
            response.setHeader("Access-Control-Allow-Credentials", "true");
            response.setHeader("Access-Control-Allow-Methods", "POST, GET, OPTIONS, DELETE");
            response.setHeader("Access-Control-Max-Age", "3600");
            response.setHeader("Access-Control-Allow-Headers", "Content-Type, Accept, X-Requested-With, remember-me");
            PrintWriter writer = response.getWriter();
            JsonResponseEntity result = new JsonResponseEntity();
            result.setCode(code);
            result.setMsg(msg);
            writer.write(JsonConverter.toJson(result));
            writer.close();
        } catch (IOException e) {
            //ignore
        }
    }
}
```
COVER：
[https://segmentfault.com/q/1010000008693779](https://link.jianshu.com?t=https%3A%2F%2Fsegmentfault.com%2Fq%2F1010000008693779)
