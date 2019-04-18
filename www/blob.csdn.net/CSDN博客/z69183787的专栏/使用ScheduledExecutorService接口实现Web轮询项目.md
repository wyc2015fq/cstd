# 使用ScheduledExecutorService接口实现Web轮询项目 - z69183787的专栏 - CSDN博客
2014年01月14日 21:46:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3222
web.xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE web-app PUBLIC "-//Sun Microsystems, Inc.//DTD Web Application 2.3//EN" "http://java.sun.com/dtd/web-app_2_3.dtd">
<web-app>
	<display-name>Schedule Application</display-name>
	 
	<context-param>
		<param-name>webAppRootKey</param-name>
		<param-value>schedule.root</param-value>
	</context-param>
	 
	 <!-- 生产环境设为0 -->
	<context-param>
		<param-name>log4jRefreshInterval</param-name>
		<param-value>0</param-value>
	</context-param>
	<context-param>
		<param-name>log4jConfigLocation</param-name>
		<param-value>/WEB-INF/classes/log4j.properties</param-value>
	</context-param>
	
	<context-param>
		<param-name>contextConfigLocation</param-name>
		<param-value>/WEB-INF/classes/*Context.xml</param-value>
		<!-- classpath:applicationContext.xml,classpath*:*Context.xml -->
	</context-param>
	
	<servlet>
		<servlet-name>SecheduleServlet</servlet-name>
		<servlet-class>com.wonders.schedule.servlet.ScheduleServlet</servlet-class>
		<load-on-startup>1</load-on-startup>
	</servlet>
	
	<filter>
		<filter-name>struts2</filter-name>
		<description>Struts2 Filter</description>
		<filter-class>
			org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter
		</filter-class>
	</filter>
	<filter>
		<filter-name>struts2-cleanup</filter-name>
		<description>Struts2 CleanUp Filter</description>
		<filter-class>
			org.apache.struts2.dispatcher.ActionContextCleanUp
		</filter-class>
	</filter>
	<filter>
		<filter-name>EncodingFilter</filter-name>
		<filter-class>
			org.springframework.web.filter.CharacterEncodingFilter
		</filter-class>
		<init-param>
			<param-name>encoding</param-name>
			<param-value>utf-8</param-value>
		</init-param>
	</filter>
	<filter-mapping>
		<filter-name>struts2-cleanup</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>struts2</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	<filter-mapping>
		<filter-name>EncodingFilter</filter-name>
		<url-pattern>/*</url-pattern>
	</filter-mapping>
	 <listener>
		<listener-class>org.springframework.web.util.Log4jConfigListener</listener-class>
	</listener>
	
	<listener>
		<listener-class>
			org.springframework.web.context.ContextLoaderListener
		</listener-class>
	</listener>
	<welcome-file-list>
		<welcome-file>login.jsp</welcome-file>
	</welcome-file-list>
	
	<!-- session失效时间 分钟
	<session-config>  
    	<session-timeout>-1</session-timeout>  
	</session-config> 
	 -->
</web-app>
```
servlet：
```java
/**   
*    
* 项目名称：schedule   
* 类名称：ScheduleServlet   
* 类描述：   
* 创建人：zhoushun   
* 创建时间：2012-11-27 下午06:45:17   
* 修改人：zhoushun   
* 修改时间：2012-11-27 下午06:45:17   
* 修改备注：   
* @version    
*    
*/
package com.wonders.schedule.servlet;
import java.io.IOException;
import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import com.wonders.schedule.common.util.SimpleLogger;
import com.wonders.schedule.main.ScheduleMain;
/** 
 * @ClassName: ScheduleServlet 
 * @Description: TODO(计划任务) 
 * @author zhoushun
 * @date 2012-11-27 下午06:45:17 
 *  
 */
public class ScheduleServlet extends HttpServlet {
	SimpleLogger log = new SimpleLogger(this.getClass());
	/**
	 * 
	 */
	private static final long serialVersionUID = -1600327747212116607L;
	@Override
	protected void doPost(HttpServletRequest req, HttpServletResponse resp)
			throws ServletException, IOException {
		// TODO Auto-generated method stub
		super.doPost(req, resp);
	}
	@Override
	public void destroy() {
		// TODO Auto-generated method stub
		super.destroy();
	}
	
	
	/** 
	* @Title: init 
	* @Description: TODO(这里用一句话描述这个方法的作用) 
	* @param @throws ServletException    设定文件 
	* @throws 
	*/
	@Override
	
