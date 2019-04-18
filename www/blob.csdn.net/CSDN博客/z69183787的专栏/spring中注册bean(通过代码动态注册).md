# spring中注册bean(通过代码动态注册) - z69183787的专栏 - CSDN博客
2016年05月04日 06:32:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8520

**[java]**[view
 plain](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
[print](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[?](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
- //将applicationContext转换为ConfigurableApplicationContext
- ConfigurableApplicationContext configurableApplicationContext = (ConfigurableApplicationContext) applicationContext;  
- 
- // 获取bean工厂并转换为DefaultListableBeanFactory
- DefaultListableBeanFactory defaultListableBeanFactory = (DefaultListableBeanFactory) configurableApplicationContext  
-         .getBeanFactory();  
- 
- // 通过BeanDefinitionBuilder创建bean定义
- BeanDefinitionBuilder beanDefinitionBuilder = BeanDefinitionBuilder  
-         .genericBeanDefinition(UserService.class);  
- // 设置属性userAcctDAO,此属性引用已经定义的bean:userAcctDAO
- beanDefinitionBuilder  
-         .addPropertyReference("userAcctDAO", "UserAcctDAO");  
- 
- // 注册bean
- defaultListableBeanFactory.registerBeanDefinition("sdfds",  
-         beanDefinitionBuilder.getRawBeanDefinition());  
User.java
**[java]**[view
 plain](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
[print](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[?](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
- publicclass User {  
- 
- private String username;  
- private String password;  
- 
- public String getUsername() {  
- return username;  
-     }  
- 
- publicvoid setUsername(String username) {  
- this.username = username;  
-     }  
- 
- public String getPassword() {  
- return password;  
-     }  
- 
- publicvoid setPassword(String password) {  
- this.password = password;  
-     }  
- 
- }  
UserService.java
**[java]**[view
 plain](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
[print](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[?](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
- publicclass UserService {  
- 
- private User user;  
- 
- 
- publicvoid test(){  
-         System.out.println(user.getUsername()+"--------------------");  
-     }  
- 
- public User getUser() {  
- return user;  
-     }  
- 
- 
- 
- publicvoid setUser(User user) {  
- this.user = user;  
-     }  
- }  
示例:
BeanFactoryAwareBean.java
**[java]**[view
 plain](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
[print](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[?](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
- import org.springframework.beans.BeansException;  
- import org.springframework.beans.factory.BeanFactory;  
- import org.springframework.beans.factory.BeanFactoryAware;  
- import org.springframework.beans.factory.support.BeanDefinitionBuilder;  
- import org.springframework.beans.factory.support.DefaultListableBeanFactory;  
- import org.springframework.context.ApplicationListener;  
- import org.springframework.context.event.ContextRefreshedEvent;  
- 
- publicclass BeanFactoryAwareBean implements BeanFactoryAware,  
-         ApplicationListener<ContextRefreshedEvent> {  
- private DefaultListableBeanFactory beanFactory;  
- 
- publicvoid setBeanFactory(BeanFactory beanFactory) throws BeansException {  
-         System.out.println("setBeanFactory.........................");  
- this.beanFactory = (DefaultListableBeanFactory) beanFactory;  
-     }  
- 
- publicvoid onApplicationEvent(ContextRefreshedEvent contextRefreshedEvent) {  
-         System.out.println("ContextRefreshed...................");  
- 
-         BeanDefinitionBuilder userBeanDefinitionBuilder = BeanDefinitionBuilder  
-                 .genericBeanDefinition(User.class);  
-         userBeanDefinitionBuilder.addPropertyValue("username", "chinfeng");  
-         userBeanDefinitionBuilder.addPropertyValue("password", "123456");  
-         beanFactory.registerBeanDefinition("user",  
-                 userBeanDefinitionBuilder.getRawBeanDefinition());  
- 
-         BeanDefinitionBuilder usersBeanDefinitionBuilder = BeanDefinitionBuilder  
-                 .genericBeanDefinition(UserService.class);  
-         usersBeanDefinitionBuilder.addPropertyReference("user", "user");  
-         beanFactory.registerBeanDefinition("userService",  
-                 usersBeanDefinitionBuilder.getRawBeanDefinition());  
-     }  
- }  
applicationContext.xml
**[html]**[view
 plain](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[copy](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
[print](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)[?](http://blog.csdn.net/buyaore_wo/article/details/8119577?utm_source=tuicool&utm_medium=referral#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:p="http://www.springframework.org/schema/p"
- xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.0.xsd">
- <beanid="beanFactoryAwareBean"class="BeanFactoryAwareBean"></bean>
- </beans>
结果:
setBeanFactory.........................
ContextRefreshed...................
chinfeng--------------------
