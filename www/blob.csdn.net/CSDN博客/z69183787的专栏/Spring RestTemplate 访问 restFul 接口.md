# Spring RestTemplate 访问 restFul 接口 - z69183787的专栏 - CSDN博客
2017年07月03日 16:28:14[OkidoGreen](https://me.csdn.net/z69183787)阅读数：824
 Get：
```java
RestTemplate restTemplate = new RestTemplate();
        Map<String, String> urlVariables = new HashMap<String, String>();
        urlVariables.put("userId","171348179");
        ResponseFormat responseFormat =
                restTemplate.getForObject("http://act.dianping.com/customized/wallet517?userId={userId}",ResponseFormat.class,urlVariables);
```
Post：
差不多
