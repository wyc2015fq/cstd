# Request processing failed; nested exception is java.lang.NullPointerException] with root cause - BlueSky - CSDN博客
2016年01月12日 10:58:09[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：9179
出现错误：
```
SEVERE: Servlet.service() for servlet [dispatcher] in context with path [] threw exception [Request processing failed; nested exception is java.lang.NullPointerException] with root cause
root cause
   java.lang.NullPointerException   com.ulotrix.spring.service.PersonServiceImpl.listPersons(PersonServiceImpl.java:35) 
    com.ulotrix.spring.controller.PersonController.listPersons(PersonController.java:28)    
     sun.reflect.NativeMethodAccessorImpl.invoke0(Native Method)
```
根本原因是引入的Service没有加@Autowired
@Autowired
private GetAppHighUrlByItimeService getAppHighUrlByItimeService;
