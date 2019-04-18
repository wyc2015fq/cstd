# web.xml filter 绕过匹配访问（针对jetty） - z69183787的专栏 - CSDN博客
2018年12月06日 11:17:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：146
使用filter时匹配URL 校验权限时，
jetty 在匹配filter 时，使用了相对严格匹配模式，导致
/api/xxxx/search 命中匹配规则 /api/* 返回权限校验失败
//api/xxxx/search 没有命中规则 /api/* 从而绕过filter，并返回对应信息
修复1： 修改匹配规则为 /* ,要求所有请求都经过filter
修复2：增加自定义过滤器，防止 //  的情况
```java
public void doFilter(ServletRequest request, ServletResponse response, FilterChain filterChain) throws IOException, ServletException {
    HttpServletRequest httpRequest = (HttpServletRequest)request;
    String path = httpRequest.getRequestURI();
    if (path.indexOf("//") < 0) {
        filterChain.doFilter(request, response);
    } else {
        response.setContentType("application/json; charset=UTF-8");
        response.getWriter().write("url error,plz check");
    }
}
```