	public void init() throws ServletException {
		// TODO Auto-generated method stub
		super.init();
		log.debug("计划任务启动");
		ScheduleMain.start();	
	}
}
```
main：
```java
/**
 * 
 */
package com.wonders.schedule.main;
import java.util.List;
import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;
import org.apache.log4j.Logger;
import com.wonders.schedule.common.util.SimpleLogger;
import com.wonders.schedule.manage.ScheduleTask;
import com.wonders.schedule.model.bo.TScheduleConfig;
import com.wonders.schedule.service.ScheduleService;
import com.wonders.schedule.util.TimeUtil;
/**
 * @ClassName: ScheduleMain
 * @Description: TODO(这里用一句话描述这个类的作用)
 * @author zhoushun
 * @date 2012-12-4 上午10:26:48
 * 
 */
public class ScheduleMain {
	static Logger log = SimpleLogger.getLogger(ScheduleMain.class);
	private ScheduleMain() {
	}
	/**
	 * 定时执行
	 */
	private static ScheduledExecutorService scheduler = null;
	/**
	 * @Title: addTask
	 * @Description: TODO(增加计划任务)
	 * @param @param config 设定文件
	 * @return void 返回类型
	 * @throws
	 */
	public static void addTask(TScheduleConfig t) {
		long delay = TimeUtil.getDelay(t.getTime(), t.getInterval());
		long period = TimeUtil.getIntervalSec(t.getInterval());
		log.debug(t.getName() + " 模块正在启动" + " 延迟为：" + delay + " 周期为：" + period);
		ScheduleTask task = new ScheduleTask(t);
		//若之前执行失败，强制执行一次
		if(!TimeUtil.isExec(t.getLastTime(), t.getTime(), t.getInterval())){
			scheduler.schedule(task,0,TimeUnit.SECONDS);
		}
		scheduler.scheduleAtFixedRate(task, delay, period,
				TimeUnit.SECONDS);
	}
	/**
	 * @Title: start
	 * @Description: TODO(计划任务启动)
	 * @param 设定文件
	 * @return void 返回类型
	 * @throws
	 */
	public static void start() {
		if (scheduler == null) {
			scheduler = Executors.newScheduledThreadPool(20);
			synchronized (scheduler) {
				List<TScheduleConfig> list = ScheduleService.getSvcConfig();
				for (TScheduleConfig t : list) {
					addTask(t);
				}
			}
		} else {
			log.warn("定时器正在运行!");
		}
	}
	/**
	 * @Title: stop
	 * @Description: TODO(停止)
	 * @param 设定文件
	 * @return void 返回类型
	 * @throws
	 */
	public static void stop() {
		synchronized (scheduler) {
			if (scheduler != null && !scheduler.isShutdown()) {
				scheduler.shutdown();
				scheduler = null;
				log.debug("关闭定时器!");
			}
		}
	}
	/**
	 * @Title: restart
	 * @Description: TODO(重启)
	 * @param 设定文件
	 * @return void 返回类型
	 * @throws
	 */
	public static void restart() {
		stop();
		start();
	}
	/**
	 * @Title: trigger
	 * @Description: TODO(手动触发 用于交互)
	 * @param 设定文件
	 * @return void 返回类型
	 * @throws
	 */
	public static void trigger() {
	}
}
```
task：需要继承runnable接口
```java
/**
 * 
 */
package com.wonders.schedule.manage;
import java.util.concurrent.Semaphore;
import com.wonders.schedule.model.bo.TScheduleConfig;
/**
 * @ClassName: ScheduleTask
 * @Description: TODO(这里用一句话描述这个类的作用)
 * @author zhoushun
 * @date 2012-12-4 下午02:52:19
 * 
 */
public class ScheduleTask implements Runnable {
	private TScheduleConfig t;
	/**
	 * 同步信号量
	 */
	private final Semaphore semaphore = new Semaphore(1);
	public ScheduleTask(TScheduleConfig t) {
		this.t = t;
	}
	/**
	 * @Title: run
	 * @Description: TODO(这里用一句话描述这个方法的作用)
	 * @param 设定文件
	 * @throws
	 */
	@Override
	public void run() {
		try {
			semaphore.acquire();
			ScheduleManager.operate(t);
		} catch (InterruptedException e) {
			e.printStackTrace();
		} finally {
			semaphore.release();
		}
	}
}
```
