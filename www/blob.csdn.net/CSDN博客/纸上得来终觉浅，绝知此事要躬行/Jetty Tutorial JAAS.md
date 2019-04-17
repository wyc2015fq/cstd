# Jetty Tutorial JAAS - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年01月06日 14:21:13[boonya](https://me.csdn.net/boonya)阅读数：894标签：[java																[jetty																[JAAS																[tomcat																[login](https://so.csdn.net/so/search/s.do?q=login&t=blog)
个人分类：[JAAS](https://blog.csdn.net/boonya/article/category/2818083)





    Jetty 也支持配置JAAS，了解更多见原文地址。

原文地址：[http://wiki.eclipse.org/Jetty/Tutorial/JAAS](http://wiki.eclipse.org/Jetty/Tutorial/JAAS)


- [1Introduction](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Introduction)
- [2Details](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Details)- [2.1Configuring
 a JAASLoginService](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Configuring_a_JAASLoginService)
- [2.2Declaring
 an org.eclipse.jetty.plus.jaas.JAASLoginService](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Declaring_an_org.eclipse.jetty.plus.jaas.JAASLoginService)
- [2.3Creating
 a JAAS login module configuration file](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Creating_a_JAAS_login_module_configuration_file)
- [2.4Specifying
 the JAAS login module configuration file on the Jetty run line](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Specifying_the_JAAS_login_module_configuration_file_on_the_Jetty_run_line)

- [3Fine
 Tuning the JAASLoginService](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Fine_Tuning_the_JAASLoginService)- [3.1Understanding
 the RoleCheckPolicy](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Understanding_the_RoleCheckPolicy)
- [3.2Using
 the CallbackHandler](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Using_the_CallbackHandler)
- [3.3Configuring
 a Role Principal Implementation Class](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Configuring_a_Role_Principal_Implementation_Class)

- [4Examining
 Sample Login Modules](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Examining_Sample_Login_Modules)- [4.1Understanding
 Passwords/Credentials](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Understanding_Passwords.2FCredentials)
- [4.2JDBCLoginModule](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#JDBCLoginModule)
- [4.3DataSourceLoginModule](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#DataSourceLoginModule)
- [4.4PropertyFileLoginModule](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#PropertyFileLoginModule)
- [4.5LdapLoginModule](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#LdapLoginModule)- [4.5.1Configuring
 the Role Principal Implementation Class for the LdapLoginModule[1]](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Configuring_the_Role_Principal_Implementation_Class_for_the_LdapLoginModule.5B1.5D)

- [4.6Writing
 Your Own LoginModule](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Writing_Your_Own_LoginModule)

- [5Examining
 an Example JAAS WebApp](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Examining_an_Example_JAAS_WebApp)
- [6Other
 Goodies](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#Other_Goodies)- [6.1RequestParameterCallback](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#RequestParameterCallback)

- [7References](http://wiki.eclipse.org/Jetty/Tutorial/JAAS#References)
- 详情请见原文，[Tomcat也支持JAAS的配置](https://www.owasp.org/index.php/JAAS_Tomcat_Login_Module)。](https://so.csdn.net/so/search/s.do?q=tomcat&t=blog)](https://so.csdn.net/so/search/s.do?q=JAAS&t=blog)](https://so.csdn.net/so/search/s.do?q=jetty&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




