## [javaweb学习总结(二十二)——基于Servlet+JSP+JavaBean开发模式的用户登录注册](https://www.cnblogs.com/xdp-gacl/p/3902537.html)

## 一、Servlet+JSP+JavaBean开发模式(MVC)介绍

　　Servlet+JSP+JavaBean模式(MVC)适合开发复杂的web应用，在这种模式下，servlet负责处理用户请求，jsp负责数据显示，javabean负责封装数据。  Servlet+JSP+JavaBean模式程序各个模块之间层次清晰，web开发推荐采用此种模式。

　　这里以一个最常用的用户登录注册程序来讲解Servlet+JSP+JavaBean开发模式，通过这个用户登录注册程序综合案例，把之前的学过的XML、Xpath、Servlet、jsp的知识点都串联起来。

## 二、创建MVC架构的Web项目

　　在MyEclipse中新创建一个webmvcframework项目，导入项目所需要的开发包(jar包)，创建项目所需要的包，在java开发中，架构的层次是以包的形式体现出来的

**项目所需要的开发包(jar包)**

| 序号 | 开发包名称                      | 描述                                       |
| ---- | ------------------------------- | ------------------------------------------ |
| 1    | **dom4j-1.6.1.jar**             | dom4j用于操作XML文件                       |
| 2    | **jaxen-1.1-beta-6.jar**        | 用于解析XPath表达式                        |
| 3    | **commons-beanutils-1.8.0.jar** | 工具类，用于处理bean对象                   |
| 4    | **commons-logging.jar**         | **commons-beanutils-1.8.0.jar**的依赖jar包 |
| 5    | **jstl.jar**                    | jstl标签库和EL表达式依赖包                 |
| 6    | **standard.jar**                | jstl标签库和EL表达式依赖包                 |

 

　　

 **项目所需要的包**

| 序号  | 包名                       | 描述                                                         | 所属层次         |
| ----- | -------------------------- | ------------------------------------------------------------ | ---------------- |
| **1** | **me.gacl.domain**         | 存放系统的JavaBean类(只包含简单的属性以及属性对应的get和set方法，不包含具体的业务处理方法)，提供给【数据访问层】、【业务处理层】、【Web层】来使用 | domain(域模型)层 |
| 2     | **me.gacl.dao**            | 存放访问数据库的操作接口类                                   | 数据访问层       |
| 3     | **me.gacl.dao.impl**       | 存放访问数据库的操作接口的实现类                             | -                |
| 4     | **me.gacl.service**        | 存放处理系统业务接口类                                       | 业务处理层       |
| 5     | **me.gacl.service.impl**   | 存放处理系统业务接口的实现类                                 | -                |
| 6     | **me.gacl.web.controller** | 存放作为系统控制器的Servlet                                  | Web层(表现层)    |
| 7     | **me.gacl.web.UI**         | 存放为用户提供用户界面的servlet(UI指的是user interface)      | -                |
| 8     | **me.gacl.web.filter**     | 存放系统的用到的过滤器(Filter)                               |                  |
| 9     | **me.gacl.web.listener**   | 存放系统的用到的监听器(**Listener**)                         |                  |
| 10    | **me.gacl.util**           | 存放系统的通用工具类，提供给【数据访问层】、【业务处理层】、【Web层】来使用 |                  |
| 11    | **junit.test**             | 存放系统的测试类                                             |                  |

 

　　一个良好的JavaWeb项目架构应该具有以上的11个包，这样显得层次分明，各个层之间的职责也很清晰明了，搭建JavaWeb项目架构时，就按照上面的1~11的序号顺序创建包：domain→dao→dao.impl→service→service.impl→web.controller→web.UI→web.filter→web.listener→util→junit.test，包的层次创建好了，项目的架构也就定下来了，当然，在实际的项目开发中，也不一定是完完全全按照上面说的来创建包的层次结构，而是根据项目的实际情况，可能还需要创建其他的包，这个得根据项目的需要来定了

　　在src目录(类目录)下面，创建用于保存用户数据的xml文件(DB.xml)

　　在WEB-INF目录下创建一个pages目录，pages目录存放系统的一些受保护(不允许用户直接通过URL地址访问)的jsp页面，用户要想访问这些受保护的jsp页面，那么只能通过**me.gacl.web.UI**这个包里面的Servlet

　　创建好的项目如下图(图-1)所示：

　　![img](data:image/png;base64,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)

　　　　　　　　　　　　　　　　图-1

## 三、分层架构的代码编写

　　分层架构的代码也是按照【域模型层(domain)】→【数据访问层(dao、dao.impl)】→【业务处理层(service、service.impl)】→【表现层(web.controller、web.UI、web.filter、web.listener)】→【工具类(util)】→【测试类(junit.test)】的顺序进行编写的。

### 3.1、开发domain层

　　在**me.gacl.domain**包下创建一个User类

