# Spring 3.X + Quartz2.X 定时任务 - z69183787的专栏 - CSDN博客
2016年10月13日 16:17:23[OkidoGreen](https://me.csdn.net/z69183787)阅读数：895
xml：每周二晚上23:50分执行
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN//EN" 
"http://www.springframework.org/dtd/spring-beans.dtd">
<beans>
	 <!-- 启动触发器的配置开始 -->  
    <bean name="startQuertz" lazy-init="false" autowire="no"  
        class="org.springframework.scheduling.quartz.SchedulerFactoryBean">  
        <property name="triggers">  
            <list>  
                <ref bean="myJobTrigger" />  
                <ref bean="myJobTrigger2" />  
            </list>  
        </property>  
    </bean>  
    <!-- 启动触发器的配置结束 -->  
  
    <!-- 调度的配置开始 -->  
    <!--  
        quartz-1.8以前的配置   
    <bean id="myJobTrigger"  
        class="org.springframework.scheduling.quartz.CronTriggerBean">  
        <property name="jobDetail">  
            <ref bean="myJobDetail" />  
        </property>  
        <property name="cronExpression">  
            <value>0/1 * * * * ?</value>  
        </property>  
    </bean>  
    -->  
	<!-- quartz-2.x的配置  
    <bean id="myJobTrigger"  
        class="org.springframework.scheduling.quartz.CronTriggerFactoryBean">  
        <property name="jobDetail">  
            <ref bean="myJobDetail" />  
        </property>  
        <property name="cronExpression">  
            <value>0 29 16 ? * TUE</value>  
        </property>  
    </bean>  
    --> 
       <bean id="myJobTrigger"  
        class="org.springframework.scheduling.quartz.CronTriggerFactoryBean">  
        <property name="jobDetail">  
            <ref bean="myJobDetail" />  
        </property>  
        <property name="cronExpression">  
            <value>0 50 23 ? * MON</value>  
        </property>  
    </bean>  
    
     <bean id="myJobTrigger2"  
        class="org.springframework.scheduling.quartz.CronTriggerFactoryBean">  
        <property name="jobDetail">  
            <ref bean="myJobDetail" />  
        </property>  
        <property name="cronExpression">  
            <value>0 00 18 ? * MON</value>  
        </property>  
    </bean>  
    <!-- 调度的配置结束 -->  
	  <!-- job的配置开始 -->  
    <bean id="myJobDetail"  
        class="org.springframework.scheduling.quartz.MethodInvokingJobDetailFactoryBean">  
        <property name="targetObject">  
            <ref bean="coccMain" />  
        </property>  
        <property name="targetMethod">  
            <value>work</value>  
        </property>  
    </bean>  
    <!-- job的配置结束 -->  
</beans>
```
```java
/**
 * 
 */
package com.wonders.quartz.cocc.main;
import java.io.StringWriter;
import java.util.List;
import javax.xml.bind.JAXBContext;
import javax.xml.bind.Marshaller;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.context.ApplicationContext;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;
import com.wonders.quartz.cocc.model.vo.CoccListVo;
import com.wonders.quartz.cocc.model.vo.CoccReportVo;
import com.wonders.quartz.cocc.model.xml.CoccReportXml;
import com.wonders.quartz.cocc.service.CoccReportService;
import com.wonders.schedule.util.SpringBeanUtil;
import com.wonders.service.util.ServiceClient;
import com.wonders.task.sample.ITaskService;
/** 
 * @ClassName: CoccMain 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013年12月10日 下午2:17:14 
 *  
 */
@Transactional(value = "stfb-txManager",propagation=Propagation.REQUIRED,rollbackFor=Exception.class)
@Service("coccMain")
public class CoccMain {
	final public static String userName="eam";
	final public static String pwd="eam2013!";
	
	private CoccReportService service;
	
	public CoccReportService getService() {
		return service;
	}
	@Autowired(required=false)
	public void setService(@Qualifier("coccReportService")CoccReportService service) {
		this.service = service;
	}
	public void work(){
		String today = new java.text.SimpleDateFormat("yyyy-MM-dd").format(new java.util.Date());
		//String today=  "2014-01-13";
		String sql = "select * from (select t.id,t.title,t.source,"
				+ " to_char(t.create_time,'yyyy-mm-dd') as createTime,"
				+ " to_char(t.pub_date,'yyyy-mm-dd') as publishTime, "
				+ " 'http://10.1.44.18/stfb'||t.url||'/con'||t.identified_no||'.htm' as url"
				+ " from tb_content t where t.del_flag=0 and t.sj_id=1254 ) where "
				+ " createTime = '"+today+"'";
		List<CoccReportVo> list = this.service.getResult(sql);
		CoccReportXml xml = new CoccReportXml();
		CoccListVo v = new CoccListVo();
		v.list = list ;
		xml.list = v;
		String result = "";
		try{
			StringWriter writer = new StringWriter();
			JAXBContext context = JAXBContext.newInstance(CoccReportXml.class);
			Marshaller m = context.createMarshaller();
			m.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true); 
			m.setProperty(Marshaller.JAXB_ENCODING, "GBK"); //防止文件中文乱码
			m.marshal(xml, writer);
			result = writer.toString();
		}catch(Exception e){
			result = "none";
		}
		System.out.println(result);
		
		//ServiceClient.setDataInfo(userName,pwd,result);
	} 
	
	public static void main(String[] args){
		ApplicationContext applicationContext = null;  
		String[] fileUrl = new String[]{"classpath*:*Context*.xml"};  
		applicationContext = new ClassPathXmlApplicationContext(fileUrl);  
		CoccMain coccMain = (CoccMain) SpringBeanUtil.getBean("coccMain");
		coccMain.work();
		
	}
}
```
"0 0 12 * * ?" 每天中午12点触发
"0 15 10 ? * *" 每天上午10:15触发
"0 15 10 * * ?" 每天上午10:15触发
"0 15 10 * * ? *" 每天上午10:15触发
"0 15 10 * * ? 2005" 2005年的每天上午10:15触发
"0 * 14 * * ?" 在每天下午2点到下午2:59期间的每1分钟触发
"0 0/5 14 * * ?" 在每天下午2点到下午2:55期间的每5分钟触发
"0 0/5 14,18 * * ?" 在每天下午2点到2:55期间和下午6点到6:55期间的每5分钟触发
"0 0-5 14 * * ?" 在每天下午2点到下午2:05期间的每1分钟触发
"0 10,44 14 ? 3 WED" 每年三月的星期三的下午2:10和2:44触发
"0 15 10 ? * MON-FRI" 周一至周五的上午10:15触发
"0 15 10 15 * ?" 每月15日上午10:15触发
"0 15 10 L * ?" 每月最后一日的上午10:15触发
"0 15 10 ? * 6L" 每月的最后一个星期五上午10:15触发 
"0 15 10 ? * 6L 2002-2005" 2002年至2005年的每月的最后一个星期五上午10:15触发
"0 15 10 ? * 6#3" 每月的第三个星期五上午10:15触发
