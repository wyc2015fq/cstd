# 【Dubbo】服务引用 - weixin_33985507的博客 - CSDN博客
2019年01月01日 12:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
在springApplication.xml中配置服务引用申明服务接口，我们就可以方便的注入远端的服务代理，通过该代理调用到provider提供的服务。
`<dubbo:reference id="demoService" interface="com.alibaba.dubbo.demo.DemoService" />`
### <dubbo:reference/>解析
```
public class DubboNamespaceHandler extends NamespaceHandlerSupport {
    static {
        Version.checkDuplicate(DubboNamespaceHandler.class);
    }
    public void init() {
......
        registerBeanDefinitionParser("service", new DubboBeanDefinitionParser(ServiceBean.class, true));
        registerBeanDefinitionParser("reference", new DubboBeanDefinitionParser(ReferenceBean.class, false));
    }
}
#com.alibaba.dubbo.config.spring.schema.DubboBeanDefinitionParser#parse
    private static BeanDefinition parse(Element element, ParserContext parserContext, Class<?> beanClass, boolean required) {
        RootBeanDefinition beanDefinition = new RootBeanDefinition();
        beanDefinition.setBeanClass(beanClass);
......
}
```
### 代理创建时机
解析service标签后spring会自动创建ServiceBean的实例,在`ServiceBean`的继承体系中，可以知道它实现了`FactoryBean`和`InitializingBean`接口
![2345741-39fa50bb709c49cf.png](https://upload-images.jianshu.io/upload_images/2345741-39fa50bb709c49cf.png)
ReferenceBean
消费者只有服务端的接口，如果要调用远程服务就需要给该接口创建一个动态代理，而创建代理只能在FactoryBean.getObject时创建，因为InitializingBean.afterPropertySet会在实例初始化之后调用，这时候实例已经生成了
![2345741-ad9422a84634751d.png](https://upload-images.jianshu.io/upload_images/2345741-ad9422a84634751d.png)
getObject调用栈信息
![2345741-2b00295ab7b40e7f.png](https://upload-images.jianshu.io/upload_images/2345741-2b00295ab7b40e7f.png)
afterPropertySet.png
### 创建代理的过程
首先从配置中心拿到provider的地址，然后构建成invoker，使用invoker来创建代理
### 执行流程
```
ReferenceBean.getObject()
  -->ReferenceConfig.get()
    -->init()
      -->createProxy(map)
        -->refprotocol.refer(interfaceClass, urls.get(0))
          -->ExtensionLoader.getExtensionLoader(Protocol.class).getExtension("registry");
          -->extension.refer(arg0, arg1);
            -->ProtocolFilterWrapper.refer
              -->RegistryProtocol.refer
                -->registryFactory.getRegistry(url)//建立zk的连接，和服务端发布一样（省略代码）
                -->doRefer(cluster, registry, type, url)
                  -->registry.register//创建zk的节点，和服务端发布一样（省略代码）。节点名为：dubbo/com.alibaba.dubbo.demo.DemoService/consumers
                  -->registry.subscribe//订阅zk的节点，和服务端发布一样（省略代码）。   /dubbo/com.alibaba.dubbo.demo.DemoService/providers, 
                                                                        /dubbo/com.alibaba.dubbo.demo.DemoService/configurators,
                                                                         /dubbo/com.alibaba.dubbo.demo.DemoService/routers]
                    -->notify(url, listener, urls);
                      -->FailbackRegistry.notify
                        -->doNotify(url, listener, urls);
                          -->AbstractRegistry.notify
                            -->saveProperties(url);//把服务端的注册url信息更新到C:\Users\bobo\.dubbo\dubbo-registry-192.168.48.117.cache
                              -->registryCacheExecutor.execute(new SaveProperties(version));//采用线程池来处理
                            -->listener.notify(categoryList)
                              -->RegistryDirectory.notify
                                -->refreshInvoker(invokerUrls)//刷新缓存中的invoker列表
                                  -->destroyUnusedInvokers(oldUrlInvokerMap,newUrlInvokerMap); // 关闭未使用的Invoker
                                  -->最终目的：刷新Map<String, Invoker<T>> urlInvokerMap 对象
                                                                                                                           刷新Map<String, List<Invoker<T>>> methodInvokerMap对象
                  -->cluster.join(directory)//加入集群路由
                    -->ExtensionLoader.getExtensionLoader(com.alibaba.dubbo.rpc.cluster.Cluster.class).getExtension("failover");
                      -->MockClusterWrapper.join
                        -->this.cluster.join(directory)
                          -->FailoverCluster.join
                            -->return new FailoverClusterInvoker<T>(directory)
                            -->new MockClusterInvoker
        -->proxyFactory.getProxy(invoker)//创建服务代理
          -->ProxyFactory$Adpative.getProxy
            -->ExtensionLoader.getExtensionLoader(com.alibaba.dubbo.rpc.ProxyFactory.class).getExtension("javassist");
              -->StubProxyFactoryWrapper.getProxy
                -->proxyFactory.getProxy(invoker)
                  -->AbstractProxyFactory.getProxy
                    -->getProxy(invoker, interfaces)
                      -->Proxy.getProxy(interfaces)//目前代理对象interface com.alibaba.dubbo.demo.DemoService, interface com.alibaba.dubbo.rpc.service.EchoService
                      -->InvokerInvocationHandler// 采用jdk自带的InvocationHandler，创建InvokerInvocationHandler对象。
```
### 详细步骤
- FactoryBean.getObject
spring将调用getObject方法返回的对象注册容器中，其中调用的`get`方法由父类`ReferenceConfig`实现
```
public class ReferenceBean<T> extends ReferenceConfig<T> implements FactoryBean, ApplicationContextAware, InitializingBean, DisposableBean {
public Object getObject() throws Exception {
        return get();
    }
}
```
- 解析配置属性
将标签的配置属性解析到map中
```
#com.alibaba.dubbo.config.ReferenceConfig#get
    public synchronized T get() {
        if (destroyed){
            throw new IllegalStateException("Already destroyed!");
        }
        if (ref == null) {
            init();
        }
        return ref;
    }
private void init() {
//将配置属性解析到map中
......
//创建代理类
        ref = createProxy(map);
}
```
- 从注册中心获取provider服务的地址生成invoker对象并创建代理类
```
private T createProxy(Map<String, String> map) {
if (isJvmRefer) {
            URL url = new URL(Constants.LOCAL_PROTOCOL, NetUtils.LOCALHOST, 0, interfaceClass.getName()).addParameters(map);
            invoker = refprotocol.refer(interfaceClass, url);
            if (logger.isInfoEnabled()) {
                logger.info("Using injvm service " + interfaceClass.getName());
            }
        } else {
if (urls.size() == 1) {
                //根据接口获取远端服务提供者的invoker对象
                invoker = refprotocol.refer(interfaceClass, urls.get(0));
            } else {
                List<Invoker<?>> invokers = new ArrayList<Invoker<?>>();
                URL registryURL = null;
                for (URL url : urls) {
                    invokers.add(refprotocol.refer(interfaceClass, url));
                    if (Constants.REGISTRY_PROTOCOL.equals(url.getProtocol())) {
                        registryURL = url; // 用了最后一个registry url
                    }
                }
                if (registryURL != null) { // 有 注册中心协议的URL
                    // 对有注册中心的Cluster 只用 AvailableCluster
                    URL u = registryURL.addParameter(Constants.CLUSTER_KEY, AvailableCluster.NAME); 
                    invoker = cluster.join(new StaticDirectory(u, invokers));
                }  else { // 不是 注册中心的URL
                    invoker = cluster.join(new StaticDirectory(invokers));
                }
            }
// 创建服务代理
        return (T) proxyFactory.getProxy(invoker);
}
```
- 生成invoker
`invoker = refprotocol.refer(interfaceClass, url);`
```
public <T> Invoker<T> refer(Class<T> type, URL url) throws RpcException {
      ......
        //获取registry对象 获取之后会做缓存key=zookeeper://192.168.99.100:2181/com.alibaba.dubbo.registry.RegistryService
        Registry registry = registryFactory.getRegistry(url);
        if (RegistryService.class.equals(type)) {
            return proxyFactory.getInvoker((T) registry, type, url);
        }
        ......
        return doRefer(cluster, registry, type, url);
    }
private <T> Invoker<T> doRefer(Cluster cluster, Registry registry, Class<T> type, URL url) {
        RegistryDirectory<T> directory = new RegistryDirectory<T>(type, url);
        directory.setRegistry(registry);
        directory.setProtocol(protocol);
        URL subscribeUrl = new URL(Constants.CONSUMER_PROTOCOL, NetUtils.getLocalHost(), 0, type.getName(), directory.getUrl().getParameters());
        if (! Constants.ANY_VALUE.equals(url.getServiceInterface())
                && url.getParameter(Constants.REGISTER_KEY, true)) {
            registry.register(subscribeUrl.addParameters(Constants.CATEGORY_KEY, Constants.CONSUMERS_CATEGORY,
                    Constants.CHECK_KEY, String.valueOf(false)));
        }
        //directory内部包含一个registry，同时实现了NotifyListener接口订阅完成后会回其方法notify方法来刷新invoker列表
        directory.subscribe(subscribeUrl.addParameter(Constants.CATEGORY_KEY, 
                Constants.PROVIDERS_CATEGORY 
                + "," + Constants.CONFIGURATORS_CATEGORY 
                + "," + Constants.ROUTERS_CATEGORY));
        return cluster.join(directory);
    }
#com.alibaba.dubbo.registry.integration.RegistryDirectory#subscribe
    public void subscribe(URL url) {
        setConsumerUrl(url);
        registry.subscribe(url, this);
    }
```
在调用`directory.subscribe`进行订阅的时候传入的listener参数是RegistryDirectory类的对象，它实现了NotifyListener接口，在订阅完成之后会回调其notify方法,在这个方法中会从urls解析注册中心的配置，然后重新刷新invoker，而且每次订阅的节点属性变更都会回调这个方法
```
public synchronized void notify(List<URL> urls) {
        List<URL> invokerUrls = new ArrayList<URL>();
        List<URL> routerUrls = new ArrayList<URL>();
        List<URL> configuratorUrls = new ArrayList<URL>();
        for (URL url : urls) {
            String protocol = url.getProtocol();
            String category = url.getParameter(Constants.CATEGORY_KEY, Constants.DEFAULT_CATEGORY);
            if (Constants.ROUTERS_CATEGORY.equals(category) 
                    || Constants.ROUTE_PROTOCOL.equals(protocol)) {
                routerUrls.add(url);
            } else if (Constants.CONFIGURATORS_CATEGORY.equals(category) 
                    || Constants.OVERRIDE_PROTOCOL.equals(protocol)) {
                configuratorUrls.add(url);
            } else if (Constants.PROVIDERS_CATEGORY.equals(category)) {
                invokerUrls.add(url);
            } else {
                logger.warn("Unsupported category " + category + " in notified url: " + url + " from registry " + getUrl().getAddress() + " to consumer " + NetUtils.getLocalHost());
            }
        }
        // providers
        //刷新invoker
        refreshInvoker(invokerUrls);
    }
private void refreshInvoker(List<URL> invokerUrls){
        if (invokerUrls != null && invokerUrls.size() == 1 && invokerUrls.get(0) != null
                && Constants.EMPTY_PROTOCOL.equals(invokerUrls.get(0).getProtocol())) {
            if (invokerUrls != null && invokerUrls.size() == 1 && invokerUrls.get(0) != null
                && Constants.EMPTY_PROTOCOL.equals(invokerUrls.get(0).getProtocol())) {
            this.forbidden = true; // 禁止访问
            this.methodInvokerMap = null; // 置空列表
            destroyAllInvokers(); // 关闭所有Invoker
        } else {
        ......
            Map<String, Invoker<T>> newUrlInvokerMap = toInvokers(invokerUrls) ;// 将URL列表转成Invoker列表
Map<String, List<Invoker<T>>> newMethodInvokerMap = toMethodInvokers(newUrlInvokerMap); // 换方法名映射Invoker列表
            // state change
            //如果计算错误，则不进行处理.
            if (newUrlInvokerMap == null || newUrlInvokerMap.size() == 0 ){
                logger.error(new IllegalStateException("urls to invokers error .invokerUrls.size :"+invokerUrls.size() + ", invoker.size :0. urls :"+invokerUrls.toString()));
                return ;
            }
            this.methodInvokerMap = multiGroup ? toMergeMethodInvokerMap(newMethodInvokerMap) : newMethodInvokerMap;
            //刷新invoker
            this.urlInvokerMap = newUrlInvokerMap;
......
    }
    private Map<String, Invoker<T>> toInvokers(List<URL> urls) {
                        invoker = new InvokerDelegete<T>(protocol.refer(serviceType, url), url, providerUrl);
...
}
```
最终通过DubboProtocol. refer创建Invoker并加入缓存中
```
public <T> Invoker<T> refer(Class<T> serviceType, URL url) throws RpcException {
        // create rpc invoker. 引用远端服务
        DubboInvoker<T> invoker = new DubboInvoker<T>(serviceType, url, getClients(url), invokers);
        invokers.add(invoker);
        return invoker;
    }
```
- 
invoker包装
cluster.join(directory);
包装invoker并返回自己的invoker对象，外部调用invoker.invoke方法的时候会从directory中获取invoker列表，用于实现重试快速失败操作；这里使用了MockClusterWrapper、FailoverCluster两种包装
![2345741-0921b1dce8ad936b.png](https://upload-images.jianshu.io/upload_images/2345741-0921b1dce8ad936b.png)
image.png
```
public class MockClusterWrapper implements Cluster {
    private Cluster cluster;
    public MockClusterWrapper(Cluster cluster) {
        this.cluster = cluster;
    }
    public <T> Invoker<T> join(Directory<T> directory) throws RpcException {
        return new MockClusterInvoker<T>(directory,
                this.cluster.join(directory));
    }
}
```
- 创建代理
`proxyFactory.getProxy(invoker)`创建动态代理的过程就是根据需要引用的interface使用Javassist进行字节码操作生成一个代理类，而且其构造方法的参数为InvocationHandler，最后将这个代理类加入spring容器。这样当我们调用代理类方法的时候会回调InvocationHandler.invoke->invoker.invoke方法来，调用invoker远端的实现。
```
public class JavassistProxyFactory extends AbstractProxyFactory {
    @SuppressWarnings("unchecked")
    public <T> T getProxy(Invoker<T> invoker, Class<?>[] interfaces) {
        //(T) Proxy.getProxy(interfaces)动态拼装成接口的代理类,并使用Javassist编译
        return (T) Proxy.getProxy(interfaces).newInstance(new InvokerInvocationHandler(invoker));
    }
}
public class InvokerInvocationHandler implements InvocationHandler {
    private final Invoker<?> invoker;
    public InvokerInvocationHandler(Invoker<?> handler){
        this.invoker = handler;
    }
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        String methodName = method.getName();
        Class<?>[] parameterTypes = method.getParameterTypes();
      ......
        //调用远端服务
        return invoker.invoke(new RpcInvocation(method, args)).recreate();
    }
}
```
### 总结
- Directory：目录服务
- StaticDirectory：静态目录服务，他的Invoker是固定的。
- 
RegistryDirectory：注册目录服务，他的Invoker集合数据来源于zk注册中心的，他实现了NotifyListener接口，并且实现回调notify(List<URL> urls), 整个过程有一个重要的map变量，methodInvokerMap（它是数据的来源；同时也是notify的重要操作对象，重点是写操作。）
![2345741-be339f984c575b95.png](https://upload-images.jianshu.io/upload_images/2345741-be339f984c575b95.png)
服务引用的流程
