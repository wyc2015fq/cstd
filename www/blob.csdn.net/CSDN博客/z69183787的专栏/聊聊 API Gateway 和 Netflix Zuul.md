# 聊聊 API Gateway 和 Netflix Zuul - z69183787的专栏 - CSDN博客
2019年03月19日 18:00:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：90
[http://www.scienjus.com/api-gateway-and-netflix-zuul/](http://www.scienjus.com/api-gateway-and-netflix-zuul/)
最近参与了公司 API Gateway 的搭建工作，技术选型是 Netflix Zuul，主要聊一聊其中的一些心得和体会。
本文主要是介绍使用 Zuul 且在不强制使用其他 Neflix OSS 组件时，如何搭建生产环境的 Gateway，以及能使用 Gateway 做哪些事。不打算介绍任何关于如何快速搭建 Zuul，或是一些轻易集成 Eureka 之类的的方法，这些在官方文档上已经介绍的很明确了。
## API Gateway
API Gateway 是随着微服务（Microservice）这个概念一起兴起的一种架构模式，它用于解决微服务过于分散，没有一个统一的出入口进行流量管理的问题。
用 Kong 官网的两张图来解释再合适不过。
![分散的 API 管理](https://getkong.org/assets/images/homepage/diagram-left.png)
当使用微服务构建整个 API 服务时，一般会有许许多多职责不同的应用在运行着，这些应用会需要一些通用的功能，例如鉴权、流控、监控、日志统计。
在传统的单体应用中，这些功能一般都是内嵌在应用中，作为一个组件运行。但是在微服务模式下，不同种类且独立运行的应用可能会有数十甚至数百种，继续使用这种方式会造成非常高的管理和发布成本。所以就需要在这些应用上抽象出一个统一的流量入口，完成这些功能的实现。
![统一的 API 管理](https://getkong.org/assets/images/homepage/diagram-right.png)
在我看来，API Gateway 的职责主要分为两部分：
- 对服务应用有感知且重要的功能，例如鉴权。
- 对服务应用无感知的边缘服务，例如流控、监控、页面级缓存等。
## Netflix Zuul
对于 API Gateway，常见的选型有基于 Openresty 的 Kong、基于 Go 的 Tyk 和基于 Java 的 Zuul。
这三个选型本身没有什么明显的区别，主要还是看技术栈是否能满足快速应用和二次开发，例如我司原有的技术栈就是使用 Go/Openresty 的平台组和使用 Java 的后端组，讨论后觉得 API Gateway 未来还是处理业务功能的场景更多些，而且后端这边有很多功能可以直接移植过来，最终就选择了 Zuul。
关于 Zuul，大部分使用 Java 做微服务的人可能都会或多或少了解 Spring Cloud 和 Netflix 全家桶。而对于完全不了解的人，可以暂时将它想象为一个类似于 Servlet 中过滤器（Filter）的概念。
![Zuul](https://camo.githubusercontent.com/4eb7754152028cdebd5c09d1c6f5acc7683f0094/687474703a2f2f6e6574666c69782e6769746875622e696f2f7a75756c2f696d616765732f7a75756c2d726571756573742d6c6966656379636c652e706e67)
就像上图中所描述的一样，Zuul 提供了四种过滤器的 API，分别为前置（Pre）、后置（Post）、路由（Route）和错误（Error）四种处理方式。
一个请求会先按顺序通过所有的前置过滤器，之后在路由过滤器中转发给后端应用，得到响应后又会通过所有的后置过滤器，最后响应给客户端。在整个流程中如果发生了异常则会跳转到错误过滤器中。
一般来说，如果需要在请求到达后端应用前就进行处理的话，会选择前置过滤器，例如鉴权、请求转发、增加请求参数等行为。在请求完成后需要处理的操作放在后置过滤器中完成，例如统计返回值和调用时间、记录日志、增加跨域头等行为。路由过滤器一般只需要选择 Zuul 中内置的即可，错误过滤器一般只需要一个，这样可以在 Gateway 遇到错误逻辑时直接抛出异常中断流程，并直接统一处理返回结果。
## 应用场景
以下介绍一些 Zuul 中不同过滤器的应用场景。
### 前置过滤器
鉴权
一般来说整个服务的鉴权逻辑可以很复杂。
- 客户端：App、Web、Backend
- 权限组：用户、后台人员、其他开发者
- 实现：OAuth、JWT
- 使用方式：Token、Cookie、SSO
而对于后端应用来说，它们其实只需要知道请求属于谁，而不需要知道为什么，所以 Gateway 可以友善的帮助后端应用完成鉴权这个行为，并将用户的唯一标示透传到后端，而不需要、甚至不应该将身份信息也传递给后端，防止某些应用利用这些敏感信息做错误的事情。
Zuul 默认情况下在处理后会删除请求的 `Authorization` 头和 `Set-Cookie` 头，也算是贯彻了这个原则。
流量转发
流量转发的含义就是将指向 `/a/xxx.json` 的请求转发到指向 `/b/xxx.json` 的请求。这个功能可能在一些项目迁移、或是灰度发布上会有一些用处。
在 Zuul 中并没有一个很好的办法去修改 Request URI。在某些 [Issue](https://github.com/spring-cloud/spring-cloud-netflix/issues/435) 中开发者会建议设置 `requestURI` 这个属性，但是实际在 Zuul 自身的 `PreDecorationFilter` 流程中又会被覆盖一遍。
不过对于一个基于 Servlet 的应用，使用 `HttpServletRequestWrapper` 基本可以解决一切问题，在这个场景中只需要重写其 `getRequestURI` 方法即可。
class RewriteURIRequestWrapper extends HttpServletRequestWrapper {
  private String rewriteURI;
  public RewriteURIRequestWrapper(HttpServletRequest request, String rewriteURI) {
    super(request);
    this.rewriteURI = rewriteURI;
  }
  @Override
  public String getRequestURI() {
    return rewriteURI;
  }
}
### 后置过滤器
跨域
使用 Gateway 做跨域相比应用本身或是 Nginx 的好处是规则可以配置的更加灵活。例如一个常见的规则。
- 对于任意的 AJAX 请求，返回 `Access-Control-Allow-Origin` 为 `*`，且 `Access-Control-Allow-Credentials` 为 `true`，这是一个常用的允许任意源跨域的配置，但是不允许请求携带任何 Cookie
- 如果一个被信任的请求者需要携带 Cookie，那么将它的 `Origin` 增加到白名单中。对于白名单中的请求，返回 `Access-Control-Allow-Origin` 为该域名，且 `Access-Control-Allow-Credentials` 为 `true`，这样请求者可以正常的请求接口，同时可以在请求接口时携带 Cookie
- 对于 302 的请求，即使在白名单内也必须要设置 `Access-Control-Allow-Origin` 为 `*`，否则重定向后的请求携带的 `Origin` 会为 `null`，有可能会导致 iOS 低版本的某些兼容问题
统计
Gateway 可以统一收集所有应用请求的记录，并写入日志文件或是发到监控系统，相比 Nginx 的 access log，好处主要也是二次开发比较方便，比如可以关注一些业务相关的 HTTP 头，或是将请求参数和返回值都保存为日志打入消息队列中，便于线上故障调试。也可以收集一些性能指标发送到类似 Statsd 这样的监控平台。
### 错误过滤器
错误过滤器的主要用法就像是 Jersey 中的 `ExceptionMapper` 或是 Spring MVC 中的 `@ExceptionHandler` 一样，在处理流程中认为有问题时，直接抛出统一的异常，错误过滤器捕获到这个异常后，就可以统一的进行返回值的封装，并直接结束该请求。
## 配置管理
虽然将这些逻辑都切换到了 Gateway，省去了很多维护和迭代的成本，但是也面临着一个很大的问题，就是 Gateway 只有逻辑却没有配置，它并不知道一个请求要走哪些流程。
例如同样是后端服务 API，有的可能是给网页版用的、有的是给客户端用的，亦或是有的给用户用、有的给管理人员用，那么 Gateway 如何知道到底这些 API 是否需要登录、流控以及缓存呢？
理论上我们可以为 Gateway 编写一个管理后台，里面有当前服务的所有 API，每一个开发者都可以在里面创建新的 API，以及为它增加鉴权、缓存、跨域等功能。为了简化使用，也许我们会额外的增加一个权限组，例如 `/admin/*` 下的所有 API 都应该为后台接口，它只允许内部来源的鉴权访问。
但是这样做依旧太复杂了，而且非常硬编码，当开发者开发了一个新的 API 之后，即使这个应用已经能正常接收特定 URI 的请求并处理之后，却还要通过人工的方式去一个管理后台进行额外的配置，而且可能会因为不谨慎打错了路径中的某个单词而造成不必要的事故，这都是不合理的。
我个人推荐的做法是，在后端应用中依旧保持配置的能力，即使应用里已经没有真实处理的逻辑了。例如在 Java 中通过注解声明式的编写 API，且在应用启动时自动注册 Gateway 就是一种比较好的选择。
/**
 * 这个接口需要鉴权，鉴权方式是 OAuth
 */
@Authorization(OAuth)
@RequestMapping(value = "/users/{id}", method = RequestMethod.DELETE)
public void del(@PathVariable int id) {
  //...  
}
/**
 * 这个接口可以缓存，并且每个 IP/User 每秒最多请求 10 次
 */
@Cacheable
@RateLimiting(limit = "10/1s", scope = {IP, USER})
@RequestMapping(value = "/users/{id}", method = RequestMethod.GET)
public void info(@PathVariable int id) {
  //...  
}
这样 API 的编写者就会根据业务场景考虑该 API 需要哪些功能，也减少了管理的复杂度。
除此之外还会有一些后端应用无关的配置，有些是自动化的，例如恶意请求拦截，Gateway 会将所有请求的信息通过消息队列发送给一些实时数据分析的应用，这些应用会对请求分析，发现恶意请求的特征，并通过 Gateway 提供的接口将这些特征上报给 Gateway，Gateway 就可以实时的对这些恶意请求进行拦截。
## 稳定性
在 Nginx 和后端应用之间又建立了一个 Java 应用作为流量入口，很多人会去担心它的稳定性，亦或是担心它能否像 Nginx 一样和后端的多个 upstream 进行交互，以下主要介绍一下 Zuul 的隔离机制以及重试机制。
### 隔离机制
在微服务的模式下，应用之间的联系变得没那么强烈，理想中任何一个应用超过负载或是挂掉了，都不应该去影响到其他应用。但是在 Gateway 这个层面，有没有可能出现一个应用负载过重，导致将整个 Gateway 都压垮了，已致所有应用的流量入口都被切断？
这当然是有可能的，想象一个每秒会接受很多请求的应用，在正常情况下这些请求可能在 10 毫秒之内就能正常响应，但是如果有一天它出了问题，所有请求都会 Block 到 30 秒超时才会断开（例如频繁 Full GC 无法有效释放内存）。那么在这个时候，Gateway 中也会有大量的线程在等待请求的响应，最终会吃光所有线程，导致其他正常应用的请求也受到影响。
在 Zuul 中，每一个后端应用都称为一个 Route，为了避免一个 Route 抢占了太多资源影响到其他 Route 的情况出现，Zuul 使用 Hystrix 对每一个 Route 都做了隔离和限流。
Hystrix 的隔离策略有两种，基于线程或是基于信号量。Zuul 默认的是基于线程的隔离机制，这意味着每一个 Route 的请求都会在一个固定大小且独立的线程池中执行，这样即使其中一个 Route 出现了问题，也只会是某一个线程池发生了阻塞，其他 Route 不会受到影响。
一般使用 Hystrix 时，只有调用量巨大会受到线程开销影响时才会使用信号量进行隔离策略，对于 Zuul 这种网络请求的用途使用线程隔离更加稳妥。
### 重试机制
一般来说，后端应用的健康状态是不稳定的，应用列表随时会有修改，所以 Gateway 必须有足够好的容错机制，能够减少后端应用变更时造成的影响。
Zuul 的路由主要有 Eureka 和 Ribbon 两种方式，由于我一直使用的都是 Ribbon，所以简单介绍下 Ribbon 支持哪些容错配置。
重试的场景分为三种：
- `okToRetryOnConnectErrors`：只重试网络错误
- `okToRetryOnAllErrors`：重试所有错误
- `OkToRetryOnAllOperations`：重试所有操作（这里不太理解，猜测是 GET/POST 等请求都会重试）
重试的次数有两种：
- `MaxAutoRetries`：每个节点的最大重试次数
- `MaxAutoRetriesNextServer`：更换节点重试的最大次数
一般来说我们希望只在网络连接失败时进行重试、或是对 5XX 的 GET 请求进行重试（不推荐对 POST 请求进行重试，无法保证幂等性会造成数据不一致）。单台的重试次数可以尽量小一些，重试的节点数尽量多一些，整体效果会更好。
如果有更加复杂的重试场景，例如需要对特定的某些 API、特定的返回值进行重试，那么也可以通过实现 `RequestSpecificRetryHandler` 定制逻辑（不建议直接使用 `RetryHandler`，因为这个子类可以使用很多已有的功能）。
