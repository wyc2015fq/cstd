# 访问spring boot项目提供的web api提示跨域问题 - 左直拳的马桶_日用桶 - CSDN博客
2019年03月28日 20:21:52[左直拳](https://me.csdn.net/leftfist)阅读数：42
天杀的，没天理啊！本机的api，居然提示跨域！
如前两篇文章
《[spring boot web api](https://leftfist.blog.csdn.net/article/details/88875738)》
《[spring boot里面对WEB API的单元测试](https://leftfist.blog.csdn.net/article/details/88876933)》
所述的api，看上去头头是道，但我在前端用js刚开始测试的时候，却提示跨域。
前端代码如下：
```
var url1 = "http://localhost:8085/api/authors/kv/1?t=2";
var data1 = "name=chenqu&desc=foolish";
var ctype1 = "application/x-www-form-urlencoded;charset=utf-8";
var url2 = "http://localhost:8085/api/authors/json/1?t=2";
var data2 = JSON.stringify({
	"name":"chenqu",
	"desc":"foolish"
});
var ctype2 = "application/json; charset=utf-8";
$.ajax({
	url: url2,
	data: data2,
	contentType: ctype2,		
	//dataType: "json",
	type: "POST",
	success: function (data) {
		alert(data);
	},
	error: function (rq, status, thrown) {
		alert(rq.responseText + "," + status + ": " + thrown);
	}
});
```
就本文涉及的案例，唯有在服务器端修改一下。从网上抄来一个例子，将代码加进去就可以了：
```java
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.http.HttpHeaders;
import org.springframework.web.servlet.config.annotation.CorsRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurer;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;
@Configuration
public class WebConfig {
    /**
     * 跨域请求支持
     */
    @Bean
    public WebMvcConfigurer corsConfigurer() {
        return new WebMvcConfigurerAdapter() {
            @Override
            public void addCorsMappings(CorsRegistry registry) {
                registry.addMapping("/**").allowedOrigins("*")
                        .allowedMethods("*").allowedHeaders("*")
                        .allowCredentials(true)
                        .exposedHeaders(HttpHeaders.SET_COOKIE).maxAge(3600L);
            }
        };
    }
}
```
也无须调用。应该是头部有个注解：`@Configuration`，自动生效了吧。
