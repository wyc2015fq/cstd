# 【SpringBoot】web加载地址默认跳转设置 - Big Smile - CSDN博客
2019年03月19日 22:21:38[王啸tr1912](https://me.csdn.net/tr1912)阅读数：50
        我们在设置完springboot项目之后，需要让他默认跳转一个地址，需要怎么做呢？答案很是简单，可以通过springboot-web的configuration配置，把默认的'/' 路径的路由，转换为你想要指定的controller的view跳转地址就行了。
        首先我们需要先构造一个主页的controller访问方法：
```java
@RequestMapping("/index")
public ModelAndView index() {
    ModelAndView mv = new ModelAndView();
    mv.setViewName("/index");
    return mv;
}
```
然后配置一个配置类，连接到这个地址：
```java
import org.springframework.context.annotation.Configuration;
import org.springframework.core.Ordered;
import org.springframework.web.servlet.config.annotation.ViewControllerRegistry;
import org.springframework.web.servlet.config.annotation.WebMvcConfigurerAdapter;
@Configuration
public class DefaultView extends WebMvcConfigurerAdapter {
    @Override
    public void addViewControllers(ViewControllerRegistry registry) {
        registry.addViewController("/").setViewName("forward:/index");
        registry.setOrder(Ordered.HIGHEST_PRECEDENCE);
        super.addViewControllers(registry);
    } 
}
```
这个路由，就是把  ‘/’  的访问路由跳转到了  ‘/index’   上，从而完成了不用输入controllermapping路径，至今自动跳转。
