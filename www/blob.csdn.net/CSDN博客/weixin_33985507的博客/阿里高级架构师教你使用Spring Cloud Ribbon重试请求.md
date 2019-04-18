# 阿里高级架构师教你使用Spring Cloud Ribbon重试请求 - weixin_33985507的博客 - CSDN博客
2018年09月06日 12:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
在微服务调用中，一些微服务圈可能调用失败，通过再次调用以达到系统稳定性效果，本文展示如何使用Ribbon和Spring Retry进行请求再次重试调用。
在Spring Cloud中，使用load balanced RestTemplate对外部服务进行请求调用，创建一个负载平衡的RestTemplate是非常简单：
> 
@Configuration
public class MyConfiguration {
  @LoadBalanced
  @Bean
  RestTemplate restTemplate() {
    return new RestTemplate();
  }
}
public class MyClass {
  @Autowired
  private RestTemplate restTemplate;
  public String doOtherStuff() {
    String results = restTemplate.getForObject("http://stores/stores", String.class);
    return results;
  }
}
 这里加入@LoadBalanced 元注解让你利用一个发现服务比如Eureka进行服务注册，正如你在doOtherStuff方法中看到，我们对URL http://stores/stores发出请求，主机名stores并不是实际注册的主机名，而是在发现服务器中注册的stroes服务名称，RestTemplate将使用stores服务所在主机名或IP地址替代stores主机名。
正如我们都知道，这样的请求可能有问题。 出于任何原因，某些可能会出错，请求可能会失败。 这就是为什么在健壮的应用程序中，当遇到失败时，可以重试类似上面的API请求。 发生这种失败问题原因可能是服务完全停止，我们永远不会得到回应。 然而，故障可能的是由于某种类型的网络问题，并且随后的请求可能会成功。 甚至可能是服务的某个特定实例可能遇到问题的情况，但是存在完全可能是请求另一实例时却能够处理。
在Spring Cloud，如果你使用一个负载平衡RESTTemplate进行API请求，如果请求失败，开发者可以决定重试请求，在 Spring Retry project 项目中提供重试逻辑，使用 Ribbon properties 配置重试次数以及哪个请求需要重试。
大家可以点击加入群：【Java高级架构进阶群】：854180697 里面有Java高级大牛直播讲解知识点 走的就是高端路线，（如果你想跳槽换工作 但是技术又不够 或者工作上遇到了瓶颈 ，我这里有一个JAVA的免费直播课程 ，讲的是高端的知识点基础不好的误入哟，只要你有1-5年的开发经验可以加群找我要课堂链接 注意：是免费的 没有开发经验误入哦）
写在最后：欢迎留言讨论，加关注，持续更新！！！
