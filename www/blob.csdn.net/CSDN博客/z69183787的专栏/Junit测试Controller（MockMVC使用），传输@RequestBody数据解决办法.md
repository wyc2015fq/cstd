# Junit测试Controller（MockMVC使用），传输@RequestBody数据解决办法 - z69183787的专栏 - CSDN博客
2017年06月16日 17:06:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1674
一、单元测试的目的
　　简单来说就是在我们增加或者改动一些代码以后对所有逻辑的一个检测，尤其是在我们后期修改后（不论是增加新功能，修改bug），都可以做到**重新测试的工作**。以减少我们在发布的时候出现更过甚至是出现之前解决了的问题再次重现。
　　这里主要是使用MockMvc对我们的系统的Controller进行单元测试。
　　对数据库的操作使用事务实现回滚，及对数据库的增删改方法结束后将会还远数据库。
二、MockMvc的使用
1、首先我们上一个例子，
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
import org.apache.commons.logging.Log;
import org.apache.commons.logging.LogFactory;
import org.junit.Before;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.MediaType;
import org.springframework.test.context.ContextConfiguration;
import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;
import org.springframework.test.context.transaction.TransactionConfiguration;
import org.springframework.test.context.web.WebAppConfiguration;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.RequestBuilder;
import org.springframework.test.web.servlet.ResultActions;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.web.context.WebApplicationContext;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.get;
import static org.springframework.test.web.servlet.request.MockMvcRequestBuilders.post;
import static org.springframework.test.web.servlet.result.MockMvcResultHandlers.print;
import static org.springframework.test.web.servlet.result.MockMvcResultMatchers.status;
/**
 * Created by zhengcanrui on 16/8/11.
 */
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration(locations={"classpath:spring/applicationContext-*xml"})
//配置事务的回滚,对数据库的增删改都会回滚,便于测试用例的循环利用
@TransactionConfiguration(transactionManager = "transactionManager", defaultRollback = true)
@Transactional
@WebAppConfiguration
public class Test {
    //记得配置log4j.properties ,的命令行输出水平是debug
    protected Log logger= LogFactory.getLog(TestBase.class);
    protected MockMvc mockMvc;
    @Autowired
    protected WebApplicationContext wac;
    @Before()  //这个方法在每个方法执行之前都会执行一遍
    public void setup() {
        mockMvc = MockMvcBuilders.webAppContextSetup(wac).build();  //初始化MockMvc对象
    }
    @org.junit.Test
    public void getAllCategoryTest() throws Exception {
        String responseString = mockMvc.perform(
                get("/categories/getAllCategory")    //请求的url,请求的方法是get
                        .contentType(MediaType.APPLICATION_FORM_URLENCODED)  //数据的格式
　　　　　　　　　　　　　　 .param("pcode","root")         //添加参数
        ).andExpect(status().isOk())    //返回的状态是200
                .andDo(print())         //打印出请求和相应的内容
                .andReturn().getResponse().getContentAsString();   //将相应的数据转换为字符串
        System.out.println("--------返回的json = " + responseString);
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　Spring MVC的测试往往看似比较复杂。其实他的不同在于，他需要一个ServletContext来模拟我们的请求和响应。但是Spring也针对Spring MVC 提供了请求和响应的模拟测试接口，以方便我们的单元测试覆盖面不只是service，dao层。
2、代码解释：
**@webappconfiguration是一级注释，用于声明一个ApplicationContext集成测试加载WebApplicationContext。作用是模拟ServletContext**
@ContextConfiguration：因为controller，component等都是使用注解，需要注解指定spring的配置文件，扫描相应的配置，将类初始化等。
```
@TransactionConfiguration(transactionManager = "transactionManager", defaultRollback = true)
@Transactional
```
上面两句的作用是，让我们对数据库的操作会事务回滚，如对数据库的添加操作，在方法结束之后，会撤销我们对数据库的操作。
为什么要事务回滚?
- 测试过程对数据库的操作，会产生脏数据，影响我们数据的正确性
- 不方便循环测试，即假如这次我们将一个记录删除了，下次就无法再进行这个Junit测试了，因为该记录已经删除，将会报错。
- 如果不使用事务回滚，我们需要在代码中显式的对我们的增删改数据库操作进行恢复，将多很多和测试无关的代码 
方法解析：
- **perform**：**执行一个RequestBuilder请求**，会自动执行SpringMVC的流程并映射到相应的控制器执行处理；
- **get**:声明发送一个get请求的方法。MockHttpServletRequestBuilder get(String urlTemplate, Object... urlVariables)：根据uri模板和uri变量值得到一个GET请求方式的。另外提供了其他的请求的方法，如：post、put、delete等。
- **param**：添加request的参数，如上面发送请求的时候带上了了pcode = root的参数。假如使用需要**发送json数据格式**的时将不能使用这种方式，可见后面**被@ResponseBody注解参数的解决方法**
- **andExpect**：添加ResultMatcher验证规则，验证控制器执行完成**后**结果是否正确（对返回的数据进行的判断）；
- **andDo**：添加ResultHandler结果处理器，比如调试时打印结果到控制台（对返回的数据进行的判断）；
- **andReturn**：最后返回相应的MvcResult；然后进行自定义验证/进行下一步的异步处理（对返回的数据进行的判断）；
注意事项：
- 在mac上使用log4j是，假如使用了${catalina.home}需要注意，mac不会去找到tomcat所在的路径，直接回到根路径 “/”，而正常情况下，根路径是没有写权限的，需要使用管理员赋权限。
- log4j在配置完成之后，需要设置起打印日志的级别，假如没有设置，在Junit中，将无法打印日志。 
3、后台的返回数据中，最好带上我们对数据库的修改的结果返回的前端。
为什么要在data中返回一个修改或者添加的对象
- 将数据返回给前端，前端容易判断数据是否添加或者修改成功
- 更新或者添加完数据经常需要刷新页面，将数据直接给了前端，前端不用再发一个请求来获取
- 单元测试的时候，能对数据库的DDL（增删改）操作的时候，我们能对数据进行审核，从何判断我们的操作是否是成功的。如下面的例子：
我们发送一个添加操作，添加一个SoftInfo对象，SoftInfo类定义如下：
```
public class SoftInfo {
    private String id;
    private String name;
}
```
添加完之后，由于我们进行了单元测试的**事务回滚**，我们将不能再数据库中看我们我们的的添加操作，**无法判断操作是否成功**。
为了解决上面的问题，我们可以在返回的json的数据中添加一个“data”字段，解析该json中的data字段数据，判断我们的添加操作是否成功的。json格式如下：
```
{
    "status":200,
    "data":{"id":"2","name":"测试"}
}
```
我们可以使用andExpect方法对返回的数据进行判断，用“$.属性”获取里面的数据，如我要获取返回数据中的"data.name"，可以写成"$.data.name"。下面的例子是判断返回的data.name=“测试”。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@Test
    public void testCreateSeewoAccountUser() throws Exception {
        mockMvc.perform(post("/users")
                        .contentType(MediaType.APPLICATION_FORM_URLENCODED)  
        ).andExpect(status().isOk())
        .andExpect(jsonPath("$.data.name", is("测试"))))  
        .andExpect(jsonPath("$.data.createTime", notNullValue()))
        ;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
三、遇到的问题
1、发送一个被@ResponseBody标识的参数，一直到400错误。 即无法发送一个json格式的数据到Controller层。
解决方法1：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
　　　　  SoftInfo softInfo = new SoftInfo();
　　　　　　//设置值
　　　　　ObjectMapper mapper = new ObjectMapper();
        ObjectWriter ow = mapper.writer().withDefaultPrettyPrinter();
        java.lang.String requestJson = ow.writeValueAsString(softInfo);
        String responseString = mockMvc.perform( post("/softs").contentType(MediaType.**APPLICATION_JSON**).content(requestJson)).andDo(print())
                .andExpect(status().isOk()).andReturn().getResponse().getContentAsString();
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
 解决方法2：使用com.alibaba.fastjson.JSONObject将对象转换为Json数据
```
SoftInfo softInfo = new SoftInfo();
//。。。设置值
　　　　String requestJson = JSONObject.toJSONString(folderInfo);
        String responseString = mockMvc.perform( post("/softs").contentType(**MediaType.APPLICATION_JSON**).content(requestJson)).andDo(print())
                .andExpect(status().isOk()).andReturn().getResponse().getContentAsString();
```
　　注意上面contentType需要设置成MediaType.APPLICATION_JSON，即声明是发送“application/json”格式的数据。使用content方法，将转换的json数据放到request的body中。
2、java.lang.NoClassDefFoundError: com/jayway/jsonpath/InvalidPathException
缺少了jar包：
可以添加一下的maven依赖
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 　　　　<dependency>
            <groupId>com.jayway.jsonpath</groupId>
            <artifactId>json-path</artifactId>
            <version>0.8.1</version>
            <scope>test</scope>
        </dependency>
        <dependency>
            <groupId>com.jayway.jsonpath</groupId>
            <artifactId>json-path-assert</artifactId>
            <version>0.8.1</version>
            <scope>test</scope>
        </dependency>
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
学习链接：[https://www.petrikainulainen.net/spring-mvc-test-tutorial/](https://www.petrikainulainen.net/spring-mvc-test-tutorial/%20)
