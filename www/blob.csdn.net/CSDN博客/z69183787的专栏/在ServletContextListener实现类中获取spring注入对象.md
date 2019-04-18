# 在ServletContextListener实现类中获取spring注入对象 - z69183787的专栏 - CSDN博客
2017年12月19日 18:09:13[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1822
由于项目需要，需在ServletContextListener监听接口实现类中调用spring注入的对象,以获取系统初始化参数.代码如下:
[java][view
 plain](http://blog.csdn.net/qbg19881206/article/details/8995253#)[copy](http://blog.csdn.net/qbg19881206/article/details/8995253#)
- import java.io.IOException;  
- import java.util.List;  
- import javax.servlet.ServletContextEvent;  
- import javax.servlet.ServletContextListener;  
- import org.springframework.web.context.WebApplicationContext;  
- import org.springframework.web.context.support.WebApplicationContextUtils;  
- import pams.model.Device;  
- import pams.service.Impl.DeviceManage;  
- import pams.socket.TcpManager;  
- import pams.socket.TcpServer;  
- /**
-  * 系统初始化
-  * @author 恶灵骑士
-  *
-  */
- publicclass SysInitServlet implements ServletContextListener {  
- //获取spring注入的bean对象
- private WebApplicationContext springContext;  
- private DeviceManage deviceManager;  
- 
- //数据采集仪服务线程
-  TcpServer daqServer = null;  
- //继电器服务线程
-  TcpServer realyServer = null;  
- //Tcp连接管理器
-  Thread tcpManager = null;  
- 
- public SysInitServlet(){  
- super();  
-  }  
- /**
-   *应用程序退出时调用
-   */
- @Override
- publicvoid contextDestroyed(ServletContextEvent event) {  
-   serverDestroyed();  
-   System.out.println("应用程序关闭!");  
-  }  
- 
- /**
-   * 应用程序加载时调用
-   */
- @Override
- publicvoid contextInitialized(ServletContextEvent event) {  
-   springContext = WebApplicationContextUtils.getWebApplicationContext(event.getServletContext());  
- if(springContext != null){  
-    deviceManager = (DeviceManage)springContext.getBean("deviceManage");  
-   }else{  
-    System.out.println("获取应用程序上下文失败!");  
- return;  
-   }  
-   System.out.println("初始化系统服务!");  
-   serverInitialized();  
-   System.out.println("TcpServer初始化完成!");  
-  }  
- 
- /**
-   * 系统服务初始化
-   * device_type:由于只需要知道采集仪,继电器端口号
-   * 所以device_type设置为0
-   */
- publicvoid serverInitialized(){  
- if(deviceManager == null){  
-             System.out.println("获取设备管理器失败!");  
- return;  
-       }  
-       List<Device> devices = this.deviceManager.load(0);  
- for(Device device : devices){  
-           System.out.println("设备ID-->"+device.getId());  
-           System.out.println("采集仪端口号-->"+device.getProperty1());  
-           System.out.println("继电器端口号-->"+device.getProperty2());  
-           System.out.println("所属大棚-->"+device.getShed().getName());  
-     }  
- //开启串口服务器-数据采集仪监听线程
-      TcpServer daqServer = new TcpServer(5678);  
-      TcpServer realyServer = new TcpServer(5679);  
-     tcpManager = new Thread(new TcpManager());  
- if(daqServer.getServer() != null)  
-     {  
- new Thread(daqServer).start();  
-     }  
- if(realyServer.getServer()!=null){  
- new Thread(realyServer).start();  
-    }  
-     tcpManager.start();  
-  }  
- 
- /**
-   * 系统服务注销
-   */
- @SuppressWarnings("deprecation")  
- publicvoid serverDestroyed(){  
- 
- if(daqServer!=null){  
- try {  
-     daqServer.getServer().close();  
-    } catch (IOException e) {  
-     System.out.println("数据采集仪服务线程关闭出错 --> "+e.getMessage());  
-    }  
-   }  
- if(realyServer!=null){  
- try {  
-     realyServer.getServer().close();  
-    } catch (IOException e) {  
-     System.out.println("继电器服务线程关闭出错 --> "+e.getMessage());  
-    }  
-   }  
- if(tcpManager!=null){  
-    tcpManager.stop();  
-   }  
-  }  
- }  
springContext为spring管理的应用程序上下文,里面存储spring管理的各种bean对象.deviceManager就是通过spring注入的设备控制业务层.
注意事项：
1.  由于实现的是ServletContextListener接口，故需要实现public void contextInitialized(ServletContextEvent event)方法和public void contextDestroyed(ServletContextEvent event)方法.前者在应用程序加载时调用,里面添加一些初始化业务.如初始化springContext,调用serverInitialized()完成系统服务初始化,后者用于应用程序关闭时调用,主要完成服务资源的注销.
既然是listener接口就要在web.xml中配置相关参数,如下：
<listener>
   <listener-class>pams.servlet.SysInitServlet</listener-class>
  </listener>
2.  由于需获取spring管理bean,故该listner配置需要放在spring监听器配置之后,以使spring完成初始化，如下：
[html][view
 plain](http://blog.csdn.net/qbg19881206/article/details/8995253#)[copy](http://blog.csdn.net/qbg19881206/article/details/8995253#)
- <!--Spring ApplicationContext载入-->
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- <!-- Spring ApplicationContext配置文件的路径,此参数用于后面的Spring-Contextloader -->
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath:beans.xml</param-value>
- </context-param>
- lt;!-- 系统服务初始化 -->
- <listener>
- <listener-class>pams.servlet.SysInitServlet</listener-class>
- </listener>
如果想在外部类中调用通过这种方法得到的spring对象,可以把springContext设为static，然后提供相应的get方法，此处由于不需要故设为private..
