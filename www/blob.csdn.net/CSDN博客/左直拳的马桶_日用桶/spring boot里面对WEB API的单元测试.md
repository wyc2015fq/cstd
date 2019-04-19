# spring boot里面对WEB API的单元测试 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月28日 20:11:33[左直拳](https://me.csdn.net/leftfist)阅读数：36
如前一篇文章（[《spring boot web api》](https://leftfist.blog.csdn.net/article/details/88875738)）所述，spring boot项目里提供web api非常方便。而作为开发人员，自己写的api，首先要自己先测试一轮，通过才能给其他人调用。API就是用来调用的，所以沟通、测试显得特别重要。
程序员测试，当然是单元测试了。下面是一个完整的单元测试代码。待测试的API是POST访问方式的api，有两个：一个提交参数格式为json，一个为键值对。
```java
import api.entity.Author;
import api.entity.EnumSex;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectWriter;
import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.http.MediaType;
import org.springframework.test.context.junit4.SpringRunner;
import org.springframework.test.web.servlet.MockMvc;
import org.springframework.test.web.servlet.request.MockMvcRequestBuilders;
import org.springframework.test.web.servlet.result.MockMvcResultMatchers;
import org.springframework.test.web.servlet.setup.MockMvcBuilders;
import org.springframework.web.context.WebApplicationContext;
import static org.springframework.test.web.servlet.result.MockMvcResultHandlers.print;
@RunWith(SpringRunner.class)
@SpringBootTest
public class MyTest {
    @Autowired
    private WebApplicationContext wac;
    private MockMvc mockMvc;
    
    @Before
    public void setUp() {
        mockMvc = MockMvcBuilders.webAppContextSetup(wac).build();
    }
    @After
    public void after() throws Exception {
        mockMvc = null;
    }
    @Test
    public void testJson() throws Exception {//测试提交参数为JSON格式
        Author req = new Author();//Author是自定义的类
        req.setName("chenqu");
        req.setDesc("foolish");
        req.setSex(EnumSex.Male);
        
        ObjectMapper mapper = new ObjectMapper();
        ObjectWriter ow = mapper.writer().withDefaultPrettyPrinter();
        java.lang.String requestJson = ow.writeValueAsString(req);
        
        mockMvc.perform(MockMvcRequestBuilders.post("/api/authors/json/1?t=108")
                .contentType(MediaType.APPLICATION_JSON).content(requestJson))
                .andDo(print())
                .andExpect(MockMvcResultMatchers.status().isOk())
                .andReturn().getResponse().getContentAsString();
        System.out.println("ok!!!");
    }
    @Test
    public void testKv() throws Exception {//测试提交参数为键值对格式
        Author req = new Author();
        req.setName("chenqu");
        req.setDesc("foolish");
        req.setSex(EnumSex.Male);
        
        ObjectMapper mapper = new ObjectMapper();
        ObjectWriter ow = mapper.writer().withDefaultPrettyPrinter();
        java.lang.String requestJson = ow.writeValueAsString(req);
        
        mockMvc.perform(MockMvcRequestBuilders.post("/api/authors/kv/1?t=108")
                .contentType(MediaType.APPLICATION_FORM_URLENCODED).content(requestJson))
                .andDo(print())
                .andExpect(MockMvcResultMatchers.status().isOk())
                .andReturn().getResponse().getContentAsString();
        System.out.println("ok!!!");
    }
}
```
附API:
```java
@RestController
@RequestMapping(value="/api")
public class ApiController {
	//提交的参数为json
    @RequestMapping(value = "/authors/json/{id}", method = RequestMethod.POST)
    public String test(@PathVariable int id, @RequestParam("t") int type,@RequestBody Author auth){
        return String.format("id:%1$d,type:%2$d,name:%3$s;params type:json",id,type,auth.getName());
    }
	//提交的参数为键值对
    @RequestMapping(value = "/authors/kv/{id}", method = RequestMethod.POST)
    public String test2(@PathVariable int id, @RequestParam("t") int type,Author auth){
        return String.format("id:%1$d,type:%2$d,name:%3$s;params type:key-value",id,type,auth.getName());
    }
	
}
```