　　![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIgAAAAoCAIAAACqxCXrAAAHj0lEQVRoge1aW08bRxSef5HHqKkaRUGVYrBpu6Uo/yAvTcBpolStNOpFVaCXRNzFpZRSNR2VKmmTlJC2MSRBKUqaJVFKaaRaMUlDC00TxAqMgzFe27sYfFuz0OnDrvfi3TUuXrBV+egTmjnznTPH83lmvDYAF60gDfzXgLX1f0Ymma+GPR2DM0WYha+GPSOTzNr6P5sX5qffAzfHQ16WX+FwEWbBy/I3x0M//R7YvDDdQ7NMbJ2J41CsCNPAxDETW+8emt28MB2DM8scDkaLMBnLHO4YnMlJmHAC0xF91FbsMBoqJJBHAezPQLgGQTlymT7vBLICon1CfzScyFmYpQT2r4gYbbPcrX9eQs/BncruaJtFYhYSyKMAOjIQBiEoRy7T5/0DWQHR9of+6FLuwrBx7FsRcfdkCTXauzB2cWHs4vjVz3prqpXduydLJGYhgTwC4A8ZCIMQlCPn9lbFxrMWZnR0tKurSysME8PesIjRj/YujF1MLt9hJhxPbp3pralWdkc/2isxCwnkawB+n4FwFYJy9Nv2VsXEshNmZGSkpKRk9+7dWmGCUexZEnGndu/C2EVmwsFMOJ7c/loQRureqd0rMdUgDwOi5QoqAwAAcPgK9lyBAAAAQFknpeAAjRNrRomWTghs6K7gHxdzimn1U5GHAewzTAgAAHJClV+KyqJ+bSXjqEzMQB4WylaEBKNZCCOoUl1drSsMHcFuVsTwe3vGr3725NaZJ7e/vtfXVndgv7I7/N4eiakGaQcA2NCvLHYLL+kY6Wax+yEqBfCCSCCaHwpkqtkG7FfSw1MeqtmWSsVSzceEhpBWSgVKOyl1uDCkmxBfOCYlVMeqc2asX68S1atThhDNDzEd2UgYSRW73a4rjD+CZ0J4JoRrK3b0HNzZW1MtoO7AfgGSp+fgztqKHQJZDdIOiKb7xu0BCNRW2kHJ4QMQ2NAvBt1fOgjpDf6tlizOAr9VeowSpsdSTTZgH8iift1K7qNScV4ljWqyEU33sT+zMEpVjITxLePpoIhrbz6nvO3rDuxXdq+9+ZzEVIOsAkTDmHF7AAIbuqMfqxmVumPIAoClg5oW2/C8lizOAs9nkzA9lmqwCaVuVL9uJVJDFSLm9C1nFKarq2u3wjo7O7XCeMN4ihZx+fVnM1z+l19/VmKqQVYBot6VoU1WAVDVL/LPHoVnaTzlQhadUareCoAV3abxVD8UGzS+3U4AAM/SaWSyvp2aoskqkF1CmqwCwNJOiWXL+TeqX7cSF7LIJUkhVL2VqHdhbzjnj8vzYfyYFnHpyK4Ml/+lI7skphrkIUDUuTK2XciSOggO9Use7ShR1w6BFd2i8WOaqrOKXstRaAHwGzqNLHjIQ0JDmbBfOjyVCQWyfPl/k239epW4kEUuQAqh6qxEnQvP5y7MUxY/WhTxnX2X8rbvralWdr+z75KYW4hLEFgRuQ0TbSWesjkLM8fiSZ8IZ3tpn/0ZCT0Hdyq7zvZSiWkqqBNWeFpsk68CsK+V2pqJtg9zuQvjZvGETx+1FTuMhkyGE+1LnS/7WqltmnQr4c5RmFM35qb8a3/58J8LRZiGv3x4yr926sbc5oUZHg8OPWCowLrBk2MRmwEVWB96wNx8mMMPZUl+/caDQPfQbN5/jv0/oXto9saDQJJf37wwRdseKwpToFYUpkAtb8Jc796ni3zVU2iWN2F+7LIkV9fS8Oj623nThkIEIBCVn8m1ljdhBj8p1QozPdL49wbakDBt+ShEmLKe/2NhnE5nNBrNkjzQWZZIrgm452H2vHH6XfTznOuL6ZHGR9ffuvxxmUHclglTYGamMA6HY2BgIBQKZUP+od0aS/CxBB/neEmYGMfHOT7G8Zc6rAZxRWH+uzkcjkAgcO7cOa/XuyH5QpstEucjcX4lLgsTifGRGB+J831tNoO4TMJQSP7FkJT5qW/zFSxIklDhkt1YN5HqmJM7evOZZSYLgzEOhUIIoenp6czk8y22cIQPR1bDUVmYcHQ1HOGXovz5lnKDOGNhlCurQ5aGheXUrKQcTiGYCiKhyJQzKfJoaeaZ+cJgjBcXF1tbW1mWzUA+0/RCaGWVWV4NrSQlYULLq8xKkllZPd34gkFchh0jbA7FKJn+jwMEonQFFPMo3JrNkJp3A5ppZr4wDMNks2O+bHiRXkrSS0k6LAtDh5OBcJIOJ3saXjKI0yxrulLCWhGI0hFRP0Oam0IE0GyPVDL5bWBAM8tMFiYYDGZ5x5w6+ZKP5e55mHf6nO/0OQVhfEzSx3KLDPf5SSNhhAVRXSHS6iDFFZE6gJTU1EmV/p5XbwWFnKq5KEQQEEoXmhHNJMvbp7KuEy/PB7l7Hqby/X5BFW+Qmw9y8yFuPpj89MTLGWKVR5RyS8h+aZV0zpqNhFHEEBASabn0jjE1zRzL23PMxx9UeGjOE+CGJ73Dk15PgPMEuLkA56E5TyDR8WGFiYVlZVtwHOVieXvyb62tcPsTs37O7U+4xb8Jtz8x60/M+hNttdstTIE9+OdPmJaaVxqOVzbWVDbWVDYo/jYer2w8XtlS88r2lSKegAW0XXDxa/+CtaIwBWr/AgPz68EfJ9+cAAAAAElFTkSuQmCC)

　　User类具体代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.domain;
 2 
 3 import java.io.Serializable;
 4 import java.util.Date;
 5 /**
 6  * @author gacl
 7  * 用户实体类
 8  */
 9 public class User implements Serializable {
10 
11     private static final long serialVersionUID = -4313782718477229465L;
12     
13     // 用户ID
14     private String id;
15     // 用户名
16     private String userName;
17     // 用户密码
18     private String userPwd;
19     // 用户邮箱
20     private String email;
21     // 用户生日
22     private Date birthday;
23 
24     public String getId() {
25         return id;
26     }
27 
28     public void setId(String id) {
29         this.id = id;
30     }
31 
32     public String getUserName() {
33         return userName;
34     }
35 
36     public void setUserName(String userName) {
37         this.userName = userName;
38     }
39 
40     public String getUserPwd() {
41         return userPwd;
42     }
43 
44     public void setUserPwd(String userPwd) {
45         this.userPwd = userPwd;
46     }
47 
48     public String getEmail() {
49         return email;
50     }
51 
52     public void setEmail(String email) {
53         this.email = email;
54     }
55 
56     public Date getBirthday() {
57         return birthday;
58     }
59 
60     public void setBirthday(Date birthday) {
61         this.birthday = birthday;
62     }
63 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 3.2、开发数据访问层(dao、dao.impl)

　　在**me.gacl.dao**包下创建一个IUserDao接口类，对于开发接口类，我习惯以字母I作类的前缀，这样一眼就看出当前这个类是一个接口，这也算是一种良好的开发习惯吧，通过看类名就可以方便区分出是接口还是具体的实现类。

　　![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIwAAAArCAIAAAAlu/c/AAAH+UlEQVR4nO2a/08b5x3Hn/+iP0ZL1SgKmhQbmy5XivIf5KcUnIY2W6XT2qkKbGsjAikFXOpRLXs0uqZpUkLaBEiCOkSSIxGjFGksJmmcQbMFcQLjYoy/3NmHv5456LMf7vzc4/MXPHw4ceqP3kL3fL49j5+XfX5sA1DFnnkDT3sBFdveKpDKwCqQysD0gdRc+4IufXbZGBrQTP44BdmSLadg2yGkyU7D1OlfYvUe3UMOJzsN+q5SJ/uZQZo6VcVO9q3OXF6duey48UlfUwM5nDpVpe8qdbLnCNLk5KTNZstfNvnegdWZy8n1cX524Mmdc31NDeRw8r0Du7PaIu15gTQxMVFVVbVv3778ZePNB1ZnLvOzA/zswJO7n8uQ8HC8ORckhgYUZCAFAACAZhBiaAAAAOTmpFwg647hKAUhsaes0lNpm71VVkhqTtqU2zUE+XnrammQZEINDQ3bQhp7d7/jxidP7px7cvfze/2dLUcOk8Oxd/fnqGNovBHy45UfKQsp5YrRbHz6TjC0ujcspPCespBOFakpDK3FnAmJbEgWFNAwrXR3TYWECVkslvyQmmtf6D26p6+pQVbLkcOysKf36J4c5z2SQbbr9Ke19sWkuR2lD7VP/Sz3rgxIRTXMfA7tlimQSELbQkIIffPWS+RJoeXIYXL4zVsv5agrAFKed4Vce5r+oqKKh1RQQxZSJXoHUyDZbLZ9hHV3d+cvu3bixTwHh2snXsxRtx0k7f2HZpC8TZlRYiOJTiykyLtTKpmB5HtSEQ3TbnelOmXs8Ah+9fjePAeHq8f35qjbFlK2d2x1T8koeXAgvDRNpW870SkTUvrZYtuGz8LBoXD7yrKXPCn0NTWQw68suSDpas/qiVl32yGk6S5jv+UXWL1H95DD6S6jvqtMGQupbGex593K7bs74mb4MyGEKt+Cl4VVIJWBVSCVge06pM2tnybm+E/HXNbhxYoK1Kdjrok5fnPrpxJBuvW9/7aDcwelsIgqKlDuoHTbwd363l8iSD0jS3xsi48jLlZRoeLjiI9t9YwslQiSdXhxXUSBqKLRnoNZhRMqkrUuIuvwYukgCQnkiyj6u82Q3NjU6PHo26M9B3FO2WsWmgDVNVtUEyFRWkihBPKGFQ1/bMyEtDDR9p/Rt0d7DuI0jew25SNs4zDyhplGQHU+IhIeQVMNtOeoLUBsZw358wg9sPNWqfVoVvj/K6QvpOnp6Wg0mgdSMI48YUVD3dWJ5Kasey5+/28++x38x7L9LwsTbY9Hf3vto2qcqdGVXwOTjfWEkSfMHAdUxyMi+giaauB0jsICxHbUgOPDqeEwDQAxfEoKxnWFNDAwMDQ0xHFcLkh8DLkFRVe6TLGEFEtIcVHCkGKiFBelmChdtZpwpkZfnwDVH7NuAbkF5nVAfeggog5YXQP/maOwALEf1oDXb6Q3BPTXO2+og/iY3pD8fv+FCxfcbndWSIEocoUUXeo0R+JSJC6F4yqkSEyKxKRIXOrvNONMjfpPgOpu1hVCrhBzDFDtDiLqgNVmOBVCrhCa6lZ/6OhXEphjKVeqA9tuBseuM8cAAGY4pQzJ6QiPA1anyokctScxEbEe7NSWs+1mvAw01U0BeeUZswSiekNCCHEcByFcWFjIhOSLIGdQ0cV2sxCRhMiGEFUhCdENISKFotLF9hqcqdGlN4Gxm3UGkTPIWAD1wUMi+hAazfA7+QLQl9IKyWT2AzOwXFcugJqJ/VhE5pvwO9l5nU6VMBaAF0P6ifUonmzl12kgrzbvLL7ILkBCCK2trXV0dASDQQ0kbwQtcorOnXmZC2/w6xtcOIkhcesbfDjJhzc+a3sZZ2r05RvAaGUXObTIMRZAnblPRO9Doxl+q4QAIKND2n+aMFrZRY49YwaWIdxBM5Q9apNvreqr80u5pzJdjvL70ChnZi3H68+b5t0NSDzP53oledbRQkDRX1t/5QslfaGkT1Ah+YSkX0j6hGRv6yGcqdHFN4DByi4E0EKAbTWD+iEiOkQDMxxXk9lWMwCAap3JDKFsHXI3nIEGkJp3BhoAfTH7dFTrDFGOM7OWpx7LuJUiQto0z7rekAKBQJ73JLeA5n2Kzp465AmK91z8O/3T7/RPy5A8fNITFNd48c+nDuFMjb5oBIYuVr6+20UBQH+hhJh6kArZ4elB2cmeNoH6QSVaP4ibyFU4qklG8z40P0gDQJ22p65N8K52UmJGMscODXKhHRrkzOzlaN4ODSa63pRvFrdQ2tPdioD+61Nke/+VlYB4z8XX/X5QJuQOiCsBcYUTVwLJP73/Cs4kdadLuRW8Nqh4zjeqdzBDF4szVX8jozjt0JDypcrZFpPaSh6qZoJ3fFlChkbaAOjzip95jTg4nFcnolrs8oXszFXOtpiIFWZLW9EX0rafk34Mosdrij76Q63LJ7r84tice2zO7fKLLr+47BddPtHlT1j/WIszy1j/ggcBfa64Jj8GS/uNw3IQzXkUdTTXOr2JJa/o9Cacyt+E05tY8iaWvInO5lqcWb661UEBE7xVXJPlEkNyBtGsR1F706utJ+vamuramupaib9tJ+vaTta1N72KM8tSV+TDJP23ols5Swnp7M3lee/mDx7079WKCtUPHjTv3Tx7c7lEkMYcgZEHPOvfyvUptaJMsf6tkQf87Yel+tEvKW3dfODvGVl66r9Jl5F6RpZuPvAnpa0SQapY8VaBVAZWgVQG9j/uWJ4Wu6WWYgAAAABJRU5ErkJggg==)

　　IUserDao接口的具体代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.dao;
 2 
 3 import me.gacl.domain.User;
 4 
 5 public interface IUserDao {
 6 
 7     /**
 8      * 根据用户名和密码来查找用户
 9      * @param userName
10      * @param userPwd
11      * @return 查到到的用户
12      */
13     User find(String userName, String userPwd);
14 
15     /**
16      * 添加用户
17      * @param user
18      */
19     void add(User user);
20 
21     /**根据用户名来查找用户
22      * @param userName
23      * @return 查到到的用户
24      */
25     User find(String userName);
26 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 　　对于接口中的方法定义，这个只能是根据具体的业务来分析需要定义哪些方法了，但是无论是多么复杂的业务，都离不开基本的CRUD(增删改查)操作，Dao层是直接和数据库交互的，所以Dao层的接口一般都会有增删改查这四种操作的相关方法。

　　在**me.gacl.dao.impl**包下创建一个UserDaoImpl类

　　![img](data:image/png;base64,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)

　　UserDaoImpl类是IUserDao接口的具体实现类，对于**接口的实现类命名方式**，我**习惯以"接口名(去除前缀I)+impl"形式或者"接口名+impl"形式来命名：IUserDao(接口)→****UserDaoImpl(实现类)或者IUserDao(接口)→IUserDaoImpl(实现类)**，这也算是一些个人的编程习惯吧，平时看到的代码大多数都是以这两种形式中的一种来来命名接口的具体实现类的，反正就是要能够一眼看出接口对应的实现类是哪一个就可以了。

　　UserDaoImpl类的具体代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.dao.impl;
 2 
 3 import java.text.SimpleDateFormat;
 4 import org.dom4j.Document;
 5 import org.dom4j.Element;
 6 import me.gacl.dao.IUserDao;
 7 import me.gacl.domain.User;
 8 import me.gacl.util.XmlUtils;
 9 
10 /**
11  * IUserDao接口的实现类
12  * @author gacl
13  */
14 public class UserDaoImpl implements IUserDao {
15 
16     @Override
17     public User find(String userName, String userPwd) {
18         try{
19             Document document = XmlUtils.getDocument();
20             //使用XPath表达式来操作XML节点
21             Element e = (Element) document.selectSingleNode("//user[@userName='"+userName+"' and @userPwd='"+userPwd+"']");
22             if(e==null){
23                 return null;
24             }
25             User user = new User();
26             user.setId(e.attributeValue("id"));
27             user.setEmail(e.attributeValue("email"));
28             user.setUserPwd(e.attributeValue("userPwd"));
29             user.setUserName(e.attributeValue("userName"));
30             String birth = e.attributeValue("birthday");
31             SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
32             user.setBirthday(sdf.parse(birth));
33             
34             return user;
35         
36         }catch (Exception e) {
37             throw new RuntimeException(e);
38         }
39     }
40 
41     @SuppressWarnings("deprecation")
42     @Override
43     public void add(User user) {
44         try{
45             Document document = XmlUtils.getDocument();
46             Element root = document.getRootElement();
47             Element user_node = root.addElement("user");  //创建user结点，并挂到root
48             user_node.setAttributeValue("id", user.getId());
49             user_node.setAttributeValue("userName", user.getUserName());
50             user_node.setAttributeValue("userPwd", user.getUserPwd());
51             user_node.setAttributeValue("email", user.getEmail());
52             
53             SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd");
54             user_node.setAttributeValue("birthday", sdf.format(user.getBirthday()));
55         
56             XmlUtils.write2Xml(document);
57             
58         }catch (Exception e) {
59             throw new RuntimeException(e);
60         }
61     }
62 
63     @Override
64     public User find(String userName) {
65         try{
66             Document document = XmlUtils.getDocument();
67             Element e = (Element) document.selectSingleNode("//user[@userName='"+userName+"']");
68             if(e==null){
69                 return null;
70             }
71             User user = new User();
72             user.setId(e.attributeValue("id"));
73             user.setEmail(e.attributeValue("email"));
74             user.setUserPwd(e.attributeValue("userPwd"));
75             user.setUserName(e.attributeValue("userName"));
76             String birth = e.attributeValue("birthday");
77             SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
78             user.setBirthday(sdf.parse(birth));
79             
80             return user;
81         
82         }catch (Exception e) {
83             throw new RuntimeException(e);
84         }
85     }
86 
87 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 3.3、开发service层(service层对web层提供所有的业务服务)

 　　在**me.gacl.service**包中创建IUserService接口类

　　![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAKIAAAApCAIAAAA552VXAAAIR0lEQVR4nO1a/U8b5x1//ov8GC1VoyhoUwz2ul4pyn+QX5aAWVirVXq0daoC25qIl4AAx2NUy06jStqEUtI14FDUoaQ5EjHK+IECTUMHzRLECYyLMT7bdxj8duagz364891zb46HbewGf/URel4+35fnPnfPPTYGKDtbn+7/6srPcKxP92cZs2Q5N1DoAkp2EFaS+VDY/mXe3fthfIF9f9TjGF4uIRu8P+oZX2B3937Ioa4a27/MX3wTuD8X8nLCNo9KyAZeTrg/F/rim0AOddXY/mXuHllhY3tsHIViJWQFNo7Y2F73yEoOddXY/mV2DC9v8SgYLSEH2OKRY3g5h7pqLCuZwwnERIzRUHnEbKqYQNUBOJiPyPOkFRCd85nyw4kilnkzgfzbEiY6LJNNP5XRc/Yo3p3osMjMYgJVB+BAPiJ/S1oB0fFtpvzNYpaZiyPftoTJS2X0RN/67K312Vtzn73XV1+DdycvlcnMYgJ1HsBPC18G4uJFIPPExERXV5dm0DG8zMaQNyxh4t2T67O3kltj7PzAswfX++pr8O7EuydlZjGB+hWA/yh8GYiNFVrm8fHxsrKy48ePa8Ydw8vBKPJsShhrOLk+e4udH2DnB549/ECUWe6ONZyUmWpQtYBoGyIrAAAA1A4hzxAEAAAAKpw0xgG6QaSbJdqcENjISXF8ToophTUORdUC2K8OOOkkUhR5Sl8A3WYDtUNULQDABmttSmGTTgLYyMk5siKdu3YkGC2ozKLGNTU1hjIzEeTmJIy+c2Lus/eePbj+7OEH0/0djWdO493Rd07ITDUoOwDARv6bQ25R4DcoN4fcj8lyAD+WCETrY5FMt9qAfUjrnhqhW22pUBzd+obYEMPKoUC5k1a7i1MpKHlxjr4AutUGgMwcgkpekaCqX59UG5CJFE5mWWO73W4osz+ClkNoOYQaKo/0nD3aV18jovHMaRHySM/Zow2VR0SyGpQdEJe/Nm+7IFBbuYNW3F0Q2MgvTbpfOpTn8iM9WcoCP9KOAKCUYVYAfdkG7C7dKr4my8WAckOf1Cigv1Ay4xqbyezbQktBCZ+/9TJ+5mo8cxrvfv7WyzJTDaoaEM2z5m0XBDZyzNhXNyt3Z0kLABYHvSS1Ya+eLGWBvQaR6WYbAOkKoJttoNqljPT+Glgc9JiDeH5So4C+rQLJ3NXVdRwzp9Opl9kbRouMhDtvvpTmCHbnzZdkphpUNSCaZtK0qWoAqgcl/o06eINBizOkxWCWbrICYCUfMmhxEEoNBj3sJACANxgNmWrqpBcZqhqoA86QTVi06kGTApTZFGZIixVWW1P1z5CWdEm1Ab3hQh/BzMwxvLwWRk8ZCbfPH0tzBLt9/pjMVIM6B4jGmbTtGdKS2t/ODcoj+lmisRMCK/mAQU8ZutEqjVrqoAXADxkNWRyhzokNLOCHdalkddRTrVeqAIZutMptZUTtYpbUIOBaMcv8PYeebEj4xH4MP3P11dfg3U/sx2RmHnEbAitJHUCiXON7rohlXuXQgk/CVGd5v/0nMnrOHsW7U53lMjOnoC9a4TWpTf0SgFPtdH4S5RerxSyzm0PzPmM0VB4xm8oxpshTqQ3wVDt9QElzDXfRynz13uqif/c7H/rPeglZ4TsfWvTvXr23mkNdNbZ/mUfngiOPWDqwZ/K9RwmZgg7sjTxi7z8uyp8VJIW9e48C3SMrBf+RzY8d3SMr9x4FksJeDnXVWOknf4fCSjIfCivJfCis8DLf7T5liELX9UJZ4WX+Z5clubOrwZO7v3tBlKZJAhAkfYCORlZ4mYf/XK6XeWm85b9plaZJ6Z+MkEIIUVBzRWiSyPYSUfL/CyGVRZgXWOapqaloNJoh2eWsSCR3RUx72BO/ufZ78l+rM39bGm95cve3d65UmDli2uZcZgoq6tIkkZXQxWB5kXlgYMDlcoVCoUzIn3ZaYwkhlhDivCDLHOOFOC/EeOG2w2rmmEeZc7AZFJflS+ZAIHDz5k2v1/tc8scdtkhciMSF7bgicyQmRGJCJC70d9jMHDOUWd7esd1X2ZBxFqQoKA3hT7MmZxpHCLF9Vtp0xXlTd/0ItgR8Q1GtQbWhKx2jpYqWL5kRQqFQiCTJpaWl9OTeNls4IoQjO+GoInM4uhOOCJtRobft52aOGcmsuso6P2VavEIYU7r+uG8GjgolRVAKoKBWS8OA2BKkAZqEKZZ8++kTGdJSlkeZEUIbGxvt7e0cx6UhX7/8Smh7h93aCW0nZZlDWzvsdpLd3rnW8oqZY2ZPsygXNktpf4pFkLTh7YDkx0OR6bmOqUpUt4HxMdEkIJ5bCax7UHWJjGmS5VFmlmUzeZr/3vwLZjPJbCaZsCIzE04GwkkmnOxpftXMEbtuOpG011RcPkHSRpfbMIIqFoBUpo4iS7nN0suc5gSg3i+00YwTGdBEy5fMwWAww3fz1Uuv+jh+2sO+3T/1dv+UKLOPTfo4foPl/3opE5nFNapegfKCSez9lrreODW1MWoeCqUj58nAURwgICT0dyDuTpHSHacLKL9pje4P1TI1icxoCKFiOGl3XXxtLchPe9iqPwyKGnuD/FqQXwvxa8HkXy6+Zuil/tyMkHoLxB8Sg8+/BlubVi2Mkn5P1G8D6otsfJIyO1sZyYyRCAgJzTKMilPTECqGz81X/ljpYXhPgB9d8I4ueD0B3hPgVwO8h+E9gYTjT5X5qPBHYGleI/+/Ff5bsPaGSrc/seLn3f6EW/qbcPsTK/7Eij/R0XBIZc7pl2BFIHNb/evNF6pa6qta6quasb8tF6paLlS11b9e6AIP3Aw+y2VrhZe5ZAdg/wOXJekSeBTjBQAAAABJRU5ErkJggg==)

　　IUserService接口的具体代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.service;
 2 
 3 import me.gacl.domain.User;
 4 import me.gacl.exception.UserExistException;
 5 
 6 public interface IUserService {
 7 
 8     /**
 9      * 提供注册服务
10      * @param user
11      * @throws UserExistException
12      */
13     void registerUser(User user) throws UserExistException;
14 
15     /**
16      * 提供登录服务
17      * @param userName
18      * @param userPwd
19      * @return
20      */
21     User loginUser(String userName, String userPwd);
22 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在**me.gacl.service.impl**包中创建UserServiceImpl类

　　![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAALUAAAApCAIAAAD8lf/GAAAJNUlEQVR4nO1a608bVxY//0U/RpuqUZRopRjsdDOlKP9BvmwCpjjJbqXRtqsqsLtNxCNleZSyVJu92nSbNkkT0iZAKOoikgyNspRFKhtIGmeh2QQxCuDiGL/GNhjjMYbe/TDjmTtPIH5A2jn6Cd177nndMz9fXxsDtsQSY4GtLsCSbS0WPywxk5zxo7rkpVyFyqcwNNBMPgKziAIKsQV0lCVvm8qGH0NNtuHaX0o4e3gHOR1qsuWwytyJxY/NyfPzY/jUXnbo0rOxK8/Grri//PBSVTk5HT61N4dV5k7y2Mqtk4LzY2hoqK2tzdxz6N09z8aupBbucOOdT74+d6mqnJwOvbsnD9VmLxY/Nic6/BgcHNy7d++uXbvMPe9U73k2doUb7+TGO5/c/kTghzS9U23ED4YGCjGIAgAAmsGYoQEAgDxkMyrQPXmlVQohWrZgxZhiWP1QOq3U89MWwCIKaIahAYCiaUqZlkKssG7obrwj2XHdzhA1KMotID8EcpSXl6/Lj4F3dru//PDJ1+ee3P7kbkdTzaGD5HTgnd0GfkKLEZsZiltTtYlovnLrDC33hUWU1DwW0Rkn2YSh1c9D00ptBv0CBBYRT0TKKxgo6tcmNd6R0tG0M5oaZMeC8EMih9PpNOdHdclLZw/vuFRVLqDm0EEBkubs4R0Gn2jIZumN5ZeH3guO9NBM1SeBylh00LLB6PAiC1A910xoBX8MkprvSP+FoTtW1UCmzD8/SHKsyw+M8VdvvkJeSGsOHSSnX735ioHfBvhhcp034ofyKDF8VIatFKhlUoD6dS9YsYhaP6n5jl4UfrS1te0ipLW11dzz+vGXTe6n14+/bOC3bhfIdxDM0NKrU7tKkIKIxCKKfH+RjmNE3j+kgCxCRLRMuzUF6L4vUDRNkVlNkhrvaJP8UG6+sO8vm5JrlTtN7qfXKnca+K3bBb0bI/klgbxK3k8JLU1Tyi4SkTT8II9/qck6V1btPUXxtBTr6qSmO9rs+UFry92W/PjcuZO8kF6qKiennzuN+JFTMT+6f2qic5fOtzw/P0aaizqcv5Bw9vAOcjrSXJTDKglhEaX4GPkzoseLxQ+VFO7/L8RZ/XMiB36x+WHJT1IsflhiJhY/LDGTQvBjde3HwQnuowFPS+9TC9sZHw14Bie41bUfC8qPm98Fb7nD3kh6kccWtjO8kfQtd/jmd8GC8qO9b5pLrHHLOJywsK3BLWMusdbeN11QfrT0Pl3gcWhJRH/7Pl1IBha2EAs8bul9Wmh+xJI4EBfxzzZbamVVhUf9b/W375NsXjCMIztQzeMFzsu4gO7KdRmx5FbwI5rE/kURvR8UafkxNVj/v/63+tv3SWYaMC6gmh4SmofIvh+NGtpvBIwr81WbqzeLOA+RXVWbKUbbqBwkXWRcQHdmUYYuovngx8jIyNLSkgk/IsvYtyiiu7U4mVoVcNfD7f7tx79H/5od/dvUYP2j/t9df79YslSCqQSq8SGheYjs+9GIvvFGwFQCVPYKY7ZxP331+UNtGld/A/Y2NrsgTCXkvubIch740dnZ2d3dHQ6HjfjBJbA3JuJqsz2RTCeS6WU+LfEjwaeX+XSCT19rsUuWSjBvAPVnN6Fxo+L96Ft94w0gS/fs8MVxKP6AzS4I8wbQX+S6MC6RH34Eg8ELFy54vV5dfoSWsCcq4nKTI76cji+nF5dlfsQT6XgiHV9OdzQ5JEslmAqgGtyExo2KHWg4ij1RPNwq//+8Q7YXpbiV9USxJ8o2OKCih6kAAAcajjIVABU9uolMHOkKh6THw60UONCwGxWb5dVqcMdxcpVq6EHFACDU00MbZpdsREcpaaYhksYtBszYsw3asnXMcGgpP/zAGIfDYYTQ1NSUlh+BOJ6JiLjY4IjF07H4SmxJ5kdsaSUWT0eX0hcb9kuWSjBOoN57QGgeoCIH+rcwAPqyoTH7ngOcPeIASEvxSZC+G3DsoUHIKxnIBTBOgKJWdr1K8OVjkhnjBBADCvUcY2YUm9Jkl3Mpd026HBMqJOy1ZeuZBeJ54wfGeH5+vrGxMRKJqPjhj+OnYRHnTr8aXlzhFlbCiymJH+GFFW4xxS2ufFz/qmSpBOME6vQ9QnMPFTnQN+ISALnarf7JZ1EL+zTMnnaAs1sd+ZsWCgDgKLNhx0wl91AR0J+FiUE3DWJJ5pXgz46KA+W+dMeq7NKUcQpJyYYQGnFfACAqNWXrmfnzxw+O44zOD98CngqJ+HvdrwLRVCCaCsRkfgRiqWAsFYilztYdkCyVYOscUNZNaLppcKA7SgMAqm5Mu2QQQQZTBlDWvVHHi0fB1sLeaaFsLexUCE+NIRvQF3VKMgooRsikpurGsPFYlV2aMmVCUglSGWPIBqCuTb9stZlvIT/8CIVCJvcPbwxPBkScOXXAF+Hveri3O0be7hgR+OHjUr4IP8/xfz11QLJU4XYzBUCfF6dMGYCtmZ0M4MlRVNslKNlaO5R1iatlohKfdwle0ioWvGwuZlKORtWObsxR8LXTZXbBBU+OIptYGOnO1DazBgHxeVemeDm10ZittQNkSiWawJQJg1FkEyylMrposKPb2qapytYz88a24vPLXAw/DohoO/naXIi/6+FK/9AlkMMb4udC/FyYnwul/nLyNclSi09d8kFta2Z19C5GVI4iW0Z3pEswY2vs0hg/DuCvm6WjldBvwFHQKHPRn6rdtRoxiJT3SBd+HGCOAFUzKoTVHbM1djjikn92KoYNMEeE8SiyCZZyGWyNPdMlFy3Xpipbz2wuH/xY9/uPHyL40byI9/9Y4gnwniA/MOEdmPB6grwnyM8GeU+A9wSTLX8qkSwtPJrHj+bZU3Y4fG0Dlv9B+4A+l126HyJb8f3pbARP+EQ0VpfM+JPTfn7Gn5wR/yZn/Mlpf3Lan2yqLpEsLUz48ISPPWmHX19d3/JmIwV2dDO7dLNbwo+ZCB73iWioer3uRGl9VWl9VWkd8bf+RGn9idKGqtclSwvjPjye4YeZzVXxg/o/sk43U3h+nLkxO+lf/d6H//vMwrbG9z486V89c2O2oPwYcIf67nNscM3giy8L2wVscK3vPnfrQWF/H5RKr924H2zvm97y389ZMEd73/SN+8FUeq2g/LDkxRWLH5aYyf8BrI0IjAezL38AAAAASUVORK5CYII=)

　　UserServiceImpl类为IUserService接口的具体实现类，具体代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.service.impl;
 2 
 3 import me.gacl.dao.IUserDao;
 4 import me.gacl.dao.impl.UserDaoImpl;
 5 import me.gacl.domain.User;
 6 import me.gacl.exception.UserExistException;
 7 import me.gacl.service.IUserService;
 8 
 9 public class UserServiceImpl implements IUserService {
10 
11     private IUserDao userDao = new UserDaoImpl();
12     
13     @Override
14     public void registerUser(User user) throws UserExistException {
15         if (userDao.find(user.getUserName())!=null) {
16             //checked exception 
17             //unchecked exception
18             //这里抛编译时异常的原因：是我想上一层程序处理这个异常，以给用户一个友好提示
19             throw new UserExistException("注册的用户名已存在！！！");
20         }
21         userDao.add(user);
22     }
23 
24     @Override
25     public User loginUser(String userName, String userPwd) {
26         return userDao.find(userName, userPwd);
27     }
28 
29 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

### 3.4、开发web层

#### 3.4.1、 开发注册功能

　　　 1、在**me.gacl.web.UI**包下写一个RegisterUIServlet为用户提供注册界面

　　　　![img](data:image/png;base64,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)

　　RegisterUIServlet收到用户请求后，就跳到register.jsp

　　RegisterUIServlet的代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.UI;
 2 
 3 import java.io.IOException;
 4 import javax.servlet.ServletException;
 5 import javax.servlet.http.HttpServlet;
 6 import javax.servlet.http.HttpServletRequest;
 7 import javax.servlet.http.HttpServletResponse;
 8 /**
 9  * @author gacl
10  * 为用户提供注册的用户界面的Servlet
11  * RegisterUIServlet负责为用户输出注册界面
12  * 当用户访问RegisterUIServlet时，就跳转到WEB-INF/pages目录下的register.jsp页面
13  */
14 public class RegisterUIServlet extends HttpServlet {
15 
16     public void doGet(HttpServletRequest request, HttpServletResponse response)
17             throws ServletException, IOException {
18         request.getRequestDispatcher("/WEB-INF/pages/register.jsp").forward(request, response);
19     }
20 
21     public void doPost(HttpServletRequest request, HttpServletResponse response)
22             throws ServletException, IOException {
23         doGet(request, response);
24     }
25 
26 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

​         2、在/WEB-INF/pages/目录下编写用户注册的jsp页面register.jsp

　　　　![img](data:image/png;base64,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)

　　　　凡是位于WEB-INF目录下的jsp页面是无法直接通过URL地址直接访问的，

　　　　![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAdEAAAD6CAIAAABxveqNAAAgAElEQVR4nO2d+VcbV5729X/0rz7xcZ/M9DszfifTme7TM3NmupvpGXoGxx2n8+IkTbZOQtqxA1m8QLwmtrHdxDh2jI0dvAovLGbzBgYvGLMahNgXsQuEQBIIIZF6f6jt3qpbpRJLgfHzOc/xwcWtW/feuvepb32rkCy36gZu1fXfqu2/WdNbXN1bVN1TVNVTVNVT+KSbV1FVT3F1T3GNo7im96Ygh6BqR3F1j6Aah6xqQUXVPbwKq3oKq/ifHUU1vcU1vcU1fcW1/cW1A6SKJNUMFNb0F1b3F1T35Vf13qh05D3uzqnoyn7Umf2g/dr99qvlbVfLW6+Wt14pk9Rypazlyr3mrFK7tdR++a7t4p1GWrZLd22XSpouldgvlzZfKm2+VGq/VGq/VNJ8UdSF0paLpS0XS1svlrZdLG2/WNr+7mf7yqqaCstrIWgFq6yq6c1Pv3nj02/Gp0KuSWgRZSmo7Cx43Hmjoj3vUVveg9acBy055c3Z5fbscvv1cvv1cnt2uT27vDnnfouoZknZYsnscn5LC63m7PLm62I918vt2eXN2fdbsu+35DxozXnQlvOwPfdRR+6jjtyKztyKrryKrryKrtyKrtyKrpxHXTmPOrIftmc/aL92v/VqecuVsmZric16t+Hi7fqLt+rPF9eeK645V1yTWVSbWVx7rrgms6gms6jmh6KaHwqrzxZUncmvPJ33mFdGXuXpG09O36jKyK8+W1hztqiW15mi2ozC2jOFtRmFdWeK6jKK6jOK6s8UNZwpbjx703b2ZtPZW/Y/bd5dWFppzb8HQStYhaWVsZ/sif1kDzx30T1315GMnUdO7zx86qvD6ckpJ5MOfp908ETSgeNJB47v2P/d9v3f7dh/fMeB4zsOHE+itePAd3yB7XQxsfAJ/oft+49v3//d9v3HBR04sePg90kHv09KSU9OSU8+dCr58Knkw6eTD2d8dTgjmdSh00kpp5JS0rcfPLl9/4mt33z35b5jn+89+tmeo5/tTk3Y9deEnUcSdh75dOeRhJ1//XTnXz/deYTXlq8Ob04+vDnp0CdJKZt2pHySdGhT0qFPkg5/knxkc/KRzTtTt+xM3bLr6Oad34o6Kipt8660zbuPbdnz3ad7jn+658Sne7//7fo3U45+/8VXX0PQClbK0e9//cpbv37lLXjuonuuyxeCIAiCzJFl1BeCIAiCzFEYz3V6gp2D41X2nvt17Xcr7TcfNtx82HDncdP9uvYqe0/7gNvpCS55HyAIgp4VWUZ8IaYG3NPVLb3FDxvq2/oGXd7JQHB6ZjY4+2Nw9sfpmdnJQHDY5a1v6yt+2PCk2THgntaqB4IgCJJkGfGGFHJ6gnWtA3cqmxxDbn8gNBP6MRD8cTr4o39G1nTwx0DwR/5X/SOeu5X2utYBpyeorg2CIAiSZHF6Q6Q6hz2PGrpaup3TwdlA8MepmfAKBH/0z4Raup2PGro6hz2KCiEIgiBJFqc3KKlnxHunsql/dHw6ODs1MzsZMKqpmdnp4Gz/qOdetf36nSf55XUQBEGQWpZhT5BXx9BEaXXrsMs7HfzRuNuSmg7+ODox9bS5w+Ob5AAAHMdZLNDzpbAzgjfcvtGpR0+7+pwT08EffYHZOcsfnHW6PS0djkBgxoT5DMByx8AiBCsHI5475AkOeYI1rf1N3UP+4I++wKx3mi3SWzXLTM/6g7Nd/cOO/mETOgjAcgee+1xhyHMngh1DnoqGrjGf36fhpLyZVj1tXvur3218b/OYd1q/5LjHb+9weCenTOgjAMsaeO5zhRHPHZwIPm7q7RwYmwzMevxseadnaxpb/+nffv+L37zyj/8aHffxZ95pzcIe/+zk9Gz/8FinY0DrqFNTUzab7e7du3l5eXl5eXfv3rXZbFNT8Giw4oDnPlcY8dzeUf/NiibfdMijYaPe6dl6e8c//0fMf/xP7Po3P/rv1975rw1v63uu1z/r8werG1pmZoLqQ/b19d24cePhw4c1NTVNTU01NTUdHR05OTl5eXm9vb2LMAwALB3w3OcKI55r6xl92jHgC8xO+BnyTM/W2Tt++dv1/xGzccPbm157Z/Mrb/3laUuXZ5pdXpIvMNvuGBgdG1ccr7e3t7Cw0Ol0er3e2travLy83NzcBw8eXL9+PS8vLzs7ezFs17re8tOkxgWvFjCwWZPTSpxm7riccJakJVtt1CZ4rkkwxn6+zGFOGvHcRw09HX0u7/Ts+JRSHv9sQ0vXL3+7/tfr3vjju5tff2/Lq29vbmzt8fgZhdX79g662nv6yYNNTk7m5OS4XK5QKBQMBj0eT3l5+VWRwsLC+vr67OzsyckFftVsQT3XWZKWLECcYZtV2EacI+MliR2sNr6E+rc2q1iRzZqsnF3U7JAOwapG3boFtbpF8FyiO8mqji8vdD3XWZKWnJaWRnXTZk1W/j+txMnsMzGdNM+ZVMZqE/9DlXSWpAn/p2pTVLcI7mWA+R712fHckuq2/lHPuH/WPUVpwj9ra3P88rfrf/PKm6+//+kbH33+1sdf1jd3MaPa8Snl7uP+2eExb2NLJ3mw+vr6kpKSmZmZmZkZl8t19erVLJqnT59evXq1vr4+sqEJxwJ6LmF3zhJx+cg/Eb83XpKoW1xvaWnK+eMsSZM3qvaVJ4fszMIvmJOGaAfH2axC+YUINRfHc5+VCFjPc/lu0CWUtih1ldFnck/GVVdE3pPh8bTnhq3BTBbzqCZeRIx47s3Hduf4lNIxp2Yb2xy/+q8Nv13/p9gPEuM2bfvTX7b+4e1Nv/rdhr95+der/+FfVv/Dr3it+j+/fO3tj9WWPT41O+rx19payYMVFBRUV1f39vbW19fn5ORcVlFcXNzV1VVQULCw47Cwnkt4Ff8zdUal/xgvqazaZk1OKymhp6CzJC2tpMSaLLujInaWdjU0uWxWVjF47nzR8VwyghWLOEvS0qzWNPL/8lVVz3MNOSZfiLq8GvHclWe5y81z8x80jk8FxyZDpManZqPWvxW1/k9vfPT5O5t38Hoz/osN73yy7s2P1r3xoaTo19//ZdSr41OzihrGJkMTU8HKejt5sEuXLuXn52dnZ19kkZub29HRMTo6eunSJUZLG5N+auFZn5T0U8t6q3IzYavW9RZyo6bnWtdbfppklSu2cuwaiHkr/ugsSSMiWpb/GS/J0b/jtyuzBXzSgRno6EVHbKiFKG1R3GnK97dEvG61SZvJaDqZdaOqSl84S9KSrSUlacnkzUDYG2ZGr8TLUjJxrdFpqtUmt0WuSpn70QokqTOjaivVFLkKp9hLYREqT6+8kd5H+5aD+K2ehyg9l1PM3nCeKzVUcbbpqao9BehwQS7K2ESvBbLnxKllZuhYR6NHT96FnGPkuVFNF3JissaS7IfNmpxWYpM2yjsZ89yGMd/M2CT1SebuqdD/xn7wx/cS3vv0K33Fbdr+n6+9455ifBy6e3JG7bnnCS5fvnz+/Pns7OycnJzz5883Njb6fL7BwcGLFy+q2mldL1uidb3kj41J6wUzbZR82LreorBYPc+VyzYm/VT4WVWDMlZgLG/GeYqgpHK9i15Nr0XSsukZFnlyQGgbVVxh84r2ED5CDYkiT0EYrJUYBzklSfWZtaMSypgJjyWuObpNJfPj8jHJwZRHXLrcUIkcdRadziGp4k+irMXCKc48eYkUPFHVJEafjeRzOaVPyG0Ub7SY+VzWtHJSFkR2mDna1KSXNqsmOHOKKq921Fipxk37aOo7THX9mlWyH6Go70/lVpKNEH824rnFFU39Lq9rkvpU3bHJUH1z98YPv3x7S/KfP9vN650tybEfffna+4mkXon7ZNO2r8cmlZ/L65oMDY75FLmF3NzczMzMzMzM+/fvNzQ0dHV1FRUVVVRUXLlyJTMz02639/X11dbW5ubmKptJu2YjEecaCWqlTcqgWFHWut6y3qqfilA4qni3yHBS4yU5jTUp7qZcoxy1C8OtjcMvbSLsUj5xoRYlMzWicyFRhTl6uZWIcguqTbpNZaV56Ag7OZlyP5uVv9EgL3SKQ7JXuLMkLdlqtSoDH3VXqUuocFjSosLkFmQLVN1LsDxXct1wca5O/kJpoPSwsc4kbetiC/mBpw6tebKYZ0lr3tCXmGRF29lXM40eMOtWhvrk+ZFXphHPvVPV2jkw5lKZ5thk6Glrz1sfb3vv050ffbH3w8/3xH+xr7VniPUALcTc3THkVjxDq6qqOnv27K1bt8bGxsbHx8fHxwcGBoqLi8+ePXv27NnMzEz+5ydPniibqeG5RMQrFdHzXGPV6niuylEoT+Q4reu5XkmtasWfSsj/qiYSPVsiNl1OMVcZ8QMdphr2XDJeYcYLC+W54Zuq4bmMw/HGIjx4FO/8VYNDHUl1ThXPrSwW5rWMqFk+EmU2up6rPVxszxVc12rV9Vyt+Un8jjXaOp6rkQGhPExr9mv0Mrznqg4RNi4J57nSf8U5Tf+G2N2I55Y/7a6x94xNhka8QYVck6H61p43P97+58/2fPTlvo0fffH7//dBQ1uvy8corNDYZMjW5mjv7iMP5vP5MjMzMzIynj59OiZSW1t7+fLljIyMc+fOWa3WzMxMn8+nbGZj0k8ZuQU5G8AXkDMDUtGkcPlculo5O0HVIAWqqtth5QJn3m7pl2SdWsr6kqmlKbfeWZKmesFBeZPEfm+BWt5MzyVujmXzZFsmmRWQD0/0n96ocAGGxysJ47nhm8ryXCpVIKdBbNbktLQ0wgyk/+jkFtRjSfTeYmHe6xL2x1ditVqp82YwztUZLaWRUDc1LJdRGwnzVk092hpnUrEG+KIlyls77dnPsc+SztE0DqE5dGKVqpktzRFh3pYoHJuMKKgajXhuXcdI8cOn7skQ+UG6kly+UH1Lz1ufJH3wxd73P9v9v2/G/+vvX69p6hr1sctLck+F7lfZRlzKv4no6Og4derUpUuXRkdHXS6Xy+VyOp3nz58vKCgoKio6e/ZsR0cHu6VyCoF4hiZt/GlS0noqM0s+FdOPc5PWU/kGVg1KA1El1eSt9M2/oZLK9aUM4piZKHEDY+mRd08acQZVSHUrJ8UzfBUlunEu1SdFVK7cqFzo7B0ZTdXNLYRtKtNzGbflYqspO1e6kO7Zo6+pycnJVs5i0YjV1CEUHb0RMG7V2SdWKsO8YWbeEWiOs7NEeq5EHpA12tpnUu4HY3/W/QPzpkB7ETGfoanXmLhF+XtyKuh4LjWZqTtQ1fwx4rntQ5PnCx4Oj/ud3qD0WbqkRn2h+hbH25u/it/6zYdf7It54+P/evXtEV+IWZiX0xscnQgUl1UyP9Gxvb09IyPj+vXrFRUVDx48uHr16vXr17OzszMyMtrb28O2mFvAd7+Wwx+o6dyBgRWAxfJsnF6mQ8+l5RHtN+/Zv6jLJ6J8F48Rz+1xzZQ97bnzuMnlCw1NBJka8YbqWxzvJOyK33bgrU1Jm5MOjHg1Cw9NBEd9oYq6ZsUfoZF4vd7Hjx9nZWWdOnXq1KlTWVlZjx8/9nq92u2UX1AgMwrzZRl4rgmWq3yWEXHG10SepbYawclZLM+m5UY0M+V3GXSyHoaOuphHi/w4mvNvXp7b7Zqx9Y5fvvm4pXd02BMcnGDL6Q3VNHfHbflq+4Hj/WOTQ9olhzzBjgHXnYdVC/xtEfLtPvke7fxYBp4LVjjP7OctROaHchLBjMvkYh+NSLJpFpiX53a7ZkpqOnNKa0Z9IR0zdXpDLl9Iv8zgRNA9NXv7YW1Lh2NenQZgZfDMei6YC0Y8t2t0pmt0pnVw8uqd2luPm0a8of7x4Jw14g09qm+9/+QpvpsHAI6D5z5nGPfcrtGZRsd4+rV7T5ocTk9oYDzY745MA+NBpydU0zKQe/sBvoMSAAF47nOFEc/tHJ2RZOvzHLt069bjpoHxQP94sNdtVP3jwcGJ4O1Ke+q5ovRr93648QiCoB9uPOIsliVvA2SajJxuS+dogFRt12hG7v1z+Q+6nZ5hT7BvfKbXrae+8ZlhT9Ax4j2X/yAj935t56iiQgh6nrX0X/0NmauwU8LSMRJQqN05nVvWuOt4VnZprWN0ctgTHBgP9rlnesdmHGMzjrGZ3rGZPvfMwHhw2BN0jE7duP9013dZuWWN7c5pdW0QBEGQJIbn8mrq914sfrI99eKJy7duPmpq6HS2Dbr7x6f7x6fbBt0Nnc7bj+0nLt/annoxs6Ciqd+75D2BIAha/rK0jwR01Do8XVrXfSbvQcoPN3afuLr1yPmtR87vOn4l5YcbGbn3S+q6W4en9WuAIAiCJFnanQEIgiDIHFnanAEIgiDIHMFzIQiCzBM8F4IgyDzBcyEIgswTPBeCIMg8wXMhCILMk+XrWx3xVxp09PWtjiVv5XzUWber6+HrOuqs27XkjYRM0IqfCQ0/ecGglrypz7MsH1yqreiekvSwa+pB91RZu+9eu6+sw1fa5v3gUu2St3JeKn8t5C8N+e+F/PdC/tLQ1N3Q5J3g5M2grzjouznjLWorf23pG7kMdK+mbd+Rk5u+2NkxEtj0xc5NX+y8U9l0p7KJ/5nfuO/IyXs1bUve1DlqRc+Ehp+80PHC3zlefCmsOl74O9juEsry5wtVFd1TJW2+u22+u22+2y3emy3eomZPftNEbuN4vn3izxeqlryV81Fz6YaQvzQ4WRj0FQR9+TPevBlPzszE9cD4lWn35YD7SnPphiVv5HLQ1t0p9+va+twzHSOBPvdMn1v4kE/+Z37j/bq2rbtTlrypc9PKngkNP3nB8Tf/5PjZy7MTHsfPXubtleM4pu3Cc5dQlvcyH1d0TwmG2+q72eIttHtu2CZyG8evP3Xn2sbfy3ysuX/5tlXRF5RbLOtS6WIFCWtWJdTyP1iUrIkv54tdiFL9ji6gOIolKt1QDxtv/yHkv8cvs6CwzK4FxrOm3Zf9Y+enxy413v6D0fFKX2dR9NcZaEtfF5XOt5/quNDZ1dsKVPWkRlv4AdGRZpnybasUdaav06mtIGGNwYHa9MXOrtEZ/T8V7xqd2fTFTr3xYfT3QpT2yZKnhHpgVUNKVijAH049JosyE2rjV1sslnWpyiktT1HWDJfg+6I1z8kygTbnhShFj8L1seEnL/CGy3Ecb7v+BxXqD291f3M4Es+VWrsmvjzQlr5ObqGxMTcy1XWmrt5QRV9Y2PakRlOzVH/hGF9WalnePXO/onvqTqvvdquvwDb+8dHsn7+d/E9vJ3/w7fXLNSPXnrrfPXNff1wUfShIWKNeP8yNbc7a+NWE57LtSem57DPBqFxQbdH6kP9e0Hcj6M2b8WQH+GU2dmnadd4/ctY/eq62aL3R8WJ6rrM2PnpbKj8O6euoVkk9UmxXwTp/F6KY1xv1vAnnuQbHig9mw6rPPWPgKOtS5S4TPqLquO6C1PZcYWD5H3grtKgOt6AzwdgZpJYDFZHUxq8WPVfTGqQyrGIGPHfgd684fvZysNvh+NnLs14vx3ETx06S4S1vu2zPLd+2itEt1WCKQVVqNKM0YwloDBTr6IyVThTWGLcFaU/5tlXRFwTPZZXnm8E8hICqbTrmbok7da+ie+pWi7e42Rv/7bV/Szi2J7tm1/Waf9ly7O3DV7LqXO9klD/t82kvjNr46G0F7BNmsURfYMTC1CSLKM69EKVepeS1jqWq/HWhqdIZb66wzNzW6bGL065z/pEzU8PpU84zNYWvtAy4DLRQ4W7yJChIWCMMOl+At2ZykRgIRfXOqDiYijLCjCRniercRxTnGvFcjThX9osC6fKjfSFsY12tVTLkualkpKl70HnNBGM3E8Y8d11UtCo8T19nid7G8lxxBhrwXMlSHS++NPzm+/6KJ7PEd2k7XnzJ//Cxpuc6A228rfBtlm5YNdxwVcIFamkQ511noBSxpP6JFkaVaZr0UMy3PeQhxFM27zi3VtEeSZY/nbxb0T1V3OwtaJqI2vr913l1xc0TN2zur67V/PvnJ3be7tl4rPD9jLKrVQ7dA2jPS3HmKUKhVQm1kcW50rmXj1Ibv1ovwuVVkRsTmiqd8WQHxq/yy8w/mjnlPDU5eGxy4JjLvrmm8JXH+a832orZNRBznVxRxOy5ECWdKi3PlVobbva0GTKjcMOuc23XHq75ea44UJLRLKrnyqO3Lmr1ulRpi64rzWsmpK9blVDLvi5GXyA9V3PkLUScm75uVUJtQcKaqARhuFKjLVHpgi+TR5GtxIDn9v8qaurmXY7jhtZv5J13svg2x3H+sgf8f6erazU910A8KMRu5dtWRV9QL+eI5224mSB7rnIiCV6xUO2Rgh693MIc4neNJWB58/jNB52T+U0T1xrGX/nmysffF594NPxt+cA7xwp+v9e6Ka9947HCm83jcd/fVu8sTA6p3nCeKzdLKMl7roYZkaguaxaLhZ3qVen+tf8JTt4JjF+Zdp2bcqb7Bo56ew9OdO+d6NrpcRwcrn2jpvAVvzu3/LrufaXYBSmLR5gv34V1qU7ac5WWwbrusRaS3iVUKq8fRGv5nd49x3xyC4wzGBWtTrPopjI1EsGsMrJtyf0SSujd8cx/JjDCNHpIjecWUqMtRGp4m5gk0Y1zxWFgWknDT15wvPiS42cvcxzny77hOXNencwN1DfqeC49pdkdF27pxMQ9OSBC37VueTVPENlr1kLQjXMXoj218dHyvuzLqng46aypJqpWcMDYbtl4rPBeuzfjsTO9YjguLf/nH+5/dc+59bsz/+8H33yaWcZ77vnq0Y3HCjVdgPBcRkPDe67CaHQSXtLMEyarEdsttUYHPMW+gW+9fUe8vQc9PV9PdO8Z7/zK3b59onsfv9Imh06UWqN1KuFnoXDnxXAuMfcaxnP1Tic5v1VI3RQmKGswqfUzB8/NvVtpxHNz71ZqDxRx7sQ2qBcwtST0zp3WVCZ7fSGKGkPinmMRZoJWnEt2Sj4v+vlc+Xljbfxq6bZvXrkFx4sv9b387xzHeS9ddbz4ki8nP9DUzHHcdO3TyYKbjhdfmq6u0/JcI6kt3Zm8JiqaESuEvwfX7lfYOHdh25MabYmKVhyLdT2QHuCL607jYZXG7s6AJfbbvOKWiZOPhl/fd/6fPz70hwNXfvFxyi/iUz46fTf98SDvuUfvD8Z+m6c5ZEQGhJHuNOy5hA2ti1I9FaFeA2Bdu7SG8vbF/54ey/b2HfI49k9075vo2jXekeRu2zrW+vlE505+pXkc+25f/G+9mSGs5zWrGDma2vjVllWr18inQZgltfGryVZp5ndUtSmKUdZD3oSq7mSJtGb4laPUfHIL7Gdoi+W5QpwrzAet55aLOBP0boc1R57MLTgvRFnWxQsrQrpOLIDnju38huO4sZ3f+HLyeXv1Zd/gn54FGmw6nitJI+XK7rgirlxYzxVmTrg4d0HaI92ys8/g6m0F4snVr58We8lb/ngkO6fR/equs//wVtJ73xX8UDV8rmb4bNXw0fv9h8r6Jc/945FszSGLviBluxjPGY3lc5VxrmTlytCMv9+keiIn/lkqPvc7/+hFT883UlDjbvtyrCXR1bxlvH2HtNKKz/1Of5mRySP1HJWuyeTjI3pCaKVQ6LPCeCRIWU9BwppV0esYlzFtdzOoeeVzhUcuF6KEBKvcd7FVEWeQwnquMLZUHKR3kzTPmWDksTW1BLTj3Hjhobbwc1Q6naESi6UmrLFEbzPuub3/+C9BRx/HccMb33XvP0K+nMtxnDfr+vTjKn3P1bpmSJ0SOyhnSKRQVJqNegOlXqea/SIMq7yWyKfVFtBrf/7tkbIKUen0cRmBqvQY30gIpRHnvnbw6qnyzl9+uO/PxwtOVQzuu+PYfcfx1a2eHTe7vynplTz3tYNX2fXyIad46U4lt9OmybosKD2XGJ118Xyeiw7xLPSzVL68/tWm4If/nBo+IwQ17dvGWj8fa0lw2T8ZbfrY3fqltNIKfvhPnUpSo+kUEutlONJ5haWSTs4n9kmijVK6zdSblOLCkAIlZVVyJku6jSUf8Wn0cX75XP7QYhC3elu82JjFi3Mj9dx5zgRGrlB5gug1puW5Fot8WZV6Ub5tlXi5aiPCrojiXN5wOY7r/9ffOf7259LrCtIPAZtdx3OF9VWumLSKrILGW8bSu1asddEm5kwjO8vCdvF+UXhIeyGK4YkL0B6ivDSfqbdxtJ1b43KiMRstG/ZbL9WMpj/sSX88uPeOY9etnuRbPdtvdn9Z1LXnjoP33L+W9MYevsaex8LkU5mFtBIMvCsm9Ye8ZBFurtpRzC0YeQsqLyPKN3hyvH2Hu+3LsZbPXM1bXE2bRm3xI40fjLUk8ivN1bk7P/N/2TXwx6K7UJCwRhGNFiSsiUoXZ4nguXwSgDyRzHNFTiBWf4k/NFA8tGTnFigLEK3QIi1mzYvzfN9bICcZsW4XyXPjhcsz+SKwxaI1YRZiJug8n5Hdnzz6XN/PpV/tjMBzh159k+O4yeLbjr/9uePFlxx/+3PX1p0cx02c+qH3734hRLsXrCzPpdevnLujz51eG9ghheRxBek6OQS2OZJBKH108fHJgraH1Qx2oEquOz0L0npv4dWvL5194vzu0eCR8oFDZf0HS/v2l/btu+vYfbtn1+0e3nPfPHwtu6pHb3nItcsXHHmkWH+7pb1C+PtTOnXLP7kysK7Uyj71W29/2kTXvonOXeMdye727e62rWMtn7uaE1zNn/IrLS/zlbqGUuZs0H4ULqbhhL6si4om1p5lTXyC4o2xMHEunSEhDVrTJZm5BdLjiBt83dPhDLQ5A9v3Hrpbadc33LuV9u17D2lMcfl6QE5fleeqZohywHURX1Imnw0YjHPnMxPUI6++YCgXLT/U0ryl/o7D0HLQjK1YfRTeWzAmZpzLsBJy0fG5TuafOxGtiuRZHLnk1VGncCMrPym10FeFhW6PMjVKDzX9Ah+VQVZvFFeExpOVtCkAABROSURBVPu5iWdu/+HrizrafPpWfa8n7CxctrqVvfn6yd/o6Nb1+Oa+kSVv55LrXnXz1t0p+p9xs3V3yr3q5iVv6ty0smdCw09e6Fz990YMt3P13y+7z1sI95dNz5z0/g7tScvAnUr7rQqblsrrOqtahxoc40vejbmJ7+DNR41aetY7uFDqGAn0jM04xmbanAHH2IxjTEjvOoiNPWMzHSNL39S5acXPBHyW4zMhfGY5BEGQeYLnQhAEmSd4LgRBkHmC50IQBJkneC4EQZB5slhv10IQBEHmyKL+wDcAAACLBDwXAADMA54LAADmAc8FAADzgOcCAIB5wHMBAMA84LkAAGAe8FwAADAPynNjYhMhCIKgxRM8F4IgyDzBcyEIgswTPBeCIMg8wXMhCILMEzwXgiDIPMFzIQiCzJPKc/P65f93l9Clz1S6pd95Kg8nxhy2jRl4Hy0C3LavYxO/rvYwfxn0e4c6bFcOJhvo2NYtJyvr+t1ef0jYOTQz6XP3NttunE7buNSDvhj6KK/fLw+Vp/KwssDGg/fr+t2TgVl5MJsrj27bOp86IQiKVM+S54oEeu+c2KDTq7hz9wamQtr7hzpv8yWvdUvb5mgoZFM785buRCaWdQfILiq6s/Xgg1E/xyLofvSthu2GqROCoLnoWfRcTnf9Hyjo0/FbjuPkfq0Qz407Vzk2qzc++qfJ2/ZdXOR1QhA0J83Pc+m6KK9U7kuqpFN3JZP1jFWfiYlNjIlN/uJ0XS8Rqvkar7ArP9Xmkwr5RysvH38nLjEmNnHDXw4fyrF1jodWnOdu/a5xUuWjZHe2XuuW3dPfXbYlLnHDtrynsqXO9t7cG2GdEATNUc+K5ybGxCZuKByQ9xt49C6r8q+ejEtFJp5eVBVI3l066Oss0Qv9uktiYvceKLDZelxuXyAobg76p9zDnbdPHtnA6AWNKlon7JjYixyi909fqXYMjUuHC/l97t6GyuM7wySvpZRraKCtkX128u1yisBZtEkczLx+6XYg1F2yIbI6IQiao54lz4351i4b6ljDV6zKEx645LpDvvbSq9v/wspXhvFcsqcKZn2dJR8pe0EToedu+LZhJMiqh9MfxsSY3TVDvHH6+68lapydbxrkEZmwH4xlbXfbvo6oTgiC5ipdz9VjCTz3o9IRabv0HEypEy3qZHDQPzXS31mZc+HD98Vic/dcjuMm604kLpznXq7zah9KZxjllKun6tutmldEMtkyXJPAPgv91yKqE4KgueoZ8dy4PdsvtxDBoDoFKenANfpxO0Voqr86+yOxsHY+90zl6NRQc+XxA/v5F8s2/OXwiadyq1xP0tVNVeRzjXouGW/2V33CXxXi9nxy7E5lz0SgU2sY91o7+W7OuqrPbeDbzPRHzRsXtecarpMl9pNP/Tgdgp4/LXfPZRIasx1UP2qXFHcko9rp03x5QTKUMM/QNiZcyHzU0trv9vqmpNdaeaToewE8d9OjPqJtgbFhW3Wl9fQpOSRnSUq5+rtLxEvIfD03gjpZgudCkBEt93yumpCn63yigb69v//Q5bq6Hpd7ckZpv6Ghgk2JMXqee+B8s1crxcotrOfG7i3oU7yVJbTS3V62Q+PSQrRcj868CHILEdTJahI8F4KM6Bny3JB/3GUrtW7RiXC19H7a8UdD6j+p0vLcd28OSTbtH2q5ciw1MSGZHJkF9dzEmLgTue1siw/1PWC+nhGBPxp+hjZPz4UgyIiWu+dS7y0Y0NfV7pGGPJYvMxqv4blbcx3SdvnlqkX0XF7v7z90uby8wdE7RgbmRAMIReKPRt8Vg+dCkAlaeZ7r4TiOC3o7q8uPHzgs/0FEce+E5DGBnvNxiTGUy8wOPT4tfg5D+iP5nYZAZ3HaxtjEDX/54Ub3pLpVZFM9bTdJryd/FXDc3xKXGPN+2vHSfvklBX6IDttGfC7bo4JDX+4RXpJ9/4cncgjK9lyWtM7O1vNtM9IvpL+JIF681XkgifcWIGiBtUI9V4/ZoQff6RXuLjnbOsPYTiC1iowWZdy2r7V+RR8oJjbMH+aGhmu2Gu279tnR/9tff9dZzXQNPBeCFlgrzXO/qnDrOV1oqv/RRflzxaT3/0m6S5jbQyH5SZfcqrh8u/rDY/g/MWB8ZAHnHxqV7S+c54Yme68ZeVoY/uzM6TNuDJxxCIIi1Urz3JjYxI07r96o7ux1ES948R/k2MD46MKNB+/bhr1+6U9uA353Q05MbOKGvWWtLvkvcb39dUeKGfncmNjEDdvyynvkz0jkggFv72Ph9YDE7Mp+XyDEcRwX9Ltbi09vVOdz4/bsv1yn/NjJuTwtDHN2+J5G9FmO8FwIWnDhM8shCILMEzwXgiDIPMFzIQiCzBM8F4IgyDzBcyEIgswT5bkAAAAWFXguAACYBzwXAADMA54LAADmAc8FAADzgOcCAIB5wHMBAMA84LkAAGAe8FwAADAPeC4AAJgHPBcAAMwDngsAAOYBzwUAAPOA5wIAgHnAcwEAwDzw+bkQBEHmCXEuAACYBzwXAADMA54LAADmAc8FAADzgOcCAIB5wHMBAMA84LkAAGAe8FwAADAPeC4AAJgHPBcAAMwDngsAAOYBzwUAAPNY9M+4WaqOAQDAMgSeCwAA5gHPBQAA84DnAgCAeSg992gdx3Fc9clF89zWlLUWkbUprcIm4ScNwhZYEpZnqwAAyxqTPTc/zmKJy5f+w/8IzwUAPC9QnptQOMIqM5K7IzEmNjFmR9kAx3Gc/WhsVjXHcdzIwKBQYqAwVTBZoQzHcRxXl6X03NaUtbLlypssROSbr4yDlQVaSa8jfs6Po6Nn8ghrU1LiLKLbSxWKBeUC9N7ykdUl165VtCrcQAMAABc2zuX/y1uq4Mh1WTGi5+buSIyJTc0dFHfhDbcuSyo8UJhKHy4/jmFQGgFja8paoSxdgOm5+XFivfkpas+lgmupYJxcPenHcsZD3ItVEnEuAGAOhMst8DY6WJYgeCvvs7zn2o/GJpL2Su170s5xHFeXpTwgEbaGMy9pe0Seq10Nx5dT2a+ygMGN8FwAQMSEzeeKVntSMl+l50r2yu9LMVimeWQ55FWaF2HLhj2X2EmRvFCWt1iUSQGmvVImLsa88FwAwPww8AyNt1SO4+S8rYE4N/y7YtLNO21e8mM27ThXO0QmkgPEJnacyypANipMRAzPBQBEjNJzJQMlfJN3WE4ObCnPJXK7O8hYODFmR9mAIreQH0fHjnJmVbI32es0Cii2i2YYJz9KUz+loy1d+p/w4gRRgHBajXwuXZXKvwEAQA/130QIz8Q4jpNMVkga1GWpXJhHfLFB8d4Cp87nSi8XaLwhsDalVSqzNi6O8kLGqwxxKUQ+VyO1YCh1wXwDQZ2uUFSlegMCAAD0MfJ3aILDEkkDOp/7rP8dGpIEAACzMOC5fD5XyhjAcwEAYK7g8xYAAMA88JnlAABgHvBcAAAwD3guAACYBzwXAADMA54LAADmAc8FAADzgOcCAIB5wHMBAMA84LkAAGAe8FwAADAPeC4AAJgHPBcAAMxj0T/jBp+VAwAAEvBcAAAwD3guAACYBzwXAADMA54LAADmAc8FAADzoD2X/+qzOrvwtb6DZQnS9/hKX/pLfrOvuFH4YmBio3IL/X3A8ne580eUEb9CWHWUOfcQAACWDyzP5exHJdMcLEsQvn19JHeHaIV1WTGxiQmFIxzHDRSmSj8IXlmXpd4i/BubKH5/JVGb8F2WYY5iznAAAMCiwoxzKbMj3ZA3YuFL18XChDsnUmEv9VXBsqRKKGsOd5TFHwoAAFh0jHsuV32SzhjwkPkHjhN2YWUSFPtKtQneqvJcxVHm1DsAAFhezMVzRZdkB7DMgDe3juNo8zUa5+IZGgBgBRGZ5woBrOSqO8oG6rKO1tEPvthbxBrI2vjDCbWp8rz0UUwaDwAAWEwi9FzFGwUckc/lGSxLUKQReOuU9xoZIGrjj0JsZ723wCGfCwBYISyb93MVsS3ezwUArESW1nOzqsnDaxsuPBcAsDJYNnFuOC3VAAEAwAKCzywHAADzgOcCAIB5wHMBAMA84LkAAGAe8FwAADAPeC4AAJgHPBcAAMwDngsAAOYBzwUAAPOA5wIAgHnAcwEAwDzguQAAYB7PzGfc4DNxAAArAHguAACYBzwXAADMQ8dz5S+FXHIDXSzPbU1ZuzaldTHGdQlZkZ1aQrTGk7kdgw/CYarnCt9+VpelU0bxpb9Gdgnvua0pay1q4vKfjRUSYSufjU6JLGRrF7rnfH3wXLCgLLs4V//r3Ocb57amrLXE5ZP/fQZWyIr23IVkcXoOzwULispzFV+4y/wiXjHqpL7fl9/If3Mwz2BZgvRFwiftHGc/Kn+vsPDN6vx3AHPCFwyTXwPMQ+6SSH9/Gt2wQXs1XRW7uyzPTYnjw15yrUiBMWsByXvxVTEL58dJ8TS/mVyN1Mpk7C7tvDYlnwrRhSKsI5KxvIYX6LeZOCh9EKkMswuaNeu2lm5Yq/roYc+RsnmtrIHSGht6HuTHKQZabrgY52r9VtkweYh0ew2eY2jPFYzVflQR5/Lb6e9gJ76MPTFmR9mAYKzSV0lmVUueOzgiVKvy3NwdiYrvclfGudIu1BcDq9omVMVXaz9q3HNFq+AXXqviR2Ijx9yLXZg4DO1Nas9l7E5sShEMgmqE0SNG1GbGQVUjo+W5dM3kABsazLUprcqja7Sc2V9qYziDa01ZaxE9Xmynsl1SGWZugfwteZ2hhyhMr8FzDOW5lI0Svkb5oGiCQpBLfFkvIy1AuTBhoIQ5kj6u47l024hdNLyY3V2d3IL8M7EcmSuGWoSswuRRwngua3eGBygiaGNHjKjNioMqyxMepNMvqmbtY6kbpmNMhg7K2KqBVEZqmPrYYhl2Ppf+rbSNr03YGK7X4DmG8lza75SeSzFYlkBnIQYKUzU9V3oCpuG5ZDGDnhs2/mV314jnKh+46XouszC5yvQ9l30seSu5jrlIjxhRmxUHpYxCHLWw9qcY3ggGU9Fl7ZYro3JObXhM1CkPwReJqpRlaM9l/1aoXjRZaRf9XoPnmMjjXI2nXtxgWe5cPXcZx7kalehFdtpxLiMU1TuWeMeqE+fqdySSNisOSg1UJPG7XpyrcTzFBUUj6FQelBFIanuunPKg0zpx+a0pa+UchaIM6blav1V0nBHnAkDByudKCVmOo3KmUopgR9lAXdbROvopljKfm5pbRzxD0/NcMrebqBnP8sW08rkL6blUFJUfp2tw7MJEVpM0BCLCYuSOxd1bU+JIU+BUvqVxRCLTGcZzjRxUI5+r7gJVM53cNTyY+aous1su/8zM52panVyaGp38OMvataLlMsoQnpuv9VuxIoUhM3tNnnfw3KJ8b4F4c4B/qYD13gJH5HN5FBlbaWMYz5Ug30gTHqlxHBfJewsL6bmsO1G6EmWCT1VYfgy/VioslYtL0T2W/MoDEVRS9esdcW1cnIF3mCI5KDFijC7o1Ey4WZjBVB897DliZiOUAyXBHh36iZ+qDJVbYP1WnTygrwqK38JzAbd0f/tL53MXQUsznGr0cowAgOeO58JzLQAsJ8xc4WC5gc+4AQAA88AlFwAAzAOeCwAA5gHPBQAA84DnAgCAecBzAQDAPOC5AABgHvBcAAAwD3guAACYBzwXAADMA54LAADmAc8FAADzgOcCAIB5rNjPuIEgCFqGQpwLAADmAc8FAADzgOcCAIB5wHMBAMA84LkAAGAe8FwAADAPeC4AAJgHPBcAAMwDngsAAOYBzwUAAPOA5wIAgHnAcwEAwDzguQAAYB7wXAAAMA98liMEQZB5gudCEASZJ3guBEGQeYLnQhAEmacl8dysao7jOK765NL3fxF10s5xHFeXtfQtgSBo2YjtuUfrxE2LYhnz8dzU3EGNx4FL4G5CYwYKU9W/5cdQ6OOOsgFWk5kjIA++ZqeYgzCSuyMxJjYxoXCE3m4/avawQBCkKabnCp64aCt2QeJc0XeWMpDU8Vy+j+zRE21RcElSguHWZSl+Ntz3rGpiu2jfsF0IWi5ieS5/U8yIxUSvrLOrfkvatMJN6KBssCxBqqdQjv6Io1DlmSEkVUzlO+w4UYg0R3ILpcbbjxLHIg6k2WC6d+pgk7A2/nDs64FYm/q3YjgsjIZwItTWrOu5xEYdc4cgaEnE8FwpvFLFWbRZUI6QVT1YlhCbGCOvc96AhF1ER+OLifUMliWwy4t1Ku2YFNN3qMBTqJlvmHR3X5dF2OVI7g5VR5gNZvdOM87lizFbrueDCpNVWLC670y7DzNEEAQtpVSeS65zwQKk9awwQYU9iZIiyh0Kh5JE10MYjVBecWusn9Mkf6vTYKJVcs20HVef1Gowu3fanstvZ0aX2kFurKbnagf7xDhQbZYDc919IQgyW0rPpUxHueYVnks7jvIx0UjuDtraZGl6LpUWkFDuTh2dU99Ky+WJFhrzXI0Gs3un6bl8YVazw9zsz8Vz5VyQOqwOfwmBIMhcKTxX460AwT40PZeyEsLdNALVcJ5r6F54MT2XboBW77Q8ly+v/WBNu4NGcwu0dIoZrAGCILNEe65qidJxGTu3oLQqdW7BcJwbSVy2mLkFusFavdPwXM3EAjvIJcdc5xkaXayaPCJZ7KSdsld4LgQtM1Gey7A86vaW8kqyMGVhRh5JaTmLOkd80h5BPjf8M7QwnstscK5m71hxsVZigXqIx9jOj4bmu2IqaxZbSI/DSTvpsJqpEgiClkiU5+q4GOuVKdKdiaTE4AjpbnrvirHfiFLlNyLw3MSYsO+KhfFcZoN1eqd6l/mknWMlFsRWqeJfVSiq236+mHKINF900xw9CIKWRhH97e/z8Te789PROpaxQhAExSbGwHMXWnp/fgZBEATPhSAIMk/4bh4AADAPeC4AAJgHPBcAAMzj/wOfaVBItxKISAAAAABJRU5ErkJggg==)

　　　　在开发中如果项目中有一些敏感web资源不想被外界直接访问，那么可以考虑将这些敏感的web资源放到WEB-INF目录下，这样就可以禁止外界直接通过URL来访问了。

　　register.jsp页面的代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4     <head>
 5         <title>用户注册</title>
 6     </head>
 7 
 8     <body style="text-align: center;">
 9         <form action="${pageContext.request.contextPath}/servlet/RegisterServlet" method="post">
10             <table width="60%" border="1">
11                 <tr>
12                     <td>用户名</td>
13                     <td>
14                         
15                         <input type="text" name="userName">
16                     </td>
17                 </tr>
18                 <tr>
19                     <td>密码</td>
20                     <td>
21                         <input type="password" name="userPwd">
22                     </td>
23                 </tr>
24                 <tr>
25                     <td>确认密码</td>
26                     <td>
27                         <input type="password" name="confirmPwd">
28                     </td>
29                 </tr>
30                 <tr>
31                     <td>邮箱</td>
32                     <td>
33                         <input type="text" name="email">
34                     </td>
35                 </tr>
36                 <tr>
37                     <td>生日</td>
38                     <td>
39                         <input type="text" name="birthday">
40                     </td>
41                 </tr>
42                 <tr>
43                     <td>
44                         <input type="reset" value="清空">
45                     </td>
46                     <td>
47                         <input type="submit" value="注册">
48                     </td>
49                 </tr>
50             </table>
51         </form>
52     </body>
53 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　register.jsp中的<form action="${pageContext.request.contextPath}/servlet/RegisterServlet" method="post">指明表单提交后，交给RegisterServlet进行处理
     3、在**me.gacl.web.controller**包下编写用于处理用户注册的RegisterServlet

　　　　![img](data:image/png;base64,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)

　　　　RegisterServlet担任着以下几个职责：
            　  1、接收客户端提交到服务端的表单数据。

　　　　　　2、校验表单数据的合法性，如果校验失败跳回到register.jsp，并回显错误信息。

　　　　　　3、如果校验通过，调用service层向数据库中注册用户。

　　　　为了方便RegisterServlet接收表单数据和校验表单数据，在此我设计一个用于校验注册表单数据RegisterFormbean，再写WebUtils工具类，封装客户端提交的表单数据到formbean中。

　　在**me.gacl.web.formbean**包下创建一个用于校验注册表单数据RegisterFormbean

　　![img](data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAL8AAAAtCAIAAABwJi0ZAAAJgUlEQVR4nO1abU8bVxa+/yIfo03VKAraVQw23U4pyj/Il03ANE7SrTTa7aoK7G4T8b68hLJUmx0tVdImQEibACEoi5JmCMoSygcKJA3a0GyDGPHiYsBje4zBGI8x9O6HGc/cmbljO2aYAp2jR+jeO+c858yZx3cuBgDTtoWR9m8v/QbFwkh7+uGW7T8DP3cBlu1hs9RjWeaWlno2t34amOA+63PX90xb2Ov4rM89MMFtbv1kknq+/s73cDzgCcZXeWhhr8MTjD8cD3z9nc8k9TT1znCRLW4dBiIW9jy4dchFtpp6Z0xST33P9AoP/WsW9glWeFjfM22eekJRyIbxKMk7oHdpN4F2AbJzWwxMXS5w3UvtA0BKN2OTvjZCUXPVsxyF3lURg7W2obJfS2g+eRCdDtbaJM/dBNoFyI5tMTC1ucDVk8xntJEA79OGlp06aQZYNlk9wXW4uCpi6GIWM9i2MHZzYezm+N1P24oL0enQxSzJczeBPg3IW9tiYGpywemeZD633gf2RsbQslMnzQDB9Z1Rz+DgYGNjo1Y9XAR6QiIGPz66MHYztvKYe9Hx6tHVtuJCdDr48VHJczeBfg+QX22LgflbLnjvbjKfr86BnE8YQ8tOnTQDcJEdUM/AwEBWVtbhw4e16vGvQfeyiMclRxfGbnIvOrgXHa/6PxfUI00flxyVPJWgiwBR3U3lAAAAKOqG7m4SAAAAyGlgEB+gWYSaq0R1Awkc1JCwPi5yirR4KroIkO0IW/s5KQVdJAV2S7TaSphqByjqlteRXCJhwoREsieQUyMkDmoodU+SJMX1CtMKukhoF+LpXzNaPYJ0CgsLsephw3A2KKLvoyPjdz999ejqq/7PR9prS08cR6d9Hx2RPJWgnQAAB/VNEM4KPTpLzwbh7HMqG5A3RAei6rngzFQ5gLNbHZ5YYaocCaogU3VWGAi0EhXIbmCU4cIlEd80EGIB3WS2gxCcb5wVorCVMFUOACQSOZeMRLimANlZSZK6J3pJdSrUaQVCS1Q9h2zYUPVI0nE6nVj1eMNwOgCnA7Ak70DzyYNtxYUCSk8cFyCtNJ88WJJ3QHBWgnYCovKp/riLBErLrmfk8C4SOKgnOtMn9YT0KW/VOotZyFZ05SmVDcjWAGw9Q1Q+pZ0O6kmAqXQkqYSpdABnl8SgmsLpAGw9k6hZXYDkrIpK1RO9pPq9UrdCQSveoNdA9aDS0VPP4gqc8ou498Gb6DG59MRxdHrvgzclTyXoAkCUj+mPu0jgoB7jYzVXpekYZQPAVs9MiWOyRessZiFbFCtMuYMoH6MLHNRjYdxF2YQofCVMuQMUdOlN4ZQftpxJVKJmEHJpo1L1RC8ptkJsKxS0YhmLK8app7Gx8TBiDQ0NWvV4QnCSFXHn3BtJTs13zr0heSpBFwCibDTJmC4AoKBT9L/mIq+xcHKUsmGuMmV2AOxUPwsnO0lxwML+OgIA8hqrcqbL6phJli4AKkLYX0fY7ARw0dLYVsdMqsMTlQhJXfSkKhdCeM0FUIbEGC2SKbPLzGn0RCcptkLdVki0TJmdKBuFnpC5v7HPh+APrIjbpw8lOTXfPn1I8lSCPgWI0tGk41HKlth2T3VKK9qrRGkdCezUIxb+wDKldnHV5iJtgPyCVTkLK/QpYaAhxCTCVsIypXZwyiW9MtBEYuAXLmCrY5D7BWpngaQz/Z7oJNWvUNMKlJYptROlo3DeZPX8GIQvl0R86TyEHpPbigvR6ZfOQ5LnDuI2CewUbUKi/Ygfg+aqZy4IJxZFDNdltzt/JaH55EF0OlyXLXkaCuaCnbwijunfAXCshtmZRPsfcyarZzYIXyziUZJ3QO+SwRimjiX26mM1jElJ9yNmzVTP5Qdzk97N7xfhfxcs7Hl8vwgnvZuXH8yZpJ6+cX/vM47xbel8DWhhL4HxbfU+4x4+N+u/w2LxrQfPfE29Mz/7f1Va2D6aemcePPPF4lsmqccyy7BmqceyzM1Sj2WZm6nqud90DAsza7DMQDNVPf9utMU2NlV4ef+PZgiIoQhAUMyO59nVZnQTTFVPzyfZWvVMDVT8L4WAGIqQ/2KU4e0b2Tgj6tExmtwpZgh3n3qGh4fX1tbSdO5qyInGNgWMuLkjv7/yJ+o/c6P/nBqoeHn/D3cu5ejEMRQBSFqc0CSQJ8aYgn8H/F/DaFJ+vDtwpwbbdtXT0dHR1dUVCATScb5VZ49E45FofJ2PS+qJ8PF1Ph7h47fr7TpxyqdFk78Q9exgGoPMAPX4fL7r1697PJ6UzjdqHeH1eHg9vrouqycciYcj8fB6vL3WoROHtlG1+co7PW6RoKjE00A4kBcPSaPvCpFCyykE06S4hH2s6DtHuqgKpElAULSYnqTlIHS/USbF8Mt3qrwTmYBS0+o0Eo1VNFaeYFLIZoB6IISBQICiqKmpqeTOLdWOUDgeCm+E1mT1hNY2QuH48lq8pTpXJw49Z6ANwX5Q0f2eoQigVg/m0aNLWE6hAIUmlAXRJFqZXIIqUJJRQgyyl+ijc+7RqYrUvuRoUkGr1I+cCBcrJ0meQjZj1AMhXFpaqqmpCQaDSZyvVr4VWN3gVjYCqzFJPYGVDW41xq1uXKl4SydOvmnFLdDq/+2VPt4YgSk5lCrUe3qSn0pxGgGqkupKVXWqwYwVTOhzxVSVyKTYGlQ601NP0tgUbrIZox6O49LZe/5V/lt2OcYux9iQrB42FPOFYmwo1lz+tk6cam9AnrT2F4jU6pE5NfuSDmcm6sFmfU31SDNsVcjGqvxspKEefKwYLIfpuElmgHr8fn+a557LF99eDPIjbu7D9uEP24cF9SxyscUgv8Tx/7iYjnrQ/Vi1E2m3J/ybi6JVtDrqlDlTqUf75sK3PNO9B1uV6oiURD1Sy9DXOyZWiCBJSXN6bgkz9XeuxgvvzPv5ETeX/+dOQToePz/v5+cD/Lw/9vcL7+jEYR6e4lyh2lXlRfypWX4PqE6QuufElOpRsGpOzahPavUADI9uVeJ9kmSyvUerHnws1IhEz000U7/vufSXPDfLu31834Snb8Lj9vFuHz/n490s7/ZF6/+at81iMIZ/E/1SzejvAEz9rrmmJG/WG53x8rPe6Kz4Mzrrjc54ozPeaG2JIephKEJxrrbEI5nhf60xVT3Vxe+Wn8+vKM6vKM4vR35WnM+vOJ9fXfyuMWmQTd6SjmjiG9HgLx+t/9CwLHOz1GNZ5mapx7LMzVKPZZmbpR7LMrf/A/+GYfI+o8MzAAAAAElFTkSuQmCC)

　　RegisterFormbean代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
  1 package me.gacl.web.formbean;
  2 
  3 import java.util.HashMap;
  4 import java.util.Map;
  5 
  6 import org.apache.commons.beanutils.locale.converters.DateLocaleConverter;
  7 
  8 /**
  9  * 封装的用户注册表单bean，用来接收register.jsp中的表单输入项的值
 10  * RegisterFormBean中的属性与register.jsp中的表单输入项的name一一对应
 11  * RegisterFormBean的职责除了负责接收register.jsp中的表单输入项的值之外还担任着校验表单输入项的值的合法性
 12  * @author gacl
 13  *
 14  */
 15 public class RegisterFormBean {
 16 
 17     //RegisterFormBean中的属性与register.jsp中的表单输入项的name一一对应
 18     //<input type="text" name="userName"/>
 19     private String userName;
 20     //<input type="password" name="userPwd"/>
 21     private String userPwd;
 22     //<input type="password" name="confirmPwd"/>
 23     private String confirmPwd;
 24     //<input type="text" name="email"/>
 25     private String email;
 26     //<input type="text" name="birthday"/>
 27     private String birthday;
 28 
 29     
 30     /**
 31      * 存储校验不通过时给用户的错误提示信息
 32      */
 33     private Map<String, String> errors = new HashMap<String, String>();
 34 
 35     public Map<String, String> getErrors() {
 36         return errors;
 37     }
 38 
 39     public void setErrors(Map<String, String> errors) {
 40         this.errors = errors;
 41     }
 42 
 43     /*
 44      * validate方法负责校验表单输入项
 45      * 表单输入项校验规则：
 46      *         private String userName; 用户名不能为空，并且要是3-8的字母 abcdABcd 
 47      *         private String userPwd; 密码不能为空，并且要是3-8的数字
 48      *         private String confirmPwd; 两次密码要一致
 49      *         private String email; 可以为空，不为空要是一个合法的邮箱 
 50      *         private String birthday; 可以为空，不为空时，要是一个合法的日期
 51      */
 52     public boolean validate() {
 53 
 54         boolean isOk = true;
 55 
 56         if (this.userName == null || this.userName.trim().equals("")) {
 57             isOk = false;
 58             errors.put("userName", "用户名不能为空！！");
 59         } else {
 60             if (!this.userName.matches("[a-zA-Z]{3,8}")) {
 61                 isOk = false;
 62                 errors.put("userName", "用户名必须是3-8位的字母！！");
 63             }
 64         }
 65 
 66         if (this.userPwd == null || this.userPwd.trim().equals("")) {
 67             isOk = false;
 68             errors.put("userPwd", "密码不能为空！！");
 69         } else {
 70             if (!this.userPwd.matches("\\d{3,8}")) {
 71                 isOk = false;
 72                 errors.put("userPwd", "密码必须是3-8位的数字！！");
 73             }
 74         }
 75 
 76         // private String password2; 两次密码要一致
 77         if (this.confirmPwd != null) {
 78             if (!this.confirmPwd.equals(this.userPwd)) {
 79                 isOk = false;
 80                 errors.put("confirmPwd", "两次密码不一致！！");
 81             }
 82         }
 83 
 84         // private String email; 可以为空，不为空要是一个合法的邮箱
 85         if (this.email != null && !this.email.trim().equals("")) {
 86             if (!this.email.matches("\\w+@\\w+(\\.\\w+)+")) {
 87                 isOk = false;
 88                 errors.put("email", "邮箱不是一个合法邮箱！！");
 89             }
 90         }
 91 
 92         // private String birthday; 可以为空，不为空时，要是一个合法的日期
 93         if (this.birthday != null && !this.birthday.trim().equals("")) {
 94             try {
 95                 DateLocaleConverter conver = new DateLocaleConverter();
 96                 conver.convert(this.birthday);
 97             } catch (Exception e) {
 98                 isOk = false;
 99                 errors.put("birthday", "生日必须要是一个日期！！");
100             }
101         }
102 
103         return isOk;
104     }
105 
106     public String getUserName() {
107         return userName;
108     }
109 
110     public void setUserName(String userName) {
111         this.userName = userName;
112     }
113 
114     public String getUserPwd() {
115         return userPwd;
116     }
117 
118     public void setUserPwd(String userPwd) {
119         this.userPwd = userPwd;
120     }
121 
122     public String getConfirmPwd() {
123         return confirmPwd;
124     }
125 
126     public void setConfirmPwd(String confirmPwd) {
127         this.confirmPwd = confirmPwd;
128     }
129 
130     public String getEmail() {
131         return email;
132     }
133 
134     public void setEmail(String email) {
135         this.email = email;
136     }
137 
138     public String getBirthday() {
139         return birthday;
140     }
141 
142     public void setBirthday(String birthday) {
143         this.birthday = birthday;
144     }
145 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　在**me.gacl.util**包下创建一个WebUtils工具类，该工具类的功能就是封装客户端提交的表单数据到formbean中

　　![img](data:image/png;base64,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)

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.util;
 2 
 3 import java.util.Enumeration;
 4 import java.util.UUID;
 5 import javax.servlet.http.HttpServletRequest;
 6 import org.apache.commons.beanutils.BeanUtils;
 7 
 8 /**
 9  * @author gacl
10  * 把request对象中的请求参数封装到bean中
11  */
12 public class WebUtils {
13 
14     /**
15      * 将request对象转换成T对象
16      * @param request 
17      * @param clazz
18      * @return
19      */
20     public static <T> T request2Bean(HttpServletRequest request,Class<T> clazz){
21         try{
22             T bean = clazz.newInstance();
23             Enumeration<String> e = request.getParameterNames(); 
24             while(e.hasMoreElements()){
25                 String name = (String) e.nextElement();
26                 String value = request.getParameter(name);
27                 BeanUtils.setProperty(bean, name, value);
28             }
29             return bean;
30         }catch (Exception e) {
31             throw new RuntimeException(e);
32         }
33     }
34     
35     /**
36      * 生成UUID
37      * @return
38      */
39     public static String makeId(){
40         return UUID.randomUUID().toString();
41     }
42     
43 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　最后看一下负责处理用户注册的RegisterServlet完整代码：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 import java.util.Date;
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.HttpServlet;
 7 import javax.servlet.http.HttpServletRequest;
 8 import javax.servlet.http.HttpServletResponse;
 9 import org.apache.commons.beanutils.BeanUtils;
10 import org.apache.commons.beanutils.ConvertUtils;
11 import org.apache.commons.beanutils.locale.converters.DateLocaleConverter;
12 import me.gacl.domain.User;
13 import me.gacl.exception.UserExistException;
14 import me.gacl.service.IUserService;
15 import me.gacl.service.impl.UserServiceImpl;
16 import me.gacl.util.WebUtils;
17 import me.gacl.web.formbean.RegisterFormBean;
18 /**
19  * 处理用户注册的Servlet
20  * @author gacl
21  *
22  */
23 public class RegisterServlet extends HttpServlet {
24 
25     public void doGet(HttpServletRequest request, HttpServletResponse response)
26             throws ServletException, IOException {
27         //将客户端提交的表单数据封装到RegisterFormBean对象中
28         RegisterFormBean formbean = WebUtils.request2Bean(request,RegisterFormBean.class);
29         //校验用户注册填写的表单数据
30         if (formbean.validate() == false) {//如果校验失败
31             //将封装了用户填写的表单数据的formbean对象发送回register.jsp页面的form表单中进行显示
32             request.setAttribute("formbean", formbean);
33             //校验失败就说明是用户填写的表单数据有问题，那么就跳转回register.jsp
34             request.getRequestDispatcher("/WEB-INF/pages/register.jsp").forward(request, response);
35             return;
36         }
37 
38         User user = new User();
39         try {
40             // 注册字符串到日期的转换器
41             ConvertUtils.register(new DateLocaleConverter(), Date.class);
42             BeanUtils.copyProperties(user, formbean);//把表单的数据填充到javabean中
43             user.setId(WebUtils.makeId());//设置用户的Id属性
44             IUserService service = new UserServiceImpl();
45             //调用service层提供的注册用户服务实现用户注册
46             service.registerUser(user);
47             String message = String.format(
48                     "注册成功！！3秒后为您自动跳到登录页面！！<meta http-equiv='refresh' content='3;url=%s'/>", 
49                     request.getContextPath()+"/servlet/LoginUIServlet");
50             request.setAttribute("message",message);
51             request.getRequestDispatcher("/message.jsp").forward(request,response);
52 
53         } catch (UserExistException e) {
54             formbean.getErrors().put("userName", "注册用户已存在！！");
55             request.setAttribute("formbean", formbean);
56             request.getRequestDispatcher("/WEB-INF/pages/register.jsp").forward(request, response);
57         } catch (Exception e) {
58             e.printStackTrace(); // 在后台记录异常
59             request.setAttribute("message", "对不起，注册失败！！");
60             request.getRequestDispatcher("/message.jsp").forward(request,response);
61         }
62     }
63 
64     public void doPost(HttpServletRequest request, HttpServletResponse response)
65             throws ServletException, IOException {
66         doGet(request, response);
67     }
68 
69 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 　　用户注册时如果填写的表单数据校验不通过，那么服务器端就将一个存储了错误提示消息和表单数据的formbean对象存储到request对象中，然后发送回register.jsp页面，因此我们需要在register.jsp页面中取出request对象中formbean对象，然后将用户填写的表单数据重新回显到对应的表单项上面，将出错时的提示消息也显示到form表单上面，让用户知道是哪些数据填写不合法！

　　修改register.jsp页面，代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4     <head>
 5         <title>用户注册</title>
 6     </head>
 7 
 8     <body style="text-align: center;">
 9         <form action="${pageContext.request.contextPath}/servlet/RegisterServlet" method="post">
10             <table width="60%" border="1">
11                 <tr>
12                     <td>用户名</td>
13                     <td>
14                         <%--使用EL表达式${}提取存储在request对象中的formbean对象中封装的表单数据(formbean.userName)以及错误提示消息(formbean.errors.userName)--%>
15                         <input type="text" name="userName" value="${formbean.userName}">${formbean.errors.userName}
16                     </td>
17                 </tr>
18                 <tr>
19                     <td>密码</td>
20                     <td>
21                         <input type="password" name="userPwd" value="${formbean.userPwd}">${formbean.errors.userPwd}
22                     </td>
23                 </tr>
24                 <tr>
25                     <td>确认密码</td>
26                     <td>
27                         <input type="password" name="confirmPwd" value="${formbean.confirmPwd}">${formbean.errors.confirmPwd}
28                     </td>
29                 </tr>
30                 <tr>
31                     <td>邮箱</td>
32                     <td>
33                         <input type="text" name="email" value="${formbean.email}">${formbean.errors.email}
34                     </td>
35                 </tr>
36                 <tr>
37                     <td>生日</td>
38                     <td>
39                         <input type="text" name="birthday" value="${formbean.birthday}">${formbean.errors.birthday}
40                     </td>
41                 </tr>
42                 <tr>
43                     <td>
44                         <input type="reset" value="清空">
45                     </td>
46                     <td>
47                         <input type="submit" value="注册">
48                     </td>
49                 </tr>
50             </table>
51         </form>
52     </body>
53 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

 　　到此，用户注册功能就算是开发完成了！

　　下面测试一下开发好的用户注册功能：

　　　　输入URL地址：http://localhost:8080/webmvcframework/servlet/RegisterUIServlet访问register.jsp页面，运行效果如下：

　　　　![img](https://images0.cnblogs.com/i/289233/201408/161418014833107.gif)

　　如果输入的表单项不符合校验规则，那么是无法进行注册的，运行效果如下：

　　　　![img](https://images0.cnblogs.com/i/289233/201408/161421034364428.gif)

#### 3.4.2、 开发登录功能

　　1、在**me.gacl.web.UI**包下写一个LoginUIServlet为用户提供登录界面

　　![img](data:image/png;base64,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)

　　LoginUIServlet收到用户请求后，就跳到login.jsp

　　LoginUIServlet的代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.UI;
 2 
 3 import java.io.IOException;
 4 
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.HttpServlet;
 7 import javax.servlet.http.HttpServletRequest;
 8 import javax.servlet.http.HttpServletResponse;
 9 
10 /**
11  * @author gacl
12  * LoginUIServlet负责为用户输出登陆界面
13  * 当用户访问LoginUIServlet时，就跳转到WEB-INF/pages目录下的login.jsp页面
14  */
15 public class LoginUIServlet extends HttpServlet {
16 
17     public void doGet(HttpServletRequest request, HttpServletResponse response)
18             throws ServletException, IOException {
19 
20         request.getRequestDispatcher("/WEB-INF/pages/login.jsp").forward(request, response);
21     }
22 
23     public void doPost(HttpServletRequest request, HttpServletResponse response)
24             throws ServletException, IOException {
25         doGet(request, response);
26     }
27 
28 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　2、在/WEB-INF/pages/目录下编写用户登录的jsp页面login.jsp

　　![img](data:image/png;base64,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)

　　login.jsp页面的代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java" pageEncoding="UTF-8"%>
 2 <!DOCTYPE HTML>
 3 <html>
 4   <head>
 5     <title>用户登陆</title>
 6   </head>
 7   
 8   <body>
 9     <form action="${pageContext.request.contextPath }/servlet/LoginServlet" method="post">
10         用户名：<input type="text" name="username"><br/>
11         密码：<input type="password" name="password"><br/>
12         <input type="submit" value="登陆">
13     </form>
14   </body>
15 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　login.jsp中的<form action="${pageContext.request.contextPath}/servlet/LoginServlet" method="post">指明表单提交后，交给LoginServlet进行处理。
     3、在**me.gacl.web.controller**包下编写用于处理用户登录的LoginServlet

　　![img](data:image/png;base64,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)

　　LoginServlet的代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 
 5 import javax.servlet.ServletException;
 6 import javax.servlet.http.HttpServlet;
 7 import javax.servlet.http.HttpServletRequest;
 8 import javax.servlet.http.HttpServletResponse;
 9 
10 import me.gacl.domain.User;
11 import me.gacl.service.IUserService;
12 import me.gacl.service.impl.UserServiceImpl;
13 
14 /**
15  * 处理用户登录的servlet
16  * @author gacl
17  *
18  */
19 public class LoginServlet extends HttpServlet {
20 
21     public void doGet(HttpServletRequest request, HttpServletResponse response)
22             throws ServletException, IOException {
23 
24         //获取用户填写的登录用户名
25         String username = request.getParameter("username");
26         //获取用户填写的登录密码
27         String password = request.getParameter("password");
28         
29         IUserService service = new UserServiceImpl();
30         //用户登录
31         User user = service.loginUser(username, password);
32         if(user==null){
33             String message = String.format(
34                     "对不起，用户名或密码有误！！请重新登录！2秒后为您自动跳到登录页面！！<meta http-equiv='refresh' content='2;url=%s'", 
35                     request.getContextPath()+"/servlet/LoginUIServlet");
36             request.setAttribute("message",message);
37             request.getRequestDispatcher("/message.jsp").forward(request, response);
38             return;
39         }
40         //登录成功后，就将用户存储到session中
41         request.getSession().setAttribute("user", user);
42         String message = String.format(
43                 "恭喜：%s,登陆成功！本页将在3秒后跳到首页！！<meta http-equiv='refresh' content='3;url=%s'", 
44                 user.getUserName(),
45                 request.getContextPath()+"/index.jsp");
46         request.setAttribute("message",message);
47         request.getRequestDispatcher("/message.jsp").forward(request, response);
48     }
49 
50     public void doPost(HttpServletRequest request, HttpServletResponse response)
51             throws ServletException, IOException {
52         doGet(request, response);
53     }
54 
55 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　到此，用户登录的功能就算是开发完成了。

　　下面测试一下开发好的用户登录功能，输入URL地址：http://localhost:8080/webmvcframework/servlet/LoginUIServlet访问login.jsp页面，输入正确的用户名和密码进行登录，运行效果如下：

　　![img](https://images0.cnblogs.com/i/289233/201408/161451385616636.gif)

　　如果输入的用户名和密码错误，那么就无法登录成功，运行效果如下：

　　![img](https://images0.cnblogs.com/i/289233/201408/161455140145725.gif)

#### 3.4.3、 开发注销功能

　　在**me.gacl.web.controller**包下编写用于处理用户注销的LogoutServlet

　　LogoutServlet的代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 package me.gacl.web.controller;
 2 
 3 import java.io.IOException;
 4 import java.text.MessageFormat;
 5 
 6 import javax.servlet.ServletException;
 7 import javax.servlet.http.HttpServlet;
 8 import javax.servlet.http.HttpServletRequest;
 9 import javax.servlet.http.HttpServletResponse;
10 
11 public class LogoutServlet extends HttpServlet {
12 
13     public void doGet(HttpServletRequest request, HttpServletResponse response)
14             throws ServletException, IOException {
15         //移除存储在session中的user对象，实现注销功能
16         request.getSession().removeAttribute("user");
17         //由于字符串中包含有单引号，在这种情况下使用MessageFormat.format方法拼接字符串时就会有问题
18         //MessageFormat.format方法只是把字符串中的单引号去掉，不会将内容填充到指定的占位符中
19         String tempStr1 = MessageFormat.format(
20                 "注销成功！！3秒后为您自动跳到登录页面！！<meta http-equiv='refresh' content='3;url={0}'/>", 
21                 request.getContextPath()+"/servlet/LoginUIServlet");
22         System.out.println(tempStr1);//输出结果：注销成功！！3秒后为您自动跳到登录页面！！<meta http-equiv=refresh content=3;url={0}/>
23         System.out.println("---------------------------------------------------------");
24         /**
25          * 要想解决"如果要拼接的字符串包含有单引号，那么MessageFormat.format方法就只是把字符串中的单引号去掉，不会将内容填充到指定的占位符中"这个问题，
26          * 那么可以需要使用单引号引起来的字符串中使用2个单引号引起来，例如："<meta http-equiv=''refresh'' content=''3;url={0}''/>"
27          * 这样MessageFormat.format("<meta http-equiv=''refresh'' content=''3;url={0}''/>","index.jsp")就可以正常返回
28          * <meta http-equiv=''refresh'' content=''3;url=index.jsp'/>
29          */
30         String tempStr2 = MessageFormat.format(
31                 "注销成功！！3秒后为您自动跳到登录页面！！<meta http-equiv=''refresh'' content=''3;url={0}''/>", 
32                 request.getContextPath()+"/servlet/LoginUIServlet");
33         /**
34          * 输出结果：
35          * 注销成功！！3秒后为您自动跳到登录页面！！
36          * <meta http-equiv='refresh' content='3;url=/webmvcframework/servlet/LoginUIServlet'/>
37          */
38         System.out.println(tempStr2);
39         
40         String message = String.format(
41                 "注销成功！！3秒后为您自动跳到登录页面！！<meta http-equiv='refresh' content='3;url=%s'/>", 
42                 request.getContextPath()+"/servlet/LoginUIServlet");
43         request.setAttribute("message",message);
44         request.getRequestDispatcher("/message.jsp").forward(request, response);
45     }
46 
47     public void doPost(HttpServletRequest request, HttpServletResponse response)
48             throws ServletException, IOException {
49         doGet(request, response);
50     }
51 
52 }
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　用户登录成功后，会将登录的用户信息存储在session中，所以我们要将存储在session中的user删除掉，这样就可以实现用户注销了。

　　用户登录成功后就会跳转到index.jsp页面，在index.jsp页面中放一个【退出登陆】按钮，当点击【退出登陆】按钮时，就访问LogoutServlet，将用户注销。

　　index.jsp的代码如下：

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

```
 1 <%@ page language="java"  pageEncoding="UTF-8"%>
 2 <%--为了避免在jsp页面中出现java代码，这里引入jstl标签库，利用jstl标签库提供的标签来做一些逻辑判断处理 --%>
 3 <%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
 4 <!DOCTYPE HTML>
 5 <html>
 6   <head>
 7     <title>首页</title>
 8      <script type="text/javascript">
 9         function doLogout(){
10             //访问LogoutServlet注销当前登录的用户
11             window.location.href="${pageContext.request.contextPath}/servlet/LogoutServlet";
12         }
13     </script>
14   </head>
15   
16   <body>
17     <h1>孤傲苍狼的网站</h1>
18     <hr/>
19     <c:if test="${user==null}">
20         <a href="${pageContext.request.contextPath}/servlet/RegisterUIServlet" target="_blank">注册</a>
21         <a href="${pageContext.request.contextPath}/servlet/LoginUIServlet">登陆</a>
22     </c:if>
23     <c:if test="${user!=null}">
24            欢迎您：${user.userName}
25            <input type="button" value="退出登陆" onclick="doLogout()">
26     </c:if>
27     <hr/>
28 </body>
29 </html>
```

[![复制代码](https://common.cnblogs.com/images/copycode.gif)](javascript:void(0);)

　　测试开发好的注销功能，效果如下：

　　![img](https://images0.cnblogs.com/i/289233/201408/161526563422997.gif)

　　到此，所有的功能都开发完成了，测试也通过了。

## 四、开发总结

　　通过这个小例子，可以了解到mvc分层架构的项目搭建，在平时的项目开发中，也都是按照如下的顺序来进行开发的：

　　**1、搭建开发环境**

　　　　1.1 创建web项目

　　　　1.2 导入项目所需的开发包

　　　　1.3 创建程序的包名，在java中是以包来体现项目的分层架构的

　　**2、开发domain**

　　把一张要操作的表当成一个VO类(VO类只定义属性以及属性对应的get和set方法，没有涉及到具体业务的操作方法)，VO表示的是值对象，通俗地说，就是把表中的每一条记录当成一个对象，表中的每一个字段就作为这个对象的属性。每往表中插入一条记录，就相当于是把一个VO类的实例对象插入到数据表中，对数据表进行操作时，都是直接把一个VO类的对象写入到表中，一个VO类对象就是一条记录。每一个VO对象可以表示一张表中的一行记录，VO类的名称要和表的名称一致或者对应。

　　**3、开发dao**

　　　　3.1 DAO操作接口：每一个DAO操作接口规定了，一张表在一个项目中的具体操作方法，此接口的名称最好按照如下格式编写：“I表名称Dao”。

　　　　　　├DAO接口里面的所有方法按照以下的命名编写：

　　　　　　　　├更新数据库：doXxx()

　　　　　　　　├查询数据库：findXxx()或getXxx()

　　　　3.2 DAO操作接口的实现类：实现类中完成具体的增删改查操作

　　　　　　├此实现类完成的只是数据库中最核心的操作，并没有专门处理数据库的打开和关闭，因为这些操作与具体的业务操作无关。

　　**4、开发service(service 对web层提供所有的业务服务)**

　　**5、开发web层**

　　[点击此处下载](http://files.cnblogs.com/xdp-gacl/webmvcframework.zip)项目源码



分类: [JavaWeb学习总结](https://www.cnblogs.com/xdp-gacl/category/574705.html)