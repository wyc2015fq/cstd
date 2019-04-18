# BeanFactoryPostProcessor和BeanPostProcessor的区别 - z69183787的专栏 - CSDN博客
2015年05月22日 13:04:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：898
BeanFactoryPostProcessor和BeanPostProcessor都是Spring初始化bean的扩展点。两个接口非常相似。
BeanFactoryPostProcessor可以对bean的定义（配置元数据）进行处理。也就是说，Spring IoC容器允许BeanFactoryPostProcessor在容器实际实例化任何其它的bean之前读取配置元数据，并有可能修改它。如果你愿意，你可以配置多个BeanFactoryPostProcessor。你还能通过设置'order'属性来控制BeanFactoryPostProcessor的执行次序。
注册BeanFactoryPostProcessor的实例，需要重载void postProcessBeanFactory(ConfigurableListableBeanFactory beanFactory) throws BeansException;
通过beanFactory可以获取bean的示例或定义等。同时可以修改bean的属性，这是和BeanPostProcessor最大的区别。
例如：
BeanDefinition bd = beanFactory.getBeanDefinition("xxBean");  
MutablePropertyValues mpv =  bd.getPropertyValues();  
if(pv.contains("xxName"))  {  
    pv.addPropertyValue("xxName", "icoder");  
}
注册BeanPostProcessor的实例，需要重载
Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException;
和
Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException;
还有一点区别就是BeanFactoryPostProcessor的回调比BeanPostProcessor要早。
