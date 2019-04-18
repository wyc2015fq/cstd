# Spring MVC中的ResponseEntity和ResponseBody的区别 - z69183787的专栏 - CSDN博客
2018年06月30日 21:31:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：399
[https://blog.csdn.net/qq_36582604/article/details/80520937](https://blog.csdn.net/qq_36582604/article/details/80520937)
1.ResponseEntity的优先级高于@ResponseBody。在不是ResponseEntity的情况下才去检查有没有@ResponseBody注解。如果响应类型是ResponseEntity可以不写@ResponseBody注解，写了也没有关系。
2.ResponseEntity 是在 org.springframework.http.HttpEntity 的基础上添加了http status code(http状态码)，用于RestTemplate以及@Controller的HandlerMethod。它在Controoler中或者用于服务端响应时，作用是和@ResponseStatus与@ResponseBody结合起来的功能一样的。用于RestTemplate时，它是接收服务端返回的http status code 和 reason的。
总结：
简单粗暴的讲
@ResponseBody可以直接返回Json结果，
@ResponseEntity不仅可以返回json结果，还可以定义返回的HttpHeaders和HttpStatus
