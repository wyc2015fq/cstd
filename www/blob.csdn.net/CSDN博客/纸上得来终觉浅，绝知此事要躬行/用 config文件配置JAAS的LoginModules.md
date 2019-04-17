# 用*.config文件配置JAAS的LoginModules - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年03月30日 07:33:49[boonya](https://me.csdn.net/boonya)阅读数：4233








首先编写一个jaas.config文件，内容视你的*LoginModules而定。



```
simple {     
   com.boonya.demo.business.security.SimpleLoginModule required debug=true;     
};
```




#### 一、错误地认为只要将文件写入属性就可以实现LoginModels的注册



一般地，对于配置属性文件而言并不能实现LoginModules的注册，如下为错误的方式：


`java.security.auth.login.config=C:\\Program Files\\jaas.config`

这样并不能实现目标文件内容的注册。访问的时候会出现这样的错误：



![](https://img-blog.csdn.net/20150330072647925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


#### 二、正确的注入方式是设置System.setProperty(k,v)方法

（1）、在properties文件中配置jaas.config的路径：

`java_security_file_path=C:\\Program Files\\jaas.config`注：创建文件时，必须在一个确定存在的路径下完成。




（2）、创建文件jaas.config文件，并作用户登录验证：


```java
package com.boonya.demo.business.controller;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import javax.security.auth.Subject;
import javax.security.auth.login.LoginContext;
import javax.security.auth.login.LoginException;
import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RequestParam;
import com.boonya.demo.business.security.MyPrivilege;
import com.boonya.demo.business.security.SimpleCallbackHandler;
import com.boonya.demo.utils.ProcessProperties;

@Controller
@RequestMapping("/securityservice")
public class AuthenticationController
{

	/**
	 * 創建JAAS登錄Model
	 * @return
	 */
	private boolean createJAASFile()
	{
		boolean status = false;
		File file = new File(ProcessProperties.getContextProperty("java_security_file_path"));
		if (file.exists())
		{
			status = true;
		} else
		{
			try
			{
				// 創建的文件必須是已經存在的路徑下
				file.createNewFile();
				BufferedWriter output = new BufferedWriter(new FileWriter(file));
				String content = "simple { com.boonya.demo.business.security.SimpleLoginModule required debug=true; };";
				output.write(content);
				output.close();
				status = true;
			} catch (IOException e)
			{
				e.printStackTrace();
			}
		}
		return status;
	}

	@SuppressWarnings("unchecked")
	@RequestMapping(value = "/authenticate", method = RequestMethod.GET)
	public String authentication(@RequestParam("username")String username,@RequestParam("password")String password)
	{
		System.out.println("create and valide file。。。。。。。。。");
		boolean status=createJAASFile();
		System.out.println("create and valide file。。。。。。。。。"+status);
		// 建立登陆上下文，并通过配置文件初始化，在这里配置文件必须与程序同目录
		LoginContext loginContext = null;
		try
		{
			// 創建JAAS文件
			if (!status)
			{
				return "error";
			}
			// 设置JAAS配置文件
			System.setProperty("java.security.auth.login.config", ProcessProperties.getContextProperty("java_security_file_path"));
			loginContext = new LoginContext("simple", new SimpleCallbackHandler(username,password));
		} catch (LoginException e)
		{
			System.out.println(e.getMessage());
		} // 建立登陆上下文，并通过配置文件初始化，在这里配置文件必须与程序同目录

		try
		{
			// 如果不抛出异常表示验证成功
			loginContext.login();
			Subject subject = loginContext.getSubject();
			// subject.d
			Subject.doAs(subject, new MyPrivilege());
		} catch (LoginException e)
		{
			return "error";
		}
		return "welcome";
	}

}
```

**此处使用了错误的属性读取方式：**



```java
package com.boonya.demo.utils;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.beans.factory.config.PropertyPlaceholderConfigurer;

public class ProcessProperties extends PropertyPlaceholderConfigurer
{

	private static Map<String, String> ctxPropertiesMap;

	@Override
	protected void processProperties(ConfigurableListableBeanFactory beanFactoryToProcess, Properties props) throws BeansException
	{
		super.processProperties(beanFactoryToProcess, props);
		ctxPropertiesMap = new HashMap<String, String>();
		for (Object key : props.keySet())
		{
			String keyStr = key.toString();
			String value = props.getProperty(keyStr);
			ctxPropertiesMap.put(keyStr, value);
		}
	}

	public static String getContextProperty(String name)
	{
		return ctxPropertiesMap.get(name);
	}

}
```


最方便的是下面所说的快速注入，请继续往下看。



#### 三、快速的注入参数到System属性上


文件配置：


```
<bean id="propertyConfigurer"  
	    class="com.boonya.demo.utils.ProcessProperties">
	    <property name="ignoreResourceNotFound" value="true" />  
	    <property name="locations">  
	        <list>  
	            <value>classpath:config.properties</value>  
	        </list>  
	    </property>  
	</bean>
```




实现代码：


```java
package com.boonya.demo.utils;

import java.util.HashMap;
import java.util.Map;
import java.util.Properties;
import org.springframework.beans.BeansException;
import org.springframework.beans.factory.config.ConfigurableListableBeanFactory;
import org.springframework.beans.factory.config.PropertyPlaceholderConfigurer;

public class ProcessProperties extends PropertyPlaceholderConfigurer
{

	@Override
	protected void processProperties(ConfigurableListableBeanFactory beanFactoryToProcess, Properties props) throws BeansException
	{
		super.processProperties(beanFactoryToProcess, props);

		for (Object key : props.keySet())
		{
			String keyStr = key.toString();
			String value = props.getProperty(keyStr);
			System.setProperty(keyStr, value);
		}
	}

	public static String getContextProperty(String name)
	{
		return System.getProperty(name);
	}

}
```







